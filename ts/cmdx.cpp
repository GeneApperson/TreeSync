/************************************************************************/
/*																		*/
/*	cmdx.cpp	--	CMDX Object Class Implementation					*/
/*																		*/
/************************************************************************/
/*	Author: 	Gene Apperson											*/
/*	Copyright 2011, Gene Apperson										*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*	This module contains the implemendation for the CMDX object class.	*/
/*	This object is used to interpret and exctute command files.			*/
/*																		*/
/* Command language syntax												*/
/*																		*/
/* Statements:															*/
/* update <src>,<dst>	update <dst> with files from <src>				*/
/* extract <src>,<dst>,<xtr>											*/
/*						extract files from <dst> that differ from <src>	*/
/*						to <xtr>										*/
/* trim <src>,<dst>		delete files from <dst> that aren't in <src>	*/
/* exclude <xcl> = <val>  add <val> to exclusion list <xcl>				*/
/* clear <clr>			clear item <clr>								*/
/* include <file>		include <file> in command execution stream		*/
/* run <file>			synonym for include								*/
/* set <sym> = <val>	set symbol <sym> to value <val>					*/
/* log [on|off]			turn logging on or off							*/
/* print pcmd,[pcmd...] print to log file								*/
/* opt = opt1[,otp2]	set options										*/
/*																		*/
/* Print Commands:														*/
/* <expr>		print the value of <expr>								*/
/* stat			print statistics										*/
/* elapsed		print elapsted time value								*/
/* date			print date program started running						*/
/* time			print time program started running						*/
/*																		*/
/* Options:																*/
/* pact			print actions											*/
/* npact		don't print actions										*/
/* cd			create directories										*/
/* ncd			don't create directories								*/
/* cf			copy files												*/
/* ncf			don't copy files										*/
/* cs			copy system files										*/
/* ncs			don't copy system files									*/
/* ch			copy hidden files										*/
/* nch			don't copy hidden files									*/
/* pf			print names of files being copied						*/
/* npf			don't print file names									*/
/* pd			print names of directories being created				*/
/* npd			don't print directory names								*/
/* pxf			log names of excluded files								*/
/* npxf			don't log names of excluded files						*/
/* pxd			log names of excluded directories						*/
/* npxd			don't log names of excluded directories					*/
/* xu			extract unique files									*/
/* nxu			don't extract unique files								*/
/* xc			extract changed files									*/
/* nxc			don't extract changed files								*/
/* sig			use file signatures										*/
/* nsig			don't use file signatures								*/
/*																		*/
/* Exclude options:														*/
/* file			file exclude list										*/
/* ext			file extension exclude list								*/
/* dir			directory exclude list									*/
/* path			path exclude list										*/
/*																		*/
/* Clear options:														*/
/* file			file exclude list										*/
/* ext			file extension exclude list								*/
/* dir			directory exclude list									*/
/* path			path exclude list										*/
/* stat			statistics registers									*/
/* elapsed		elapsed time counter									*/
/*																		*/
/* Initial values for the options will come from either the program		*/
/* default settings or the options parameters entered on the command	*/
/* line that launched the program.										*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	03/05/2011(GeneA): created											*/
/*																		*/
/************************************************************************/


/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include	<windows.h>
#include	<stdio.h>
#include	<time.h>

#include	"../inc/dtypes.h"
#include	"../inc/dutil.h"
#include	"../inc/dassert.h"

#include	"config.h"
#include	"appdefs.h"
#include	"apst.h"
#include	"fldrmg.h"
#include	"app.h"
#include	"cmdx.h"

/* ------------------------------------------------------------ */
/*				Local Type and Constant Definitions				*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

extern APST		apst;
extern APP		app;
extern FLDRMG	fldrmg;

/* ------------------------------------------------------------ */
/*				Local Variables									*/
/* ------------------------------------------------------------ */

__SetCurFile;

/* Language syntax elements for the various save files and other
** parse related commands.
*/
char	szKwdUpdate[]		= "update";
char	szKwdExtract[]		= "extract";
char	szKwdTrim[]			= "trim";
char	szKwdInclude[]		= "include";
char	szKwdRun[]			= "run";
char	szKwdPrint[]		= "print";
char	szKwdSet[]			= "set";
char	szKwdExclude[]		= "exclude";
char	szKwdClear[]		= "clear";
char	szKwdLog[]			= "log";
char	szKwdOpt[]			= "opt";
char	szKwdOn[]			= "on";
char	szKwdOff[]			= "off";
char	szKwdCd[]			= "cd";
char	szKwdNcd[]			= "ncd";
char	szKwdCf[]			= "cf";
char	szKwdNcf[]			= "ncf";
char	szKwdCs[]			= "cs";
char	szKwdNcs[]			= "ncs";
char	szKwdCh[]			= "ch";
char	szKwdNch[]			= "nch";
char	szKwdPf[]			= "pf";
char	szKwdNpf[]			= "npf";
char	szKwdPd[]			= "pd";
char	szKwdNpd[]			= "npd";
char	szKwdPxf[]			= "pxf";
char	szKwdNpxf[]			= "npxf";
char	szKwdPxd[]			= "pxd";
char	szKwdNpxd[]			= "npxd";
char	szKwdXu[]			= "xu";
char	szKwdNxu[]			= "nxu";
char	szKwdXc[]			= "xc";
char	szKwdNxc[]			= "nxc";
char	szKwdExt[]			= "ext";
char	szKwdFile[]			= "file";
char	szKwdDir[]			= "dir";
char	szKwdPath[]			= "path";
char	szKwdDate[]			= "date";
char	szKwdTime[]			= "time";
char	szKwdStat[]			= "stat";
char	szKwdElapsed[]		= "elapsed";
char	szKwdPact[]			= "pact";
char	szKwdNpact[]		= "npact";
char	szKwdSig[]			= "sig";
char	szKwdNsig[]			= "nsig";

const int	idKwdUpdate		= 1;
const int	idKwdExtract	= 2;
const int	idKwdTrim		= 3;
const int	idKwdInclude	= 4;
const int	idKwdRun		= 5;
const int	idKwdPrint		= 6;
const int	idKwdSet		= 7;
const int	idKwdExclude	= 8;
const int	idKwdClear		= 9;
const int	idKwdLog		= 10;
const int	idKwdOpt		= 11;
const int	idKwdOn			= 12;
const int	idKwdOff		= 13;
const int	idKwdCd			= 14;
const int	idKwdNcd		= 15;
const int	idKwdCf			= 16;
const int	idKwdNcf		= 17;
const int	idKwdCs			= 18;
const int	idKwdNcs		= 19;
const int	idKwdCh			= 20;
const int	idKwdNch		= 21;
const int	idKwdPf			= 22;
const int	idKwdNpf		= 23;
const int	idKwdPd			= 24;
const int	idKwdNpd		= 25;
const int	idKwdPxf		= 26;
const int	idKwdNpxf		= 27;
const int	idKwdPxd		= 28;
const int	idKwdNpxd		= 29;
const int	idKwdXu			= 30;
const int	idKwdNxu		= 31;
const int	idKwdXc			= 32;
const int	idKwdNxc		= 33;
const int	idKwdExt		= 34;
const int	idKwdFile		= 35;
const int	idKwdDir		= 36;
const int	idKwdPath		= 37;
const int	idKwdDate		= 38;
const int	idKwdTime		= 39;
const int	idKwdStat		= 40;
const int	idKwdElapsed	= 41;
const int	idKwdPact		= 42;
const int	idKwdNpact		= 43;
const int	idKwdSig		= 44;
const int	idKwdNsig		= 45;

/* Default log file name when one hasn't been specified in some
** way by the user.
*/
char	szLogFileDefault[]	= "startup.log";

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				CMDX Object Class Implementation				*/
/* ------------------------------------------------------------ */
/***	CMDX::CMDX
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
**		Default constructor. Set object to default initial state.
*/

CMDX::CMDX()
	{
	
}

/* ------------------------------------------------------------ */
/***	CMDX::~CMDX
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
**		Default destruction.
*/

CMDX::~CMDX()
	{
	
}

/* ------------------------------------------------------------ */
/***	CMDX::FInit()
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Initialize object for use after construction
*/

bool
CMDX::FInit()
	{

	if (!prsr.FInit()) {
		return fFalse;
	}
	InitKeywords();	
	return fTrue;
	
}

/* ------------------------------------------------------------ */
/***	CMDX::Terminate
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
**		Prepare object for destruction. Return to the default state.
*/

void
CMDX::Terminate()
	{

	prsr.Terminate();
	
}

/* ------------------------------------------------------------ */
/***	CMDX::InitKeywords
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
**		Initialise the parser with the keywords for the command
**		language.
*/

void
CMDX::InitKeywords()
	{
	
	prsr.AddKeyword(szKwdUpdate, idKwdUpdate);
	prsr.AddKeyword(szKwdExtract, idKwdExtract);
	prsr.AddKeyword(szKwdTrim, idKwdTrim);
	prsr.AddKeyword(szKwdInclude, idKwdInclude);
	prsr.AddKeyword(szKwdRun, idKwdRun);
	prsr.AddKeyword(szKwdPrint, idKwdPrint);
	prsr.AddKeyword(szKwdSet, idKwdSet);
	prsr.AddKeyword(szKwdExclude, idKwdExclude);
	prsr.AddKeyword(szKwdClear, idKwdClear);
	prsr.AddKeyword(szKwdLog, idKwdLog);
	prsr.AddKeyword(szKwdOpt, idKwdOpt);
	prsr.AddKeyword(szKwdOn, idKwdOn);
	prsr.AddKeyword(szKwdOff, idKwdOff);
	prsr.AddKeyword(szKwdCd, idKwdCd);
	prsr.AddKeyword(szKwdNcd, idKwdNcd);
	prsr.AddKeyword(szKwdCf, idKwdCf);
	prsr.AddKeyword(szKwdNcf, idKwdNcf);
	prsr.AddKeyword(szKwdCs, idKwdCs);
	prsr.AddKeyword(szKwdNcs, idKwdNcs);
	prsr.AddKeyword(szKwdCh, idKwdCh);
	prsr.AddKeyword(szKwdNch, idKwdNch);
	prsr.AddKeyword(szKwdPf, idKwdPf);
	prsr.AddKeyword(szKwdNpf, idKwdNpf);
	prsr.AddKeyword(szKwdPd, idKwdPd);
	prsr.AddKeyword(szKwdNpd, idKwdNpd);
	prsr.AddKeyword(szKwdPxf, idKwdPxf);
	prsr.AddKeyword(szKwdNpxf, idKwdNpxf);
	prsr.AddKeyword(szKwdPxd, idKwdPxd);
	prsr.AddKeyword(szKwdNpxd, idKwdNpxd);
	prsr.AddKeyword(szKwdXu, idKwdXu);
	prsr.AddKeyword(szKwdNxu, idKwdNxu);
	prsr.AddKeyword(szKwdXc, idKwdXc);
	prsr.AddKeyword(szKwdNxc, idKwdNxc);
	prsr.AddKeyword(szKwdExt, idKwdExt);
	prsr.AddKeyword(szKwdFile, idKwdFile);
	prsr.AddKeyword(szKwdDir, idKwdDir);
	prsr.AddKeyword(szKwdPath, idKwdPath);
	prsr.AddKeyword(szKwdDate, idKwdDate);
	prsr.AddKeyword(szKwdTime, idKwdTime);
	prsr.AddKeyword(szKwdStat, idKwdStat);
	prsr.AddKeyword(szKwdElapsed, idKwdElapsed);
	prsr.AddKeyword(szKwdPact, idKwdPact);
	prsr.AddKeyword(szKwdNpact, idKwdNpact);
	prsr.AddKeyword(szKwdSig, idKwdSig);
	prsr.AddKeyword(szKwdNsig, idKwdNsig);

}

/* ------------------------------------------------------------ */
/***	CMDX::FExecute
**
**	Parameters:
**		szFile		- file name of command file to execute
**
**	Return Value:
**		none
**
**	Errors:
**		Return fTrue if successful, fFalse if not
**
**	Description:
**		Execute the command file.
*/

bool
CMDX::FExecute(char * szFile)
	{
	bool	fStat;

	apst.SetTicStart(GetTickCount());
	
	/* Initialize the parser with the command file.
	*/
	if (!prsr.FBeginFile(szFile)) {
		goto lReportError;
	}

	/* Process each line in the command file.
	*/
	fStat = fTrue;
	while (fStat) {
		/* Get the token at the beginning of the line.
		*/
		if (prsr.FIsEof()) {
			break;
		}
		
		prsr.FParseAny();
		
		if (prsr.TktToken() == tktEoln) {
			/* Blank lines are OK.
			*/
			prsr.FNextLine();
		}
		else if (prsr.TktToken() == tktKeyword) {
			/* Determine the statement type.
			** Check for actions first
			*/
			if (prsr.IdToken() == idKwdUpdate) {
				if (fStat) {
					apst.SetFUpdateDst(fTrue);
					fStat &= FParseUpdate();
					if (fStat) {
						fStat &= FDoUpdate();
					}
					apst.SetFUpdateDst(fFalse);
				}
			}
			
			else if (prsr.IdToken() == idKwdExtract) {
				if (fStat) {
					/* The appropriate extract options need to be
					** set in the command file before the extract
					** statement.
					*/
					apst.SetFExtractDst(fTrue);
					fStat &= FParseExtract();
					if (fStat) {
						fStat &= FDoExtract();
					}
					apst.SetFExtractDst(fFalse);
				}
			}
			
			else if (prsr.IdToken() == idKwdTrim) {
				if (fStat) {
					apst.SetFTrimDst(fTrue);
					fStat &= FParseTrim();
					if (fStat) {
						fStat &= FDoTrim();
					}
					apst.SetFTrimDst(fFalse);
				}
			}
			
			/* Check for the include directive.
			*/
			else if ((prsr.IdToken() == idKwdInclude) || (prsr.IdToken() == idKwdRun)) {
				if (fStat) {
					fStat &= FParseInclude();
				}
			}
			
			/* Check for the exclude statement.
			*/
			else if (prsr.IdToken() == idKwdExclude) {
				if (fStat) {
					prsr.UngetToken();
					fStat &= FParseExclude();
				}
			}
			
			/* Check for the clear statement.
			*/
			else if (prsr.IdToken() == idKwdClear) {
				if (fStat) {
					prsr.UngetToken();
					fStat &= FParseClear();
				}
			}
			
			/* Check for the log statement
			*/
			else if (prsr.IdToken() == idKwdLog) {
				if (fStat) {
					fStat &= FParseLog();
				}
			}
			
			/* Check for print statements
			*/
			else if (prsr.IdToken() == idKwdPrint) {
				if (fStat) {
					prsr.UngetToken();
					fStat &= FParsePrint();
				}
			}
			
			/* Check for the option statement.
			*/
			else if (prsr.IdToken() == idKwdOpt) {
				if (fStat) {
					prsr.UngetToken();
					fStat &= FParseOpt();
				}
			}
			
			/* Check for the set statement.
			*/
			else if (prsr.IdToken() == idKwdSet) {
				if (fStat) {
					prsr.UngetToken();
					fStat &= FParseSet();
				}
			}
			
			/* Unknown keyword on the line.
			*/
			else {
				apst.SetErrLast(errInternalError);
				goto lReportError;
			}
		}
			
		/* Unrecognized statement in the command file.
		*/
		else {
			apst.SetErrLast(errAppSyntax);
			goto lReportError;
		}
	}
	
	if (fStat) {
		goto lExit;
	}

lReportError:
	ReportError();
	fStat = fFalse;
	
lExit:
	/* All done executing the command file.
	*/
	if (apst.FhLogFile() != NULL) {
		/* A log file was being written to. Close it.
		*/
		CloseLogFile();
		apst.SetFLogEnable(fFalse);
	}

	return fStat;
		
}

/* ------------------------------------------------------------ */
/*			CMDX Private Helper Functions						*/
/* ------------------------------------------------------------ */
/***	CMDX::ReportError()
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
**		Print an error message to the user.
*/

void
CMDX::ReportError()
	{
	int		errPrsr;
	int		errApp;
	int		err;
	char *	sz;
	int		iln;
	int		istmg;
	
	errApp = apst.ErrLast();
	errPrsr = prsr.Err();
	err = errApp != errOK ? errApp : errPrsr;
	
	if (err <= errAppLast) {
		if (err == errPathLength) {
			printf("Error: Path length exceeds MAX_PATH\n");
		}
		else if (err == errOutOfMemory) {
			printf("Internal Error: Out of memory\n");
		}
		else if (err == errFileSystem) {
			printf("Error: File system error\n");
		}
		else if (err == errFileNotFound) {
			printf("Error: File not found\n");
		}
		else if (err == errInternalError) {
			printf("Error: Internal error\n");
		}
		else if (err == errAppSyntax) {
			printf("Error: Syntax Error\n");
		}
		else if (err == errAppNoSrcPath) {
			printf("Error: No source path specified\n");
		}
		else if (err == errAppNoDstPath) {
			printf("Error: No destination path specified\n");
		}
		else if (err == errAppNoXtrPath) {
			printf("Error: No file extraction path specified\n");
		}
		else if (err == errAppStringTooLong) {
			printf("Error: Syntax error, string too long\n");
		}
		else {
			printf("Errror: Unknown error\n");
		}	
	}
	else {
		if (err == errFileNotFound) {
			printf("Error: File not found\n");
		}
		else if (err == errPrsrUnexpectedToken) {
			printf("Error: Syntax error, unexpected token on line\n");
		}
		else if (err == errPrsrStringTooLong) {
			printf("Error: Syntax error, string too long\n");
		}
		else if (err == errPrsrIncorrectKeyword) {
			printf("Error: Syntax error, unexpected keyword on line\n");
		}
		else if (err == errPrsrSyntax) {
			printf("Error: Syntax error on line\n");
		}
		else if (err == errPrsrOutOfRange) {
			printf("Error: Syntax error, numeric constant out of range\n");
		}
		else if (err == errPrsrSymbolExpected) {
			printf("Error: Syntax error, symbol expected\n");
		}
		else if (err == errPrsrStringExpected) {
			printf("Error: Syntax error, string expected\n");
		}
		else if (err == errPrsrSymbolTooLong) {
			printf("Error: Syntax error, symbol name too long\n");
		}
		else if (err == errPrsrUndefinedSymbol) {
			printf("Error: Syntax error, undefined symbol\n");
		}
		else if (err == errLexrReadError) {
			printf("Error: Error reading from file\n");
		}
		else if (err == errLexrIncludeDepth) {
			printf("Error: Include file depth limit exceeded\n");
		}
		else if (err == errTknInvalidChar) {
			printf("Error: Syntax error, invalid character on line\n");
		}
		else if (err == errTknNumber) {
			printf("Error: Error parsing number\n");
		}
		else if (err == errTknStringTooLong) {
			printf("Error: Syntax error, string too long\n");
		}
		else if (err == errTknMissingEndQuote) {
			printf("Error: Syntax error, missing quote at end of string\n");
		}
		else if (err == errTknConstant) {
			printf("Error: Syntax error, invalid character in constant\n");
		}
		else if (err == errTknCharConstant) {
			printf("Error: Syntax error, invalid character constant\n");
		}
		else {
			printf("Error: Internal error\n");
		}	
	}

	sz = prsr.SzCurrentFile();
	if (!FIsNulSz(sz)) {
		printf("In file: %s\n", sz);
	}
	
	iln = prsr.IlnCur();
	if (iln > 0) {	
		printf("Line %d : %s\n", prsr.IlnCur(), prsr.SzCurrentLine());
	}

	if (prsr.IstmgCur() > 0) {
		istmg = prsr.IstmgCur();
		while (istmg > 1) {
			printf("  Included from line %d in file %s\n",
										prsr.IlnCur(istmg), prsr.SzFile(istmg));
			istmg -= 1;
		}
	}	
}

/* ------------------------------------------------------------ */
/***	CMDX::FDoUpdate()
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Perform the UPDATE action
*/

bool
CMDX::FDoUpdate()
	{
	FLDR *	pfldr;
	bool	fStat;
	
	/* Print to the log file if logging actions.
	*/
	LogMessage(idMsgAction, "Performing update\n\n");
	
	pfldr = new FLDS;
	if (pfldr == NULL) {
		apst.SetErrLast(errOutOfMemory);
		return fFalse;
	}
	
	/* Assign the root source and destination paths.
	*/
	pfldr->SetPathSrc(apst.SzRootSrc());
	pfldr->FindFsSrc(apst.SzRootSrc());
	pfldr->SetPathDst(apst.SzRootDst());
	pfldr->FindFsDst(apst.SzRootDst());

	/* Process this folder tree.
	*/
	fldrmg.BeginDirectory();
	fldrmg.AddPfldr(pfldr);
	fldrmg.EndDirectory();
	
	fStat = fldrmg.FProcess();
	
	return fStat;
	
}

/* ------------------------------------------------------------ */
/***	CMDX::FDoExtract()
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Perform the EXTRACT action
*/

bool
CMDX::FDoExtract()
	{
	FLDR *	pfldr;
	bool	fStat;
	
	/* Print to the log file if logging actions.
	*/
	LogMessage(idMsgAction, "Performing extract\n\n");
	
	pfldr = new FLDX;
	if (pfldr == NULL) {
		apst.SetErrLast(errOutOfMemory);
		return fFalse;
	}
	
	/* Assign the root source and destination paths.
	*/
	pfldr->SetPathSrc(apst.SzRootSrc());
	pfldr->FindFsSrc(apst.SzRootSrc());
	pfldr->SetPathDst(apst.SzRootDst());
	pfldr->FindFsDst(apst.SzRootDst());
	((FLDX *)pfldr)->SetPathXtr(apst.SzRootXtr());
	((FLDX *)pfldr)->FindFsXtr(apst.SzRootXtr());
	
	/* Process this folder tree.
	*/	
	fldrmg.BeginDirectory();
	fldrmg.AddPfldr(pfldr);
	fldrmg.EndDirectory();
	
	fStat = fldrmg.FProcess();
	
	return fStat;
	
}

/* ------------------------------------------------------------ */
/***	CMDX::FDoTrim()
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Perform the TRIM action
*/

bool
CMDX::FDoTrim()
	{
	FLDR *	pfldr;
	bool	fStat;
	
	/* Print to the log file if logging actions.
	*/
	LogMessage(idMsgAction, "Performing trim\n\n");
	
	pfldr = new FLDT;
	if (pfldr == NULL) {
		apst.SetErrLast(errOutOfMemory);
		return fFalse;
	}

	/* Assign the root source and destination paths.
	*/
	pfldr->SetPathSrc(apst.SzRootSrc());
	pfldr->FindFsSrc(apst.SzRootSrc());
	pfldr->SetPathDst(apst.SzRootDst());
	pfldr->FindFsDst(apst.SzRootDst());

	/* Process this folder tree.
	*/	
	fldrmg.BeginDirectory();
	fldrmg.AddPfldr(pfldr);
	fldrmg.EndDirectory();
	
	fStat = fldrmg.FProcess();
	
	return fStat;
	
}

/* ------------------------------------------------------------ */
/***	CMDX::FParseUpdate
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Parse the UPDATE statement and set the state to allow
**		execution.
*/

bool
CMDX::FParseUpdate()
	{
	char	szSrc[MAX_PATH];
	char	szDst[MAX_PATH];
	
	/* The first element is an expression for the src path.
	*/
	if (!FParseExpression(cchStringMax, szSrc)) {
		goto lSyntaxError;
	}
	
	/* Next is a comma
	*/
	if (!prsr.FParseTkt(tktComma)) {
		apst.SetErrLast(errPrsrCommaExpected);
		goto lSyntaxError;
	}
	
	/* The final element is an expression for the dst path.
	*/
	if (!FParseExpression(cchStringMax, szDst)) {
		goto lSyntaxError;
	}
	
	/* There shouldn't be anything else on the line.
	*/
	if (!prsr.FParseTkt(tktEof) && !prsr.FParseTkt(tktEoln)) {
		apst.SetErrLast(errPrsrUnexpectedToken);
		goto lSyntaxError;
	}
	prsr.UngetToken();
	
	/* We have successfull parsed the statement. Set up the
	** parameters for later execution.
	*/
	if (!apst.FSetSzRootSrc(szSrc) || !apst.FSetSzRootDst(szDst)) {
		apst.SetErrLast(errInternalError);
		goto lSyntaxError;
	}
	
	return fTrue;
	
lSyntaxError:
	return fFalse;
		
}

/* ------------------------------------------------------------ */
/***	CMDX::FParseExtract
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Parse the EXTRACT statement and set the state to allow
**		execution.
*/

bool
CMDX::FParseExtract()
	{
	char	szSrc[MAX_PATH];
	char	szDst[MAX_PATH];
	char	szXtr[MAX_PATH];
	
	/* The first element is an expression for the src path.
	*/
	if (!FParseExpression(cchStringMax, szSrc)) {
		goto lSyntaxError;
	}
	
	/* Next is a comma
	*/
	if (!prsr.FParseTkt(tktComma)) {
		apst.SetErrLast(errPrsrCommaExpected);
		goto lSyntaxError;
	}
	
	/* The next element is an expression for the dst path.
	*/
	if (!FParseExpression(cchStringMax, szDst)) {
		goto lSyntaxError;
	}
	
	/* Next is a comma
	*/
	if (!prsr.FParseTkt(tktComma)) {
		apst.SetErrLast(errPrsrCommaExpected);
		goto lSyntaxError;
	}
	
	/* The final element is an expression for the xtr path.
	*/
	if (!FParseExpression(cchStringMax, szXtr)) {
		goto lSyntaxError;
	}
	
	/* There shouldn't be anything else on the line.
	*/
	if (!prsr.FParseTkt(tktEof) && !prsr.FParseTkt(tktEoln)) {
		apst.SetErrLast(errPrsrUnexpectedToken);
		goto lSyntaxError;
	}
	prsr.UngetToken();
	
	/* We have successfull parsed the statement. Set up the
	** parameters for later execution.
	*/
	if (!apst.FSetSzRootSrc(szSrc) ||
		!apst.FSetSzRootDst(szDst) ||
		!apst.FSetSzRootXtr(szXtr)) {
		apst.SetErrLast(errInternalError);
		goto lSyntaxError;
	}
	
	return fTrue;
	
lSyntaxError:
	return fFalse;
		
}

/* ------------------------------------------------------------ */
/***	CMDX::FParseTrim
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Parse the UPDATE statement and set the state to allow
**		execution.
*/

bool
CMDX::FParseTrim()
	{
	char	szSrc[MAX_PATH];
	char	szDst[MAX_PATH];
	
	/* The first element is an expression for the src path.
	*/
	if (!FParseExpression(cchStringMax, szSrc)) {
		goto lSyntaxError;
	}
	
	/* Next is a comma
	*/
	if (!prsr.FParseTkt(tktComma)) {
		apst.SetErrLast(errPrsrCommaExpected);
		goto lSyntaxError;
	}
	
	/* The final element is an expression for the dst path.
	*/
	if (!FParseExpression(cchStringMax, szDst)) {
		goto lSyntaxError;
	}
	
	/* There shouldn't be anything else on the line.
	*/
	if (!prsr.FParseTkt(tktEof) && !prsr.FParseTkt(tktEoln)) {
		apst.SetErrLast(errPrsrUnexpectedToken);
		goto lSyntaxError;
	}
	prsr.UngetToken();
	
	/* We have successfull parsed the statement. Set up the
	** parameters for later execution.
	*/
	if (!apst.FSetSzRootSrc(szSrc) || !apst.FSetSzRootDst(szDst)) {
		apst.SetErrLast(errInternalError);
		goto lSyntaxError;
	}
	
	return fTrue;
	
lSyntaxError:
	return fFalse;
		
}

/* ------------------------------------------------------------ */
/***	CMDX::FParseExclude
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Parse and execute the EXCLUDE statement. 
*/

bool
CMDX::FParseExclude()
	{
	int		idKwd;
	char	szVal[cchStringMax+1];

	/* Check for 'exclude' keyword
	*/
	if (!prsr.FParseTkt(tktKeyword)) {
		apst.SetErrLast(errPrsrUnexpectedToken);
		goto lSyntaxError;
	}

	if (prsr.IdToken() != idKwdExclude) {
		prsr.UngetToken();
		apst.SetErrLast(errPrsrIncorrectKeyword);
		goto lSyntaxError;
	}
	
	/* Get the keyword for the thing being excluded and save it
	** to use after we have parsed the rest of the statment.
	*/
	if (!prsr.FParseTkt(tktKeyword)) {
		apst.SetErrLast(errPrsrSymbolExpected);
		goto lSyntaxError;
	}
	
	idKwd = prsr.IdToken();
	
	/* Eat the '='
	*/
	if (!prsr.FParseTkt(tktOperator) || (prsr.OptToken() != optAssign)) {
		apst.SetErrLast(errPrsrUnexpectedToken);
		goto lSyntaxError;
	}
	
	/* Get the string for the symbol value.
	*/
	if (!FParseExpression(cchStringMax, szVal)) {
		goto lSyntaxError;
	}
	
	/* Add the string value to the appropriate exclusion list based
	** on the keyword parsed earlier.
	*/
	switch (idKwd) {
		case idKwdExt:
			fldrmg.AddXclExt(szVal);
			break;
			
		case idKwdFile:
			fldrmg.AddXclFile(szVal);
			break;
			
		case idKwdDir:
			fldrmg.AddXclDir(szVal);
			break;
			
		case idKwdPath:
			fldrmg.AddXclPath(szVal);
			break;
			
		default:
			apst.SetErrLast(errPrsrIncorrectKeyword);
			goto lSyntaxError;
	}
	
	return fTrue;
	
lSyntaxError:
	return fFalse;
		
}

/* ------------------------------------------------------------ */
/***	CMDX::FParseClear
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Parse and execute the CLEAR statement. 
*/

bool
CMDX::FParseClear()
	{
	int		idKwd;

	/* Check for 'clear' keyword
	*/
	if (!prsr.FParseTkt(tktKeyword)) {
		apst.SetErrLast(errPrsrUnexpectedToken);
		goto lSyntaxError;
	}

	if (prsr.IdToken() != idKwdClear) {
		prsr.UngetToken();
		apst.SetErrLast(errPrsrIncorrectKeyword);
		goto lSyntaxError;
	}
	
	/* Get the keyword for the thing being cleared
	*/
	if (!prsr.FParseTkt(tktKeyword)) {
		apst.SetErrLast(errPrsrSymbolExpected);
		goto lSyntaxError;
	}
	
	idKwd = prsr.IdToken();
	
	/* Clear the thing being requested to be cleared.
	*/
	switch (idKwd) {
		case idKwdExt:
			fldrmg.ClearXclExt();
			break;
			
		case idKwdFile:
			fldrmg.ClearXclFile();
			break;
			
		case idKwdDir:
			fldrmg.ClearXclDir();
			break;
			
		case idKwdPath:
			fldrmg.ClearXclPath();
			break;
			
		case idKwdStat:
			fldrmg.ClearStat();
			break;
			
		case idKwdElapsed:
			apst.SetTicStart(GetTickCount());
			break;
			
		default:
			apst.SetErrLast(errPrsrIncorrectKeyword);
			goto lSyntaxError;
	}
	
	return fTrue;
	
lSyntaxError:
	return fFalse;
		
}

/* ------------------------------------------------------------ */
/***	CMDX::FParsePrint
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Parse and execute the PRINT statement. 
*/

bool
CMDX::FParsePrint()
	{
	FILE *		fhLog;
	char		szDate[cchDateSzMax];
	char		szTime[cchTimeSzMax];
	char		szVal[cchStringMax];
	__time64_t	tmmStart;
	struct tm	tmsStart;
	bool		fNewline;

	/* Check for 'print' keyword
	*/
	if (!prsr.FParseTkt(tktKeyword)) {
		goto lSyntaxError;
	}

	if (prsr.IdToken() != idKwdPrint) {
		prsr.UngetToken();
		apst.SetErrLast(errPrsrUnexpectedToken);
		goto lSyntaxError;
	}

	fhLog = apst.FhLogFile();
	if (fhLog == NULL) {
		/* Log file hasn't been created yet.
		*/
		printf ("No log file\n");
		goto lSyntaxError;
	}
		
	/* Parse the statement for print operations.
	*/
	fNewline = fTrue;			//default to printing newline at end of statement
	while (fTrue) {
		/* Check for expression. An expression will begin with a symbol
		** or a literal string.
		*/
		if (prsr.FParseTkt(tktSymbol) || prsr.FParseTkt(tktString)) {
			prsr.UngetToken();
			if (!FParseExpression(cchStringMax, szVal)) {
				goto lSyntaxError;
			}
			fprintf(fhLog, "%s", szVal);
		}
			
		/* Check for a keyword.
		*/	
		else if (prsr.FParseTkt(tktKeyword)) {
			/* Check for print statistics
			*/
			if (prsr.IdToken() == idKwdStat) {
				fldrmg.PrintStat(fhLog);			
			}
			
			/* Check for print elapsed time.
			*/
			else if (prsr.IdToken() == idKwdElapsed) {
				fprintf(fhLog, "Elapsed time: %f", 
				    (double)(GetTickCount() - apst.TicStart())/1000.0);
			}
			
			/* Check for print current date and time
			*/
			else if (prsr.IdToken() == idKwdDate) {	
				tmmStart = apst.TmmStart();
				_localtime64_s(&tmsStart, &tmmStart);
				DateSzFromTms(&tmsStart, cchDateSzMax, szDate);	
				fprintf(fhLog, "%s", szDate);
			}
			
			else if (prsr.IdToken() == idKwdTime) {
				tmmStart = apst.TmmStart();
				_localtime64_s(&tmsStart, &tmmStart);
				TimeSzFromTms(&tmsStart, cchTimeSzMax, szTime);	
				fprintf(fhLog, "%s", szTime);
			}
		}
		
		/* Check for hash '#' symbol to suppress printing newline at the
		** of statement.
		*/
		else if (prsr.FParseTkt(tktHash)) {
			fNewline = fFalse;
		}
			
		/* Check for end of file or end of line. If so, print
		** a newline and then stop parsing the line. We'll get here
		** when it is a print statement with no print operations
		*/
		else if (prsr.FParseTkt(tktEof) || prsr.FParseTkt(tktEoln)) {
			if (fNewline) {
				fprintf(fhLog, "\n");
			}
			break;
		}			
		else {
			/* Unexpected token on line. Syntax error.
			*/
			apst.SetErrLast(errPrsrUnexpectedToken);
			goto lSyntaxError;
		}

		/* Following the print operation, there should be either
		** an end of file, end of line, or a comma.
		*/
		if (prsr.FParseTkt(tktEof) || prsr.FParseTkt(tktEoln)) {
			if (fNewline) {
				fprintf(fhLog, "\n");
			}
			break;
		}
					
		if (!prsr.FParseTkt(tktComma)) {
			apst.SetErrLast(errAppSyntax);
			goto lSyntaxError;
		}	
	}
	
	prsr.FNextLine();	
	return fTrue;
	
lSyntaxError:
	return fFalse;
	
}
		
/* ------------------------------------------------------------ */
/***	CMDX::FParseOpt
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Parse and execute the OPT statement
*/

bool
CMDX::FParseOpt()
	{

	/* Eat any blank lines.
	*/
	while (prsr.FParseTkt(tktEoln)) {
		prsr.FNextLine();
	}

	/* Check for 'opt' keyword
	*/
	if (!prsr.FParseTkt(tktKeyword)) {
		apst.SetErrLast(errPrsrUnexpectedToken);
		goto lSyntaxError;
	}

	if (prsr.IdToken() != idKwdOpt) {
		prsr.UngetToken();
		apst.SetErrLast(errPrsrIncorrectKeyword);
		goto lSyntaxError;
	}
		
	/* Eat the '='
	*/
	if (!prsr.FParseTkt(tktOperator) || (prsr.OptToken() != optAssign)) {
		apst.SetErrLast(errPrsrUnexpectedToken);
		goto lSyntaxError;
	}
	
	while (fTrue) {
		if (!prsr.FParseAny()) {
			/* Syntax error in line.
			*/
			apst.SetErrLast(prsr.Err());
			goto lSyntaxError;
		}
		if ((prsr.TktToken() == tktEof) || (prsr.TktToken() == tktEoln)) {
			break;
		}
	
		if (prsr.TktToken() == tktKeyword) {
			if (prsr.IdToken() == idKwdCd) {
				/* copy directories option.
				*/
				apst.SetFNoCopyDir(fFalse);
			}
			else if (prsr.IdToken() == idKwdNcd) {
				/* no copy directories option
				*/
				apst.SetFNoCopyDir(fTrue);
			}
			else if (prsr.IdToken() == idKwdCf) {
				/* copy file option
				*/
				apst.SetFNoCopyFile(fFalse);
			}
			else if (prsr.IdToken() == idKwdNcf) {
				/* no copy file option
				*/
				apst.SetFNoCopyFile(fTrue);
			}
			else if (prsr.IdToken() == idKwdCs) {
				/* copy system files option
				*/
				apst.SetFDoSystem(fTrue);
			}
			else if (prsr.IdToken() == idKwdNcs) {
				/* no copy system files option
				*/
				apst.SetFDoSystem(fFalse);
			}
			else if (prsr.IdToken() == idKwdCh) {
				/* copy hidden files option
				*/
				apst.SetFDoHidden(fTrue);
			}
			else if (prsr.IdToken() == idKwdNch) {
				/* no copy hidden files option
				*/
				apst.SetFDoHidden(fFalse);
			}
			else if (prsr.IdToken() == idKwdPf) {
				/* log file names option
				*/
				apst.SetFLogFile(fTrue);
			}
			else if (prsr.IdToken() == idKwdNpf) {
				/* no log file names option
				*/
				apst.SetFLogFile(fFalse);
			}
			else if (prsr.IdToken() == idKwdPd) {
				/* log directory names option
				*/
				apst.SetFLogDir(fTrue);
			}
			else if (prsr.IdToken() == idKwdNpd) {
				/* no log directory names option.
				*/
				apst.SetFLogDir(fFalse);
			}
			else if (prsr.IdToken() == idKwdPxf) {
				/* log excluded files option
				*/
				apst.SetFLogXclFile(fTrue);
			}
			else if (prsr.IdToken() == idKwdNpxf) {
				/* don't log excluded files option
				*/
				apst.SetFLogXclFile(fFalse);
			}
			else if (prsr.IdToken() == idKwdPxd) {
				/* log excluded directories option
				*/
				apst.SetFLogXclDir(fTrue);
			}
			else if (prsr.IdToken() == idKwdPact) {
				/* log actions begin performed.
				*/
				apst.SetFLogAction(fTrue);
			}
			else if (prsr.IdToken() == idKwdNpact) {
				/* don't log actions being performed.
				*/
				apst.SetFLogAction(fFalse);
			}
			else if (prsr.IdToken() == idKwdNpxd) {
				/* don't log excluded directories option
				*/
				apst.SetFLogXclDir(fFalse);
			}
			else if (prsr.IdToken() == idKwdXu) {
				/* extract unique files option
				*/
				apst.SetFExtractUnique(fTrue);
			}
			else if (prsr.IdToken() == idKwdNxu) {
				/* don't extract unique files option
				*/
				apst.SetFExtractUnique(fFalse);
			}
			else if (prsr.IdToken() == idKwdXc) {
				/* extract changed file option
				*/
				apst.SetFExtractDiff(fTrue);
			}
			else if (prsr.IdToken() == idKwdNxc) {
				/* don't extract changed files option
				*/
				apst.SetFExtractDiff(fFalse);
			}
			else if (prsr.IdToken() == idKwdSig) {
				/* use file signatures
				*/
				apst.SetFUseSignatures(fTrue);
			}
			else if (prsr.IdToken() == idKwdNsig) {
				/* don't use file signatures
				*/
				apst.SetFUseSignatures(fFalse);
			}
			else {
				/* invalid option keyword.
				*/
				apst.SetErrLast(errPrsrUnexpectedToken);
				goto lSyntaxError;
			}	
		}
		else {
			/* Unexpected token on line. Syntax error.
			*/
			apst.SetErrLast(errPrsrUnexpectedToken);
			goto lSyntaxError;
		}
		
		/* Following the option keyword, there should be either
		** an end of file, end of line, or a comma.
		*/
		if (prsr.FParseTkt(tktEof) || prsr.FParseTkt(tktEoln)) {
			break;
		}
		
		if (!prsr.FParseTkt(tktComma)) {
			apst.SetErrLast(errPrsrUnexpectedToken);
			goto lSyntaxError;
		}	
	}
	
	prsr.FNextLine();
	
	return fTrue;
	
lSyntaxError:
	apst.SetStProg(stFileSyntax);
	return fFalse;
	
}

/* ------------------------------------------------------------ */
/***	CMDX::FParseInclude
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
**		Parse and execute the INCLUDE statement. Parse the file
**		name from the command line, find the command file, and
**		begin executing it if found.
*/

bool
CMDX::FParseInclude()
	{
	char	szCmd[MAX_PATH];

	/* Get the file name string from the statement.
	*/	
	if (!prsr.FParseTkt(tktString)) {
		apst.SetErrLast(errAppSyntax);
		return fFalse;
	}
	
	/* Check to see if the file exists and get the complete path
	** to the file.
	*/
	if (!app.FFindCommandFile(prsr.SzToken(), szCmd)) {
		return fFalse;
	}
	
	/* Start execution of the command file.
	*/
	if (!prsr.FBeginFile(szCmd)) {
		return fFalse;
	}
	
	return fTrue;
}

/* ------------------------------------------------------------ */
/***	CMDX::FParseSet
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Parse and execute the SET statement. This is used to set the
**		value of a symbol in the symbol table.
*/

bool
CMDX::FParseSet()
	{
	char	szSym[cchSymbolMax+1];
	char	szVal[cchStringMax+1];

	/* Eat any blank lines.
	*/
	while (prsr.FParseTkt(tktEoln)) {
		prsr.FNextLine();
	}

	/* Check for 'set' keyword
	*/
	if (!prsr.FParseTkt(tktKeyword)) {
		apst.SetErrLast(errPrsrUnexpectedToken);
		goto lSyntaxError;
	}

	if (prsr.IdToken() != idKwdSet) {
		prsr.UngetToken();
		apst.SetErrLast(errPrsrIncorrectKeyword);
		goto lSyntaxError;
	}
	
	/* Get the symbol name.
	*/
	if (!prsr.FParseTkt(tktSymbol)) {
		apst.SetErrLast(errPrsrSymbolExpected);
		goto lSyntaxError;
	}
	
	/* Save it for later.
	*/
	if (CchOfSz(prsr.SzToken()) > cchSymbolMax) {
		apst.SetErrLast(errPrsrSymbolTooLong);
		goto lSyntaxError;
	}
	SzCopy(szSym, prsr.SzToken());
		
	/* Eat the '='
	*/
	if (!prsr.FParseTkt(tktOperator) || (prsr.OptToken() != optAssign)) {
		apst.SetErrLast(errPrsrUnexpectedToken);
		goto lSyntaxError;
	}
	
	/* Get the string for the symbol value.
	*/
	if (!FParseExpression(cchStringMax, szVal)) {
		goto lSyntaxError;
	}
	
	/* Assign the value to the symbol in the symbol table.
	*/
	smtb[szSym] = szVal;
	
	return fTrue;
	
lSyntaxError:
	return fFalse;
	
}

/* ------------------------------------------------------------ */
/***	CMDX::FParseLog
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
**		Parse the log statement and turn logging on or off as
**		appropriate. The first time logging is turned on, the
**		log file will be created.
*/

bool
CMDX::FParseLog()
	{
	
	/* The 'log' keyword has already been parsed. The next token 
	** must be either the keyword 'on' or the keyword 'off'.
	*/
	if (!prsr.FParseTkt(tktKeyword)) {
		apst.SetErrLast(errPrsrUnexpectedToken);
		goto lSyntaxError;
	}
	
	if (prsr.IdToken() == idKwdOn) {
		if (apst.FhLogFile() == NULL) {
			/* Log file hasn't been created yet. Create it.
			*/
			if (!FCreateLogFile()) {
				goto lFileError;
			}
		}
		
		/* Turn logging on.
		*/
		apst.SetFLogEnable(fTrue);
	
	}
	else if (prsr.IdToken() == idKwdOff) {
		/* Turn logging off.
		*/
		apst.SetFLogEnable(fFalse);
	}
	else {
		apst.SetErrLast(errPrsrUnexpectedToken);
		goto lSyntaxError;
	}
	
	/* All done
	*/
	return fTrue;
	
lSyntaxError:
	apst.SetStProg(stFileSyntax);
	return fFalse;
	
lFileError:
	apst.SetErrLast(errFileSystem);
	return fFalse;
	
}

/* ------------------------------------------------------------ */
/*				Misc. Parsing Functions							*/
/* ------------------------------------------------------------ */
/***	CMDX::FParseStringValue
**
**	Parameters:
**		idKwd		- keyword to look for
**		cchMax		- maximum number of characters to return
**		szVal		- buffer to receive string
**
**	Returns:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		This routine will parse a string valued statement from a
**		command file.
*/

bool
CMDX::FParseStringValue(int idKwd, int cchMax, char * szVal)
	{
	bool	fStat;

	fStat = fTrue;

	/* Check for keyword symbol
	*/
	if (!prsr.FParseTkt(tktKeyword) || (prsr.IdToken() != idKwd)) {
		prsr.UngetToken();
		apst.SetErrLast(errPrsrIncorrectKeyword);
		return fFalse;
	}
		
	/* Eat the '='
	*/
	if (!prsr.FParseTkt(tktOperator) || (prsr.OptToken() != optAssign)) {
		apst.SetErrLast(errPrsrIncorrectKeyword);
		return fFalse;
	}

	/* Get the string from the statement.
	*/
	if (!FParseExpression(cchMax, szVal)) {
		return fFalse;
	}			

	return fStat;
}
/* ------------------------------------------------------------ */
/***	CMDX::FParseExpression
**
**	Parameters:
**		cchMax		- length of result buffer
**		szRes		- buffer for parsed string
**
**	Return Value:
*		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not.
**
**	Description:
**		Parse and expression from the current statement. The only
**		expressions have a string as the resulting value. Expressions
**		contain the following:
**			elem :=	<literal> | <symbol>
**			expr := <elem>[+elem]
*/

bool
CMDX::FParseExpression(int cchMax, char * szRes)
	{
	char	szStr[cchStringMax];
	bool	fOperandOk;
	bool	fOperatorOk;

	fOperandOk = fTrue;
	fOperatorOk = fFalse;
	szStr[0] = '\0';		
	while (fTrue) {
		if (!prsr.FParseAny()) {
			/* Syntax error in line.
			*/
			apst.SetErrLast(prsr.Err());
			return fFalse;
		}

		/* Check if it is a literal string, and append it to the result
		** string if so.
		*/
		if (fOperandOk && (prsr.TktToken() == tktString)) {
			fOperandOk = fFalse;
			fOperatorOk = fTrue;
			if ((CchOfSz(szStr)+CchOfSz(prsr.SzToken())) < cchMax) {
				SzAppend(szStr, prsr.SzToken());
			}
			else {
				goto lStringTooLong;
			}
		}
		
		/* Check for symbol and append the value if so.
		*/
		else if (fOperandOk && (prsr.TktToken() == tktSymbol)) {
			fOperandOk = fFalse;
			fOperatorOk = fTrue;
			if (smtb.find(prsr.SzToken()) == smtb.end()) {
				goto lSymNotFound;
			}
			string st = smtb[prsr.SzToken()];	//M00BUG - This creates an empty symbol if it isn't already defined
			const char * sz = st.c_str();
			if ((CchOfSz(szStr)+CchOfSz(sz)) < cchMax) {
				SzAppend(szStr, sz);
			}
			else {
				goto lStringTooLong;
			}
		}
		
		/* Check for the '+' to indicate concatenation.
		** In this case continue parsing.
		*/
		else if (fOperatorOk &&
				((prsr.TktToken() == tktOperator) &&
				 (prsr.OptToken() == optPlus))) {
			fOperandOk = fTrue;
			fOperatorOk = fFalse;
		}
		
		/* Any other token ends the expression.
		*/
		else {
			prsr.UngetToken();
			break;
		}

	}

	if (CchOfSz(szStr) != 0) {
		SzCopy(szRes, szStr);
	}	
	return fTrue;

lStringTooLong:
	apst.SetErrLast(errAppStringTooLong);
	return fFalse;
	
lSymNotFound:
	apst.SetErrLast(errPrsrUndefinedSymbol);
	return fFalse;
}

/* ------------------------------------------------------------ */
/***	CMDX::FCreateLogFile
**
**	Parameters:
**		none
**
**	Return Value:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Generate the log file name from the command file name
**		and the current date and time. Create a new log file.
*/

bool
CMDX::FCreateLogFile()
	{
	bool		fStat;
	FILE *		fh;
	char		szName[MAX_PATH];
	char		szTemp[MAX_PATH];
	char *		pchT;
	errno_t		err;
	__time64_t	tmmStart;
	struct tm	tmsStart;

	fStat = fFalse;	

	/* Check if the user specified a log file name on command line.
	** If not, generate default name from command file name, date and time.
	*/
	if (apst.SzLogFile() != NULL) {
		SzCopy(szName, apst.SzLogFile());
	}
	else {	
		/* Generate the file name. If the path to the standard log file location
		** is set, use that as the root of the path. If not, the log file will be
		** created in the current directory.
		*/
		szName[0] = '\0';
		if (apst.SzLogPath() != NULL) {
			SzCopy(szName, apst.SzLogPath());
			SzAppend(szName, "\\");
		}
		
		/* If we are running from a command file entered via the -f
		** parameter on the command line, SzComamndFile will have a
		** value. If SzCommandFile is null, then we must be executing
		** the startup.ts command file and the user is doing a
		** command line operation and not running a command file. We
		** don't have a name to base the log file name on, so we'll
		** default to 'startup.log'.
		*/
		if (apst.SzCommandFile() != NULL) {
			SzCopy(szTemp, apst.SzCommandFile());
			SzAppend(szName, SzTitleOfName(szTemp));
		}
		else {
			SzAppend(szName, szLogFileDefault);
		}
		pchT = SzExtOfName(szName);
		if (pchT == NULL) {
			/* filename has no extension.
			*/
			pchT = SzEnd(szName);
		}
		*pchT = '\0';
		
		tmmStart = apst.TmmStart();
		_localtime64_s(&tmsStart, &tmmStart);
		DateSzFromTms(&tmsStart, cchDateSzMax, szTemp);
		
		while ((pchT = PchOfCh(szTemp, '/')) != NULL) {
			*pchT = '-';
		}
		
		SzAppend(szName, " [");
		SzAppend(szName, szTemp);
		
		tmmStart = apst.TmmStart();
		_localtime64_s(&tmsStart, &tmmStart);
		TimeSzFromTms(&tmsStart, cchTimeSzMax, szTemp);
		
		while ((pchT = PchOfCh(szTemp, ':')) != NULL) {
			*pchT = '-';
		}
		
		SzAppend(szName, ",");
		SzAppend(szName, szTemp);
		SzAppend(szName, "]");

		SzAppend(szName, ".log");
	}
	
	/* Create the file.
	*/
	if (!apst.FLogFileCreated()) {
		/* If this is the first time to open the log file on this
		** execution, we need to create it empty.
		*/
		err = fopen_s(&fh, szName, "wt");
		apst.SetFLogFileCreated(fTrue);
	}
	else {
		/* If the log file was previously created, open it for
		** append. This will happen if a startup.ts command file
		** turned logging on.
		*/
		err = fopen_s(&fh, szName, "a+t");
	}
	if (err != 0) {
		apst.SetErrLast(errFileSystem);
		goto lErrorExit;
	}
	
	/* Store the file handle.
	*/
	apst.SetFhLogFile(fh); 

	fStat = fTrue;
	
lErrorExit:	
	return fStat;
	
}

/* ------------------------------------------------------------ */
/***	CMDX::CloseLogFile
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
**		Close the log output file.
*/

void
CMDX::CloseLogFile()
	{
	FILE *	fh;
	
	fh = apst.FhLogFile();
	if (fh != NULL) {
		fclose(fh);
		apst.SetFhLogFile(NULL);
	}
	
}

/* ------------------------------------------------------------ */
/***	CMDX::LogMessage
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
**		Write the message string to the log file if logging the
**		particular type of message is enabled
*/

void
CMDX::LogMessage(int idMsg, char * szMsg)
	{
	FILE *	fh;
	int		cch;
	
	cch = CchOfSz(szMsg);

	/* Check that the log file has been created and that logging
	** is enabled.
	*/
	if (!apst.FLogEnable()) {
		return;
	}
	fh = apst.FhLogFile();
	Assert(fh != NULL, "Logging enabled, but log file not open");
	if (fh == NULL) {
		return;
	}
	
	/* Determine what type of message this is. If logging of that
	** type of message is enabled, write it to the log file.
	*/
	switch(idMsg) {
		case idMsgError:
			fprintf(fh, "%s",szMsg);
			break;
			
		case idMsgCreateDir:
			if (apst.FLogDir()) {
				fprintf(fh, "%s", szMsg);
			}
			break;
			
		case idMsgSkipDir:
			if (apst.FLogXclDir()) {
				fprintf(fh, "%s", szMsg);
			}
			break;
			
		case idMsgCopyFile:
			if (apst.FLogFile()) {
				fprintf(fh, "%s", szMsg);
			}
			break;
			
		case idMsgExtrFile:
			if (apst.FLogFile()) {
				fprintf(fh, "%s", szMsg);
			}
			break;
			
		case idMsgDelFile:
			if (apst.FLogFile()) {
				fprintf(fh, "%s", szMsg);
			}
			break;
			
		case idMsgExclFile:
			if (apst.FLogXclFile()) {
				fprintf(fh, "%s", szMsg);
			}
			break;
			
		case idMsgSetTime:
			if (apst.FLogFile()) {
				fprintf(fh, "%s", szMsg);
			}
			break;
			
		case idMsgAction:
			if (apst.FLogAction()) {
				fprintf(fh, "%s", szMsg);
			}
			break;
			
		default:
			Assert(fFalse, "Unknown log message type");
			
	}
}

/* ------------------------------------------------------------ */
/***	CMDX::DateSzFromTms
**
**	Parameters:
**		ptms		- pointer to time structure
**		cchBuf		- length of buffer to receive result
**		szBuf		- buffer to receive result.
**
**	Return Value:
**		none		
**
**	Errors:
**		none
**
**	Description:
**		Converts date fields in time structure to string.
*/

void
CMDX::DateSzFromTms(tm * ptms, int cchBuf, char * szBuf)
	{
	char	szDate[cchDateSzMax];
	
	sprintf_s(szDate, cchDateSzMax, "%2.2d/%2.2d/%4.4d",
		ptms->tm_mon+1, ptms->tm_mday, ptms->tm_year+1900);
		
	strncpy_s(szBuf, cchBuf, szDate, cchBuf-1);

}

/* ------------------------------------------------------------ */
/***	CMDX::TimeSzFromTms
**
**	Parameters:
**		ptms		- pointer to time structure
**		cchBuf		- length of buffer to receive result
**		szBuf		- buffer to receive result.
**
**	Return Value:
**		none		
**
**	Errors:
**		none
**
**	Description:
**		Converts time fields in time structure to string.
*/

void
CMDX::TimeSzFromTms(tm * ptms, int cchBuf, char * szBuf)
	{
	char	szTime[cchTimeSzMax];
	
	sprintf_s(szTime, cchTimeSzMax, "%2.2d:%2.2d:%2.2d",
				ptms->tm_hour, ptms->tm_min, ptms->tm_sec);
	strncpy_s(szBuf, cchBuf, szTime, cchBuf-1);
	
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

