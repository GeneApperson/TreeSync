/************************************************************************/
/*																		*/
/*	app.cpp	--	Implementation for APP Object Class						*/
/*																		*/
/************************************************************************/
/*	Author: 	Gene Apperson											*/
/*	Copyright 2010, Gene Apperson										*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*	This module contains the implementation for the APP object class.	*/
/*	This object class implements the top level application behavior.	*/
/*	There is every only a single static instance of this object class.	*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	12/28/2010(GeneA): created											*/
/*																		*/
/************************************************************************/

#define	WIN32_LEAN_AND_MEAN
#define	_CRT_SECURE_NO_WARNINGS

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include <windows.h>
#include <shlobj.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#include "../inc/dassert.h"
#include "../inc/dtypes.h"
#include "../inc/dutil.h"

#include "config.h"
#include "appdefs.h"
#include "apst.h"
#include "app.h"
#include "fldrmg.h"
#include "cmdx.h"

/* ------------------------------------------------------------ */
/*				Local Type and Constant Definitions				*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

__SetCurFile;

extern APP	app;
extern APST	apst;
extern CMDX cmdx;

FLDRMG	fldrmg;

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
/*				APP Object Class Implementation					*/
/* ------------------------------------------------------------ */
/***	APP::APP
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
**		Default constructor
*/

APP::APP() {


}

/* ------------------------------------------------------------ */
/***	APP::~APP
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
**		Default destructor
*/

APP::~APP() {

	AssertMsg(szMsg, "APP object not terminated before destruction");

	
}

/* ------------------------------------------------------------ */
/***	APP::FInit
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not.
**
**	Description:
**		Initialize the object for use after construction
*/

bool
APP::FInit()
	{
	
	if (!apst.FInit()) {
		apst.SetStProg(stInternalError);
		return fFalse;
	}

	if (!fldrmg.FInit()) {
		apst.SetStProg(stOutOfMemory);
		return fFalse;
	}	
	
	return fTrue;
	
}

/* ------------------------------------------------------------ */
/***	APP::Terminate
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
**		Prepare object for destruction. Free any dynamically
**		allocated resources owned by the object.
*/

void
APP::Terminate()
	{

	/* Release anything held by the APST object
	*/	
	apst.Terminate();

	/* Release anything held by the FLDRMG object
	*/
	fldrmg.Terminate();
	
}

/* ------------------------------------------------------------ */
/***	APP::FParseParameters
**
**	Parameters:
**		cszArg		- number of command line argument strings
**		rgszArg		- array of command line argument strings
**
**	Return Value:
**		Global variables initialized based on parsed parameters.
**
**	Errors:
**		Returns fTrue if successful, fFalse if parameter error
**
**	Description:
**		Parset the command line arguments and set up the global
**		state based on the parameters found.
*/

bool
APP::FParseParameters(int cszArg, char * rgszArg[])
	{
	int		isz;
	int		csz;
	int		cszPath;

	/* Set the default values for parameters with default values.
	*/
	apst.SetFDoSystem(fDoSystemDefault);
	apst.SetFDoHidden(fDoHiddenDefault);
	apst.SetFNoCopyFile(fDoNoCopyFileDefault);
	apst.SetFNoCopyDir(fDoNoCopyDirDefault);
	
	isz = 1;					//consume the first param, the command name
	csz = cszArg - 1;
	cszPath = 0;
	
	while (csz > 0) {
		/* Check if the user wants to perform update on destination folder.
		*/
		if (CompareSz(rgszArg[isz], "-u") == 0) {
			apst.SetFUpdateDst(fTrue);
		}
		
		/* Check if the user wants to extract unique files from destination.
		*/
		else if (CompareSz(rgszArg[isz], "-xu") == 0) {
			apst.SetFExtractUnique(fTrue);
			if (csz-- == 0) {
				goto lSyntaxError;
			}
			isz += 1;
			apst.FSetSzRootXtr(rgszArg[isz]);
		}
		
		/* Check if the user wants to extract changed files from destination.
		*/
		else if (CompareSz(rgszArg[isz], "-xc") == 0) {
			apst.SetFExtractDiff(fTrue);
			if (csz-- == 0) {
				goto lSyntaxError;
			}
			isz += 1;
			apst.FSetSzRootXtr(rgszArg[isz]);
		}
		
		/* Check if user wants to extract unique and changed files from destination
		*/
		else if (CompareSz(rgszArg[isz], "-xd") == 0) {
			apst.SetFExtractUnique(fTrue);
			apst.SetFExtractDiff(fTrue);
			if (csz-- == 0) {
				goto lSyntaxError;
			}
			isz += 1;
			apst.FSetSzRootXtr(rgszArg[isz]);
		}
		
		/* Check if user has specified input from a command file
		*/
		else if (CompareSz(rgszArg[isz], "-f") == 0) {
			apst.SetFCommandFile(fTrue);
			if (csz-- == 0) {
				goto lSyntaxError;
			}
			isz += 1;
			apst.FSetSzCommandFile(rgszArg[isz]);
		}
		
		/* Check if user has specified log file name
		*/
		else if (CompareSz(rgszArg[isz], "-log") == 0) {
			if (csz-- == 0) {
				goto lSyntaxError;
			}
			isz += 1;
			apst.FSetSzLogFile(rgszArg[isz]);
		}
		
		/* Check if the user wants to delete destination files not in source
		*/
		else if (CompareSz(rgszArg[isz], "-trim") == 0) {
			apst.SetFTrimDst(fTrue);
		}			
		
		/* Check if the user wants to process system files
		*/
		else if (CompareSz(rgszArg[isz], "-s") == 0) {
			apst.SetFDoSystem(fTrue);
		}

		/* Check if the user wants to suppress processing system files
		*/
		else if (CompareSz(rgszArg[isz], "-ns") == 0) {
			apst.SetFDoSystem(fFalse);
		}

		/* Check if the user wants to process hidden files.
		*/
		else if (CompareSz(rgszArg[isz], "-h") == 0) {
			apst.SetFDoHidden(fTrue);
		}

		/* Check if the user wants to suppress processing hidden files
		*/
		else if (CompareSz(rgszArg[isz], "-nh") == 0) {
			apst.SetFDoHidden(fFalse);
		}

		/* Check for options to print actions being performed.
		*/
		else if (CompareSz(rgszArg[isz], "-pf") == 0) {
			/* Print names of files being copied.
			*/
			apst.SetFPrintFile(fTrue);
		}

		else if (CompareSz(rgszArg[isz], "-pd") == 0) {
			/* Print names of directories being created.
			*/
			apst.SetFPrintDir(fTrue);
		}

		else if (CompareSz(rgszArg[isz], "-pa") == 0) {
			/* Turn on all print options.
			*/
			apst.SetFPrintFile(fTrue);
			apst.SetFPrintDir(fTrue);
		}

		/* Check for options to suppress copying files or directories.
		*/
		else if (CompareSz(rgszArg[isz], "-nc") == 0) {
			/* Suppress all copying.
			*/
			apst.SetFNoCopyFile(fTrue);
			apst.SetFNoCopyDir(fTrue);
		}

		else if (CompareSz(rgszArg[isz], "-nf") == 0) {
			/* Suppress copying files, but copy directories.
			*/
			apst.SetFNoCopyFile(fTrue);
		}

		/* Check if user wants to use file signatures to compare files
		** that are the same size, but have different dates.
		*/
		else if (CompareSz(rgszArg[isz], "-sig") == 0) {
			apst.SetFUseSignatures(fTrue);
		}
	
		/* Check if the user wants us to print the elapsed time.
		*/
		else if (CompareSz(rgszArg[isz], "-t") == 0) {
			apst.SetFShowTime(fTrue);
		}

		/* Check if user wants runtime statistics printed.
		*/
		else if (CompareSz(rgszArg[isz], "-st") == 0) {
			apst.SetFPrintStat(fTrue);
		}
		
		/* Check for print version number parameter.
		*/
		else if (CompareSz(rgszArg[isz], "-ver") == 0) {
			apst.SetFShowVer(fTrue);
		}
		
		/* Check if the user asked for the command line syntax
		*/
		else if (CompareSz(rgszArg[isz], "-?") == 0) {
			apst.SetFShowUseage(fTrue);
		}

		/* Check for unknown command line parameter.
		*/
		else if (*(rgszArg[isz]) == '-') {
			goto lSyntaxError;
		}
		
		/* Not one of the defined command line parameters preceded by a '-'
		*/
		else {
			/* If this is the first raw command line parameter string, it is the
			** source path.
			*/
			if (cszPath == 0) {
				if (!apst.FSetSzRootSrc(rgszArg[isz])) {
					goto lInternalError;
				}
				cszPath += 1;
			}
			else if (cszPath == 1) {
				if (!apst.FSetSzRootDst(rgszArg[isz])) {
					goto lInternalError;
				}
				cszPath += 1;
			}
			else {
				goto lSyntaxError;
			}
		}
		
		/* Go the the next paramter.
		*/
		csz -= 1;
		isz += 1;
	
	}

	/* If either of the extraction options are set, set the
	** flag to indicate that we are doing extraction.
	*/	
	if (apst.FExtractUnique() || apst.FExtractDiff()) {
		apst.SetFExtractDst(fTrue);
	}
	
	return fTrue;
	
lInternalError:
	/* Some internal error, such as out of memory, occured.
	*/
	printf ("internal program error");
	apst.SetStProg(stInternalError);
	goto lExit;

lSyntaxError:
	/* Syntax error on the command line.
	*/
	printf ("Syntax error on command line\n\n");
	apst.SetFShowUseage(fTrue);
	apst.SetStProg(stSyntaxError);
	goto lExit;
	
	
lExit:
	return fFalse;
	
}

/* ------------------------------------------------------------ */
/***	APP::ShowUseage
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
**		Prints a description of how to use the program to stdout
*/

void
APP::ShowUseage()
	{

	printf("\n");	
	printf("Tree Synchronization Utility, Rev %d.%d.%d\n", verMajor, verMinor, verBuild);
	printf("This program will merge the source tree into the destination tree\n");
	printf("ensuring that the destination tree contains the most recent version\n");
	printf("of any files in the source tree.\n");

	printf("\n");
	
	printf("ts [options] <source path> <destination path>\n");
	
	printf("\n");
	
	printf(" Options:\n");
	printf("  -f <file>\tperform operations specified by <file>\n");
	printf("  -log <file>\tspecify log file name\n");
	printf("  -u\t\tupdate destination tree with changes in source tree\n");
	printf("  -xu <path>\textract unique files:\n");
	printf("  \t\tcopy files in dst tree but not in src tree to <path>\n");
	printf("  -xc <path>\textract changed files:\n");
	printf("  \t\tcopy files in dst tree different from src tree to <path>\n");
	printf("  -xd <path>\textract changed and unique files:\n");
	printf("  \t\tsame as -xu plus -xc\n");
	printf("  -trim\t\tdelete files in destination tree that aren't in src tree\n");
	printf("  -pf\t\tprint names of files being copied\n");
	printf("  -pd\t\tprint names of directories being created\n");
	printf("  -pa\t\tturn on all print options\n");
	printf("  -st\t\tprint statistics\n");
	printf("  -cd\t\tcreate directories, but don't copy files\n");
	printf("  -nc\t\tdon't copy files or create directories\n");
	printf("  -s\t\tcopy system files\n");
	printf("  -ns\t\tdon't copy system files\n");
	printf("  -h\t\tcopy hidden files\n");
	printf("  -nh\t\tdon't copy hidden files\n");
	printf("  -t \t\tprint elapsed time\n");
	printf("  -ver\t\tprint program version number\n");

}

/* ------------------------------------------------------------ */
/***	APP::Run
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		Sets program status in apst and returns if error occurs
**
**	Description:
**		Execute the main program function
*/

void
APP::Run()
	{
	DWORD		ticStart;
	DWORD		ticEnd;
	__time64_t	tmmCur;


	/* Get the current time so that we can print elapsed time.
	*/
	_time64(&tmmCur);
	apst.SetTmmStart(tmmCur);
	
	ticStart = GetTickCount();
	
	/* Set up the paths to the default command file folder and the
	** log file folder.
	*/
	if (!FGetDocumentPaths()) {
		printf("Error initializing program\n");
		return;
	}
	
	/* Determine what major operation is being performed and
	** create the appropriate object to start the process.
	*/
	if (apst.FCommandFile()) {
		/* Run the startup command file first, if there is one accessible.
		*/
		ProcessCommandFile("startup.ts");
		if ((apst.ErrLast() != errOK) && (apst.ErrLast() != errFileNotFound)) {
			return;
		}
		apst.SetErrLast(errOK);

		/* Run the primary command file.
		*/
		ProcessCommandFile(apst.SzCommandFile());
		if (apst.ErrLast() == errFileNotFound) {
			printf("File not found: %s\n", apst.SzCommandFile());
		}
	}
	else {
		ProcessCommandLine();
	}
	
	/* Get the current tick count so we can print the elapsed time.
	*/
	ticEnd = GetTickCount();
	
	if (apst.FShowTime()) {
		printf("\nTotal Elapsed time: %f\n", (double)(ticEnd-ticStart)/1000.0);
	}
	
	return;

}

/* ------------------------------------------------------------ */
/***	APP::ProcessCommandFile
**
**	Parameters:
**		szFile		- file name of command file to execute
**
**	Return Value:
**		none
**
**	Errors:
**		Error message printed on error
**
**	Description:
**		Process the commands in the command file specified on the
**		command line.
*/

void
APP::ProcessCommandFile(char * szFile)
	{
	char	szCmd[MAX_PATH];

	/* See the command file exists and get the complete path to
	** its location.
	*/
	if (!FFindCommandFile(szFile, szCmd)) {
		return;
	}		

	/* Execute the command file.
	*/
	if (!cmdx.FInit()) {
		goto lInternalError;
	}
	cmdx.FExecute(szCmd);
	cmdx.Terminate();
	
	return;
			
lInternalError:
	apst.SetErrLast(errInternalError);
	printf("Internal error, unable to initialize program\n");
	return;	
}

/* ------------------------------------------------------------ */
/***	APP::FFindCommandFile
**
**	Parameters:
**		szFile		- file name of command file to execute
**		szCmd		- buffer to return complete file path
**
**	Return Value:
**		none
**
**	Errors:
**		Return fTrue if successful, fFalse if not
**
**	Description:
**		Search for a command file with the specified name and if
**		found, fill in szCmd with the complete path to the file.
*/

bool
APP::FFindCommandFile(char * szFile, char * szCmd)
	{
	FILE *	fh;
	errno_t	errno;

	/* Check in the current directory first.
	*/	
	SzCopy(szCmd, szFile);
	if ((errno = fopen_s(&fh, szCmd, "rt")) != 0) {
		if (errno != ENOENT) {
			apst.SetErrLast(errFileSystem);
			return fFalse;
		}
		/* Not in the current directory. If the default command
		** file folder exists, look there.
		** c:\Documents and Settings\<user>\My Documents\TreeSync\Cmd
		**/
		if (apst.SzCmdPath() != NULL) {
			SzCopy(szCmd, apst.SzCmdPath());
			SzAppendPathSeparator(SzEnd(szCmd));
			SzAppend(szCmd, szFile);
			
			if ((errno = fopen_s(&fh, szCmd, "rt")) != 0) {
				if (errno == ENOENT) {
					apst.SetErrLast(errFileNotFound);
				}
				else {
					apst.SetErrLast(errFileSystem);
				}
				return fFalse;
			}
		}
		else {
			apst.SetErrLast(errFileNotFound);
			return fFalse;
		}
	}
	fclose(fh);	
	return fTrue;
	
}

/* ------------------------------------------------------------ */
/***	APP::ProcessCommandLine
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		Prints error messages
**
**	Description:
**		Process the operation and parameters specified on the
**		command line.
*/

void
APP::ProcessCommandLine()
	{
	FLDR *	pfldr;


	/* Create the appropriate FLDR object for the requested action.
	*/
	if (apst.FUpdateDst()) {	
		pfldr = new FLDS;
		if (pfldr == NULL) {
			goto lOutOfMemory;
		}
	}
	else if (apst.FExtractUnique() || apst.FExtractDiff()) {
		pfldr = new FLDX;
		if (pfldr == NULL) {
			goto lOutOfMemory;
		}
		((FLDX *)pfldr)->SetPathXtr(apst.SzRootXtr());
	}
	else if (apst.FTrimDst()) {
		pfldr = new FLDT;
		if (pfldr == NULL) {
			goto lOutOfMemory;
		}
	}
	else {
		printf("No operation specified\n");
		printf("Must specify -u -xu -xc -xd or -trim\n");
		apst.SetStProg(stSyntaxError);
		return;
	}

	/* Assign the root source and destination paths.
	*/
	pfldr->SetPathSrc(apst.SzRootSrc());
	pfldr->SetPathDst(apst.SzRootDst());
	
	/* Perform the action specified on the command line.
	*/
	FDoAction(pfldr);
	
	/* Print useage statistics if requested.
	*/
	PrintStatistics();
	
	return;
	
lOutOfMemory:
	apst.SetStProg(stOutOfMemory);
	printf("Unable to initialize program, out of memory\n");
	return;
	
}

/* ------------------------------------------------------------ */
/***	APP::FDoAction
**
**	Parameters:
**		pfldr		- initial folder object
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Perform the action implied by the given FLDR object
*/

bool
APP::FDoAction(FLDR * pfldr)
	{
	bool	fStat;
	
	fldrmg.BeginDirectory();
	fldrmg.AddPfldr(pfldr);
	fldrmg.EndDirectory();
	
	fStat = fldrmg.FProcess();
	
	return fStat;
}

/* ------------------------------------------------------------ */
/***	APP::PrintMessage
**
**	Parameters:
**		idMsg		- id code for type of message
**		szMsg		- the message string
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Print the message string if printing the
**		particular type of message is enabled
*/

void
APP::PrintMessage(int idMsg, char * szMsg)
	{

	
	/* Determine what type of message this is. If logging of that
	** type of message is enabled, write it to the log file.
	*/
	switch(idMsg) {
		case idMsgError:
			printf("%s", szMsg);
			break;
			
		case idMsgCreateDir:
			if (apst.FPrintDir()) {
				printf("%s", szMsg);
			}
			break;
			
		case idMsgSkipDir:
			if (apst.FPrintXclDir()) {
				printf("%s", szMsg);
			}
			break;
			
		case idMsgCopyFile:
			if (apst.FPrintFile()) {
				printf("%s", szMsg);
			}
			break;
			
		case idMsgExtrFile:
			if (apst.FPrintFile()) {
				printf("%s", szMsg);
			}
			break;
			
		case idMsgDelFile:
			if (apst.FPrintFile()) {
				printf("%s", szMsg);
			}
			break;
			
		case idMsgExclFile:
			if (apst.FPrintXclFile()) {
				printf("%s", szMsg);
			}
			break;
			
		case idMsgSetTime:
			if (apst.FPrintFile()) {
				printf("%s", szMsg);
			}
			break;
			
		default:
			Assert(fFalse, "Unknown log message type");
			
	}
}

/* ------------------------------------------------------------ */
/***	APP::PrintStatistics
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
**		Print statistics for the operation.
*/

void
APP::PrintStatistics()
	{
	
	if (apst.FPrintStat()) {
		printf("\n");
		fldrmg.PrintStat(stdout);
		printf("\n");
	}
	
}

/* ------------------------------------------------------------ */
/***	APP::FGetDocumentPaths
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful. fFalse if system errors occur.
**
**	Description:
**		Get the paths to the application data folders
**
**		C:\My Documents\<user>\My Documents\TreeSync\Cmd
**		c:\My Documents\<user>\My Documents\TreeSync\Log
**
**		If these folders exist, the program will look for default
**		Init and Terminate command files in the Cmd folder and
**		will place generated log files in the Log folder.
*/

bool
APP::FGetDocumentPaths()
	{
	HANDLE		fh;
	HRESULT		hres;
	DWORD		err;
	char		szUsrPath[MAX_PATH];
	char		szCmdPath[MAX_PATH];
	char		szLogPath[MAX_PATH];

	/* Get the path to the user's My Documents folder.
	*/	
	FillMem(MAX_PATH, (BYTE *)(&szUsrPath[0]), 0x00);	
	hres = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, szUsrPath);
	
	/* See if the Log path exists and set the path name if it does.	
	*/
	if (hres == S_OK) {
		SzCopy(szLogPath, szUsrPath);
		SzAppend(szLogPath, "\\TreeSync\\Log");

		fh = CreateFile(szLogPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
					FILE_ATTRIBUTE_READONLY|FILE_FLAG_BACKUP_SEMANTICS, NULL);
		if (fh != INVALID_HANDLE_VALUE) {			
			CloseHandle(fh);
			if (!apst.FSetSzLogPath(szLogPath)) {
				goto lMemError;
			}
		}
		else {
			err = GetLastError();
			if ((err != ERROR_FILE_NOT_FOUND) && (err != ERROR_PATH_NOT_FOUND)) {
				goto lFileError;
			}
		}	
	}

	/* See if the Cmd path exists and set the path name if it does.	
	*/
	if (hres == S_OK) {
		SzCopy(szCmdPath, szUsrPath);
		SzAppend(szCmdPath, "\\TreeSync\\Cmd");

		fh = CreateFile(szCmdPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
					FILE_ATTRIBUTE_READONLY|FILE_FLAG_BACKUP_SEMANTICS, NULL);
		if (fh != INVALID_HANDLE_VALUE) {			
			CloseHandle(fh);
			if (!apst.FSetSzCmdPath(szCmdPath)) {
				goto lMemError;
			}
		}
		else {
			err = GetLastError();
			if ((err != ERROR_FILE_NOT_FOUND) && (err != ERROR_PATH_NOT_FOUND)) {
				goto lFileError;
			}
		}	
	}
	
	return fTrue;
	
lFileError:
	apst.SetStProg(stFileSystemError);
	return fFalse;

lMemError:
	apst.SetStProg(stOutOfMemory);
	return fFalse;

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

