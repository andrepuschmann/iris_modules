/**
 * \file components/gpp/stack/FrameDetector/FrameDetectorComponent.h
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
 * Implementation of a simple detector frame component.
 * This allows to trigger an event if a dataset pushed through
 * the component matches a certain pattern, e.g. contains
 * a UDP header.
 *
 */

#ifndef STACK_FRAMEDETECTORCOMPONENT_H_
#define STACK_FRAMEDETECTORCOMPONENT_H_

#include "irisapi/StackComponent.h"
#include <net/ethernet.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

namespace iris
{
namespace stack
{

class FrameDetectorComponent
  : public StackComponent
{
public:
  FrameDetectorComponent(std::string name);
  virtual void initialize();
  virtual void processMessageFromAbove(boost::shared_ptr<StackDataSet> set);
  virtual void processMessageFromBelow(boost::shared_ptr<StackDataSet> set);

private:
  void runFrameDetector(boost::shared_ptr<StackDataSet> set);
  bool matchUdp_x;
  bool matchIcmp_x;
};

} // namespace stack
} // namespace iris

#endif // STACK_FRAMEDETECTORCOMPONENT_H_
