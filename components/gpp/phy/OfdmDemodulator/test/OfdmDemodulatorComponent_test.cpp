/**
 * \file components/gpp/phy/OfdmDemodulator/OfdmDemodulatorComponent_test.cpp
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
 * Main test file for OfdmDemodulator component.
 */

#define BOOST_TEST_MODULE OfdmDemodulatorComponent_Test

#include <boost/test/unit_test.hpp>

#include "../OfdmDemodulatorComponent.h"
#include "utility/DataBufferTrivial.h"

using namespace std;
using namespace iris;
using namespace iris::phy;

BOOST_AUTO_TEST_SUITE (OfdmDemodulatorComponent_Test)

BOOST_AUTO_TEST_CASE(OfdmDemodulatorComponent_Basic_Test)
{
  BOOST_REQUIRE_NO_THROW(OfdmDemodulatorComponent demod("test"));
}

BOOST_AUTO_TEST_SUITE_END()
