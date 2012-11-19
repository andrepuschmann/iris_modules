/**
 * \file FileRawWriterTemplateComponent.cpp
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
 * Implementation of a template sink component used to write data to file.
 */

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "FileRawWriterTemplateComponent.h"
#include "irisapi/TypeVectors.h"

using namespace std;

namespace iris
{
	// export library symbols
	IRIS_COMPONENT_EXPORTS(PNComponent, FileRawWriterTemplateComponent);

    FileRawWriterTemplateComponent::FileRawWriterTemplateComponent(string name):
        TemplatePNComponent<FileRawWriterTemplateComponent>(name, "filerawwritertemplate", "A templated filewriter", "Paul Sutton & Jorg Lotze", "0.2")
    {
        //format:        (name,   description,     default,   dynamic, parameter, list/Interval)
        registerParameter("fileName", "The file to read", "temp.bin", false, x_fileName);

        //Initialize variables
    }

    void FileRawWriterTemplateComponent::registerPorts()  //confine to uint8_t input for the moment
    {
        //Register all ports
        vector<int> validTypes = convertToTypeIdVector<IrisDataTypes>();
        //format:        (name, vector of valid types)
        registerInputPort("input1", validTypes);
    }

    map<string, int> FileRawWriterTemplateComponent::calculateOutputTypes(map<string, int> inputTypes)
    {
        //No output
        map<string, int> outputTypes;
        return outputTypes;
    }

    template <class Tin, class Tout>
    void FileRawWriterTemplateComponentImpl<Tin,Tout>::initialize()
    {
        using namespace std;


        hOutFile.open(x_fileName.c_str(), ios::out|ios::binary);
        if (hOutFile.fail() || hOutFile.bad() || !hOutFile.is_open())
        {
            cout << "Could not open file " << x_fileName << " for writing." << endl;
            //TODO: Throw an exception
        }
    }

    template <class Tin, class Tout>
    void FileRawWriterTemplateComponentImpl<Tin,Tout>::process()
    {
        inBuf = castToType<T>(inputBuffers[0]);

       //Get the input buffer
        inBuf->getReadData(readDataSet);

        //Write to file
        hOutFile.write((char*)&readDataSet->data[0], (streamsize)readDataSet->data.size()*sizeof(T));

        //Release data set
        inBuf->releaseReadData(readDataSet);
    }

	template <class Tin, class Tout>
    FileRawWriterTemplateComponentImpl<Tin,Tout>::~FileRawWriterTemplateComponentImpl()
	{
		hOutFile.close();
	}

} /* namespace iris */
