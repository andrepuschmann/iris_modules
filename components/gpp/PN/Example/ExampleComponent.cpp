/**
 * @file ExampleComponent.cpp
 * @version 1.0
 *
 * @section COPYRIGHT
 *
 * Copyright 2012 The Iris Project Developers. See the
 * COPYRIGHT file at the top-level directory of this distribution
 * and at http://www.softwareradiosystems.com/iris/copyright.html.
 *
 * @section LICENSE
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
 * @section DESCRIPTION
 *
 * Implementation of an example PNComponent to be used as a template
 * for new PNComponents.
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
