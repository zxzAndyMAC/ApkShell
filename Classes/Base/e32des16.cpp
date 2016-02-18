#include "Base/e32des8.h"
#include "Base/e32des16.h"
#include "Base/e32std.h"
#include <stdio.h>
#include <math.h>
#include <stdarg.h>

//-----------------------------------------------------------------------------
// TDesC16
//-----------------------------------------------------------------------------
const TUint16* TDesC16::Ptr() const
{
	switch (iType)
	{
	case DES_TYPE_TDESC:
		return (TUint16*)(((unsigned char*)this)+sizeof(TDesC16));
	case DES_TYPE_TBUFBASE:
		return (TUint16*)(((unsigned char*)this)+sizeof(TBufBase16));
	case DES_TYPE_TPTR:
		return (TUint16*)(*( (int*)( (unsigned char*)this + sizeof(TDes16) ) ));
	case DES_TYPE_TPTRC:
		return (TUint16*)(*( (int*)( (unsigned char*)this + sizeof(TDesC16) ) ));
	}
	THROW_EXCEPTION_NULL(KErrInvalidType);//return NULL;
};

// to be improved so that same as Sybmians's API
TInt TDesC16::Compare(const TDesC16& aDes) const
{
	if (aDes.Length()!=iLength) return 1;
	for (int i=0; i<iLength; i++)
	{
		if (aDes[i] != Ptr()[i]) return 1;
	}
	return 0;
}

TInt TDesC16::Locate(TChar aChar) const
{
	for (int i=0; i<iLength; i++)
	{
		if (Ptr()[i] == (TUint16)aChar)
		{
			return i;
		}
	}
	THROW_EXCEPTION_ERR(KErrNotFound);//return KErrNotFound;
}

TInt TDesC16::LocateReverse(TChar aChar) const
{
	for (int i=iLength-1; i>=0; i--)
	{
		if (Ptr()[i] == (TUint16)aChar)
		{
			return i;
		}
	}
	THROW_EXCEPTION_ERR(KErrNotFound);//return KErrNotFound;
}

TInt TDesC16::Find(const TDesC16& aDes) const
{
	for (int i=0; i<iLength; i++)
	{
		int j = 0;
		while (Ptr()[i+j] == aDes[j])
		{
			j++;
			if (j == aDes.Length())
			{
				return i;
			}
		}
	}
	THROW_EXCEPTION_ERR(KErrNotFound);//return KErrNotFound;
}

TInt TDesC16::Find(const TUint16* pS, TInt aLenS) const
{
	TPtrC16 ptr(pS, aLenS);
	return Find(ptr);
}

TPtrC16 TDesC16::Left(TInt aLength) const
{
	return TPtrC16(Ptr(), Min(Length(), aLength));
}

TPtrC16 TDesC16::Right(TInt aLength) const
{
	return TPtrC16(Ptr()+Length()-aLength, Min(Length(), aLength));
}

TPtrC16 TDesC16::Mid(TInt aPos) const
{
	return TPtrC16(Ptr()+aPos, Length()-aPos);
}

TPtrC16 TDesC16::Mid(TInt aPos, TInt aLength) const
{
	return TPtrC16(Ptr()+aPos, Min(aLength, Length()-aPos));
}

//void TDesC16::CopyTo(char* aStr) const
//{
//	for (int i=0; i<Length(); i++)
//	{
//		aStr[i] = (char)(*this)[i];
//	}
//	aStr[Length()] = '\0';
//}

//const TUint16& TDesC16::AtC(TInt aIndex) const
//{
//	return (*this)[aIndex];
//}

//-----------------------------------------------------------------------------
// TPtrC16
//-----------------------------------------------------------------------------
TPtrC16::TPtrC16()
:	TDesC16(DES_TYPE_TPTRC,0),
	iPtr(NULL)
{
}

TPtrC16::TPtrC16(const TDesC16& aDes)
:	TDesC16(DES_TYPE_TPTRC,aDes.Length()),
	iPtr(aDes.Ptr())
{
}

TPtrC16::TPtrC16(const TUint16* aString)
:	TDesC16(DES_TYPE_TPTRC, wcslen((const wchar_t*)aString)),
iPtr(aString)
{
}

TPtrC16::TPtrC16(const TUint16* aBuf,TInt aLength)
:	TDesC16(DES_TYPE_TPTRC,aLength), iPtr(aBuf)
{
}

void TPtrC16::Set(const TUint16* aBuf,TInt aLength)
{
	iPtr=aBuf;
	DoSetLength(aLength);
}

void TPtrC16::Set(const TDesC16& aDes)
{
	iPtr=aDes.Ptr();
	DoSetLength(aDes.Length());
}

void TPtrC16::Set(const TPtrC16& aPtr)
{
	iPtr=aPtr.Ptr();
	DoSetLength(aPtr.Length());
}

TPtrC16& TPtrC16::operator=(const TPtrC16& aDes)
{
	iPtr=aDes.Ptr();
	DoSetLength(aDes.Length());
	return *this;
}

//-----------------------------------------------------------------------------
// TDes16
//-----------------------------------------------------------------------------
void TDes16::Zero()
{
	DoSetLength(0);
}

void TDes16::ZeroTerminate()
{
	if (Length() < iMaxLength)
	{
		(*this)[Length()] = '\0';
	}
	else
	{
		(*this)[iMaxLength-1] = '\0';
	}
}

const TUint16* TDes16::PtrZ()
{
	ZeroTerminate();
	return Ptr();
}

void TDes16::SetLength(TInt aLength)
{
	if (aLength > MaxLength())
	{
		User::Leave(KErrTooBig);
	}
	DoSetLength(aLength);
}

void TDes16::SetMax()
{
	DoSetLength(iMaxLength);
}

void TDes16::Copy(const TDesC8& aDes)
{
	int len = Min(MaxLength(), aDes.Length());
	for (int i=0; i<len; i++)
	{
		(*this)[i] = (TUint8)aDes[i];
	}
	SetLength(len);
}

void TDes16::Copy(const TDesC16& aDes)
{
	int len = Min(MaxLength(), aDes.Length());
	for (int i=0; i<len; i++)
	{
		(*this)[i] = aDes[i];
	}
	SetLength(len);
}

void TDes16::Copy(const TUint16 *aBuf,TInt aLength)
{
	int len = Min(MaxLength(), aLength);
	for (int i=0; i<len; i++)
	{
		(*this)[i] = aBuf[i];
	}
	SetLength(len);
}

void TDes16::Copy(const TUint16 *aString)
{
	int len = Min(MaxLength(), wcslen((const wchar_t*)aString));
	for (int i=0; i<len; i++)
	{
		(*this)[i] = aString[i];
	}
	SetLength(len);
}

void TDes16::CopyLC(const TDesC16& aDes)
{
	int len = Min(MaxLength(), aDes.Length());
	for (int i=0; i<len; i++)
	{
		(*this)[i] = (aDes[i] >= 'A' && aDes[i] <= 'Z') ? (aDes[i]+'a'-'A') : aDes[i];
	}
	SetLength(len);
}

void TDes16::CopyUC(const TDesC16& aDes)
{
	int len = Min(MaxLength(), aDes.Length());
	for (int i=0; i<len; i++)
	{
		(*this)[i] = (aDes[i] >= 'a' && aDes[i] <= 'z') ? (aDes[i]+'A'-'a') : aDes[i];
	}
	SetLength(len);
}

void TDes16::Append(TChar aChar)
{
	if (Length() >= MaxLength())
	{
		return;
	}
	(*this)[Length()] = aChar;
	SetLength(Length() + 1);
}

void TDes16::Append(const TDesC8& aDes)
{
	TInt len1 = Length();
	TInt len2 = Min(MaxLength()-len1, aDes.Length());
	for (TInt i=0; i<len2; i++)
	{
		(*this)[len1+i] = aDes[i];
	}
	SetLength(len1 + len2);
}

void TDes16::Append(const TDesC16& aDes)
{
	int len = Min(aDes.Length(), MaxLength()-Length());
	for (int i=0; i<len; i++)
	{
		(*this)[Length()+i] = (TUint16)aDes[i];
		if (aDes[i] & 0xff00)
		{
			SetLength(Length()+1);
			(*this)[Length()+i] = (TUint16)(aDes[i]>>8);
		}
	}
	SetLength(Length() + len);
}

void TDes16::Append(const TUint16* aBuf, TInt aLength)
{
	int len = Min(aLength, MaxLength()-Length());
	for (int i=0; i<len; i++)
	{
		(*this)[Length()+i] = aBuf[i];
	}
	SetLength(Length()+aLength);
}

void TDes16::AppendFill(TChar aChar, TInt aLength)
{
	(*this)[Length()] = aChar;
	SetLength(Length() + 1);
}

void TDes16::AppendNum(TInt aVal)
{
	TBuf16<16> buf;
	buf.Num(aVal);
	Append(buf);
}

void TDes16::Num(TInt aVal)
{
	TUint16 str[16];
//18:46 2011/4/9 TODO	ANDROID_NDK
	swprintf((wchar_t*)str,16*sizeof(TUint16), L"%d", aVal);
//	swprintf((wchar_t*)str, L"%d", aVal);
	Copy(str);
}

void TDes16::Delete(TInt aPos,TInt aLength)
{
	if (aPos >= Length()) { return; }

	if ((aPos+aLength) >= Length())
	{
		DoSetLength(aPos);
		return;
	}

	DoSetLength(Length()-aLength);
	Mem::Copy(WPtr()+aPos, WPtr()+aPos+aLength, Length()*2);
}

void TDes16::Fill(TChar aChar)
{
	for (int i=0; i<Length(); i++)
	{
		(*this)[i] = (TUint16)aChar;
	}
}

void TDes16::Fill(TChar aChar, TInt aLength)
{
	int len = Min(MaxLength(), aLength);
	for (int i=0; i<len; i++)
	{
		(*this)[i] = (TUint16)aChar;
	}
	DoSetLength(len);
}

void TDes16::Insert(TInt aPos, const TDesC16& aDes)
{
	TInt maxLength = MaxLength();
	TUint16 * temp = new TUint16[maxLength];
	Mem::FillZ(temp, maxLength * sizeof(TUint16));
	TInt i = 0;
	TInt tempIndex = 0;
	// 拷贝前段
	TInt len = Min(aPos, maxLength);
	for (i = 0; i < len; i++)
	{
		temp[tempIndex++] = (*this)[i];
	}
	// 拷贝插入
	for (i = 0; i < aDes.Length(); i++)
	{
		if (tempIndex >= maxLength)
		{
			return;
		}
		temp[tempIndex++] = aDes[i];
	}
	// 拷贝后段
	len = Min(Length(), maxLength);
	for (i = aPos; i < len; i++)
	{
		if (tempIndex >= maxLength)
		{
			return;
		}
		temp[tempIndex++] = (*this)[i];
	}

	len = Min(Length() + aDes.Length(), maxLength);

	for (i = 0; i < len; i++)
	{
		(*this)[i] = temp[i];
	}
	DoSetLength(len);
	
	delete[] temp;
	temp = NULL;
}

void TDes16::LowerCase()
{
	for (int i=0; i<Length(); i++)
	{
		(*this)[i] = ((*this)[i] >= 'A' && (*this)[i] <= 'Z') ? ((*this)[i]+'a'-'A') : (*this)[i];
	}
}

void TDes16::UpperCase()
{
	for (int i=0; i<Length(); i++)
	{
		(*this)[i] = ((*this)[i] >= 'a' && (*this)[i] <= 'z') ? ((*this)[i]+'A'-'a') : (*this)[i];
	}
}

void TDes16::Format(TRefByValue<const TDesC16> aFmt, ...)
{
	Zero();

	const TDesC16* des = (const TDesC16*)( * ((int*)&aFmt) );

	const TUint16* fmtStr = (const TUint16*)des->Ptr();

	va_list list;
	va_start(list, aFmt);

	char* pArg = NULL;	//等价于原来的va_list 
	TUint16 c;
	pArg = (char*) &aFmt;	//注意不要写成p = aFmt !!因为这里要对//参数取址，而不是取值
	pArg += sizeof(aFmt);	//等价于原来的va_start          
	do
	{
		c = *fmtStr;
		if (c != '%')
		{
			Append(c);	//照原样输出字符
		}
		else
		{
			//按格式字符输出数据
			switch(*++fmtStr) 
			{
			case 'd':
				AppendNum(*((int*)pArg));           
				pArg += sizeof(int);	//等价于原来的va_arg: va_arg( pArg, char *);
				break;
			case 'c':
				Append(TChar(*((int*)pArg)));           
				pArg += sizeof(int);	//等价于原来的va_arg: va_arg( pArg, char *);
				break;
			case 'x':
				//AppendNum(*((int*)pArg));
				Append(_L16("FMT_X"));
				pArg += sizeof(int);
				break;
			case '.':	// %.Nf - N: precision
				++fmtStr;
				if ( (*fmtStr < '0' || *fmtStr > '9') || *(fmtStr+1) != 'f')
				{
					Append(_L16("FMT_ERR"));
				}
				else
				{
					int precision = (*fmtStr) - '0';
					++fmtStr;
					double var = *((double*)pArg);
					AppendNum((int)var);
					Append('.');
					int decimal = (int)( (var - (int)var) * pow((double)10, precision) );
					TBuf8<32> tmp; tmp.Num(decimal); int _len = precision-tmp.Length();
					for (int i=0; i<_len; i++)
						Append('0');
					AppendNum( decimal );
				}
				pArg += sizeof(double);
				break;
			case '%':
				Append('%');
				break;
			default:
				Append(_L16("FMT_ERR"));
				break;
			} 
		}
		++fmtStr;
	} while (*fmtStr != '\0'); 
	pArg = NULL;	//等价于va_end(pArg)
//	va_end( pArg ); // 将pArg置为NULL
}

void TDes16::Replace(TInt aPos,TInt aLength,const TDesC16& aDes)
{
	for (int i=0; i<aLength; i++)
	{
		(*this)[aPos+i] = aDes[i];
		if ( (aPos+i+1) >= MaxLength() )
		{
			break;
		}
	}
}

// end of file `e32des16.cpp'
