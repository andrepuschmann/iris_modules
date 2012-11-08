/*
 * This file is part of Iris 2.
 * 
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the 
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin. 
 * All rights reserved.
 * 
 */



/**
 * \file ExampleController.cpp
 *
 *  Created on: 28-Sept-2009
 *  Created by: suttonp
 *  $Revision: 763 $
 *  $LastChangedDate: 2009-12-16 13:57:33 +0000 (Wed, 16 Dec 2009) $
 *  $LastChangedBy: suttonp $
 */

#include <sstream>

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "ExampleController.h"

using namespace std;

namespace iris
{
	//Export library functions
	IRIS_CONTROLLER_EXPORTS(ExampleController);

	/*! Constructor
	*
	*	Call the constructor on Controller and pass in all details about the controller.
    */
    ExampleController::ExampleController():
        Controller("examplecontroller", "An example controller", "Paul Sutton", "0.1")
    {
		counter = 1;
    }

	/*! Subscribe to events
	*
	*	Subscribe to events on specific components that this controller is interested in.
    */
	void ExampleController::subscribeToEvents()
	{
		// Format: subscribeToEvent(Event name, Component name);
		subscribeToEvent("testevent", "snk1");
	}

	/*! Initialize this controller
	*
	*	Do any initialization required by this controller.
	*	This function is guaranteed to be called by this controller's thread.
    */
    void ExampleController::initialize()
    {
    }

	/*! Process an event
	*
	*	If an event which this controller has subscribed to is activated, this function is called by 
	*	the controller thread. 
	*
	*	\param	e	The event which was activated by the component 
    */
    void ExampleController::processEvent(Event &e)
    {
		//Reconfigure the radio by creating a reconfiguration set
		ReconfigSet r;

		ParametricReconfig p;
		p.engineName = "pnengine1";
		p.componentName = "src1";
		p.parameterName = "counter";
		stringstream str;
		str << counter++;
		p.parameterValue = str.str();

		r.d_paramReconfigs.push_back(p);

		reconfigureRadio(r);
    }

	/*! Destroy this controller
	*
	*	Do any destruction required by this controller.
	*	This function is guaranteed to be called by this controller's thread.
	*	IMPORTANT: This function should never throw an exception.
    */
	void ExampleController::destroy()
	{
	}

} /* namespace iris */
