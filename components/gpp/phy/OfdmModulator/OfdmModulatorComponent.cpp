/**
 * \file components/gpp/phy/OfdmModulator/OfdmModulatorComponent.cpp
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
 * Implementation of the OfdmModulator component.
 */

#include "OfdmModulatorComponent.h"

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"

using namespace std;

namespace iris
{
namespace phy
{

// export library symbols
IRIS_COMPONENT_EXPORTS(PhyComponent, OfdmModulatorComponent);

OfdmModulatorComponent::OfdmModulatorComponent(std::string name)
  : PhyComponent(name,                          // component name
                "ofdmmodulator",                // component type
                "An OFDM modulation component", // description
                "Paul Sutton",                  // author
                "1.0")                          // version
{
/*
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
*/
}

void OfdmModulatorComponent::registerPorts()
{
  registerInputPort("input1", TypeInfo< uint8_t >::identifier);
  registerOutputPort("output1", TypeInfo< complex<float> >::identifier);
}

void OfdmModulatorComponent::calculateOutputTypes(
    const std::map<std::string,int>& inputTypes,
    std::map<std::string,int>& outputTypes)
{
  //One output type - always complex<float>
  outputTypes["output1"] = TypeInfo< complex<float> >::identifier;
}

void OfdmModulatorComponent::initialize()
{

}

void OfdmModulatorComponent::process()
{

}

} // namesapce phy
} // namespace iris
