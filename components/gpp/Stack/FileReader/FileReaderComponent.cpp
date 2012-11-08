/*
 * This file is part of Iris 2.
 *
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin.
 * All rights reserved.
 *
 */



/**
 * \file FileReaderComponent.cpp
 *
 *  Created on: 11-Jun-2010
 *  Created by: cosull13
 *  $Revision: 1525 $
 *  $LastChangedDate: 2012-07-20 10:40:43 +0100 (Fri, 20 Jul 2012) $
 *  $LastChangedBy: suttonp $
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
