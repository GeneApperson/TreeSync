/************************************************************************/
/*																		*/
/*	cmdx.h	--	CMDX Object Class Declarations							*/
/*																		*/
/************************************************************************/
/*	Author:		Gene Apperson											*/
/*	Copyright 2011, Gene Apperson										*/
/************************************************************************/
/*  File Description:													*/
/*																		*/
/*	Interface declarations for the CMDX object class which is used to	*/
/*	parse and exeucte command files.									*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	03/05/2011(GeneA): created											*/
/*																		*/
/************************************************************************/

#if !defined(CMDX_INC)
#define	CMDX_INC

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include "prsr.h"

#include <map>
#include <string>

using namespace std;

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*					General Type Declarations					*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Object Class Declarations					*/
/* ------------------------------------------------------------ */

typedef map<string,string> SMTB;

/* Forward declaration for tm structure so that it isn't necessary
** to  include <time.h> when including this file.
*/
struct tm;

class CMDX {
private:
	PRSR 	prsr;
	SMTB	smtb;

protected:
	void	InitKeywords();
	bool	FDoUpdate();
	bool	FDoExtract();
	bool	FDoTrim();
	bool	FParseUpdate();
	bool	FParseExtract();
	bool	FParseTrim();
	bool	FParseSet();
	bool	FParseOpt();
	bool	FParseLog();
	bool	FParsePrint();
	bool	FParseInclude();
	bool	FParseExclude();
	bool	FParseClear();
	bool	FParseExpression(int cchMax, char * szRes);
	bool	FParseStringValue(int idKwd, int cchMax, char * szVal);
	bool	FCreateLogFile();
	void	CloseLogFile();
	void	DateSzFromTms(tm * tms, int cchBuf, char * szBuf);
	void	TimeSzFromTms(tm * tms, int cchBuf, char * szBuf);
	void	ReportError();

public:
			CMDX();
		   ~CMDX();
	bool	FInit();
	void	Terminate();
	bool	FExecute(char * szFile);
	void	LogMessage(int idMsg, char * szMsg);
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
