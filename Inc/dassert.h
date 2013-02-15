/************************************************************************/
/*                                                                      */
/*		dassert.h  --	Debug Support Macros							*/
/*                                                                      */
/************************************************************************/
/*	Author:		Gene Apperson											*/
/*	Copyright 2003, 2004, 2005, Digilent Inc.							*/
/************************************************************************/
/*  File Description:                                                   */
/*                                                                      */
/*	This header file contains various ASSERT macros for debugging		*/
/*	support.															*/
/*                                                                      */
/************************************************************************/
/*  Revision History:                                                   */
/*                                                                      */
/*	06/09/2003(GeneA): created											*/
/*                                                                      */
/************************************************************************/

#if !defined(_INC_DASSERT)

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */

#if defined(_DEBUG)
#define Assert(cond, msg) if (!(cond)) { DoAssert(_szCurFile, __LINE__, msg); }
#else
#define Assert(cond, msg)
#endif


#if defined(_DEBUG)
#define __SetCurFile static char _szCurFile[] = __FILE__
#else
#define __SetCurFile
#endif

#if defined(_DEBUG)
#define	AssertMsg(szVar, szMsg) char szVar[] = szMsg
#else
#define AssertMsg(szVar, szMsg)
#endif

/* ------------------------------------------------------------ */
/*						Type Declarations						*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*						Variable Declarations					*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*						Procedure Declarations					*/
/* ------------------------------------------------------------ */

#if defined(_DEBUG)
extern void DoAssert(char * szFile, int ilnLine, char * szMsg);
#endif

/* ------------------------------------------------------------ */

#endif

/************************************************************************/
