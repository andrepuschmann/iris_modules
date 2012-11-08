/*
 * This file is part of Iris 2.
 *
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin.
 * All rights reserved.
 *
 */



/**
 * \file FileReaderComponent.h
 * An example StackComponent to be used as a template for new StackComponents
 *
 *  Created on: 11-Jun-2010
 *  Created by: cosull13
 *  $Revision: 1245 $
 *  $LastChangedDate: 2011-04-21 15:52:55 +0100 (Thu, 21 Apr 2011) $
 *  $LastChangedBy: difrancescop $
 *
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
