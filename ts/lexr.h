/************************************************************************/
/*                                                                      */
/*		lexr.h  --	Lexical Analysis Object Declarations				*/
/*                                                                      */
/************************************************************************/
/*	Author:		Gene Apperson											*/
/*	Copyright 2003, Digilent Inc.										*/
/************************************************************************/
/*  File Description:                                                   */
/*                                                                      */
/*	The following compile time configuration options are supported:		*/
/*	OptLexrKeywords		The lexr supports keyword symbols. This option	*/
/*						uses a keyword symbol table and looks up symbol	*/
/*						in the keyword table to see if they are a		*/
/*						keyword in the language being scanned.			*/
/*	OptLexrInclude		The lexr supports nested files. Multiple calls	*/
/*						file open can be called without intervening		*/
/*						calls to close. A stack of opened files is		*/
/*						maintained. This requires the stack depth to	*/
/*						be defined via the constant cfhLexrIncludeMax	*/
/*	OptLexrDigitTokens	Each numeric character is returned as a digit	*/
/*						token rather than being converted into a		*/
/*						numeric constant. This is used by languages,	*/
/*						such as SVF that have extremely long digit		*/
/*						sequences.						
/*                                                                      */
/************************************************************************/
/*  Revision History:                                                   */
/*                                                                      */
/*	08/01/2003(GeneA): created											*/
/*	01/22/2003(JPederson):  modified to add new Lex convention style	*/
/*							(LexrStyleNoDollar)							*/
/*	02/14/2004(GeneA): added style flgLexrStyleDigitTokens				*/
/*                                                                      */
/************************************************************************/

#if !defined(LEXR_INC)
#define	LEXR_INC

/* ------------------------------------------------------------ */
/*					Include File Definitions					*/
/* ------------------------------------------------------------ */

#if OptLexrKeywords != 0
#include	<map>
#include	<string>

using namespace std;
#endif

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */

#define	cchTokenMax		128	// maximum length of a token string
#define	cchUngetMax		32	//maximum number of chars that can be pushed back

/* ------------------------------------------------------------ */
/*					Type Declarations							*/
/* ------------------------------------------------------------ */

/* Operating modes.
*/
const DWORD flgLexrNoEoln		= 0x00000001;
const DWORD	flgLexrNoSkipSpace	= 0x00000002;
const DWORD	flgLexrEof			= 0x10000000;

/* Lexical convention sytles.
*/
const DWORD flgLexrStyleIntel			= 0x00000100;
const DWORD	flgLexrStyleMotorola		= 0x00000200;
const DWORD	flgLexrStyleC				= 0x00000400;
const DWORD	flgLexrStyleBasic			= 0x00000800;
const DWORD	flgLexrStyleNoDollar		= 0x00001000;
const DWORD	flgLexrStyleNoCaseSensitive	= 0x00002000;
const DWORD	flgLexrStyleDigitTokens		= 0x00004000;

/* Token type definitions:
**		tktInvalid		- lexical analyzer not properly initialize
**		tktEoln			- end of input line
**		tktSpace		- white space
**		tktSymbol	    - symbol string
**		tktKeyword		- language keyword
**		tktString		- quoted string
**		tktConstant		- numeric or character constant
**		tktOperator		- operator
**		tktColon		-
**		tktSemicolon	-
**		tktComma		-
**		tktDollar		-
**		tktHash			-
**		tktLPara		-
**		tktRPara		-
**		tktLBracket		-
**		tktRBracket		-
**		tktLBrace		-
**		tktRBrace		-
**		tktDigit		- digit character
*/
const int	tktInvalid		= 0;
const int	tktEof			= 1;
const int	tktEoln			= 2;
const int	tktSpace		= 3;
const int	tktSymbol		= 4;
const int	tktKeyword		= 5;
const int	tktString		= 6;
const int	tktConstant		= 7;
const int	tktOperator		= 8;
const int	tktColon		= 9;
const int	tktSemicolon	= 10;
const int	tktComma		= 11;
const int	tktDollar		= 12;
const int	tktHash			= 13;
const int	tktLPara		= 14;
const int	tktRPara		= 15;
const int	tktLBracket		= 16;
const int	tktRBracket		= 17;
const int	tktLBrace		= 18;
const int	tktRBrace		= 19;
const int	tktDigit		= 20;

/* Types of constants.
*/
const int	cstNumber		= 1;
const int	cstChar			= 2;

/* Types of operators.
*/
const int	optPlus			= 0;	// '+'
const int	optMinus		= 1;	// '-'
const int	optAsterisk		= 2;	// '*'
const int	optSlash		= 3;	// '/'
const int	optBitXor		= 4;	// '^'
const int	optBitNot		= 5;	// '~'
const int	optLogNot		= 6;	// '!'
const int	optAmpersand	= 7;	// '&'
const int	optBar			= 8;	// '|'
const int	optDot			= 9;	// '.'
const int	optMod			= 10;	// '%'
const int	optPercent		= 10;	// alias for optMod
const int	optQuestion		= 11;	// '?'
const int	optLogAnd		= 12;	// '&&'
const int	optLogOr		= 13;	// '||'
const int	optLsh			= 14;	// '<<'
const int	optRsh			= 15;	// '>>'
const int	optArrow		= 16;	// '->'
const int	optInc			= 17;	// '++'
const int	optDec			= 18;	// '--'
const int	optAssign		= 19;	// '='
const int	optRelEQ		= 20;	// '=='
const int	optRelNE		= 21;	// '!='
const int	optRelLT		= 22;	// '<'
const int	optRelLE		= 23;	// '<='
const int	optRelGT		= 24;	// '>'
const int	optRelGE		= 25;	// '>='
const int	optPlusEqual	= 26;	// '+='
const int	optMinusEqual	= 27;	// '-='
const int	optMulEqual		= 28;	// '*='
const int	optDivEqual		= 29;	// '/='
const int	optModEqual		= 30;	// '%='
const int	optAndEqual		= 31;	// '&='
const int	optOrEqual		= 32;	// '|='
const int	optXorEqual		= 33;	// '^='
const int	optLshEqual		= 34;	// '<<='
const int	optRshEqual		= 35;	// '>>='

/* Error codes for the token scan.
*/
const int	errLexrBase				= 0x0300;
const int	errLexrLast				= 0x03FF;
const int	errLexrOK				= 0;	//no error
const int	errLexrReadError		= errLexrBase+1;
const int	errLexrEndOfFile		= errLexrBase+2;
const int	errLexrNoFile			= errLexrBase+3;
const int	errLexrStringTooLong	= errLexrBase+4;
const int	errLexrInvalidStyle		= errLexrBase+5;
const int	errLexrIncludeDepth		= errLexrBase+6;
const int	errLexrIncludeUnderflow	= errLexrBase+7;

const int	errTknBase				= 0x0400;
const int	errTknLast				= 0x04FF;
const int	errTknOK				= 0;
const int	errTknInvalidChar		= errTknBase+1;	//unexpected char in stream
const int	errTknNumber			= errTknBase+2;
const int	errTknStringTooLong		= errTknBase+3;
const int	errTknMissingEndQuote	= errTknBase+4;
const int	errTknConstant			= errTknBase+5;	//invalid character in constant
const int	errTknCharConstant		= errTknBase+6;

/* ------------------------------------------------------------ */
/*					Variable Declarations						*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Object Class Declarations					*/
/* ------------------------------------------------------------ */

#if OptLexrKeywords != 0
/* This object class is used to maintain a symbol table that maps
** a keyword string to the corresponding keyword value. The LEXR
** doesn't provide the definitions of the keyword strings or the
** corresponding keyword ids. These are provided after the LEXR has
** been initialized by calling the AddKeyword method.
*/
typedef map<string,int> MPSM;

#endif

#if OptLexrInclude != 0
/* This object class is used to keep track of open input stream
** files for systems that support nested input streams (i.e.
** include operations). Note: This object doesn't own either the
** file pointer or the name string pointer. The code using this
** object is responsible for managing these. This object is 
** merely a repository for storing this information.
*/
class STMG {
private:
	FLG		flgCur;
	FILE *	fhFile;
	char *	szFile;
	int		ilnCur;

protected:

public:
			STMG();
		   ~STMG();
	void	SetSzFile(char * sz) { szFile = sz; };
	void	SetFhFile(FILE * fh) { fhFile = fh; };
	void	SetIlnCur(int iln) { ilnCur = iln; };
	void	SetFlgCur(FLG flg) { flgCur = flg; };
	FILE *	FhFile() { return fhFile; };
	char *	SzFile() { return szFile; };
	int		IlnCur() { return ilnCur; };
	FLG		FlgCur() { return flgCur; };
};
#endif

/* This class represents a lexical token in the language being
** scanned from the input stream.
*/
class TKN {

friend class LEXR;
friend class PRSR;

private:
	TKN *	ptknNext;
	int		err;
	int		ich;
	int		tktTkn;
	char 	szTkn[cchTokenMax];
	int		cstVal;		// type of constant
	int		nvRadix;	// radix for numeric constants
	int		cbtVal;		// size of constant (number of bytes)
	DWORD	dwVal;		// value of constant for numeric constant tokens
	int		optTkn;		// operator id for operator valued token
	int		idTkn;		// token id for id valued tokens

			TKN();
		   ~TKN();
	void	Reset();
	void	SetPtknNext(TKN * ptkn) { ptknNext = ptkn; };
	TKN *	PtknNext() { return ptknNext; };
	void	PutCh(char ch);
	void	DelCh();
	void	SetErr(int errN) { err = errN; };
	void	SetTktTkn(int tkt) { tktTkn = tkt; };
	void	SetSzTkn(char * sz) { SzCopy(szTkn, sz); };
	void	SetCstVal(int cst) { cstVal = cst; };
	void	SetDwVal(DWORD dw) { dwVal = dw; };
	void	SetOpt(int opt) { optTkn = opt; };
	void	SetId(int id) { idTkn = id; };
	void	SetNvRadix(int nv) { nvRadix = nv; };
	void	SetCbtVal(int cbt) { cbtVal = cbt; };
	bool	FSetConstNumber(int nv);
	void	SetConstChar(DWORD dwT) { 
				dwVal = dwT; tktTkn = tktConstant; cstVal = cstChar; };
	
public:
	int		TktToken() { return tktTkn; };
	DWORD	DwVal() { return dwVal; };
	int		OptToken() { return optTkn; };
	int		IdToken() { return idTkn; };
	char *	SzToken() { return szTkn; };
	int		CstVal() { return cstVal; };
	int		NvRadix() { return nvRadix; };
	int		CbtVal() { return cbtVal; };
	int		Err() { return err; };
};

/* This object class represents the lexcial analyzer that breaks the
** input stream down into a sequence of tokens.
*/
class LEXR {
private:
	FLG		flg;
	int		err;
	char *	szFile;
	char *	szLine;
	FILE *	fhFile;
#if OptLexrInclude != 0
	int		istmgCur;
	STMG	rgstmg[cstmgLexrIncludeMax];
#endif
#if OptLexrKeywords != 0
	MPSM	mpsmKwd;
#endif
	int		cchLine;
	int		cchBuffer;
	char	rgchUnget[cchUngetMax];
	int		ilnCur;
	int		ichCur;
	int		cchUnget;
	TKN *	ptknCur;
	TKN *	ptknUnget;
	TKN *	ptknFree;

	bool	FGetFileLine();
	char	ChNext();
	char	ChPutChGetNext(char ch);
	void	UngetCh(char ch);
	char	ChEatUntil(char ch);
	void	FreePtkn(TKN * ptkn);
	TKN *	PtknGet();
	void	ScanToken(TKN * ptkn);
	bool	FScanSymbol(char ch);
	bool	FScanConstant(char ch);
	bool	FScanOperator(char ch);
	bool	FScanDigit(char ch);
	void	ScanString(char ch);
	bool	FScanCharEscape();
	bool	FIsSpace(char ch) { return ((ch == ' ') || (ch == '\t')); };
	bool	FIsLower(char ch) { return ((ch >= 'a') && (ch <= 'z')); };
	bool	FIsUpper(char ch) { return ((ch >= 'A') && (ch <= 'Z')); };
	bool	FIsAlpha(char ch) { return FIsUpper(ch) || FIsLower(ch); };
	bool	FIsDigit(char ch) { return ((ch >= '0') && (ch <= '9')); };
	bool	FIsBinDigit(char ch) { return ((ch >= '0') && (ch <= '1')); };
	bool	FIsOctDigit(char ch) { return ((ch >= '0') && (ch <= '7')); };
	bool	FIsHexDigit(char ch) { return FIsDigit(ch) ||
										((ch >= 'A') && (ch <= 'F')) ||
										((ch >= 'a') && (ch <= 'f')); };
	bool	FIsSymChar(char ch);
	char	ChMakeUpper(char ch);


public:
			LEXR();
		   ~LEXR();
	bool	FInit(int cchLine);
	void	Terminate();
#if OptLexrKeywords != 0
	void	AddKeyword(char * sz, int id);
#endif
	int		Err() { return err; };
	bool	FSetScanStyle(DWORD flgStyle);
	void	StartScan();
	char *	SzFile() { return szFile; };
#if OptLexrInclude != 0
	char *	SzFile(int istmg);
#endif
	bool	FOpenFile(char * sz);
	void	CloseFile();
	bool	FNextLine();
	bool	FIsEof() { return flg.FTst(flgLexrEof); };
	bool	FSetLine(char * sz);
	char *	SzLine() { return szLine; };
	void	SetIln(int iln) { ilnCur = iln; };
	int		IlnCur() { return ilnCur; };
#if OptLexrInclude != 0
	int		IlnCur(int istmg);
	int		IstmgCur() { return istmgCur; };
#endif
	void	NextToken();
	void	UngetToken();
	TKN *	PtknCur() { return ptknCur; };
	int		TktToken() { return ptknCur->TktToken(); };
	int		OptToken() { return ptknCur->OptToken(); };
	int		ErrToken() { return ptknCur->Err(); };
	const char *	SzToken() { return ptknCur->SzToken(); };
	DWORD	DwValToken() { return ptknCur->DwVal(); };
};

/* ------------------------------------------------------------ */

#endif

/************************************************************************/
