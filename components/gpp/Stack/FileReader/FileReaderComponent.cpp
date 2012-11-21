/**
 * \file FileReaderComponent.cpp
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
 * Implementation of a source stack component which reads data from file.
 */

#include "FileReaderComponent.h"

#include <algorithm>
#include <boost/scoped_array.hpp>

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "utility/EndianConversion.h"

using namespace std;

namespace iris
{

//! Export library symbols
IRIS_COMPONENT_EXPORTS(StackComponent, FileReaderComponent);

FileReaderComponent::FileReaderComponent(std::string name)
  : StackComponent(name, "filereaderstackcomponent",
                   "A simple stack component for reading bytes from a file",
                   "Colman O'Sullivan & Paul Sutton",
                   "0.1")
  ,count_(0)
{
  //Format: registerParameter(name, description, default, dynamic?, parameter, allowed values);
  registerParameter("filename", "The file to read", "temp.bin", false, fileName_x);
  registerParameter("blocksize", "Size of output blocks", "1024", true, blockSize_x, Interval<uint32_t>(1, 1024000));
  registerParameter("sendbelow", "Send data below rather than above", "true", false, sendBelow_x);
  registerParameter("delay","Delay in us between reads","100000",true, delay_x, Interval<uint32_t>(0,5000000));
  registerParameter("enabled","Dynamic parameter to pause or resume file reading.", "true", true, enabled_x);
  registerParameter("intermittentpauselength","Number of iterations to pause reading for (0 means no intermittent pauses).","0",false,intermittentPauseLength_x,Interval<uint32_t>(0,5000));
  registerParameter("packets","How many packets to be sent","500",true,packets_x);

  registerEvent("transmitbegin","Event indicating transmitter should be active",TypeInfo<int32_t>::identifier);
  registerEvent("transmitend","Event indicating transmission has ended",TypeInfo<int32_t>::identifier);
}

//! Do any initialization required
void FileReaderComponent::initialize()
{
  hInFile_.open(fileName_x.c_str(), ios::in|ios::binary|ios::ate);
  if (hInFile_.fail() || hInFile_.bad() || !hInFile_.is_open())
  {
      LOG(LFATAL) << "Could not open file " << fileName_x << " for reading.";
      throw ResourceNotFoundException("Could not open file " + fileName_x + " for reading.");
  }
  hInFile_.seekg(0, ios::beg);
}

/*! Process a message from above
*
*    This example just passes data through.
*/
void FileReaderComponent::processMessageFromAbove(boost::shared_ptr<StackDataSet> set)
{
  sendDownwards(set);
}

/*! Process a message from below
*
*    This example just passes data through.
*/
void FileReaderComponent::processMessageFromBelow(boost::shared_ptr<StackDataSet> set)
{
  sendUpwards(set);
}


void FileReaderComponent::start()
{
  thread_.reset( new boost::thread( boost::bind( &FileReaderComponent::fileReadingLoop, this ) ) );
}

void FileReaderComponent::stop()
{
  thread_->interrupt();
  thread_->join();
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
      if(enabled_x)
      {
        boost::shared_ptr<StackDataSet> readDataBuffer(new StackDataSet);
        readBlock(readDataBuffer);

        if(sendBelow_x)
        {
          sendDownwards(readDataBuffer);
        }
        else
        {
          sendUpwards(readDataBuffer);
        }
      }
      boost::this_thread::interruption_point();
      boost::this_thread::sleep(boost::posix_time::microseconds(delay_x));
      if(intermittentPauseLength_x > 0 && ++counter_ % intermittentPauseLength_x == 0)
      {
        LOG(LDEBUG) << "Toggling read loop activity for "<<intermittentPauseLength_x<<" iterations";
        enabled_x = !enabled_x;
        if(enabled_x)
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
  char *bytebuffer = new char[blockSize_x];
  char * bytebufferBackup = bytebuffer;
  ifstream::pos_type toread = blockSize_x * sizeof(uint8_t);

  //Read a block (loop if necessary)
  while( toread > 0 )
  {
    hInFile_.read(bytebuffer, toread);
    for(int i=0; i < hInFile_.gcount(); i++)
    {
      readDataBuffer->data.push_back(*bytebuffer);
      bytebuffer++;
    }
    toread -= hInFile_.gcount();
    if( hInFile_.eof() )
    {
      hInFile_.clear();
      hInFile_.seekg(0, ios::beg);
    }
  }
  delete[] bytebufferBackup;
  LOG(LDEBUG) << "One block read.";
  count_++;
  if(count_==packets_x)
  {
    thread_->interrupt();
    thread_->join();
  }
}

} // namespace iris
