/************************************************************************/
/*                                                                      */
/*  vwapp.cpp	--  Application Main Window View Object Class			*/
/*                                                                      */
/************************************************************************/
/*	Author: 	Gene Apperson											*/
/*	Copyright 2005, Digilent Inc.										*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*	This object class implements the behavior of the main application	*/
/*	window.
/*                                                                      */
/************************************************************************/
/*  Public Functions:													*/
/*                                                                      */
/*                                                                      */
/************************************************************************/
/*  Private Functions:													*/
/*                                                                      */
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*	01/13/2005(GeneA): created											*/
/*	06/03/2005(GeneA): added minimize box to caption bar				*/
/*	07/28/2005(GeneA): fix bug 27. Modified PutStatusMessage to delete	*/
/*		the first line in the window before inserting a new line if the	*/
/*		window contains the maximum number of lines.					*/
/*	12/16/2005(GeneA): fix bug 50. Added call to FSetMode function to	*/
/*		set speed to a lower rate if the first attempt to enter program	*/
/*		mode fails.														*/
/*                                                                      */
/************************************************************************/


/* ------------------------------------------------------------ */
/*					Include File Definitions					*/
/* ------------------------------------------------------------ */

#include	<windows.h>
#include	<commctrl.h>
#include	<stdio.h>

#include	"resource.h"
#include	"config.h"
#include	"gendefs.h"
#include	"appdefs.h"
#include	"dassert.h"

#include	"dutil.h"
#include	"winutil.h"

#include	"app.h"
#include	"view.h"
#include	"vwapp.h"

/* ------------------------------------------------------------ */
/*					Local Type Definitions						*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*					 Global Variables							*/
/* ------------------------------------------------------------ */

extern APP		app;

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


/* ------------------------------------------------------------ */
/*					VWAPP Object Class Implementation			*/
/* ------------------------------------------------------------ */
/***	VWAPP::FInitVWAPP
**
**	Synopsis:
**		fSuccess = FInitVWAPP(hinst)
**
**	Input:
**		none
**
**	Output:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		This routine initializes the system for use of the
**		VWAPP object class. This includes registering the
**		associated window class with the system.
*/

bool
VWAPP::FInitVWAPP()
	{
	char		szClassApp[cchNameMax];
    WNDCLASSEX				wcl;

    /* Load string resources for the class name
    */
    SzLoadStringRes(idsClassApp, szClassApp);

    /* Register the window class.
    */
	wcl.cbSize			= sizeof(wcl);
    wcl.style			= CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
    wcl.lpfnWndProc		= MresWndProcView;
    wcl.cbClsExtra		= 0;
    wcl.cbWndExtra		= sizeof(HANDLE);
    wcl.hInstance		= app.HinstApp();
    wcl.hIcon			= LoadIcon(app.HinstApp(), MAKEINTRESOURCE(idiApp));
    wcl.hIconSm			= NULL;
    wcl.hCursor 		= LoadCursor(NULL, IDC_ARROW);
	wcl.hbrBackground	= NULL;
    wcl.lpszMenuName	= NULL;
    wcl.lpszClassName	= (LPSTR) szClassApp;

    if (!RegisterClassEx(&wcl)) {
		return fFalse;
    }

	return fTrue;

}

/* ------------------------------------------------------------ */
/***	VWAPP::TerminateVWAPP
**
**	Synopsis:
**		TerminateVWAPP()
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
**		Free any resources allocated by FInitVWAPP
*/

void
VWAPP::TerminateVWAPP()
	{
	char	szClassApp[cchNameMax];

    SzLoadStringRes(idsClassApp, szClassApp);
	UnregisterClass(szClassApp, app.HinstApp());

}

/* ------------------------------------------------------------ */
/***	VWAPP::VWAPP
**
**	Synopsis:
**		VWAPP()
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

VWAPP::VWAPP()
	{
	int		ipdlg;

	hwnd = NULL;
	hwndTab = NULL;
	hwndRpt = NULL;
	hwndPrg = NULL;
	hfntRpt = NULL;
	
	for (ipdlg = 0; ipdlg < ctabMax; ipdlg++) {
		rgpdlgTabs[ipdlg] = NULL;
	}
	
	ipdlgCur = 0;

}

/* ------------------------------------------------------------ */
/***	VWAPP::~VWAPP()
**
**	Synopsis:
**		~VWAPP()
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

VWAPP::~VWAPP()
	{

}

/* ------------------------------------------------------------ */
/***	VWAPP::FInit
**
**	Synopsis:
**		fSuccess = FInit(hwndApp)
**
**	Input:
**		hwndApp	- window handle of the application main window
**
**	Output:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Initialize the object for use after construction
*/

bool
VWAPP::FInit(VIEW * pviewParent)
	{
	int			err;

	VIEW::FInit(pviewParent);
	
	/* Attempt to load the application settings from the system registry.
	** If this fails, initialize to the default state and attempt to
	** save the default state into the registry.
	*/
	
	
	return fTrue;

}

/* ------------------------------------------------------------ */
/***	VWAPP::Terminate
**
**	Synopsis:
**		Terminate()
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
**		This routine terminates use of the object and frees any
**		resources before destruction.
*/

void
VWAPP::Terminate()
	{
	int		ipdlg;

	for (ipdlg = 0; ipdlg < ctabMax; ipdlg++) {
		if (rgpdlgTabs[ipdlg] != NULL) {
			rgpdlgTabs[ipdlg]->Terminate();
			delete rgpdlgTabs[ipdlg];
		}
	}
	
	if (hfntRpt != NULL) {
		DeleteObject(hfntRpt);
		hfntRpt = NULL;
	}
	
	app.ViewClosing();
	VIEW::Terminate();
}

/* ------------------------------------------------------------ */
/***	VWAPP::FCreateHwnd
**
**	Synopsis:
**		fSuccess = FCreateHwnd(xco, yco, dxco, dyco, pviewParent)
**
**	Parameters:
**		xco			- x coordinate of window origin
**		yco			- y coordinate of window origin
**		dxco		- window width
**		dyco		- window height
**		pviewParent	- pointer to parent window object
**
**	Return Values:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Create and display the main application window.
*/

bool
VWAPP::FCreateHwnd(int xco, int yco, int dxco, int dyco, VIEW * pviewParent)
	{
	char		szClassApp[cchNameMax];
	char		szAppName[cchNameMax];
	HWND		hwndParent;
	LOGFONT		lpf;
	HDC			hdc;
	HFONT		hfntOld;
	TEXTMETRIC	txm;

	SzLoadStringRes(idsClassApp, szClassApp);
	SzLoadStringRes(idsAppName, szAppName);

	hwndParent = NULL;
	if (pviewParent != NULL) {
		hwndParent = pviewParent->Hwnd();
	}

    hwnd = CreateWindow(
		szClassApp,								/* window class	     */
		szAppName,                      		/* window name	     */
		WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,	/* window style	     */
		xco,									/* x position		 */
		yco,									/* y position 	     */
		dxco,									/* width		     */
		dyco,									/* height		     */
		hwndParent,								/* parent handle	 */
        NULL,							/* menu or child ID  */
		app.HinstApp(),							/* instance		     */
        this									/* additional info   */
    );

    if (!hwnd) {
        return (fFalse);
    }

	SetWindowLong(hwnd, GWL_USERDATA, (DWORD)this);

	if (SendMessage(hwnd, WM_INITVIEW, 0, 0) == 0) {
		DestroyWindow(hwnd);
		return fFalse;
	}
		
	/* Create the fonts to be used for the multi-line edit control for
	** reporting status and the VWLST controls in the Fuses and Lock Bits
	** windows.
	*/
	hdc = GetDC(hwnd);
	lpf.lfWidth = 0;
	lpf.lfEscapement = 0;
	lpf.lfOrientation = 0;
	lpf.lfWeight = FW_NORMAL;
	lpf.lfItalic = 0;
	lpf.lfUnderline = 0;
	lpf.lfStrikeOut = 0;
	lpf.lfCharSet = DEFAULT_CHARSET;
	lpf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lpf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lpf.lfQuality = DEFAULT_QUALITY;
	lpf.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
	SzCopy(lpf.lfFaceName, "Tahoma");

	lpf.lfHeight = -MulDiv(sptCharRpt, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	hfntRpt = CreateFontIndirect(&lpf);
	
	/* Determine the height and average width of the font so that we
	** can use it for determining child window size and placement
	*/
    if (hfntRpt != NULL) {
		hfntOld = (HFONT)SelectObject(hdc, hfntRpt);
	}
    GetTextMetrics (hdc, &txm);
    dycoCharRpt = txm.tmHeight+txm.tmExternalLeading;
    dxcoCharRpt = txm.tmAveCharWidth;
    if (hfntRpt != NULL) {
		SelectObject(hdc, hfntOld);
	}
 
	ReleaseDC(hwnd, hdc);
	
	/* Load the tab dialogs, create the tab control and then size the
	** window to fit the dialogs.
	*/
	if (!FLoadTabDialogs()) {
		return fFalse;
	}
	
	if (!FCreateTabControl()) {
		return fFalse;
	}
	
	if (!FCreateRptControl()) {
		return fFalse;
	}
	
	if (!FCreatePrgControl()) {
		return fFalse;
	}
	
	/* Now that all of the children of the main window have been created,
	** size and position everything before we display the main window
	** for the first time.
	*/
	if (!FAdjustWindowSize()) {
		return fFalse;
	}

	/* Select the default tab and display the associated dialog box.
	*/
	::ShowWindow(hwndRpt, SW_SHOW);
	::ShowWindow(hwndPrg, SW_SHOW);
	ipdlgCur = 0;
	::ShowWindow(rgpdlgTabs[ipdlgCur]->Hdlg(), SW_SHOW);

	return fTrue;

}

/* ------------------------------------------------------------ */
/***	VWAPP::DoStartup
**
**	Synopsis:
**		DoStartup()
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
**		The app has been initialized and the main window created
**		successfully. Perform any one time startup tasks that
**		need to be performed after the window is alive.
*/

void
VWAPP::DoStartup()
	{
	char	szTemp[cchTempMax];
	
	sprintf(szTemp, SzLoadStringRes(idsMsgSignon), verMajor, verMinor);
	PutStatusMessage(szTemp);
	
	app.SetHdlgModeless(rgpdlgTabs[ipdlgCur]->Hdlg());
	
    ShowWindow(SW_SHOW);
	UpdateWindow();

}

/* ------------------------------------------------------------ */
/***	VWAPP::DoClose
**
**	Synopsis:
**		DoClose()
**
**	Parameters:
**		none
**
**	Return Values:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This function is called in response to a WM_CLOSE message.
**		A request has occured to close the window.
*/

void
VWAPP::DoClose()
	{
	int		err;
	HWND	hdlg;
	
	/* Tell the currently selected tab dialog to deactivate so that
	** it updates its current state into the PGMP object before we
	** save it to the registry.
	*/
	hdlg = rgpdlgTabs[ipdlgCur]->Hdlg();
	SendMessage(hdlg, WM_ACTIVATE, MAKEWPARAM(WA_INACTIVE, 0), NULL);
	
	PostQuitMessage(0);

}

/* ------------------------------------------------------------ */
/***	VWAPP::DoCommand
**
**	Synopsis:
**		DoCommand(idCmd, idNotify, hwndCtl)
**
**	Input:
**		idCmd		- command id or child window id
**		idNotify	- notification code if command is from a control
**		hwndCtl		- window handle fo control if command is from
**						a control
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Process command messages for the VWAPP window.
*/

void
VWAPP::DoCommand(UINT idCmd, int idNotify, HWND hwndCtl)
	{

	switch(idCmd) {
		case idmFileExit:
			Close();
			return;

	}
}

/* ------------------------------------------------------------ */
/***	VWAPP::DoNotify
**
**	Synopsis:
**		DoNotify(idc, pnmhdr)
**
**	Parameters:
**		idc			- window id of control sending message
**		pnmhdr		- pointer to notification message structure
**
**	Return Values:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Default WM_NOTIFY mesage handling
*/

void
VWAPP::DoNotify(int idc, NMHDR * pnmhdr)
	{
	int		ipdlgNew;
	HWND	hdlg;

	if (idc == idcTab) {
		if (pnmhdr->code == TCN_SELCHANGE) {

			/* The tab selection has changed. Find out which
			** one is the new selected tab.
			*/
			ipdlgNew = TabCtrl_GetCurSel(hwndTab);
			
			if ((ipdlgNew < 0) || (ipdlgNew >= ctabMax)) {
				return;
			}
						
			/* Tell the old one it is being deactivated.
			*/
			hdlg = rgpdlgTabs[ipdlgCur]->Hdlg();
			SendMessage(hdlg, WM_ACTIVATE, (WPARAM)WA_INACTIVE, (LPARAM)hdlg);
			
			::ShowWindow(rgpdlgTabs[ipdlgCur]->Hdlg(), SW_HIDE);
			::ShowWindow(rgpdlgTabs[ipdlgNew]->Hdlg(), SW_SHOW);
			ipdlgCur = ipdlgNew;
			
			/* Tell the new one it is being activated.
			*/
			hdlg = rgpdlgTabs[ipdlgCur]->Hdlg();
			SendMessage(hdlg, WM_ACTIVATE, (WPARAM)WA_ACTIVE, (LPARAM)hdlg);
			
			app.SetHdlgModeless(hdlg);
			
			UpdateWindow();
		}
	}
}

/* ------------------------------------------------------------ */
/***	VWAPP::DoMove
**
**	Synopsis:
**		DoMove(xcoNew, ycoNew)
**
**	Input:
**		xcoNew		- new x position of the window
**		ycoNew		- new y position of the window
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Adjust based on the new window location
*/

void
VWAPP::DoMove(int xcoNew, int ycoNew)
	{

}

/* ------------------------------------------------------------ */
/***	VWAPP::DoSize
**
**	Synopsis:
**		DoSize(dxcoNew, dycoNew)
**
**	Input:
**		dxcoNew		- new width of the window
**		dycoNew		- new height of the window
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Adjust based on the new window size
*/

void
VWAPP::DoSize(int dxcoNew, int dycoNew)
	{

}

/* ------------------------------------------------------------ */
/***	VWAPP:DoPaint
**
**	Synopsis:
**		DoPaint(hdc, pps)
**
**	Input:
**		hdc		- display context to use for drawing
**		pps		- pointer to the paint struct to use
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Redraw the window.
*/

void
VWAPP::DoPaint(HDC hdc, PAINTSTRUCT * pps)
	{
	RECT	rctClient;
	HBRUSH	hbr;	

	GetClientRect(hwnd, &rctClient);
	hbr = CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
	FillRect(hdc, &rctClient, hbr);
	DeleteObject(hbr);

}

/* ------------------------------------------------------------ */
/***	VWAPP::FLoadTabDialogs
**
**	Synopsis:
**		fSuccess = FLoadTabDialogs()
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
**		Load the modeless dialog boxes used for the various
**		tabs on the application window. 
*/

bool
VWAPP::FLoadTabDialogs()
	{
	int		ipdlg;

#if defined(DEAD)
	/* Create the dialog for the Program tab.
	*/
	ipdlg = ipdlgTabProgram;
	if (((rgpdlgTabs[ipdlg] = new VWPGM) == NULL) || !rgpdlgTabs[ipdlg]->FInit()) {
		return fFalse;
	}
	if (!rgpdlgTabs[ipdlg]->FCreateDialog(app.HinstApp(), hwnd, MAKEINTRESOURCE(iddTabProgram))) {
		return fFalse;
	}
#endif
	
#if defined(DEAD)
	/* Create the dialog for the Fuses tab.
	*/
	ipdlg = ipdlgTabFuses;
	if (((rgpdlgTabs[ipdlg] = new VWFSE) == NULL) || !rgpdlgTabs[ipdlg]->FInit()) {
		return fFalse;
	}
	if (!rgpdlgTabs[ipdlg]->FCreateDialog(app.HinstApp(), hwnd, MAKEINTRESOURCE(iddTabFuses))) {
		return fFalse;
	}
#endif

#if defined(DEAD)
	/* Create the dialog for the Lock Bits tab
	*/
	ipdlg = ipdlgTabLockBits;
	if (((rgpdlgTabs[ipdlg] = new VWLKB) == NULL) || !rgpdlgTabs[ipdlg]->FInit()) {
		return fFalse;
	}
	if (!rgpdlgTabs[ipdlg]->FCreateDialog(app.HinstApp(), hwnd, MAKEINTRESOURCE(iddTabLockBits))) {
		return fFalse;
	}
#endif

#if defined(DEAD)	
	/* Create the dialog for the Programmer Settings tab.
	*/
	ipdlg = ipdlgTabPgmSettings;
	if (((rgpdlgTabs[ipdlg] = new VWPSTG) == NULL) || !rgpdlgTabs[ipdlg]->FInit()) {
		return fFalse;
	}
	if (!rgpdlgTabs[ipdlg]->FCreateDialog(app.HinstApp(), hwnd, MAKEINTRESOURCE(iddTabPgmSettings))) {
		return fFalse;
	}
#endif

#if defined(DEAD)
	/* Create the dialog for the Device Settings tab.
	*/
	ipdlg = ipdlgTabDevSettings;
	if (((rgpdlgTabs[ipdlg] = new VWDSTG) == NULL) || !rgpdlgTabs[ipdlg]->FInit()) {
		return fFalse;
	}
	if (!rgpdlgTabs[ipdlg]->FCreateDialog(app.HinstApp(), hwnd, MAKEINTRESOURCE(iddTabDevSettings))) {
		return fFalse;
	}
#endif

	return fTrue;

}

/* ------------------------------------------------------------ */
/***	VWAPP::FCreateTabControl
**
**	Synopsis:
**		fSuccess = FCreateTabControl()
**
**	Input:
**		none
**
**	Output:
**		none
**
**	Errors:
**		return fTrue if successful, fFalse if not
**
**	Description:
**		This will create the tab control for the main appliction
**		window, create the tab items for the dialog boxes used
**		for each tab, and then size the tab control and window
**		appropriately.
*/

bool
VWAPP::FCreateTabControl()
	{
	TCITEM		tci;
	int			itci;
	int			ipdlg;
	char		szTemp[cchStringMax];
	RECT		rct;
	int			dxco;
	int			dyco;
	int			dxcoTab;
	int			dycoTab;
	
	/* Find the dimensions of the largest dialog box to be displayed
	** in the tab control.
	*/
	dxcoTab = 0;
	dycoTab = 0;

	for (ipdlg = 0; ipdlg < ctabMax; ipdlg++) {
		GetWindowRect(rgpdlgTabs[ipdlg]->Hdlg(), &rct);
		dxco = rct.right - rct.left;
		dyco = rct.bottom - rct.top;
		if (dxco > dxcoTab) {
			dxcoTab = dxco;
		}
		if (dyco > dycoTab) {
			dycoTab = dyco;
		}
	}

	/* Adjust the size to account for the placement of the dialog
	** box within the tab control.
	*/
	dxcoTab += dxcoDlgMarginLeft + dxcoDlgMarginRight;
	dycoTab += dycoDlgMarginTop + dycoDlgMarginBottom;

	/* Create the tab control to fit into the window.
	*/
    hwndTab = CreateWindow( 
				WC_TABCONTROL,
				"", 
				WS_CHILD|WS_CLIPSIBLINGS|WS_VISIBLE|TCS_MULTILINE, 
				dxcoTabMarginLeft, dycoTabMarginTop,
				dxcoTab, dycoTab, 
				hwnd,
				(HMENU)idcTab,
				app.HinstApp(),
				NULL 
			);

	if (hwndTab == NULL) {
		return fFalse;
    }
 
    /* Add the tabs to the tab control.
	*/
    for (itci = 0; itci < ctabMax; itci++) { 
	    tci.mask = TCIF_TEXT | TCIF_IMAGE; 
	    tci.iImage = -1; 
	    tci.pszText = szTemp; 
        LoadString(app.HinstApp(), idsTabStart+itci, 
                szTemp, sizeof(szTemp)); 
 
        if (TabCtrl_InsertItem(hwndTab, itci, &tci) == -1) { 
            return fFalse; 
        } 
    }

	/* Make the modeless dialogs be children of the tab control.
	*/
	for (ipdlg = 0; ipdlg < ctabMax; ipdlg++) {
		SetWindowPos(rgpdlgTabs[ipdlg]->Hdlg(), NULL, 
				dxcoDlgMarginLeft, dycoDlgMarginTop, 0, 0,
				SWP_NOZORDER|SWP_NOSIZE);
		SetParent(rgpdlgTabs[ipdlg]->Hdlg(), hwndTab);
	}
	
	return fTrue;

}

/* ------------------------------------------------------------ */
/***	VWAPP::FCreateRptControl
**
**	Synopsis:
**		fSuccess = FCreateRptControl()
**
**	Input:
**		none
**
**	Output:
**		none
**
**	Errors:
**		Return fTrue if successful, fFalse if not
**
**	Description:
**		Create the multiline edit control used for the status
**		report window.
*/

bool
VWAPP::FCreateRptControl()
	{
	int			xcoRpt;
	int			ycoRpt;
	int			dxcoRpt;
	int			dycoRpt;

    /* Create the edit control used for the status report display.
    ** The position and width aren't critical, since these will be
    ** adjusted later. The height is based on the size of the font
    ** that will be used.
	*/
	xcoRpt = dxcoTabMarginLeft;
	ycoRpt = dycoTabMarginTop;
	dxcoRpt = 100;
	dycoRpt = 10 * dycoCharRpt;

    hwndRpt = CreateWindowEx( 
        WS_EX_CLIENTEDGE, 
        "EDIT",		// name of status bar class 
        (LPCTSTR) NULL,			// no text when first created 
        WS_CHILD|WS_VSCROLL|ES_MULTILINE|ES_READONLY|ES_WANTRETURN|ES_AUTOVSCROLL, 
        xcoRpt, ycoRpt,
        dxcoRpt, dycoRpt,
        hwnd,					// handle to parent window 
        (HMENU) idcRpt,			// child window identifier 
        app.HinstApp(),			// handle to application instance 
        NULL);					// no window creation data

	if (hwndRpt == NULL) {
		return fFalse;
	}

	if (hfntRpt != NULL) {	
		SendMessage(hwndRpt, WM_SETFONT, (WPARAM)hfntRpt, 0);
	}
	
	return fTrue;
	
}

/* ------------------------------------------------------------ */
/***	VWAPP::FCreatePrgControl
**
**	Synopsis:
**		fSuccess = FCreatePrgControl()
**
**	Input:
**		none
**
**	Output:
**		none
**
**	Errors:
**		Return fTrue if successful, fFalse if not
**
**	Description:
**		Create the progress bar control used to display progress
**		of time consuming operations.
*/

bool
VWAPP::FCreatePrgControl()
	{
	int		xcoPrg;
	int		ycoPrg;
	int		dxcoPrg;
	int		dycoPrg;
	
	xcoPrg = dxcoTabMarginLeft;
	ycoPrg = dycoTabMarginTop;
	dxcoPrg = 100;
	dycoPrg = dycoCharRpt;

    hwndPrg = CreateWindowEx( 
        WS_EX_CLIENTEDGE, 
        PROGRESS_CLASS,		// name of progress bar class 
        (LPCTSTR) NULL,			// no text when first created 
        WS_CHILD, 
        xcoPrg, ycoPrg,
        dxcoPrg, dycoPrg,
        hwnd,					// handle to parent window 
        (HMENU) idcPrg,			// child window identifier 
        app.HinstApp(),			// handle to application instance 
        NULL);					// no window creation data

	if (hwndPrg == NULL) {
		return fFalse;
	}
	
	SendMessage(hwndPrg, PBM_SETBARCOLOR, 0, (LPARAM)clrPrgBar);

	return fTrue;
	
}

/* ------------------------------------------------------------ */
/***	VWAPP::FAdjustWindowSize
**
**	Synopsis:
**		fSuccess = FAdjustWindowSize()
**
**	Input:
**		none
**
**	Output:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Make the final adjustments to the size of the main application
**		window and the placement of the control windows within the
**		main window.
*/
bool
VWAPP::FAdjustWindowSize()
	{
	RECT		rct;
	RECT		rctWnd;
	RECT		rctClient;
	int			xco;
	int			yco;
	int			dxcoWnd;
	int			dycoWnd;
	int			dxcoBorder;
	int			dycoBorder;
	int			dxcoTab;
	int			dycoTab;
	int			dxcoRpt;
	int			dycoRpt;
	int			dxcoPrg;
	int			dycoPrg;

	/* Get the sizes of the component child windows.
	*/
	GetWindowRect(hwndTab, &rct);
	dxcoTab = rct.right - rct.left;
	dycoTab = rct.bottom - rct.top;	
	
	GetWindowRect(hwndRpt, &rct);
	dxcoRpt = dxcoTab;
	dycoRpt = rct.bottom - rct.top;
	
	GetWindowRect(hwndPrg, &rct);
	dxcoPrg = dxcoTab;
	dycoPrg = rct.bottom - rct.top;
	
	/* Determine the amount of the main window needed for borders,
	** title bar, etc.
	*/
	GetWindowRect(hwnd, &rctWnd);
	GetClientRect(hwnd, &rctClient);

	dxcoBorder = rctWnd.right - rctWnd.left - rctClient.right;
	dycoBorder = rctWnd.bottom - rctWnd.top - rctClient.bottom;
	
	/* Compute the required main window size and size the main window.
	*/
	dxcoWnd = dxcoTab + dxcoTabMarginLeft + dxcoTabMarginRight + dxcoBorder;
	dycoWnd = dycoTabMarginTop + dycoTab + dycoTabMarginBottom +
					dycoRpt + dycoRptMarginBottom +
					dycoPrg + dycoPrgMarginBottom +
					dycoBorder;
	
	SetWindowPos(hwnd, NULL, rctWnd.left, rctWnd.top, dxcoWnd, dycoWnd, SWP_NOZORDER);
	
	/* Position and size the child components within the parent window.
	*/
	xco = dxcoTabMarginLeft;
	yco = dycoTabMarginTop;
	SetWindowPos(hwndTab, NULL, xco, yco, dxcoTab, dycoTab, SWP_NOZORDER);
	
	yco += dycoTab + dycoTabMarginBottom;
	SetWindowPos(hwndRpt, NULL, xco, yco, dxcoRpt, dycoRpt, SWP_NOZORDER);
	
	yco += dycoRpt + dycoRptMarginBottom;
	SetWindowPos(hwndPrg, NULL, xco, yco, dxcoPrg, dycoPrg, SWP_NOZORDER);
		

	return fTrue;
	
}

/* ------------------------------------------------------------ */
/***	VWAPP::HfntRpt
**
**	Synopsis:
**		hfnt = HfntRpt()
**
**	Input:
**		none
**
**	Output:
**		Returns font to use for the status report window
**
**	Errors:
**		none
**
**	Description:
**		Return handle to font to be used for drawing the status
**		report window.
*/

HFONT
VWAPP::HfntRpt() const
	{
	
	if (hfntRpt != NULL) {
		return hfntRpt;
	}
	else {
		return (HFONT)GetStockObject(SYSTEM_FONT);
	}
	
}

/* ------------------------------------------------------------ */
/***	VWAPP::PutStatusMessage
**
**	Synopsis:
**		PutStatusMessage(sz)
**
**	Input:
**		sz		- message to write to status window
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Write the specified message to the status window.
*/

void
VWAPP::PutStatusMessage(char * sz)
	{
	int		cln;
	int		cch;
	int		ich;
	
	cln = SendMessage(hwndRpt, EM_GETLINECOUNT, 0, 0);
	if (cln > clnRptMax) {
		cch = SendMessage(hwndRpt, EM_LINELENGTH, 0, 0);
		SendMessage(hwndRpt, EM_SETSEL, 0, cch+2);
		SendMessage(hwndRpt, EM_REPLACESEL, 0, (LPARAM)"");
	}
	
	cln = SendMessage(hwndRpt, EM_GETLINECOUNT, 0, 0);
	ich = SendMessage(hwndRpt, EM_LINEINDEX, cln-1, 0);
	cch = SendMessage(hwndRpt, EM_LINELENGTH, cln-1, 0);
	SendMessage(hwndRpt, EM_SETSEL, ich+cch, ich+cch);

	SendMessage(hwndRpt, EM_REPLACESEL, 0, (LPARAM)sz);
	SendMessage(hwndRpt, EM_REPLACESEL, 0, (LPARAM)"\r\n");
	
}

/* ------------------------------------------------------------ */
/***	VWAPP::SetProgressRange
**
**	Synopsis:
**		SetProgressRange(xpvLim)
**
**	Input:
**		xpvLim		- progress bar limit value
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Set the range value for the progress bar.
*/

void
VWAPP::SetProgressRange(int xpvLim)
	{

	SendMessage(hwndPrg, PBM_SETRANGE, 0, (LPARAM)MAKELPARAM(0, xpvLim));
	
}

/* ------------------------------------------------------------ */
/***	VWAPP::SetProgressPos
**
**	Synopsis:
**		SetProgressPos(xpvPos)
**
**	Input:
**		xpvPos		- progress bar position to set
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Set the position of the progress bar.
*/

void
VWAPP::SetProgressPos(int xpvPos)
	{
	
	SendMessage(hwndPrg, PBM_SETPOS, xpvPos, 0);
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

