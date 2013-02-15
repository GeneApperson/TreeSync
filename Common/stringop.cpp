/************************************************************************/
/*                                                                      */
/*  stringop.cpp  --  General Purpose String Manipulation Functions		*/
/*                                                                      */
/************************************************************************/
/*	Author: 	Gene Apperson											*/
/*	Copyright 2003, 2004 Digilent Inc.									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*	This module contains general purpose utility functions for			*/
/*	manipulating nul terminates strings.								*/
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*	10/06/2004(GeneA): created by copying functions from genutil.cpp	*/
/*	06/04/2009(JoshP):  Upgraded to VS2008								*/
/*                                                                      */
/************************************************************************/


/* ------------------------------------------------------------ */
/*					Include File Definitions					*/
/* ------------------------------------------------------------ */
#define	_CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include	<windows.h>
#include	<string.h>

#include	"../inc/dtypes.h"
#include	"../inc/dutil.h"

/* ------------------------------------------------------------ */
/*					Local Type Definitions						*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*					 Global Variables							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*					Local Variables								*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*					Forward Declarations						*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*			String Manipulation Routines						*/
/* ------------------------------------------------------------ */
/***	IszOfSz
**
**	Synopsis:
**		isz = IszOfSz(sz, rgsz, csz)
**
**	Input:
**		sz		- string to find
**		rgsz	- array of pointers to strings
**		csz		- count of string pointers in the array
**
**	Output:
**		Returns index of the specified string within the array
**
**	Errors:
**		Returns -1 if the string isn't in the array
**
**	Description:
**		This routine will search an array of pointers to strings
**		for a match on the specified string. If found, the array
**		index of the first match is returned.
*/

int
IszOfSz(char * sz, int csz, char * rgsz[])
	{
	int		isz;

	for (isz = 0; isz < csz; isz++) {
		if (CompareSz(sz, rgsz[isz]) == 0) {
			return isz;
		}
	}

	return -1;

}

/* ------------------------------------------------------------ */
/***	IszOfSzI
**
**	Synopsis:
**		isz = IszOfSzI(sz, rgsz, csz)
**
**	Input:
**		sz		- string to find
**		rgsz	- array of pointers to strings
**		csz		- count of string pointers in the array
**
**	Output:
**		Returns index of the specified string within the array
**
**	Errors:
**		Returns -1 if the string isn't in the array
**
**	Description:
**		This routine will search an array of pointers to strings
**		for a match on the specified string. If found, the array
**		index of the first match is returned. The string comparison
**		is case insensitive.
*/

int
IszOfSzI(char * sz, int csz, char * rgsz[])
	{
	int		isz;

	for (isz = 0; isz < csz; isz++) {
		if (CompareSzI(sz, rgsz[isz]) == 0) {
			return isz;
		}
	}

	return -1;

}

/* ------------------------------------------------------------ */
/***	IszSortPos
**
**	Synopsis:
**		isz = IszSortPos(sz, rgsz, csz)
**
**	Input:
**		sz		- string to test
**		rgsz	- array of pointers to strings
**		csz		- number of pointers in rgsz
**
**	Output:
**		Returns sort index for sort into ascending order
**
**	Errors:
**		none
**
**	Description:
**		This routine will return the string index at which the
**		given string should be inserted into the array to
**		maintain it in sorted order.
*/

int
IszSortPos(char * sz, char * rgsz[], int csz)
	{
	int		isz;

	if (csz == 0) {
		return 0;
	}

	isz = 0;
	while (strcmp(sz, rgsz[isz]) > 0) {
		isz += 1;
		if (isz >= csz) {
			break;
		}
	}


	return isz;

}

/* ------------------------------------------------------------ */
/***	IchFirstDif
**
**	Synopsis:
**		ich = IchFirstDif(szTst, szPat)
**
**	Input:
**		szTst	- string to test against szPat
**		szPat	- pattern string to match	
**
**	Output:
**		Returns index to first char in szTst that doesn't match
**		the corresponding char in szPat
**
**	Errors:
**		none
**
**	Description:
**		This function will compare characters in szTst against
**		the corresponding characters in szPat. It will return the
**		index of the first char in szTst that doesn't match the
**		corresponding char in szPat. If the two strings are
**		identical, it returns the index of the end of the string.
*/

int
IchFirstDif(char * szTst, char * szPat)
	{
	char *	pchTst;
	char *	pchPat;

	pchTst = szTst;
	pchPat = szPat;
	while ((*pchTst != '\0') && (*pchPat != '\0')) {
		if (*pchTst != *pchPat) {
			return (int)(pchTst-szTst);
		}
		pchTst += 1;
		pchPat += 1;
	}

	return (int)(pchTst - szTst);

}

/* ------------------------------------------------------------ */
/***	PchMatchSz
**
**	Synopsis:
**		pch = PchMatchSz(szBuf, szPat)
**
**	Input:
**		szBuf		- string to search
**		szPat		- string to search for
**
**	Output:
**		Returns character position of first occurance of szPat
**		in szBuf
**
**	Errors:
**		Returns NULL if not found
**
**	Description:
**		This routine will search for the first match of the szPat
**		string in the szBuf string.
*/

char *
PchMatchSz(char * szBuf, char * szPat)
	{
	char *	pchBufStart;
	char *	pchBuf;
	char *	pchPat;

	pchBufStart = szBuf;
	pchBuf = szBuf;
	pchPat = szPat;

	while (*pchBuf != '\0') {
		if (*pchPat == '\0') {
			/* If we hit the end of the search string, we must
			** have found a match.
			*/
			return pchBufStart;
		}

		if (*pchBuf == *pchPat) {
			pchBuf += 1;
			pchPat += 1;
		}
		else {
			pchBufStart += 1;
			pchBuf = pchBufStart;
			pchPat = szPat;
		}
	}
	
	/* If the two strings are identical, we will hit the end of
	** the pattern string at the same time.
	*/
	if (*pchPat == '\0') {
		return pchBufStart;
	}

	return NULL;

}

/* ------------------------------------------------------------ */
/***	CchOfSz
**
**	Synopsis:
**		cch = CchOfSz(sz)
**
**	Input:
**		sz		- string to count characters of
**
**	Output:
**		Returns count of characters in sz
**
**	Errors:
**		none
**
**	Description:
**		Count the number of characters in sz
*/

int
CchOfSz(const char * sz)
	{
	int		cch;

	cch = 0;
	while (*sz++ != '\0') {
		cch += 1;
	}

	return cch;

}

/* ------------------------------------------------------------ */
/***	IchOfCh
**
**	Synopsis:
**		ich = IchOfCh(sz, ch)
**
**	Input:
**		sz		- string to search
**		ch		- character to find
**
**	Output:
**		return index of first occurrence of ch in sz
**
**	Errors:
**		returns -1 if ch not found in sz
**
**	Description:
**		This routine will search the given string for the specified
**		character. If found, it returns the index location of the
**		character.
*/

int
IchOfCh(char * sz, char ch)
	{
	int		ich;

	ich = 0;
	while (sz[ich] != '\0') {
		if (sz[ich] == ch) {
			return ich;
		}
		ich += 1;
	}

	return -1;

}

/* ------------------------------------------------------------ */
/***	PchOfCh
**
**	Synopsis:
**		pch = PchOfCh(sz, ch)
**
**	Input:
**		sz		- string to search
**		ch		- character to find
**
**	Output:
**		return pointer to first occurrence of ch in sz
**
**	Errors:
**		returns NULL if ch not found in sz
**
**	Description:
**		This routine will search the given string for the specified
**		character. If found, it returns a pointer to the character.
*/

char *
PchOfCh(char * sz, char ch)
	{
	char	chT;

	while ((chT = *sz) != '\0') {
		if (chT == ch) {
			return sz;
		}
		sz += 1;
	}

	return NULL;

}

/* ------------------------------------------------------------ */
/***	FIsNulSz
**
**	Synopsis:
**		fMatch = FIsNulSz(sz)
**
**	Input:
**		sz		- string to test
**
**	Output:
**		Returns fTrue is sz is a nul (zero length) string
**
**	Errors:
**		none
**
**	Description:
**		Test for nul string.
*/

bool
FIsNulSz(const char * sz)
	{

	return (*sz == '\0');

}

/* ------------------------------------------------------------ */
/***	SzCopy
**
**	Synopsis:
**		szOut = SzCopy(szDst, szSrc)
**
**	Input:
**		szDst	- string to receive copy
**		szSrc	- string to copy
**
**	Output:
**		Returns szDst
**
**	Errors:
**		none
**
**	Description:
**		This will copy the source string to the destination
**		buffer.
*/

char *
SzCopy(char * szDst, const char * szSrc)
	{
	char *	szOut;

	szOut = szDst;
	while ((*szOut++ = *szSrc++) != '\0') {
	}

	return szDst;
}

/* ------------------------------------------------------------ */
/***	WszFromSz
**
**	Synopsis:
**		wsz = WszFromSz(wsz, sz)
**
**	Input:
**		wsz		- buffer to receive converted string
**		sz		- source string to convert
**
**	Output:
**		Returns pointer to converted string
**
**	Errors:
**		none
**
**	Description:
**		This function copies the specified string into a wide
**		character string.
*/

WCHAR *
WszFromSz(WCHAR * wsz, char * sz)
	{
	char		ch;
	WCHAR *		wszRet;

	wszRet = wsz;
	while ((ch = *sz++) != '\0') {
		*wsz++ = ch;
	}
	*wsz = 0;

	return wszRet;

}

/* ------------------------------------------------------------ */
/***	SzFromWsz
**
**	Synopsis:
**		sz = SzFromWsz(sz, wsz)
**
**	Input:
**		sz		- buffer to receive converted string
**		wsz		- source string to convert
**
**	Output:
**		Returns pointer to converted string
**
**	Errors:
**		none
**
**	Description:
**		This function copies the specified string into a
**		normal character string. This only works for wide
**		char strings that contain only ANSI equivalent 
*		characters
*/

char *
SzFromWsz(char * sz, WCHAR * wsz)
	{
	WCHAR		wch;
	char *		szRet;

	szRet = sz;
	while ((wch = *wsz++) != 0) {
		*sz++ = (char)wch;
	}
	*sz = 0;

	return szRet;

}

/* ------------------------------------------------------------ */
/***	CompareSz
**
**	Synopsis:
**		ir = CompareSz(szA, szB)
**
**	Input:
**		szA		- first string to compare
**		szB		- second string to compare
**
**	Output:
**		returns -1 if szA < szB, 0 if szA == szB, +1 if szA > szB
**
**	Errors:
**		none
**
**	Description:
**		This routine will compare the two strings and return the
**		relative order.
*/

int
CompareSz(const char * szA, const char * szB)
	{

	return strcmp(szA, szB);

}

/* ------------------------------------------------------------ */
/***	CompareSzI
**
**	Synopsis:
**		ir = CompareSzI(szA, szB)
**
**	Input:
**		szA		- first string to compare
**		szB		- second string to compare
**
**	Output:
**		returns -1 if szA < szB, 0 if szA == szB, +1 if szA > szB
**
**	Errors:
**		none
**
**	Description:
**		This routine will compare the two strings and return the
**		relative order.
*/

int
CompareSzI(const char * szA, const char * szB)
	{

	return _stricmp(szA, szB);

}

/* ------------------------------------------------------------ */
/***	SzReplaceCh
**
**	Synopsis:
**		sz = SzReplaceCh(sz, chOld, chNew)
**
**	Input:
**		sz		- string to modify
**		chOld	- char to replace
**		chNew	- char to replace with
**
**	Output:
**		Returns a pointer to the beginning of sz
**		
**	Errors:
**		none
**
**	Description:
**		This routine will replace all instances of the character
**		chOld in the string 'sz' with the new character chNew.
*/

char *
SzReplaceCh(char * sz, char chOld, char chNew)
	{
	char *		pch;

	pch = sz;
	while (*pch != '\0') {
		if (*pch == chOld) {
			*pch = chNew;
		}
		pch += 1;
	}

	return sz;

}

/* ------------------------------------------------------------ */
/***	SzAppend
**
**	Synopsis:
**		szOut = SzAppend(szDst, szSrc)
**
**	Input:
**		szDst	- string to receive copy
**		szSrc	- string to copy
**
**	Output:
**		Returns szDst
**
**	Errors:
**		none
**
**	Description:
**		This will append the source string to the destination
**		buffer.
*/

char *
SzAppend(char * szDst, const char * szSrc)
	{
	char *	szOut;

	szOut = SzEnd(szDst);
	while ((*szOut++ = *szSrc++) != '\0') {
	}

	return szDst;
}

/* ------------------------------------------------------------ */
/***	SzEnd
**
**	Synopsis:
**		sz = SzEnd(sz)
**
**	Input:
**		sz		- string to find end of
**
**	Output:
**		Returns pointer to null at end of sz
**
**	Errors:
**		none
**
**	Description:
**		This routine will return a pointer to the end of the
**		given string.
*/

char *
SzEnd(char * sz)
	{

	while (*sz != '\0') {
		sz += 1;
	}

	return sz;

}

/* ------------------------------------------------------------ */
/***	SzMakeUpper
**
**	Synopsis:
**		sz = SzMakeUpper(sz)
**
**	Input:
**		sz		- string to force to upper case
**
**	Output:
**		Returns pointer to the beginning of the string
**
**	Errors:
**		none
**
**	Description:
**		This routine will force the specified string to upper case.
*/

char *
SzMakeUpper(char * sz)
	{
	char *	pch;

	pch = sz;
	while (*pch != '\0') {
		*pch = ChMakeUpper(*pch);
		pch += 1;
	}
	return sz;
	
}

/* ------------------------------------------------------------ */
/***	SzSkipSpace
**
**	Synopsis:
**		szRes = SzSkipSpace(sz)
**
**	Input:
**		sz		- input string
**
**	Output:
**		pointer to next non-whitespace character or end of string
**
**	Errors:
**		none
**
**	Description:
**		This function will skip white space characters and return
**		a pointer to the next non-whitespace character in the
**		string, or the end of the string.
*/

char *
SzSkipSpace(char * sz)
	{
	char	ch;

	while (((ch = *sz) == ' ') || (ch == '\t') || (ch == '\n') || (ch == '\r')) {
		sz += 1;
	}

	return sz;

}

/* ------------------------------------------------------------ */
/***	ProcName
**
**	Synopsis:
**
**	Input:
**
**	Output:
**
**	Errors:
**
**	Description:
**
*/

/* ------------------------------------------------------------ */

/************************************************************************/

