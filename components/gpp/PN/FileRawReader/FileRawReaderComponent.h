/**
 * \file FileRawReaderComponent.h
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
 * Source component to read data from file.
 */

#ifndef PN_FILERAWREADERCOMPONENT_H_
#define PN_FILERAWREADERCOMPONENT_H_

#include <fstream>

#include "irisapi/PNComponent.h"

namespace iris
{

/*!
 * \brief Read data from file
 *
 * The FileRawReaderComponent reads raw data from a file
 * and interprets it as a given data type.
 */
class FileRawReaderComponent: public PNComponent
{
 public:
  /*!
	 * Call the constructor on PNComponent and pass in all details
	 * about the component.	Register all parameters and events.
	 * \param name the name given to this component in the radio config
	 */
  FileRawReaderComponent(std::string name);

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

 private:
  //! template function used to read the data
  template<typename T> void readBlock();

  //! size of blocks to read from file
  int blockSize_x;
  //! name of file to read
  std::string fileName_x;
  //! interpret the data as this data type
  std::string dataType_x;
  //! endianness of the data
  std::string endian_x;

  //! the file stream
  std::ifstream hInFile_;
};

} // namespace iris
#endif // PN_FILERAWREADERCOMPONENT_H_

