/**
 * \file MatlabTemplateComponent.cpp
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
 * Implementation of a template component used to read/write data to
 * a matlab script.
 */

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "MatlabTemplateComponent.h"
#include "irisapi/TypeVectors.h"

using namespace std;

namespace iris
{
	// export library symbols
	IRIS_COMPONENT_EXPORTS(PNComponent, MatlabTemplateComponent);

    MatlabTemplateComponent::MatlabTemplateComponent(string name):
        TemplatePNComponent<MatlabTemplateComponent>(name, "matlabtemplate", "A component which interfaces to a Matlab script", "Paul Sutton", "0.1")
		,matlab_input(NULL)
		,matlab_output(NULL)
    {
        //format:        (name,   description,     default,   dynamic, parameter, list/Interval)
        registerParameter("scriptName", "The Matlab script to be called", "Plot(matlab_input)", false, x_scriptName);
		registerParameter("hasOutput", "Does this component output data?", "false", false, x_hasOutput);
		registerParameter("passThrough", "Does this component pass data through?", "false", false, x_passThrough);

        //Initialize variables
    }

	MatlabTemplateComponent::~MatlabTemplateComponent()
	{
		mxDestroyArray(matlab_input);
		mxDestroyArray(matlab_output);
	}

    void MatlabTemplateComponent::registerPorts()
    {
        //This component accepts all the Iris data types
        vector<int> validTypes = convertToTypeIdVector<IrisDataTypes>();

        //format:        (name, vector of valid types)
        registerInputPort("input1", validTypes);

		if(x_hasOutput || x_passThrough)
		{
			registerOutputPort("output1", validTypes);
		}
    }

    map<string, int> MatlabTemplateComponent::calculateOutputTypes(map<string, int> inputTypes)
    {
        //Output type is the same as input type
        map<string, int> outputTypes;

		if(x_hasOutput || x_passThrough)
		{
			outputTypes["output1"] = inputTypes["input1"];
		}

        return outputTypes;
    }

    template <class Tin, class Tout>
    void MatlabTemplateComponentImpl<Tin,Tout>::initialize()
    {
		inBuf = castToType<T>(inputBuffers[0]);
		if(x_hasOutput || x_passThrough)
		{
			outBuf = castToType<T>(outputBuffers[0]);
		}

		//Open Matlab Engine
		if(!matlab.Open(""))
		{
			throw ResourceNotFoundException("Failed to start Matlab engine");
		}
		
		//Clear Matlab WorkSpace
		matlab.EvalString("clear all;");
		
		//Write the matlab command
		command = "matlab_output = ";
		command += x_scriptName;
		command += ";";

		//Setup an initial mxArray
		matlab_input = mxCreateDoubleMatrix(1, 10, mxREAL);
    }

    template <class Tin, class Tout>
    void MatlabTemplateComponentImpl<Tin,Tout>::process()
    {
		//Get the input buffer
        inBuf->getReadData(readDataSet);

		//Check dimensions of our matlab matrix
		const mwSize* dims = mxGetDimensions(matlab_input);
		size_t s = readDataSet->data.size();
		if(s != dims[0])
		{
			mxDestroyArray(matlab_input);
			if(TypeInfo<T>::isComplex)
			{
				matlab_input = mxCreateDoubleMatrix(1, s, mxCOMPLEX);
			}
			else
			{
				matlab_input = mxCreateDoubleMatrix(1, s, mxREAL);
			}
		}

		//Copy data into our matlab matrix
		copyInData(readDataSet->data, matlab_input);

		//bug in matlab windows - has to be reopened every time
		#ifdef _WIN32 // _WIN32 is defined by all Windows 32 compilers, but not by others.
			if(!matlab.Open(""))
			{
				throw ResourceNotFoundException("Failed to start Matlab engine");
			}
		#endif

		//Send data to matlab
		matlab.PutVariable("matlab_input", matlab_input);
		
		//Use OutputBuffer to capture MATLAB output
		memset(buffer, 0, 256 * sizeof(char));
		matlab.OutputBuffer(buffer, 256);

		//Process in Matlab
		matlab.EvalString(command.c_str());
		
		//The evaluate string returns the result into the output buffer
		if (buffer[0] != 0)
		{
			LOG(LWARNING) << buffer;
		}

		//Output data if required
		if(x_hasOutput || x_passThrough)
		{
			if(x_passThrough)
			{
				//Get a write data set
				outBuf->getWriteData(writeDataSet, readDataSet->data.size());
				//Copy data through 
				copy(readDataSet->data.begin(), readDataSet->data.end(), writeDataSet->data.begin());
				writeDataSet->sampleRate = readDataSet->sampleRate;
				writeDataSet->timeStamp = readDataSet->timeStamp;
			}
			else
			{
				//Get the matlab output
				mxArray* matlab_output = matlab.GetVariable("matlab_output");
				size_t m = mxGetM(matlab_output);
				size_t n = mxGetN(matlab_output);
				//Get a write data set of the correct size and copy data
				outBuf->getWriteData(writeDataSet, m>n?m:n);
				copyOutData(matlab_output, writeDataSet->data);
			}

			//Release write data set
			outBuf->releaseWriteData(writeDataSet);
		}

        //Release read data set
        inBuf->releaseReadData(readDataSet);
		
    }

	template <class Tin, class Tout>
    MatlabTemplateComponentImpl<Tin,Tout>::~MatlabTemplateComponentImpl()
	{

	}

} /* namespace iris */
