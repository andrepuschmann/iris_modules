/*
 * This file is part of Iris 2.
 * 
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the 
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin. 
 * All rights reserved.
 * 
 */



/**
 * \file FileRawReaderComponent.h
 * Read data from file
 *
 *  Created on: 19-Jan-2009
 *  Created by: suttonp
 *  $Revision: 1146 $
 *  $LastChangedDate: 2010-10-28 19:04:17 +0100 (Thu, 28 Oct 2010) $
 *  $LastChangedBy: suttonp $
 *
 */

#ifndef FILERAWREADERCOMPONENT_H_
#define FILERAWREADERCOMPONENT_H_

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
private:
    //! size of blocks to read from file
    int x_blockSize;
    //! name of file to read
    std::string x_fileName;
    //! interpret the data as this data type
    std::string x_dataType;
    //! endianness of the data
    std::string x_endian;

	//! the file stream
    std::ifstream hInFile;
    
	//! template function used to read the data
    template<typename T> void readBlock();


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
};

}
#endif /* FILERAWREADERCOMPONENT_H_ */

