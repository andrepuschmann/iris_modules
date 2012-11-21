/**
 * \file MatlabTemplateComponent.h
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
 * A template component used to read/write data to a matlab script.
 */

#ifndef PN_MATLABTEMPLATECOMPONENT_H_
#define PN_MATLABTEMPLATECOMPONENT_H_

#include "irisapi/TemplatePNComponent.h"
#include "utility/Matlab.h"

namespace iris
{

// forward declaration
template <class Tin, class Tout>
class MatlabTemplateComponentImpl;


/*!
 * \brief A template component used to interface with Matlab
 *
 * The MatlabTemplateComponent is derived from TemplatePNComponent,
 * so we need to give the class name itself as a template parameter.
 * This component passes data to a named Matlab script and optionally
 * accepts returned data to be passed on.
 */
class MatlabTemplateComponent
  : public TemplatePNComponent<MatlabTemplateComponent>
{
 public:
	/*!
	 * supported types at the input port
	 * (can be a boost::mpl::vector of types, or of other
	 * mpl vectors if multiple inputs are present)
	 */
  typedef IrisDataTypes supportedInputTypes;

  //! supported types at the output port
  typedef IrisDataTypes supportedOutputTypes;

  /*!
	 * Constructor - call the constructor on TemplatePNComponent and pass in all details
	 * about the component.	Register all parameters and events.
	 * \param name the name given to this component in the radio config
	 */
  MatlabTemplateComponent(std::string name);

  /*!
	 * Destructor - clean up.
	 */
	virtual ~MatlabTemplateComponent();

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
    return new MatlabTemplateComponentImpl<Tin,Tout>(*comp);
  }

 protected:
  //! Matlab script to be called
  std::string scriptName_x;
  //! Does this component output data?
  bool hasOutput_x;
  //! Does this component pass data through?
  bool passThrough_x;

  //! The Matlab engine
  Matlab matlab_;
  //! Array for Matlab input
  mxArray *matlabInput_;
  //! Array for Matlab output
  mxArray *matlabOutput_;

  //! Command used to execute Matlab script
  std::string command_;
};

/*!
 * \brief The MatlabComponent implementation
 *
 * The actual implementation of the MatlabComponent - implemented as template
 */
template <class Tin, class Tout>
class MatlabTemplateComponentImpl
  : public MatlabTemplateComponent
{
 public:
	/*!
	 * Constructor - call the constructor on parent
	 * and assign all values from other.
	 * \param other the PNComponent with correct i/o datatypes
	 */
  MatlabTemplateComponentImpl(const PNComponent& other)
    : MatlabTemplateComponent(other.getName())
  {
    // assign all values from other to this
    assign(other);
  }

  /*!
	 * Destructor - clean up.
	 */
	~MatlabTemplateComponentImpl();

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
  //! the type of the first input port (we have only one anyway)
  typedef typename boost::mpl::front<Tin>::type T;

  /*!
  * This captures boost::mpl::void_ template instantiations
  * \param data the data to be copied
  * \param matlab_input the target for the data to be copied
  */
  void copyInData(std::vector<boost::mpl::void_> &data, mxArray* matlabInput)
  {}

  /*!
  * Template specialization to handle complex data
  * \param data the data to be copied
  * \param matlab_input the target for the data to be copied
  */
  template<class U>
  void copyInData(std::vector< std::complex<U> > &data, mxArray* matlabInput)
  {
    double* pR = mxGetPr(matlabInput);
    double* pI = mxGetPi(matlabInput);
    for(size_t i=0;i<data.size();i++)
    {
     pR[i] = (double)data[i].real();
     pI[i] = (double)data[i].imag();
    }
  }

  /*!
  * Copy data from our vector into the mxArray container used by Matlab
  * \param data the data to be copied
  * \param matlab_input the target for the data to be copied
  */
  template<class U>
  void copyInData(std::vector<U> &data, mxArray* matlabInput)
  {
    double* ptr = mxGetPr(matlabInput);
    for(size_t i=0;i<data.size();i++)
    {
     ptr[i] = (double)data[i];
    }
  }

  /*!
  * This captures boost::mpl::void_ template instantiations
  * \param data the data to be copied
  * \param matlab_input the target for the data to be copied
  */
  void copyOutData(mxArray* matlabOutput, std::vector<boost::mpl::void_> &data)
  {}

  /*!
  * Template specialization to handle complex data
  * \param data the data to be copied
  * \param matlab_input the target for the data to be copied
  */
  template<class U>
  void copyOutData(mxArray* matlabOutput, std::vector< std::complex<U> > &data)
  {
    double* pR = mxGetPr(matlabOutput);
    double* pI = mxGetPi(matlabOutput);
    for(size_t i=0;i<data.size();i++)
    {
     data[i] = std::complex<U>((U)pR[i], (U)pI[i]);
    }
  }

  /*!
  * Copy data from the mxArray container used by Matlab into our vector
  * \param data the data to be copied
  * \param matlab_input the target for the data to be copied
  */
  template<class U>
  void copyOutData(mxArray* matlabOutput, std::vector<U> &data)
  {
    double* ptr = mxGetPr(matlabOutput);
    for(size_t i=0;i<data.size();i++)
    {
     data[i] = (U)ptr[i];
    }
  }

  //Convenience pointers to DataSets and Buffers
  ReadBuffer<T>* inBuf_;
  DataSet<T>* readDataSet_;
  WriteBuffer<T>* outBuf_;
  DataSet<T>* writeDataSet_;

  //! Buffer to capture messages from Matlab
  char buffer_[256];
};

}	// namepace iris

#endif	// PN_MATLABTEMPLATECOMPONENT_H_
