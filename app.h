/************************************************************************/
/*                                                                      */
/*	app.h  --	Application Global State Object Declarations			*/
/*                                                                      */
/************************************************************************/
/*	Author:		Gene Apperson											*/
/*	Copyright 2004, 2005 Digilent Inc.									*/
/************************************************************************/
/*  File Description:                                                   */
/*                                                                      */
/*	This header files contains declarations for the application global	*/
/*	state object class. There is a single instance of this object class	*/
/*	instantiated as a global object. This object class contains all		*/
/*	global state of the application.									*/
/*                                                                      */
/************************************************************************/
/*  Revision History:                                                   */
/*                                                                      */
/*	09/08/2004(GeneA): created											*/
/*	01/12/2005(GeneA): copied and customized for AvrProg application	*/
/*	05/03/2004(GeneA): Added support for USB-SPI interface.	Added path	*/
/*		string management and dynamic loading of DPCUTIL DLL			*/
/*                                                                      */
/************************************************************************/

#if !defined(_INC_APP)
#define _INC_APP

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*					Type Declarations							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*					Object Class Declarations					*/
/* ------------------------------------------------------------ */

class VWAPP;

class APP {
private:
	FLG			flg;
	HINSTANCE	hinstApp;
	VWAPP *		pvwapp;
	HMENU		hmenuApp;
	HACCEL		haclApp;
	HWND		hdlgModeless;
	
protected:
	bool		FInitPathStrings(int & err);
	bool		FGetDataPath(char * sz, int & err);
	bool		FGetAtmelPath(char * sz, int & err);

public:
				APP();
			   ~APP();
	bool		FInit(HINSTANCE hinst);
	void		Terminate();
	bool		FCreateView(int idShow);
	void		DestroyView();
	void		ViewClosing();
	HINSTANCE	HinstApp() const { return hinstApp; };
	VWAPP *		Pvwapp() { return pvwapp; };
	HMENU		HmenuApp() const { return hmenuApp; };
	HACCEL		HaclApp() const { return haclApp; };
	HWND		HdlgModeless() const { return hdlgModeless; };
	void		SetHdlgModeless(HWND hdlg) { hdlgModeless = hdlg; };
};

/* ------------------------------------------------------------ */
/*					 Procedure Declarations						*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */

#endif

/************************************************************************/
