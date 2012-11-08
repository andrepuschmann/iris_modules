/*
 * This file is part of Iris 2.
 * 
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the 
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin. 
 * All rights reserved.
 * 
 */



/**
 * \file FileRawWriterComponent.h
 * Write data to a file
 *
 *  Created on: 19-Jan-2009
 *  Created by: suttonp
 *  $Revision: 1147 $
 *  $LastChangedDate: 2010-10-28 19:04:29 +0100 (Thu, 28 Oct 2010) $
 *  $LastChangedBy: suttonp $
 *
 */

#ifndef FILERAWWRITERCOMPONENT_H_
#define FILERAWWRITERCOMPONENT_H_

#include <fstream>

#include "irisapi/PNComponent.h"

namespace iris
{

/*!
 * \brief The FileRawWriterComponent writes data to a file
 */
class FileRawWriterComponent: public PNComponent
{
private:
    //! name of file to write to
    std::string x_fileName;
    //! endianness of data
	std::string x_endian;

	//! the output file stream
    std::ofstream hOutFile;

    //! template function to write data
    template<typename T> void writeBlock();

public:
    /*!
	 * Call the constructor on PNComponent and pass in all details
	 * about the component.	Register all parameters and events.
	 * \param name the name given to this component in the radio config
	 */
    FileRawWriterComponent(std::string name);

    /*!
	 * Destructor - clean up.
	 */
	~FileRawWriterComponent();

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

}
#endif /* FILERAWWRITERCOMPONENT_H_ */
