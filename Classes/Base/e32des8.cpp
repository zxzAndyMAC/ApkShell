#include "Base/e32des8.h"
#include "Base/e32des16.h"
#include "Base/e32std.h"
#include <stdio.h>
#include <math.h>
//#include <stdarg.h>
#include <string.h>

//-----------------------------------------------------------------------------
// TDesC8
//-----------------------------------------------------------------------------
const TUint8* TDesC8::Ptr() const
{
	switch (iType)
	{
	case DES_TYPE_TDESC:
		return (TUint8*)(((unsigned char*)this)+sizeof(TDesC8));
	case DES_TYPE_TBUFBASE:
		return (TUint8*)(((unsigned char*)this)+sizeof(TBufBase8));
	case DES_TYPE_TPTR:
		return (TUint8*)(*( (int*)( (unsigned char*)this + sizeof(TDes8) ) ));
	case DES_TYPE_TPTRC:
		return (TUint8*)(*( (int*)( (unsigned char*)this + sizeof(TDesC8) ) ));
	}
	THROW_EXCEPTION_NULL(KErrInvalidType);//return NULL;
};

// to be improved so that same as Sybmians's API
TInt TDesC8::Compare(const TDesC8& aDes) const
{
	if (aDes.Length()!=iLength) return 1;
	for (int i=0; i<iLength; i++)
	{
		if (aDes[i] != Ptr()[i]) return 1;
	}
	return 0;
}

TInt TDesC8::Locate(TChar aChar) const
{
	for (int i=0; i<iLength; i++)
	{
		if (Ptr()[i] == (TUint8)aChar)
		{
			return i;
		}
	}
	THROW_EXCEPTION_ERR(KErrNotFound);//return KErrNotFound;
}

TInt TDesC8::LocateReverse(TChar aChar) const
{
	for (int i=iLength-1; i>=0; i--)
	{
		if (Ptr()[i] == (TUint8)aChar)
		{
			return i;
		}
	}
	THROW_EXCEPTION_ERR(KErrNotFound);//return KErrNotFound;
}

TInt TDesC8::Find(const TDesC8& aDes) const
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

TInt TDesC8::Find(const TUint8* pS, TInt aLenS) const
{
	TPtrC8 ptr(pS, aLenS);
	return Find(ptr);
}

TPtrC8 TDesC8::Left(TInt aLength) const
{
	return TPtrC8(Ptr(), Min(Length(), aLength));
}

TPtrC8 TDesC8::Right(TInt aLength) const
{
	return TPtrC8(Ptr()+Length()-aLength, Min(Length(), aLength));
}

TPtrC8 TDesC8::Mid(TInt aPos) const
{
	return TPtrC8(Ptr()+aPos, Length()-aPos);
}

TPtrC8 TDesC8::Mid(TInt aPos, TInt aLength) const
{
	return TPtrC8(Ptr()+aPos, Min(aLength, Length()-aPos));
}

//HBufC8* TDesC8::Alloc() const
//{
//	HBufC8* buf = HBufC8::New(Length());
//	*buf = *this;
//	return buf;
//}

//void TDesC8::CopyTo(char* aStr) const
//{
//	Mem::Copy(aStr, Ptr(), Length());
//	aStr[Length()] = '\0';
//}

//const TUint8& TDesC8::AtC(TInt aIndex) const
//{
//	return (*this)[aIndex];
//}

//-----------------------------------------------------------------------------
// TPtrC8
//-----------------------------------------------------------------------------
TPtrC8::TPtrC8()
:	TDesC8(DES_TYPE_TPTRC,0),
	iPtr(NULL)
{
}

TPtrC8::TPtrC8(const TDesC8& aDes)
:	TDesC8(DES_TYPE_TPTRC,aDes.Length()),
	iPtr(aDes.Ptr())
{
}

TPtrC8::TPtrC8(const TUint8* aString)
:	TDesC8(DES_TYPE_TPTRC, strlen((const char*)aString)),
iPtr(aString)
{
}

TPtrC8::TPtrC8(const TUint8* aBuf,TInt aLength)
:	TDesC8(DES_TYPE_TPTRC,aLength), iPtr(aBuf)
{
}

void TPtrC8::Set(const TUint8* aBuf,TInt aLength)
{
	iPtr=aBuf;
	DoSetLength(aLength);
}

void TPtrC8::Set(const TDesC8& aDes)
{
	iPtr=aDes.Ptr();
	DoSetLength(aDes.Length());
}

void TPtrC8::Set(const TPtrC8& aPtr)
{
	iPtr=aPtr.Ptr();
	DoSetLength(aPtr.Length());
}

TPtrC8& TPtrC8::operator=(const TPtrC8& aDes)
{
	iPtr=aDes.Ptr();
	DoSetLength(aDes.Length());
	return *this;
}

//-----------------------------------------------------------------------------
// TDes8
//-----------------------------------------------------------------------------
void TDes8::Zero()
{
	DoSetLength(0);
}

void TDes8::ZeroTerminate()
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

const TUint8* TDes8::PtrZ()
{
	ZeroTerminate();
	return Ptr();
}

void TDes8::SetLength(TInt aLength)
{
	if (aLength > MaxLength())
	{
		User::Leave(KErrTooBig);
	}
	DoSetLength(aLength);
}

void TDes8::SetMax()
{
	DoSetLength(iMaxLength);
}

void TDes8::Copy(const TDesC8& aDes)
{
	int len = Min(MaxLength(), aDes.Length());
	for (int i=0; i<len; i++)
	{
		(*this)[i] = aDes[i];
	}
	SetLength(len);
}

void TDes8::Copy(const TUint8 *aBuf,TInt aLength)
{
	int len = Min(MaxLength(), aLength);
	for (int i=0; i<len; i++)
	{
		(*this)[i] = aBuf[i];
	}
	SetLength(len);
}

void TDes8::Copy(const TUint8 *aString)
{
	int len = Min(MaxLength(), strlen((const char*)aString));
	for (int i=0; i<len; i++)
	{
		(*this)[i] = aString[i];
	}
	SetLength(len);
}

void TDes8::Copy(const TDesC16& aDes)
{
	int len = Min(MaxLength(), aDes.Length());
	for (int i=0; i<len; i++)
	{
		(*this)[i] = (TUint8)aDes[i];
	}
	SetLength(len);
}

void TDes8::CopyLC(const TDesC8& aDes)
{
	int len = Min(MaxLength(), aDes.Length());
	for (int i=0; i<len; i++)
	{
		(*this)[i] = (aDes[i] >= 'A' && aDes[i] <= 'Z') ? (aDes[i]+'a'-'A') : aDes[i];
	}
	SetLength(len);
}

void TDes8::CopyUC(const TDesC8& aDes)
{
	int len = Min(MaxLength(), aDes.Length());
	for (int i=0; i<len; i++)
	{
		(*this)[i] = (aDes[i] >= 'a' && aDes[i] <= 'z') ? (aDes[i]+'A'-'a') : aDes[i];
	}
	SetLength(len);
}

void TDes8::Append(TChar aChar)
{
	if (Length() >= MaxLength())
	{
		return;
	}
	(*this)[Length()] = aChar;
	SetLength(Length() + 1);
}

void TDes8::Append(const TDesC8& aDes)
{
	TInt len1 = Length();
	TInt len2 = Min(MaxLength()-len1, aDes.Length());
	for (TInt i=0; i<len2; i++)
	{
		(*this)[len1+i] = aDes[i];
	}
	SetLength(len1 + len2);
}

void TDes8::Append(const TDesC16& aDes)
{
	int len = Min(aDes.Length(), MaxLength()-Length());
	for (int i=0; i<len; i++)
	{
		(*this)[Length()+i] = (TUint8)aDes[i];
		if (aDes[i] & 0xff00)
		{
			SetLength(Length()+1);
			(*this)[Length()+i] = (TUint8)(aDes[i]>>8);
		}
	}
	SetLength(Length() + len);
}

void TDes8::Append(const TUint8* aBuf, TInt aLength)
{
	int len = Min(aLength, MaxLength()-Length());
	for (int i=0; i<len; i++)
	{
		(*this)[Length()+i] = aBuf[i];
	}
	SetLength(Length()+aLength);
}

void TDes8::AppendFill(TChar aChar, TInt aLength)
{
	(*this)[Length()] = aChar;
	SetLength(Length() + 1);
}

void TDes8::AppendNum(TInt aVal)
{
	TBuf8<16> buf;
	buf.Num(aVal);
	Append(buf);
}

void TDes8::Num(TInt aVal)
{
	char str[16];
	sprintf(str, "%d", aVal);
	Copy((const TUint8*)str);
}

void TDes8::Delete(TInt aPos,TInt aLength)
{
	if (aPos >= Length()) { return; }

	if ((aPos+aLength) >= Length())
	{
		DoSetLength(aPos);
		return;
	}

	DoSetLength(Length()-aLength);
	Mem::Copy(WPtr()+aPos, WPtr()+aPos+aLength, Length());
}

void TDes8::Fill(TChar aChar)
{
	for (int i=0; i<Length(); i++)
	{
		(*this)[i] = (TUint8)aChar;
	}
}

void TDes8::Fill(TChar aChar, TInt aLength)
{
	int len = Min(MaxLength(), aLength);
	for (int i=0; i<len; i++)
	{
		(*this)[i] = (TUint8)aChar;
	}
	DoSetLength(len);
}

void TDes8::Insert(TInt aPos, const TDesC8& aDes)
{
	int i, len = Min(MaxLength()-aPos, aDes.Length());
	for (i=aPos+len; i<MaxLength(); i++)
	{
		(*this)[i] = (*this)[i-len];
	}
	for (i=0; i<len; i++)
	{
		(*this)[aPos+i] = aDes[i];
	}
	DoSetLength(aPos + len);
}

void TDes8::LowerCase()
{
	for (int i=0; i<Length(); i++)
	{
		(*this)[i] = ((*this)[i] >= 'A' && (*this)[i] <= 'Z') ? ((*this)[i]+'a'-'A') : (*this)[i];
	}
}

void TDes8::UpperCase()
{
	for (int i=0; i<Length(); i++)
	{
		(*this)[i] = ((*this)[i] >= 'a' && (*this)[i] <= 'z') ? ((*this)[i]+'A'-'a') : (*this)[i];
	}
}

void TDes8::Format(TRefByValue<const TDesC8> aFmt, ...)
{
	Zero();

	const TDesC8* des = (const TDesC8*)( * ((int*)&aFmt) );

	const char* fmtStr = (const char*)des->Ptr();

//	va_list list;
//	va_start(list, aFmt);

	char* pArg = NULL;	//等价于原来的va_list 
	char c;
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
			case 'x':
				//AppendNum(*((int*)pArg));
				Append(_L8("FMT_X"));
				pArg += sizeof(int);
				break;
			case '.':
				++fmtStr;
				if ( (*fmtStr < '0' || *fmtStr > '9') || *(fmtStr+1) != 'f')
				{
					Append(_L8("FMT_ERR"));
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
				Append(_L8("FMT_ERR"));
				break;
			} 
		}
		++fmtStr;
	} while (*fmtStr != '\0'); 
	pArg = NULL;	//等价于va_end(pArg)
//	va_end( pArg ); // 将pArg置为NULL
}

void TDes8::Replace(TInt aPos,TInt aLength,const TDesC8& aDes)
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

// end of file `e32des8.cpp'
