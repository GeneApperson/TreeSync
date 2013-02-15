/************************************************************************/
/*																		*/
/*	fldrmg.cpp	--	File Folder Processing Manager						*/
/*																		*/
/************************************************************************/
/*	Author: 	Gene Apperson											*/
/*	Copyright 2011, Gene Apperson										*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	02/27/2011(GeneA): created											*/
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

#include "config.h"
#include "appdefs.h"

#include "apst.h"
#include "fldrmg.h"
#include "app.h"
#include "cmdx.h"

#include "../inc/Crc32Gen.h"

#include "fsig.h"

/* ------------------------------------------------------------ */
/*				Local Type and Constant Definitions				*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

__SetCurFile;

extern APST		apst;
extern APP		app;
extern FLDRMG	fldrmg;
extern CMDX		cmdx;

/* ------------------------------------------------------------ */
/*				Local Variables									*/
/* ------------------------------------------------------------ */

char szFindAll[] = "\\*.*";

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				FLDRMG Object Class Implementation				*/
/* ------------------------------------------------------------ */
/***	FLDRMG::FLDRMG
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

FLDRMG::FLDRMG()
	{

	flg.Reset();
	
}

/* ------------------------------------------------------------ */
/***	FLDRMG::~FLDRMG
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

FLDRMG::~FLDRMG()
	{
	AssertMsg(szMsg, "FLDRMG not terminated before destruction");
	
	Assert(pbSigBuf == NULL, szMsg);
	
}

/* ------------------------------------------------------------ */
/***	FLDRMG::FInit()
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if error
**
**	Description:
**		Initialize the object for use after construction.
*/

bool
FLDRMG::FInit()
	{
	BYTE *	pb;
	
	flg.Reset();

	pb = new BYTE[cbReadChunk];
	if (pb == NULL) {
		apst.SetStProg(stOutOfMemory);
		return fFalse;
	}

	flg.Set(flgFldrmgOwnBuf);
	
	return FInit(cbReadChunk, pb);

}

/* ------------------------------------------------------------ */
/***	FLDRMG::FInit
**
**	Parameters:
**		cbBuf		- size of file buffer
**		pbBuf		- buffer to use when computing file signatures
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if error
**
**	Description:
**		Initialize the object for use after construction.
*/

bool
FLDRMG::FInit(int cbBuf, BYTE * pbBuf)
	{

	cbSigBuf = cbBuf;
	pbSigBuf = pbBuf;
	ClearStat();
	ClearXclExt();
	ClearXclFile();
	ClearXclDir();

	return fTrue;

}

/* ------------------------------------------------------------ */
/***	FLDRMG::Terminate
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
**		Prepare object for destruction.
*/

void
FLDRMG::Terminate()
	{

	if ((pbSigBuf != NULL) && flg.FTst(flgFldrmgOwnBuf)) {
		delete [] pbSigBuf;
	}
	pbSigBuf = NULL;
	
}

/* ------------------------------------------------------------ */
/***	FLDRMG::FProcess
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
**		Process the set of folders current contained in the
**		FLDR list.
*/

bool
FLDRMG::FProcess()
	{
	FLDR *	pfldr;
	
	while (!FIsEmpty()) {
		pfldr = fldrmg.PfldrGetNext();
		if (!pfldr->FProcess()) {
			return fFalse;
		}
	}

	return fTrue;
	
}

/* ------------------------------------------------------------ */
/***	FLDRMG::BeginDirectory
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
**		Start the processing of a new directory.
*/

void
FLDRMG::BeginDirectory()
	{
	Assert(lfldrCur.size() == 0, "lfldCur not empty");
	
	lfldrCur.clear();
		
}

/* ------------------------------------------------------------ */
/***	FLDRMG::EndDirectory
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
**		Complete the processing of a directory
*/

void
FLDRMG::EndDirectory()
	{
	int		cfldrT;
	int		cfldrCur;

	cfldrCur = lfldrCur.size();
	
	lfldrMain.splice(lfldrMain.begin(), lfldrCur);

	cfldrTotal += cfldrCur;
	cfldrT = lfldrMain.size();
	if (cfldrT > cfldrDepthMax) {
		cfldrDepthMax = cfldrT;
	}	
}

/* ------------------------------------------------------------ */
/***	FLDRMG::AddPfldr
**
**	Parameters:
**		pfldr		- pointer to folder to add to list
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Add the specified folder to the list of folders to process
*/

void
FLDRMG::AddPfldr(FLDR * pfldr)
	{

	lfldrCur.insert(lfldrCur.end(), pfldr);

}

/* ------------------------------------------------------------ */
/***	FLDMG::PfldrGetNext
**
**	Parameters:
**		none
**
**	Return Value:
**		Pointer to the next folder on the folder list
**
**	Errors:
**		none
**
**	Description:
**		This function returns the next folder on the folder list
**		and removes it from the list.
*/

FLDR *
FLDRMG::PfldrGetNext()
	{
	FLDR *	pfldrT;
	
	pfldrT = lfldrMain.front();
	lfldrMain.pop_front();

	return pfldrT;

}

/* ------------------------------------------------------------ */
/***	FLDRMG::AddXclExt
**
**	Parameters:
**		szExt		- file extension to add
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Add the specified file extension to the file extension
**		exclusion set.
*/

void
FLDRMG::AddXclExt(char * szExt)
	{
	string	stExt;
	
	stExt = szExt;
	mpstXclExt[stExt] = 1;

}

/* ------------------------------------------------------------ */
/***	FLDRMG::AddXclFile
**
**	Parameters:
**		szFile		- file name to add
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Add the specified file name to the file name
**		exclusion set.
*/

void
FLDRMG::AddXclFile(char * szFile)
	{
	string stFile;
	
	stFile = szFile;
	mpstXclFile[stFile] = 1;
}

/* ------------------------------------------------------------ */
/***	FLDRMG::AddXclDir
**
**	Parameters:
**		szDir		- directory name to add
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Add the specified directory name to the exclusion set.
*/

void
FLDRMG::AddXclDir(char * szDir)
	{
	string stDir;
	
	stDir = szDir;
	mpstXclDir[stDir] = 1;
}

/* ------------------------------------------------------------ */
/***	FLDRMG::AddXclPath
**
**	Parameters:
**		szPath		- path name to add
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Add the specified path to the exclusion set.
*/

void
FLDRMG::AddXclPath(char * szPath)
	{
	string stPath;
	
	stPath = szPath;
	mpstXclPath[stPath] = 1;
}

/* ------------------------------------------------------------ */
/***	FLDRMG::FMatchXclExt
**
**	Parameters:
**		szExt		- file extension to match
**
**	Return Value:
**		Returns fTrue if match found, fFalse if not
**
**	Errors:
**		none
**
**	Description:
**		Search the file extension exclusion list for a match
*/

bool
FLDRMG::FMatchXclExt(char * szExt)
	{
	string	stExt;

	if ((szExt == NULL) || CchOfSz(szExt) == 0) {
		return fFalse;
	}	
	stExt = szExt;
	return (mpstXclExt.find(stExt) != mpstXclExt.end());	
}

/* ------------------------------------------------------------ */
/***	FLDRMG::FMatchXclFile
**
**	Parameters:
**		szFile		- file name to match
**
**	Return Value:
**		Returns fTrue if match found, fFalse if not
**
**	Errors:
**		none
**
**	Description:
**		Search the file name exclusion list for a match
*/

bool
FLDRMG::FMatchXclFile(char * szFile)
	{
	string	stFile;

	if ((szFile == NULL) || CchOfSz(szFile) == 0) {
		return fFalse;
	}	
	stFile = szFile;
	return (mpstXclFile.find(stFile) != mpstXclFile.end());		
}

/* ------------------------------------------------------------ */
/***	FLDRMG::FMatchXclDir
**
**	Parameters:
**		szDir		- directory to match
**
**	Return Value:
**		Returns fTrue if match found, fFalse if not
**
**	Errors:
**		none
**
**	Description:
**		Search the directory exclusion list for a match
*/

bool
FLDRMG::FMatchXclDir(char * szDir)
	{
	string	stDir;

	if ((szDir == NULL) || CchOfSz(szDir) == 0) {
		return fFalse;
	}	
	stDir = szDir;
	return (mpstXclDir.find(stDir) != mpstXclDir.end());	
}

/* ------------------------------------------------------------ */
/***	FLDRMG::FMatchXclPath
**
**	Parameters:
**		szPath		- path to match
**
**	Return Value:
**		Returns fTrue if match found, fFalse if not
**
**	Errors:
**		none
**
**	Description:
**		Search the path exclusion list for a match
*/

bool
FLDRMG::FMatchXclPath(char * szPath)
	{
	string	stPath;

	if ((szPath == NULL) || CchOfSz(szPath) == 0) {
		return fFalse;
	}	
	stPath = szPath;
	return (mpstXclPath.find(stPath) != mpstXclPath.end());	
}

/* ------------------------------------------------------------ */
/***	FLDRMG::PrintStat
**
**	Parameters:
**		fhOut		- output stream to print to
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
FLDRMG::PrintStat(FILE * fhOut)
	{
	
	fprintf(fhOut, "Program Statistics:\n");
	fprintf(fhOut, "  Directories processed:   %d\n", fldrmg.CfldrTotal());
	fprintf(fhOut, "  Directories created:     %d\n", fldrmg.CntDirCreated());
	fprintf(fhOut, "  Files processed:         %d\n", fldrmg.CntFileProcessed());
	fprintf(fhOut, "  Files copied:            %d\n", fldrmg.CntFileCopy());
	fprintf(fhOut, "  Files deleted:           %d\n", fldrmg.CntFileDelete());
	fprintf(fhOut, "  Files excluded:          %d\n", fldrmg.CntFileExcluded());
	fprintf(fhOut, "  Directories skipped:     %d\n", fldrmg.CntDirExcluded());
	fprintf(fhOut, "  Max pending directories: %d\n", fldrmg.CfldrDepthMax());
	fprintf(fhOut, "  Maximum path length:     %d", fldrmg.CchPathMax());
	
}

/* ------------------------------------------------------------ */
/***	FLDRMG::ClearStat
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
**		Clear the statistics accumulation variables.
*/

void
FLDRMG::ClearStat()
	{
	
	cfldrTotal = 0;
	cfldrDepthMax = 0;
	cntFileProcessed = 0;
	cntFileCopy = 0;
	cntFileDelete = 0;
	cntDirCreated = 0;
	cntXclFile = 0;
	cntXclDir = 0;
	cchPathMax = 0;
	
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
/*				FLDR Object Class Implementation				*/
/* ------------------------------------------------------------ */
/***	FLDR::FLDR
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

FLDR::FLDR()
	{

}

/* ------------------------------------------------------------ */
/***	FLDR::~FLDR
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

FLDR::~FLDR()
	{
	AssertMsg(szMsg, "FLDR object not terminated before destruction");

}

/* ------------------------------------------------------------ */
/***	FLDR::FInit
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
**		Initialize object for user after construction
*/

bool
FLDR::FInit()
	{

	szPathSrc[0] = '\0';
	szPathDst[0] = '\0';

	return fTrue;

}

/* ------------------------------------------------------------ */
/***	FLDR::Terminate
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
**		Prepare object for destruction
*/

void
FLDR::Terminate()
	{

}

/* ------------------------------------------------------------ */
/***	FLDR::FProcess()
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
**		Process this folder. Find all of the files and directories
**		in thie folder. Add each directory to the folder list. For
**		each file, check length, file signature, and date. For files
**		that differ, copy the newest from the source tree to the
**		destination tree.
*/

bool
FLDR::FProcess()
	{
	bool			fStat;
	char			szPath[MAX_PATH];
	char			szName[MAX_PATH];
	char			szMsg[cchLogMsgMax];
	HANDLE			hfind;
	WIN32_FIND_DATA	ffdt;

	fStat = fFalse;

	if (CchOfSz(szPathSrc) == 0) {
		apst.SetErrLast(errAppNoSrcPath);
		return fStat;
	}

	if (CchOfSz(szPathDst) == 0) {
		apst.SetErrLast(errAppNoDstPath);
		return fStat;
	}

	/* Determine whether we are walking the source tree or the destinaton tree,
	** and initialize szPath appropriately.
	*/
	if (apst.FUpdateDst()) {
		/* If we are performing an update operation, we need to walk the
		** source tree.
		*/
		SzCopy(szPath, szPathSrc);
	}
	else if (apst.FTrimDst()) {
		/* When deleting files from the destination tree that aren't in the
		** source tree, we need to walk the destination tree.
		*/
		SzCopy(szPath, szPathDst);
	}
	else if (apst.FExtractDst()) {
		/* When performing an extract operation, we need to walk the
		** destination tree.
		*/
		SzCopy(szPath, szPathDst);
	}
	else {
		/* Internal error. We shouldn't be able to get here without one
		** of the above conditions being true.
		*/
		apst.SetErrLast(errInternalError);
		return fStat;
	}

	if ((CchOfSz(szPath)+CchOfSz(szFindAll)) > MAX_PATH) {
		/* The path name is too long to process.
		*/
		apst.SetErrLast(errPathLength);
		sprintf_s(szMsg, cchLogMsgMax, "Path name too long: %s\n", szPath);
		app.PrintMessage(idMsgError, szMsg);
		cmdx.LogMessage(idMsgError, szMsg);
		return fStat;
	}	
	//SzAppendPathSeparator(SzEnd(szPath));
	SzAppend(szPath, szFindAll);

	if ((hfind = FindFirstFile(szPath, &ffdt)) == INVALID_HANDLE_VALUE) {
		apst.SetErrLast(errFileSystem);
		if (apst.ErrLast() == ERROR_PATH_NOT_FOUND) {
			sprintf_s(szMsg, cchLogMsgMax, "Path not found: %s\n", szPath);
			app.PrintMessage(idMsgError, szMsg);
			cmdx.LogMessage(idMsgError, szMsg);
		}
		return fStat;
	}
	
	fldrmg.BeginDirectory();

	do {
		/* Check for system files, and only process them if the parameter
		** was set on the command line.
		*/
		if ((ffdt.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0) {
			if (!apst.FDoSystem()) {
				continue;
			}
		}

		/* Check for hidden files and only process them if the parameter
		** was set on the command line.
		*/
		if ((ffdt.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0) {
			if (!apst.FDoHidden()) {
				continue;
			}
		}

		/* Check if this is a directory.
		*/
		if ((ffdt.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
			/* Is thie folder being excluded.
			*/
			if (fldrmg.FMatchXclDir(ffdt.cFileName)) {
				if (apst.FPrintXclDir() || apst.FLogXclDir()) {
					SzCopy(szName, szPathSrc);
					SzAppendPathSeparator(SzEnd(szName));					
					if (CchOfSz(szName)+CchOfSz(ffdt.cFileName) > MAX_PATH) {
						apst.SetStProg(stPathLength);
						sprintf_s(szMsg, cchLogMsgMax, "Path name too long: %s%s\n", szPathSrc, ffdt.cFileName);
						app.PrintMessage(idMsgSkipDir, szMsg);
						cmdx.LogMessage(idMsgSkipDir, szMsg);
						continue;
					}
					SzAppend(szName, ffdt.cFileName);					
					sprintf_s(szMsg, cchLogMsgMax, "Skipping: %s\n", szName);
					app.PrintMessage(idMsgSkipDir, szMsg);
					cmdx.LogMessage(idMsgSkipDir, szMsg);
				}
				fldrmg.DirExcluded();
				continue;
			}
			/* Add it to the list of folders to be processed.
			*/
			if (!FProcessDirectory(ffdt.cFileName)) {
				break;
			}
		}
		else {
			/* Process the file
			*/
			if (fldrmg.FMatchXclExt(SzExtOfName(ffdt.cFileName)) || 
			    fldrmg.FMatchXclFile(ffdt.cFileName)) {
			    if (apst.FPrintXclFile()|| apst.FLogXclFile()) {
					SzCopy(szName, szPathSrc);
					SzAppendPathSeparator(SzEnd(szName));					
					if (CchOfSz(szName)+CchOfSz(ffdt.cFileName) > MAX_PATH) {
						apst.SetStProg(stPathLength);
						sprintf_s(szMsg, cchLogMsgMax, "Path name too long: %s%s\n", szPathSrc, ffdt.cFileName);
						app.PrintMessage(idMsgSkipDir, szMsg);
						cmdx.LogMessage(idMsgSkipDir, szMsg);
						continue;
					}
					SzAppend(szName, ffdt.cFileName);					
					sprintf_s(szMsg, cchLogMsgMax, "Excluding: %s\n", szName);
					app.PrintMessage(idMsgExclFile, szMsg);
					cmdx.LogMessage(idMsgExclFile, szMsg);
				}
				fldrmg.FileExcluded();
				continue;
			}
			if (!FProcessFile(ffdt.cFileName)) {
				break;
			}
		}

	} while (FindNextFile(hfind, &ffdt) != 0);
	
	fldrmg.EndDirectory();

	if (GetLastError() == ERROR_NO_MORE_FILES) {
		fStat = fTrue;
	}

	FindClose(hfind);
	return fStat;
}

/* ------------------------------------------------------------ */
/***	FLDR::FindFsSrc
**
**	Parameters:
**		sz			- src path string
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Determine if the src is on a FAT file system and set the
**		flag in apst if so.
*/

void
FLDR::FindFsSrc(char * sz)
	{
	char	szPath[MAX_PATH];
	char	szFsName[MAX_PATH];
	char	szVolName[MAX_PATH];
	DWORD	snVolume;
	DWORD	cchMaxComponent;
	DWORD	fsFileSysFlags;
	
	SzCopy(szPath, sz);
	VolumeOfPath(szPath);
	if (!FIsNulSz(szPath)) {
		SzAppendPathSeparator(SzEnd(szPath));
		GetVolumeInformation(szPath, szVolName, MAX_PATH, &snVolume, 
			&cchMaxComponent, &fsFileSysFlags, szFsName, MAX_PATH);
		if ((ChMakeUpper(szFsName[0]) == 'F') &&
			(ChMakeUpper(szFsName[1]) == 'A') &&
			(ChMakeUpper(szFsName[2]) == 'T')) {
			
			apst.SetFSrcFsFat(fTrue);
		}	
	}	
}

/* ------------------------------------------------------------ */
/***	FLDR::FindFsDst
**
**	Parameters:
**		sz			- dst path string
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Determine if the dst is on a FAT file system and set the
**		flag in apst if so.
*/

void
FLDR::FindFsDst(char * sz)
	{
	char	szPath[MAX_PATH];
	char	szFsName[MAX_PATH];
	char	szVolName[MAX_PATH];
	DWORD	snVolume;
	DWORD	cchMaxComponent;
	DWORD	fsFileSysFlags;
	
	SzCopy(szPath, sz);
	VolumeOfPath(szPath);
	if (!FIsNulSz(szPath)) {
		SzAppendPathSeparator(SzEnd(szPath));
		GetVolumeInformation(szPath, szVolName, MAX_PATH, &snVolume, 
			&cchMaxComponent, &fsFileSysFlags, szFsName, MAX_PATH);
		if ((ChMakeUpper(szFsName[0]) == 'F') &&
			(ChMakeUpper(szFsName[1]) == 'A') &&
			(ChMakeUpper(szFsName[2]) == 'T')) {
			
			apst.SetFDstFsFat(fTrue);
		}	
	}	
}

/* ------------------------------------------------------------ */
/*				FLDS Object Class Implementation				*/
/* ------------------------------------------------------------ */
/***	FLDS::FLDS
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

FLDS::FLDS()
	{

}

/* ------------------------------------------------------------ */
/***	FLDS::~FLDS
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

FLDS::~FLDS()
	{
	AssertMsg(szMsg, "FLDR object not terminated before destruction");

}

/* ------------------------------------------------------------ */
/***	FLDS::FInit
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
**		Initialize object for user after construction
*/

bool
FLDS::FInit()
	{

	/* Call the base class FInit method to inherit the basic
	** behavior.
	*/
	if (!FLDR::FInit()) {
		return fFalse;
	}

	/* Perform the init for this class.
	*/

	return fTrue;

}

/* ------------------------------------------------------------ */
/***	FLDS::Terminate
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
**		Prepare object for destruction
*/

void
FLDS::Terminate()
	{

	/* Clean up for this class.
	*/

	/* Call the base class Terminate to clean it up.
	*/
	FLDR::Terminate();

}

/* ------------------------------------------------------------ */
/***	FLDS::FProcessDirectory
**
**	Parameters:
**		szDir		- directory name
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Process the given directory name.
*/

bool
FLDS::FProcessDirectory(char * szDir)
	{
	bool		fStat;
	FLDS *		pflds;
	HANDLE		fh;
	char 		szSrc[MAX_PATH];
	char 		szDst[MAX_PATH];
	char		szMsg[cchLogMsgMax];
	DWORD		err;

	fStat = fFalse;

	/* Don't add the '.' and '..' directories to the list.
	*/
	if ((CompareSz(szDir, ".") == 0) || (CompareSz(szDir, "..") == 0)) {
		return fTrue;
	}

	/* Set the source path string
	*/
	if ((CchOfSz(szPathSrc)+CchOfSz(szDir)+1) > MAX_PATH) {
		apst.SetErrLast(errPathLength);
		sprintf_s(szMsg, cchLogMsgMax, "Path name too long: %s\\%s\n", szPathSrc, szDir);
		app.PrintMessage(idMsgError, szMsg);
		cmdx.LogMessage(idMsgError, szMsg);
		goto lErrorExit;
	}
	SzCopy(szSrc, szPathSrc);
	SzAppendPathSeparator(SzEnd(szSrc));
	SzAppend(szSrc, szDir);

	/* Set up the destination path string.
	*/	
	if ((CchOfSz(szPathDst)+CchOfSz(szDir)+1) > MAX_PATH) {
		apst.SetErrLast(errPathLength);
		sprintf_s(szMsg, cchLogMsgMax, "Path name too long: %s\\%s\n", szPathDst, szDir);
		app.PrintMessage(idMsgError, szMsg);
		cmdx.LogMessage(idMsgError, szMsg);
		goto lErrorExit;
	}
	SzCopy(szDst, szPathDst);
	SzAppendPathSeparator(SzEnd(szDst));
	SzAppend(szDst, szDir);

	/* Check if this directory is one that is being excluded.
	*/	
	if (fldrmg.FMatchXclPath(szSrc)) {
		if (apst.FPrintXclDir() || apst.FLogXclDir()) {
			sprintf_s(szMsg, cchLogMsgMax, "Skipping: %s\n", szSrc);
			app.PrintMessage(idMsgSkipDir, szMsg);
			cmdx.LogMessage(idMsgSkipDir, szMsg);
		}
		fldrmg.DirExcluded();
		return fTrue;
	}

	/* Check that the destination directory exists, and create it if
	** it doesn't
	*/
	fh = CreateFile(szDst, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
				FILE_ATTRIBUTE_READONLY|FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if (fh == INVALID_HANDLE_VALUE) {

		err = GetLastError();
		if ((err != ERROR_FILE_NOT_FOUND) && (err != ERROR_PATH_NOT_FOUND)) {
			apst.SetStProg(stFileSystemError);
			goto lErrorExit;
		}

		if (apst.FPrintDir() || apst.FLogDir()) {
			sprintf_s(szMsg, cchLogMsgMax, "Creating: %s\n", szDst);
			app.PrintMessage(idMsgCreateDir, szMsg);
			cmdx.LogMessage(idMsgCreateDir, szMsg);
		}

		if (!apst.FNoCopyDir()) {
			fldrmg.DirCreated();
			if (!FCreatePath(szDst)) {
				apst.SetErrLast(errFileSystem);
				sprintf_s(szMsg, cchLogMsgMax, "Error creating directory: %s", szDst);
				app.PrintMessage(idMsgError, szMsg);
				cmdx.LogMessage(idMsgError, szMsg);
				goto lErrorExit;
			}
		}
	}
	else {
		CloseHandle(fh);
	}

	/* Create the new folder object.
	*/
	pflds = new FLDS;
	if (pflds == NULL) {
		apst.SetErrLast(errOutOfMemory);
		goto lErrorExit;
	}

	/* Set the path names into the FLDR object and add it to the list
	** of directories to be processed.
	*/
	pflds->SetPathSrc(szSrc);
	pflds->SetPathDst(szDst);

	fldrmg.AddPfldr(pflds);
	fStat = fTrue;

lErrorExit:
	return fStat;

}

/* ------------------------------------------------------------ */
/***	FLDS::FProcessFile
**
**	Parameters:
**		szFile		- file name of the file to process
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not.
**
**	Description:
**		Process the file.
*/

bool
FLDS::FProcessFile(char * szFile)
	{
	bool		fStat;
	char		szSrc[MAX_PATH];
	char		szDst[MAX_PATH];
	char		szMsg[cchLogMsgMax];
	HANDLE		hfindSrc;
	HANDLE		hfindDst;
	HANDLE		fhDst;
	WIN32_FIND_DATA	ffdtSrc;
	WIN32_FIND_DATA	ffdtDst;
	FSIG		fsig;
	bool		fSameSize;
	bool		fSameTime;
	int			dftTime;
	DWORD		sigSrc;
	DWORD		sigDst;
	DWORD		err;

	fStat = fFalse;
	fldrmg.FileProcessed();

	/* Set up the source file name.
	*/
	if ((CchOfSz(szPathSrc)+CchOfSz(szFile)+1) >= MAX_PATH) {
		apst.SetErrLast(errPathLength);
		sprintf_s(szMsg, cchLogMsgMax, "Path name too long: %s\\%s\n", szPathSrc, szFile);
		app.PrintMessage(idMsgError, szMsg);
		cmdx.LogMessage(idMsgError, szMsg);
		goto lErrorExit;
	}
	SzCopy(szSrc, szPathSrc);
	SzAppendPathSeparator(SzEnd(szSrc));
	SzAppend(szSrc, szFile);
	
	/* Set up the destination file name
	*/
	if ((CchOfSz(szPathDst)+CchOfSz(szFile)+1) >= MAX_PATH) {
		apst.SetErrLast(errPathLength);
		sprintf_s(szMsg, cchLogMsgMax, "Path name too long: %s\\%s\n", szPathDst, szFile);
		app.PrintMessage(idMsgError, szMsg);
		cmdx.LogMessage(idMsgError, szMsg);
		goto lErrorExit;
	}
	SzCopy(szDst, szPathDst);
	SzAppendPathSeparator(SzEnd(szDst));
	SzAppend(szDst, szFile);

	/* Accumulate the maximum path length.
	*/
	fldrmg.SetCchPath(CchOfSz(szSrc));
	fldrmg.SetCchPath(CchOfSz(szDst));

	/* Get information about the files to determine which is newer.
	*/
	hfindSrc = FindFirstFile(szSrc, &ffdtSrc);
	FindClose(hfindSrc);
	hfindDst = FindFirstFile(szDst, &ffdtDst);
	if (hfindDst == INVALID_HANDLE_VALUE) {
		err = GetLastError();
		if (err == ERROR_FILE_NOT_FOUND) { 
			goto lCopy;
		}
		else if (apst.FNoCopyDir() && (err == ERROR_PATH_NOT_FOUND)) {
			goto lNoCopy;
		}
		else {
			apst.SetErrLast(errFileSystem);
			goto lErrorExit;
		}
	}
	FindClose(hfindDst);

	/* Compare the file sizes.
	*/
	if ((ffdtSrc.nFileSizeHigh == ffdtDst.nFileSizeHigh) &&
		(ffdtSrc.nFileSizeLow  == ffdtDst.nFileSizeLow)) {
		fSameSize = fTrue;
	}
	else {
		fSameSize = fFalse;
	}

	/* Check if there is a FAT file system involved.
	*/
	if (apst.FSrcFsFat() || apst.FDstFsFat()) {
		/* Either the source or destination are on a FAT file system.
		** modify the times so that they will compare. the NTFS keeps
		** last write time at 10ms resolution, the FAT file system keeps
		** last write time as 2s resolution.
		*/
		ffdtSrc.ftLastWriteTime.dwLowDateTime &= 0xFE000000;
		ffdtDst.ftLastWriteTime.dwLowDateTime &= 0xFE000000;
	}
		
	/* Compare the file times
	** dftTime has the following meaning:
	**		-1	source file older than destination
	**		 0	source file and destination file have same times
	**		 1  source file newer than destination
	*/
	dftTime = CompareFileTime(&(ffdtSrc.ftLastWriteTime), &(ffdtDst.ftLastWriteTime));
	if (dftTime == 0) {
		fSameTime = fTrue;
	}
	else {
		fSameTime = fFalse;
	}

	/* Determine if we need to copy the source file over the destination file.
	*/
	if (apst.FUseSignatures()) {
		if (fSameSize) {
			/* When using file signatures:
			** If they are the same size and  have the same last write time,
			** they are identical, so don't copy.
			*/
			if (fSameTime) {
				goto lNoCopy;
			}

			/* When the files are the same size, but have different times,
			** check the signatures to see if they are identical.
			*/
			fsig.FInit(szSrc, fldrmg.CbSigBuf(), fldrmg.PbSigBuf());
			fsig.FGenerate(&sigSrc);
			fsig.Terminate();

			fsig.FInit(szDst, fldrmg.CbSigBuf(), fldrmg.PbSigBuf());
			fsig.FGenerate(&sigDst);
			fsig.Terminate();

			if (sigSrc == sigDst) {
				/* They have the same content. Update the destination file
				** to have the same times as the source file.
				*/
				goto lSetTime;
			}

			/* They don't have the same content, so check the last write
			** times to see whether or not to copy the source file.
			*/
			if (dftTime <= 0) {
				goto lNoCopy;
			}
		}
	}
	else {
		/* When not using file signatures:
		** Files that are the same size and have the same date and time
		** don't get copied.
		** If they are different size or have a different date and time,
		** copy the source file if it is newer.
		*/
		if (fSameTime & !fSameSize) {
			/* This shouldn't happen. The files have the same name and same
			** time, but are different size.
			*/
			printf("** Files have same modification data and time, but are different size:\n");
			printf("    %s\n", szSrc);
			printf("    %s\n", szDst);
			goto lNoCopy;
		}
		
		if (fSameTime & fSameSize) {
			goto lNoCopy;
		}

		if (dftTime <= 0) {
			goto lNoCopy;
		}
	}

	/* We haven't come up with a reason not to copy this file, so
	** the source file needs to be copied over the destination file.
	*/
	goto lCopy;

	/* The files are identical, but have different times.
	** Set the Creation Time and Last Write Time of the
	** destination file to match the source file.
	*/
lSetTime:
	if (apst.FPrintFile() || apst.FLogFile()) {
		sprintf_s(szMsg, cchLogMsgMax, "Setting time: %s\n", szDst);
		app.PrintMessage(idMsgCopyFile, szMsg);
		cmdx.LogMessage(idMsgCopyFile, szMsg);
	}

	if (!apst.FNoCopyFile()) {
		fhDst = CreateFile(szDst, GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
					0, NULL);
		SetFileTime(fhDst, &(ffdtSrc.ftCreationTime), NULL, &(ffdtSrc.ftLastWriteTime));
		CloseHandle(fhDst);
	}
	goto lNoCopy;

	/* Copy the source file to the destination.
	*/
lCopy:
	if (apst.FPrintFile() || apst.FLogFile()) {
		sprintf_s(szMsg, cchLogMsgMax, "Copying:  %s\n", szSrc);
		app.PrintMessage(idMsgCopyFile, szMsg);
		cmdx.LogMessage(idMsgCopyFile, szMsg);
	}

	if (!apst.FNoCopyFile()) {
		fldrmg.FileCopied();
		if (CopyFile(szSrc, szDst, fFalse) == 0) {
			apst.SetErrLast(errFileSystem);
			sprintf_s(szMsg, cchLogMsgMax, "Error copying file: %s\n", szSrc);
			app.PrintMessage(idMsgError, szMsg);
			cmdx.LogMessage(idMsgError, szMsg);
			goto lErrorExit;
		}
	}

lNoCopy:
	fStat = fTrue;

lErrorExit:
	apst.FSetSzFileCur(szFile);
	return fStat;

}

/* ------------------------------------------------------------ */
/*				FLDX Object Class Implementation				*/
/* ------------------------------------------------------------ */
/***	FLDX::FLDX
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

FLDX::FLDX()
	{

}

/* ------------------------------------------------------------ */
/***	FLDX::~FLDX
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

FLDX::~FLDX()
	{
	AssertMsg(szMsg, "FLDR object not terminated before destruction");

}

/* ------------------------------------------------------------ */
/***	FLDX::FInit
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
**		Initialize object for user after construction
*/

bool
FLDX::FInit()
	{

	/* Call the base class FInit method to inherit the basic
	** behavior.
	*/
	if (!FLDR::FInit()) {
		return fFalse;
	}

	/* Perform the init for this class.
	*/

	return fTrue;

}

/* ------------------------------------------------------------ */
/***	FLDX::Terminate
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
**		Prepare object for destruction
*/

void
FLDX::Terminate()
	{

	/* Clean up for this class.
	*/

	/* Call the base class Terminate to clean it up.
	*/
	FLDR::Terminate();

}

/* ------------------------------------------------------------ */
/***	FLDX::FindFsXtr
**
**	Parameters:
**		sz			- dst path string
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Determine if the xtr is on a FAT file system and set the
**		flag in apst if so.
*/

void
FLDX::FindFsXtr(char * sz)
	{
	char	szPath[MAX_PATH];
	char	szFsName[MAX_PATH];
	char	szVolName[MAX_PATH];
	DWORD	snVolume;
	DWORD	cchMaxComponent;
	DWORD	fsFileSysFlags;
	
	SzCopy(szPath, sz);
	VolumeOfPath(szPath);
	if (!FIsNulSz(szPath)) {
		SzAppendPathSeparator(SzEnd(szPath));
		GetVolumeInformation(szPath, szVolName, MAX_PATH, &snVolume, 
			&cchMaxComponent, &fsFileSysFlags, szFsName, MAX_PATH);
		if ((ChMakeUpper(szFsName[0]) == 'F') &&
			(ChMakeUpper(szFsName[1]) == 'A') &&
			(ChMakeUpper(szFsName[2]) == 'T')) {
			
			apst.SetFXtrFsFat(fTrue);
		}	
	}	
}

/* ------------------------------------------------------------ */
/***	FLDX::FProcessDirectory
**
**	Parameters:
**		szDir		- directory name
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Process the given directory name. We are walking the
**		destination folder tree, looking for files that either
**		don't exist in the source tree, or where the file in
**		the source tree is different and newer.
**		All we need to do here is to add this directory to the
**		directory list being generated for this level of the tree.
*/

bool
FLDX::FProcessDirectory(char * szDir)
	{
	bool		fStat;
	FLDX *		pfldx;
	char 		szSrc[MAX_PATH];
	char 		szDst[MAX_PATH];
	char		szXtr[MAX_PATH];
	char		szMsg[cchLogMsgMax];

	fStat = fFalse;

	/* Don't add the '.' and '..' directories to the list.
	*/
	if ((CompareSz(szDir, ".") == 0) || (CompareSz(szDir, "..") == 0)) {
		return fTrue;
	}

	/* Set up the destination path string.
	*/
	if ((CchOfSz(szPathDst)+CchOfSz(szDir)+1) >= MAX_PATH) {
		apst.SetErrLast(errPathLength);
		sprintf_s(szMsg, cchLogMsgMax, "Path name too long: %s\\%s\n", szPathDst, szDir);
		app.PrintMessage(idMsgError, szMsg);
		cmdx.LogMessage(idMsgError, szMsg);
		goto lErrorExit;
	}
	SzCopy(szDst, szPathDst);
	SzAppendPathSeparator(SzEnd(szDst));
	SzAppend(szDst, szDir);
	
	/* Check if this directory is one that is being excluded.
	*/	
	if (fldrmg.FMatchXclPath(szDst)) {
		if (apst.FPrintXclDir()) {
			printf("Skipping: %s\n", szDst);
		}
		fldrmg.DirExcluded();
		return fTrue;
	}

	/* Set the source path string
	*/
	if ((CchOfSz(szPathSrc)+CchOfSz(szDir)+1) >= MAX_PATH) {
		apst.SetErrLast(errPathLength);
		sprintf_s(szMsg, cchLogMsgMax, "Path name too long: %s\\%s\n", szPathSrc, szDir);
		app.PrintMessage(idMsgError, szMsg);
		cmdx.LogMessage(idMsgError, szMsg);
		goto lErrorExit;
	}
	SzCopy(szSrc, szPathSrc);
	SzAppendPathSeparator(SzEnd(szSrc));
	SzAppend(szSrc, szDir);

	/* Set up the extraction tree path string.
	*/
	if ((CchOfSz(szPathXtr)+CchOfSz(szDir)+1) > MAX_PATH) {
		apst.SetErrLast(errPathLength);
		sprintf_s(szMsg, cchLogMsgMax, "Path name too long: %s\\%s\n", szPathXtr, szDir);
		app.PrintMessage(idMsgError, szMsg);
		cmdx.LogMessage(idMsgError, szMsg);
		goto lErrorExit;
	}
	SzCopy(szXtr, szPathXtr);
	SzAppendPathSeparator(SzEnd(szXtr));	
	SzAppend(szXtr, szDir);

	/* Create the new folder object.
	*/
	pfldx = new FLDX;
	if (pfldx == NULL) {
		apst.SetErrLast(errOutOfMemory);
		goto lErrorExit;
	}

	/* Set the path names into the FLDR object and add it to the list
	** of directories to be processed.
	*/
	pfldx->SetPathSrc(szSrc);
	pfldx->SetPathDst(szDst);
	pfldx->SetPathXtr(szXtr);

	fldrmg.AddPfldr(pfldx);
	fStat = fTrue;

lErrorExit:
	return fStat;
}

/* ------------------------------------------------------------ */
/***	FLDX::FProcessFile
**
**	Parameters:
**		szFile		- file name of the file to process
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not.
**
**	Description:
**		Process the file.
*/

bool
FLDX::FProcessFile(char * szFile)
	{
	bool		fStat;
	char		szSrc[MAX_PATH];
	char		szDst[MAX_PATH];
	char		szXtr[MAX_PATH];
	char		szMsg[cchLogMsgMax];
	HANDLE		hfindSrc;
	HANDLE		hfindDst;
	WIN32_FIND_DATA	ffdtSrc;
	WIN32_FIND_DATA	ffdtDst;
	FSIG		fsig;
	bool		fSameSize;
	bool		fSameTime;
	int			dftTime;
	DWORD		sigSrc;
	DWORD		sigDst;
	DWORD		err;

	fStat = fFalse;
	fldrmg.FileProcessed();

	/* Set up the path and file name for the source file.
	*/
	if ((CchOfSz(szPathSrc)+CchOfSz(szFile)+1) >= MAX_PATH) {
		apst.SetErrLast(errPathLength);
		sprintf_s(szMsg, cchLogMsgMax, "Path name too long: %s\\%s\n", szPathSrc, szFile);
		app.PrintMessage(idMsgError, szMsg);
		cmdx.LogMessage(idMsgError, szMsg);
		goto lErrorExit;
	}
	SzCopy(szSrc, szPathSrc);
	SzAppendPathSeparator(SzEnd(szSrc));
	SzAppend(szSrc, szFile);
	
	/* Set up the path and file name for the destination file.
	*/
	if ((CchOfSz(szPathDst)+CchOfSz(szFile)+1) >= MAX_PATH) {
		apst.SetErrLast(errPathLength);
		sprintf_s(szMsg, cchLogMsgMax, "Path name too long: %s\\%s\n", szPathDst, szFile);
		app.PrintMessage(idMsgError, szMsg);
		cmdx.LogMessage(idMsgError, szMsg);
		goto lErrorExit;
	}
	SzCopy(szDst, szPathDst);
	SzAppendPathSeparator(SzEnd(szDst));
	SzAppend(szDst, szFile);
	
	/* Tell the FLDRMG about these path strings to it can keep
	** track of the longest.
	*/
	fldrmg.SetCchPath(CchOfSz(szSrc));
	fldrmg.SetCchPath(CchOfSz(szDst));

	/* Get information about the files to determine which is newer.
	*/
	hfindDst = FindFirstFile(szDst, &ffdtDst);
	FindClose(hfindDst);
	
	hfindSrc = FindFirstFile(szSrc, &ffdtSrc);
	if (hfindSrc == INVALID_HANDLE_VALUE) {
		err = GetLastError();
		if ((err == ERROR_FILE_NOT_FOUND) || (err = ERROR_PATH_NOT_FOUND)) {
			if (apst.FExtractUnique()) {
				/* The file doesn't exist in the source tree. If we are
				** extracting unique files, we need to copy this one.
				*/ 
				goto lCopy;
			}
			else {
				/* If we're not extracting unique files, there's nothing
				** to do.
				*/
				goto lNoCopy;
			}
		}
		else {
			/* Some other error is a real error.
			*/
			goto lErrorExit;
		}
	}
	FindClose(hfindDst);
	
	/* We get here if both files exist. If we're not doing extract
	** files that differ, then get we shouldn't be copying the file.
	*/
	if (!apst.FExtractDiff()) {
		goto lNoCopy;
	}

	/* Compare the file sizes.
	*/
	if ((ffdtSrc.nFileSizeHigh == ffdtDst.nFileSizeHigh) &&
		(ffdtSrc.nFileSizeLow  == ffdtDst.nFileSizeLow)) {
		fSameSize = fTrue;
	}
	else {
		fSameSize = fFalse;
	}

	/* Check if there is a FAT file system involved.
	*/
	if (apst.FSrcFsFat() || apst.FDstFsFat()) {
		/* Either the source or destination are on a FAT file system.
		** modify the times so that they will compare. the NTFS keeps
		** last write time at 10ms resolution, the FAT file system keeps
		** last write time as 2s resolution.
		*/
		ffdtSrc.ftLastWriteTime.dwLowDateTime &= 0xFE000000;
		ffdtDst.ftLastWriteTime.dwLowDateTime &= 0xFE000000;
	}
		
	/* Compare the file times
	** dftTime has the following meaning:
	**		-1	source file older than destination
	**		 0	source file and destination file have same times
	**		 1  source file newer than destination
	*/
	dftTime = CompareFileTime(&(ffdtSrc.ftLastWriteTime), &(ffdtDst.ftLastWriteTime));
	if (dftTime == 0) {
		fSameTime = fTrue;
	}
	else {
		fSameTime = fFalse;
	}

	/* Determine if we need to copy the source file over the destination file.
	*/
	if (apst.FUseSignatures()) {
		if (fSameSize) {
			/* When using file signatures:
			** If they are the same size and  have the same last write time,
			** they are identical, so don't extract this file.
			*/
			if (fSameTime) {
				goto lNoCopy;
			}

			/* When the files are the same size, but have different times,
			** check the signatures to see if they are identical.
			*/
			fsig.FInit(szSrc, fldrmg.CbSigBuf(), fldrmg.PbSigBuf());
			fsig.FGenerate(&sigSrc);
			fsig.Terminate();

			fsig.FInit(szDst, fldrmg.CbSigBuf(), fldrmg.PbSigBuf());
			fsig.FGenerate(&sigDst);
			fsig.Terminate();

			if (sigSrc == sigDst) {
				/* They have the same content, so don't extract the file.
				*/
				goto lNoCopy;
			}

		}
	}
	else {
		/* When not using file signatures:
		** Files that are the same size and have the same date and time
		** don't get copied.
		** If they are different size or have a different date and time,
		** copy the destination file to the extraction tree.
		*/
		if (fSameTime & fSameSize) {
			goto lNoCopy;
		}
	}

	/* We haven't come up with a reason not to copy this file, so
	** the destination file needs to be copied to the extraction tree.
	*/
lCopy:

	/* Make sure that this directory path exists.
	*/
	if (!apst.FNoCopyDir()) {
		fldrmg.DirCreated();
		if (!FCreatePath(szPathXtr)) {
			apst.SetErrLast(errFileSystem);
			sprintf_s(szMsg, cchLogMsgMax, "Error creating folder: %s\n", szPathXtr);
			app.PrintMessage(idMsgError, szMsg);
			cmdx.LogMessage(idMsgError, szMsg);
			goto lErrorExit;
		}
	}
	
	/* Set up the path and file name for the extracted file.
	*/
	if ((CchOfSz(szPathXtr)+CchOfSz(szFile)+1) >= MAX_PATH) {
		apst.SetErrLast(errPathLength);
		sprintf_s(szMsg, cchLogMsgMax, "Path name too long: %s%s\n", szPathXtr, szFile);
		app.PrintMessage(idMsgError, szMsg);
		cmdx.LogMessage(idMsgError, szMsg);
		goto lErrorExit;
	}
	SzCopy(szXtr, szPathXtr);	
	SzAppendPathSeparator(SzEnd(szXtr));
	SzAppend(szXtr, szFile);

	fldrmg.SetCchPath(CchOfSz(szXtr));
	
	if (apst.FPrintFile() || apst.FLogFile()) {
		sprintf_s(szMsg, cchLogMsgMax, "Extracting:  %s\n", szDst);
		app.PrintMessage(idMsgExtrFile, szMsg);
		cmdx.LogMessage(idMsgExtrFile, szMsg);
	}

	if (!apst.FNoCopyFile()) {
		fldrmg.FileCopied();
		if (CopyFile(szDst, szXtr, fFalse) == 0) {
			apst.SetErrLast(errFileSystem);
			sprintf_s(szMsg, cchLogMsgMax, "Error copying file: %s\n", szSrc);
			app.PrintMessage(idMsgError, szMsg);
			cmdx.LogMessage(idMsgError, szMsg);
			goto lErrorExit;
		}
	}

lNoCopy:
	fStat = fTrue;

lErrorExit:
	apst.FSetSzFileCur(szFile);
	return fStat;
}

/* ------------------------------------------------------------ */
/*				FLDT Object Class Implementation				*/
/* ------------------------------------------------------------ */
/***	FLDT::FLDT
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

FLDT::FLDT()
	{

}

/* ------------------------------------------------------------ */
/***	FLDT::~FLDT
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

FLDT::~FLDT()
	{
	AssertMsg(szMsg, "FLDR object not terminated before destruction");

}

/* ------------------------------------------------------------ */
/***	FLDT::FInit
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
**		Initialize object for user after construction
*/

bool
FLDT::FInit()
	{

	/* Call the base class FInit method to inherit the basic
	** behavior.
	*/
	if (!FLDR::FInit()) {
		return fFalse;
	}

	/* Perform the init for this class.
	*/

	return fTrue;

}

/* ------------------------------------------------------------ */
/***	FLDT::Terminate
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
**		Prepare object for destruction
*/

void
FLDT::Terminate()
	{

	/* Clean up for this class.
	*/

	/* Call the base class Terminate to clean it up.
	*/
	FLDR::Terminate();

}

/* ------------------------------------------------------------ */
/***	FLDT::FProcessDirectory
**
**	Parameters:
**		szDir		- directory name
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Process the given directory name.
*/

bool
FLDT::FProcessDirectory(char * szDir)
	{
	bool		fStat;
	FLDT *		pfldt;
	char 		szSrc[MAX_PATH];
	char 		szDst[MAX_PATH];
	char		szMsg[cchLogMsgMax];

	fStat = fFalse;

	/* Don't add the '.' and '..' directories to the list.
	*/
	if ((CompareSz(szDir, ".") == 0) || (CompareSz(szDir, "..") == 0)) {
		return fTrue;
	}

	/* Set up the destination path string.
	*/
	if ((CchOfSz(szPathDst)+CchOfSz(szDir)+1) >= MAX_PATH) {
		apst.SetErrLast(errPathLength);
		sprintf_s(szMsg, cchLogMsgMax, "Path name too long: %s%s\n", szPathDst, szDir);
		app.PrintMessage(idMsgError, szMsg);
		cmdx.LogMessage(idMsgError, szMsg);
		goto lErrorExit;
	}
	SzCopy(szDst, szPathDst);
	SzAppendPathSeparator(SzEnd(szDst));
	SzAppend(szDst, szDir);
	
	/* Check if this directory is one that is being excluded.
	*/	
	if (fldrmg.FMatchXclPath(szDst)) {
		if (apst.FPrintXclDir()) {
			sprintf_s(szMsg, cchLogMsgMax, "Skipping: %s\n", szDst);
			app.PrintMessage(idMsgExclFile, szDst);
			cmdx.LogMessage(idMsgExclFile, szDst);
		}
		fldrmg.DirExcluded();
		return fTrue;
	}

	/* Set the source path string
	*/
	if ((CchOfSz(szPathSrc)+CchOfSz(szDir)+1) >= MAX_PATH) {
		apst.SetErrLast(errPathLength);
		sprintf_s(szMsg, cchLogMsgMax, "Path name too long: %s%s\n", szPathSrc, szDir);
		app.PrintMessage(idMsgError, szMsg);
		cmdx.LogMessage(idMsgError, szMsg);
		goto lErrorExit;
	}
	SzCopy(szSrc, szPathSrc);
	SzAppendPathSeparator(SzEnd(szSrc));
	SzAppend(szSrc, szDir);

	/* Create the new folder object.
	*/
	pfldt = new FLDT;
	if (pfldt == NULL) {
		apst.SetErrLast(errOutOfMemory);
		goto lErrorExit;
	}

	/* Set the path names into the FLDR object and add it to the list
	** of directories to be processed.
	*/
	pfldt->SetPathSrc(szSrc);
	pfldt->SetPathDst(szDst);

	fldrmg.AddPfldr(pfldt);
	fStat = fTrue;

lErrorExit:
	return fStat;
	
}

/* ------------------------------------------------------------ */
/***	FLDT::FProcessFile
**
**	Parameters:
**		szFile		- file name of the file to process
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not.
**
**	Description:
**		Process the file.
*/

bool
FLDT::FProcessFile(char * szFile)
	{
	bool		fStat;
	char		szSrc[MAX_PATH];
	char		szDst[MAX_PATH];
	char		szMsg[cchLogMsgMax];
	HANDLE		hfindSrc;
	HANDLE		hfindDst;
	WIN32_FIND_DATA	ffdtSrc;
	WIN32_FIND_DATA	ffdtDst;
	DWORD		err;

	fStat = fFalse;
	fldrmg.FileProcessed();

	/* Set up the path and file name for the source file.
	*/
	if ((CchOfSz(szPathSrc)+CchOfSz(szFile)+1) >= MAX_PATH) {
		apst.SetErrLast(errPathLength);
		sprintf_s(szMsg, cchLogMsgMax, "Path name too long: %s%s\n", szPathSrc, szFile);
		app.PrintMessage(idMsgError, szMsg);
		cmdx.LogMessage(idMsgError, szMsg);
		goto lErrorExit;
	}
	SzCopy(szSrc, szPathSrc);
	SzAppendPathSeparator(SzEnd(szSrc));
	SzAppend(szSrc, szFile);
	
	/* Set up the path and file name for the destination file.
	*/
	if ((CchOfSz(szPathDst)+CchOfSz(szFile)+1) >= MAX_PATH) {
		apst.SetStProg(stPathLength);
		sprintf_s(szMsg, cchLogMsgMax, "Path name too long: %s%s\n", szPathDst, szFile);
		app.PrintMessage(idMsgError, szMsg);
		cmdx.LogMessage(idMsgError, szMsg);
		goto lErrorExit;
	}
	SzCopy(szDst, szPathDst);
	SzAppendPathSeparator(SzEnd(szDst));
	SzAppend(szDst, szFile);
	
	/* Tell the FLDRMG about these path strings to it can keep
	** track of the longest.
	*/
	fldrmg.SetCchPath(CchOfSz(szSrc));
	fldrmg.SetCchPath(CchOfSz(szDst));

	/* Get information about the files to determine which is newer.
	*/
	hfindDst = FindFirstFile(szDst, &ffdtDst);
	FindClose(hfindDst);
	
	hfindSrc = FindFirstFile(szSrc, &ffdtSrc);
	if (hfindSrc == INVALID_HANDLE_VALUE) {
		err = GetLastError();
		if (err == ERROR_FILE_NOT_FOUND) {
			/* The file doesn't exist in the source tree, delete it.
			*/ 
			goto lDeleteFile;
		}
		else if (err = ERROR_PATH_NOT_FOUND) {
			/* The folder that this file is in doesn't exist in the
			** source tree, so we should delete it. Ideally, we would
			** also delete the folder.
			*/
			goto lDeleteFile;
		}
		else {
			/* Some other error is a real error.
			*/
			goto lErrorExit;
		}
	}
	FindClose(hfindDst);
	
	/* We get here if both files exist. Since we're only deleting files
	** in the destination tree that don't exist in the source tree, we
	** don't need to do anything with this file.
	*/
	goto lNoDelete;


	/* We haven't come up with a reason not to copy this file, so
	** the destination file needs to be copied to the extraction tree.
	*/
lDeleteFile:
	if (apst.FPrintFile() || apst.FLogFile()) {
		sprintf_s(szMsg, cchLogMsgMax, "Deleting:  %s\n", szDst);
		app.PrintMessage(idMsgDelFile, szMsg);
		cmdx.LogMessage(idMsgDelFile, szMsg);
	}

	if (!apst.FNoCopyFile()) {
		fldrmg.FileDeleted();
		if (DeleteFile(szDst) == 0) {
			if (GetLastError() == ERROR_ACCESS_DENIED) {
				if (apst.FPrintFile() || apst.FLogFile()) {
					sprintf_s(szMsg, cchLogMsgMax, "Access Denied!\n");
					app.PrintMessage(idMsgError, szMsg);
					cmdx.LogMessage(idMsgError, szMsg);
				}
			}
			else {
				apst.SetErrLast(errFileSystem);
				sprintf_s(szMsg, cchLogMsgMax, "Error copying file: %s\n", szDst);
				app.PrintMessage(idMsgError, szMsg);
				cmdx.LogMessage(idMsgError, szMsg);
				goto lErrorExit;
			}
		}
	}

lNoDelete:
	fStat = fTrue;

lErrorExit:
	apst.FSetSzFileCur(szFile);
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

