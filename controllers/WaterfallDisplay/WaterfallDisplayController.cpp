/**
 * \file controllers/WaterfallDisplay/WaterfallDisplayController.cpp
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
 * A spectrogram waterfall display using the qt waterfallplot widget.
 */

#include <sstream>

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "WaterfallDisplayController.h"

using namespace std;

namespace iris
{

//! Export library functions
IRIS_CONTROLLER_EXPORTS(WaterfallDisplayController);

WaterfallDisplayController::WaterfallDisplayController()
  : Controller("WaterfallDisplay", "A spectrogram waterfall display",
               "Paul Sutton", "0.1")
{
  registerParameter("spectrogramcomponent", "Name of the spectrogram component",
      "spectrogram1", false, spgrmCompName_x);
  registerParameter("numrows", "Number of rows to use in the waterfall plot",
      "512", false, numRows_x, Interval<int>(1,65536));
}

void WaterfallDisplayController::subscribeToEvents()
{
  subscribeToEvent("psdevent", spgrmCompName_x);
}

void WaterfallDisplayController::initialize()
{
  plot_.reset(new Waterfallplot(512, numRows_x));
  plot_->setPlotYAxisScale(-100,0);
  plot_->setSpectrogramZAxisScale(-100,0);
}

void WaterfallDisplayController::processEvent(Event &e)
{
  //We've only subscribed to psdevent
  vector<float> data;
  for(int i=0;i<e.data.size();i++)
    data.push_back(boost::any_cast<float>(e.data[i]));

  plot_->appendNewData(data.begin(), data.end());
}

void WaterfallDisplayController::destroy()
{}

} // namespace iris
