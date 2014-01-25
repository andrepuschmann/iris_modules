/**
 * \file controllers/RadioConfig/RadioConfigController.cpp
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

#include <sstream>

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "RadioConfigController.h"

namespace iris
{
    //Export library functions
    IRIS_CONTROLLER_EXPORTS(RadioConfigController);

    RadioConfigController::RadioConfigController():
        Controller("radioconfigcontroller", "Radio re-configuration controller", "Andre Puschmann, Asad Asad", "0.1")
        ,controlGate_(NULL)
    {
    }


    void RadioConfigController::subscribeToEvents()
    {
        // Format: subscribeToEvent(Event name, Component name);
        //subscribeToEvent("radioConfigEvent", "filerawreader1");
    }


    void RadioConfigController::initialize()
    {
        GateFactory &myFactory = GateFactory::getInstance();
        controlGate_ = myFactory.createGate("RadioConfigController", "control");
    }


    void RadioConfigController::processEvent(Event &e)
    {
        // no event to process here
    }


    /*! Override the work function of this controller
     *  This work function is a separate thread which may be used
     *  to coordinate cyclic operations executed by a controller
     */
    void RadioConfigController::workFunction()
    {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        LOG(LDEBUG) << "Listening to incoming messages ..";
        while (true) {
            boost::this_thread::interruption_point();
            handleRequest();
        }
    }


    void RadioConfigController::destroy()
    {
    }


    void RadioConfigController::handleRequest()
    {
        RadioConfigControl request, reply;

        if (controlGate_ == NULL) return;

        // call recv function in a non-blocking fassion (needed to kill thread properly)
        if (controlGate_->recvProto(request, false) == 0) {
            //LOG(LDEBUG) << "Request received.";
            if (request.type() == REQUEST) {
                switch(request.command()) {
                    case GET_RADIO_CONFIG:
                        //LOG(LDEBUG) << "Get radio configuration request received.";
                        getRadioConfig(&reply);
                        break;

                    case SET_RADIO_CONFIG:
                        //LOG(LDEBUG) << "Set radio configuration request received.";
                        setRadioConfig(&request);
                        break;
                }

                reply.set_type(REPLY);
                reply.set_command(request.command());
                controlGate_->sendProto(reply);
            } else {
                LOG(LERROR) << "Request type not supported!";
            }
        }
        boost::this_thread::sleep(boost::posix_time::milliseconds(500));
    }


    //! send radio configuration to the external app via proto buff
    void RadioConfigController::getRadioConfig(RadioConfigControl *request)
    {
        RadioConfig *config = request->mutable_radioconf();
        int nrEngines = getNrEngines();
        int totalNrComponents = getNrComponents();

        for (int i = 0; i < nrEngines; i++) {
            RadioEngine *engine = config->add_engines();
            std::string engineName = getEngineNameFromIndex(i);
            engine->set_name(engineName);

            //FIXME: Iris core has no API call to iterate over all components
            //of a engine, we therefore iterate over all components and check whether
            //it belongs to the current engine
            for (int k = 0; k < totalNrComponents; k++) {
                int dummy;
                std::string componentName = getComponentName(k);
                std::string engineOfComponent = getEngineName(componentName, &dummy, &dummy);
                if (engineName.compare(engineOfComponent) == 0) {
                    // component belongs to current engine
                    RadioComponent *component = engine->add_components();
                    component->set_name(componentName);
                    int nrParameters = getNrParameters( componentName);
                    for (int l = 0; l < nrParameters; l++) {
                        ComponentParameter *parameter = component->add_parameters();
                        std::string value;
                        std::string name = getParameterName(componentName, l, value);
                        parameter->set_name(name);
                        parameter->set_value(value);
                    }
                }
            }
        }
    }


    //! apply radio re-configuration based on the recieved message via SCL
    void RadioConfigController::setRadioConfig(RadioConfigControl *request)
    {
        RadioConfig *radioConfig = request->mutable_radioconf();
        ReconfigSet r;

        // FIXME: replace with for_each
        // iterate over engines
        for (int i = 0; i < radioConfig->engines_size(); i++) {
            RadioEngine *engine = radioConfig->mutable_engines(i);

            // iterate over components
            for (int k = 0; k < engine->components_size(); k++) {
                RadioComponent *component = engine->mutable_components(k);

                // iterate over parameter
                for (int l = 0; l < component->parameters_size(); l++) {
                    ComponentParameter *parameter = component->mutable_parameters(l);
                    ParametricReconfig p;
                    p.engineName = engine->name();
                    p.componentName = component->name();
                    p.parameterName = parameter->name();
                    p.parameterValue = parameter->value();
                    r.paramReconfigs.push_back(p);
                }
            }
        }

        // finally apply new configuration
        reconfigureRadio(r);
    }

} /* namespace iris */
