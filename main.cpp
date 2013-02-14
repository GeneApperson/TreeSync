/************************************************************************/
/*																		*/
/*	main.cpp	--	Folder Tree Synchronization Application				*/
/*																		*/
/************************************************************************/
/*	Author: 	Gene Apperson											*/
/*	Copyright 2006, Digilent Inc.										*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	09/18/2006(GeneA): created											*/
/*																		*/
/************************************************************************/


/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include	<windows.h>
#include	<commctrl.h>

#include	"resource.h"
#include	"config.h"
#include	"gendefs.h"
#include	"appdefs.h"
#include	"dassert.h"

#include	"app.h"
#include	"view.h"
#include	"vwapp.h"

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

APP			app;	//single global instance of APP object

/* ------------------------------------------------------------ */
/*				Local Variables									*/
/* ------------------------------------------------------------ */

__SetCurFile;

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */
/***	WinMain
**
**	Synopsis:
**		Windows main program entry point
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

int APIENTRY 
WinMain(HINSTANCE hinstCur, HINSTANCE hinstPrev, LPSTR szCmdLine, int cmdShow)
	{
    MSG			msg;
	VWAPP *		pvwapp;

    szCmdLine; // This will prevent 'unused formal parameter' warnings

	/* Perform application starup initialization.
	*/
	if (!app.FInit(hinstCur)) {
		return -1;
	}

	if (!app.FCreateView(cmdShow)) {
		return -1;
	}

	pvwapp = app.Pvwapp();
	pvwapp->DoStartup();

	/* Process messages for this program.
	*/
    while (GetMessage(&msg, NULL, 0, 0)) {
		if (!IsDialogMessage(app.HdlgModeless(), &msg)) {
			if (!TranslateAccelerator(pvwapp->Hwnd(), app.HaclApp(), &msg)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
    }

	/* Free any resources allocated when we initialized.
	*/
	app.DestroyView();
	app.Terminate();

    return (msg.wParam);

}

/* ------------------------------------------------------------ */
/***	ProcName
**
**	Synopsis:
**
**	Parameters:
**
**	Return Values:
**
**	Errors:
**
**	Description:
**
*/

/* ------------------------------------------------------------ */

/************************************************************************/

