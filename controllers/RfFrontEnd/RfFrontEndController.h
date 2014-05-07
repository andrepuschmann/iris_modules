/**
 * \file controllers/RfFrontEnd/RfFrontEndController.h
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
 * A generic RF front-end controller. This controller
 * provides a gui to manually adjust parameters including frequency,
 * bandwidth and power.
 */

#ifndef CONTROLLERS_RFFRONTENDCONTROLLER_H_
#define CONTROLLERS_RFFRONTENDCONTROLLER_H_

#include "boost/scoped_ptr.hpp"
#include "irisapi/Controller.h"
#include "RfFrontEndGui/RfFrontEndGui.h"

namespace iris
{

class RfFrontEndController
  : public Controller
{
public:
  RfFrontEndController();
	virtual void subscribeToEvents();
	virtual void initialize();
  virtual void processEvent(Event &e);
	virtual void destroy();

private:
  void processFrequency(double f);
  void processBandwidth(double b);
  void processGain(double g);

  double minFrequency_x;
  double maxFrequency_x;
  double stepFrequency_x;
  double minBandwidth_x;
  double maxBandwidth_x;
  double stepBandwidth_x;
  double minGain_x;
  double maxGain_x;
  double stepGain_x;
  std::string frontEnd_x;
  std::string frontEndEngine_x;

  boost::scoped_ptr<RfFrontEndGui> gui_;
};

} // namespace iris

#endif // CONTROLLERS_RFFRONTENDCONTROLLER_H_
