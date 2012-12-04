/**
 * \file components/gpp/phy/OfdmModulator/OfdmModulatorComponent_test.cpp
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
 * Main test file for OfdmModulator component.
 */

#define BOOST_TEST_MODULE OfdmModulatorComponent_Test

#include <boost/test/unit_test.hpp>

#include "../OfdmModulatorComponent.h"

using namespace std;
using namespace iris::phy;

BOOST_AUTO_TEST_SUITE (OfdmModulatorComponent_Test)

BOOST_AUTO_TEST_CASE(OfdmModulatorComponent_Basic_Test)
{
  BOOST_REQUIRE_NO_THROW(OfdmModulatorComponent mod("test"));
}

BOOST_AUTO_TEST_CASE(OfdmModulatorComponent_Parm_Test)
{
  OfdmModulatorComponent mod("test");
  BOOST_CHECK(mod.getParameterDefaultValue("numdatacarriers") == "192");
  BOOST_CHECK(mod.getParameterDefaultValue("numpilotcarriers") == "8");
  BOOST_CHECK(mod.getParameterDefaultValue("numguardcarriers") == "311");
  BOOST_CHECK(mod.getParameterDefaultValue("modulationdepth") == "1");
  BOOST_CHECK(mod.getParameterDefaultValue("cyclicprefixlength") == "32");
  BOOST_CHECK(mod.getParameterDefaultValue("maxsymbolsperframe") == "32");
}

BOOST_AUTO_TEST_SUITE_END()
