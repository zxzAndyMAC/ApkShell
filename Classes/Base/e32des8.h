// e32des8.h
//
// Created 2005-04-08, ER1C
//

#ifndef __DES8_H__
#define __DES8_H__

#include <string.h>
#include "Base/e32def.h"
#include "Base/des_type.h"

#pragma warning(disable:4018)
#pragma warning(disable:4200)

#define __Size8			(sizeof(TInt)/sizeof(TInt8))
#define __Align8(s)		((((s)+__Size8-1)/__Size8)*__Size8)

const TUint KMaskDesLength8=0xfffffff;	// 28 bits

template <class T> class TRefByValue;
class TBufCBase8;
class HBufC8;
class TDes8;
class TPtrC8;
class TPtr8;
class TDesC8
{
public:
//	inline TInt operator==(const TDesC8& aDes) const;
//	inline TInt operator!=(const TDesC8& aDes) const;
//	inline const TUint8& operator[](TInt aIndex) const;
	IMPORT_C TInt operator==(const TDesC8& aDes) const { return !Compare(aDes); }
	IMPORT_C TInt operator!=(const TDesC8& aDes) const { return Compare(aDes); }
	IMPORT_C const TUint8& operator[](TInt aIndex) const { return Ptr()[aIndex]; }
	IMPORT_C TInt Length() const
	{
	#ifdef __DESC_NO_BIT_FIELDS__
		return iLength & KMaskDesLength8;
	#else
		return iLength;
	#endif
	}
	IMPORT_C TInt Size() const
	{
		return iLength;
	}
	IMPORT_C const TUint8* Ptr() const;
	IMPORT_C TInt Compare(const TDesC8& aDes) const;
	IMPORT_C TInt Locate(TChar aChar) const;
	IMPORT_C TInt LocateReverse(TChar aChar) const;
	IMPORT_C TInt Find(const TDesC8& aDes) const;
	IMPORT_C TInt Find(const TUint8* pS,TInt aLenS) const;
	IMPORT_C TPtrC8 Left(TInt aLength) const;
	IMPORT_C TPtrC8 Right(TInt aLength) const;
	IMPORT_C TPtrC8 Mid(TInt aPos) const;
	IMPORT_C TPtrC8 Mid(TInt aPos,TInt aLength) const;
//	HBufC8* alloc() const;
//	HBufC8* allocL() const;
//	HBufC8* allocLC() const;
//	IMPORT_C void CopyTo(char* aStr) const;	// added for Win32
protected:
	inline TDesC8(TInt aType,TInt aLength)
	{
#ifdef __DESC_NO_BIT_FIELDS__
		iLength = aLength | (aType << 28);
#else
		iLength = aLength;
		iType = aType;
#endif
	}
	inline TDesC8() {}
	TInt Type() const
	{
	#ifdef __DESC_NO_BIT_FIELDS__
		return (iLength & (~KMaskDesLength8)) >> 28;
	#else
		return iType;
	#endif
	}
	void DoSetLength(TInt aLength)
	{
	#ifdef __DESC_NO_BIT_FIELDS__
		iLength = (iLength & (~KMaskDesLength8)) | aLength;
	#else
		iLength = aLength;
	#endif
	}
//	const TUint8& AtC(TInt aIndex) const;
private:
#ifdef __DESC_NO_BIT_FIELDS__
	TUint iLength;
#else
	unsigned int iLength:28;
	unsigned int iType:4;
#endif
};

class TPtrC8 : public TDesC8
{
public:
	IMPORT_C TPtrC8();
	IMPORT_C TPtrC8(const TDesC8& aDes);
	IMPORT_C TPtrC8(const TUint8* aString);
	IMPORT_C TPtrC8(const TUint8* aBuf,TInt aLength);
	inline void Set(const TUint8* aBuf,TInt aLength);
	inline void Set(const TDesC8& aDes);
	inline void Set(const TPtrC8& aPtr);
private:
	TPtrC8& operator=(const TPtrC8& aDes);
protected:
	const TUint8* iPtr;
};

class TDesC16;
class TDes8 : public TDesC8
{
public:
	/*
	inline TDes8& operator=(const TUint8* aString);
	inline TDes8& operator=(const TDesC8& aDes);
	inline TDes8& operator=(const TDes8& aDes);
	inline TInt MaxLength() const;
	inline TInt Mae32size() const;
	inline TUint8& operator[](TInt aIndex);
	inline TDes8& operator+=(const TDesC8& aDes);
	*/
	TDes8& operator=(const TUint8* aString) { Copy(aString); return *this; }
	TDes8& operator=(const TDesC8& aDes) { Copy(aDes); return *this; }
	TDes8& operator=(const TDes8& aDes) { Copy(aDes); return *this; }
	TInt MaxLength() const { return iMaxLength; }
	TInt Mae32size() const { return iMaxLength; }
	inline const TUint8& operator[](TInt aIndex) const { return Ptr()[aIndex]; }
	TUint8& operator[](TInt aIndex) { return ((TUint8*)Ptr())[aIndex]; }
	TDes8& operator+=(const TDesC8 &aDes) { Append(aDes); return *this; }

	IMPORT_C void Zero();
	IMPORT_C const TUint8* PtrZ();
 	IMPORT_C void SetLength(TInt aLength);
 	IMPORT_C void SetMax();
	IMPORT_C void Copy(const TDesC8& aDes);
	IMPORT_C void Copy(const TUint8* aBuf,TInt aLength);
	IMPORT_C void Copy(const TUint8* aString);
	IMPORT_C void Copy(const TDesC16& aDes);
	IMPORT_C void CopyLC(const TDesC8& aDes);
	IMPORT_C void CopyUC(const TDesC8& aDes);
	IMPORT_C void Append(TChar aChar);
	IMPORT_C void Append(const TDesC8& aDes);
	IMPORT_C void Append(const TDesC16& aDes);
	IMPORT_C void Append(const TUint8* aBuf,TInt aLength);
	IMPORT_C void AppendFill(TChar aChar,TInt aLength);
	IMPORT_C void ZeroTerminate();
	IMPORT_C void Fill(TChar aChar);
	IMPORT_C void Fill(TChar aChar,TInt aLength);
	IMPORT_C void LowerCase();
	IMPORT_C void UpperCase();
	IMPORT_C void Insert(TInt aPos,const TDesC8& aDes);
	IMPORT_C void Delete(TInt aPos,TInt aLength);
	IMPORT_C void Replace(TInt aPos,TInt aLength,const TDesC8& aDes);
	IMPORT_C void Num(TInt aVal);
	IMPORT_C void Format(TRefByValue<const TDesC8> aFmt,...);
	IMPORT_C void AppendNum(TInt aVal);
protected:
	inline TDes8(TInt aType,TInt aLength,TInt aMaxLength) : TDesC8(aType,aLength), iMaxLength(aMaxLength) {}
	inline TDes8() {}
	inline TUint8* WPtr() const {return (TUint8*)Ptr();}
protected:
	TInt iMaxLength;
};

class TPtr8 : public TDes8
{
public:
	/*
	TPtr8(TUint8* aBuf,TInt aMaxLength) : TDes8(DES_TYPE_TPTR,0,aMaxLength), iPtr(aBuf) {}
	TPtr8(TUint8* aBuf,TInt aLength,TInt aMaxLength) : TDes8(DES_TYPE_TPTR,aLength,aMaxLength), iPtr(aBuf) {}
	inline void Set(TUint8* aBuf,TInt aLength,TInt aMaxLength);
protected:
	TUint8* iPtr;
	*/
	IMPORT_C TPtr8(TUint8* aBuf,TInt aMaxLength) : TDes8(DES_TYPE_TPTR,0,aMaxLength), iPtr(aBuf) {}
	IMPORT_C TPtr8(TUint8* aBuf,TInt aLength,TInt aMaxLength) : TDes8(DES_TYPE_TPTR,aLength,aMaxLength), iPtr(aBuf) {}
	inline TPtr8& operator=(const TUint8* aString) {Copy(aString); return *this;}
	inline TPtr8& operator=(const TDesC8& aDes) {Copy(aDes); return *this;}
	inline TPtr8& operator=(const TPtr8& aPtr) {Copy(aPtr); return *this;}
	inline void Set(TUint8* aBuf,TInt aLength,TInt aMaxLength) {Copy(aBuf,aLength); iMaxLength=aMaxLength;}
	inline void Set(const TPtr8& aPtr) {Copy(aPtr);}
private:
//	TPtr8(TBufCBase8& aLcb,TInt aMaxLength);
protected:
	TUint8* iPtr;
private:
	friend class TBufCBase8;
};

class TBufBase8 : public TDes8
{
protected:
	IMPORT_C TBufBase8(TInt aMaxLength) : TDes8(DES_TYPE_TBUFBASE,0,aMaxLength) {}
	IMPORT_C TBufBase8(TInt aLength,TInt aMaxLength) : TDes8(DES_TYPE_TBUFBASE,aLength,aMaxLength) {}
	IMPORT_C TBufBase8(const TUint8* aString,TInt aMaxLength) : TDes8(DES_TYPE_TBUFBASE,strlen((const char*)aString),aMaxLength) {}
	IMPORT_C TBufBase8(const TDesC8& aDes,TInt aMaxLength) : TDes8(DES_TYPE_TBUFBASE,aDes.Length(),aMaxLength) {}
};

template<TInt S>
class TBuf8 : public TBufBase8
{
public:
	/*
	inline TBuf8() : TBufBase8(S) {}
	inline TBuf8(TInt aLength) : TBufBase8(aLength,S) {}
	inline TBuf8(const TUint8* aString) : TBufBase8(S) {Copy(aString);}
	inline TBuf8(const char* aString) : TBufBase8(S) {Copy((const TUint8*)aString);}
	inline TBuf8(const TDesC8& aDes) : TBufBase8(S) {Copy(aDes);}
	const TUint8& operator[](TInt aIndex) const {return iBuf[aIndex];}
	TUint8& operator[](TInt aIndex) {return iBuf[aIndex];}
	const TUint8* Ptr() const {return &iBuf[0];}
	TUint8* Ptr() {return &iBuf[0];}
	inline TBuf8<S>& operator=(const char* aStr) {Copy((const TUint8*)aStr); return *this;}
	*/
	inline TBuf8() : TBufBase8(S) {}
	inline TBuf8(TInt aLength) : TBufBase8(aLength,S) {}
    inline TBuf8(const TUint8* aString) : TBufBase8(S) {Copy(aString);}
	inline TBuf8(const TDesC8& aDes) : TBufBase8(S) {Copy(aDes);}
	inline TBuf8<S>& operator=(const TUint8* aString) {Copy(aString); return *this;}
	inline TBuf8<S>& operator=(const TDesC8& aDes) {Copy(aDes); return *this;}
	inline TBuf8<S>& operator=(const TBuf8<S>& aBuf) {Copy(aBuf); return *this;}
protected:
	TUint8 iBuf[__Align8(S)];
};

#include "e32std.h"

typedef TRefByValue<const TDesC8> __TRefDesC8;
template <TInt S>
class TLitC8
{
public:
	inline const TDesC8* operator&() const
	{
		return reinterpret_cast<const TDesC8*>(this);
	}
	inline operator const TDesC8&() const
	{
		return *operator&();
	}
	inline const TDesC8& operator()() const
	{
		return *operator&();
	}
	inline operator const __TRefDesC8() const
	{
		return *operator&();
	}
public:
	TUint iTypeLength;
	TText8 iBuf[__Align8(S)];
};

#endif	//__DES8_H__
