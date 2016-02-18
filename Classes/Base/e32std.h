// e32std.h
//
// Created 2005-04-08, ER1C
//

#ifndef __STD_H__
#define __STD_H__

#include "Base/e32def.h"

const TInt KMaxFileName=0x100;

// from E32STD.H
const TInt KErrNone=0;
const TInt KErrNotFound=(-1); // Must remain set to -1
const TInt KErrGeneral=(-2);
const TInt KErrCancel=(-3);
const TInt KErrNoMemory=(-4);
const TInt KErrNotSupported=(-5);
const TInt KErrArgument=(-6);
const TInt KErrTotalLossOfPrecision=(-7);
const TInt KErrBadHandle=(-8);
const TInt KErrOverflow=(-9);
const TInt KErrUnderflow=(-10);
const TInt KErrAlreadyExists=(-11);
const TInt KErrPathNotFound=(-12);
const TInt KErrDied=(-13);
const TInt KErrInUse=(-14);
const TInt KErrServerTerminated=(-15);
const TInt KErrServerBusy=(-16);
const TInt KErrCompletion=(-17);
const TInt KErrNotReady=(-18);
const TInt KErrUnknown=(-19);
const TInt KErrCorrupt=(-20);
const TInt KErrAccessDenied=(-21);
const TInt KErrLocked=(-22);
const TInt KErrWrite=(-23);
const TInt KErrDisMounted=(-24);
const TInt KErrEof=(-25);
const TInt KErrDiskFull=(-26);
const TInt KErrBadDriver=(-27);
const TInt KErrBadName=(-28);
const TInt KErrCommsLineFail=(-29);
const TInt KErrCommsFrame=(-30);
const TInt KErrCommsOverrun=(-31);
const TInt KErrCommsParity=(-32);
const TInt KErrTimedOut=(-33);
const TInt KErrCouldNotConnect=(-34);
const TInt KErrCouldNotDisconnect=(-35);
const TInt KErrDisconnected=(-36);
const TInt KErrBadLibraryEntryPoint=(-37);
const TInt KErrBadDescriptor=(-38);
const TInt KErrAbort=(-39);
const TInt KErrTooBig=(-40);
const TInt KErrDivideByZero=(-41);
const TInt KErrBadPower=(-42);
const TInt KErrDirFull=(-43);
const TInt KErrHardwareNotAvailable=(-44);
const TInt KErrInvalidType=(-45);	// Added 2005.08.22
const TInt KErrCreateThreadFailed=(-46);	// ditto

const TInt KMaxTInt=0x7fffffff;

enum TFalse {EFalse=FALSE};
enum TTrue {ETrue=TRUE};

enum TLeave {ELeave};

//#pragma warning(disable:4291)
//extern IMPORT_C inline void* operator new(TUint aSize, TLeave);

TReal e32sqrt(TReal va);

//----------------------------------------------------------------------------
// TPoint
//----------------------------------------------------------------------------
class TPoint
{
public:
	inline TPoint() {iX=0; iY=0;}
	inline TPoint(TInt aX,TInt aY) {SetXY(aX,aY);}
	IMPORT_C void SetXY(TInt aX,TInt aY) {iX=aX; iY=aY;}
public:
	TInt iX;
	TInt iY;
};

//----------------------------------------------------------------------------
// TSize
//----------------------------------------------------------------------------
class TSize
{
public:
	TSize() {iWidth=0; iHeight=0;}
	TSize(TInt aWidth,TInt aHeight) {iWidth=aWidth; iHeight=aHeight;}
	IMPORT_C TInt operator==(const TSize& aSize) const;
	IMPORT_C TInt operator!=(const TSize& aSize) const;
	IMPORT_C void SetSize(TInt aWidth,TInt aHeight) {iWidth=aWidth; iHeight=aHeight;}
public:
	TInt iWidth;
	TInt iHeight;
};

//----------------------------------------------------------------------------
// TRect
//----------------------------------------------------------------------------
class TRect
{
public:
	IMPORT_C TRect();
	IMPORT_C TRect(const TPoint& aPoint,const TSize& aSize);
	IMPORT_C TRect(const TPoint& aTl, const TPoint& aBr);
	IMPORT_C void SetRect(const TPoint& aPointTL,const TPoint& aPointBR);
	IMPORT_C void SetRect(const TPoint& aPoint,const TSize& aSize);
	IMPORT_C void Shrink(TInt aDx,TInt aDy);
	IMPORT_C void Grow(TInt aDx,TInt aDy);
	IMPORT_C void Move(TInt aDx,TInt aDy);
	IMPORT_C TBool Intersects(const TRect& aRect) const;
	IMPORT_C TBool Contains(const TPoint& aPoint) const;
	IMPORT_C TSize Size() const;
	IMPORT_C TInt Width() const;
	IMPORT_C TInt Height() const;
public:
	TPoint iTl;
	TPoint iBr;
};

template <class T> inline T Max(T aLeft,T aRight)
{
	return(aLeft<aRight ? aRight : aLeft);
}

#pragma warning(disable:4018)

template <class T> inline T Min(T aLeft,T aRight)
{
	return(aLeft>aRight ? aRight : aLeft);
}

template <class T> inline int Max(T aLeft,TUint aRight)
{
	return(aLeft<aRight ? aRight : aLeft);
}

template <class T> inline int Min(T aLeft,TUint aRight)
{
	return(aLeft>aRight ? aRight : aLeft);
}

template <class T> inline T Abs(T aVal)
{
	return(aVal<0 ? -aVal : aVal);
}

class Mem
{
public:
	IMPORT_C static void Copy(void* dest, const void* src, int size);
	IMPORT_C static void*Remove(void* Dest, const void* Src, int Count);
	IMPORT_C static void Swap(void* Ptr1, void* Ptr2 ,int size);
	IMPORT_C static void FillZ(void* ptr, int size);
	IMPORT_C static void Fill(void* ptr, int size, TUint8 stuff);
};

extern IMPORT_C void DllMemFree(void* ptr);

class CleanupStack
{
public:
	static void PushL(void*) {}
	static void Pop() {}
};

// exception
class TException
{
public:
	IMPORT_C TException(int aCode) : iCode(aCode) {}
	inline int Code() const {return iCode;}
private:
	int iCode;
};

typedef int TTimeIntervalMicroSeconds32;

class User
{
public:
	IMPORT_C static void Leave(int aCode) {THROW_EXCEPTION_VOID(aCode);}
	IMPORT_C static void LeaveIfError(int aCode) {if (aCode!=KErrNone) THROW_EXCEPTION_VOID(aCode);}
	IMPORT_C static TInt TickCount();
	IMPORT_C static TAny* AllocL(TInt aSize);
	IMPORT_C static TAny* RllocL(void* Original, TInt Count );
	IMPORT_C static void Free(TAny* aPtr);
	IMPORT_C static void After(TTimeIntervalMicroSeconds32 aInterval);
};

template <class T>
class TRefByValue
{
public:
	inline TRefByValue(T& aRef) : iRef(aRef) {}
	inline operator T&() {return iRef;}
private:
	TRefByValue& operator=(TRefByValue aRef) {iRef=aRef; return (*this);}
//	TRefByValue& operator=(TRefByValue aRef) {iRef=aRef; return *this};}   //old  
private:
	T& iRef;
};

#ifdef _UNICODE
#define TDesC TDesC16
#define TPtrC TPtrC16
#define TDes  TDes16
#else
#define TDesC TDesC8
#define TPtrC TPtrC8
#define TDes  TDes8
#endif

inline TAny* operator new(size_t aSize, TLeave)
{
	return (TAny*)malloc(aSize);
}

inline void operator delete(void* p, TLeave)
{
	free(p);
}

#include "Base/e32des8.h"
#include "Base/e32des16.h"

#define __Align(s) ((((s)+__Size-1)/__Size)*__Size)

#if defined(_UNICODE)
#define __Size (sizeof(TUint)/sizeof(TUint16))
#else
#define __Size (sizeof(TUint)/sizeof(TUint8))
#endif

template <TInt S>
#ifdef _UNICODE
class TBuf : public TBufBase16
#else
class TBuf : public TBufBase8
#endif
{
public:
	inline TBuf();
	inline TBuf(TInt aLength);
	inline TBuf(const TText* aString);
	inline TBuf(const TDesC& aDes);
	inline TBuf<S>& operator=(const TText* aString) {Copy(aString);return(*this);}
	inline TBuf<S>& operator=(const TDesC& aDes) {Copy(aDes);return(*this);}
	inline TBuf<S>& operator=(const TBuf<S>& aBuf) {Copy(aBuf);return(*this);}
private:
	TText iBuf[__Align(S)];
};
#ifdef _UNICODE
template <TInt S>
inline TBuf<S>::TBuf()
	: TBufBase16(S)
	{}
template <TInt S>
inline TBuf<S>::TBuf(TInt aLength)
	: TBufBase16(aLength,S)
	{}
template <TInt S>
inline TBuf<S>::TBuf(const TText *aString)
	: TBufBase16(aString,S)
	{}
template <TInt S>
inline TBuf<S>::TBuf(const TDesC &aDes)
	: TBufBase16(aDes,S)
	{}
#endif

typedef TBuf<KMaxFileName> TFileName;

//-----------------------------------------------------------------------------
// RArrayBase & RArray
//-----------------------------------------------------------------------------

class RArrayBase
{
protected:
	IMPORT_C RArrayBase(TInt aEntrySize);
	IMPORT_C void Close();
	IMPORT_C TInt Count() const;
	IMPORT_C TAny* At(TInt aIndex) const;
	IMPORT_C TInt Append(const TAny* aEntry);
	IMPORT_C TInt Insert(const TAny* aEntry, TInt aPos);
	IMPORT_C void Remove(TInt aIndex);
	IMPORT_C void Reset();
//	IMPORT_C static TInt GetCount(const CBase* aPtr);
//	IMPORT_C static const TAny* GetElementPtr(const CBase* aPtr, TInt aIndex);
//private:
//	TInt Grow();
private:
	TInt iCount;
	TAny* iEntries;
	TInt iEntrySize;
	TInt iKeyOffset;
	TInt iAllocated;
	TInt iGranularity;
};

template <class T>
class RArray : private RArrayBase
{
public:
	inline RArray() : RArrayBase(sizeof(T)) {}
	inline void Close() {RArrayBase::Close();}
	inline TInt Count() const {return RArrayBase::Count();}
	inline const T& operator[](TInt aIndex) const {return *(const T*)At(aIndex);}
	inline T& operator[](TInt aIndex) {return *(T*)At(aIndex);}
	inline TInt Append(const T& aEntry) {return RArrayBase::Append(&aEntry);}
	inline TInt Insert(const T& aEntry, TInt aPos) {return RArrayBase::Insert(&aEntry,aPos);}
	inline void Remove(TInt aIndex) {RArrayBase::Remove(aIndex);}
	inline void Reset() {RArrayBase::Reset();}
};

#endif	//__STD_H__

// end of file `e32std.h'
