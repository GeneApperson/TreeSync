/************************************************************************/
/*                                                                      */
/*  regop.cpp  --  General Purpose Registry Manipulation Functions		*/
/*                                                                      */
/************************************************************************/
/*	Author: 	Joshua Pederson											*/
/*	Copyright 2005 Digilent Inc.										*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*	This module contains general purpose utility functions for			*/
/*	manipulating nul terminates strings.								*/
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*	05/11/2005(JPederson): created by copying functions from genutil.cpp	*/
/*	06/04/2009(JoshP):  Upgraded to VS2008								*/
/*                                                                      */
/************************************************************************/


/* ------------------------------------------------------------ */
/*					Include File Definitions					*/
/* ------------------------------------------------------------ */
#define	_CRT_SECURE_NO_WARNINGS
#include	<windows.h>
#include	<string.h>

#include	"dtypes.h"
#include	"dutil.h"

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
/*			Registry Manipulation Routines						*/
/* ------------------------------------------------------------ */
/***	FGetRegString
**
**	Synopsis:
**		isz = FGetRegString(hkey, szRegPath, szRegString)
**
**	Input:
**		hkey		- reg key to open
**		rgsz		- registry path name
**		szRegString	- pointer to hold value of string in registry
**
**	Output:
**		returns boolean.  true if success, false otherwise.
**
**	Errors:
**		none
**
**	Description:
**		This routine will get the string value in the registry specified 
**		by the key and pathname
*/

bool
FGetRegString(HKEY hkey, char * szRegPath, char * szRegString)
	{

	char			szName[MAX_PATH+1];
	DWORD			dwType;
	DWORD			dwSize = MAX_PATH;
	HKEY			hkeyGet;

	if (szRegPath == NULL) {
		return false;
	}

	strcpy(szName, SzTitleOfName(szRegPath));
	SzRemovePathLevel(szRegPath, SzEnd(szRegPath));

	if (szName == NULL || strlen (szName) >255) {
		return false;
	}

	if (RegOpenKeyEx(hkey, szRegPath, 
		0, KEY_QUERY_VALUE | KEY_READ, &hkeyGet) != ERROR_SUCCESS) {
		return false;
	}

	if (RegQueryValueEx(hkeyGet, szName, 
		NULL, &dwType, (BYTE*) szRegString, &dwSize) != ERROR_SUCCESS) {
		RegCloseKey(hkeyGet);
		return false;
	}

	RegCloseKey(hkeyGet);
	return true;
}

/* ------------------------------------------------------------ */
/***	FGetRegDword
**
**	Synopsis:
**		isz = FGetRegString(hkey, szRegPath, pidDword)
**
**	Input:
**		hkey		- reg key to open
**		rgsz		- registry path name
**		pidDword	- pointer to hold value of DWORD in registry
**
**	Output:
**		returns boolean.  true if success, false otherwise.
**
**	Errors:
**		none
**
**	Description:
**		This routine will get the string value in the registry specified 
**		by the key and pathname
*/

bool
FGetRegDword(HKEY hkey, char * szRegPath, DWORD * pidDword)
	{

	char			szName[MAX_PATH+1];
	char			szKey[MAX_PATH+1];
	DWORD			dwType;
	DWORD			dwSize = MAX_PATH;
	HKEY			hkeyGet;

	if (szRegPath == NULL) {
		return false;
	}

	strcpy(szName, SzTitleOfName(szRegPath));
	strcpy(szKey, SzRemovePathLevel(szRegPath, SzEnd(szRegPath)));

	if (szName == NULL || strlen (szName) >255) {
		return false;
	}

	if (RegOpenKeyEx(hkey, szRegPath, 
		0, KEY_QUERY_VALUE | KEY_READ, &hkeyGet) != ERROR_SUCCESS) {
		return false;
	}

	if (RegQueryValueEx(hkeyGet, szName, 
		NULL, &dwType, (BYTE*) pidDword, &dwSize) != ERROR_SUCCESS) {
		RegCloseKey(hkey);
		return false;
	}

	RegCloseKey(hkeyGet);
	return true;
}

/* ------------------------------------------------------------ */
/***	FSetRegString
**
**	Synopsis:
**		isz = FGetRegString(hkey, szRegPath, szRegString)
**
**	Input:
**		hkey		- reg key to open
**		rgsz		- registry path name
**		szRegString	- pointer to hold value of string in registry
**
**	Output:
**		returns boolean.  true if success, false otherwise.
**
**	Errors:
**		none
**
**	Description:
**		This routine will get the string value in the registry specified 
**		by the key and pathname
*/

bool
FSetRegString(HKEY hkey, char * szRegPath, char * szRegString)
	{

	char			szName[MAX_PATH+1];
	char			szKey[MAX_PATH+1];
	DWORD			dwSize = MAX_PATH;
	HKEY			hkeySet;
	DWORD			idDisposition;

	if (szRegPath == NULL) {
		return false;
	}
	if (szRegString == NULL) {
		return false;
	}

	strcpy(szName, SzTitleOfName(szRegPath));
	strcpy(szKey, SzRemovePathLevel(szRegPath, SzEnd(szRegPath)));

	if (szName == NULL || strlen (szName) >255) {
		return false;
	}

	if (RegCreateKeyEx(
			hkey, 
			szRegPath, 
			0, 
			NULL, 
			REG_OPTION_NON_VOLATILE, 
			KEY_ALL_ACCESS , 
			NULL,
			&hkeySet,
			&idDisposition
			) != ERROR_SUCCESS) {
		return false;
	}

	if (RegSetValueEx(hkeySet, szName, 
		NULL, REG_SZ, (BYTE*) szRegString, (DWORD)(strlen(szRegString))) != ERROR_SUCCESS) {
		RegCloseKey(hkey);
		return false;
	}

	RegCloseKey(hkeySet);
	return true;
}

/* ------------------------------------------------------------ */
/***	FSetRegDword
**
**	Synopsis:
**		isz = FSetRegDword(hkey, szRegPath, idDword)
**
**	Input:
**		hkey		- reg key to open
**		rgsz		- registry path name
**		idDword		- value of DWORD in registry
**
**	Output:
**		returns boolean.  true if success, false otherwise.
**
**	Errors:
**		none
**
**	Description:
**		This routine will get the string value in the registry specified 
**		by the key and pathname
*/

bool
FSetRegDword(HKEY hkey, char * szRegPath, DWORD idDword)
	{

	char			szName[MAX_PATH+1];
	char			szKey[MAX_PATH+1];
	DWORD			dwSize = MAX_PATH;
	HKEY			hkeySet;
	DWORD			idDisposition;

	if (szRegPath == NULL) {
		return false;
	}

	strcpy(szName, SzTitleOfName(szRegPath));
	strcpy(szKey, SzRemovePathLevel(szRegPath, SzEnd(szRegPath)));

	if (szName == NULL || strlen (szName) >255) {
		return false;
	}

	if (RegCreateKeyEx(
			hkey, 
			szRegPath, 
			0, 
			NULL, 
			REG_OPTION_NON_VOLATILE, 
			KEY_ALL_ACCESS , 
			NULL,
			&hkeySet,
			&idDisposition
			) != ERROR_SUCCESS) {
		return false;
	}

	if (RegSetValueEx(hkeySet, szName, 
		NULL, REG_DWORD, (BYTE*) idDword, 4) != ERROR_SUCCESS) {
		RegCloseKey(hkey);
		return false;
	}

	RegCloseKey(hkeySet);
	return true;
}


/* ------------------------------------------------------------ */
/***	FOpenRegKey
**
**	Synopsis:
**		isz = FOpenRegKey(hkey, szRegPath, szRegString)
**
**	Input:
**		hkey		- reg key to open
**		rgsz		- registry path name
**		idDword		- value of DWORD in registry
**
**	Output:
**		returns boolean.  true if success, false otherwise.
**
**	Errors:
**		none
**
**	Description:
**		This routine will get the string value in the registry specified 
**		by the key and pathname
*/

bool
FOpenRegKey(HKEY hkeyRoot, char* szRegPath, HKEY *hkey)
	{

	char			szName[MAX_PATH+1];
	char			szKey[MAX_PATH+1];
	DWORD			dwSize = MAX_PATH;
	DWORD			idDisposition;

	if (szRegPath == NULL) {
		return false;
	}

	strcpy(szName, SzTitleOfName(szRegPath));
	strcpy(szKey, SzRemovePathLevel(szRegPath, SzEnd(szRegPath)));

	if (szName == NULL || strlen (szName) >255) {
		return false;
	}

	if (RegCreateKeyEx(
			hkeyRoot, 
			szRegPath, 
			0, 
			NULL, 
			REG_OPTION_NON_VOLATILE, 
			KEY_SET_VALUE , 
			NULL,
			hkey,
			&idDisposition
			) != ERROR_SUCCESS) {
		return false;
	}

	return true;
}