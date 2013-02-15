/************************************************************************/
/*																		*/
/*	apst.h	--	Interface Declarations for APST Object Class			*/
/*																		*/
/************************************************************************/
/*	Author:		Gene Apperson											*/
/*	Copyright 2010, Gene Apperson										*/
/************************************************************************/
/*  File Description:													*/
/*																		*/
/*	This module contains the declarations for the APST object class		*/
/*	This object class is used to maintain the global application state.	*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	12/28/2010(GeneA): created											*/
/*																		*/
/************************************************************************/

#if !defined(APST_INC)
#define APST_INC

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */

const int	stSuccess		= 0;	//program succeeded
const int	stInternalError	= 1;	//internal program error
const int	stBuildInconsistency = 2;	//the build version contains inconsistent code
const int	stSyntaxError	= 3;	//command line syntax error
const int	stParamError	= 4;	//error in command line parameter
const int	stPathLength	= 5;	//path length exceeds MAX_PATH
const int	stFileSystemError = 6;
const int	stOutOfMemory	= 7;
const int	stFileSyntax	= 8;	//syntax error in command file

/* ------------------------------------------------------------ */
/*					General Type Declarations					*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Object Class Declarations					*/
/* ------------------------------------------------------------ */

class APST {
private:
	char *		szRootSrc;			//source root path string
	char *		szRootDst;			//destination root path string
	char *		szRootXtr;			//extraction tree root path string
	char *		szFileCur;			//current file being processed
	char *		szCmdPath;			//path to default command files
	char *		szCommandFile;		//file name for command file
	char *		szLogPath;			//path to log file folder
	char *		szLogFile;			//log file name
	int			stProg;				//program execution status
	int			errLast;			//error code for last error encountered
	FILE *		fhLogFile;			//file handle for log file
	__time64_t	tmmStart;			//date and time program started

	bool	fSrcFsFat;			//source file system is FAT
	bool	fDstFsFat;			//destination file system is FAT
	bool	fXtrFsFat;			//extraction file system is FAT	
	bool	fShowTime;			//print elapsed time
	bool	fShowVer;			//print the software version number
	bool	fShowUseage;		//print use help
	bool	fDoSystem;			//process system files
	bool	fDoHidden;			//process hidden files
	bool	fPrintFile;			//print file names of files being copied
	bool	fPrintDir;			//print directory names being created
	bool	fPrintXclFile;		//print names of excluded files
	bool	fPrintXclDir;		//print names of excluded directories
	bool	fLogEnable;			//enable printing to log file
	bool	fLogFile;			//log names of files being copied
	bool	fLogDir;			//log names of directories being created
	bool	fLogXclFile;		//log names of files being excluded
	bool	fLogXclDir;			//log names of directories being skipped
	bool	fLogAction;			//log actions being performed
	bool	fNoCopyFile;		//don't copy files, just print names
	bool	fNoCopyDir;			//don't create directories
	bool	fUseSignatures;		//use file signatures to compare files
	bool	fCommandFile;		//use instructions from command file
	bool	fPrintStat;			//print statistics
	bool	fUpdateDst;			//update destination with newer files in source
	bool	fExtractDst;		//extract files from dst that differ from src
	bool	fTrimDst;			//delete destination files  not in source
	bool	fExtractUnique;		//copy unique files from destination to specified path
	bool	fExtractDiff;		//copy changed files from destination to specified path
	bool	fLogFileCreated;	//a log file has been created in this execution
	DWORD	ticStart;			//time tick for beginning of current timing interval

public:
			APST();
		   ~APST();
		   
	bool	FInit();
	void	Terminate();
	void	SetStProg(int st) { stProg = st; };	
	int		StProg() { return stProg; };
	void	SetErrLast(int err) { errLast = err; };
	int		ErrLast() { return errLast; };

	bool	FSetSzRootSrc(char * sz);
	bool	FSetSzRootDst(char * sz);
	bool	FSetSzRootXtr(char * sz);
	bool	FSetSzFileCur(char * sz);
	bool	FSetSzCommandFile(char * sz);
	bool	FSetSzCmdPath(char * sz);
	bool	FSetSzLogPath(char * sz);
	bool	FSetSzLogFile(char * sz);
	void	SetFhLogFile(FILE * fh) { fhLogFile = fh; };
	void	SetFLogFileCreated(bool f) { fLogFileCreated = f; };
	void	SetTmmStart(__time64_t tmm) { tmmStart = tmm; };
	char *	SzRootSrc() { return szRootSrc; };
	char *	SzRootDst() { return szRootDst; };
	char *	SzRootXtr() { return szRootXtr; };
	char *	SzFileCur() { return szFileCur; };
	char *	SzCommandFile() { return szCommandFile; };
	char *	SzCmdPath() { return szCmdPath; };
	char *	SzLogPath() { return szLogPath; };
	char *	SzLogFile() { return szLogFile; };
	FILE *	FhLogFile() { return fhLogFile; };
	bool	FLogFileCreated() { return fLogFileCreated; };
	__time64_t TmmStart() { return tmmStart; };

	void	SetFShowUseage(bool f) { fShowUseage = f; };
	void	SetFSrcFsFat(bool f) { fSrcFsFat = f; };
	void	SetFDstFsFat(bool f) { fDstFsFat = f; };
	void	SetFXtrFsFat(bool f) { fXtrFsFat = f; };
	void	SetFShowTime(bool f) { fShowTime = f; };
	void	SetFShowVer(bool f) { fShowVer = f; };
	void	SetFDoSystem(bool f) { fDoSystem = f; };
	void	SetFDoHidden(bool f) { fDoHidden = f; };
	void	SetFPrintFile(bool f) { fPrintFile = f; };
	void	SetFPrintDir(bool f) { fPrintDir = f; };
	void	SetFPrintXclFile(bool f) { fPrintXclFile = f; };
	void	SetFPrintXclDir(bool f) { fPrintXclDir = f; };
	void	SetFLogEnable(bool f) { fLogEnable = f; };
	void	SetFLogFile(bool f) { fLogFile = f; };
	void	SetFLogDir(bool f) { fLogDir = f; };
	void	SetFLogXclFile(bool f) { fLogXclFile = f; };
	void	SetFLogXclDir(bool f) { fLogXclDir = f; };
	void	SetFLogAction(bool f) { fLogAction = f; };
	void	SetFNoCopyFile(bool f) { fNoCopyFile = f; };
	void	SetFNoCopyDir(bool f) { fNoCopyDir = f; };
	void	SetFUseSignatures(bool f) { fUseSignatures = f; };
	void	SetFCommandFile(bool f) { fCommandFile = f; };
	void	SetFPrintStat(bool f) { fPrintStat = f; };
	void	SetFUpdateDst(bool f) { fUpdateDst = f; };
	void	SetFExtractDst(bool f) { fExtractDst = f; };
	void	SetFTrimDst(bool f) { fTrimDst = f; };
	void	SetFExtractUnique(bool f) { fExtractUnique = f; };
	void	SetFExtractDiff(bool f) { fExtractDiff = f; };
	void	SetTicStart(DWORD tic) { ticStart = tic; };

	bool	FShowTime() { return fShowTime; };
	bool	FShowVer() { return fShowVer; };
	bool	FShowUseage() { return fShowUseage; };
	bool	FSrcFsFat() { return fSrcFsFat; };
	bool	FDstFsFat() { return fDstFsFat; };
	bool	FXtrFsFat() { return fXtrFsFat; };
	bool	FDoSystem() { return fDoSystem; };
	bool	FDoHidden() { return fDoHidden; };
	bool	FPrintFile() { return fPrintFile; };
	bool	FPrintDir() { return fPrintDir; };
	bool	FPrintXclFile() { return fPrintXclFile; };
	bool	FPrintXclDir() { return fPrintXclDir; };
	bool	FLogEnable() { return fLogEnable; };
	bool	FLogFile() { return fLogFile; };
	bool	FLogDir() { return fLogDir; };
	bool	FLogXclFile() { return fLogXclFile; };
	bool	FLogXclDir() { return fLogXclDir; };
	bool	FLogAction() { return fLogAction; };
	bool	FNoCopyFile() { return fNoCopyFile; };
	bool	FNoCopyDir() { return fNoCopyDir; };
	bool	FUseSignatures() { return fUseSignatures; };
	bool	FCommandFile() { return fCommandFile; };
	bool	FPrintStat() { return fPrintStat; };
	bool	FUpdateDst() { return fUpdateDst; };
	bool	FExtractDst() { return fExtractDst; };
	bool	FTrimDst() { return fTrimDst; };
	bool	FExtractUnique() { return fExtractUnique; };
	bool	FExtractDiff() { return fExtractDiff; };
	DWORD	TicStart() { return ticStart; };
};

/* ------------------------------------------------------------ */
/*					Variable Declarations						*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Procedure Declarations						*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */

#endif

/************************************************************************/
