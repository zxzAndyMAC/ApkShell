

#include "CPakReader.h"
#include "BootFileUtils.h"

#include <string.h>
#include <zlib.h>
#include "DexMarcoDef.h"

CPakReader* CPakReader::Create(const char* pakFile, bool inMem)
{
	CPakReader* self = NULL;

//	Data tData = FileUtils::getInstance()->getDataFromFile(pakFile);
	ssize_t size;
	unsigned char* filedata = BootFileUtils::getInstance()->getFileData(pakFile, "rb", &size);
//	ssize_t size = tData.getSize();
	if (size == 0)
	{
	//	std::string fullPath = FileUtils::getInstance()->fullPathForFilename(pakFile);
		LOGD("CPakReader::Create -> can't open file: %s", pakFile);
	}
	else
	{
		TUint8* data = (TUint8*)malloc(size);
		memcpy(data, filedata, size);
		self = new CPakReader(data, size);
		if (self->iHeader.fileNameLen != 80)
		{
			free(data);
			delete self;
			self = NULL;
		}
		else
		{
			if(inMem)
			{
				self->iFileStream = data;
			}
			else
			{
				self->iFileStream = NULL;
				free(data);
			}
			strcpy(self->sFileName, pakFile);
			strcpy(self->sFullPath, pakFile);
			int len = strlen(pakFile);
			int s = len;
			for (int i=len-1; i>0; i--)
			{		
				if (pakFile[i] == '.')				
				{
					s = i;
					break;
				}
			}
			strncpy(self->sName, pakFile, s);
            self->sName[s] = '\0';
		}
	}
	
	return self;
}

CPakReader* CPakReader::Create(TUint8* oData, int oSize)
{
	CPakReader* self = new CPakReader(oData, oSize);
	if (self->iHeader.fileNameLen != 80)
	{
		delete self;
		self = NULL;
	}
	return self;
}

CPakReader::CPakReader(TUint8* oData, int oSize)
:iFileStreamSize(oSize)
{
	TUlong pos = 0;
	TUlong size = sizeof(TPakHeader);
	memcpy(&iHeader, oData+pos, size);
	pos += size;

	int idxSize = iHeader.fileNumber*sizeof(TPakIndex);
	if (iHeader.fileNameLen != 80 || idxSize+sizeof(iHeader) > oSize)
	{
		iHeader.fileNameLen = 0;
	}
	else
	{
		iPakIndex = (TPakIndex*)malloc(idxSize);
		memcpy(iPakIndex, oData+pos, idxSize);

		InitCryptTable();
	}
}

CPakReader::~CPakReader()
{
	if (iCryptTable)
	{
		free(iCryptTable);
	}
	if (iPakIndex)
	{
		free(iPakIndex);
	}
	if (iFileStream)
	{
		free(iFileStream);
	}
}

int CPakReader::GetBlock(const char* fn, TFileBlock& fb)
{
	TUint32 nHash1 = HashStr(fn, 1, iCryptTable);
	TUint32 nHash2 = HashStr(fn, 2, iCryptTable);
	
	for (int i=0; i<iHeader.fileNumber; i++)
	{
		if (iPakIndex[i].nHash1==nHash1 && iPakIndex[i].nHash2==nHash2)
		{
			return GetBlock(i, fb);
		}
	}
	return KErrNotFound;
}

int CPakReader::GetBlock(int index, TFileBlock& fb)
{
	if (index<0 || index>=iHeader.fileNumber)
	{
		return KErrArgument;
	}
	memset(&fb, 0, sizeof(fb));
	memcpy(&fb.index, &iPakIndex[index], sizeof(TPakIndex));
	ssize_t size = 0;
	TUint8* data = iFileStream;
	if(!iFileStream)
	{
	//	Data tData = FileUtils::getInstance()->getDataFromFile(sFullPath);
		ssize_t size;
		unsigned char* filedata = BootFileUtils::getInstance()->getFileData(sFullPath, "rb", &size);
	//	size = tData.getSize();
		data = (TUint8*)malloc(size);
		memcpy(data, filedata, size);
	}
	TUint8* zData = (TUint8*)malloc(fb.index.zSize);
	fb.oData = (TUint8*)malloc(fb.index.oSize);
	memcpy(zData, data+fb.index.filePos, fb.index.zSize);
    unsigned long destlen = fb.index.oSize;
	uncompress(fb.oData, &destlen, zData, fb.index.zSize);
	free(zData);
	if(!iFileStream)
		free(data);

	return KErrNone;
}

//----------------------------------------------------------------------------
// Hash str
//----------------------------------------------------------------------------

void CPakReader::InitCryptTable()
{
	TUint32 seed = 0x00100001, index1 = 0, index2 = 0, i;

	iCryptTable = (TUint32*)malloc(sizeof(TUint32)*0x500);
	
	for(index1 = 0; index1 < 0x100; index1++)
	{
		for(index2 = index1, i = 0; i < 5; i++, index2 += 0x100)
		{
			TUint32 temp1, temp2;
			
			seed = (seed * 125 + 3) % 0x2AAAAB;
			temp1 = (seed & 0xFFFF) << 0x10;
			
			seed = (seed * 125 + 3) % 0x2AAAAB;
			temp2 = (seed & 0xFFFF);

			iCryptTable[index2] = (temp1 | temp2);
		}
	}
}

int CharUpper(char lc)
{
	if (lc>='a' && lc<='z') return lc+'A'-'a';
	return lc;
}

TUint32 CPakReader::HashStr(const char* str, TUint32 hashType, const TUint32* cryptTable)
{
	TUint32 seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;
	while (*str)
	{
		int uc = CharUpper(*str++);
		int index = (hashType << 8) + uc;
		seed1 = cryptTable[index] ^ (seed1 + seed2);
		seed2 = uc + seed1 + seed2 + (seed2 << 5) + 3;
	}
	return seed1;
}

void CPakReader::FreeFileStream()
{
	if (iFileStream)
	{
		free(iFileStream);
		iFileStream = NULL;
	}
}

//----------------------------------------------------------------------------
// TFileBlock
//----------------------------------------------------------------------------
TFileBlock::~TFileBlock()
{
	if (NULL != oData)
	{
		free(oData);
		oData = NULL;
	}
}

void TFileBlock::Read(void* buf, int len)
{
	if (((TUint32)len+i) > index.oSize)
	{
		len = index.oSize-i;
	}
	if (len > 0)
	{
		memcpy(buf, oData+i, len);
		i += len;
	}
}

// end of file
