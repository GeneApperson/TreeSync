/************************************************************************/
/*                                                                      */
/*  lexr.cpp	--  Lexical Analyzer Object Implementation				*/
/*                                                                      */
/************************************************************************/
/*	Author: 	Gene Apperson											*/
/*	Copyright 2011, Gene Apperson										*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*	This module defines the implementation for a lexical analyzer		*/
/*	object class used to separate an input stream into lexical tokens.	*/
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*	03/05/2011(GeneA) created from earlier version						*/
/*                                                                      */
/************************************************************************/


/* ------------------------------------------------------------ */
/*					Include File Definitions					*/
/* ------------------------------------------------------------ */

#include	<windows.h>
#include	<stdio.h>
#include	<errno.h>

#include	"../inc/dtypes.h"
#include	"../inc/dutil.h"
#include	"../inc/dassert.h"

#include	"config.h"
#include	"appdefs.h"
#include	"apst.h"
#include	"lexr.h"

/* ------------------------------------------------------------ */
/*					Local Type Definitions						*/
/* ------------------------------------------------------------ */

/* The following flags are defined for mode and status flags:
**	flgLexrInit		- The LEXR has been initialized for use
**	flgLexrOwnLine	- The LEXR owns the line buffer memory and should free
**						it when it terminates.
**	flgLexrNoSkipSpace - The LEXR returns white space as a token
**	flgLexrStyleIntel	- Intel style assembler syntax conventions
**	flgLexrStyleMotorola - Motorola style asembler syntax conventions
**	flgLextStyleC		- C style syntax conventions
**	flgLexrStyleBasic	- BASIC style syntax conventions
**	flgLExrStyleNoDollar - The $ character is not a special token
**	flgLexrStyleNoCaseSensitive - symbol tokens are forced to upper case
**	flgLexrStyleDigitTokens - Digit characters are returned as a token.
**		This is used when parsing SVF files as this file format can have
**		very long strings of digit characters in constants (e.g. The SVF
**		format supports constants that are hundreds of thousands of bits
**		long.)
*/
const DWORD	flgLexrInit			= 0x80000000;
const DWORD	flgLexrOwnLine		= 0x40000000;
const DWORD	flgLexrOwnFile		= 0x20000000;

/* Bits 9-15 are use to select language syntax conventions.
*/
const DWORD	mskLexrStyleAll		= 0x0000FF00;
const DWORD mskLexrStyleDefined	= flgLexrStyleIntel
						| flgLexrStyleMotorola
						| flgLexrStyleC
						| flgLexrStyleBasic
						| flgLexrStyleNoDollar
						| flgLexrStyleNoCaseSensitive
#if OptLexrDigitTokens != 0
						| flgLexrStyleDigitTokens
#endif
							;	//end of statement


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
/***	LEXR::LEXR
**
**	Synopsis:
**		LEXR()
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
**		Default constructor.
*/

LEXR::LEXR()
	{
	
	flg.Reset();
	szFile = NULL;
	szLine = NULL;
	fhFile = NULL;
	cchBuffer = 0;
	ilnCur = 0;
	ichCur = -1;
	cchUnget = 0;
	ptknCur = NULL;
	ptknUnget = NULL;
	ptknFree = NULL;
#if OptLexrInclude != 0
	istmgCur = 0;
#endif

#if OptLexrKeywords != 0
	mpsmKwd.clear();
#endif

}

/* ------------------------------------------------------------ */
/***	LEXR::~LEXR
**
**	Synopsis:
**		~LEXR()
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
**		Default destructor.
*/

LEXR::~LEXR()
	{
	AssertMsg(szMsg, "LEXR not properly terminted before destruction");

#if defined(_DEBUG)
	if (flg.FTst(flgLexrOwnLine)) {
		Assert(szLine == NULL, szMsg);
	}
#endif

	Assert(szFile == NULL, szMsg);
	Assert(ptknCur == NULL, szMsg);
	Assert(ptknUnget == NULL, szMsg);
	Assert(ptknFree == NULL, szMsg);
}

/* ------------------------------------------------------------ */
/***	LEXR::FInit
**
**	Synopsis:
**		fSuccess = FInit(cchLine)
**
**	Input:
**		cchLine		- size of line buffer to use
**
**	Output:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Initialize the LEXR object for use.
*/

bool
LEXR::FInit(int cchLine)
	{

	AssertMsg(szMsg, "LEXR already initialized");
	Assert(ptknCur == NULL, szMsg);
	Assert(ptknUnget == NULL, szMsg);
	Assert(ptknFree == NULL, szMsg);
	Assert(szLine == NULL, szMsg);

	cchBuffer = 0;

	/* Allocate the source line buffer if we are supposed to own
	** it.
	*/
	if (cchLine > 0) {
		if ((szLine = new char[cchLine+1]) == NULL) {
			err = errOutOfMemory;
			return fFalse;
		}
		cchBuffer = cchLine+1;
		flg.Set(flgLexrOwnLine);
	}

	fhFile = NULL;

	ilnCur = -1;
	ichCur = -1;
	cchUnget = 0;
	cchLine = 0;
	
#if OptLexrInclude != 0
	istmgCur = 0;
#endif

#if OptLexrKeywords != 0
	mpsmKwd.clear();
#endif

	flg.Set(flgLexrInit);

	return fTrue;

}

/* ------------------------------------------------------------ */
/***	LEXR::Terminate
**
**	Synopsis:
**		void Terminate()
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
**		Deinitialize the LEXR object.
*/

void
LEXR::Terminate()
	{
	TKN *		ptkn;
		
#if OptLexrInclude != 0
	while (istmgCur > 0) {
		if ((fhFile = rgstmg[istmgCur].FhFile()) != NULL) {
			fclose(fhFile);
			fhFile = NULL;
		}
		if ((szFile = rgstmg[istmgCur].SzFile()) != NULL) {
			delete [] szFile;
			szFile = NULL;
		}
		istmgCur -= 1;
	}
#endif

	/* Free all of the TKN objects used by this LEXR
	*/
	StartScan();			// this puts all TKN's on the free list.
	while (ptknFree != NULL) {
		ptkn = ptknFree;
		ptknFree = ptkn->PtknNext();
		delete ptkn;
	}

	/* Release the line buffer if we allocated it.
	*/
	if (flg.FTst(flgLexrOwnLine)) {
		delete [] szLine;
		szLine = NULL;
	}

	if (szFile != NULL) {
		delete [] szFile;
		szFile = NULL;
	}

	if (fhFile != NULL) {
		fclose(fhFile);
		fhFile = NULL;
	}

#if OptLexrKeywords != 0
	mpsmKwd.clear();
#endif

	flg.Reset();

}

#if OptLexrKeywords != 0
/* ------------------------------------------------------------ */
/***	LEXR::AddKeyword
**
**	Parameters:
**		sz		- keyword string
**		id		- corresponding identifier
**
**	Returns:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Add the specified keyword to the mapping table.
*/

void
LEXR::AddKeyword(char * sz, int id)
	{
	
	mpsmKwd[sz] = id;
	
}

#endif

/* ------------------------------------------------------------ */
/***	LEXR::StartScan
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
**		Resets the LEXR back to the null state.
*/

void
LEXR::StartScan()
	{

	if (ptknCur != NULL) {
		FreePtkn(ptknCur);
		ptknCur = NULL;
	}

	while (ptknUnget != NULL) {
		NextToken();
		FreePtkn(ptknCur);
		ptknCur = NULL;
	}

	cchLine = 0;
	ilnCur = 0;
	ichCur = -1;
	cchUnget = 0;
	flg.Clr(flgLexrEof);

}

/* ------------------------------------------------------------ */
/***	LEXR::FSetLine
**
**	Synopsis:
**		fSuccess =  FSetLine(sz)
**
**	Input:
**		sz		- line to set as current line
**
**	Output:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if error.
**
**	Description:
**		This will set the specified line as the current line. It
**		will fail if the LEXR was initialized with its own line
**		buffer and the string is too large to fit.
*/

bool
LEXR::FSetLine(char * sz)
	{
	int		cch;

	cch = strlen(sz);

	if (flg.FTst(flgLexrOwnLine)) {
		if ((cch+1) > cchBuffer) {
			err = errLexrStringTooLong;
			return fFalse;
		}
		SzCopy(szLine, sz);
	}
	else {
		szLine = sz;
		cchBuffer = cch+1;
	}

	cchLine = cch;
	ichCur = 0;

	return fTrue;

}

/* ------------------------------------------------------------ */
/***	LEXR::FOpenFile
**
**	Input:
**		sz			- file name of file to open
**
**	Output:
**		Member data initialized
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		This will open the file whose name was previously set.
*/

bool
LEXR::FOpenFile(char * sz)
	{
	errno_t		errno;

	if (sz == NULL) {
		err = errLexrNoFile;
		return fFalse;
	}
	
#if OptLexrInclude != 0
	if ((istmgCur+1) >= cstmgLexrIncludeMax) {
		err = errLexrIncludeDepth;
		return fFalse;
	}

	rgstmg[istmgCur].SetFlgCur(flg);
	rgstmg[istmgCur].SetSzFile(szFile);
	rgstmg[istmgCur].SetFhFile(fhFile);
	rgstmg[istmgCur].SetIlnCur(ilnCur);
	istmgCur += 1;
#endif

	if ((szFile = new char [strlen(sz)+1]) == NULL) {
		err = errOutOfMemory;
		return fFalse;
	}

	SzCopy(szFile, sz);
	
	if ((fopen_s(&fhFile, szFile, "rt") != 0) || (fhFile == NULL)) {
		_get_errno(&errno);
		if (errno == ENOENT) {
			err = errFileNotFound;
		}
		else {
			err = errFileSystem;
		}
		return fFalse;
	}
	
	flg.Set(flgLexrOwnFile);
	flg.Clr(flgLexrEof);
	
	return fTrue;

}

/* ------------------------------------------------------------ */
/***	LEXR::CloseFile
**
**	Synopsis:
**		void CloseFile()
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
**		This function will close the input file.
*/

void
LEXR::CloseFile()
	{

	if (szFile != NULL) {
		delete [] szFile;
		szFile = NULL;
	}
		
	if (fhFile != NULL) {
		fclose(fhFile);
		fhFile = NULL;
	}
	
#if OptLexrInclude != 0
	if (istmgCur <= 0) {
		err = errLexrIncludeUnderflow;
		flg.Set(flgLexrEof);
		return;
	}
	
	istmgCur -= 1;
	fhFile = rgstmg[istmgCur].FhFile();
	szFile = rgstmg[istmgCur].SzFile();
	ilnCur = rgstmg[istmgCur].IlnCur();
	flg    = rgstmg[istmgCur].FlgCur();
	
	if (istmgCur == 0) {
		flg.Clr(flgLexrOwnFile);
		flg.Set(flgLexrEof);
	}
	if (istmgCur > 0) {
		flg.Clr(flgLexrEof);
	}
#else
	flg.Clr(flgLexrOwnFile);
#endif

}

/* ------------------------------------------------------------ */
/***	LEXR::SzFile
**
**	Input:
**		istmg	- include stack index
**
**	Output:
**		returns pointer to file name string
**
**	Errors:
**		returns pointer to string or NULL if error
**
**	Description:
**		This returns a pointer to the file name string for the
**		source file at the specified include stack depth.
*/
#if OptLexrInclude != 0

char *
LEXR::SzFile(int istmg)
	{

	if (istmg > istmgCur) {
		return NULL;
	}

	if (istmg == 0) {
		return szFile;
	}
	else {
		return rgstmg[istmg-1].SzFile();
	}
}

#endif

/* ------------------------------------------------------------ */
/***	LEXR::IlnCur
**
**	Input:
**		istmg	- include stack index
**
**	Output:
**		returns line number
**
**	Errors:
**		returns line number of -1 if error
**
**	Description:
**		This returns the line number of the current line in the
**		source file at the specified include stack depth
*/
#if OptLexrInclude != 0

int
LEXR::IlnCur(int istmg)
	{

	if (istmg > istmgCur) {
		return -1;
	}

	if (istmg == 0) {
		return ilnCur;
	}
	else {
		return rgstmg[istmg-1].IlnCur();
	}
}

#endif

/* ------------------------------------------------------------ */
/***	LEXR::FNextLine
**
**	Synopsis:
**		fOK = FNextLine()
**
**	Input:
**		none
**
**	Output:
**		Member data initialized
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		This will read the next line from an open input stream
**		file. If the end of the current file is reached, and we
**		are doing include files, it will check to see if this
**		is the end of a nested file. If so, it will close the
**		current file to pop back to the outer file and get the
**		next line from that file.
*/

bool
LEXR::FNextLine()
	{

	if (!flg.FTst(flgLexrOwnFile)) {
		err = errLexrNoFile;
		return fFalse;
	}

#if OptLexrInclude != 0
	do {
		if (FGetFileLine()) {
			ilnCur += 1;
			break;
		}
		else {
			if (!FIsEof()) {
				// error code already set
				return fFalse;
			}
			CloseFile();
		}	
	} while (istmgCur > 0);

#else
	if (!FGetFileLine()) {
		// error code already set
		return fFalse;
	}	
	ilnCur += 1;
#endif
		
	return fTrue;

}

/* ------------------------------------------------------------ */
/***	LEXR::FGetFileLine
**
**  Synopsis
**		fSuccess = FGetFileLine()
**
**  Input:
**		none
**
**  Output:
**		Object member data initialized
**
**  Errors:
**		Returns fTrue if successful, fFalse if not. Sets error
**		code in err.
**
**  Description:
**		This function will read the next line of the input
**		stream from the current source file.
*/

bool
LEXR::FGetFileLine()
    {
	char *	szOut;
    int     ch;

	Assert((cchBuffer > 0) && (szLine != NULL), "No line buffer allocated");

	ichCur = 0;

    err = errLexrOK;
    cchLine = 0;
	szOut = szLine;
    while (cchLine < cchBuffer-1) {
		ch = fgetc(fhFile);
		if (ch == EOF) {
			if (ferror(fhFile)) {
				err = errLexrReadError;
			}
			else if (cchLine == 0) {
				err = errLexrEndOfFile;
				flg.Set(flgLexrEof);
			}
			break;
		}

		if (ch == '\n') {
			break;
		}

		*szOut++ = ch;
		cchLine += 1;
    }

    *szOut = '\0';
    return (err == errLexrOK);

}

/* ------------------------------------------------------------ */
/***	LEXR::FSetScanStyle
**
**	Synopsis:
**		fSuccess = FSetScanStyle(flgStyle)
**
**	Input:
**		flgStyle	- scanning style to set
**
**	Output:
**		none
**
**	Errors:
**		Returns fFalse if invalid style specified.
**
**	Description:
**
*/

bool
LEXR::FSetScanStyle(DWORD flgStyle)
	{

	if ((flgStyle & ~mskLexrStyleDefined) != 0) {
		/* Requested something outside of the range of defined
		** lexical styles.
		*/
		err = errLexrInvalidStyle;
		return fFalse;
	}

	flg.Mask(~mskLexrStyleAll);
	flg.Set(flgStyle);
	
	return fTrue;

}

/* ------------------------------------------------------------ */
/***	LEXR::NextToken
**
**	Synopsis:
**		void NextToken()
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
**		Scans the next token from the input stream.
*/

void
LEXR::NextToken()
	{

	/* If there is a current token, release it.
	*/
	if (ptknCur != NULL) {
		FreePtkn(ptknCur);
		ptknCur = NULL;
	}

	/* Find the token to return.
	*/
	if (ptknUnget != NULL) {
		/* Return the first token from the unget queue.
		*/
		ptknCur = ptknUnget;
		ptknUnget = ptknCur->PtknNext();
		ptknCur->SetPtknNext(NULL);
		return;
	}

	ptknCur = PtknGet();
	ptknCur->Reset();
	Assert(ptknCur != NULL, "Unable to get a new TKN");
	//M00BUG - no error handling here

	/* There is no token pushed back, we will need to scan
	** one from the input stream. Make sure that there is
	** in fact an input stream to scan from.
	*/
	if (ichCur < 0) {
		if (flg.FTst(flgLexrOwnFile)) {
			/* We are reading from a file, but the first line
			** hasn't been read in yet.
			*/
			if (!FNextLine()) {
				// error code already set
				return;
			}
		}
		else {
			ptknCur->SetTktTkn(tktEoln);
			return;
		}
	}

	/* Scan the next token from the input stream.
	*/
	ScanToken(ptknCur);
	err = ptknCur->Err();
	
	
	while ((ptknCur->TktToken() == tktEoln) && flg.FTst(flgLexrNoEoln)) {
		/* We are at the end of a line and we aren't supposed to return
		** tktEoln tokens, so read the new line from the file and
		** scan the first token from it.
		*/
		if (!flg.FTst(flgLexrOwnFile)) {
			/* The LEXR doesn't have a file to read from, so we
			** have to return this token.
			*/
			return;
		}

		if (!FNextLine()) {
			if (err == errLexrEndOfFile) {
				ptknCur->SetTktTkn(tktEof);
			}
			else {
				ptknCur->SetTktTkn(tktInvalid);
			}
			return;
		}

		ScanToken(ptknCur);
	}

}

/* ------------------------------------------------------------ */
/***	LEXR::UngetToken
**
**	Synopsis:
**		void UngetToken()
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
**		Places the current token back into the input stream.
*/

void
LEXR::UngetToken()
	{

	if (ptknCur != NULL) {
		ptknCur->SetPtknNext(ptknUnget);
		ptknUnget = ptknCur;
	}

	ptknCur = NULL;		

}

/* ------------------------------------------------------------ */
/***	LEXR::ChNext
**
**	Synopsis:
**		ch = ChNext()
**
**	Input:
**		none
**
**	Output:
**		Returns next character in input stream
**
**	Errors:
**		none
**
**	Description:
**		Returns the next character in the input stream.
*/

char 
LEXR::ChNext()
	{

	if (cchUnget > 0) {
		return rgchUnget[--cchUnget];
	}

	if (ichCur < cchLine) {
		return szLine[ichCur++];
	}

	return '\0';

}

/* ------------------------------------------------------------ */
/***	LEXR::ChPutChGetNext
**
**	Synopsis:
**		ch = ChPutChGetNext(ch)
**
**	Input:
**		ch		- character to put
**
**	Output:
**		Returns next character from input stream
**
**	Errors:
**		none
**
**	Description:
**		This will write the specified character to the current
**		token string and then fetch the next character from the
**		input stream.
*/

char
LEXR::ChPutChGetNext(char ch)
	{

	ptknCur->PutCh(ch);
	return ChNext();

}

/* ------------------------------------------------------------ */
/***	LEXR::UngetCh
**
**	Synopsis:
**		void UngetCh(ch)
**
**	Input:
**		ch		- character to push back
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Pushes the given character back onto the input stream.
**		If the unget buffer is full, the character is discarded
*/

void
LEXR::UngetCh(char ch)
	{

	if (cchUnget < cchUngetMax) {
		rgchUnget[cchUnget++] = ch;
	}

}

/* ------------------------------------------------------------ */
/***	LEXR::ChEatUntil
**
**	Synopsis:
**		ch = ChEatUntil(ch)
**
**	Input:
**		none
**
**	Output:
**		Returns the character	
**
**	Errors:
**		none
**
**	Description:
**		This routine will consume characters from the current
**		line until it hits the requested character or the end
**		of the line.
*/

char
LEXR::ChEatUntil(char chF)
	{
	char	ch;

	while (fTrue) {
		ch = ChNext();
		if ((ch == chF) || (ch = '\0')) {
			break;
		}
	}

	return ch;

}

/* ------------------------------------------------------------ */
/***	LEXR::PtknGet
**
**	Synopsis:
**		ptkn = PtknGet()
**
**	Input:
**		none
**
**	Output:
**		Pointer to a token to use
**
**	Errors:
**		Returns NULL if unable to get a token
**
**	Description:
**		This functions returns a pointer to a free TKN object
**		for use.
*/

TKN *
LEXR::PtknGet()
	{
	TKN *	ptkn;

	if (ptknFree != NULL) {
		ptkn = ptknFree;
		ptknFree = ptkn->PtknNext();
	}
	else {
		ptkn = new TKN;
	}

	ptkn->SetPtknNext(NULL);
	return ptkn;

}

/* ------------------------------------------------------------ */
/***	LEXR::FreePtkn
**
**	Synopsis:
**		void FreePtkn(ptkn)
**
**	Input:
**		ptkn	- pointer to token to release
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This frees a TKN.
*/

void
LEXR::FreePtkn(TKN * ptkn)
	{

	ptkn->SetPtknNext(ptknFree);
	ptknFree = ptkn;

}

/* ------------------------------------------------------------ */
/***	LEXR::ScanToken
**
**	Synopsis:
**		void ScanToken()
**
**	Input:
**		none
**
**	Output:
**		Member data initialized with result of the scan
**
**	Errors:
**		Error code set in the token
**
**	Description:
**		This routine will scan the next token from the intput
**		stream and initialize the specified TKN with the result.
*/

void
LEXR::ScanToken(TKN * ptkn)
    {
    char	    ch;
	char		chT;
    BOOL        fSpace;

    /* Skip over any white space at the beginning of the next token.
    */
    fSpace = fFalse;
    while (fTrue) {
		ch = ChNext();

		/* Check for end of line.
		*/
		if ((ch == '\n') || (ch == '\0')) {
			break;
		}

		/* If doing Intel or Motorola style assembler, check for comment.
		*/
		if (flg.FTst(flgLexrStyleIntel|flgLexrStyleMotorola) && (ch == ';')) {
			ch = ChEatUntil('\0');
			break;
		}

        /* The Motorola Freeware assemblers use the convention
        ** of an '*' in the first column on a line to make the
        ** entire line a comment.
        */
        if (flg.FTst(flgLexrStyleMotorola) &&
			((ch == '*') && (ichCur == 0))) {
			ch = ChEatUntil('\0');
			break;
		}

		/* The C/C++ convention for a single line comment is a
		** double slash.
		*/
		if (flg.FTst(flgLexrStyleC) && (ch == '/')) {
			chT = ChNext();
			if (chT == '/') {
				ch = ChEatUntil('\0');
			}
			else {
				UngetCh(chT);
			}
			break;
		}

		/* Any non white space character is the start of the token.
		*/
		if (!FIsSpace(ch)) {
			break;
		}

        fSpace = fTrue;
    }

	/* We have skipped any white space in front of the next token
	** and ch contains the first character of the token. Decide what
	** kind of token it is and proceed accordingly.
	*/
    if (flg.FTst(flgLexrNoSkipSpace) && fSpace) {
		/* If we return space as a token, indicate that we have
		** seen some white space.
		*/
        ptkn->SetTktTkn(tktSpace);
    }

    else if ((ch == '\n') || (ch == '\0')) {
		ptkn->SetTktTkn(tktEoln);
    }

	else if (FScanOperator(ch)) {
		ptkn->SetTktTkn(tktOperator);
	}

#if OptLexrDigitTokens != 0
	else if (FScanDigit(ch)) {
		ptkn->SetTktTkn(tktDigit);
	}
#endif

	else if (FScanConstant(ch)) {
		ptkn->SetTktTkn(tktConstant);
	}

    else if (FScanSymbol(ch)) {
		ptkn->SetTktTkn(tktSymbol);
#if OptLexrKeywords != 0
		if (mpsmKwd.find(ptkn->SzToken()) != mpsmKwd.end()) {
			ptkn->SetTktTkn(tktKeyword);
			ptkn->SetId(mpsmKwd[ptkn->SzToken()]);
		}
#endif
	}

    else if (ch == '"') {
		ScanString(ch);
	}

	else if (ch == ':') {
		ptkn->SetTktTkn(tktColon);
	}

	else if (ch == ';') {
		ptkn->SetTktTkn(tktSemicolon);
	}

	else if (ch == ',') {
		ptkn->SetTktTkn(tktComma);
	}

	else if ((ch == '$') && !flg.FTst(flgLexrStyleNoDollar)) {
		ptkn->SetTktTkn(tktDollar);
	}

	else if (ch == '#') {
		ptkn->SetTktTkn(tktHash);
	}

	else if (ch == '(') {
		ptkn->SetTktTkn(tktLPara);
	}

	else if (ch == ')') {
		ptkn->SetTktTkn(tktRPara);
	}

	else if (ch == '[') {
		ptkn->SetTktTkn(tktLBracket);
	}

	else if (ch == ']') {
		ptkn->SetTktTkn(tktRBracket);
	}

	else if (ch == '{') {
		ptkn->SetTktTkn(tktLBrace);
	}

	else if (ch == '}') {
		ptkn->SetTktTkn(tktRBrace);
	}

    else {
		/* This character isn't a valid first character for any token
		** so we have an error.
		*/
		ptkn->SetErr(errTknInvalidChar);
    }

	/* If we got any errors, change the token type to invalid.
	*/
	if (ptkn->Err() != errLexrOK) {
		ptkn->SetTktTkn(tktInvalid);
	}

}

/* ------------------------------------------------------------ */
/***	LEXR::FScanSymbol
**
**	Synopsis:
**		fIsSym = FScanSymbol(ch)
**
**	Input:
**		ch		- first character of token
**
**	Output:
**		Returns fTrue if a symbol was scanned from input stream,
**		fFalse if not.
**
**	Errors:
**		none
**
**	Description:
**		This routine will attempt to scan a symbol from the input
**		stream. If successful, it will eat the input characters
***		and initialize the token buffer with the symbol. It not
**		it returns the input stream to the entry state and
**		returns fFalse.
*/

bool
LEXR::FScanSymbol(char ch)
	{
	char	chT;

	if (flg.FTst(flgLexrStyleNoCaseSensitive)) {
		ch = ChMakeUpper(ch);
	}

	if (!FIsSymChar(ch)) {
		return fFalse;
	}

	ptknCur->PutCh(ch);
	while (fTrue) {
		chT = ChNext();
		if (flg.FTst(flgLexrStyleNoCaseSensitive)) {
			ch = ChMakeUpper(ch);
		}
		if (!FIsSymChar(chT) && !FIsDigit(chT)) {
			UngetCh(chT);
			break;
		}
		ptknCur->PutCh(chT);
	}

	return fTrue;

}

/* ------------------------------------------------------------ */
/***	LEXR::ScanString
**
**	Synopsis:
**		void ScanString(ch)
**
**	Input:
**		ch		- initial character (should be the ")
**
**	Output:
**		ptknCur initialized with token information
**
**	Errors:
**		Sets error status in the ptknCur
**
**	Description:
**		This will scan a quoted string from the source stream.
*/

void
LEXR::ScanString(char ch)
	{
	char	chT;

	Assert(ch == '"', "Quote expected at beginning of string");

	while (fTrue) {
		chT = ChNext();
		if (chT == '"') {
			break;
		}
		if ((chT == '\n') || (chT == '\0')) {
			ptknCur->SetErr(errTknMissingEndQuote);
			return;
		}
		ptknCur->PutCh(chT);
	}

	ptknCur->SetTktTkn(tktString);

}

/* ------------------------------------------------------------ */
/***	LEXR::FScanOperator
**
**	Synopsis:
**		f = FScanOperator(ch)
**
**	Input:
**		ptkn		- pointer to the TKN
**		ch			- first char of potential operator
**
**	Output:
**		Return fTrue if an operator was scanned from the input
**		stream, else fFalse.
**
**	Errors:
**		Error code is set in TKN if any error is detected
**
**	Description:
**		This routine will scan an operator from the input stream
**		and initialize ptknCur if an opeator is found.
*/

bool
LEXR::FScanOperator(char ch)
	{

    char	chT;
	char	chN;
	int		opt;

	ptknCur->PutCh(ch);
    chT = ChNext();

	if (ch == '+') {
		/* Either '+', '+=', or '++'
		*/
		if (flg.FTst(flgLexrStyleC)) {
			if (chT == '=') {
				opt = optPlusEqual;
				chT = ChPutChGetNext(chT);
			}
			else if (chT == '+') {
				opt = optInc;
				chT = ChPutChGetNext(chT);
			}
		}	
		else {
			opt = optPlus;
		}
	}

	else if (ch == '-') {
		/* Either a '-', '-=', '--', or '->'
		*/
		if (flg.FTst(flgLexrStyleC)) {
			if (chT == '=') {
				opt = optMinusEqual;
				chT = ChPutChGetNext(chT);
			}
			else if (chT == '-') {
				opt = optDec;
				chT = ChPutChGetNext(chT);
			}
			else if (chT == '>') {
				opt = optArrow;
				chT = ChPutChGetNext(chT);
			}
			opt = optMinus;
		}
		else {
			opt = optMinus;
		}
	}

	else if (ch == '*') {
		/* Either a '*' or '*='. Depending on the language, '*' is
		** either always a multiply, or either a multiply or a pointer
		** deref depending on how it used. The higher level parser that
		** understands the usage context has to determine which it is.
		*/
		if (flg.FTst(flgLexrStyleC) && (chT == '=')) {
			opt = optMulEqual;
			chT = ChPutChGetNext(chT);
		}
		else {
			opt = optAsterisk;
		}
	}

	else if (ch == '/') {
		/* Either a '/' or a '/='
		*/
		if (flg.FTst(flgLexrStyleC) && (chT == '=')) {
			opt = optDivEqual;
			chT = ChPutChGetNext(chT);
		}
		else {
			opt = optSlash;
		}
	}

	else if (flg.FTst(flgLexrStyleC) && (ch == '^')) {
		/* Either '^' or '^='.
		*/
		if (chT == '=') {
			opt = optXorEqual;
			chT = ChPutChGetNext(chT);
		}
		else {
			opt = optBitXor;
		}
	}

	else if (flg.FTst(flgLexrStyleC) && (ch == '~')) {
		opt = optBitNot;
	}

	else if (ch == '!') {
		/* Either a '!' or a '!='
		*/
		if (chT == '=') {
			opt = optRelNE;
			chT = ChPutChGetNext(chT);
		}
		else {
			opt = optLogNot;
		}
	}

	else if (ch == '&') {
		/* Either a '&' or '&&', or '&='.
		*/
		if (flg.FTst(flgLexrStyleC) && (chT = '=')) {
			opt = optAndEqual;
			chT = ChPutChGetNext(chT);
		}
		else if (chT == '&') {
			opt = optLogAnd;
			chT = ChPutChGetNext(chT);
		}
		else {
			opt = optAmpersand;
		}
	}

	else if (ch == '|') {
		/* Either a '|', a '||' or a '|='
		*/
		if (flg.FTst(flgLexrStyleC) && (chT == '=')) {
			opt = optOrEqual;
			chT = ChPutChGetNext(chT);
		}
		else if (chT == '|') {
			opt = optLogOr;
			chT = ChPutChGetNext(chT);
		}
		else {
			opt = optBar;
		}
	}

	else if (ch == '.') {
		opt = optDot;
	}

	else if (ch == '%') {
		/* Either a '%' or a '%='.
		*/
		if (flg.FTst(flgLexrStyleC) && (chT == '=')) {
			opt = optModEqual;
			chT = ChPutChGetNext(chT);
		}
		else {
			opt = optMod;
		}
	}

	else if (flg.FTst(flgLexrStyleC) && (ch == '?')) {
		opt = optQuestion;
	}

	else if (ch == '=') {
		/* Either a '=' or a '=='.
		*/
		if (chT == '=') {
			opt = optRelEQ;
			chT = ChPutChGetNext(chT);
		}
		else {
			opt = optAssign;
		}
	}

	else if (ch == '<') {
		/* Either a '<', '<=', '<<', or '<<='
		*/
		if (chT == '=') {
			opt = optRelLE;
			chT = ChPutChGetNext(chT);
		}
		else if (chT == '<') {
			/* It is either a '<<' or a '<<='
			*/
			opt = optLsh;
			chT = ChPutChGetNext(chT);
			if (flg.FTst(flgLexrStyleC)) {
				/* Only check for '<<=' if scanning for C style operators
				*/
				chN = ChNext();
				if (chN == '=') {
					opt = optLshEqual;
					chT = ChPutChGetNext(chN);
				}
				else {
					UngetCh(chN);
				}
			}
		}
		else {
			opt = optRelLT;
		}
	}

	else if (ch == '>') {
		/* Either a '>', '>=', '>>', or '>>='
		*/
		if (chT == '=') {
			opt = optRelGE;
			chT = ChPutChGetNext(chT);
		}
		else if (chT == '>') {
			/* It is either a '>>' or a '>>='
			*/
			opt = optRsh;
			chT = ChPutChGetNext(chT);
			if (flg.FTst(flgLexrStyleC)) {
				/* Only check for '>>=' if scanning for C style operators
				*/
				chN = ChNext();
				if (chN == '=') {
					opt = optRshEqual;
					chT = ChPutChGetNext(chN);
				}
				else {
					UngetCh(chN);
				}
			}
		}
		else {
			opt = optRelGT;
		}
	}

	else {
		/* Not a beginning character for an operator.
		*/
		UngetCh(chT);
		ptknCur->DelCh();
		return fFalse;
	}

	/* Put back whatever character terminated the scan and then
	** set the token type and operator type.
	*/
	UngetCh(chT);
	ptknCur->SetTktTkn(tktOperator);
	ptknCur->SetOpt(opt);
	return fTrue;

}

#if OptLexrDigitTokens != 0
/* ------------------------------------------------------------ */
/***	LEXR::FScanDigit
**
**	Synopsis:
**		fIsDigit = FScanDigit(ch)
**
**	Input:
**		ch		- current character in scan
**
**	Output:
**		Returns fTrue if the scan style requires returning
**		digit characters as a token and ch is a digit character
**
**	Errors:
**		none
**
**	Description:
**		Check if scanning digit characters as tokens and return
**		true if we are and this is a digit character.
*/


bool
LEXR::FScanDigit(char ch)
	{

	if (FTstFlag(flgLexrStyleDigitTokens)) {

		if (FIsDigit(ch)) {
			ptknCur->PutCh(ch);
			ptknCur->FSetConstNumber(10);
			return fTrue;
		}

		if (FIsHexDigit(ch)) {
			ptknCur->PutCh(ch);
			ptknCur->FSetConstNumber(16);
			return fTrue;
		}
	}

	return fFalse;

}
#endif

/* ------------------------------------------------------------ */
/***	LEXR::FScanConstant
**
**	Synopsis:
**		fIsConst = FScanConstant(ch)
**
**	Input:
**		chF		- first character of potential constant
**
**	Output:
**		Returns fTrue if a constant was found, fFalse if not.
**
**	Errors:
**		none
**
**	Description:
**		This routine will scan a constant from the input stream.
*/

bool
LEXR::FScanConstant(char chF)
	{
	char		ch;
	char		chLast;
	int			nvRadix;
	DWORD		dwT;

	ch = chF;

	if (FIsDigit(ch)) {
		/* It begins with a digit, therefore it is a numeric constant.
		** get the rest of the number.
		*/
		ptknCur->PutCh(ch);		// put first character into token buffer

		if (flg.FTst(flgLexrStyleC)) {
			/* Parse a C style numeric constant.
			*/
			if (ch == '0') {
				/* This is either an octal or hex constant.
				*/
				if (((ch = ChNext()) == 'x') || (ch == 'X')) {
					/* It is a hex constant.
					*/
					while (FIsHexDigit(ch = ChNext())) {
						ptknCur->PutCh(ch);
					}
					UngetCh(ch);
					ptknCur->FSetConstNumber(16);
					return fTrue;
				}

				else {
					/* It is an octal constant.
					*/
					UngetCh(ch);
					while (FIsOctDigit(ch = ChNext())) {
						ptknCur->PutCh(ch);
					}
					UngetCh(ch);
					ptknCur->FSetConstNumber(8);
					return fTrue;
				}
			}

			else {
				/* It is a decimal constant.
				*/
				//M00BUG - This needs to handle all possible C numeric constant
				//		forms. Integer, Floating Point, Scientific Notation,
				//		etc.
				while(FIsDigit(ch = ChNext())) {
					ptknCur->PutCh(ch);
				}
				UngetCh(ch);
				ptknCur->FSetConstNumber(10);
				return fTrue;
			}
		}

		else if (flg.FTst(flgLexrStyleIntel)) {
			/* Parse Intel style numeric constant
			*/
			while (FIsHexDigit(ch = ChNext())) {
				ptknCur->PutCh(ch);
				chLast = ch;
			}

			if ((chLast == 'b') || (chLast == 'B')) {
				/* If the last character transferred to the buffer
				** was a B, this is supposed to be a binary number, and
				** the B is a radix specifier.  If the B was a hex digit,
				** the number would have ended in H to be a valid hex
				** number.
				*/
				if ((ch != 'h') && (ch != 'H')) {
					ptknCur->DelCh();
					ch = chLast;
				}
			}

			/* Determine the radix of the constant from the final
			** character.
			*/
			if ((ch == 'h') || (ch == 'H')) {
				nvRadix = 16;
			}
			else if ((ch == 'q') || (ch == 'Q') ||
					 (ch == 'o') || (ch == 'O')) {
				nvRadix = 8;
			}
			else if ((ch == 'b') || (ch == 'B')) {
				nvRadix = 2;
			}
			else {
				nvRadix = 10;
				UngetCh(ch);
			}

			ptknCur->FSetConstNumber(nvRadix);
			return fTrue;
		}

		if (flg.FTst(flgLexrStyleMotorola)) {
			/* Parse Motorola assembly language style decimal constant
			*/
			while (FIsDigit(ch = ChNext())) {
				ptknCur->PutCh(ch);
			}
			UngetCh(ch);

			ptknCur->FSetConstNumber(10);
			return fTrue;
		}

    }	
	
	else if (flg.FTst(flgLexrStyleMotorola) && (ch == '$')) {		
		/* Motorola style source syntax uses a leading $ character to
		** indicate a hex numeric constant.
		*/
		while (FIsHexDigit(ch = ChNext())) {
			ptknCur->PutCh(ch);
		}
		UngetCh(ch);

		ptknCur->FSetConstNumber(16);
		return fTrue;

	}
	else if (flg.FTst(flgLexrStyleMotorola) && (ch == '@')) {		
		/* Motorola style source syntax uses an @ character to
		** indicate an octal numeric constant.
		*/
		while (FIsOctDigit(ch = ChNext())) {
			ptknCur->PutCh(ch);
		}
		UngetCh(ch);

		ptknCur->FSetConstNumber(8);
		return fTrue;
	}
	else if (flg.FTst(flgLexrStyleMotorola) && (ch == '%')) {		
		/* Motorola style source syntax uses a % character to
		** indicate a binary contstant.
		*/
		while (FIsBinDigit(ch = ChNext())) {
			ptknCur->PutCh(ch);
		}
		UngetCh(ch);

		ptknCur->FSetConstNumber(2);
		return fTrue;
	}

    else if (ch == '\'') {
		/* A single quote is the lead in for a character constant.
		*/
		if (flg.FTst(flgLexrStyleC)) {
			ch = ChNext();
			if (ch == '\\') {
				/* It is a character constant escape sequence.
				*/
				FScanCharEscape();
			}
			else {
				dwT = ch;
				ch = ChNext();
				if (ch == '\'') {
					ptknCur->SetConstChar(dwT);
				}
				else {
					ptknCur->SetErr(errTknCharConstant);
				}
			}
			return fTrue;
		}

		else if (flg.FTst(flgLexrStyleIntel)) {
			/* A single quote is used to delimit a character value to
			** be treated as a single or double byte quantity.  Get the
			** following one or two bytes and eat the ending quote.
			*/
			ch = ChNext();
			dwT = ch & 0xFF;
			ch = ChNext();
			if ((ch != '\'') && (ch != '\n') && (ch != '\0')) {
				dwT = (dwT << 8) + (ch & 0xFF);
				ch = ChNext();
			}

			if (ch == '\'') {
				ptknCur->SetConstChar(dwT);
			}
			else {
				ptknCur->SetErr(errTknCharConstant);
			}
			return fTrue;
		}

		else if (flg.FTst(flgLexrStyleMotorola)) {
			/* Motorola style syntax uses a single quote preceeding a
			** character to generate an 8 bit ASCII character constant.
			** In strict Motorola syntax, there isn't a trailing quote.
			** We allow the trailing quote to either be present or not.
			*/
			ch = ChNext();
			dwT = ch & 0xFF;
			if ((ch = ChNext()) != '\'') {
				UngetCh(ch);
			}
    		ptknCur->SetConstChar(dwT);
			return fTrue;
		}
		
    }

	return fFalse;

}

/* ------------------------------------------------------------ */
/***	LEXR::FIsSymChar
**
**	Synopsis:
**		f = FIsSymChar(ch)
**
**	Input:
**		ch		- character to test
**
**	Output:
**		Returns fTrue if ch is a valid character in a symbol
**
**	Errors:
**		none
**
**	Description:
**		Checks if ch is a valid symbol character.
*/

bool
LEXR::FIsSymChar(char ch)
	{

	if (FIsAlpha(ch) || (ch == '_'))
		return fTrue;

	if (flg.FTst(flgLexrStyleNoDollar) && (ch == '$')) {
		return fTrue;
	}

	if (flg.FTst(flgLexrStyleIntel) && (ch == '?')) {
		return fTrue;
	}

	return fFalse;

}

/* ------------------------------------------------------------ */
/***	LEXR::FScanCharEscape
**
**	Synopsis:
**		fSuccess = FScanCharEscape()
**
**	Input:
**		characters from the input stream
**
**	Output:
**		values set in current token
**
**	Errors:
**		error status set in current token
**
**	Description:
**		This routine will scan a C style character escape sequence
**		from the input stream and initialize the current token
**		with the result.
*/

bool
LEXR::FScanCharEscape()
	{
	DWORD	dwT;
	char	ch;
	char *	pchEnd;

	ch = ChNext();
	if (ch == 'x') {
		/* Lead in for hex character constant sequence.
		*/
		while (FIsHexDigit(ch = ChNext())) {
			ptknCur->PutCh(ch);
		}

		if (ch != '\'') {
			goto lErrorExit;
		}

		dwT = strtol(ptknCur->SzToken(), &pchEnd, 16);
	}

	else if (FIsOctDigit(ch)) {
		/* Lead in for octal character constant sequence.
		*/
		ptknCur->PutCh(ch);
		while (FIsOctDigit(ch = ChNext())) {
			ptknCur->PutCh(ch);
		}

		if (ch != '\'') {
			goto lErrorExit;
		}

		dwT = strtol(ptknCur->SzToken(), &pchEnd, 8);
	}
	
	else {
		ptknCur->PutCh(ch);

		switch (ch) {
			case 'n':
				dwT = '\n';
				break;

			case 't':
				dwT = '\t';
				break;

			case 'v':
				dwT = '\v';
				break;

			case 'b':
				dwT = '\b';
				break;

			case 'r':
				dwT = '\r';
				break;

			case 'f':
				dwT = '\f';
				break;

			case 'a':
				dwT = '\a';
				break;

			case '\\':
				dwT = '\\';
				break;

			case '?':
				dwT = '\?';
				break;

			case '\'':
				dwT = '\'';
				break;

			case '\"':
				dwT = '\"';
				break;

			default:
				goto lErrorExit;
		}

		ch = ChNext();
		if (ch != '\'') {
			goto lErrorExit;
		}

	}

	ptknCur->SetConstChar(dwT);
	return fTrue;

lErrorExit:
	ptknCur->SetErr(errTknCharConstant);
	UngetCh(ch);
	return fFalse;


}

/* ------------------------------------------------------------ */
/***	LEXR::ChMakeUpper
**
**	Synopsis:
**		chUpper = ChMakeUpper(ch)
**
**	Input:
**		ch		- char to convert
**
**	Output:
**		Returns upper case character
**
**	Errors:
**		none
**
**	Description:
**		Returns the upper case version of the character if it
**		is a lower case alpha, else the original character.
*/

char
LEXR::ChMakeUpper(char ch)
	{

	if ((ch >= 'a') && (ch <= 'z')) {
		ch -= 'a' - 'A';
	}

	return ch;

}

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
/*					TKN Object Implementation					*/
/* ------------------------------------------------------------ */
/***	TKN::TKN
**
**	Synopsis:
**		TKN()
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

TKN::TKN()
	{
	ptknNext = NULL;
}

/* ------------------------------------------------------------ */
/***	TKN::~TKN
**
**	Synopsis:
**		~TKN()
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

TKN::~TKN()
	{

}

/* ------------------------------------------------------------ */
/***	TKN::Reset
**
**	Synopsis:
**		void Reset()
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
**		Reinitializes the TKN for use.
*/

void
TKN::Reset()
	{
	err = errLexrOK;
	ich = 0;
	szTkn[ich] = '\0';
}

/* ------------------------------------------------------------ */
/***	TKN::PutCh
**
**	Synopsis:
**		void PutCh(ch)
**
**	Input:
**		ch		- character to put into token string
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This routine will place the specified character into the
**		token string buffer. If the buffer is full, the character
**		is discarded.
*/

void
TKN::PutCh(char ch)
	{

	if (ich < cchTokenMax) {
		szTkn[ich++] = ch;
		szTkn[ich] = '\0';
	}
	else {
		err = errTknStringTooLong;
	}

}

/* ------------------------------------------------------------ */
/***	TKN::DelCh
**
**	Synopsis:
**		void DelCh()
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
**		This routine will delete the last character in the
**		token string buffer.
*/

void
TKN::DelCh()
	{

	if (ich > 0) {
		ich -= 1;
		szTkn[ich] = '\0';
	}

}

/* ------------------------------------------------------------ */
/***	TKN::FSetConstNumber
**
**	Synopsis:
**		f = FSetConstNumber(ptkn)
**
**	Input:
**		none
**
**	Output:
**		Initializes member data
**
**	Errors:
**		Returns fTrue if the number string was converted successfully,
**		else fFalse.
**
**	Description:
**		This routine will convert the string in the token string
**		buffer into a numeric constant and set the internal state
**		appropriately.
*/

bool
TKN::FSetConstNumber(int nv)
	{
	char *		pchEnd;

	SetDwVal(strtol(szTkn, &pchEnd, nv));

	if (*pchEnd == '\0') {
		tktTkn =tktConstant;
		nvRadix = nv;
		cstVal = cstNumber;
		return fTrue;
	}
	else {
		tktTkn = tktInvalid;
		err = errTknConstant;
		return fFalse;
	}

}

/* ------------------------------------------------------------ */
/*				STMG Object Class Implementation				*/
/* ------------------------------------------------------------ */
#if OptLexrInclude != 0
/***	STMG::STMG
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
**		Default constructor
*/

STMG::STMG()
	{
	
	fhFile = NULL;
	szFile = NULL;
	ilnCur = 0;
	
}

/* ------------------------------------------------------------ */
/***	STMG::~STMG
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
**		Default destructor.
*/

STMG::~STMG()
	{
	
}

/* ------------------------------------------------------------ */
#endif	//OptLexrInclude
/* ------------------------------------------------------------ */
/***	ProcName
**
**	Parameters:
**
**	Returns:
**
**	Errors:
**
**	Description:
**
*/

/* ------------------------------------------------------------ */

/************************************************************************/

