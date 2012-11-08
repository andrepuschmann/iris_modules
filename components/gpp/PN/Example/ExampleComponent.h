/*
 * This file is part of Iris 2.
 * 
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the 
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin. 
 * All rights reserved.
 * 
 */



/**
 * \file ExampleComponent.h
 * An example PNComponent to be used as a template for new PNComponents
 *
 *  Created on: 28-Sept-2009
 *  Created by: suttonp
 *  $Revision: 1145 $
 *  $LastChangedDate: 2010-10-28 19:04:06 +0100 (Thu, 28 Oct 2010) $
 *  $LastChangedBy: suttonp $
 *
 */

#ifndef EXAMPLECOMPONENT_H_
#define EXAMPLECOMPONENT_H_

#include "irisapi/PNComponent.h"

namespace iris
{

/*!
 * \brief An example PNComponent to be used as a template for new PNComponents
 *
 * Copy this component, rename folder, .h, .cpp and .vcproj files.
 * Edit all files for your new component and build.
 */
class ExampleComponent: public PNComponent
{
private:
	//! convenience pointer for incoming DataSet buffer
	ReadBuffer<uint32_t>* d_inBuf;
	//! convenience pointer for outgoing DataSet buffer
	WriteBuffer< uint32_t >* d_outBuf;

	//! Example of a parameter which will be exposed by this component
	uint32_t x_example;

public:
	/*!
	 * Call the constructor on PNComponent and pass in all details
	 * about the component.	Register all parameters and events.
	 * \param name the name given to this component in the radio config
	 */
    ExampleComponent(std::string name);

    /*!
	 * Given the data-types associated with each input port, provide
	 * the data-types which will be produced on each output port.
	 * \param inputTypes the map of input port names and data-type identifiers
	 * \return map of output port names and data-type identifiers
	 */
    virtual std::map<std::string, int> calculateOutputTypes(std::map<std::string, int> inputTypes);

    /*!
	 * Register the input and output ports of this component
	 * by declaring them as input or output, naming them and
	 * providing a list of valid data types.
	 */
    virtual void registerPorts();

    /*!
	 * Do any initialization required by this component.
	 */
    virtual void initialize();

    /*!
	 * This is where the work of this component gets done.
	 * Typically components will take DataSets from their input
	 * ports, process them and write DataSets to their output ports.
	 */
    virtual void process();
};

} /* namespace iris */

#endif /* EXAMPLECOMPONENT_H_ */
