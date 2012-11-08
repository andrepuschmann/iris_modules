/*
 * This file is part of Iris 2.
 *
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin.
 * All rights reserved.
 *
 */



/**
 * \file FileWriterComponent.cpp
 *
 *  Created on: 2-Mar-2010
 *  Created by: suttonp
 *  $Revision: 1525 $
 *  $LastChangedDate: 2012-07-20 10:40:43 +0100 (Fri, 20 Jul 2012) $
 *  $LastChangedBy: suttonp $
 */

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "FileWriterComponent.h"
#include "utility/RawFileUtility.h"

using namespace std;

namespace iris
{
	// export library symbols
	IRIS_COMPONENT_EXPORTS(StackComponent, FileWriterComponent);

	/*! Constructor
	*
	*	Call the constructor on StackComponent and pass in all details about the component.
	*	Register all parameters and events in the constructor.
    *
    *   \param  name	The name assigned to this component when loaded
    */
	FileWriterComponent::FileWriterComponent(std::string name):
        StackComponent(name, "filewriterstackcomponent", "A simple stack component for writing bytes to a file", "Colman O'Sullivan & Paul Sutton", "0.1")
    {
        registerParameter("filename", "The file to read", "temp.bin", false, x_fileName);
		registerParameter("frombelow","Write data from below rather than above","true",false,x_fromBelow);
    }

	//! Do any initialization required
    void FileWriterComponent::initialize()
    {
        hOutFile.open(x_fileName.c_str(), ios::out|ios::binary);
        if (hOutFile.fail() || hOutFile.bad() || !hOutFile.is_open())
        {
            LOG(LFATAL) << "Could not open file " << x_fileName << " for writing.";
			throw ResourceNotFoundException("Could not open file " + x_fileName + " for writing.");
        }
		LOG(LDEBUG) << "FileWriterComponent:: initialize() [sic] completed successfully.";
    }

	/*! Process a message from above
	*
	*	This example just passes data through.
    */
	void FileWriterComponent::processMessageFromAbove(boost::shared_ptr<StackDataSet> set)
	{
		LOG(LDEBUG) << "FileWriter::processMessageFromAbove() called.";
		if(!x_fromBelow)
		{
			writeBlock(set);
		}
		sendDownwards(set);
	}

	/*! Process a message from below
	*
	*	This example just passes data through.
    */
	void FileWriterComponent::processMessageFromBelow(boost::shared_ptr<StackDataSet> set)
	{
		LOG(LDEBUG) << "FileWriter::processMessageFromBelow() called.";
		if(x_fromBelow)
		{
			writeBlock(set);
		}
		sendUpwards(set);
	}


	void FileWriterComponent::writeBlock(boost::shared_ptr<StackDataSet> toWrite)
    {
        //Write to file
		RawFileUtility::write(toWrite->data.begin(), toWrite->data.end(), hOutFile);
    }
} /* namespace iris */
