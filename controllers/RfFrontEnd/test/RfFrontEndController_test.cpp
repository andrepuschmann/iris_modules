/**
 * \file controllers/RfFrontEnd/test/RfFrontEndController_test.cpp
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
 * Main test file for RfFrontEnd controller.
 */

#define BOOST_TEST_MODULE RfFrontEndController_Test

#include <boost/test/unit_test.hpp>
#include <qapplication.h>
#include "../RfFrontEndController.h"

using namespace std;
using namespace iris;

BOOST_AUTO_TEST_SUITE (RfFrontEndController_Test)

BOOST_AUTO_TEST_CASE(RfFrontEndController_Basic_Test)
{
  BOOST_REQUIRE_NO_THROW(RfFrontEndController c);
}

BOOST_AUTO_TEST_CASE(RfFrontEndController_Parm_Test)
{
  RfFrontEndController c;
  BOOST_CHECK(c.getParameterDefaultValue("minfrequency") == "2400000000");
  BOOST_CHECK(c.getParameterDefaultValue("maxfrequency") == "2405000000");
  BOOST_CHECK(c.getParameterDefaultValue("stepfrequency") == "100000");
  BOOST_CHECK(c.getParameterDefaultValue("minbandwidth") == "200000");
  BOOST_CHECK(c.getParameterDefaultValue("maxbandwidth") == "5000000");
  BOOST_CHECK(c.getParameterDefaultValue("stepbandwidth") == "250000");
  BOOST_CHECK(c.getParameterDefaultValue("mingain") == "0");
  BOOST_CHECK(c.getParameterDefaultValue("maxgain") == "20");
  BOOST_CHECK(c.getParameterDefaultValue("stepgain") == "1");
}

BOOST_AUTO_TEST_CASE(RfFrontEndController_Init_Test)
{
  RfFrontEndController c;
  c.initialize();
  c.destroy();
}

void threadMain1()
{
  // Load and start - controller runs own thread
  RfFrontEndController c;
  c.load();
  c.start();
  boost::this_thread::sleep(boost::posix_time::seconds(10));
  c.stop();
  c.unload();
}

BOOST_AUTO_TEST_CASE(RfFrontEndController_Process_Test)
{
  int argc = 1;
  char* argv[] = { const_cast<char *>("RfFrontEndController_Process_Test"), NULL };
  QApplication a(argc, argv);

  boost::scoped_ptr< boost::thread > thread1_;
  thread1_.reset( new boost::thread( &threadMain1 ) );
  qApp->exec();
  thread1_->join();
}

BOOST_AUTO_TEST_SUITE_END()
