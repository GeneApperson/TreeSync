/************************************************************************/
/*                                                                      */
/*  Dassert.cpp	--  Debugging Support Routines							*/
/*                                                                      */
/************************************************************************/
/*	Author: 	Gene Apperson											*/
/*	Copyright 2003, 2007, Digilent Inc.									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*	This module contains the implementation for debug support stuff.	*/
/*	These are the runtime support for Assert macros and other debug		*/
/*	version build things.												*/
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*	06/10/2003(GeneA): created											*/
/*	12/04/2007(GeneA): revised to conform to current coding standards	*/
/*                                                                      */
/************************************************************************/

#if defined(_DEBUG)

#define	_CRT_SECURE_NO_WARNINGS

/* ------------------------------------------------------------ */
/*					Include File Definitions					*/
/* ------------------------------------------------------------ */

#include	<windows.h>
#include	<stdio.h>

const int cchDbgMsgMax = 256;

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
/*					Procedure Definitions						*/
/* ------------------------------------------------------------ */
/***	DoAssert
**
**	Parameters:
**		szFile		- file name where assertion failed
**		ilnLine		- line number where assertion failed
**		szMsg		- supplemental message
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This function is used by the Assert macro to display
**		a message box to the user about an internal failure
**		in the program.
*/

void
DoAssert(char * szFile, int ilnLine, char * szMsg)
	{
	char szBuf[cchDbgMsgMax];

	sprintf(szBuf, "%s:%d: %s", szFile, ilnLine, szMsg);
	MessageBox(NULL, szBuf, "Assertion Failure", MB_OK|MB_ICONEXCLAMATION);

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

#endif

/************************************************************************/

