/**
 * \file FileWriterComponent.h
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
 * Implementation of a sink stack component which writes data to file.
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
