/*
 * This file is part of Iris 2.
 * 
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the 
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin. 
 * All rights reserved.
 * 
 */



/**
 * \file FileRawWriterTemplateComponent.cpp
 *
 *  Created on: 19-Jan-2009
 *  Created by: lotzej
 *  $Revision: 714 $
 *  $LastChangedDate: 2009-10-20 20:14:02 +0100 (Tue, 20 Oct 2009) $
 *  $LastChangedBy: lotzej $
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
