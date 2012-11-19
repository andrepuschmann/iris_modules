/**
 * @file FileReaderComponent.h
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
 * A source stack component which reads data from file.
 */

#ifndef FILEREADERCOMPONENT_H_
#define FILEREADERCOMPONENT_H_

#include <fstream>
#include "irisapi/StackComponent.h"


namespace iris
{

class FileReaderComponent: public StackComponent
{
private:
	//Exposed parameters
	uint32_t x_blockSize;
    std::string x_fileName;
	bool x_sendBelow;
	bool x_enabled;
	uint32_t x_delay;
	uint32_t x_intermittentPauseLength;
	int x_packets;
	int count;

	//@todo Remove these filthy hacks
	uint32_t d_counter;

	//! Handle for file reading thread
    boost::scoped_ptr< boost::thread > d_thread;

	//The file stream
    std::ifstream hInFile;
    
	//Private function
    void readBlock(boost::shared_ptr<StackDataSet> readDataBuffer);
	void fileReadingLoop();

public:
    FileReaderComponent(std::string name);
    virtual void initialize();
	virtual void start();
	virtual void stop();
    virtual void processMessageFromAbove(boost::shared_ptr<StackDataSet> set);
	virtual void processMessageFromBelow(boost::shared_ptr<StackDataSet> set);
};

} /* namespace iris */

#endif /* FILEREADERCOMPONENT_H_ */
