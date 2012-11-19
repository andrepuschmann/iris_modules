/**
 * @file FileReaderComponent.cpp
 * @version 1.0
 *
 * @section COPYRIGHT
 *
 * Copyright 2012 The Iris Project Developers. See the
 * COPYRIGHT file at the top-level directory of this distribution
 * and at http://www.softwareradiosystems.com/iris/copyright.html.
 *
 * @section LICENSE
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
 * @section DESCRIPTION
 *
 * Implementation of a source stack component which reads data from file.
 */

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "FileReaderComponent.h"

#include "utility/EndianConversion.h"

#include <algorithm>
#include <boost/scoped_array.hpp>

using namespace std;

namespace iris
{
	// export library symbols
	IRIS_COMPONENT_EXPORTS(StackComponent, FileReaderComponent);

	/*! Constructor
	*
	*	Call the constructor on StackComponent and pass in all details about the component.
	*	Register all parameters and events in the constructor.
    *
    *   \param  name	The name assigned to this component when loaded
    */
	FileReaderComponent::FileReaderComponent(std::string name):
        StackComponent(name, "filereaderstackcomponent", "A simple stack component for reading bytes from a file", "Colman O'Sullivan & Paul Sutton", "0.1")
	,count(0)
    {
		//Format: registerParameter(name, description, default, dynamic?, parameter, allowed values);
        registerParameter("filename", "The file to read", "temp.bin", false, x_fileName);
        registerParameter("blocksize", "Size of output blocks", "1024", true, x_blockSize, Interval<uint32_t>(1, 1024000));
		registerParameter("sendbelow", "Send data below rather than above", "true", false, x_sendBelow);
		registerParameter("delay","Delay in us between reads","100000",true, x_delay, Interval<uint32_t>(0,5000000));
		registerParameter("enabled","Dynamic parameter to pause or resume file reading.","true",true,x_enabled);
		registerParameter("intermittentpauselength","Number of iterations to pause reading for (0 means no intermittent pauses).","0",false,x_intermittentPauseLength,Interval<uint32_t>(0,5000));
		registerParameter("packets","How many packets to be sent","500",true,x_packets);

		registerEvent("transmitbegin","Event indicating transmitter should be active",TypeInfo<int32_t>::identifier);
		registerEvent("transmitend","Event indicating transmission has ended",TypeInfo<int32_t>::identifier);
		
	}

	//! Do any initialization required
    void FileReaderComponent::initialize()
    {
        hInFile.open(x_fileName.c_str(), ios::in|ios::binary|ios::ate);
        if (hInFile.fail() || hInFile.bad() || !hInFile.is_open())
        {
            LOG(LFATAL) << "Could not open file " << x_fileName << " for reading.";
            throw ResourceNotFoundException("Could not open file " + x_fileName + " for reading.");
        }
        hInFile.seekg(0, ios::beg);
    }

	/*! Process a message from above
	*
	*	This example just passes data through.
    */
	void FileReaderComponent::processMessageFromAbove(boost::shared_ptr<StackDataSet> set)
	{
		sendDownwards(set);
	}

	/*! Process a message from below
	*
	*	This example just passes data through.
    */
	void FileReaderComponent::processMessageFromBelow(boost::shared_ptr<StackDataSet> set)
	{
		sendUpwards(set);
	}


	void FileReaderComponent::start()
	{
		d_thread.reset( new boost::thread( boost::bind( &FileReaderComponent::fileReadingLoop, this ) ) );
	}

	void FileReaderComponent::stop()
	{
		d_thread->interrupt();
        	d_thread->join();
	}

	void FileReaderComponent::fileReadingLoop()
	{
		
		int32_t x=1;
		boost::this_thread::sleep(boost::posix_time::milliseconds(100));
		try
		{
			activateEvent("transmitbegin", x);
			while(true)
			{
				if(x_enabled)
				{
					boost::shared_ptr<StackDataSet> readDataBuffer(new StackDataSet);
					readBlock(readDataBuffer);

					if(x_sendBelow)
					{
						sendDownwards(readDataBuffer);
					}
					else
					{
						sendUpwards(readDataBuffer);
					}
				}
				boost::this_thread::interruption_point();
				boost::this_thread::sleep(boost::posix_time::microseconds(x_delay));
				if(x_intermittentPauseLength > 0 && ++d_counter % x_intermittentPauseLength == 0)
				{
					LOG(LDEBUG) << "Toggling read loop activity for "<<x_intermittentPauseLength<<" iterations";
					x_enabled = !x_enabled;
					if(x_enabled)
						activateEvent("transmitbegin", x);
					else
						activateEvent("transmitend", x);
				}
			}
		}
		catch(IrisException& ex)
		{
			LOG(LFATAL) << "Error in FileReader component: " << ex.what() << " - file reading thread exiting.";
		}
	}

	void FileReaderComponent::readBlock(boost::shared_ptr<StackDataSet> readDataBuffer)
    {
        char *bytebuffer = new char[x_blockSize];
	char * bytebufferBackup = bytebuffer;
        ifstream::pos_type toread = x_blockSize * sizeof(uint8_t);

        //Read a block (loop if necessary)
        while( toread > 0 )
        {
            hInFile.read(bytebuffer, toread);
			for(int i=0; i < hInFile.gcount(); i++)
			{
				readDataBuffer->data.push_back(*bytebuffer);
				bytebuffer++;
			}
			toread -= hInFile.gcount();
            if( hInFile.eof() )
            {
                hInFile.clear();
                hInFile.seekg(0, ios::beg);
            }
        }
		delete[] bytebufferBackup;
		LOG(LDEBUG) << "One block read.";
		count++;
		if(count==x_packets)
			{
				d_thread->interrupt();
				d_thread->join();
			}
    }

} /* namespace iris */
