/************************************************************************/
/*																		*/
/*	main.cpp	--	Tree Sync Utility Main Program File					*/
/*																		*/
/************************************************************************/
/*	Author: 	Gene Apperson											*/
/*	Copyright 2010, Gene Apperson										*/
/************************************************************************/
/*  Module Description: 												*/
/*																		*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	12/28/2010(GeneA): created											*/
/*																		*/
/************************************************************************/

#define	WIN32_LEAN_AND_MEAN

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include	<windows.h>
#include	<stdio.h>
#include	<stdlib.h>

#include	"../inc/dtypes.h"
#include	"../inc/dutil.h"
#include	"../inc/dassert.h"

#include	"config.h"
#include	"appdefs.h"

#include	"apst.h"
#include	"app.h"
#include	"cmdx.h"

/* ------------------------------------------------------------ */
/*				Local Type and Constant Definitions				*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

APP		app;		//single global instande of the APP object
APST	apst;		//single global instance of the APST object
CMDX	cmdx;

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

void
main(int cszArg, char * rgszArg[]) {

	if (!app.FInit()) {
		printf("Unable to initialize program\n");
		goto lExit;
	}

	/* Parse the command line parameters. If there is a syntax error,
	** or if the user requests it, print the useage info.
	*/	
	if (!app.FParseParameters(cszArg, rgszArg) || apst.FShowUseage()) {
		app.ShowUseage();
		goto lExit;
	}
	
	/* Print the software version number if requested.
	*/
	if (apst.FShowVer()) {
		printf("Version: %d.%d.%d\n", verMajor, verMinor, verBuild);	
		goto lExit;
	}		

	/* Perform the requested operations.
	*/	
	app.Run();	

lExit:	
	app.Terminate();
	exit(apst.StProg());
	
}

/* ------------------------------------------------------------ */
/*				XXXX Object Class Implementation				*/
/* ------------------------------------------------------------ */

/***	ProcName
**
**	Parameters:
**
**	Return Value:
**
**	Errors:
**
**	Description:
**
*/

/* ------------------------------------------------------------ */

/************************************************************************/

