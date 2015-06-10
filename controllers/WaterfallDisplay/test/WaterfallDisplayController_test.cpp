/**
 * \file controllers/WaterfallDisplay/test/WaterfallDisplayController_test.cpp
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
 * Main test file for WaterfallDisplay controller.
 */

#define BOOST_TEST_MODULE WaterfallDisplayController_Test

#include <boost/test/unit_test.hpp>
#include <qapplication.h>
#include "../WaterfallDisplayController.h"

#define PI     3.14159265358979323846

using namespace std;
using namespace iris;

BOOST_AUTO_TEST_SUITE (WaterfallDisplayController_Test)

BOOST_AUTO_TEST_CASE(WaterfallDisplayController_Basic_Test)
{
  BOOST_REQUIRE_NO_THROW(WaterfallDisplayController c);
}

BOOST_AUTO_TEST_CASE(WaterfallDisplayController_Parm_Test)
{
  WaterfallDisplayController c;
  BOOST_CHECK(c.getParameterDefaultValue("spectrogramcomponent") == "spectrogram1");
  BOOST_CHECK(c.getParameterDefaultValue("numrows") == "512");
}

BOOST_AUTO_TEST_CASE(WaterfallDisplayController_Init_Test)
{
  WaterfallDisplayController c;
  c.initialize();
  c.destroy();
}

void threadMain1()
{
  // This thread acts as the controller thread
  WaterfallDisplayController c;
  c.initialize();

  int n=512;
  float step = 2.0*PI/n;
  float* data = new float[n*2];
  for(int i=0;i<n*2;i++)
    data[i] = sinf(step*i);

  for(int i=0;i<n;i++)
  {
    Event e;
    e.eventName = "psdevent";
    e.data.assign(data+i, data+i+n);
    c.processEvent(e);
    boost::this_thread::sleep(boost::posix_time::milliseconds(10));
  }

  c.destroy();
  delete [] data;
}

BOOST_AUTO_TEST_CASE(WaterfallDisplayController_Process_Test)
{
  int argc = 1;
  char* argv[] = { const_cast<char *>("WaterfallDisplayController_Process_Test"), NULL };
  QApplication a(argc, argv);

  boost::scoped_ptr< boost::thread > thread1_;
  thread1_.reset( new boost::thread( &threadMain1 ) );
  qApp->exec();
  thread1_->join();
}

BOOST_AUTO_TEST_SUITE_END()
