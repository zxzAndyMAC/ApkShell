//
// Created: 2007.12.13, ER1C
//

#ifndef __DEX_XFILE_H__
#define __DEX_XFILE_H__

#include <stdio.h>
#include <stdlib.h>
#include "Base/e32def.h"
#include "Base/e32base.h"

#define PAK_FN_MAX	80

typedef struct TPakHeader
{
	TUint16 fileNumber;	// how many files packed in the .pak
	TUint16 fileNameLen;	// eg. 256
	int fileNamePos;	// fileNamePos | 0x80000000(fileName compressed or not)
	int fnSize;	// zFNSize or oFNSize, oFNSize = fileNameLen*fileNumber
} TPakHeader;

typedef struct TPakIndex
{
	TUint32 nHash1;
	TUint32 nHash2;
	TUint32 filePos;
	TUint32 oSize;
	TUint32 zSize;
	TUint32 flag;	// reserved
} TPakIndex;

struct TFileBlock
{
	TPakIndex index;
	TUint8* oData;
	int i;

	TFileBlock() : oData(NULL) {}
	void Read(void* buf, int len);
	~TFileBlock();
};

class CPakReader : public CBase
{
public:
	static CPakReader* Create(const char* pakFile, bool inMem = false);
	static CPakReader* Create(TUint8* oData, int oSize);
	~CPakReader();
	void FreeFileStream();
	int GetBlock(const char* fn, TFileBlock& fb);
	int GetBlock(int index, TFileBlock& fb);

	static TUint32 HashStr(const char* str, TUint32 hashType, const TUint32* cryptTable);

	int GetNum() const {return iHeader.fileNumber;}
	char*  GetName(){return sName;}

private:
	CPakReader(TUint8* oData, int oSize);
	void InitCryptTable();

private:
	TPakHeader	iHeader;
	TPakIndex*	iPakIndex;
	TUint8*		iFileStream;
	int			iFileStreamSize;
	char		sFullPath[256];
	char		sFileName[100];
	char		sName[32];

public:
	TUint32*	iCryptTable;
};

#endif	// __DEX_XFILE_H__
