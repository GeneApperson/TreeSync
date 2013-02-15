/************************************************************************/
/*                                                                      */
/*		prsr.h  --	Interface Declarations for PRSR Object				*/
/*                                                                      */
/************************************************************************/
/*	Author:		Gene Apperson											*/
/*	Copyright 2003, Digilent Inc.										*/
/************************************************************************/
/*  File Description:                                                   */
/*                                                                      */
/*                                                                      */
/************************************************************************/
/*  Revision History:                                                   */
/*                                                                      */
/*	08/04/2003(GeneA): created											*/
/*                                                                      */
/************************************************************************/

#if !defined(PRSR_INC)
#define	PRSR_INC

#include	"lexr.h"

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */

/* Extra token types defined by the parser on top of those defined
** by the scanner.
*/
const int	tktAny = -1;

const int	tktParser		= 100;
const int	tktRegister		= tktParser+0;

const int	errPrsrBase				= 0x200;
const int	errPrsrLast				= 0x2FF;
const int	errPrsrOk				= 0;	// no syntax error
const int	errPrsrUnexpectedToken	= errPrsrBase+1;
const int	errPrsrStringTooLong	= errPrsrBase+2;
const int	errPrsrIncorrectKeyword	= errPrsrBase+3;
const int	errPrsrSyntax			= errPrsrBase+4;
const int	errPrsrOutOfRange		= errPrsrBase+5;
const int	errPrsrSymbolExpected	= errPrsrBase+6;
const int	errPrsrStringExpected	= errPrsrBase+7;
const int	errPrsrCommaExpected	= errPrsrBase+7;
const int	errPrsrSymbolTooLong	= errPrsrBase+8;
const int	errPrsrUndefinedSymbol	= errPrsrBase+9;

/* ------------------------------------------------------------ */
/*						Type Declarations						*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*						Variable Declarations					*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Object Class Declarations					*/
/* ------------------------------------------------------------ */

class PRSR {
private:
	int		err;			//error code for parse errors
	LEXR *	plexr;
	TKN *	ptkn;

public:
				PRSR();
			   ~PRSR();
	bool		FInit();
	void		Terminate();
	bool		FBeginFile(char * szFile);
#if OptLexrKeywords != 0
	void		AddKeyword(char * sz, int id);
#endif
#if OptPrsrStringParse != 0
	bool		FBeginStringParse(char * sz);
	void		EndStringParse();
#endif		
	void		Start();
	bool		FNextLine() { return plexr->FNextLine(); };
	bool		FSetLine(char * sz);
	char *		SzCurrentLine() { return plexr->SzLine(); };
	char *		SzCurrentFile() { return plexr->SzFile(); }
	int			IlnCur() { return plexr->IlnCur(); };
#if OptLexrInclude != 0
	char *		SzFile(int istmg) { return plexr->SzFile(istmg); }
	int			IlnCur(int istmg) { return plexr->IlnCur(istmg); }
	int			IstmgCur() { return plexr->IstmgCur(); }
#endif
	bool		FIsEof() { return plexr->FIsEof(); };
	bool		FParseAny() { return FParseTkt(tktAny); };
	bool		FParseTkt(int tkt);
	void		UngetToken() { plexr->UngetToken(); };
	DWORD		DwValToken() { return ptkn->DwVal(); };
	int			NvRadixToken() { return ptkn->NvRadix(); };
	char *		SzToken() { return ptkn->SzToken(); };
	int			TktToken() { return ptkn->TktToken(); };
	int			OptToken() { return ptkn->OptToken(); };
	int			IdToken() { return ptkn->IdToken(); };
	int			CstToken() { return ptkn->CstVal(); };
	int			ErrToken() { return ptkn->Err(); };
	int			Err() { return err != errOK ? err : plexr->Err(); };

	bool		FParseNumConst(long & lVval);
	bool		FParseWord(WORD & wVal);
	bool		FParseSegOff(WORD & seg, WORD & off);
	bool		FParseBoolValue(char * szSym, bool & fVal);
	bool		FParseIntValue(char * szSym, int & iVal);
	bool		FParseStringValue(char * szSym, int cchMax, char * szVal);
	bool		FParseStringValue(int idKwd, int cchMax, char * szVal);
	bool		FParseRect(RECT & prct);
#if defined(DEAD)
	bool		FParseSel(SELS & sels);
	bool		FParseActive(bool & pfActive);
	bool		FParseLine(int & iln);
	bool		FParseFile(char * szFile);
#endif
};

/* ------------------------------------------------------------ */

#endif

/************************************************************************/
