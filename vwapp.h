/************************************************************************/
/*                                                                      */
/*	vwapp.h  --	Declarations for Application Main Window View Object	*/
/*                                                                      */
/************************************************************************/
/*	Author:		Gene Apperson											*/
/*	Copyright 2006, Digilent Inc.										*/
/************************************************************************/
/*  File Description:                                                   */
/*                                                                      */
/*                                                                      */
/************************************************************************/
/*  Revision History:                                                   */
/*                                                                      */
/*	09/18/2006(GeneA): created											*/
/*                                                                      */
/************************************************************************/

#if !defined(_INC_VWAPP)
#define	_INC_VWAPP

#if !defined(_INC_VIEW)
#include "view.h"
#endif

#if !defined(_INC_DLGVIEW)
#include "dlgview.h"
#endif

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */

const int	idcTab = 0x1001;
const int	idcRpt = 0x1002; 
const int	idcPrg = 0x1003;

const int	dxcoDlgMarginLeft = 5;
const int	dxcoDlgMarginRight = 5;
const int	dycoDlgMarginTop = 35;
const int	dycoDlgMarginBottom = 5;

const int	dxcoTabMarginLeft = 5;
const int	dxcoTabMarginRight = 5;
const int	dycoTabMarginTop = 5;	//space between top of tab control and caption
const int	dycoTabMarginBottom = 10;

const int	dycoRptMarginBottom = 10;
const int	dycoPrgMarginBottom = 10;

const int	ctabMax = 5;
const int	ipdlgTabProgram		= 0;
const int	ipdlgTabDevSettings = 1;
const int	ipdlgTabFuses		= 2;
const int	ipdlgTabLockBits	= 3;
const int	ipdlgTabPgmSettings = 4;

/* Flag settings for the object status flag bits.
*/

/* ------------------------------------------------------------ */
/*					Type Declarations							*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*					Object Class Declarations					*/
/* ------------------------------------------------------------ */

class VWAPP : public VIEW {
	friend class APP;
	friend int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);

private:
	HWND		hwndTab;
	HWND		hwndRpt;
	HWND		hwndPrg;
	DLGVIEW *	rgpdlgTabs[ctabMax];
	int			ipdlgCur;
	HFONT		hfntRpt;
	int			dycoCharRpt;	//height of font used
	int			dxcoCharRpt;	//width of font used

	bool		FLoadTabDialogs();
	bool		FCreateTabControl();
	bool		FCreateRptControl();
	bool		FCreatePrgControl();
	bool		FAdjustWindowSize();

protected:
	virtual bool	FCreateHwnd(int xco, int yco, int dxco, int dyco, VIEW * pviewParent);
	virtual void	DoClose();
	virtual void	DoCommand(UINT idCmd, int wNotifyCode, HWND hwndCtl);
	virtual void	DoNotify(int idCtl, NMHDR * pnmhdr);
	virtual void	DoSize(int dxcoNew, int dycoNew);
	virtual void	DoMove(int xcoNew, int ycoNew);
	virtual void	DoPaint(HDC hdc, PAINTSTRUCT * pps);

public:
	static bool	FInitVWAPP();
	static void	TerminateVWAPP();

					VWAPP();
				   ~VWAPP();
	bool			FInit(VIEW * pviewParent);
	virtual void	Terminate();
	void			DoStartup();
	bool			FConnectAvrDevice();
	void			DisconnectAvrDevice();
	void			PutStatusMessage(char * sz);
	void			SetProgressRange(int xpvLim);
	void			SetProgressPos(int xpvPos);
	HFONT			HfntRpt() const;
	int				DycoCharRpt() const { return dycoCharRpt; };
	int				DxcoCharRpt() const { return dxcoCharRpt; };
};

/* ------------------------------------------------------------ */
/*					Procedure Declarations						*/
/* ------------------------------------------------------------ */

inline VWAPP *	PvwappFromHwnd(HWND hwnd) {
	return (VWAPP *)GetWindowLong(hwnd, GWL_USERDATA);
};

/* ------------------------------------------------------------ */

#endif

/************************************************************************/
