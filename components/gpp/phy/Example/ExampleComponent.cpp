/**
 * \file components/gpp/phy/Example/ExampleComponent.cpp
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
 * Implementation of an example PhyComponent to be used as a template
 * for new PhyComponents.
 */

#include "ExampleComponent.h"

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"

using namespace std;

namespace iris
{
namespace phy
{

// export library symbols
IRIS_COMPONENT_EXPORTS(PhyComponent, ExampleComponent);

ExampleComponent::ExampleComponent(std::string name)
  : PhyComponent(name,                       // component name
                "example",                  // component type
                "An example phy component",  // description
                "Paul Sutton",              // author
                "0.1")                      // version
{
  registerParameter(
      "exampleparameter",                   // name
      "An example parameter",               // description
      "0",                                  // default value
      true,                                 // dynamic?
      example_x,                            // parameter
      Interval<uint32_t>(0,5));             // allowed values

  registerEvent(
      "exampleevent",                       // name
      "An example event",                   // description
      TypeInfo< uint32_t >::identifier);    // data type
}

void ExampleComponent::registerPorts()
{
  //Create a vector of the valid types for each port
  vector<int> validTypes;
  validTypes.push_back(TypeInfo< uint32_t >::identifier);

  //format:        (name, vector of valid types)
  registerInputPort("input1", validTypes);
  registerOutputPort("output1", validTypes);
}

map<string, int> ExampleComponent::calculateOutputTypes(std::map<std::string,
                                                        int> inputTypes)
{
  //One output type - always uint32_t
  map<string, int> outputTypes;
  outputTypes["output1"] = TypeInfo< uint32_t >::identifier;
  return outputTypes;
}

void ExampleComponent::initialize()
{
  // Set up the input and output DataBuffers
  inBuf_ = castToType<uint32_t>(inputBuffers.at(0));
  outBuf_ = castToType<uint32_t>(outputBuffers.at(0));
}

void ExampleComponent::process()
{
  //Get a DataSet from the input DataBuffer
  DataSet<uint32_t>* readDataSet = NULL;
  inBuf_->getReadData(readDataSet);

  std::size_t size = readDataSet->data.size();

  //Get a DataSet from the output DataBuffer
  DataSet<uint32_t>* writeDataSet = NULL;
  outBuf_->getWriteData(writeDataSet, size);

  //Copy the input DataSet to the output DataSet
  copy(readDataSet->data.begin(), readDataSet->data.end(), writeDataSet->data.begin());

  //Copy the timestamp and sample rate for the DataSets
  writeDataSet->timeStamp = readDataSet->timeStamp;
  writeDataSet->sampleRate = readDataSet->sampleRate;

  //Release the DataSets
  inBuf_->releaseReadData(readDataSet);
  outBuf_->releaseWriteData(writeDataSet);
}

} // namesapce phy
} // namespace iris
