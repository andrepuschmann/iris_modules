/*
 * This file is part of Iris 2.
 *
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin.
 * All rights reserved.
 *
 */



/**
 * \file ExampleComponent.cpp
 *
 *  Created on: 2-Mar-2010
 *  Created by: suttonp
 *  $Revision: 1095 $
 *  $LastChangedDate: 2010-10-22 10:36:32 +0100 (Fri, 22 Oct 2010) $
 *  $LastChangedBy: suttonp $
 */

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "ExampleComponent.h"

using namespace std;

namespace iris
{
	// export library symbols
	IRIS_COMPONENT_EXPORTS(StackComponent, ExampleComponent);

	/*! Constructor
	*
	*	Call the constructor on StackComponent and pass in all details about the component.
	*	Register all parameters and events in the constructor.
    *
    *   \param  name	The name assigned to this component when loaded
    */
	ExampleComponent::ExampleComponent(std::string name):
        StackComponent(name, "examplestackcomponent", "An example stack component", "Paul Sutton", "0.1")
    {
		//Format: registerParameter(name, description, default, dynamic?, parameter, allowed values);
        registerParameter("exampleparameter", "An example parameter", "0", true, x_example, Interval<uint32_t>(0,5));

		//Format: registerEvent(name, description, data type);
		registerEvent("exampleevent", "An example event", TypeInfo< uint32_t >::identifier);
    }

	//! Do any initialization required
    void ExampleComponent::initialize()
    {
    }

    //! Start this stack component
	void ExampleComponent::start()
	{
	}

	//! Stop this stack component
	void ExampleComponent::stop()
	{
	}

	/*! Process a message from above
	*
	*	This example just passes data through.
    */
	void ExampleComponent::processMessageFromAbove(boost::shared_ptr<StackDataSet> set)
	{
		sendDownwards(set);
	}

	/*! Process a message from below
	*
	*	This example just passes data through.
    */
	void ExampleComponent::processMessageFromBelow(boost::shared_ptr<StackDataSet> set)
	{
		sendUpwards(set);
	}

} /* namespace iris */
