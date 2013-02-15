/************************************************************************/
/*                                                                      */
/*  winutil.cpp	--  Miscellaneous Window Management Utility Functions	*/
/*                                                                      */
/************************************************************************/
/*	Author: Gene Apperson												*/
/*	Copyright 2003, 2004, Digilent Inc.									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*	This module containst the definitions for various window management	*/
/*	utility functions.													*/
/*		                                                                */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*	05/29/2003(GeneA): created by copying from earlier version			*/
/*	06/04/2009(JoshP):  Upgraded to VS2008								*/
/*                                                                      */
/************************************************************************/


/* ------------------------------------------------------------ */
/*					Include File Definitions					*/
/* ------------------------------------------------------------ */
#define	_CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <shlobj.h>

#include "dtypes.h"
#include "winutil.h"

/* ------------------------------------------------------------ */
/*					Local Type Definitions						*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*					 Global Variables							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*					Local Variables								*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*					Forward Declarations						*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*					Procedure Definitions						*/
/* ------------------------------------------------------------ */
/***	FCenterWindow
**
**  Synopsis:
**		fSuccess = FCenterWindow(hwndChild, hwndParent)
**
**  Input:
**		hwndChild	- window handle of window to center
**		hwndParent	- window handle of window to center within
**
**  Output:
**		none
**
**  Errors:
**		Returns fFalse if error occurs
**
**  Description:
**		This function will center the specified child window
**		within the client rect of the specified parent window.
*/

GLOBAL BOOL
FCenterWindow (HWND hwndChild, HWND hwndParent)
{
	RECT    rChild, rParent, rWorkArea;
	int     wChild, hChild, wParent, hParent;
	int     xNew, yNew;
	BOOL	bResult;

	/* Get the Height and Width of the child window
	*/
	GetWindowRect (hwndChild, &rChild);
	wChild = rChild.right - rChild.left;
	hChild = rChild.bottom - rChild.top;

	/* Get the Height and Width of the parent window
	*/
	GetWindowRect (hwndParent, &rParent);
	wParent = rParent.right - rParent.left;
	hParent = rParent.bottom - rParent.top;

	/* Get the limits of the 'workarea'
	*/
	bResult = SystemParametersInfo(
				SPI_GETWORKAREA,  // system parameter to query or set
				sizeof(RECT),
				&rWorkArea,
				0);

	if (!bResult) {
		rWorkArea.left = rWorkArea.top = 0;
		rWorkArea.right = GetSystemMetrics(SM_CXSCREEN);
		rWorkArea.bottom = GetSystemMetrics(SM_CYSCREEN);
	}

	/* Calculate new X position, then adjust for workarea
	*/
	xNew = rParent.left + ((wParent - wChild) /2);
	if (xNew < rWorkArea.left) {
		xNew = rWorkArea.left;
	} else if ((xNew+wChild) > rWorkArea.right) {
		xNew = rWorkArea.right - wChild;
	}

	/* Calculate new Y position, then adjust for workarea
	*/
	yNew = rParent.top  + ((hParent - hChild) /2);
	if (yNew < rWorkArea.top) {
		yNew = rWorkArea.top;
	} else if ((yNew+hChild) > rWorkArea.bottom) {
		yNew = rWorkArea.bottom - hChild;
	}

	/* Set it, and return
	*/
	return SetWindowPos (hwndChild, NULL, xNew, yNew, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

}

/* ------------------------------------------------------------ */
/***	SzLoadStringRes
**
**  Synopsis:
**		szRes = SzGetStringRes(idrsz)
**
**  Input:
**		idrsz	- the resource id to acquire
**
**  Output:
**		returns 		
**
**  Errors:
**
**
**  Description:
**
**
*/

GLOBAL LPTSTR
SzLoadStringRes(int idrsz)
	{
	static char		szBuffer[cchDutilStringMax];

	szBuffer[0]=0;
	LoadString (GetModuleHandle(NULL), idrsz, szBuffer, cchDutilStringMax);
	return szBuffer;
}

/* ------------------------------------------------------------ */
/***	SzLoadStringRes
**
**  Synopsis:
**		szRes = SzGetStringRes(idrsz, szBuf)
**
**  Input:
**		idrsz	- the resource id to acquire
**		szBuf	- buffer to receive the string
**
**  Output:
**		returns 		
**
**  Errors:
**
**
**  Description:
**
**
*/

GLOBAL LPTSTR
SzLoadStringRes(int idrsz, char * szBuf)
	{

	szBuf[0]=0;
	LoadString (GetModuleHandle(NULL), idrsz, szBuf, cchDutilStringMax);
	return szBuf;
}

/* ------------------------------------------------------------ */
/***	SzLoadStringRes
**
**  Synopsis:
**		szRes = SzGetStringRes(idrsz, szBuf)
**
**  Input:
**		idrsz	- the resource id to acquire
**		szBuf	- buffer to receive the string
**		szMod	- name of module where resource exists
**
**  Output:
**		returns 		
**
**  Errors:
**
**
**  Description:
**
**
*/

GLOBAL LPTSTR
SzLoadStringRes(int idrsz, char * szBuf, HINSTANCE hinstRes)
	{

	szBuf[0]=0;
	LoadString (hinstRes, idrsz, szBuf, cchDutilStringMax);
	return szBuf;
}

/* ------------------------------------------------------------ */
/***	SzCopyStringRes
**
**  Synopsis:
**		szRes = SzCopyStringRes(idrsz)
**
**  Input:
**		idrsz	- the resource id to acquire
**
**  Output:
**		Returns pointer to a copy of the string resource		
**
**  Errors:
**		Returns NULL if resource doesn't exist	
**
**  Description:
**		This function will load the specified string resource
**		from the root application module's string table, allocate
**		dynamic memory for a copy of the string, and return a
**		pointer to a copy of the string.
*/

GLOBAL LPTSTR
SzCopyStringRes(int idrsz)
	{
	static char		szBuffer[cchDutilStringMax];
	char *			szTemp;
	int				cch;

	szBuffer[0]=0;
	cch = LoadString (GetModuleHandle(NULL), idrsz, szBuffer, cchDutilStringMax);
	szTemp = (char *)HeapAlloc(GetProcessHeap(), 0, cch+1);
	if (szTemp != NULL) {
		strcpy(szTemp, szBuffer);
	}
	return szTemp;
}

/* ------------------------------------------------------------ */
/***	FBrowseForFolder
**
**	Synopsis:
**		fOK = FBrowseForFolder(hwnd, szDisplayName)
**
**	Input:
**		hwnd			- window handle of parent window
**		szDisplayName	- buffer to receive folder name
**
**	Output:
**		Returns fTrue if use hit OK, fFalse if use hit CANCEL
**
**	Errors:
**		none
**
**	Description:
**		This function will bring up a dialog box for the user
**		to browse for and choose a folder for the project.
*/

bool
FBrowseForFolder(HWND hwnd, LPSTR pszDisplayName)
	{
    LPITEMIDLIST pidlRoot = NULL;
    LPITEMIDLIST pidlSelected = NULL;
    BROWSEINFO bi = {0};
    LPMALLOC pMalloc = NULL;
	bool				fStat;

    SHGetMalloc(&pMalloc);

	pidlRoot = NULL;

    bi.hwndOwner = hwnd;
    bi.pidlRoot = pidlRoot;
    bi.pszDisplayName = pszDisplayName;
    bi.lpszTitle = "Choose a folder";
    bi.ulFlags = 0;
    bi.lpfn = NULL;
    bi.lParam = 0;

    pidlSelected = SHBrowseForFolder(&bi);
	fStat = (pidlSelected != NULL);
	if (fStat) {
		fStat = SHGetPathFromIDList(pidlSelected, pszDisplayName) == TRUE;
	}

	pMalloc->Free(pidlSelected);
    pMalloc->Release();
    return fStat;
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

/************************************************************************/

