/************************************************************************/
/*                                                                      */
/*  charop.cpp  --  General Purpose Character Manipulation Functions	*/
/*                                                                      */
/************************************************************************/
/*	Author: 	Gene Apperson											*/
/*	Copyright 2003, 2004 Digilent Inc.									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*	This module contains general purpose utility functions for			*/
/*	character manipulations				.								*/
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*	10/06/2004(GeneA): created by copying functions from genutil.cpp	*/
/*	03/27/2008(GeneA): added FIsAlpha and FIsHexDigit					*/
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

/* This array contains all characters that are valid in project and
** workspace names.
*/
static char	rgchProjChar[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789~!@#$%^&()-=+_[]{};";
static char	rgchFileChar[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789~!@#$%^&()-=+_[]{};.";
static char	rgchPathChar[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789~!@#$%^&()-=+_[]{};:\\";

/* ------------------------------------------------------------ */
/*					Forward Declarations						*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*			Character  Manipulation Routines					*/
/* ------------------------------------------------------------ */
/***	CompareChI
**
**	Input:
**		chA		- first character
**		chB		- second character
**
**	Output:
**		returns -1 if chA < chB, 0 if chA == chB, +1 if chA > chB
**
**	Errors:
**		none
**
**	Description:
**		This routine will compare the two characters and return
**		the relative order.
*/

int
CompareChI(char chA, char chB)
	{

	if ((chA >= 'a') && (chA <= 'z')) {
		chA -= 'a' - 'A';
	}
	
	if ((chB >= 'a') && (chB <= 'z')) {
		chB -= 'a' - 'A';
	}

	if (chA == chB) {
		return 0;
	}

	return (chA < chB) ? -1 : 1;

}

/* ------------------------------------------------------------ */
/***	ChMakeUpper
**
**	Input:
**		ch		- character to force to upper case
**
**	Output:
**		Returns upper case character
**
**	Errors:
**		none
**
**	Description:
**		This routine will force the input character to upper case.
**		If the input character isn't a lower case alpha, it does
**		nothing.
*/

char
ChMakeUpper(char ch)
	{

	if ((ch >= 'a') && (ch <= 'z')) {
		ch -= 'a' - 'A';
	}

	return ch;

}

/* ------------------------------------------------------------ */
/***	FIsAlpha
**
**	Parameters:
**		ch		- character to check
**
**	Return Value:
**		Returns fTrue if ch is a hex digit character
**
**	Errors:
**		none
**
**	Description:
**
*/

bool
FIsAlpha(char ch)
	{
	
	if (((ch >= 'A') && (ch <= 'Z')) ||
		((ch >= 'a') && (ch <= 'z'))) {
		return fTrue;
	}
	else {
		return fFalse;
	}
}

/* ------------------------------------------------------------ */
/***	FIsDigit
**
**	Input:
**		ch		- character to test
**
**	Output:
**		Returns fTrue if ch is a decimal digit
**
**	Errors:
**		none
**
**	Description:
**		Check if a character is a decimal digit.
*/

bool
FIsDigit(char ch)
	{

	return ((ch >= '0') && (ch <= '9'));

}

/* ------------------------------------------------------------ */
/***	FIsHexDigit
**
**	Parameters:
**		ch		- character to check
**
**	Return Value:
**		Returns fTrue if ch is a hex digit character
**
**	Errors:
**		none
**
**	Description:
**
*/

bool
FIsHexDigit(char ch)
	{
	
	if (((ch >= '0') && (ch <= '9')) ||
		((ch >= 'A') && (ch <= 'F')) ||
		((ch >= 'a') && (ch <= 'f'))) {
		return fTrue;
	}
	else {
		return fFalse;
	}
}

/* ------------------------------------------------------------ */
/***	FIsSymChar
**
**	Input:
**		ch		- character to test
**
**	Output:
**		Returns fTrue if character is a valid symbol name character
**
**	Errors:
**		none
**
**	Description:
**		This function will test to see if the specified character
**		is a valid character in a symbol name.
*/

bool
FIsSymChar(char ch)
	{

	if (((ch >= 'A') && (ch <= 'Z')) ||
		((ch >= 'a') && (ch <= 'z')) ||
		((ch >= '0') && (ch <= '9')) ||
		(ch == '_')) {
		return fTrue;
	}

	return fFalse;

}

/* ------------------------------------------------------------ */
/***	FIsFileChar
**
**	Input:
**		ch		- character to check
**
**	Output:
**		Returns fTrue if ch is a valid character for a file name.
**
**	Errors:
**		none
**
**	Description:
**		Checks if the specified character is valid in a file
**		name.
*/

bool
FIsFileChar(char ch)
	{

	if (PchOfCh(rgchFileChar, ch) == NULL) {
		return fFalse;
	}

	return fTrue;

}

/* ------------------------------------------------------------ */
/***	FIsPathChar
**
**	Input:
**		ch		- character to check
**
**	Output:
**		Returns fTrue if ch is a valid character for a path name.
**
**	Errors:
**		none
**
**	Description:
**		Checks if the specified character is valid in a path
**		name.
*/

bool
FIsPathChar(char ch)
	{

	if (PchOfCh(rgchPathChar, ch) == NULL) {
		return fFalse;
	}

	return fTrue;

}

/* ------------------------------------------------------------ */
/***	FIsProjChar
**
**	Input:
**		ch		- character to check
**
**	Output:
**		Returns fTrue if ch is a valid character for a project name.
**
**	Errors:
**		none
**
**	Description:
**		Checks if the specified character is valid in a project
**		name.
*/

bool
FIsProjChar(char ch)
	{

	if (PchOfCh(rgchProjChar, ch) == NULL) {
		return fFalse;
	}

	return fTrue;

}

/* ------------------------------------------------------------ */
/***	ProcName
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

