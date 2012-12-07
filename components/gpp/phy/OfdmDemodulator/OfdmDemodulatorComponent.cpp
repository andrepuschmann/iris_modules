/**
 * \file components/gpp/phy/OfdmDemodulator/OfdmDemodulatorComponent.cpp
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
 * Implementation of the OfdmDemodulator component.
 */

#include "OfdmDemodulatorComponent.h"

#include <cmath>
#include <algorithm>
#include <boost/lambda/lambda.hpp>

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "kissfft/kissfft.hh"
#include "modulation/OfdmIndexGenerator.h"
#include "modulation/OfdmPreambleGenerator.h"
#include "modulation/Crc.h"
#include "modulation/Whitener.h"
#include "modulation/QamDemodulator.h"

using namespace std;

namespace iris
{
namespace phy
{

// export library symbols
IRIS_COMPONENT_EXPORTS(PhyComponent, OfdmDemodulatorComponent);

OfdmDemodulatorComponent::OfdmDemodulatorComponent(std::string name)
  : PhyComponent(name,                            // component name
                "ofdmdemodulator",                // component type
                "An OFDM demodulation component", // description
                "Paul Sutton",                    // author
                "1.0")                            // version
{
  registerParameter(
    "numdatacarriers", "Number of data carriers (excluding pilots)",
    "192", true, numDataCarriers_x, Interval<int>(1,65536));

  registerParameter(
    "numpilotcarriers", "Number of pilot carriers",
    "8", true, numPilotCarriers_x, Interval<int>(1,65536));

  registerParameter(
    "numguardcarriers", "Number of guard carriers",
    "311", true, numGuardCarriers_x, Interval<int>(1,65536));

  registerParameter(
    "cyclicprefixlength", "Length of cyclic prefix",
    "32", true, cyclicPrefixLength_x, Interval<int>(1,65536));

  registerParameter(
    "threshold", "Frame detection threshold",
    "0.827", true, threshold_x, Interval<float>(0.0,1.0));

  // Create our pilot sequence
  typedef Cplx c;
  c seq[] = {c(1,0),c(1,0),c(-1,0),c(-1,0),c(-1,0),c(1,0),c(-1,0),c(1,0),};
  pilotSequence_.assign(begin(seq), end(seq));
}

void OfdmDemodulatorComponent::registerPorts()
{
  registerInputPort("input1", TypeInfo< complex<float> >::identifier);
  registerOutputPort("output1", TypeInfo< uint8_t >::identifier);
}

void OfdmDemodulatorComponent::calculateOutputTypes(
    const std::map<std::string,int>& inputTypes,
    std::map<std::string,int>& outputTypes)
{
  outputTypes["output1"] = TypeInfo< uint8_t >::identifier;
}

void OfdmDemodulatorComponent::initialize()
{
  setup();
}

void OfdmDemodulatorComponent::process()
{

}

void OfdmDemodulatorComponent::setup()
{
  // Set up index vectors
  pilotIndices_.clear();
  pilotIndices_.resize(numPilotCarriers_x);
  dataIndices_.clear();
  dataIndices_.resize(numDataCarriers_x);
  OfdmIndexGenerator::generateIndices(numDataCarriers_x,
                                      numPilotCarriers_x,
                                      numGuardCarriers_x,
                                      pilotIndices_.begin(), pilotIndices_.end(),
                                      dataIndices_.begin(), dataIndices_.end());

  // Create preamble
  numBins_ = numDataCarriers_x + numPilotCarriers_x + numGuardCarriers_x + 1;
  preamble_.clear();
  preamble_.resize(numBins_);
  OfdmPreambleGenerator::generatePreamble(numDataCarriers_x,
                                          numPilotCarriers_x,
                                          numGuardCarriers_x,
                                          preamble_.begin(), preamble_.end());

  // Set up our frame detector
  detector_.reset(numBins_,cyclicPrefixLength_x,threshold_x);
}

} // namesapce phy
} // namespace iris
