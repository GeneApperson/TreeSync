/************************************************************************/
/*																		*/
/*	config.h	--	Define Build Configuration Control Symbols			*/
/*																		*/
/************************************************************************/
/*	Author:		Gene Apperson											*/
/*	Copyright 2010, Digilent Inc.										*/
/************************************************************************/
/*  File Description:													*/
/*																		*/
/*	Define symbols used to control build configurations.				*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	12/28/2010(GeneA): created											*/
/*																		*/
/************************************************************************/

#if !defined(CONFIG_INC)
#define CONFIG_INC

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */

const int	verMajor	= 1;
const int	verMinor	= 0;
const int	verBuild	= 0;

/* ------------------------------------------------------------ */
/*					General Symbol Declarations					*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*					Compile Switches for PRSR					*/
/* ------------------------------------------------------------ */

/* Build configuration options for prsr.cpp
*/
#define OptPrsrBoolValue	0
#define OptPrsrStringValue	1
#define OptPrsrIntValue		0
#define OptPrsrRectStmt		0
#define OptPrsrFileStmt		0
#define	OptPrsrLineStmt		0

/* Build configuration options for prsr.cpp
*/
#define	OptPrsrStringParse	0

/* Build configuration options for lexr.cpp
*/
#define	OptLexrKeywords		1
#define	OptLexrInclude		1
#define	OptLexrDigitTokens	0

/* ------------------------------------------------------------ */
/*					Variable Declarations						*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Procedure Declarations						*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */

#endif	//_INC_CONFIG

/************************************************************************/
