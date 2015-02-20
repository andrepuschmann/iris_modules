/**
 * \file components/gpp/phy/LiquidOfdmDemod/LiquidOfdmDemodComponent.h
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
 * An OFDM demodulation component that uses liquid-dsp to do the hard
 * work.
 *
 */

#ifndef CBFESTIMATORCOMPONENT_H_
#define CBFESTIMATORCOMPONENT_H_

#include "irisapi/PhyComponent.h"
#include <boost/thread.hpp>
#include <boost/date_time.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <vector>

namespace iris
{
class CbfEstimatorComponent: public PhyComponent
{
public:
    CbfEstimatorComponent(std::string name);
    virtual ~CbfEstimatorComponent();
    virtual void calculateOutputTypes(
        std::map<std::string, int>& inputTypes,
        std::map<std::string, int>& outputTypes);
    virtual void registerPorts();
    virtual void initialize();
    virtual void parameterHasChanged(std::string name);
    virtual void process();

private:
    //Exposed parameters
    bool debug_x;           ///< Whether to output debug information
    float threshold_x;      ///< Busy channel threshold in dBm
    unsigned int window_x;  ///< Length of the window to calculate the CBF (in ms)
    bool isProbe_x;         ///< Act as a probe? (Provide CBF estimates via events).
    bool isSink_x;          ///< Act as a sink? (Has no output).

    //local variables
    boost::system_time nextRefresh_;
    typedef boost::accumulators::accumulator_set<float, boost::accumulators::stats<boost::accumulators::tag::mean> > Accumulator_t;
    Accumulator_t accumulator_;
};

} /* namespace iris */

#endif /* CBFESTIMATORCOMPONENT_H_ */
