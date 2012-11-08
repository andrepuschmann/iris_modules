/*
 * This file is part of Iris 2.
 *
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin.
 * All rights reserved.
 *
 */



/**
 * \file MatlabTemplateComponent.h
 *
 *  Created on: 10-Mar-2010
 *  Created by: suttonp
 *  $Revision: 1525 $
 *  $LastChangedDate: 2012-07-20 10:40:43 +0100 (Fri, 20 Jul 2012) $
 *  $LastChangedBy: suttonp $
 *
 */

#ifndef MATLABTEMPLATECOMPONENT_H_
#define MATLABTEMPLATECOMPONENT_H_

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
class MatlabTemplateComponent: public TemplatePNComponent<MatlabTemplateComponent>
{
protected:
	//! Matlab script to be called
    std::string x_scriptName;
    //! Does this component output data?
	bool x_hasOutput;
	//! Does this component pass data through?
	bool x_passThrough;

	//! The Matlab engine
	Matlab matlab;  
	//! Array for Matlab input
	mxArray *matlab_input;
	//! Array for Matlab output
	mxArray *matlab_output;

	//! Command used to execute Matlab script
	std::string command;

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

};

/*!
 * \brief The MatlabComponent implementation
 *
 * The actual implementation of the MatlabComponent - implemented as template
 */
template <class Tin, class Tout>
class MatlabTemplateComponentImpl : public MatlabTemplateComponent
{
private:

    //! the type of the first input port (we have only one anyway)
    typedef typename boost::mpl::front<Tin>::type T;

    //Convenience pointers to DataSets and Buffers
    ReadBuffer<T>* inBuf;
    DataSet<T>* readDataSet;
	WriteBuffer<T>* outBuf;
	DataSet<T>* writeDataSet;

	//! Buffer to capture messages from Matlab
	char buffer[256];

	/*!
	 * This captures boost::mpl::void_ template instantiations
	 * \param data the data to be copied
	 * \param matlab_input the target for the data to be copied
	 */
	void copyInData(std::vector<boost::mpl::void_> &data, mxArray* matlab_input)
	{
	}

	/*!
	 * Template specialization to handle complex data
	 * \param data the data to be copied
	 * \param matlab_input the target for the data to be copied
	 */
	template<class U>
	void copyInData(std::vector< std::complex<U> > &data, mxArray* matlab_input)
	{
		double* pR = mxGetPr(matlab_input);
		double* pI = mxGetPi(matlab_input);
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
	void copyInData(std::vector<U> &data, mxArray* matlab_input)
	{
		double* ptr = mxGetPr(matlab_input);
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
	void copyOutData(mxArray* matlab_output, std::vector<boost::mpl::void_> &data)
	{
	}

	/*!
	 * Template specialization to handle complex data
	 * \param data the data to be copied
	 * \param matlab_input the target for the data to be copied
	 */
	template<class U>
	void copyOutData(mxArray* matlab_output, std::vector< std::complex<U> > &data)
	{
		double* pR = mxGetPr(matlab_output);
		double* pI = mxGetPi(matlab_output);
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
	void copyOutData(mxArray* matlab_output, std::vector<U> &data)
	{
		double* ptr = mxGetPr(matlab_output);
		for(size_t i=0;i<data.size();i++)
		{
			data[i] = (U)ptr[i];
		}
	}

public:
	/*!
	 * Constructor - call the constructor on parent
	 * and assign all values from other.
	 * \param other the PNComponent with correct i/o datatypes
	 */
    MatlabTemplateComponentImpl(const PNComponent& other) :
        MatlabTemplateComponent(other.getName())
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
};

}	/* namepace iris */
#endif	/* MATLABTEMPLATECOMPONENT_H_ */
