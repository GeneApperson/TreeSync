/************************************************************************/
/*																		*/
/*	app.h	--	Interface Declarations for APP Object					*/
/*																		*/
/************************************************************************/
/*	Author:		Gene Apperson											*/
/*	Copyright 2010, Gene Apperson										*/
/************************************************************************/
/*  File Description:													*/
/*																		*/
/*	Define the interface to the APP object class. This object class is	*/
/*	used to implement the top level application behavior. There is only	*/
/*	a single static instance of this object ever instantiated.			*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	12/28/2010(GeneA): created											*/
/*																		*/
/************************************************************************/

#if !defined(APP_INC)
#define APP_INC

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					General Type Declarations					*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Object Class Declarations					*/
/* ------------------------------------------------------------ */

class FLDR;
class CMDX;

class APP {
private:

	bool	FGetDocumentPaths();
		
protected:

public:
			APP();
		   ~APP();
	bool	FInit();
	void	Terminate();
	
	bool	FParseParameters(int cszArg, char * rgszArg[]);
	void	ShowUseage();
	void	Run();
	void	ProcessCommandLine();
	void	ProcessCommandFile(char * szFile);
	bool	FFindCommandFile(char * szFile, char * szCmd);
	bool	FDoAction(FLDR * pfldr);
	void	PrintMessage(int idMsg, char * szMsg);
	void	PrintStatistics();

};

/* ------------------------------------------------------------ */
/*					Variable Declarations						*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Procedure Declarations						*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */

#endif

/************************************************************************/
