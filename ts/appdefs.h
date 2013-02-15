/************************************************************************/
/*																		*/
/*	appdefs.h	--	General Application Specific Symbol Declarations	*/
/*																		*/
/************************************************************************/
/*	Author:		Gene Apperson											*/
/*	Copyright 2010, Digilent Inc.										*/
/************************************************************************/
/*  File Description:													*/
/*																		*/
/*	Define various application specific symbols							*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	12/28/2010(GeneA): created											*/
/*																		*/
/************************************************************************/

#if !defined(APPDEFS_INC)
#define APPDEFS_INC

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */

/* Set user interface defaults.
*/
const bool fDoSystemDefault = fTrue;
const bool fDoHiddenDefault = fTrue;
const bool fDoNoCopyFileDefault = fFalse;
const bool fDoNoCopyDirDefault = fFalse;

/* Configuration value for file signature generation. This sets the
** size of the buffer used to read the file.
*/
const int	cbReadChunk		= 256*1024;

/* Set the size of the line buffer used to parse command files.
*/
const int	cchLexrLineBuf	= 1024;

/* Set the limit on nested include files.
*/
const int	cstmgLexrIncludeMax	= 8;

/* Size of message buffer for log messages.
*/
const int	cchLogMsgMax	= MAX_PATH+256;

/* Maximum sizes of a symbol name and a string value
*/
const int	cchSymbolMax	= 64;
const int	cchStringMax	= MAX_PATH;

/* Maximum length of a date or time string.
*/
const int	cchDateSzMax	= 64;
const int	cchTimeSzMax	= 64;

/* Message codes for types of messages that can be printed or logged
*/
const int	idMsgError		= 1;	//error messages always get printed
const int	idMsgCreateDir	= 2;	//creating directory
const int	idMsgSkipDir	= 3;	//skipping directory
const int	idMsgCopyFile	= 4;	//copying file from SRC to DST
const int	idMsgExtrFile	= 5;	//copying file from DST to XTR
const int	idMsgDelFile	= 6;	//deleting file from DST
const int	idMsgExclFile	= 7;	//skipping file
const int	idMsgSetTime	= 8;	//setting time on destination file
const int	idMsgAction		= 9;	//action being performed

/* General Error codes.
*/
const int	errPathLength		= 1;	//path length exceeds MAX_PATH
const int	errOutOfMemory		= 2;
const int	errFileSystem		= 3;	//error other than not found
const int	errFileNotFound		= 4;
const int	errPathNotFound		= 5;
const int	errInternalError	= 6;

/* Application specific error codes.
*/
const int	errAppBase			= 0x0100;
const int	errAppLast			= 0x01FF;
const int	errAppSyntax		= errAppBase+2; //syntax error in command file
const int	errAppParam			= errAppBase+3;	//error on comamnd line
const int	errAppNoSrcPath		= errAppBase+4;
const int	errAppNoDstPath		= errAppBase+5;
const int	errAppNoXtrPath		= errAppBase+6;
const int	errAppStringTooLong	= errAppBase+7;

/* ------------------------------------------------------------ */
/*					General Type Declarations					*/
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



/* ------------------------------------------------------------ */

#endif

/************************************************************************/
