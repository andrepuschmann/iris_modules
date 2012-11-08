/*
 * This file is part of Iris 2.
 *
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin.
 * All rights reserved.
 *
 */

/**
 * \file Matlab.h
 *
 * 	This Class wraps the matlab engine - mainly used in the matlab component
 *
 *  Created on: May-2007
 *  Created by: hlahlou & suttonp
 *  $Revision: 865 $
 *  $LastChangedDate: 2010-03-17 10:33:49 +0000 (Wed, 17 Mar 2010) $
 *  $LastChangedBy: suttonp $
 *
 */

#ifndef MATLAB_H_
#define MATLAB_H_

#include "engine.h"

class Matlab 
{
public:
	Matlab()
	{
		pEng=NULL;
	};
	
	virtual ~Matlab()
	{
		if (pEng!=NULL)
			Close();
	};
	
	bool Open(const char* StartCmd)
	{
		pEng=engOpen(StartCmd);
		return (pEng != NULL);
	};
	
	int Close()
	{
		int Result=engClose(pEng);
		if (Result==0)	
			pEng=NULL;

		return Result;
	};
	
	int EvalString(const char* string)
	{
		return (engEvalString(pEng, string));
	};
	
	mxArray* GetVariable(const char* name)
	{
		return (engGetVariable(pEng, name));
	};
	
	int GetVisible(bool* value)
	{
		return (engGetVisible(pEng, value));
	};
	
	void OpenSingleUse(const char *startcmd, void *dcom, int *retstatus)
	{
		pEng=engOpenSingleUse(startcmd, dcom, retstatus);
	};
	
	int OutputBuffer(char *p, int n)
	{
		return (engOutputBuffer(pEng, p, n));
	};
	
	
	int SetVisible(bool value)
	{
		return (engSetVisible(pEng, value));
	};
	
	int PutVariable(const char *name, const mxArray *mp)
	{
		return (engPutVariable(pEng, name, mp));
	};

protected:
	Engine* pEng;
	
}; /* class Matlab */

#endif /* MATLAB_H_ */
