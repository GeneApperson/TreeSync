/************************************************************************/
/*                                                                      */
/*  memop.cpp  --  General Purpose Memory Buffer Manipulation Routines	*/
/*                                                                      */
/************************************************************************/
/*	Author: 	Gene Apperson											*/
/*	Copyright 2004, Digilent Inc.										*/
/************************************************************************/
/*  Module Description: 												*/
/*                                                                      */
/*	This module contains general purpose utility functions for use in	*/
/*	the Digilent utility library.										*/
/*                                                                      */
/************************************************************************/
/*  Revision History:													*/
/*                                                                      */
/*	10/06/2004(GeneA): created from genutil.cpp							*/
/*                                                                      */
/************************************************************************/


/* ------------------------------------------------------------ */
/*					Include File Definitions					*/
/* ------------------------------------------------------------ */
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include	<windows.h>

#include	"../inc/dtypes.h"
#include	"../inc/dutil.h"

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
/*			Array and Memory Manipulation Routines				*/
/* ------------------------------------------------------------ */
/***	CopyMem
**
**	Synopsis:
**		void CopyMem(cb, pvdst, pvsrc)
**
**	Input:
**		cb		- number of bytes to copy
**		pvdst	- pointer to the destination buffer
**		pvsrc	- pointer to the source buffer
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This will copy memory bytes from one location to another.
**		It does not check for buffer overlap.
*/

void
CopyMem(int cb, BYTE * pvDst, const BYTE * pvSrc)
	{
	int		ib;

	for (ib=0; ib<cb; ib++) {
		*pvDst++ = *pvSrc++;
	}

}

/* ------------------------------------------------------------ */
/***	MoveMem
**
**	Synopsis:
**		void MoveMem(cb, pvDst, pvSrc)
**
**	Input:
**		cb		- number of bytes to move
**		pbDst	- pointer to destination
**		pbSrc	- pointer to source bytes
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This routine will move a block of bytes from one location
**		in memory to another. It checks for and handles overlap
**		between the source and the destination.
*/

void
MoveMem(int cb, BYTE * pbDst, BYTE * pbSrc)
	{
	int		ib;

	if ((pbSrc == pbDst) || (cb == 0)) {
		return;
	}

	if ((pbDst > pbSrc) && (pbDst < pbSrc+cb)) {
		/* This is the case where the start of the destination
		** region is within the source region. In this case, we
		** need to copy from the end of the buffers down.
		*/
		pbDst = pbDst + cb - 1;
		pbSrc = pbSrc + cb - 1;
		for (ib = 0; ib < cb; ib++) {
			*pbDst-- = *pbSrc--;
		}
	}
	else {
		/* This is the case where the start of the source
		** region is within the destination region, or the
		** two regions don't overlap. Start at the bottom
		** and copy up.
		*/
		for (ib = 0; ib < cb; ib++) {
			*pbDst++ = *pbSrc++;
		}
	}

}

/* ------------------------------------------------------------ */
/***	FillMem
**
**	Synopsis:
*		void FillMem(cb, pb, bVal)
**
**	Input:
**		cb		- number of bytes to fill
**		pb		- address of buffer it fill
**		bVal	- byte value to fill
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This routine will initialize the specified memory block
**		with the specified byte value.
*/

void
FillMem(int cb, BYTE * pb, BYTE bVal)
	{
	int			ib;

	for (ib=0; ib<cb; ib++) {
		*pb++ = bVal;
	}

}

/* ------------------------------------------------------------ */
/***	FillMem
**
**	Synopsis:
*		void FillMem(cw, pw, wVal)
**
**	Input:
**		cw		- number of words to fill
**		pw		- address of buffer it fill
**		wVal	- word value to fill
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This routine will initialize the specified memory block
**		with the specified word value.
*/

void
FillMem(int cw, WORD * pw, WORD wVal)
	{
	int			iw;

	for (iw=0; iw<cw; iw++) {
		*pw++ = wVal;
	}

}

/* ------------------------------------------------------------ */
/***	FillMem
**
**	Synopsis:
*		void FillMem(cd, pd, dVal)
**
**	Input:
**		cd		- number of dwords to fill
**		pd		- address of buffer to fill
**		dVal	- dword value to fill
**
**	Output:
**		none
**
**	Errors:
**		none
**
**	Description:
**		This routine will initialize the specified memory block
**		with the specified byte value.
*/

void
FillMem(int cd, DWORD * pd, DWORD dVal)
	{
	int			id;

	for (id=0; id<cd; id++) {
		*pd++ = dVal;
	}

}

/* ------------------------------------------------------------ */
/***	FCmpMem
**
**	Synopsis:
**		bool FCmpMem
**
**	Input:
**		cb		- number of bytes to copy
**		pvdst	- pointer to the destination buffer
**		pvsrc	- pointer to the source buffer
**
**	Output:
**		Returns fTrue if both buffers are identical
**
**	Errors:
**		none
**
**	Description:
**		This will compare two memory buffers for differnces
*/

bool
FCmpMem(int cb, BYTE * pvDst, BYTE * pvSrc)
	{
	int		ib;

	for (ib=0; ib<cb; ib++) {
		if (*pvDst++ != *pvSrc++) {
			return fFalse;
		}
	}
	return fTrue;
}

/* ------------------------------------------------------------ */
/***	DwPackBytes
**
**	Synopsis:
**		dwVal = DwPackBytes(cb, rgb)
**
**	Parameters:
**		cb		- number of bytes to pack
**		rgb		- array of bytes to pack		
**
**	Return Value:
**		Returns dword containing packet bytes
**
**	Errors:
**		none
**
**	Description:
**		Convert input array to packed dword.
*/

DWORD
DwPackBytes(int cb, BYTE * rgb)
	{
	DWORD	dw;
	int		ib;
	
	dw = 0;
	for (ib	= 0; ib < cb; ib++) {
		dw += rgb[ib] << (8*ib);
	}
	return dw;
}

/* ------------------------------------------------------------ */
/***	UnpackBytes
**
**	Synopsis:
**		UnpackBytes(cb, rgb, dw)
**
**	Parameters:
**		cb		- number of bytes
**		rgb		- array to receive unpacked bytes
**		dw		- dword containing bytes to unpack
**
**	Return Value:
**		none
**
**	Errors:
**		none
**
**	Description:
**		Convert dword into array of bytes
*/

void
UnpackBytes(int cb, BYTE * rgb, DWORD dw)
	{
	int		ib;
	
	for (ib = 0; ib < cb; ib++) {
		rgb[ib] = (BYTE)((dw >> (8*ib)) & 0xFF);
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

