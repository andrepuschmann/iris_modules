/*
 * This file is part of Iris 2.
 * 
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the 
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin. 
 * All rights reserved.
 * 
 */



/**
 * \file FileRawWriterComponent.cpp
 *
 *  Created on: 19-Jan-2009
 *  Created by: suttonp
 *  $Revision: 1525 $
 *  $LastChangedDate: 2012-07-20 10:40:43 +0100 (Fri, 20 Jul 2012) $
 *  $LastChangedBy: suttonp $
 */

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "FileRawWriterComponent.h"
#include "irisapi/TypeVectors.h"
#include "utility/RawFileUtility.h"

using namespace std;

namespace iris
{
	// export library symbols
	IRIS_COMPONENT_EXPORTS(PNComponent, FileRawWriterComponent);

    FileRawWriterComponent::FileRawWriterComponent(string name):
        PNComponent(name, "filerawwriter", "A filewriter", "Paul Sutton", "0.1")
    {
        //format:        (name,   description,     default,   dynamic, parameter, list/Interval)
        registerParameter("filename", "The file to read", "temp.bin", false, x_fileName);
		registerParameter("endian", "Endianess of file (little|big|native)", "native", false, x_endian);

        //Initialize variables
    }

    void FileRawWriterComponent::registerPorts()  //confine to uint8_t input for the moment
    {
        //Register all ports
        vector<int> validTypes = convertToTypeIdVector<IrisDataTypes>();
        //format:        (name, vector of valid types)
        registerInputPort("input1", validTypes);
    }

    map<string, int> FileRawWriterComponent::calculateOutputTypes(map<string, int> inputTypes)
    {
        //No output
        map<string, int> outputTypes;
        return outputTypes;
    }

    void FileRawWriterComponent::initialize()
    {
        hOutFile.open(x_fileName.c_str(), ios::out|ios::binary);
        if (hOutFile.fail() || hOutFile.bad() || !hOutFile.is_open())
        {
            cout << "Could not open file " << x_fileName << " for writing." << endl;
            //TODO: Throw an exception
        }
    }

    void FileRawWriterComponent::process()
    {
        if( outputBuffers.size() != 0 || inputBuffers.size() != 1)
        {
            //Need to throw an exception here
        }

        switch(inputBuffers[0]->getTypeIdentifier())
        {
        case 0:
            writeBlock<uint8_t>();
            break;
        case 1:
            writeBlock<uint16_t>();
            break;
        case 2:
            writeBlock<uint32_t>();
            break;
        case 3:
            writeBlock<uint64_t>();
            break;
        case 4:
            writeBlock<int8_t>();
            break;
        case 5:
            writeBlock<int16_t>();
            break;
        case 6:
            writeBlock<int32_t>();
            break;
        case 7:
            writeBlock<int64_t>();
            break;
        case 8:
            writeBlock<float>();
            break;
        case 9:
            writeBlock<double>();
            break;
        case 10:
            writeBlock<long double>();
            break;
        case 11:
            writeBlock< complex<float> >();
            break;
        case 12:
            writeBlock< complex<double> >();
            break;
        case 13:
            writeBlock<complex< long double> >();
            break;
        default:
            break;
        }
    }

    template<typename T>
    void FileRawWriterComponent::writeBlock()
    {
       //Get the input buffer
        ReadBuffer< T >* inBuf = castToType<T>(inputBuffers[0]);
        DataSet<T>* readDataSet = NULL;
        inBuf->getReadData(readDataSet);

        //Write to file
		RawFileUtility::write(readDataSet->data.begin(), readDataSet->data.end(), hOutFile, x_endian);

        //Release data set
        inBuf->releaseReadData(readDataSet);
    }

	FileRawWriterComponent::~FileRawWriterComponent()
	{
		hOutFile.close();
	}

} /* namespace iris */
