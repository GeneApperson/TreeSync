/************************************************************************/
/*																		*/
/*	fldrmg.h	--	Interface Declarations for FLDRMG Object Class		*/
/*																		*/
/************************************************************************/
/*	Author:		Gene Apperson											*/
/*	Copyright 2010, Gene Apperson										*/
/************************************************************************/
/*  File Description:													*/
/*																		*/
/*	This file defines the interface for the FLDRMG object class. This	*/
/*	class is used to perform the processing on folders in the folder	*/
/*	heirarchy being processed.											*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	12/30/2010(GeneA): created											*/
/*																		*/
/************************************************************************/

#if !defined(FLDRMG_INC)
#define	FLDRMG_INC

/* ------------------------------------------------------------ */
/*					Include File Definitions					*/
/* ------------------------------------------------------------ */

#include	<list>
#include	<map>
#include	<string>

using namespace std;

/* ------------------------------------------------------------ */
/*					Miscellaneous Declarations					*/
/* ------------------------------------------------------------ */

const DWORD		flgFldrmgOwnBuf	= 0x00000001;

/* ------------------------------------------------------------ */
/*					General Type Declarations					*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*					Object Class Declarations					*/
/* ------------------------------------------------------------ */

class FLDR;

typedef list<FLDR *> LFLDR;
typedef map<string, int> MPST;

/* The FLDRMG class is used to maintain a list of FLDR objects.
*/
class FLDRMG {
private:
	FLG			flg;
	LFLDR		lfldrMain;
	LFLDR		lfldrCur;
	MPST		mpstXclExt;
	MPST		mpstXclFile;
	MPST		mpstXclDir;
	MPST		mpstXclPath;
	BYTE *		pbSigBuf;
	int			cbSigBuf;
	int			cfldrTotal;
	int			cfldrDepthMax;
	int			cntFileProcessed;
	int			cntFileCopy;
	int			cntFileDelete;
	int			cntDirCreated;
	int			cntXclFile;
	int			cntXclDir;
	int			cchPathMax;

public:
				FLDRMG();
			   ~FLDRMG();
	bool		FInit();
	bool		FInit(int cbBuf, BYTE * pbBuf);
	void		Terminate();

	bool		FProcess();
	void		BeginDirectory();
	void		EndDirectory();
	void		AddPfldr(FLDR * pfldr);
	FLDR *		PfldrGetNext();
	bool		FIsEmpty() { return lfldrMain.empty(); };
	void		ClearXclExt() { mpstXclExt.clear(); };
	void		ClearXclFile() { mpstXclFile.clear(); };
	void		ClearXclDir() { mpstXclDir.clear(); };
	void		ClearXclPath() { mpstXclPath.clear(); };
	void		AddXclExt(char * sz);
	void		AddXclFile(char * sz);
	void		AddXclDir(char * sz);
	void		AddXclPath(char * sz);
	bool		FMatchXclExt(char * sz);
	bool		FMatchXclFile(char * sz);
	bool		FMatchXclDir(char * sz);
	bool		FMatchXclPath(char * sz);
	void		FileCopied() { cntFileCopy += 1; };
	void		FileDeleted() { cntFileDelete += 1; };
	void		FileProcessed() { cntFileProcessed += 1; };
	void		DirCreated() { cntDirCreated += 1; };
	void		FileExcluded() { cntXclFile += 1; };
	void		DirExcluded() { cntXclDir += 1; };
	void		SetCchPath(int cch) { cchPathMax = cch > cchPathMax ? cch : cchPathMax; };

	void		PrintStat(FILE * fhOut);
	void		ClearStat();
	int			CfldrTotal() { return cfldrTotal; };
	int			CfldrDepthMax() { return cfldrDepthMax; };
	int			CntFileCopy() { return cntFileCopy; };
	int			CntFileDelete() { return cntFileDelete; };
	int			CntFileProcessed() { return cntFileProcessed; };
	int			CntDirCreated() { return cntDirCreated; };
	int			CntFileExcluded() { return cntXclFile; };
	int			CntDirExcluded() { return cntXclDir; };
	int			CchPathMax() { return cchPathMax; };
	int			CbSigBuf() { return cbSigBuf; };
	BYTE *		PbSigBuf() { return pbSigBuf; };
};

/* The FLDR object class is used to keep track of a folder to be
** processed and to perform the processing of that folder.
*/
class FLDR {
private:

protected:
	char		szPathSrc[MAX_PATH];
	char		szPathDst[MAX_PATH];

	virtual bool	FProcessDirectory(char * szDir) = 0;
	virtual bool	FProcessFile(char * szFile) = 0;

public:
				FLDR();
			   ~FLDR();
	bool		FInit();
	void		Terminate();

	void		SetPathSrc(char * sz) { SzCopy(szPathSrc, sz); };
	void		SetPathDst(char * sz) { SzCopy(szPathDst, sz); };
	void		FindFsSrc(char * sz);
	void		FindFsDst(char * sz);
	bool		FProcess();
};

/* Subclass of FLDR used for performing update of destination from source.
*/
class FLDS : public FLDR {
private:

protected:
	virtual bool	FProcessDirectory(char * szDir);
	virtual bool	FProcessFile(char * szFile);

public:
				FLDS();
			   ~FLDS();
	bool		FInit();
	void		Terminate();
};

/* Subclass of FLDR used for performing extraction from destination tree
*/
class FLDX : public FLDR {
private:
	char 		szPathXtr[MAX_PATH];

protected:
	virtual bool	FProcessDirectory(char * szDir);
	virtual bool	FProcessFile(char * szFile);
	
public:
				FLDX();
			   ~FLDX();
	bool		FInit();
	void		Terminate();
	void		SetPathXtr(char * sz) { SzCopy(szPathXtr, sz); };
	void		FindFsXtr(char * sz);
};

/* Subclass of FLDR used to delete files from destination not in source
*/
class FLDT : public FLDR {
private:

protected:
	virtual bool	FProcessDirectory(char * szDir);
	virtual bool	FProcessFile(char * szFile);
	
public:
				FLDT();
			   ~FLDT();
	bool		FInit();
	void		Terminate();
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
