/************************************************************************/
/*                                                                      */
/*  prsr.cpp	--  PRSR Object Class Implementation					*/
/*                                                                      */
/************************************************************************/
/*	Author: 	Gene Apperson											*/
/*	Copyright 2011, Gene Apperson										*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*	This module contains the implementation for the object class used	*/
/*	to parse command files.												*/
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*	03/05/2011(GeneA): created by copying from ddb86 project.			*/
/*                                                                      */
/************************************************************************/


/* ------------------------------------------------------------ */
/*					Include File Definitions					*/
/* ------------------------------------------------------------ */

#include	<windows.h>
#include	<stdio.h>
#include	<commctrl.h>

#include	"../inc/dtypes.h"
#include	"../inc/dutil.h"
#include	"../inc/dassert.h"

#include	"config.h"
#include	"appdefs.h"
#include	"apst.h"
#include	"lexr.h"
#include	"prsr.h"

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
/*				PRSR Object Class Implementation				*/
/* ------------------------------------------------------------ */
/***	PRSR::PRSR
**
**	Synopsis:
**			PRSR()
**
**	Input:
**		none
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Default constructor
*/

PRSR::PRSR()
	{

	plexr = NULL;

}

/* ------------------------------------------------------------ */
/***	PRSR::~PRSR
**
**	Synopsis:
**		PRSR::~PRSR()
**
**	Input:
**		none
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Default destructor
*/

PRSR::~PRSR()
	{
	AssertMsg(szMsg, "PRSR not properly terminated before destruction");

	Assert(plexr == NULL, szMsg);

}

/* ------------------------------------------------------------ */
/***	PRSR::FInit
**
**	Parameters:
**		none
**
**	Returns:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Initialize the parser for use. Create and initialize the
**		LEXR object used to scan the input stream.
*/

bool
PRSR::FInit()
	{
	LEXR *	plexrT;
	
	plexrT = new LEXR;
	if (plexrT == NULL) {
		err = errOutOfMemory;
		return fFalse;
	}
	if (!plexrT->FInit(cchLexrLineBuf)) {
		return fFalse;
	}

	plexrT->FSetScanStyle(flgLexrStyleIntel);

	plexr = plexrT;	

	return fTrue;

}

/* ------------------------------------------------------------ */
/***	PRSR::Terminate
**
**	Parameters:
**		none
**
**	Returns:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Free any resources before destruction.
*/

void
PRSR::Terminate()
	{

	if (plexr != NULL) {
		plexr->Terminate();
		delete plexr;
		plexr = NULL;
	}

}

#if OptLexrKeywords != 0
/* ------------------------------------------------------------ */
/***	PRSR::AddKeyword
**
**	Parameters:
**		sz		- keyword string
**		id		- corresponding keyword id
**
**	Returns:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Add the specified to the LEXR keyword mapping table.
*/

void
PRSR::AddKeyword(char * sz, int id)
	{
	
	plexr->AddKeyword(sz, id);
	
}

#endif

/* ------------------------------------------------------------ */
/***	PRSR::FBeginFile
**
**	Parameters:
**		szFile		- file name string for file to parse
**
**	Returns:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Open the specified input file for parsing.
*/

bool
PRSR::FBeginFile(char * szFile)
	{

	if (!plexr->FOpenFile(szFile)) {
		return fFalse;
	}
	
	Start();

	return fTrue;

}

/* ------------------------------------------------------------ */
/***	PRSR::Start
**
**	Synopsis:
**		void Start()
**
**	Input:
**		none
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This routine resets internal state to begin a new parse.
*/

void
PRSR::Start()
	{

	plexr->StartScan();

}

/* ------------------------------------------------------------ */
#if OptPrsrStringParse != 0
/* ------------------------------------------------------------ */
/***	PRSR::FBeginStringParse
**
**	Synopsis:
**		fSuccess = FInitStringParse(sz)
**
**	Input:
**		sz		- string to begin parsing
**
**	Output:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		This routine will initialize for parsing the specified
**		string.
*/

bool
PRSR::FBeginStringParse(char * sz)
	{
	LEXR *	plexrT;

	plexrT = new LEXR;
	if (plexrT == NULL) {
		return fFalse;
	}
	if (!plexrT->FInit(cchLexrLineBuf)) {
		return fFalse;
	}

	plexrT->FSetScanStyle(flgLexrStyleC);
	
	FInit(plexrT);
	Start();
	if (!FSetLine(sz)) {
		return fFalse;
	}

	return fTrue;

}

/* ------------------------------------------------------------ */
/***	PRSR::EndStringParse
**
**	Input:
**		none
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Called to clean up after parse begun using FInitStringParse
*/

void
PRSR::EndStringParse()
	{
	
	Terminate();
	
}

/* ------------------------------------------------------------ */
#endif	//OptPrsrStringParse
/* ------------------------------------------------------------ */
/***	PRSR::FSetLine
**
**	Synopsis:
**		fSuccess = FSetLine(sz)
**
**	Input:
**		sz		- text string to set as next line to parse
**
**	Output:
**		none
**
**	Errors:
**		Return fTrue if successful, fFalse if not
**
**	Description:
**		Set the specified line as the next line to parse.
*/

bool
PRSR::FSetLine(char * sz)
	{

	return plexr->FSetLine(sz);

}

/* ------------------------------------------------------------ */
/***	PRSR::FParseTkt
**
**	Synopsis:
**		fSuccess = FParseTkt(tkt)
**
**	Input:
**		tkt		- Type of token to parse.
**
**	Output:
**		none
**
**	Errors:
**		Returns fTrue if next token is of specified type
**
**	Description:
**		This routine will parse the next token from the input
**		stream. It returns fTrue if the token is of the specified
**		type and fFalse if not. If the next token isn't of the
**		specified type, it is pushed back onto the input stream.
*/

bool
PRSR::FParseTkt(int tkt)
	{

	/* Get the next token and see if it is of the requested
	** type.
	*/
	plexr->NextToken();
	ptkn = plexr->PtknCur();
	err = plexr->Err();

	if (err == errOK) {
		if ((tkt == tktAny) || (ptkn->TktToken() == tkt)) {
			return fTrue;
		}
	}

#if defined(DEAD)
	/* Wasn't the requested type. Was the request for one of
	** the extended types defined by the parser.
	*/
	WORD		wvalT;
	if (tkt == tktRegister) {
		if (ptkn->TktToken() == tktSymbol) {
			pem = pvwapp->PdbgsCur()->GetEm();
			if ((idr = pem->IdrFromSz(ptkn->SzToken())) == -1) {
				goto lTktNotFound;
			}
			pem->FGetClientReg(idr, &wvalT);
			ptkn->SetTktTkn(tktRegister);
			ptkn->SetDwVal(wvalT);
			return fTrue;
		}
		goto lTktNotFound;
	}
lTktNotFound:
#endif

	/* Not the kind of token that we were looking for.
	*/
	plexr->UngetToken();
	ptkn = NULL;
	return fFalse;

}

/* ------------------------------------------------------------ */
/*				Specific Token Parse Routines					*/
/* ------------------------------------------------------------ */
/***	PRSR::FParseNumConst
**
**	Synopsis:
**		fSuccess = FParseNumConst(iVal)
**
**	Input:
**		pval	- variable to receive parsed numeric constant
**
**	Output:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		This routine will parse a signed numeric constant.
*/

bool
PRSR::FParseNumConst(long & lVal)
	{
	int		lSign;


	lSign = 1;

	/* Check for unary '-'
	*/
	if (FParseTkt(tktOperator) && (OptToken() == optMinus)) {
		lSign = -1;
	}

	if (!FParseTkt(tktConstant) || (CstToken() != cstNumber)) {
		err = errPrsrSyntax;
		return fFalse;
	}

	lVal = lSign * (long)(ptkn->DwVal());

	return fTrue;

}

/* ------------------------------------------------------------ */
/***	PRSR::FParseWord
**
**	Synopsis:
**		fSuccess = FParseWord(wVal)
**
**	Input:
**		wVal		- variable to receive parsed value
**
**	Output:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		This routine will parse an unsigned word.
*/

bool
PRSR::FParseWord(WORD & wVal)
	{
	DWORD	dw;

	if (!FParseTkt(tktConstant) || (CstToken() != cstNumber)) {
		err = errPrsrSyntax;
		return fFalse;
	}

	dw = ptkn->DwVal();
	if ((dw & 0xFFFF0000) != 0) {
		/* Value out of range.
		*/
		err = errPrsrOutOfRange;
		return fFalse;
	}

	wVal = (WORD)dw;

	return fTrue;

}

#if OptPrsrSegOff != 0
/* ------------------------------------------------------------ */
/***	PRSR::FParseSegOff
**
**	Synopsis:
**		fSuccess = FParseWord(seg, off)
**
**	Input:
**		seg		- variable to receive parsed segment value
**		off		- variable to receive parsed offset value
**
**	Output:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		This routine will parse an unsigned word.
*/

bool
PRSR::FParseSegOff(WORD & seg, WORD & off)
	{

	if (!FParseWord(seg)) {
		return fFalse;
	}
	if (!FParseTkt(tktColon)) {
		return fFalse;
	}
	if (!FParseWord(off)) {
		return fFalse;
	}

	return fTrue;

}
#endif

/* ------------------------------------------------------------ */
/*				General Statement Parse Routines				*/
/* ------------------------------------------------------------ */

#if OptPrsrBoolValue != 0
/* ------------------------------------------------------------ */
/***	PRSR::FParseBoolValue
**
**	Synopsis:
**		fSuccess = FParseBoolValue(fVal)
**
**	Input:
**		szKwd	- keyword to look for
**		fVal	- variable to receive parsed value
**
**	Output:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not.
**
**	Description:
**		This routine will parse a boolean value statement from a
**		workspace file.
*/

bool
PRSR::FParseBoolValue(char * szKwd, bool & fVal)
	{
	bool	fStat;
	int		ival;

	err = errPrsrOk;

	/* Eat any blank lines.
	*/
	while (FParseTkt(tktEoln)) {
		FNextLine();
	}

	/* Check for 'active' symbol
	*/
	if (!FParseTkt(tktSymbol)) {
		return fFalse;
	}

	if (strcmp(SzToken(), szKwd) != 0) {
		UngetToken();
		return fFalse;
	}
		
	/* Eat the '='
	*/
	if (!FParseTkt(tktOperator) || (OptToken() != optAssign)) {
		err = errPrsrUnexpectedToken;
		return fFalse;
	}

	/* Get the statement value.
	*/
	if (fStat = (FParseTkt(tktConstant) && (CstToken() == cstNumber))) {
		ival = ptkn->DwVal();
		fVal = (ival != 0);
	}

	fStat &= FParseTkt(tktEoln);


	return fStat;
}
#endif

#if OptPrsrIntValue != 0
/* ------------------------------------------------------------ */
/***	PRSR::FParseIntValue
**
**	Synopsis:
**		fSuccess = FParseIntValue(szKwd, iVal)
**
**	Input:
**		szKwd	- keyword string to look for
**		iVal	- reference to variable to receive parsed line number
**
**	Output:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not.
**
**	Description:
**		This routine will parse an integer valued statement from a
**		workspace file.
*/

bool
PRSR::FParseIntValue(char * szKwd, int & iVal)
	{
	bool	fStat;

	err = errPrsrOk;

	/* Eat any blank lines.
	*/
	while (FParseTkt(tktEoln)) {
		FNextLine();
	}

	/* Check for keyword symbol
	*/
	if (!FParseTkt(tktSymbol)) {
		return fFalse;
	}

	if (strcmp(SzToken(), szKwd) != 0) {
		UngetToken();
		return fFalse;
	}
		
	/* Eat the '='
	*/
	if (!FParseTkt(tktOperator) || (OptToken() != optAssign)) {
		err = errPrsrUnexpectedToken;
		return fFalse;
	}

	/* Get the line number value.
	*/
	if (fStat = (FParseTkt(tktConstant) && (CstToken() == cstNumber))) {
		iVal = (int)ptkn->DwVal();
	}

	fStat &= FParseTkt(tktEoln);


	return fStat;
}
#endif

#if OptPrsrStringValue != 0
#if OptLexrKeywords != 0
/* ------------------------------------------------------------ */
/***	PRSR::FParseStringValue
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
PRSR::FParseStringValue(int idKwd, int cchMax, char * szVal)
	{
	bool	fStat;
	char *	sz;

	err = errPrsrOk;
	fStat = fTrue;

	/* Eat any blank lines.
	*/
	while (FParseTkt(tktEoln)) {
		FNextLine();
	}

	/* Check for keyword symbol
	*/
	if (!FParseTkt(tktKeyword)) {
		err = errOutOfMemory;
		return fFalse;
	}

	if (IdToken() != idKwd) {
		UngetToken();
		err = errPrsrIncorrectKeyword;
		return fFalse;
	}
		
	/* Eat the '='
	*/
	if (!FParseTkt(tktOperator) || (OptToken() != optAssign)) {
		err = errPrsrUnexpectedToken;
		return fFalse;
	}

	/* Get the string from the statement.
	*/
	if (FParseTkt(tktString)) {
		sz = SzToken();
		if (CchOfSz(sz) < cchMax) {
			SzCopy(szVal, SzToken());
		}
		else {
			err = errPrsrStringTooLong;
			fStat = fFalse;
		}
	}
	else {
		err = errPrsrUnexpectedToken;
		fStat = fFalse;
	}			

	if (!FParseTkt(tktEoln)) {
		err = errPrsrSyntax;
		fStat = fFalse;
	}

	return fStat;
}
#endif	//OptLexrKeywords != 0

#if OptLexrKeywords == 0
/* ------------------------------------------------------------ */
/***	PRSR::FParseStringValue
**
**	Parameters:
**		szKwd		- keyword to look for
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
PRSR::FParseStringValue(char * szKwd, int cchMax, char * szVal)
	{
	bool	fStat;
	char *	sz;

	err = errPrsrOk;
	fStat = fTrue;

	/* Eat any blank lines.
	*/
	while (FParseTkt(tktEoln)) {
		FNextLine();
	}

	/* Check for keyword symbol
	*/
	if (!FParseTkt(tktSymbol)) {
		err = errOutOfMemory;
		return fFalse;
	}

	if (CompareSz(SzToken(), szKwd) != 0) {
		UngetToken();
		err = errPrsrIncorrectKeyword;
		return fFalse;
	}
		
	/* Eat the '='
	*/
	if (!FParseTkt(tktOperator) || (OptToken() != optAssign)) {
		err = errPrsrUnexpectedToken;
		return fFalse;
	}

	/* Get the string from the statement.
	*/
	if (FParseTkt(tktString)) {
		sz = SzToken();
		if (CchOfSz(sz) < cchMax) {
			SzCopy(szVal, SzToken());
		}
		else {
			err = errPrsrStringTooLong;
			fStat = fFalse;
		}
	}
	else {
		err = errPrsrUnexpectedToken;
		fStat = fFalse;
	}			

	if (!FParseTkt(tktEoln)) {
		err = errPrsrSyntax;
		fStat = fFalse;
	}

	return fStat;
}
#endif	//OptLexrKeywords == 0
#endif	//OptPrsrStringValue
/* ------------------------------------------------------------ */

#if OptPrsrRectStmt != 0
/* ------------------------------------------------------------ */
/***	PRSR::FParseRect
**
**	Synopsis:
**		fSuccess = FParseRect(prct)
**
**	Input:
**		prct	- pointer to RECT to fill in
**
**	Output:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		This routine will parse the operands for a RECT
**		statment.
*/

bool
PRSR::FParseRect(RECT & rct)
	{
	bool	fStat;

	err = errPrsrOk;

	/* Eat any blank lines.
	*/
	while (FParseTkt(tktEoln)) {
		FNextLine();
	}

	/* Check for 'rect' symbol
	*/
	if (!FParseTkt(tktSymbol)) {
		return fFalse;
	}

	if (strcmp(SzToken(), szKwdRect) != 0) {
		UngetToken();
		return fFalse;
	}
		
	/* Eat the '='
	*/
	if (!FParseTkt(tktOperator) || (OptToken() != optAssign)) {
		err = errPrsrUnexpectedToken;
		return fFalse;
	}

	/* Get the rect field values.
	*/
	fStat = FParseNumConst(rct.left);
	fStat &= FParseTkt(tktComma);

	fStat &= FParseNumConst(rct.top);
	fStat &= FParseTkt(tktComma);

	fStat &= FParseNumConst(rct.right);
	fStat &= FParseTkt(tktComma);

	fStat &= FParseNumConst(rct.bottom);
	fStat &= FParseTkt(tktEoln);


	return fStat;
}
#endif

#if OptPrsrFileStmt != 0
/* ------------------------------------------------------------ */
/***	PRSR::FParseFile
**
**	Synopsis:
**		fSuccess = FParseFile(szFile)
**
**	Input:
**		szFile		- buffer to receive file name
**
**	Output:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		This routine will parse the 'file' statement from a
**		workspace file.
*/

bool
PRSR::FParseFile(char * szFile)
	{
	bool	fStat;

	err = errPrsrOk;
	fStat = fTrue;

	/* Eat any blank lines.
	*/
	while (FParseTkt(tktEoln)) {
		FNextLine();
	}

	/* Check for 'file' symbol
	*/
	if (!FParseTkt(tktSymbol)) {
		return fFalse;
	}

	if (strcmp(SzToken(), szKwdFile) != 0) {
		UngetToken();
		return fFalse;
	}
		
	/* Eat the '='
	*/
	if (!FParseTkt(tktOperator) || (OptToken() != optAssign)) {
		err = errPrsrUnexpectedToken;
		return fFalse;
	}

	/* Get the file name string from the statement.
	*/
	if (fStat &= FParseTkt(tktString)) {
		strcpy(szFile, SzToken());
	}			

	fStat &= FParseTkt(tktEoln);


	return fStat;
}
#endif

#if OptPrsrLineStmt != 0
/* ------------------------------------------------------------ */
/***	PRSR::FParseLine
**
**	Synopsis:
**		fSuccess = FParseLine(iln)
**
**	Input:
**		iln		- reference to variable to receive parsed line number
**
**	Output:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not.
**
**	Description:
**		This routine will parse a 'line' statement from a
**		workspace file.
*/

bool
PRSR::FParseLine(int & iln)
	{
	bool	fStat;

	err = errPrsrOk;

	/* Eat any blank lines.
	*/
	while (FParseTkt(tktEoln)) {
		FNextLine();
	}

	/* Check for 'line' symbol
	*/
	if (!FParseTkt(tktSymbol)) {
		return fFalse;
	}

	if (strcmp(SzToken(), szKwdLine) != 0) {
		UngetToken();
		return fFalse;
	}
		
	/* Eat the '='
	*/
	if (!FParseTkt(tktOperator) || (OptToken() != optAssign)) {
		err = errPrsrUnexpectedToken;
		return fFalse;
	}

	/* Get the line number value.
	*/
	if (fStat = (FParseTkt(tktConstant) && (CstToken() == cstNumber))) {
		iln = ptkn->DwVal();
	}

	fStat &= FParseTkt(tktEoln);


	return fStat;
}
#endif

#if defined(DEAD)
/* ------------------------------------------------------------ */
/***	PRSR::FParseActive
**
**	Synopsis:
**		fSuccess = FParseActive(fActive)
**
**	Input:
**		fActive	- reference to variable to receive parsed value
**
**	Output:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not.
**
**	Description:
**		This routine will parse a 'active' statement from a
**		workspace file.
*/

bool
PRSR::FParseActive(bool & fActive)
	{
	bool	fStat;
	int		ival;

	err = errPrsrOk;

	/* Eat any blank lines.
	*/
	while (FParseTkt(tktEoln)) {
		FNextLine();
	}

	/* Check for 'active' symbol
	*/
	if (!FParseTkt(tktSymbol)) {
		return fFalse;
	}

	if (strcmp(SzToken(), szKwdActive) != 0) {
		UngetToken();
		return fFalse;
	}
		
	/* Eat the '='
	*/
	if (!FParseTkt(tktOperator) || (OptToken() != optAssign)) {
		err = errPrsrUnexpectedToken;
		return fFalse;
	}

	/* Get the statement value.
	*/
	if (fStat = (FParseTkt(tktConstant) && (CstToken() == cstNumber))) {
		ival = ptkn->DwVal();
		fActive = (ival != 0);
	}

	fStat &= FParseTkt(tktEoln);


	return fStat;
}

#endif

/* ------------------------------------------------------------ */
/***	ProcName
**
**	Synopsis:
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

