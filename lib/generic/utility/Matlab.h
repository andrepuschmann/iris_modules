/**
 * \file Matlab.h
 * \version 1.0
 *
 * \section COPYRIGHT
 *
 * Copyright 2012 The Iris Project Developers. See the
 * COPYRIGHT file at the top-level directory of this distribution
 * and at http://www.softwareradiosystems.com/iris/copyright.html.
 *
 * \section LICENSE
 *
 * This file is part of the Iris Project.
 *
 * Iris is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Iris is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * A copy of the GNU Lesser General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 * \section DESCRIPTION
 *
 * This Class wraps the Matlab engine - mainly used in the matlab component.
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
