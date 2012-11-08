/*
 * This file is part of Iris 2.
 * 
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the 
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin. 
 * All rights reserved.
 * 
 */



/**
 * \file ExampleComponent.cpp
 *
 *  Created on: 28-Sept-2009
 *  Created by: suttonp
 *  $Revision: 1145 $
 *  $LastChangedDate: 2010-10-28 19:04:06 +0100 (Thu, 28 Oct 2010) $
 *  $LastChangedBy: suttonp $
 */

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "ExampleComponent.h"

using namespace std;

namespace iris
{
	// export library symbols
	IRIS_COMPONENT_EXPORTS(PNComponent, ExampleComponent);

	ExampleComponent::ExampleComponent(std::string name):
        PNComponent(name, "examplepncomponent", "An example pn component", "Paul Sutton", "0.1")
    {
		//Format: registerParameter(name, description, default, dynamic?, parameter, allowed values);
        registerParameter("exampleparameter", "An example parameter", "0", true, x_example, Interval<uint32_t>(0,5));

		//Format: registerEvent(name, description, data type);
		registerEvent("exampleevent", "An example event", TypeInfo< uint32_t >::identifier);
    }

	void ExampleComponent::registerPorts()
    {
        //Register all ports
        vector<int> validTypes;
        validTypes.push_back(TypeInfo< uint32_t >::identifier);

        //format:        (name, vector of valid types)
        registerInputPort("input1", validTypes);
		registerOutputPort("output1", validTypes);
    }

	map<string, int> ExampleComponent::calculateOutputTypes(std::map<std::string, int> inputTypes)
    {
        //One output type - uint32_t
        map<string, int> outputTypes;
		outputTypes["output1"] = TypeInfo< uint32_t >::identifier;
        return outputTypes;
    }

    void ExampleComponent::initialize()
    {
		// Set up the input and output DataBuffers
		d_inBuf = castToType<uint32_t>(inputBuffers.at(0));
		d_outBuf = castToType<uint32_t>(outputBuffers.at(0));
    }

    void ExampleComponent::process()
    {
		//Get a DataSet from the input DataBuffer
		DataSet<uint32_t>* readDataSet = NULL;
        d_inBuf->getReadData(readDataSet);

		size_t size = readDataSet->data.size();

		//Get a DataSet from the output DataBuffer
        DataSet<uint32_t>* writeDataSet = NULL;
        d_outBuf->getWriteData(writeDataSet, size);

		//Copy the input DataSet to the output DataSet
		copy(readDataSet->data.begin(), readDataSet->data.end(), writeDataSet->data.begin());

		//Copy the timestamp and sample rate for the DataSets
		writeDataSet->timeStamp = readDataSet->timeStamp;
		writeDataSet->sampleRate = readDataSet->sampleRate;

		//Release the DataSets
        d_inBuf->releaseReadData(readDataSet);
		d_outBuf->releaseWriteData(writeDataSet);
    }

} /* namespace iris */
