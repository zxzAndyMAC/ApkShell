// e32base.h
//
// Created 2005-04-08, ER1C
//

#ifndef __BASE_H__
#define __BASE_H__

#include "Base/e32std.h"

class CBase
{
public:
	IMPORT_C virtual ~CBase() {}
	IMPORT_C inline void* operator new(size_t size)
	{
		void* p = malloc(size);
		if (p)	{ memset(p, 0, size); }
		return p;
	}
	IMPORT_C inline void* operator new(size_t size, TLeave)
	{
		void* p = malloc(size);
		if (p)	{ memset(p, 0, size); }
		else	{ User::Leave(KErrNoMemory); }
		return p;
	}
	IMPORT_C inline void operator delete(void* p) { if (p!=NULL) free(p); }
	IMPORT_C inline void operator delete(void* p, TLeave) { if (p!=NULL) free(p); }
protected:
	CBase() {}
};

template <class T>
class CArrayPtrFlat : public CBase
{
public:
	IMPORT_C ~CArrayPtrFlat() {free(iData); iData=NULL;}
	IMPORT_C CArrayPtrFlat(TInt aGranularity) : iCount(0),iGranularity(aGranularity),iData(NULL) {}
	IMPORT_C TInt Count() const {return iCount;}
	IMPORT_C T* At(TInt aIndex) const {return (T*)iData[aIndex];}
	IMPORT_C const T* operator[](TInt aIndex) const {return (T*)iData[aIndex];}
	IMPORT_C T* operator[](TInt aIndex) {return (T*)iData[aIndex];}
	IMPORT_C void Replace(TInt aIndex, T* aPtr) {iData[aIndex]=(TInt)aPtr;}
	IMPORT_C void Delete(TInt aIndex)
	{
		if (aIndex >= iCount || aIndex<0) { return; }
		if (aIndex != (iCount-1))
		{
			Mem::Copy(iData+aIndex, iData+aIndex+1, (iCount-aIndex-1)*sizeof(TInt));
		}
		iGranularity--;
		iCount--;
		iData = (TInt*)realloc(iData, sizeof(TInt)*iCount);
	}
	IMPORT_C void AppendL(T* aRef)
	{
		if (iGranularity == iCount) { iGranularity++; }
		if (iData)	{ iData = (TInt*)realloc(iData, sizeof(TInt)*iGranularity); }
		else		{ iData = (TInt*)malloc(sizeof(TInt)*iGranularity); }
		iData[iCount++] = (TInt)aRef;
	}
	IMPORT_C void InsertL(TInt aIndex, T* aRef)		// to be test
	{
		if (aIndex > iCount || aIndex < 0) { return; }
		if (iGranularity == iCount) { iGranularity++; }
		if (iData)	{ iData = (TInt*)realloc(iData, sizeof(TInt)*iGranularity); }
		else		{ iData = (TInt*)malloc(sizeof(TInt)*iGranularity); }
		Mem::Copy(iData+aIndex+1, iData+aIndex, (iCount-aIndex)*sizeof(TInt));
		iData[aIndex] = (TInt)aRef;
		iCount ++;
	}
	IMPORT_C void ResetAndDestroy()
	{
		for (int i=0; i<iCount; i++) { delete At(i); }
		iCount = 0;
	}
	IMPORT_C void Reset() {iCount=0; Compress();}
	IMPORT_C void Compress() {iData = (TInt*)realloc(iData, sizeof(TInt)*iCount);}
private:
	TInt iCount;
	TInt iGranularity;
	TInt iLength;
	TInt* iData;
};

#endif	//__BASE_H__
