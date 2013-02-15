/************************************************************************/
/*                                                                      */
/*	dutil.h  --	Interface to Digilent Utility Function Library			*/
/*                                                                      */
/************************************************************************/
/*	Author:		Gene Apperson											*/
/*	Copyright 2004, Digilent Inc.										*/
/************************************************************************/
/*  File Description:                                                   */
/*                                                                      */
/*	This file contains interface declarations for the functions and		*/
/*	data types in the Digilent Utility Function Library. This library	*/
/*	contains general purpose functions used by many Digilent			*/
/*	applications programs.												*/
/*                                                                      */
/************************************************************************/
/*  Revision History:                                                   */
/*                                                                      */
/*	10/06/2004(GeneA): Created by copying from the old winutil.h and	*/
/*		genutil.h header files.											*/
/*	05/11/2005(JPederson):  Updated with registry functions				*/
/*	03/27/2008(GeneA): added FIsAlpha and FIsHexDigit					*/
/*                                                                      */
/************************************************************************/

#if !defined(DUTIL_INC)
#define DUTIL_INC

#if !defined(DTYPES_INC)
#include <dtypes.h>
#endif

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Type Declarations							*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Object Class Declarations					*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Variable Declarations						*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Procedure Declarations						*/
/* ------------------------------------------------------------ */

void	CopyMem(int cb, BYTE * pvDst, BYTE * pvSrc);
void	MoveMem(int cb, BYTE * pvDst, BYTE * pvSrc);
void	FillMem(int cb, BYTE * pb, BYTE bVal);
void	FillMem(int cw, WORD * pw, WORD wVal);
void	FillMem(int cd, DWORD * pd, DWORD dVal);
bool	FCmpMem(int cb, BYTE * pvDst, BYTE * pvSrc);
int		IdwOfDw(DWORD dw, DWORD * rgdw, int cdwMac);
void	InsDwAtIdw(DWORD dw, DWORD * rgdw, int idw, int cdwMac);
void	DelDwAtIdw(DWORD * rgdw, int idw, int cdwMac);
DWORD	DwPackBytes(int cb, BYTE * rgb);
void	UnpackBytes(int cb, BYTE * rgb, DWORD dw);
char *	SzTitleOfName(char * szName);
char *	SzExtOfName(char * szName);
void	MakeAbsPath(char * szAbs, char * szRoot, char * szRel);
void	MakeRelPath(char * szRel, char * szRoot, char * szAbs);
bool	FCmpDriveSpec(char * szA, char * szB);
bool	FCmpPathLevel(char * szA, char * szB);
bool	FIsDriveSpec(char * sz);
bool	FIsParentDir(char * szRel);
bool	FIsCurDir(char * szRel);
bool	FCreatePath(char * szPath);
char *	SzSkipDriveSpec(char * sz);
char *	SzSkipPathLevel(char * sz);
char *	SzRemovePathLevel(char * szStart, char * szEnd);
char *	SzAppendDriveSpec(char * szDst, char * szSrc);
char *	SzAppendPathLevel(char * szDst, char * szSrc);
char *	SzAppendPathSeparator(char * szPath);
int		IszOfSz(char * sz, int csz, char * rgsz[]);
int		IszOfSzI(char * sz, int csz, char * rgsz[]);
int		IszSortPos(char * sz, char * rgsz[], int cszMac);
int		IchFirstDif(char * szTst, char * szPat);
char *	PchMatchSz(char * szBuf, char * szPat);
int		CchOfSz(char * sz);
int		IchOfCh(char * sz, char ch);
char *	PchOfCh(char * sz, char ch);
bool	FIsNulSz(char * sz);
char *	SzCopy(char * szDst, char * szSrc);
char *	SzAppend(char * szDst, char * szSrc);
char *	SzEnd(char * sz);
char *	SzReplaceCh(char * sz, char chOld, char chNew);
WCHAR *	WszFromSz(WCHAR * wsz, char * sz);
char *	SzFromWsz(char * sz, WCHAR * wsz);
int		CompareSz(char * szA, char * szB);
int		CompareSzI(char * szA, char * szB);
int		CompareChI(char chA, char chB);	
char	ChMakeUpper(char ch);
char *	SzMakeUpper(char * sz);
char *	SzSkipSpace(char * sz);
bool	FIsAlpha(char ch);
bool	FIsSymChar(char ch);
bool	FIsDigit(char ch);
bool	FIsHexDigit(char ch);
bool	FIsFileChar(char ch);
bool	FIsPathChar(char ch);
bool	FIsProjChar(char ch);


//new 5/11/05
bool	FGetRegString(HKEY hkey, char* szRegPath, char * szRegString);
bool	FGetRegDword(HKEY hkey, char* szRegPath, DWORD * pidDword);
bool	FSetRegString(HKEY hkey, char* szRegPath, char * szRegString);
bool	FSetRegDword(HKEY hkey, char* szRegPath, DWORD idDword);
bool	FOpenRegKey(HKEY hkeyRoot, char* szRegPath, HKEY *hkey);

/* ------------------------------------------------------------ */

#endif

/************************************************************************/
