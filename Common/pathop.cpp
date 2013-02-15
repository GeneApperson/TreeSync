/************************************************************************/
/*                                                                      */
/*  pathop.cpp  --  File Directory Path Manipulation Routines			*/
/*                                                                      */
/************************************************************************/
/*	Author: 	Gene Apperson											*/
/*	Copyright 2003, 2004 Digilent Inc.									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*	This module contains functions for manipulation file directory		*/
/*	path strings.														*/
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*	10/06/2004(GeneA): created by copying functions from genutil.cpp	*/
/*	04/07/2005(GeneA): changed SzAppendPathLevel to not append a path	*/
/*		separator (\) to the destination if there is already one there.	*/
/*	06/04/2009(JoshP):  Upgraded to VS2008								*/
/*                                                                      */
/************************************************************************/

#define	_CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

/* ------------------------------------------------------------ */
/*					Include File Definitions					*/
/* ------------------------------------------------------------ */

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
/*				File Path Manipulation Routines					*/
/* ------------------------------------------------------------ */
/***	FCreatePath
**
**	Synopsis:
**		fSuccess = FCreatePath(szPath)
**
**	Input:
**		szPath	- absolute path string
**
**	Output:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if error occurs
**
**	Description:
**		This routine will ensure that the specified path exists.
**		It will walk down each level in the path string from
**		the root, ensuring that the directory exists at that level
**		and creating it if it doesn't. The given path string
**		should only contain directory names. It should not contain
**		a file name at the end of the string.
*/

bool
FCreatePath(char * szPath)
	{
	char		szTemp[MAX_PATH];
	HANDLE		fh;
	int			ilvCur;
	int			ilvMac;
	char *		pch;
	DWORD		ers;

	ilvMac = 1;

	while (fTrue) {

		/* Construct the current path level to test by copying
		** the original string, skipping over the drive spec
		** if it exists and then skipping over the number of
		** path levels to get to the current path depth.
		*/
		SzCopy(szTemp, szPath);
		pch = SzSkipDriveSpec(szTemp);

		for (ilvCur = 0; ilvCur < ilvMac; ilvCur++) {
			if (*pch == '\0') {
				/* If we are at the end of the input string,
				** we have checked all levels.
				*/
				return fTrue;
			}
			pch += 1;
			pch = SzSkipPathLevel(pch);
		}

		if (*pch == '\\') {
			pch -= 1;
		}
		*pch = '\0';

		/* Attempt to open this directory.
		*/
		fh = CreateFile(szTemp,
						GENERIC_READ,
						FILE_SHARE_READ|FILE_SHARE_DELETE,
						NULL,
						OPEN_EXISTING,
						FILE_FLAG_BACKUP_SEMANTICS,
						NULL
					);

		if (fh == INVALID_HANDLE_VALUE) {
			ers = GetLastError();
			if ((ers != ERROR_FILE_NOT_FOUND) && (ers != ERROR_PATH_NOT_FOUND)) {
				/* Some kind of system error, so give up.
				*/
				return fFalse;
			}
			if (CreateDirectory(szTemp, NULL) == NULL) {
				/* Couldn't create the directory for some reason.
				*/
				return fFalse;
			}

		}
		else {
			/* This directory level exists. Close it and go to the
			** next one.
			*/
			CloseHandle(fh);
		}

		ilvMac += 1;

	}

	return fTrue;

}

/* ------------------------------------------------------------ */
/***	VolumeOfPath
**
**	Input:
**		szPath	- pointer to path string
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This function will truncate the given path string so that
**		it contains only the volume name. This will either be a
**		drive spec, or a server share of the form \\server\share.
**		If the path doesn't contains a volume spec, it is truncated
**		to an empty string.
*/

void
VolumeOfPath(char * szPath)
	{
	char *	szT;
	
	/* Check for null string and return the argument if it is null
	*/
	if (CchOfSz(szPath) == 0) {
		return;
	}
	
	/* Is it a drive spec?
	*/
	if (FIsDriveSpec(szPath)) {
		szPath[2] = '\0';
	}
	
	/* Is it a network share?
	*/
	else if ((szPath[0] == '\\') && (szPath[1] == '\\')) {
		/* It begins with \\, skip two path levels.
		*/
		szT = SzSkipPathLevel(&szPath[2]);
		szT = SzSkipPathLevel(szT);
		if (CchOfSz(szT) > 0) {
			szT -= 1;
			*szT = '\0';
		}
	}
	
	/* Don't know what it is, make it be a null string.
	*/
	else {
		*szPath = '\0';
	}
	
}

/* ------------------------------------------------------------ */
/***	SzTitleOfName
**
**	Synopsis:
**		sz = SzTitleOfName(szName)
**
**	Input:
**		szName	- pointer to file name string
**
**	Output:
**		Returns pointer to file title portion of file name string
**
**	Errors:
**		none
**
**	Description:
**		This function will parse the file title from the given
**		file name string. A file name string is the full path
**		to the file. The file title is the file name specifier
**		without the directory specifiers.
*/

char *
SzTitleOfName(char * szName)
	{
	char		ch;
	char *		pch;

	/* Start at the end of the string 
	*/
	pch = SzEnd(szName);

	/* Work backward looking for the first '\' or ':'.
	*/
	while (pch > szName) {
		ch = *pch;
		if ((ch == '\\') || (ch == ':')) {
			return pch+1;
		}
		pch -= 1;
	}

	/* We worked our way all the way back to the beginning,
	** so the given name must also be the title.
	*/
	return szName;

}

/* ------------------------------------------------------------ */
/***	SzExtOfName
**
**	Synopsis:
**		sz = SzExtOfName(szName)
**
**	Input:
**		szName	- pointer to file name string
**
**	Output:
**		Returns pointer to file extension portion of file name string
**
**	Errors:
**		Returns NULL if file has no extension
**
**	Description:
**		This function will parse the file title from the given
**		file name string. A file name string is the full path
**		to the file. The file title is the file name specifier
**		without the directory specifiers.
*/

char *
SzExtOfName(char * szName)
	{
	char		ch;
	char *		pch;

	/* Start at the end of the string 
	*/
	pch = SzEnd(szName);

	/* Work backward looking for the first '.'
	*/
	while (pch > szName) {
		pch -= 1;
		ch = *pch;
		if (ch == '\\') {
			/* If we hit a path separator, that means that there is
			** no extension on the name.
			*/
			break;
		}
		if (ch == '.') {
			return pch;
		}
	}

	/* We worked our way all the way back to the beginning,
	** so the given name must also be the title.
	*/
	return NULL;

}

/* ------------------------------------------------------------ */
/***	MakeAbsPath
**
**	Synopsis:
**		MakeFullPath(szAbs, szRoot, szRel)
**
**	Input:
**		szAbs		- buffer to receive output string
**		szRoot		- root path string
**		szRel		- relative path suffix string
**
**	Output:
**		Result in szAbs
**
**	Errors:
**		none
**
**	Description:
**		This routine will construct an absolute path to a file
**		from the given root path and relative path strings.
*/

void
MakeAbsPath(char * szAbs, char * szRoot, char * szRel)
	{
	char *		szEnd;

	/* If szRel is an absolute path (it begins with a '\' or
	** a drive spec), use it as the output value.
	*/
	if (FIsDriveSpec(szRel) || (*szRel == '\\')) {
		SzCopy(szAbs, szRel);
		return;
	}

	/* SzRel is a relative path, so we need to build up an
	** output string using szRoot and szRel.
	** Start with the root path.
	*/
	SzCopy(szAbs, szRoot);
	szEnd = SzEnd(szAbs);
	if (*(szEnd-1) == '\\') {
		szEnd = SzRemovePathLevel(szAbs, szEnd);
	}

	/* Append directory levels from the relative path string
	** to the end of the output string.
	*/
	while (!FIsNulSz(szRel)) {
		if (FIsCurDir(szRel)) {
			szRel = SzSkipPathLevel(szRel);
		}
		else if (FIsParentDir(szRel)) {
			szEnd = SzRemovePathLevel(szAbs, szEnd);
			szRel = SzSkipPathLevel(szRel);
		}
		else {
			szEnd = SzAppendPathLevel(szEnd, szRel);
			szRel = SzSkipPathLevel(szRel);
		}
	}

}

/* ------------------------------------------------------------ */
/***	MakeRelPath
**
**	Synopsis:
**		MakeFullPath(szRel, szRoot, szAbs)
**
**	Input:
**		szRel		- buffer to receive output string
**		szRoot		- root path string
**		szAbs		- absolute path to convert
**
**	Output:
**		Result in szRel
**
**	Errors:
**		none
**
**	Description:
**		This routine will construct a relative path string
**		to go from szRoot to the full path in szAbs. szRoot is
**		a complete directory path string (including drive specifier)
**		to a root directory (it does not contain a file specifier).
**		szAbs is a complete path string (including drive spec)
**		to a file.
*/

void
MakeRelPath(char * szRel, char * szRoot, char * szAbs)
	{
	char *		szOut;

	szOut = szRel;

	/* If the drive specifiers for the two strings are
	** different, it isn't possible to create a relative
	** path and so the output string is just szAbs.
	*/
	if (!FCmpDriveSpec(szRoot, szAbs)) {
		SzCopy(szRel, szAbs);
		return;
	}

	/* Skip past the drive and root level directory separator
	** at the beginning of each string.
	*/
	szRoot = SzSkipDriveSpec(szRoot);
	szRoot = SzSkipPathLevel(szRoot);

	szAbs = SzSkipDriveSpec(szAbs);
	szAbs = SzSkipPathLevel(szAbs);

	/* Skip over any matching path levels at the top of the
	** two strings.
	*/
	while (!FIsNulSz(szRoot) && FCmpPathLevel(szRoot, szAbs)) {
		szRoot = SzSkipPathLevel(szRoot);
		szAbs  = SzSkipPathLevel(szAbs);
	}

	/* We have hit the point where the two string start to 
	** differ. For each remaining level in the root string,
	** put a a parent directory specifier (..\) in the output
	** string.
	*/
	szOut = SzCopy(szRel, "");
	while (!FIsNulSz(szRoot)) {
		SzAppend(szOut, "..\\");
		szOut = SzEnd(szOut);
		szRoot = SzSkipPathLevel(szRoot);
	}

	/* Append to remaining part of szAbs to the output string.
	*/
	SzAppend(szOut, szAbs);

	/* If we ended up with a nul string for the relative
	** path, then make it be ".\"
	*/
	if (FIsNulSz(szRel)) {
		SzCopy(szRel, ".\\");
	}

}

/* ------------------------------------------------------------ */
/***	FCmpDriveSpec
**
**	Synopsis:
**		fMatch = FCmpDriveSpec(szA, szB)
**
**	Input:
**		szA		- pointer to first path string
**		szB		- pointer to second path string
**
**	Output:
**		Returns fTrue if both strings have a drive specifier
**		that matches.
**
**	Errors:
**		none
**
**	Description:
**		This routine will compare the drive specifiers on
**		the two path strings to see if they match. If either
**		one doesn't have a drive specifier it returns fFalse.
*/

bool
FCmpDriveSpec(char * szA, char * szB)
	{

	if ((ChMakeUpper(*szA) == ChMakeUpper(*szB)) &&
		(*(szA+1) == ':') && (*(szB+1) == ':')) {
		return fTrue;
	}
	else {
		return fFalse;
	}
}

/* ------------------------------------------------------------ */
/***	FCmpPathLevel
**
**	Synopsis:
**		fMatch = FCmpPathLevel(szA, szB)
**
**	Input:
**		szA		- pointer to one path string
**		szB		- pointer to other path string
**
**	Output:
**		Returns fTrue if the two match
**
**	Errors:
**		none
**
**	Description:
**		This routine will compare the current path level in
**		two path strings. Since case is not significant in
**		windows file names, it does a case insensitive compare.
*/

bool
FCmpPathLevel(char * szA, char * szB)
	{
	char		chA;
	char		chB;

	while (fTrue) {
		chA = *szA++;
		chB = *szB++;

		if (((chA == '\\') || (chA == '\0')) &&
			((chB == '\\') || (chB == '\0'))) {
			return fTrue;
		}

		if (ChMakeUpper(chA) != ChMakeUpper(chB)) {
			return fFalse;
		}
	}

}

/* ------------------------------------------------------------ */
/***	FIsDriveSpec
**
**	Synopsis:
**		fMatch = FIsDriveSpec(sz)
**
**	Input:
**		sz		- path string
**
**	Output:
**		Returns fTrue if sz is a drive specifier
**
**	Errors:
**		none
**
**	Description:
**		This routine will check if the beginning of the
**		given path string has a drive specifier.
*/

bool
FIsDriveSpec(char * sz)
	{
	char	ch;

	if (*sz != '\0') {
		ch = ChMakeUpper(*sz);
		if ((ch >= 'A') && (ch <= 'Z') && (*(sz+1) == ':')) {
			return fTrue;
		}
	}

	return fFalse;

}

/* ------------------------------------------------------------ */
/***	FIsParentDir
**
**	Synopsis:
**		fMatch = FIsParentDir(szRel)
**
**	Input:
**		szRel	- path to check
**
**	Output:
**		returns fTrue if match
**
**	Errors:
**		none
**
**	Description:
**		This function will check if the top level directory
**		specifier in the given relative path string is a  
**		reference to the parent directory in the path (..\).
*/

bool
FIsParentDir(char * szRel)
	{

	if ((*szRel == '.') && (*(szRel+1) == '.') && (*(szRel+2) == '\\')) {
		return fTrue;
	}
	else {
		return fFalse;
	}
}

/* ------------------------------------------------------------ */
/***	FIsCurDir
**
**	Synopsis:
**		fMatch = FIsCurDir(szRel)
**
**	Input:
**		szRel	- path to check
**
**	Output:
**		returns fTrue if match
**
**	Errors:
**		none
**
**	Description:
**		This function will check if the top level directory
**		specifier in the given relative path string is a
**		reference to the current directory (.\).
*/

bool
FIsCurDir(char * szRel)
	{

	if ((*szRel == '.') &&  (*(szRel+1) == '\\')) {
		return fTrue;
	}
	else {
		return fFalse;
	}
}

/* ------------------------------------------------------------ */
/***	SzSkipDriveSpec
**
**	Synopsis:
**		szPath = SzSkipDriveSpec(sz)
**
**	Input:
**		sz		- path string
**
**	Output:
**		Returns pointer to top level directory specifier in the
**		given path string.
**
**	Errors:
**		none
**
**	Description:
**		This routine will remove the drive specifier from the
**		specified path string. If the path string doesn't have
**		a drive specifier, it returns a pointer to the beginning
**		of the input string.
*/

char *
SzSkipDriveSpec(char * sz)
	{

	if (*sz != '\0') {
		if (*(sz+1) == ':') {
			sz = sz+2;
		}
	}

	return sz;

}

/* ------------------------------------------------------------ */
/***	SzSkipPathLevel
**
**	Synopsis:
**		szPath = SzSkipPathLevel(sz)
**
**	Input:
**		sz		- pointer to path string
**
**	Output:
**		Returns pointer to next leve in path string
**
**	Errors:
**		none
**
**	Description:
**		This routine will skip over the first path level in the
**		path string, returning a pointer to the next level in
**		the string. It returns a pointer to the character
**		following the '/'. If the input path level is the bottom
**		level in the path, it will return a pointer to a null
**		string.
*/

char *
SzSkipPathLevel(char * sz)
	{
	char		ch;

	while ((ch = *sz++) != '\0') {
		if (ch == '\\') {
			return sz;
		}
	}

	return sz-1;

}

/* ------------------------------------------------------------ */
/***	SzRemovePathLevel
**
**	Synopsis:
**		szEnd = SzRemovePathLevel(szStart, szEnd)
**
**	Input:
**		szStart	- pointer to beginning of path string
**		szEnd	- pointer to current end of path string
**
**	Output:
**		Returns pointer to new end of string.
**
**	Errors:
**		none
**
**	Description:
**		This routine will remove the bottom level from the
**		specified path string. It returns a pointer to the
**		new end of the resulting string. This can result in
**		a null string if there is only a single level in the 
**		input path.
*/

char *
SzRemovePathLevel(char * szStart, char * szEnd)
	{

	while (szEnd > szStart) {
		if ((*szEnd == '\\') || (*szEnd == ':')) {
			break;
		}
		szEnd -= 1;
	}

	*szEnd = '\0';
	return szEnd;

}

/* ------------------------------------------------------------ */
/***	SzAppendDriveSpec
**
**	Synopsis:
**		szEnd = SzAppendDriveSpec(szDst, szSrc)
**
**	Input:
**		szDst	- string to receive drive spec
**		szSrc	- string containing drive spec
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		szSrc is assumed to have a drive specifier. This will
**		copy the first two characters from szSrc to szDst and
**		return a pointer to the end of the copied characters.
*/

char *
SzAppendDriveSpec(char * szDst, char * szSrc)
	{

	*szDst++ = *szSrc++;
	*szDst++ = *szSrc++;
	*szDst = '\0';
	return szDst;

}

/* ------------------------------------------------------------ */
/***	SzAppendPathLevel
**
**	Synopsis:
**		szEnd = SzAppendPathLevel(szDst, szSrc)
**
**	Input:
**		szDst	- pointer to the end of the destination string
**		szSrc	- pointer to the beginning of the source string
**
**	Output:
**		Next Path level from szSrc appended to the end of szDst
**		Returns pointer to the new end of the destination string
**
**	Errors:
**		none
**
**	Description:
**		This routine will append the first path level from
**		the source path string onto the end of the destination
**		path string.
*/

char *
SzAppendPathLevel(char * szDst, char * szSrc)
	{
	char		ch;

	if ((*szSrc != '\0') && (*(szDst-1) != '\\')) {
		*szDst++ = '\\';
	}
	while ((ch = *szSrc) != '\0') {
		if (ch == '\\') {
			break;
		}
		*szDst++ = ch;
		szSrc += 1;
	}

	*szDst = '\0';
	return szDst;

}

/* ------------------------------------------------------------ */
/***	SzAppendPathSeparator
**
**	Synopsis:
**		szEnd = SzAppendPathSeparator(szPath)
**
**	Input:
**		szPath		- pointer to end of path string to modify
**
**	Output:
**		Returns pointer to new end of path string
**
**	Errors:
**		none
**
**	Description:
**		This will check if the last character in the path string
**		is a '\' and append on if not.
*/

char *
SzAppendPathSeparator(char * szPath)	
	{

	if (*(szPath-1) != '\\') {
		*szPath++ = '\\';
		*szPath = '\0';
	}

	return szPath;

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

