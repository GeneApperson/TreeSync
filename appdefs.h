/************************************************************************/
/*                                                                      */
/*	appdefs.h  --	Application Specific Configuration Declarations		*/
/*                                                                      */
/************************************************************************/
/*	Author:		Gene Apperson											*/
/*	Copyright 2004, 2005, Digilent Inc.									*/
/************************************************************************/
/*  File Description:                                                   */
/*                                                                      */
/*	This module contains definitions for configuration constants that	*/
/*	are specific to this application.									*/
/*                                                                      */
/************************************************************************/
/*  Revision History:                                                   */
/*                                                                      */
/*	09/08/2004(GeneA): created											*/
/*	01/12/2005(GeneA): copied and customized for TreeSync application	*/
/*                                                                      */
/************************************************************************/

#if !defined(_INC_APPDEFS)
#define _INC_APPDEFS

/* ------------------------------------------------------------ */
/*				Application Control Related Definitions			*/
/* ------------------------------------------------------------ */

const int	verMajor = 0;
const int	verMinor = 1;

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */

const int cchFileNameMax	= MAX_PATH+1;
const int cchStringMax		= 256;
const int cchCaptionMax		= 256;
const int cchNameMax		= 32;
const int cchTempMax		= 128;

const int clnRptMax			= 128;


const int	sptCharRpt		= 8;	//point size of text in status report window
const int	sptCharVwlst	= 9;	//point size of text in VWLST controls in Fuses
									// and Lock Bits windows


/* Application wide error messages
*/
const int	errNoError					= 0;
const int	errAppInternalError			= 1;
const int	errAppSysKeyNotFound		= 2;
const int	errAppKeyNotFound			= 3;
const int	errAppValueNotFound			= 4;
const int	errAppOutOfMemory			= 5;
const int	errAppAlreadyExits			= 6;
const int	errAppDeviceNotAvailable	= 7;

/* ------------------------------------------------------------ */
/*						Type Declarations						*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*						Variable Declarations					*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */

#endif

/************************************************************************/
