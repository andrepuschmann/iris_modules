/**
 * \file components/gpp/phy/CbfEstimator/CbfEstimatorComponent.cpp
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
 * A component for estimating the channel busy fraction.
 *
 */

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "CbfEstimatorComponent.h"
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace std;

namespace iris
{
// export library symbols
IRIS_COMPONENT_EXPORTS(PhyComponent, CbfEstimatorComponent);


CbfEstimatorComponent::CbfEstimatorComponent(string name):
    PhyComponent(name, "cbfestimatorphycomponent", "A component for estimating the channel busy fraction.", "Andre Puschmann", "0.1")
    ,nextRefresh_(boost::get_system_time())
{
    //Format: registerParameter(name, description, default, dynamic?, parameter)
    registerParameter("debug", "Whether to output debug information",
        "false", true, debug_x);

    registerParameter("threshold", "Busy channel threshold in dBm",
        "-70", true, threshold_x);

    registerParameter("window", "Length of the window to calculate the CBF (in ms)",
        "1000", true, window_x);

    registerParameter("isprobe", "Act as a probe (provide PSDs via events)",
         "false", false, isProbe_x);

    registerParameter("issink", "Act as a sink (do not provide output)",
         "false", false, isSink_x);

    registerEvent("cbfevent", "An event providing the current estimated CBF [0:1]",
        TypeInfo< float >::identifier);
}


CbfEstimatorComponent::~CbfEstimatorComponent()
{
}


void CbfEstimatorComponent::registerPorts()
{
    registerInputPort("input1", TypeInfo< complex<float> >::identifier);
    if (!isSink_x)
    {
        std::vector<int> types;
        types.push_back(TypeInfo< float >::identifier);
        registerOutputPort("output1", types);
    }
}


void CbfEstimatorComponent::calculateOutputTypes(
        std::map<std::string,int>& inputTypes,
        std::map<std::string,int>& outputTypes)
{
    if (!isSink_x)
    {
        outputTypes["output1"] = TypeInfo< float >::identifier;
    }
}


void CbfEstimatorComponent::initialize()
{
}


void CbfEstimatorComponent::parameterHasChanged(std::string name)
{
}


void CbfEstimatorComponent::process()
{
    //Get a DataSet from the input DataBuffer
    DataSet< float >* readDataSet = NULL;
    getInputDataSet("input1", readDataSet);
    std::size_t size = readDataSet->data.size();

    // accumulate data
    if (size == 1) {
        float rssi = readDataSet->data[0];
        accumulator_(rssi > threshold_x ? 1.0 : 0.0);
    }

    // check for next refresh
    if (boost::get_system_time() >= nextRefresh_) {
        float cbf = boost::accumulators::mean(accumulator_);
        if (debug_x) {
            LOG(LDEBUG) << "CBF: " << cbf;
        }
        if (isProbe_x) {
            activateEvent("cbfevent", cbf);
        }
        accumulator_ = Accumulator_t(); // reset accumulator
        nextRefresh_ = boost::get_system_time() + boost::posix_time::milliseconds(window_x);
    }

    if (!isSink_x && isProbe_x)
    {
        //Pass data through
        DataSet< float >* writeDataSet = NULL;
        getOutputDataSet("output1", writeDataSet, size);
        writeDataSet->data = readDataSet->data;
        releaseOutputDataSet("output1", writeDataSet);
    }

    releaseInputDataSet("input1", readDataSet);
}

} /* namespace iris */
