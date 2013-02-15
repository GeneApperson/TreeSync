/************************************************************************/
/*																		*/
/*	apst.cpp	--	Implementation of APST Object Class					*/
/*																		*/
/************************************************************************/
/*	Author: 	Gene Apperson											*/
/*	Copyright 2010, Gene Apperson										*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*	This module contains the implementation of the APST object class	*/
/*	This object class is used to maintain the global application state.	*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	12/28/2010(GeneA): created											*/
/*																		*/
/************************************************************************/

#define	WIN32_LEAN_AND_MEAN

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include	<windows.h>
#include	<stdio.h>

#include "../inc/dassert.h"
#include "../inc/dtypes.h"
#include "../inc/dutil.h"

#include	"apst.h"

/* ------------------------------------------------------------ */
/*				Local Type and Constant Definitions				*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

__SetCurFile;

extern APST	apst;

/* ------------------------------------------------------------ */
/*				Local Variables									*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				APST Object Class Implementation				*/
/* ------------------------------------------------------------ */
/***	APST::APST
**
**	Parameters:
**
**	Return Value:
**
**	Errors:
**
**	Description:
**
*/

APST::APST()
	{

	szRootSrc = NULL;
	szRootDst = NULL;
	szRootXtr = NULL;
	szCommandFile = NULL;
	szCmdPath = NULL;
	szLogPath = NULL;
	szLogFile = NULL;
	
}

/* ------------------------------------------------------------ */
/***	APST::~APST
**
**	Parameters:
**
**	Return Value:
**
**	Errors:
**
**	Description:
**
*/

APST::~APST()
	{
	AssertMsg(szMsg, "APST object not terminated before destruction");
	
	Assert(szRootSrc == NULL, szMsg);
	Assert(szRootDst == NULL, szMsg);
	Assert(szRootXtr == NULL, szMsg);
	Assert(szCommandFile == NULL, szMsg);
	Assert(szCmdPath == NULL, szMsg);
	Assert(szLogPath == NULL, szMsg);
	Assert(szLogFile == NULL, szMsg);
	Assert(fhLogFile == NULL, szMsg);

}

/* ------------------------------------------------------------ */
/***	APST::FInit
**
**	Parameters:
**
**	Return Value:
**
**	Errors:
**
**	Description:
**
*/

bool
APST::FInit()
	{
	
	stProg = stSuccess;
	fhLogFile = NULL;
	fSrcFsFat = fFalse;
	fDstFsFat = fFalse;
	fXtrFsFat = fFalse;
	fShowTime = fFalse;
	fShowUseage = fFalse;
	fPrintFile = fFalse;
	fPrintDir = fFalse;
	fPrintXclFile = fFalse;
	fPrintXclDir = fFalse;
	fLogFile = fFalse;
	fLogDir = fFalse;
	fLogXclFile = fFalse;
	fLogXclDir = fFalse;
	fLogAction = fFalse;
	fNoCopyFile = fFalse;
	fNoCopyDir = fFalse;
	fUseSignatures = fFalse;
	fCommandFile = fFalse;
	fUpdateDst = fFalse;
	fExtractDst = fFalse;
	fTrimDst = fFalse;
	fExtractUnique = fFalse;
	fExtractDiff = fFalse;
	fLogFileCreated = fFalse;
	
	return fTrue;
	
}

/* ------------------------------------------------------------ */
/***	APST::Terminate
**
**	Parameters:
**
**	Return Value:
**
**	Errors:
**
**	Description:
**
*/

void
APST::Terminate()
	{

	if (szRootSrc != NULL) {
		delete [] szRootSrc;
		szRootSrc = NULL;
	}
	
	if (szRootDst != NULL) {
		delete [] szRootDst;
		szRootDst = NULL;
	}
	
	if (szRootXtr != NULL) {
		delete [] szRootXtr;
		szRootXtr = NULL;
	}
	
	if (szCommandFile != NULL) {
		delete [] szCommandFile;
		szCommandFile = NULL;
	}
	
	if (szCmdPath != NULL) {
		delete [] szCmdPath;
		szCmdPath = NULL;
	}
	
	if (szLogPath != NULL) {
		delete [] szLogPath;
		szLogPath = NULL;
	}
	
	if (szLogFile != NULL) {
		delete [] szLogFile;
		szLogFile = NULL;
	}
			
}

/* ------------------------------------------------------------ */
/***	APST::FSetSzRootSrc
**
**	Parameters:
**		sz		- root path string to set
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Store the specified string in szRootSrc
*/

bool
APST::FSetSzRootSrc(char * sz)
	{
	
	if (szRootSrc != NULL) {
		delete [] szRootSrc;
	}
	
	if ((szRootSrc = new char[CchOfSz(sz)+1]) == NULL) {
		return fFalse;
	}
	
	SzCopy(szRootSrc, sz);
	
	return fTrue;
	
}

/* ------------------------------------------------------------ */
/***	APST::FSetSzRootDst
**
**	Parameters:
**		sz		- root path string to set
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Store the specified string in szRootDst
*/

bool
APST::FSetSzRootDst(char * sz)
	{
	
	if (szRootDst != NULL) {
		delete [] szRootDst;
	}
	
	if ((szRootDst = new char[CchOfSz(sz)+1]) == NULL) {
		return fFalse;
	}
	
	SzCopy(szRootDst, sz);
	
	return fTrue;
	
}

/* ------------------------------------------------------------ */
/***	APST::FSetSzRootXtr
**
**	Parameters:
**		sz		- root path string to set
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Store the specified string in szRootXtr
*/

bool
APST::FSetSzRootXtr(char * sz)
	{
	
	if (szRootXtr != NULL) {
		delete [] szRootXtr;
	}
	
	if ((szRootXtr = new char[CchOfSz(sz)+1]) == NULL) {
		return fFalse;
	}
	
	SzCopy(szRootXtr, sz);
	
	return fTrue;
	
}

/* ------------------------------------------------------------ */
/***	APST::FSetSzFileCur
**
**	Parameters:
**		sz		- file name to set
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Store the specified string in szFileCur. This holds the file
**		name of the file currently being processed. This is only
**		set when a file processing error occurs so that the file
**		name can be printed as part of the error report.
*/

bool
APST::FSetSzFileCur(char * sz)
	{
	
	if (szFileCur != NULL) {
		delete [] szFileCur;
	}
	
	if ((szFileCur = new char[CchOfSz(sz)+1]) == NULL) {
		return fFalse;
	}
	
	SzCopy(szFileCur, sz);
	
	return fTrue;
	
}

/* ------------------------------------------------------------ */
/***	APST::FSetSzCmdPath
**
**	Parameters:
**		sz		- path string to set
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Store the specified string in szCmdPath
*/

bool
APST::FSetSzCmdPath(char * sz)
	{
	
	if (szCmdPath != NULL) {
		delete [] szCmdPath;
	}
	
	if ((szCmdPath = new char[CchOfSz(sz)+1]) == NULL) {
		return fFalse;
	}
	
	SzCopy(szCmdPath, sz);
	
	return fTrue;
	
}

/* ------------------------------------------------------------ */
/***	APST::FSetSzLogPath
**
**	Parameters:
**		sz		- path string to set
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Store the specified string in szLogPath
*/

bool
APST::FSetSzLogPath(char * sz)
	{
	
	if (szLogPath != NULL) {
		delete [] szLogPath;
	}
	
	if ((szLogPath = new char[CchOfSz(sz)+1]) == NULL) {
		return fFalse;
	}
	
	SzCopy(szLogPath, sz);
	
	return fTrue;
	
}

/* ------------------------------------------------------------ */
/***	APST::FSetSzCommandFile
**
**	Parameters:
**		sz		- root path string to set
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Store the specified string in szCommandFile
*/

bool
APST::FSetSzCommandFile(char * sz)
	{
	
	if (szCommandFile != NULL) {
		delete [] szCommandFile;
	}
	
	if ((szCommandFile = new char[CchOfSz(sz)+1]) == NULL) {
		return fFalse;
	}
	
	SzCopy(szCommandFile, sz);
	
	return fTrue;
	
}

/* ------------------------------------------------------------ */
/***	APST::FSetSzLogFile
**
**	Parameters:
**		sz		- root path string to set
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Store the specified string in szLogFile
*/

bool
APST::FSetSzLogFile(char * sz)
	{
	
	if (szLogFile != NULL) {
		delete [] szLogFile;
	}
	
	if ((szLogFile = new char[CchOfSz(sz)+1]) == NULL) {
		return fFalse;
	}
	
	SzCopy(szLogFile, sz);
	
	return fTrue;
	
}

/* ------------------------------------------------------------ */
/***	ProcName
**
**	Parameters:
**
**	Return Value:
**
**	Errors:
**
**	Description:
**
*/

/* ------------------------------------------------------------ */

/************************************************************************/

