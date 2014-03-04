/**
 * \file components/gpp/phy/EnergyDetector/EnergyDetectorComponent.cpp
 * \version 1.0
 *
 * \section COPYRIGHT
 *
 * Copyright 2012-2013 The Iris Project Developers. See the
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
 * A component for calculating the energy level of a given set
 * of I/Q samples using a simple IIR filter.
 *
 */

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "EnergyDetectorComponent.h"
#include <algorithm>

using namespace std;

namespace iris
{
// export library symbols
IRIS_COMPONENT_EXPORTS(PhyComponent, EnergyDetectorComponent);


EnergyDetectorComponent::EnergyDetectorComponent(string name):
    PhyComponent(name, "energydetectorphycomponent", "A component for calculating the received energy level.", "Andre Puschmann", "0.1")
{
    //Format: registerParameter(name, description, default, dynamic?, parameter)
    registerParameter("debug", "Whether to output debug information",
        "false", true, debug_x);

    registerParameter("alpha", "Alpha value of the IIR filter",
        "0.2", true, alpha_x);

    registerParameter("isprobe", "Act as a probe (provide received energy via events)",
        "false", false, isProbe_x);

    registerParameter("ispassthrough", "Pass incoming samples through to output",
         "false", false, isPassthrough_x);

    registerParameter("issink", "Act as a sink (do not provide output)",
         "false", false, isSink_x);

    registerEvent("rssievent", "An event providing the current received energy level in dBm",
        TypeInfo< float >::identifier);
}


EnergyDetectorComponent::~EnergyDetectorComponent()
{
}


void EnergyDetectorComponent::registerPorts()
{
    registerInputPort("input1", TypeInfo< complex<float> >::identifier);
    if (!isSink_x)
    {
        std::vector<int> types;
        if (isPassthrough_x) {
            types.push_back(TypeInfo< complex<float> >::identifier);
        } else {
            types.push_back(TypeInfo< float >::identifier);
        }
        types.push_back(TypeInfo< float >::identifier);
        registerOutputPort("output1", types);
    }
}

void EnergyDetectorComponent::calculateOutputTypes(
        std::map<std::string,int>& inputTypes,
        std::map<std::string,int>& outputTypes)
{
    if (!isSink_x)
    {
        if (isPassthrough_x) {
            outputTypes["output1"] = TypeInfo< complex<float> >::identifier;
        } else {
            outputTypes["output1"] = TypeInfo< float >::identifier;
        }
    }
}


void EnergyDetectorComponent::initialize()
{
}

void EnergyDetectorComponent::parameterHasChanged(std::string name)
{
}


float EnergyDetectorComponent::calculateEnergyLevel(std::vector<std::complex<float> >& data, const int len)
{
    float rssiHat = 0.0f;
    // this should help producing vectorized code if flags are enabled in compiler settings
#pragma simd
#pragma vector aligned
    for (int i = 0; i < len; i++) {
        // compute magnitude squared
        float magnitude = data[i].real() * data[i].real() + data[i].imag() * data[i].imag();

        // update rssi estimate
        rssiHat = (1.0f - alpha_x) * rssiHat + alpha_x * magnitude;
    }
    return (10 * log10f(rssiHat));
}


void EnergyDetectorComponent::process()
{
    //Get a DataSet from the input DataBuffer
    DataSet< complex<float> >* readDataSet = NULL;
    getInputDataSet("input1", readDataSet);
    std::size_t size = readDataSet->data.size();

    float energy = calculateEnergyLevel(readDataSet->data, size);
    if (debug_x) {
        LOG(LDEBUG) << "RSSI: " << energy;
    }
    if (isProbe_x) {
        activateEvent("rssievent", energy);
    }

    if(!isSink_x && isProbe_x) {
        if (isPassthrough_x) {
            // Pass data through
            DataSet< complex<float> >* writeDataSet = NULL;
            getOutputDataSet("output1", writeDataSet, size);
            writeDataSet->data = readDataSet->data;
            releaseOutputDataSet("output1", writeDataSet);
        } else {
            // Output energy level
            DataSet< float >* writeDataSet = NULL;
            getOutputDataSet("output1", writeDataSet, 1);
            writeDataSet->data[0] = energy;
            releaseOutputDataSet("output1", writeDataSet);
        }
    }

    releaseInputDataSet("input1", readDataSet);
}

} /* namespace iris */
