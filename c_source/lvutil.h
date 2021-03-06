/*
   lvutil.h -- support functions for LabVIEW integration

   Version 1.17, Dec 8th, 2018

   Copyright (C) 2002-2018 Rolf Kalbermatter

   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
   following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the
	   following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
       following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of SciWare, James Kring, Inc., nor the names of its contributors may be used to endorse
	   or promote products derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef _lvUtil_H
#define _lvUtil_H

#ifdef __cplusplus
extern "C" {
#endif

    /* Possible values for ProcessorType */
#define kM68000		1		/* OBSOLETE */
#define kX86		2
#define kSparc		3
#define kPPC		4
#define kPARISC		5
#define kMIPS		6
#define kDECAlpha	7		/* OBSOLETE */
#define kPIC		8
#define kARM		9
#define kX64		10

#if defined(macintosh) || defined(__PPCC__) || defined(THINK_C) || defined(__SC__) || defined(__MWERKS__) || defined(__APPLE_CC__)
 #if defined(__powerc) || defined(__ppc__)
  #define ProcessorType	kPPC
  #define BigEndian 1
 #elif defined(__i386__)
  #define ProcessorType	kX86
  #define BigEndian 0
 #elif defined(__x86_64)
  #define ProcessorType	kX64
  #define BigEndian 0
 #else
  #define ProcessorType	kM68000
  #define BigEndian 1
 #endif
 #define MacOS 1
 #define Win32 0
 #ifdef __APPLE_CC__
  #define MacOSX 1
 #endif
#elif defined(WIN32) || defined(_WIN32)  || defined(__WIN32__) || defined(_WIN32_WCE)
 #ifdef _M_PPC
  #define ProcessorType	kPPC
 #elif defined(_M_IX86)
  #define ProcessorType	kX86
 #elif defined(_M_X64)
  #define ProcessorType	kX64
 #elif defined(_M_ALPHA)
  #define ProcessorType	kDECAlpha
 #elif defined(_ARM_)
  #define ProcessorType 	kARM
 #else
  #error "We don't know the ProcessorType architecture"
 #endif
 #define Win32 1
 #if defined(_DEBUG) || defined(_CVI_DEBUG_)
  #define DEBUG 1
 #endif
 #define BigEndian 0
 #if EMBEDDED
  #define Pharlap 1
 #endif
 #define _WIN32_WINNT 0x0501
#elif defined(linux) || defined(__linux) || defined(__linux__)
 #if defined(i386)
  #define ProcessorType	kX86
  #define BigEndian		0
 #elif defined(__alpha)
  #define ProcessorType	kDECAlpha
  #define BigEndian		0
 #elif defined(powerpc)
  #define ProcessorType	kPPC
  #define BigEndian		1
 #elif defined(sparc)
  #define ProcessorType	kSparc
  #define BigEndian		1
 #elif defined(mips)
  #define ProcessorType	kMIPS
  #define BigEndian		1
 #elif defined(arm) || defined(__arm__)
  #define ProcessorType	kARM
  #define BigEndian		0
 #elif defined(__x86_64__)
  #define ProcessorType	kX64
  #define BigEndian		0
 #else
  #error "Unknown Linux platform"
 #endif
 #define Unix 1
 #define Win32 0
 #define HAVE_FCNTL
 #define HAVE_ICONV
// #define HAVE_WCRTOMB
// #define HAVE_MBRTOWC
#elif defined(__VXWORKS__)
 #define Unix 1
 #define VxWorks 1
 #define Win32 0
 #if defined (__ppc)
  #define ProcessorType	kPPC
  #define BigEndian 1
 #else
  #define ProcessorType	kX86
  #define BigEndian 0
 #endif
#else
 #error No target defined
#endif

#if Win32
 #define LibAPI(retval) extern __declspec(dllexport) retval
#else
 #define LibAPI(retval)	__attribute__((visibility ("default"))) extern retval
#endif

#if defined(DEBUG)
 #if Win32
  #if defined(_CVI_DEBUG_)
   #define DoDebugger()
  #elif _MSC_VER >= 1400
	 #define DoDebugger()   __debugbreak()
  #else
   #define DoDebugger()    {__asm int 3}
  #endif
 #elif MacOS
  #define DoDebugger()    Debugger()
 #else
  #define DoDebugger()
 #endif
 #define DEBUGPRINTF(args)      DbgPrintf args
#else
 #define DoDebugger()
 #define DEBUGPRINTF(args)
/* long DebugPrintf(char* fmt, ...);
 long DebugPrintf(char* fmt, ...)
 {
   return 0;
 }
*/
#endif

#ifndef Unused
/* The macro Unused can be used to avoid compiler warnings for
unused parameters or locals. */
#   ifdef __cplusplus
/* This implementation of Unused is safe for const parameters. */
#       define Unused(var_or_param)    _Unused((const void *)&var_or_param)
        inline void _Unused(const void *) {}
#   elif MSWin
/* This implementation of Unused is not safe for const parameters. */
#       define Unused(var_or_param)    var_or_param=var_or_param
#   else
#       define Unused(var_or_param)    var_or_param=var_or_param
#   endif
#endif /* Unused */

typedef signed char			int8;
typedef unsigned char		uInt8;
typedef uInt8				uChar;
typedef signed short		int16;
typedef unsigned short		uInt16;
typedef signed int			int32;
typedef unsigned int		uInt32;
#if Win32
typedef signed __int64		int64;
typedef unsigned __int64	uInt64;
#else
typedef signed long long 	int64;
typedef unsigned long long 	uInt64;
#endif
typedef float				float32;
typedef double				float64;

typedef uInt8				LVBoolean;

#include <wchar.h>

#define LV_FALSE	0
#define LV_TRUE		1

#define Private(T)  typedef struct T##_t { void *p; } *T
#define PrivateH(T)  struct T##_t; typedef struct T##_t **T

typedef enum {  iB=1, iW, iL, iQ, uB, uW, uL, uQ, fS, fD, fX, cS, cD, cX } NumType;

typedef int32           MgErr;

typedef int32           Bool32;

#if ProcessorType==kX64
#define uPtr uQ
#else
#define uPtr uL
#endif

/*** The Support Manager ***/

#define HiNibble(x)		(uInt8)(((x) >> 4) & 0x0F)
#define LoNibble(x)		(uInt8)((x) & 0x0F)
#define HiByte(x)		((uInt8)((uInt16)(x) >> 8))
#define LoByte(x)		((uInt8)(x))
#define Word(hi,lo)		(((uInt16)(hi) << 8) | ((uInt16)(lo & 0xFF)))
#define Hi16(x)			((uInt16)((uInt32)(x) >> 16))
#define Lo16(x)			((uInt16)(x))
#define Long(hi,lo)		(((uInt32)(hi) << 16) | ((uInt32)(lo & 0xFFFF)))
#define Hi32(x)			((uInt32)((uInt64)(x) >> 32))
#define Lo32(x)			((uInt32)(x))
#define Quad(hi,lo)		(((uInt64)(hi) << 32) | ((uInt64)(lo & 0xFFFFFFFF)))

#define Cat4Chrs(c1,c2,c3,c4)   (((int32)(uInt8)(c1)<<24)|((int32)(uInt8)(c2)<<16)|((int32)(uInt8)(c3)<<8)|((int32)(uInt8)(c4)))
#define Cat2Chrs(c1,c2)         (((int16)(uInt8)(c1)<<8)|((int16)(uInt8)(c2)))

#define Swap16(x)       (uInt16)(((x & 0xff) >> 8) | (x << 8))
#define Swap32(x)       (uInt32)(((x & 0xff000000) >> 24) | ((x & 0x00ff0000) >> 8) | ((x & 0x0000ff00) << 8) | (x << 24))

#if BigEndian
#define RTToL(c1,c2,c3,c4)  Cat4Chrs(c1,c2,c3,c4)
#define RTToW(c1,c2)        Cat2Chrs(c1,c2)
#define ConvertBE16(x)		x
#define ConvertBE32(x)		x
#define ConvertLE16(x)		Swap16(x)
#define ConvertLE32(x)		Swap32(x)
#else
#define RTToL(c1,c2,c3,c4)  Cat4Chrs(c4,c3,c2,c1)
#define RTToW(c1,c2)        Cat2Chrs(c2,c1)
#define ConvertBE16(x)		Swap16(x)
#define ConvertBE32(x)		Swap32(x)
#define ConvertLE16(x)		x
#define ConvertLE32(x)		x
#endif

enum {                  /* Manager Error Codes */
    mgNoErr=0,
#if !MacOS && !QTMLIncl
    noErr = mgNoErr,
#endif
    mgArgErr=1,
    mFullErr,           /* Memory Mgr   2-3     */
    mZoneErr,

    fEOF,               /* File Mgr     4-12    */
    fIsOpen,
    fIOErr,
    fNotFound,
    fNoPerm,
    fDiskFull,
    fDupPath,
    fTMFOpen,
    fNotEnabled,

    rFNotFound,         /* Resource Mgr 13-15   */
    rAddFailed,
    rNotFound,

    iNotFound,          /* Image Mgr    16-17   */
    iMemoryErr,

    dPenNotExist,       /* Draw Mgr     18      */

    cfgBadType,         /* Config Mgr   19-22   */
    cfgTokenNotFound,
    cfgParseError,
    cfgAllocError,

    ecLVSBFormatError,  /* extCode Mgr  23      */
    ecLVSBOffsetError,  /* extCode Mgr  24      */
    ecLVSBNoCodeError,  /* extCode Mgr  25      */

    wNullWindow,        /* Window Mgr   26-27   */
    wDestroyMixup,

    menuNullMenu,       /* Menu Mgr     28      */

    pAbortJob,          /* Print Mgr    29-35   */
    pBadPrintRecord,
    pDriverError,
    pWindowsError,
    pMemoryError,
    pDialogError,
    pMiscError,

    dvInvalidRefnum,    /* Device Mgr   36-41   */
    dvDeviceNotFound,
    dvParamErr,
    dvUnitErr,
    dvOpenErr,
    dvAbortErr,

    bogusError,         /* generic error 42 */
    cancelError,        /* cancelled by user 43 */

    OMObjLowErr,        /* object message dispatcher errors 44-52 */
    OMObjHiErr,
    OMObjNotInHeapErr,
    OMOHeapNotKnownErr,
    OMBadDPIdErr,
    OMNoDPinTabErr,
    OMMsgOutOfRangeErr,
    OMMethodNullErr,
    OMUnknownMsgErr,

    mgNotSupported,

    ncBadAddressErr,        /* Net Connection errors 54-66 */
    ncInProgress,
    ncTimeOutErr,
    ncBusyErr,
    ncNotSupportedErr,
    ncNetErr,
    ncAddrInUseErr,
    ncSysOutOfMem,
    ncSysConnAbortedErr,    /* 62 */
    ncConnRefusedErr,
    ncNotConnectedErr,
    ncAlreadyConnectedErr,
    ncConnClosedErr,        /* 66 */

	amInitErr,				/* (Inter-)Application Message Manager 67- */

	occBadOccurrenceErr,	/* 68  Occurrence Mgr errors */
	occWaitOnUnBoundHdlrErr,
	occFunnyQOverFlowErr,

	fDataLogTypeConflict,	/* 71 */
	ecLVSBCannotBeCalledFromThread, /* ExtCode Mgr	72*/
	amUnrecognizedType,
	mCorruptErr,
	ecLVSBErrorMakingTempDLL,
	ecLVSBOldCIN,			/* ExtCode Mgr	76*/

	dragSktNotFound,		/* Drag Manager 77 - 80*/
	dropLoadErr,
	oleRegisterErr,
	oleReleaseErr,

	fmtTypeMismatch,		/* String processing (printf, scanf) errors */
	fmtUnknownConversion,
	fmtTooFew,
	fmtTooMany,
	fmtScanError,

	ecLVSBFutureCIN,		/* ExtCode Mgr	86*/

	lvOLEConvertErr,
	rtMenuErr,
	pwdTampered,			/* Password processing */
	LvVariantAttrNotFound,		/* LvVariant attribute not found 90-91*/
	LvVariantTypeMismatch,		/* Cannot convert to/from type */

	axEventDataNotAvailable,	/* Event Data Not Available 92-96*/
	axEventStoreNotPresent,		/* Event Store Not Present */
	axOccurrenceNotFound,		/* Occurence Not Found */
	axEventQueueNotCreated,		/* Event Queue not created */
	axEventInfoNotAvailable,	/* Event Info is not available */

	oleNullRefnumPassed,		/* Refnum Passed is Null */

	omidGetClassGUIDErr,		/* Error retrieving Class GUID from OMId 98-100*/
	omidGetCoClassGUIDErr,		/* Error retrieving CoClass GUID from OMId */
	omidGetTypeLibGUIDErr,		/* Error retrieving TypeLib GUID from OMId */

	appMagicBad,				/* bad built application magic bytes */

	iviInvalidDowncast,         /* IVI Invalid downcast*/
	iviInvalidClassSesn,		/* IVI No Class Session Opened */

	maxErr,						/* max manager 104-107 */
	maxConfigErr,				/* something not right with config objects */
	maxConfigLoadErr,			/* could not load configuration */
	maxGroupNotSupported,

	ncSockNotMulticast,			/* net connection multicast specific errors 108-112 */
	ncSockNotSinglecast,
	ncBadMulticastAddr,
	ncMcastSockReadOnly,
	ncMcastSockWriteOnly,

	ncDatagramMsgSzErr,			/* net connection Datagram message size error 113 */

	bufferEmpty,				/* CircularLVDataBuffer (queues/notifiers) */
	bufferFull,					/* CircularLVDataBuffer (queues/notifiers) */
	dataCorruptErr,				/* error unflattening data */

	requireFullPathErr,			/* supplied folder path where full file path is required  */
	folderNotExistErr,			/* folder doesn't exist */

	ncBtInvalidModeErr,			/* invalid Bluetooth mode 119 */
	ncBtSetModeErr,				/* error setting Bluetooth mode 120 */

	mgBtInvalidGUIDStrErr,		/* The GUID string is invalid 121 */

	rVersInFuture,			/* Resource file is a future version 122 */

	mgErrSentinel,		/* 123 */

	mgPrivErrBase = 500,	/* Start of Private Errors */
	mgPrivErrLast = 599,	/* Last allocated in Error DB */


	kAppErrorBase = 1000,	/* Start of application errors */
	kAppInvalidEvent = 1325, /* Invalid event refnum */
	kAppLicenseErr = 1380,	/* Failure to check out license error */
	kAppCharsetConvertErr =1396, /* could not convert text from charset to charset */
	kAppErrorLast = 1399	/* Last allocated in Error DB */
};

typedef struct
{
    int16 v;
    int16 h;
} LVPoint;

typedef uChar        Str255[256], *PStr, *CStr, *UPtr, **UHandle, **PStrHandle;
typedef const uChar  *ConstCStr, *ConstPStr, *ConstUPtr, ConstStr255[256];

#define PStrBuf(b)  (&((PStr)(b))[1])
#define PStrLen(b)  (((PStr)(b))[0])    /* # of chars in string */
#define PStrSize(b) (PStrLen(b)+1)      /* # of bytes including length */

int32 CToPStr(ConstCStr src, PStr dest);
int32 PStrCpy(PStr d, ConstPStr s);
int32 StrCpy(CStr t, const CStr s);
int32 StrNCpy(CStr t, const CStr s, int32 l);
int32 StrLen(ConstCStr str);
int32 StrCmp(ConstCStr str1, ConstCStr str2);
int32 StrNCmp(ConstCStr, ConstCStr, size_t);
int32 StrNCaseCmp(ConstCStr, ConstCStr, size_t);

/** @brief Concatenated Pascal string types. */
typedef struct {
    int32	cnt;		/* number of pascal strings that follow */
    uChar	str[1];		/* cnt bytes of concatenated pascal strings */
} CPStr, *CPStrPtr, **CPStrHandle;

typedef struct {
    int32   cnt; 
    uChar   str[1];
} LStr, *LStrPtr, **LStrHandle;
typedef LStr const* ConstLStrP;
typedef LStr const*const* ConstLStrH;

#define LStrLen(p)  ((p)->cnt)
#define LStrBuf(p)  ((p)->str)

#define LStrLenH(h) ((h) ? (*(h))->cnt : 0)
#define LStrBufH(h) ((h) ? (*(h))->str : NULL)

/*** Concatenated Pascal String Support Functions ***/
#define CPStrLen		LStrLen			/* concatenated Pascal vs. LabVIEW strings */
#define CPStrBuf		LStrBuf			/* concatenated Pascal vs. LabVIEW strings */

typedef struct {
    int32   cnt;
    uChar   str[256];
} LStr256;

typedef struct ATIME128 {
	union {
	  struct {
#if BigEndian
		int32 valHi;
	    uInt32 valLo;
	    uInt32 fractHi;
	    uInt32 fractLo;
#else
	    uInt32 fractLo;
	    uInt32 fractHi;
	    uInt32 valLo;
		int32 valHi;
#endif
	  } p;
	  struct {
#if BigEndian
	    int64 val;
	    uInt64 fract;
#else
	    uInt64 fract;
		int64 val;
#endif
	  } f;
	} u;
} ATime128, *ATime128Ptr;

/* Magic Cookies */
typedef uInt32 MagicCookie;

struct MagicCookieJarStruct;
typedef struct MagicCookieJarStruct MCJar, **MagicCookieJar;
typedef UPtr MagicCookieInfo;

#define kNotAMagicCookieJar ((MagicCookieJar) 0)

typedef struct {
	int32		count;		/* number of cookies that follow */
	MagicCookie cookie[1];	/* cookies */
} MagicCookieList, *MagicCookieListPtr, **MagicCookieListHandle;

MagicCookieJar MCNewBigJar(uInt32 size);
MgErr MCDisposeJar(MagicCookieJar);
MagicCookie MCNewCookie(MagicCookieJar, MagicCookieInfo);
MgErr MCDisposeCookie(MagicCookieJar, MagicCookie, MagicCookieInfo);
MgErr MCGetCookieInfo(MagicCookieJar, MagicCookie, MagicCookieInfo);
MgErr MCGetCookieInfoPtr(MagicCookieJar, MagicCookie, MagicCookieInfo*);
MgErr MCGetCookieList(MagicCookieJar, MagicCookieListHandle*);
Bool32 MCIsACookie(MagicCookieJar, MagicCookie);

/* Runtime Cleanup support */
enum {		/* cleanup modes (when to call cleanup proc) */
	kCleanRemove,
	kCleanExit,				/* only when LabVIEW exits */
	kCleanOnIdle,			/* whenever active vi goes idle */
	kCleanAfterReset,		/* whenever active vi goes idle after a reset */
	kCleanOnIdleIfNotTop,	/* whenever active vi goes idle if active vi is not current vi */
	kCleanAfterResetIfNotTop/* whenever active vi goes idle after a reset if active vi is not current vi */
	};
typedef int32	(*CleanupProcPtr)(UPtr);
int32 RTSetCleanupProc(CleanupProcPtr, UPtr, int32);

/* File Manager */
typedef enum _FMFileType {

	kInvalidType	= 0,
	kUnknownFileType= RTToL('?','?','?','?'),
    kTextFileType	= RTToL('T','E','X','T'),
    kLinkFileType	= RTToL('s','l','n','k'),
	/** Typical directory types */
	kHardDiskDirType= RTToL('h','d','s','k'),
	kFloppyDirType	= RTToL('f','l','p','y'),
	kNetDriveDirType= RTToL('s','r','v','r')
} FMFileType;

typedef enum  {
	kInvalidCreator	= 0,
	kUnknownCreator = RTToL('?','?','?','?'),
	/** LabVIEW creator type */
	kLVCreatorType	= RTToL('L','B','V','W')
} FMFileCreator;

/** Used for FGetInfo */
typedef struct {			/**< file/directory information record */
	FMFileType type;		/**< system specific file type-- 0 for directories */
	FMFileCreator creator;	/**< system specific file creator-- 0 for directories */
	int32	permissions;	/**< system specific file access rights */
	int32	size;			/**< file size in bytes (data fork on Mac) or entries in folder */
	int32	rfSize;			/**< resource fork size (on Mac only) */
	uInt32	cdate;			/**< creation date */
	uInt32	mdate;			/**< last modification date */
	Bool32	folder;			/**< indicates whether path refers to a folder */
	Bool32	isInvisible;    /**< indicates whether the file is visible in File Dialog */
	LVPoint location;		/**< system specific geographical location */
	Str255	owner;			/**< owner (in pascal string form) of file or folder */
	Str255	group;			/**< group (in pascal string form) of file or folder */
} FInfoRec, *FInfoPtr;

/** Used for FGetInfo, 64-bit version */
typedef uInt32	FGetInfoWhich;
enum {
	kFGetInfoType			= 1L << 0,
	kFGetInfoCreator		= 1L << 1,
	kFGetInfoPermissions	= 1L << 2,
	kFGetInfoSize			= 1L << 3,
	kFGetInfoRFSize			= 1L << 4,
	kFGetInfoCDate			= 1L << 5,
	kFGetInfoMDate			= 1L << 6,
	kFGetInfoFolder			= 1L << 7,
	kFGetInfoIsInvisible	= 1L << 8,
	kFGetInfoLocation		= 1L << 9,
	kFGetInfoOwner			= 1L << 10,
	kFGetInfoGroup			= 1L << 11,
	kFGetInfoAll			= 0xEFFFFFFFL
};
typedef struct {			/**< file/directory information record */
	FMFileType type;		/**< system specific file type-- 0 for directories */
	FMFileCreator creator;	/**< system specific file creator-- 0 for directories */
	int32	permissions;	/**< system specific file access rights */
	int64	size;			/**< file size in bytes (data fork on Mac) or entries in folder */
	int64	rfSize;			/**< resource fork size (on Mac only) */
	uInt32	cdate;			/**< creation date */
	uInt32	mdate;			/**< last modification date */
	Bool32	folder;			/**< indicates whether path refers to a folder */
	Bool32	isInvisible;    /**< indicates whether the file is visible in File Dialog */
	LVPoint location;		/**< system specific geographical location */
	Str255	owner;			/**< owner (in pascal string form) of file or folder */
	Str255	group;			/**< group (in pascal string form) of file or folder */
} FInfoRec64, *FInfo64Ptr;

/** Used for FGetVolInfo */
typedef struct {
	uInt32	size;			/**< size in bytes of a volume */
	uInt32	used;			/**< number of bytes used on volume */
	uInt32	free;			/**< number of bytes available for use on volume */
} VInfoRec;

/** Used with FListDir2 */
typedef struct {
	uInt32 fileFlags;
	FMFileType fileType;
} FMListDetails;

/** @brief Data types used to describe a list of entries from a directory. */
typedef CPStr FDirEntRec, *FDirEntPtr, **FDirEntHandle;

/** Type Flags used with FMListDetails */
#define kIsFile				0x01
#define kRecognizedType		0x02
#define kIsLink				0x04
#define kFIsInvisible		0x08
#define kIsTopLevelVI		0x10	/* Used only for VIs in archives */
#define kErrGettingType		0x20	/* error occurred getting type info */
#if Mac
#define kFIsStationery		0x40
#endif
#define kIsCompressed		0x80
    
enum { openReadWrite, openReadOnly, openWriteOnly, openWriteOnlyTruncate }; /* open modes */
enum { denyReadWrite, denyWriteOnly, denyNeither}; /* deny modes */

enum { fStart = 1, fEnd, fCurrent };	/* seek modes */
/*
Win32:
#define FILE_BEGIN           0
#define FILE_CURRENT         1
#define FILE_END             2

MacOSX:
enum {
	fsAtMark = 0,
	fsFromStart = 1,
	fsFromLEOF = 2,
	fsFromMark = 3
}

Unix:
SEEK_SET
SEEK_END
SEEK_START
*/
enum { fAbsPath, fRelPath, fNotAPath, fUNCPath, nPathTypes};                            /* path type codes */

Private(File);
typedef struct PATHREF PathRef;
typedef PathRef* Path;
typedef const PathRef* ConstPath;

typedef MagicCookie LVRefNum;
#define kNotARefNum ((LVRefNum)0L)	/* canonical invalid magic cookie */

/* LabVIEW exported functions */
MgErr FRelPath(ConstPath start, ConstPath end, Path relPath);
MgErr FAddPath(ConstPath basePath, ConstPath relPath, Path newPath);
MgErr FAppendName(Path path, ConstPStr name);
MgErr FName(ConstPath path, PStrHandle name);
MgErr FNamePtr(ConstPath path, PStr name);
MgErr FDirName(ConstPath path, Path dir);
MgErr FVolName(ConstPath path, Path vol);
Path FMakePath(Path path, int32 type, ...);
Path FEmptyPath(Path);
Path FNotAPath(Path);
Bool32 FIsAPath(Path path);
Bool32 FIsAbsPath(Path path);
Bool32 FIsEmptyPath(Path path);
MgErr FTextToPath(UPtr str, int32 len, Path* path);
MgErr FPathToText(Path path, LStrPtr lstr);
MgErr FPathToPath(Path *p);
Bool32 FIsAPathOfType(Path path, int32 ofType);
MgErr FGetPathType(Path, int32*);
int32 FDepth(Path path);
MgErr FDisposePath(Path p);

#define FIsAbsPath(path) FIsAPathOfType(path, fAbsPath)
#define FIsRelPath(path) FIsAPathOfType(path, fRelPath)

MgErr FNewRefNum(Path path, File fd, LVRefNum* refnum);
Bool32 FIsARefNum(LVRefNum);
MgErr FRefNumToFD(LVRefNum, File*);
MgErr FDisposeRefNum(LVRefNum);
Bool32 FExists(ConstPath path);
MgErr FGetInfo(ConstPath path, FInfoPtr infop);
MgErr FGetInfo64(ConstPath path, FInfo64Ptr infop, FGetInfoWhich which);
MgErr FSetInfo(ConstPath path, FInfoPtr infop);
MgErr FSetInfo64(ConstPath path, FInfo64Ptr infop);
MgErr FMRead(File fd, int32 inCount, int32* outCount, UPtr buffer);
MgErr FMWrite(File fd, int32 inCount, int32* outCount, UPtr buffer);
MgErr FListDir(ConstPath path, FDirEntHandle list, FMListDetails **);
uInt32 PStrHasRezExt(PStr str);
uInt32 HasRezExt(Path path);

int32 DbgPrintf(CStr fmt, ...);

UPtr DSNewPtr(size_t size);
UPtr DSNewPClr(size_t size);
MgErr DSDisposePtr(UPtr);
UHandle DSNewHandle(size_t size);
UHandle DSNewHClr(size_t size);
MgErr DSSetHandleSize(UHandle, size_t);
int32 DSGetHandleSize(UHandle);
MgErr DSDisposeHandle(UHandle);
MgErr DSCopyHandle(void *ph, const void *hsrc);

UHandle AZNewHandle(size_t size);
UHandle AZNewHClr(size_t size);
MgErr AZSetHandleSize(UHandle, size_t);
int32 AZGetHandleSize(UHandle);
MgErr AZDisposeHandle(UHandle);
MgErr AZCopyHandle(void *ph, const void *hsrc);

void MoveBlock(ConstUPtr ps, UPtr pd, size_t size);

MgErr NumericArrayResize(int32, int32, UHandle*, size_t);

#define Min(a, b)      ((a) < (b)) ? (a) : (b) 
#define Max(a, b)      ((a) > (b)) ? (a) : (b) 

typedef struct
{
	int32 numItems;
	LStrHandle elm[1];
} LStrArrRec, *LStrArrPtr, **LStrArrHdl;

typedef struct
{
	int32 numItems;
	FMListDetails elm[1];
} FileInfoArrRec, *FileInfoArrPtr, **FileInfoArrHdl;

/* Our exported functions */
/**************************/

/* Version string of the zlib library */
LibAPI(void) DLLVersion(uChar*  Version);

/* Convert the path into a string representation for the current platform */
LibAPI(MgErr) LVPath_ToText(Path path, LStrHandle *str);
LibAPI(MgErr) LVPath_FromText(CStr str, int32 len, Path *path, LVBoolean isDir);

/* Check if the file path points to has a resource fork */
LibAPI(MgErr) LVPath_HasResourceFork(Path path, LVBoolean *hasResFork, uInt32 *sizeLow, uInt32 *sizeHigh);

/* List the directory contents with an additional array with flags and file type for each file in the names array */
LibAPI(MgErr) LVPath_ListDirectory(Path folderPath, LStrArrHdl *names, FileInfoArrHdl *fileInfo, int32 flags);
LibAPI(MgErr) LVFile_ListDirectory(LStrHandle folderPath, LStrArrHdl *nameArr, FileInfoArrHdl *typeArr, int32 flags);

/* Windows portion of the flags parameter */
#define kWinFileInfoReadOnly             0x00000001  
#define kWinFileInfoHidden               0x00000002  
#define kWinFileInfoSystem               0x00000004  
#define kWinFileInfoDirectory            0x00000010  
#define kWinFileInfoArchive              0x00000020  
#define kWinFileInfoDevice               0x00000040  
#define kWinFileInfoNormal               0x00000080  
#define kWinFileInfoTemporary            0x00000100  
#define kWinFileInfoSparseFile           0x00000200  
#define kWinFileInfoReparsePoint         0x00000400  
#define kWinFileInfoCompressed           0x00000800  
#define kWinFileInfoOffline              0x00001000  
#define kWinFileInfoNotIndexed           0x00002000  
#define kWinFileInfoEncrypted            0x00004000  

/* Mac extended flags */
#define kMacFileInfoNoDump               0x00000001	   /* do not dump file */
#define	kMacFileInfoImmutable		     0x00000002	   /* file may not be changed */
#define kMacFileInfoCompressed           0x00000020    /* file is hfs-compressed (Mac OS X 10.6+) */
//#define kMacFileInfoSystem               0x00000080	   /* Windows system file bit */
//#define kMacFileInfoSparse               0x00000100	   /* sparse file */
//#define kMacFileInfoOffline	           0x00000200	   /* file is offline */
//#define kMacFileInfoArchive              0x00000800    /* file needs to be archived */
#define kMacFileInfoHidden               0x00008000    /* hint that this item should not be displayed in a GUI (Mac OS X 10.5+) */

typedef struct {        /* off */
	FMFileType type;    /*  0: handled by LabVIEW Type & Creator */
	FMFileType creator; /*  4: handled by LabVIEW Type & Creator */
	uInt32 uid;         /*  8: Unix user id */
	uInt32 gid;         /* 12: Unix group id */
	uInt64 size;        /* 16: file size or file count for directories */
	uInt64 rfSize;      /* 24: resource fork size, 0 on non MacOS platforms */
	ATime128 cDate;     /* 32: Creation date */
	ATime128 mDate;     /* 48: Modification date */
	ATime128 aDate;     /* 64: ast access date */
	uInt16 winFlags;    /* 80: Windows compatible flags */
	uInt16 unixFlags;   /* 82: Unix compatible flags */
	uInt32 macFlags;    /* 84: MacOSX extra file flags */
	uInt32 lvFlags;     /* 88: LabVIEW file flags */
} LVFileInfo;           /* 92: Total length */

/* Retrieve file information from the path */
LibAPI(MgErr) LVPath_FileInfo(Path path, uInt8 write, LVFileInfo *fileInfo);
LibAPI(MgErr) LVFile_FileInfo(LStrHandle path, uInt8 write, LVFileInfo *fileInfo);

/* Creation flags */
#define kLinkSoft       0x00
#define kLinkHard       0x01
#define kLinkDir		0x02

/* Resolution flags */
#define kResolve        0x01
#define kRecursive		0x02

/* Create and read a link */
LibAPI(MgErr) LVPath_CreateLink(Path path, Path target, uInt32 flags);
LibAPI(MgErr) LVFile_CreateLink(LStrHandle path, LStrHandle target, uInt32 flags);

LibAPI(MgErr) LVPath_ReadLink(Path path, Path *target, uInt32 flags, uInt32 *fileFlags);
LibAPI(MgErr) LVFile_ReadLink(LStrHandle path, LStrHandle *target, uInt32 flags, uInt32 *fileFlags);

typedef union
{
	int64 q;
#if BigEndian
	struct
	{
		int32 hi;
		uInt32 lo;
	} l;
#else
	struct
	{
		uInt32 lo;
		int32 hi;
	} l;
#endif
} FileOffset;

enum { /* values for rsrc parameter */
	kOpenFileRsrcData,
	kOpenFileRsrcResource,
	kOpenFileRsrcInfo,
	kOpenFileRsrcDesktop,
	kOpenFileRsrcIndex,
	kOpenFileRsrcComment
};

LibAPI(MgErr) LVPath_OpenFile(LVRefNum *refnum, Path path, uInt32 rsrc, uInt32 openMode, uInt32 denyMode);
LibAPI(MgErr) LVFile_OpenFile(LVRefNum *refnum, LStrHandle path, uInt32 rsrc, uInt32 openMode, uInt32 denyMode);
LibAPI(MgErr) LVFile_CloseFile(LVRefNum *refnum);
LibAPI(MgErr) LVFile_GetSize(LVRefNum *refnum, FileOffset *size);
LibAPI(MgErr) LVFile_SetSize(LVRefNum *refnum, FileOffset *size);
LibAPI(MgErr) LVFile_GetFilePos(LVRefNum *refnum, FileOffset *offs);
LibAPI(MgErr) LVFile_SetFilePos(LVRefNum *refnum, FileOffset *offs, uInt16 mode);
LibAPI(MgErr) LVFile_Read(LVRefNum *refnum, uInt32 inCount, uInt32 *outCount, UPtr buffer);
LibAPI(MgErr) LVFile_Write(LVRefNum *refnum, uInt32 inCount, uInt32 *outCount, UPtr buffer);

LibAPI(MgErr) InitializeFileFuncs(LStrHandle filefunc_def);
LibAPI(MgErr) InitializeStreamFuncs(LStrHandle  filefunc_def, LStrHandle *memory);

LibAPI(MgErr) ZeroTerminateLString(LStrHandle *dest);

#ifdef __cplusplus
}
#endif

#endif /* _lvUtil_H */
