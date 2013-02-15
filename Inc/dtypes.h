/************************************************************************/
/*                                                                      */
/*	dtypes.h  --	Digilent Common Type Declarations					*/
/*                                                                      */
/************************************************************************/
/*	Author:		Gene Apperson											*/
/*	Copyright 2004, Digilent Inc.										*/
/************************************************************************/
/*  File Description:                                                   */
/*                                                                      */
/*	This module contains declarations for constants and types used		*/
/*	across most Digilent applications.									*/
/*                                                                      */
/************************************************************************/
/*  Revision History:                                                   */
/*                                                                      */
/*	10/08/2004(GeneA): created											*/
/*	03/21/2008(GeneA): added bit number based methods to FLG object		*/
/*                                                                      */
/************************************************************************/

#if !defined(DTYPES_INC)
#define DTYPES_INC

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */

#define	LOCAL
#define	GLOBAL

/* ------------------------------------------------------------ */
/*					Common Type Definitions						*/
/* ------------------------------------------------------------ */

#if defined(__cplusplus)
const bool fFalse = false;
const bool fTrue = true;
#else
#define	fFalse	0
#define	fTrue	(!fFalse)
#endif

typedef	unsigned char	BYTE;
typedef	unsigned short	WORD;
typedef	unsigned long	DWORD;
typedef	unsigned long	ULONG;
typedef unsigned short	USHORT;

/* ------------------------------------------------------------ */
/*					Object Class Declarations					*/
/* ------------------------------------------------------------ */

class	FLG {
	DWORD	fsFlg;

public:
	void	Reset() { fsFlg = 0; };
	void	Set(DWORD fs) { fsFlg |= fs; };
	void	Set(int bn) { fsFlg |= (1 << bn); };
	void	Clr(DWORD fs) { fsFlg &= ~fs; };
	void	Clr(int bn) { fsFlg &= ~(1 << bn); };
	bool	FTst(DWORD fs) const { return (fsFlg & fs) != 0; };
	bool	FTst(int bn) const { return (fsFlg & (1 << bn)) != 0; };
	void	Mask(DWORD fs) { fsFlg &= fs; };
	DWORD	DwValue() const { return fsFlg; };
};

/* ------------------------------------------------------------ */
/*					Misc Constant Declarations					*/
/* ------------------------------------------------------------ */

const int	errOK				= 0;

/* ------------------------------------------------------------ */

#endif

/************************************************************************/
