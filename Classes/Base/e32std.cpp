#include "Base/w32std.h"
#include <string.h>
#include <math.h>
	
#ifdef ANDROID
#include <unistd.h>
#elif defined(CC_TARGET_OS_IPHONE)
#include <unistd.h>
#else	
#include <windows.h>
#endif


TReal e32sqrt(TReal va)
{
    return sqrt(va);
}

//----------------------------------------------------------------------------
// TSize
//----------------------------------------------------------------------------
TInt TSize::operator==(const TSize& aSize) const
{
	return (aSize.iWidth==this->iWidth && aSize.iHeight==this->iHeight)
		? 1 : 0;
}

TInt TSize::operator!=(const TSize& aSize) const
{
	return (aSize.iWidth==this->iWidth && aSize.iHeight==this->iHeight)
		? 0 : 1;
}

//----------------------------------------------------------------------------
// TRect
//----------------------------------------------------------------------------
TRect::TRect()
{
	iTl.SetXY(0,0);
	iBr.SetXY(0,0);
}

TRect::TRect(const TPoint& aPoint,const TSize& aSize)
{
	SetRect(aPoint, aSize);
}

TRect::TRect(const TPoint& aTl, const TPoint& aBr)
:	iTl(aTl), iBr(aBr)
{
}

void TRect::SetRect(const TPoint& aPointTL,const TPoint& aPointBR)
{
	iTl = aPointTL;
	iBr = aPointBR;
}

void TRect::SetRect(const TPoint& aPoint,const TSize& aSize)
{
	iTl.SetXY(aPoint.iX, aPoint.iY);
	iBr.SetXY(aPoint.iX+aSize.iWidth, aPoint.iY+aSize.iHeight);
}

void TRect::Shrink(TInt aDx,TInt aDy)
{
	int dx = (Width()/2 < aDx) ? Width()/2 : aDx;
	int dy = (Height()/2 < aDy) ? Height()/2 : aDy;
	iTl.iX += dx;
	iTl.iY += dy;
	iBr.iX -= dx;
	iBr.iY -= dy;
}

void TRect::Grow(TInt aDx,TInt aDy)
{
	iTl.iX -= aDx;
	iTl.iY -= aDy;
	iBr.iX += aDx;
	iBr.iY += aDy;
}

void TRect::Move(TInt aDx,TInt aDy)
{
	iTl.iX += aDx;
	iTl.iY += aDy;
	iBr.iX += aDx;
	iBr.iY += aDy;
}

TBool TRect::Intersects(const TRect& aRect) const
{
	return  aRect.iTl.iX <= iBr.iX &&
			aRect.iTl.iY <= iBr.iY &&
			iTl.iX <= aRect.iBr.iX &&
			iTl.iY <= aRect.iBr.iY;
}

TBool TRect::Contains(const TPoint& aPoint) const
{
	return aPoint.iX >= iTl.iX && aPoint.iY >= iTl.iY &&
		aPoint.iX <= iBr.iX && aPoint.iY <= iBr.iY;
}

TSize TRect::Size() const
{
	return TSize(Width(), Height());
}

TInt TRect::Width() const
{
	return iBr.iX-iTl.iX;
}

TInt TRect::Height() const
{
	return iBr.iY-iTl.iY;
}

//----------------------------------------------------------------------------
// Mem
//----------------------------------------------------------------------------
void Mem::Copy(void* dest, const void* src, int size)
{
	/*
	UCHAR* _dest = (UCHAR*)dest;
	UCHAR* _src  = (UCHAR*)src;
	for (int i=0; i<size; i++)
	{
		*(_dest++) = *(_src++);
	}
	*/
	memcpy(dest, src, size);
}

void* Mem::Remove(void* Dest, const void* Src, int Count)
{
	return memmove( Dest, Src, Count );
}

void Mem::Swap(void* Ptr1, void* Ptr2 ,int size)
{
	void* Temp = User::AllocL(size);
	Copy( Temp, Ptr1, size );
	Copy( Ptr1, Ptr2, size );
	Copy( Ptr2, Temp, size );
}

void Mem::FillZ(void* ptr, int size)
{
	/*
	UCHAR* _ptr = (UCHAR*)ptr;
	for (int i=0; i<size; i++)
	{
		*(_ptr++) = 0;
	}
	*/
	memset(ptr, 0, size);
}

void Mem::Fill(void* ptr, int size, TUint8 stuff)
{
	/*
	UCHAR* _ptr = (UCHAR*)ptr;
	for (int i=0; i<size; i++)
	{
		*(_ptr++) = stuff;
	}
	*/
	memset(ptr, stuff, size);
}

int User::TickCount()
{
//17:33 2011/3/7 by dean	
//从操作系统启动到现在所经过（elapsed）的毫秒数，它的返回值是DWORD
#ifdef ANDROID
	struct timeval current;
	gettimeofday(&current, NULL);
	TInt32 currentTime = current.tv_sec * 1000 + current.tv_usec/1000;

	return currentTime*64/1000;
#elif defined(CC_TARGET_OS_IPHONE)
    return TickCount()*64/1000;
#else	
	return GetTickCount()*64/1000;
#endif	
}

TAny* User::AllocL(TInt aSize)
{
	return (TAny*)malloc(aSize);
}

TAny* User::RllocL(void* InPtr, TInt NewSize )
{	
	return (TAny*)realloc(InPtr,NewSize);
}

void User::Free(TAny* aPtr)
{
	free(aPtr);
}

void User::After(TTimeIntervalMicroSeconds32 aInterval)
{
//17:40 2011/3/7 by dean	
#ifdef ANDROID 
	sleep(aInterval/1000);
#elif defined(CC_TARGET_OS_IPHONE)
    sleep(aInterval/1000);
#else
	Sleep(aInterval/1000);	
#endif
}

//void* operator new(TUint aSize, TLeave)
//{
//	return malloc(aSize);
//}

void DllMemFree(void* ptr)
{
	delete[] ptr;
}

//-----------------------------------------------------------------------------
// RArrayBase & RArray
//-----------------------------------------------------------------------------

RArrayBase::RArrayBase(TInt aEntrySize)
:	iCount(0),
	iEntries(NULL),
	iEntrySize(aEntrySize),
	iKeyOffset(0),
	iAllocated(0),
	iGranularity(8)
{
}

void RArrayBase::Close()
{
	free(iEntries);
	iEntries = NULL;
}

TInt RArrayBase::Count() const
{
	return iCount;
}

TAny* RArrayBase::At(TInt aIndex) const
{
	return (TAny*)(((TUint8*)iEntries)+(iEntrySize*aIndex));
}

TInt RArrayBase::Append(const TAny* aEntry)
{
	if (iEntries == NULL)
	{
		iEntries = User::AllocL(iEntrySize);
	}
	else
	{
		iEntries = realloc(iEntries, iAllocated+iEntrySize);
	}
	if (iEntries == NULL) return KErrNoMemory;
	Mem::Copy(((TUint8*)iEntries)+iAllocated, aEntry, iEntrySize);
	iAllocated += iEntrySize;
	iCount ++;
	return KErrNone;
}

TInt RArrayBase::Insert(const TAny* aEntry, TInt aPos)
{
	if (aPos > iCount || aPos < 0)
	{
		return KErrGeneral;
	}
//	if (iGranularity == iCount) { iGranularity++; }
	iCount ++;
	if (iEntries)
	{
		iEntries = realloc(iEntries, iAllocated+iEntrySize);
	}
	else
	{
		iEntries = malloc(iAllocated+iEntrySize);
	}
	if (iEntries == NULL) return KErrNoMemory;
	iAllocated += iEntrySize;
	Mem::Copy(At(aPos+1), At(aPos), (iCount-aPos-1)*iEntrySize);
	Mem::Copy(At(aPos), aEntry, iEntrySize);
	return KErrNone;
}

void RArrayBase::Remove(TInt aIndex)
{
	if (aIndex >= iCount || aIndex<0) { return; }
	if (aIndex != (iCount-1))
	{
		Mem::Copy(At(aIndex), At(aIndex+1), (iCount-aIndex-1)*iEntrySize);
	}
//	iGranularity--;
	iCount--;
	iAllocated -= iEntrySize;
	iEntries = realloc(iEntries, iAllocated);
}

void RArrayBase::Reset()
{
	Close();
	iCount = 0;
	iKeyOffset = 0;
	iAllocated = 0;
}

// end of file `w32std.cpp"
