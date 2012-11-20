/**
 * \file ExampleComponent.h
 * \version 1.0
 *
 * \section COPYRIGHT
 *
 * Copyright 2012 The Iris Project Developers. See the
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
 * An example PNComponent to be used as a template for new PNComponents.
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
	ReadBuffer<uint32_t>* inBuf_;
	//! convenience pointer for outgoing DataSet buffer
	WriteBuffer< uint32_t >* outBuf_;

	//! Example of a parameter which will be exposed by this component
	uint32_t example_x;

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
