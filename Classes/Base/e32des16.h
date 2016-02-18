// e32des16.h
//
// Created 2005-04-08, ER1C
//

#ifndef __DES16_H__
#define __DES16_H__

#include <string.h>
#include "Base/e32def.h"
#include "Base/des_type.h"

#pragma warning(disable:4018)
#pragma warning(disable:4200)

#define __Size16			(sizeof(TInt)/sizeof(TInt16))
#define __Align16(s)		((((s)+__Size16-1)/__Size16)*__Size16)

const TUint KMaskDesLength16=0xfffffff;	// 28 bits

template <class T> class TRefByValue;
class TBufCBase16;
class HBufC16;
class TDes16;
class TPtrC16;
class TPtr16;
class TDesC16
{
public:
	TInt operator==(const TDesC16& aDes) const
	{
		return !Compare(aDes);
	}

	TInt operator!=(const TDesC16& aDes) const
	{
		return Compare(aDes);
	}

	const TUint16& operator[](TInt aIndex) const
	{
		return Ptr()[aIndex];
	}

	TInt Length() const
	{
	#ifdef __DESC_NO_BIT_FIELDS__
		return iLength & KMaskDesLength16;
	#else
		return iLength;
	#endif
	}


	TInt Size() const
	{
		return iLength;
	}
//	inline TInt operator==(const TDesC16& aDes) const;
//	inline TInt operator!=(const TDesC16& aDes) const;
//	inline const TUint16 &operator[](TInt aIndex) const;
//	inline TInt Length() const;
//	inline TInt Size() const;
	IMPORT_C const TUint16 *Ptr() const;
	IMPORT_C TInt Compare(const TDesC16& aDes) const;
	IMPORT_C TInt Locate(TChar aChar) const;
	IMPORT_C TInt LocateReverse(TChar aChar) const;
	IMPORT_C TInt Find(const TDesC16& aDes) const;
	IMPORT_C TInt Find(const TUint16 *pS,TInt aLenS) const;
	IMPORT_C TPtrC16 Left(TInt aLength) const;
	IMPORT_C TPtrC16 Right(TInt aLength) const;
	IMPORT_C TPtrC16 Mid(TInt aPos) const;
	IMPORT_C TPtrC16 Mid(TInt aPos,TInt aLength) const;
//	IMPORT_C void CopyTo(char* aStr) const;	// added for Win32
protected:
	inline TDesC16(TInt aType,TInt aLength)
	{
#ifdef __DESC_NO_BIT_FIELDS__
		iLength = aLength | (aType << 28);
#else
		iLength = aLength;
		iType = aType;
#endif
	}
	inline TDesC16() {}
//	inline TInt Type() const;
//	inline void DoSetLength(TInt aLength);
	TInt Type() const
	{
	#ifdef __DESC_NO_BIT_FIELDS__
		return (iLength & (~KMaskDesLength16)) >> 216;
	#else
		return iType;
	#endif
	}

	void DoSetLength(TInt aLength)
	{
	#ifdef __DESC_NO_BIT_FIELDS__
		iLength = (iLength & (~KMaskDesLength16)) | aLength;
	#else
		iLength = aLength;
	#endif
	}
//	const TUint16& AtC(TInt aIndex) const;
private:
#ifdef __DESC_NO_BIT_FIELDS__
	TUint iLength;
#else
	unsigned int iLength:28;
	unsigned int iType:4;
#endif
};

class TPtrC16 : public TDesC16
{
public:
	IMPORT_C TPtrC16();
	IMPORT_C TPtrC16(const TDesC16& aDes);
	IMPORT_C TPtrC16(const TUint16* aString);
	IMPORT_C TPtrC16(const TUint16* aBuf,TInt aLength);
	inline void Set(const TUint16* aBuf,TInt aLength);
	inline void Set(const TDesC16& aDes);
	inline void Set(const TPtrC16& aPtr);
private:
	TPtrC16& operator=(const TPtrC16& aDes);
protected:
	const TUint16 *iPtr;
};

class TDesC8;
class TDesC16;
class TDes16 : public TDesC16
{
public:
	TDes16& operator=(const TUint16* aString)
	{
		Copy(aString);
		return *this;
	}

	TDes16& operator=(const TDesC16& aDes)
	{
		Copy(aDes);
		return *this;
	}

	TDes16& operator=(const TDes16& aDes)
	{
		Copy(aDes);
		return *this;
	}

	TDes16& operator+=(const TDesC16 &aDes)
	{
		Append(aDes);
		return *this;
	}

	TInt MaxLength() const
	{
		return iMaxLength;
	}

	TInt Mae32size() const
	{
		return iMaxLength;
	}

	TUint16& operator[](TInt aIndex)
	{
		return ((TUint16*)Ptr())[aIndex];
	}

	inline const TUint16& operator[](TInt aIndex) const {return Ptr()[aIndex];}
	/*
	inline TDes16& operator=(const TUint16* aString);
	inline TDes16& operator=(const TDesC16& aDes);
	inline TDes16& operator=(const TDes16& aDes);
	inline TInt MaxLength() const;
	inline TInt Mae32size() const;
	inline TUint16& operator[](TInt aIndex);
	inline TDes16& operator+=(const TDesC16& aDes);
	*/
	IMPORT_C void Zero();
	IMPORT_C const TUint16 *PtrZ();
 	IMPORT_C void SetLength(TInt aLength);
 	IMPORT_C void SetMax();
	IMPORT_C void Copy(const TDesC8& aDes);
	IMPORT_C void Copy(const TDesC16& aDes);
	IMPORT_C void Copy(const TUint16* aBuf,TInt aLength);
	IMPORT_C void Copy(const TUint16* aString);
	IMPORT_C void CopyLC(const TDesC16& aDes);
	IMPORT_C void CopyUC(const TDesC16& aDes);
	IMPORT_C void Append(TChar aChar);
	IMPORT_C void Append(const TDesC8& aDes);
	IMPORT_C void Append(const TDesC16& aDes);
	IMPORT_C void Append(const TUint16* aBuf,TInt aLength);
	IMPORT_C void AppendFill(TChar aChar,TInt aLength);
	IMPORT_C void ZeroTerminate();
	IMPORT_C void Fill(TChar aChar);
	IMPORT_C void Fill(TChar aChar,TInt aLength);
	IMPORT_C void LowerCase();
	IMPORT_C void UpperCase();
	IMPORT_C void Insert(TInt aPos,const TDesC16& aDes);
	IMPORT_C void Delete(TInt aPos,TInt aLength);
	IMPORT_C void Replace(TInt aPos,TInt aLength,const TDesC16& aDes);
	IMPORT_C void Num(TInt aVal);
	IMPORT_C void Format(TRefByValue<const TDesC16> aFmt,...);
	IMPORT_C void AppendNum(TInt aVal);
protected:
	inline TDes16(TInt aType,TInt aLength,TInt aMaxLength) : TDesC16(aType,aLength), iMaxLength(aMaxLength) {}
	inline TDes16() {}
	inline TUint16* WPtr() const {return (TUint16*)Ptr();}
protected:
	TInt iMaxLength;
};

class TPtr16 : public TDes16
{
public:
	/*
	TPtr16(TUint16* aBuf,TInt aMaxLength) : TDes16(DES_TYPE_TPTR,0,aMaxLength), iPtr(aBuf) {}
	TPtr16(TUint16* aBuf,TInt aLength,TInt aMaxLength) : TDes16(DES_TYPE_TPTR,aLength,aMaxLength), iPtr(aBuf) {}
	inline void Set(TUint16* aBuf,TInt aLength,TInt aMaxLength);
protected:
	TUint16 *iPtr;
	*/
	IMPORT_C TPtr16(TUint16* aBuf,TInt aMaxLength) : TDes16(DES_TYPE_TPTR,0,aMaxLength), iPtr(aBuf) {}
	IMPORT_C TPtr16(TUint16* aBuf,TInt aLength,TInt aMaxLength) : TDes16(DES_TYPE_TPTR,aLength,aMaxLength), iPtr(aBuf) {}
	inline TPtr16& operator=(const TUint16* aString) {Copy(aString); return *this;}
	inline TPtr16& operator=(const TDesC16& aDes) {Copy(aDes); return *this;}
	inline TPtr16& operator=(const TPtr16& aPtr) {Copy(aPtr); return *this;}
	inline void Set(TUint16* aBuf,TInt aLength,TInt aMaxLength) {Copy(aBuf,aLength); iMaxLength=aMaxLength;}
	inline void Set(const TPtr16& aPtr) {Copy(aPtr);}
private:
//	TPtr16(TBufCBase16& aLcb,TInt aMaxLength);
protected:
	TUint16 *iPtr;
private:
	friend class TBufCBase16;
};

class TBufBase16 : public TDes16
{
protected:
	IMPORT_C TBufBase16(TInt aMaxLength) : TDes16(DES_TYPE_TBUFBASE,0,aMaxLength) {}
	IMPORT_C TBufBase16(TInt aLength,TInt aMaxLength) : TDes16(DES_TYPE_TBUFBASE,aLength,aMaxLength) {}
	IMPORT_C TBufBase16(const TUint16* aString,TInt aMaxLength) : TDes16(DES_TYPE_TBUFBASE,wcslen((const wchar_t*)aString),aMaxLength) {}
	IMPORT_C TBufBase16(const TDesC16& aDes,TInt aMaxLength) : TDes16(DES_TYPE_TBUFBASE,aDes.Length(),aMaxLength) {}
};

template<TInt S>
class TBuf16 : public TBufBase16
{
public:
	/*
	inline TBuf16() : TBufBase16(S) {}
	inline TBuf16(TInt aLength) : TBufBase16(aLength,S) {}
	inline TBuf16(const TUint16* aString) : TBufBase16(S) {Copy(aString);}
	inline TBuf16(const char* aString) : TBufBase16(S) {Copy((const TUint16*)aString);}
	inline TBuf16(const TDesC16& aDes) : TBufBase16(S) {Copy(aDes);}
	const TUint16& operator[](TInt aIndex) const {return iBuf[aIndex];}
	TUint16& operator[](TInt aIndex) {return iBuf[aIndex];}
	const TUint16* Ptr() const {return &iBuf[0];}
	TUint16* Ptr() {return &iBuf[0];}
	inline TBuf16<S>& operator=(const char* aStr) {Copy((const TUint16*)aStr); return *this;}
	*/
	inline TBuf16() : TBufBase16(S) {}
	inline TBuf16(TInt aLength) : TBufBase16(aLength,S) {}
    inline TBuf16(const TUint16* aString) : TBufBase16(S) {Copy(aString);}
	inline TBuf16(const TDesC16& aDes) : TBufBase16(S) {Copy(aDes);}
	inline TBuf16<S>& operator=(const TUint16* aString) {Copy(aString); return *this;}
	inline TBuf16<S>& operator=(const TDesC16& aDes) {Copy(aDes); return *this;}
	inline TBuf16<S>& operator=(const TBuf16<S>& aBuf) {Copy(aBuf); return *this;}
protected:
	TUint16 iBuf[__Align16(S)];
};

#include "e32std.h"

typedef TRefByValue<const TDesC16> __TRefDesC16;
template <TInt S>
class TLitC16
{
public:
	inline const TDesC16* operator&() const
	{
		return reinterpret_cast<const TDesC16*>(this);
	}
	inline operator const TDesC16&() const
	{
		return *operator&();
	}
	inline const TDesC16& operator()() const
	{
		return *operator&();
	}
	inline operator const __TRefDesC16() const
	{
		return *operator&();
	}
public:
	TUint iTypeLength;
	TText16 iBuf[__Align16(S)];
};

//const TInt KMaxFileName=0x100;
//typedef TBuf16<KMaxFileName> TFileName;

#endif	//__DES16_H__
