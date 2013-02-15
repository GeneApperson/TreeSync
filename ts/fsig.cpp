/************************************************************************/
/*                                                                      */
/*  FILENAME.C	--  Standard C Source File Format						*/
/*                                                                      */
/************************************************************************/
/*	Author: 															*/
/*	Copyright:															*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*                                                                      */
/************************************************************************/

#define	WIN32_LEAN_AND_MEAN
#define	_CRT_SECURE_NO_WARNINGS

/* ------------------------------------------------------------ */
/*					Include File Definitions					*/
/* ------------------------------------------------------------ */

#include <windows.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>

#include "../inc/dassert.h"
#include "../inc/dtypes.h"
#include "../inc/dutil.h"

#include "config.h"
#include "appdefs.h"

#include "../inc/Crc32Gen.h"

#include "fsig.h"

/* ------------------------------------------------------------ */
/*					Local Type Definitions						*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*					 Global Variables							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*					Local Variables								*/
/* ------------------------------------------------------------ */

__SetCurFile;

/* ------------------------------------------------------------ */
/*					Forward Declarations						*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*					Procedure Definitions						*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				FSIG Object Class Implementation				*/
/* ------------------------------------------------------------ */
/***	FSIG::FSIG
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Default constructor.
*/

FSIG::FSIG()
	{

	fhFile = NULL;
	cbBuf = 0;
	pbBuf = NULL;
	sig = 0;

}

/* ------------------------------------------------------------ */
/***	FSIG::~FSIG
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		nonf
**
**	Description:
**		Default destructor
*/

FSIG::~FSIG()
	{
	AssertMsg(szMsg, "FSIG not terminated before destruction");

	Assert(fhFile == NULL, szMsg);
	Assert(pbBuf == NULL, szMsg);
}

/* ------------------------------------------------------------ */
/***	FSIG::FInit
**
**	Parameters:
**		szFile		- file name of file to compute signature for
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Initialize for operation. This will open the specified
**		file for computing the signature. It allocates a default
**		sized buffer for reading the file while computing the
**		signature.
*/

bool
FSIG::FInit(char * szFile)
	{
	BYTE *		pb;

	pb = new BYTE[cbReadChunk];
	if (pb == NULL) {
		return fFalse;
	}

	return FInit(szFile, cbReadChunk, pb);

}

/* ------------------------------------------------------------ */
/***	FSIG::FInit
**
**	Parameters:
**		szFile		- file name of file to compute signature for
**		cb			- size of file buffer
**		pb			- buffer to use while processing file
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Initialize for operation. This will open the specified
**		file for computing the signature.
*/

bool
FSIG::FInit(char * szFile, int cb, BYTE * pb)
	{

	cbBuf = cb;
	pbBuf = pb;

	/* Initialize the CRC value.
	*/
	Crc32Init(&sig);
	
	/* Open the requested file.
	*/
	fhFile = fopen(szFile, "rb");

	if (fhFile == NULL) {
		printf("Error opening file: %s\n", szFile);
		return fFalse;
	}
		
	return fTrue;
		
}

/* ------------------------------------------------------------ */
/***	FSIG::Terminate
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Cleans up object after use.
*/

void
FSIG::Terminate()
	{

	if (fhFile != NULL) {
		fclose(fhFile);
	}
	fhFile = NULL;

	pbBuf = NULL;

}

/* ------------------------------------------------------------ */
/***	FSIG::FGenerate
**
**	Parameters:
**		psig		- pointer to variable to receive signature
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Compute the signature for the file.
*/

bool
FSIG::FGenerate(DWORD * psig)
	{
	int		cb;
	bool	fStat;

	fStat = fFalse;

	/* Read the contents of the file and generate the CRC
	*/
	while (!feof(fhFile)) {
		/* Read the next chunk of data from the file.
		*/
		cb = fread(pbBuf, 1, cbBuf, fhFile);
		if (ferror(fhFile)) {
			printf ("*** Error reading file ***\n");
			Terminate();
			goto lErrorExit;
		}
		
		/* Accumulate this data into the sum.
		*/
		if (cb != 0) {
			Crc32Update(&sig, pbBuf, cb);
		}
	}
	
	/* Complete the generation of the CRC value.
	*/
	Crc32Finish(&sig);

	/* Store the return value.
	*/
	*psig = sig;
	fStat = fTrue;

lErrorExit:
	return fStat;

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

