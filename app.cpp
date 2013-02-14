/************************************************************************/
/*																		*/
/*	app.cpp	--	Implementation for APP Object Class						*/
/*																		*/
/************************************************************************/
/*	Author: 	Gene Apperson											*/
/*	Copyright 2004, 2005, Digilent Inc.									*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*	APP object class implementation. The APP object class is used to	*/
/*	manage global application state. There is a single global APP		*/
/*	object instance in the program and all manipulation of application	*/
/*	global state is made through accessor methods on this object.		*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	09/08/2004(GeneA): created											*/
/*	01/12/2005(GeneA): copied and customized for AvrProg application	*/
/*	05/03/2004(GeneA): Added support for USB-SPI interface.	Added path	*/
/*		string management and dynamic loading of DPCUTIL DLL			*/
/*	05/06/2004(GeneA): Improved error handling in parsing functions		*/
/*	06/03/2005(GeneA): Updated to use new Digilent convention for the	*/
/*		registry value to give the path to the digilent common folders	*/
/*																		*/
/************************************************************************/

#define _WIN32_DCOM

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include <windows.h>
#include <commctrl.h>

#include "config.h"
#include "gendefs.h"
#include "resource.h"
#include "appdefs.h"
#include "dassert.h"
#include "winutil.h"

#include "app.h"
#include "view.h"
#include "vwapp.h"

/* ------------------------------------------------------------ */
/*				Local Type Definitions							*/
/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

extern APP		app;

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


/* ------------------------------------------------------------ */
/*				APP Object Class Implementation					*/
/* ------------------------------------------------------------ */
/***	APP:APP
**
**	Synopsis:
**		APP()
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
**		Default constructor
*/

APP::APP()
	{

	flg.Reset();
	hdlgModeless = NULL;
	haclApp = NULL;

}

/* ------------------------------------------------------------ */
/***	APP:~APP
**
**	Synopsis:
**		~APP()
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
**		Default destructor
*/

APP::~APP()
	{

	AssertMsg(szMsg, "APP object not properly terminated before destruction");


}

/* ------------------------------------------------------------ */
/***	APP::FInit
**
**	Synopsis:
**		fSuccess = FInit(HINST hinst)
**
**	Parameters:
**		hinst	- instance handle for this instance of the program
**
**	Return Values:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not.
**
**	Description:
**		Perform application startup initialization.
*/

bool
APP::FInit(HINSTANCE hinst)
	{
	INITCOMMONCONTROLSEX	iccex;
	int			err;

	hinstApp = hinst;

	/* Init general system resources that we are going to be using.
	*/
	iccex.dwSize = sizeof(iccex);
	iccex.dwICC  = ICC_WIN95_CLASSES|ICC_PROGRESS_CLASS;
	if (!InitCommonControlsEx(&iccex)) {
		return fFalse;
	}
	
	/* Init COM support so that we can use the XML parser.
	*/
	if (CoInitializeEx(NULL, COINIT_APARTMENTTHREADED) != S_OK) {
		return fFalse;
	}

	/* Load any resources needed initially.
	*/
	hmenuApp = LoadMenu(hinst, MAKEINTRESOURCE(idrMenuApp));
	haclApp = LoadAccelerators(hinst, MAKEINTRESOURCE(idrAclApp));

	/* Initialize the object class used for the application main
	** window.
	*/
	if (!VWAPP::FInitVWAPP()) {
		return fFalse;
	}

	/* Initialize other VIEW window based object classes.
	*/
	
	
	/* All done.
	*/
	return fTrue;

}

/* ------------------------------------------------------------ */
/***	APP::Terminate
**
**	Synopsis:
**		Terminate()
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
**		Free resources used by the application in preparation
**		for application termination.
*/

void
APP::Terminate()
	{

	CoUninitialize();
	
	VWAPP::TerminateVWAPP();

}

/* ------------------------------------------------------------ */
/***	APP::FCreateView
**
**	Synopsis:
**		fSuccess = FCreateView(idShow)
**
**	Parameters:
**		idShow		- display style for initial program window
**
**	Return Values:
**		none
**
**	Errors:
**		Returns fTrue if successful, fFalse if not
**
**	Description:
**		Create the main application window and associated view
**		object.
*/

bool
APP::FCreateView(int idShow)
	{

	if ((pvwapp = new VWAPP()) == NULL) {
		return fFalse;
	}

	if (!pvwapp->FInit(NULL)) {
		delete pvwapp;
		return fFalse;
	}

	if (!pvwapp->FCreateHwnd(CW_USEDEFAULT, CW_USEDEFAULT,
							 CW_USEDEFAULT, CW_USEDEFAULT,
							 NULL)) {
		goto lErrorExit;
	}

	return fTrue;

lErrorExit:
	if (pvwapp != NULL) {
		pvwapp->Terminate();
		delete pvwapp;
	}
	return fFalse;

}

/* ------------------------------------------------------------ */
/***	APP::DestroyView
**
**	Synopsis:
**		DestroyView()
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
**		Destroy the main application window and associated
**		view object in preparation for application termination.
*/

void
APP::DestroyView()
	{

	if (pvwapp != NULL) {
		pvwapp->Terminate();
		delete pvwapp;
		pvwapp = NULL;
	}

}

/* ------------------------------------------------------------ */
/***	APP::ViewClosing
**
**	Synopsis:
**		ViewClosing()
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
**		This function is called by the VWAPP object when it is
**		being terminated to inform the APP that the VWAPP is
**		going away.
*/

void
APP::ViewClosing()
	{

	pvwapp = NULL;

}

/* ------------------------------------------------------------ */
/*				Miscellaneous Utility Functions					*/
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

