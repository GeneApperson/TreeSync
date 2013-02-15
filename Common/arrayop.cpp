/************************************************************************/
/*                                                                      */
/*  arrayop.cpp  --  Array Manipulation Routines						*/
/*                                                                      */
/************************************************************************/
/*	Author: 	Gene Apperson											*/
/*	Copyright 2003, 2004 Digilent Inc.									*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*	This module contains some array manipation routines.				*/
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*	10/06/2004(GeneA): created by copying functions from genutil.cpp	*/
/*	06/04/2009(JoshP):  Upgraded to VS2008								*/
/*                                                                      */
/************************************************************************/


/* ------------------------------------------------------------ */
/*					Include File Definitions					*/
/* ------------------------------------------------------------ */
#define	_CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include	<windows.h>
#include	"dtypes.h"
#include	"dutil.h"

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
/*			Array Manipulation Routines							*/
/* ------------------------------------------------------------ */
/***	IdwOfDw
**
**	Synopsis:
**		idw = IdwOfDw(dw, rgdw, cdwMac)
**
**	Input:
**		dw		- word to search for
**		rgdw	- array to search
**		cdwMac	- number of elements in the array
**
**	Output:
**		returns index within array of specified value
**
**	Errors:
**		returns -1 if not found
**
**	Description:
**		This routine will search the specified array of DWORDS
**		for the first occurance of the specified value. It
**		returns the index location of the first occurence
**		found.
*/

int
IdwOfDw(DWORD dw, DWORD * rgdw, int cdwMac)
	{
	int		idw;

	for (idw = 0; idw < cdwMac; idw++) {
		if (rgdw[idw] == dw) {
			return idw;
		}
	}

	return -1;

}

/* ------------------------------------------------------------ */
/***	InsDwAtIdw
**
**	Synopsis:
**		void InsDwAtIdw(dw, rgdw, idw, cdwMac)
**
**	Input:
**		dw		- word to insert
**		rgdw	- array to insert into
**		idw		- location in rgdw
**		cdwMac	- number of elements in rgdw
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This routine will insert the specified dword into
**		the dword array at the indicated location.
*/

void
InsDwAtIdw(DWORD dw, DWORD * rgdw, int idw, int cdwMac)
	{
	int		i;

	if ((idw < 0) || (idw > cdwMac)) {
		return;
	}

	for (i = cdwMac; i > idw; i--) {
		rgdw[i] = rgdw[i-1];
	}

	rgdw[idw] = dw;

}

/* ------------------------------------------------------------ */
/***	DelDwAtIdw
**
**	Synopsis:
**		void DelDwAtIdw(rgdw, idw, cdwMac)
**
**	Input:
**		rgdw	- array to insert into
**		idw		- location in rgdw
**		cdwMac	- number of elements in rgdw
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This routine will delete the specified dword from the
**		dword array at the indicated location.
*/

void
DelDwAtIdw(DWORD * rgdw, int idw, int cdwMac)
	{
	int		i;

	if ((idw < 0) || (idw >= cdwMac)) {
		return;
	}

	for (i = idw; i < cdwMac-1; i++) {
		rgdw[i] = rgdw[i+1];
	}

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

