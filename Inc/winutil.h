/************************************************************************/
/*                                                                      */
/*	winutil.h  --	Interface for Window Management Utility Functions	*/
/*                                                                      */
/************************************************************************/
/*	Author:		Gene Apperson											*/
/*	Copyright 2003, Digilent Inc.										*/
/************************************************************************/
/*  File Description:                                                   */
/*                                                                      */
/*	This file contains the interface declarations for misc. window		*/
/*	management utility functions in WINUTIL.CPP							*/
/*                                                                      */
/************************************************************************/
/*  Revision History:                                                   */
/*                                                                      */
/*	05/29/2003(GeneA): created by copying from earlier version			*/
/*                                                                      */
/************************************************************************/

#if !defined(WINUTIL_INC)
#define WINUTIL_INC

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */

#define	cchDutilStringMax	257

/* ------------------------------------------------------------ */
/*						Type Declarations						*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*						Variable Declarations					*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*						Procedure Declarations					*/
/* ------------------------------------------------------------ */

GLOBAL LPTSTR	SzLoadStringRes (int idrsz);
GLOBAL LPTSTR	SzLoadStringRes (int idrsz, char * szBuf);
GLOBAL LPTSTR	SzLoadStringRes (int idrsz, char * szBuf, HINSTANCE hinstRes);
GLOBAL LPTSTR	SzCopyStringRes (int idrsz);
GLOBAL BOOL		FCenterWindow (HWND hwndChild, HWND hwndParent);

bool		FBrowseForFolder(HWND hwnd, LPSTR pszDisplayName);

/* ------------------------------------------------------------ */

#endif

/************************************************************************/
