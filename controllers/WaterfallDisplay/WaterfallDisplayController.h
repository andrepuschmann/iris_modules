/**
 * \file controllers/WaterfallDisplay/WaterfallDisplayController.h
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

#ifndef CONTROLLERS_WATERFALLDISPLAYCONTROLLER_H_
#define CONTROLLERS_WATERFALLDISPLAYCONTROLLER_H_

#include "irisapi/Controller.h"
#include <boost/scoped_ptr.hpp>
#include "graphics/qt/waterfallplot/Waterfallplot.h"

namespace iris
{

class WaterfallDisplayController
  : public Controller
{
public:
  WaterfallDisplayController();
  virtual void subscribeToEvents();
  virtual void initialize();
  virtual void processEvent(Event &e);
	virtual void destroy();

private:
  std::string spgrmCompName_x;
  int numRows_x;

  boost::scoped_ptr<Waterfallplot> plot_;
};

} // namespace iris

#endif // CONTROLLERS_WATERFALLDISPLAYCONTROLLER_H_
