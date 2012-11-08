/*
 * This file is part of Iris 2.
 *
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin.
 * All rights reserved.
 *
 */

/**
 * \file MultiportExampleComponent.cpp
 *
 *  Created on: 2-Mar-2010
 *  Created by: suttonp
 *  $Revision: 845 $
 *  $LastChangedDate: 2010-03-04 13:21:23 +0000 (Thu, 04 Mar 2010) $
 *  $LastChangedBy: suttonp $
 */

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "MultiportExampleComponent.h"

using namespace std;
using namespace boost;

namespace iris
{
	// export library symbols
	IRIS_COMPONENT_EXPORTS(StackComponent, MultiportExampleComponent);

	/*! Constructor
	*
	*	Call the constructor on StackComponent and pass in all details about the component.
	*	Register all parameters and events in the constructor.
    *
    *   \param  name	The name assigned to this component when loaded
    */
	MultiportExampleComponent::MultiportExampleComponent(std::string name):
        StackComponent(name, "multiportexamplestackcomponent", "An example stack component with multiple ports", "Paul Sutton", "0.1")
    {
		//Format: registerParameter(name, description, default, dynamic?, parameter, allowed values);
        registerParameter("exampleparameter", "An example parameter", "0", true, x_example, Interval<uint32_t>(0,5));
    }

	//We're overriding registerPorts as we want to use multiple ports
	void MultiportExampleComponent::registerPorts()
	{
		std::vector<int> types;
        types.push_back( int(TypeInfo< uint8_t >::identifier) );

		//The port on top of the componen
		registerInputPort("topport1", types);

		//The first port below the component
		registerInputPort("bottomport1", types);

		//The second port below the component
		registerInputPort("bottomport2", types);
	};

	//! Do any initialization required
    void MultiportExampleComponent::initialize()
    {
    }

	/*! Process a message from above
	*
	*	This example copies StackDataSets onto all its bottom ports
    */
	void MultiportExampleComponent::processMessageFromAbove(boost::shared_ptr<StackDataSet> set)
	{
		//Create a second StackDataSet and copy the set into it
		shared_ptr<StackDataSet> cpy(new StackDataSet);
		*cpy = *set;

		sendDownwards("bottomport1", set);
		sendDownwards("bottomport2", cpy);
	}

	/*! Process a message from below
	*
	*	This example just passes data through.
    */
	void MultiportExampleComponent::processMessageFromBelow(boost::shared_ptr<StackDataSet> set)
	{
		sendUpwards(set);
	}

} /* namespace iris */
