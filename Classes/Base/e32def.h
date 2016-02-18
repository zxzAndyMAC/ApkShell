// e32def.h
//
// Created 2005-04-08, ER1C
//

#ifndef __DEF_H__
#define __DEF_H__

#include <stdlib.h>

#define TRUE	1
#define FALSE	0
#ifndef NULL
#	define NULL	0
#endif

typedef unsigned char		TUint8;
typedef unsigned short		TUint16;
typedef unsigned int		TUint;
typedef unsigned int		TUint32;
typedef unsigned long       TUlong;
typedef char				TInt8;
typedef short				TInt16;
typedef int					TInt;
typedef int					TInt32;
typedef int					TBool;
typedef long				TLong;
typedef void				TAny;
typedef unsigned int		TChar;
typedef unsigned char		TText8;
typedef unsigned short		TText16;
typedef float				TFloat;
typedef double				TReal;

#define _L8(a)				(TPtrC8((const TText8 *)(a)))
#define _L16(a)				(TPtrC16((const TText16 *)L ## a))
#define _LIT8(name,s)		const static TLitC8<sizeof(s)> name={sizeof(s)-1,s}
#define _LIT16(name,s)		const static TLitC16<sizeof(L##s)/2> name={sizeof(L##s)/2-1,(const TText16)L##s}

// assume unicode:
#ifndef _UNICODE
#define _UNICODE
#else
//#undef _UNICODE
#endif

#ifdef _UNICODE
#define TText				TText16
#define _LIT(name,s)		_LIT16(name,s)
#define _OL(s)				_L16(s)
#define textlen(a)			wcslen(a)
#else
#define TText				TText8
#define _LIT(name,s)		_LIT8(name,s)
#define _OL(s)				_L8(s)
#define textlen(s)			strlen((const char*)s)
#endif

#ifdef ANDROID
#define EXPORT_C //__declspec(dllexport)	
#define IMPORT_C //__declspec(dllexport)
#include <wchar.h>	
#include "../DexMarcoDef.h"
#elif defined(CC_TARGET_OS_IPHONE)
#define EXPORT_C //__declspec(dllexport)
#define IMPORT_C //__declspec(dllexport)
#include <wchar.h>
#else
#define EXPORT_C //__declspec(dllexport)
#define IMPORT_C __declspec(dllexport)
#endif

#if defined(_WIN32_WCE)||defined(ANDROID)	
#define THROW_EXCEPTION_NULL(a) return NULL
#define THROW_EXCEPTION_ERR(a)  return (a)
#define THROW_EXCEPTION_VOID(a) return
#else 	// else
#define THROW_EXCEPTION_NULL(a) throw TException(a)
#define THROW_EXCEPTION_ERR(a)  throw TException(a)
#define THROW_EXCEPTION_VOID(a) throw TException(a)
#endif	// endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)			{ if(p) { delete (p);		(p)=NULL; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete [] (p);		(p)=NULL; } }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)			{ if(p) { (p)->Release();	(p)=NULL; } }
#endif

#endif	//__DEF_H__

// end of file `e32def.h'
