/**
 * \file components/gpp/phy/EnergyDetector/EnergyDetectorComponent.h
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

#ifndef ENERGYDETECTORCOMPONENT_H_
#define ENERGYDETECTORCOMPONENT_H_

#include "irisapi/PhyComponent.h"
#include <boost/scoped_ptr.hpp>

namespace iris
{
class EnergyDetectorComponent: public PhyComponent
{
public:
    typedef std::vector<float>    FloatVec;
    typedef FloatVec::iterator    FloatVecIt;

    EnergyDetectorComponent(std::string name);
    virtual ~EnergyDetectorComponent();
    virtual void calculateOutputTypes(
        std::map<std::string, int>& inputTypes,
        std::map<std::string, int>& outputTypes);
    virtual void registerPorts();
    virtual void initialize();
    virtual void parameterHasChanged(std::string name);
    virtual void process();

private:
    //Exposed parameters
    bool debug_x;
    float alpha_x;          ///< alpha of the IIR filter
    bool isProbe_x;         ///< Act as a probe? (Provide CBF estimates via events).
    bool isPassthrough_x;   ///< Pass incoming samples through to output
    bool isSink_x;          ///< Act as a sink? (Has no output).

    // member functions
    float calculateEnergyLevel(std::vector<std::complex<float> >& data, const int len);
};

} /* namespace iris */

#endif /* ENERGYDETECTORCOMPONENT_H_ */
