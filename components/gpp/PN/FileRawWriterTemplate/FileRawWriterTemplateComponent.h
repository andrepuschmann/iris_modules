/**
 * \file FileRawWriterTemplateComponent.h
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
 * A template sink component used to write data to file.
 */

#ifndef FILERAWWRITERTEMPLATECOMPONENT_H_
#define FILERAWWRITERTEMPLATECOMPONENT_H_

#include <fstream>

#include "irisapi/TemplatePNComponent.h"

namespace iris
{

// forward declaration
template <class Tin, class Tout>
class FileRawWriterTemplateComponentImpl;


/*!
 * \brief A template component used to write data to file
 *
 * The FileRawWriterTemplateComponent is derived from TemplatePNComponent,
 * so we need to give the class name itself as a template parameter.
 */
class FileRawWriterTemplateComponent: public TemplatePNComponent<FileRawWriterTemplateComponent>
{
protected:
  //! the file name to write to
  std::string x_fileName;

public:

  /*!
   * supported types at the input port
   * (can be a boost::mpl::vector of types, or of other
   * mpl vectors if multiple inputs are present)
   */
  typedef IrisDataTypes supportedInputTypes;

  /*!
   * we have no output, so empty vector here.
   * \todo we should typdef the empty mpl::vector<> to some name for convenience.
   */
  typedef boost::mpl::vector<> supportedOutputTypes;

  /*!
	 * Constructor - call the constructor on TemplatePNComponent and pass in all details
	 * about the component.	Register all parameters and events.
	 * \param name the name given to this component in the radio config
	 */
  FileRawWriterTemplateComponent(std::string name);

  /*!
	 * Destructor - clean up.
	 */
	virtual ~FileRawWriterTemplateComponent(){};

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
	 * Creates a new instance of the implementation class,
	 * with the correct template parameters.
	 * Also copies all values of the parameters to the newly
	 * created instance.
	 * \param comp the component used to create the instance
	 * \return the created component instance
	 */
  template <typename Tin, typename Tout>
  static PNComponent* createInstance(const PNComponent* comp)
  {
    return new FileRawWriterTemplateComponentImpl<Tin,Tout>(*comp);
  }

};

/*!
 * \brief The FileRawWriterComponent implementation
 *
 * The actual implementation of the FileRawWriter - implemented as template
 */
template <class Tin, class Tout>
class FileRawWriterTemplateComponentImpl : public FileRawWriterTemplateComponent
{
private:
	//! The file stream
  std::ofstream hOutFile;

  //! the type of the first input port (we have only one anyway)
  typedef typename boost::mpl::front<Tin>::type T;
  //! convenience pointer for incoming DataSet buffer
  ReadBuffer<T>* inBuf;
  //! convenience pointer for incoming DataSet
  DataSet<T>* readDataSet;

public:
  /*!
	 * Constructor - call the constructor on parent
	 * and assign all values from other.
	 * \param other the PNComponent with correct i/o datatypes
	 */
  FileRawWriterTemplateComponentImpl(const PNComponent& other) :
    FileRawWriterTemplateComponent(other.getName())
  {
    // assign all values from other to this
    assign(other);
  }

  /*!
	 * Destructor - clean up.
	 */
	~FileRawWriterTemplateComponentImpl();

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

}
#endif /* FILERAWWRITERTEMPLATECOMPONENT_H_ */
