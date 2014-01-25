/**
 * \file controllers/RadioConfig/RadioConfigController.h
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
 * Implementation of a controller that allows to reconfigure Iris
 * from an external application.
 */

#ifndef RADIOCONFIGCONTROLLER_H_
#define RADIOCONFIGCONTROLLER_H_

#include "irisapi/Controller.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp> 
#include <boost/format.hpp>
#include "sclhelper.hpp"
#include "radioconfig.pb.h"

using namespace std;
using namespace boost;
using namespace scl_radioconfig;

namespace iris
{
    class RadioConfigController: public Controller
    {
        private:
        sclGate *controlGate_;
        void handleRequest();
        void getRadioConfig(RadioConfigControl *request);
        void setRadioConfig(RadioConfigControl *request);
        
    public:
        RadioConfigController();
        virtual void subscribeToEvents();
        virtual void initialize();
        virtual void processEvent(Event &e);
        virtual void destroy();
        virtual void workFunction();
    };
    
} /* namespace iris */

#endif /* RADIOCONFIGCONTROLLER_H_ */
