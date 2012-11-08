/*
 * This file is part of Iris 2.
 *
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin.
 * All rights reserved.
 *
 */



/**
 * \file FileWriterComponent.h
 * An example StackComponent to be used as a template for new StackComponents
 *
 *  Created on: 2-Mar-2010
 *  Created by: suttonp
 *  $Revision: 936 $
 *  $LastChangedDate: 2010-06-08 17:51:18 +0100 (Tue, 08 Jun 2010) $
 *  $LastChangedBy: cosull13 $
 *
 */

#ifndef FILEWRITERCOMPONENT_H_
#define FILEWRITERCOMPONENT_H_

#include <fstream>

#include "irisapi/StackComponent.h"

namespace iris
{

class FileWriterComponent: public StackComponent
{
private:
	//Exposed parameters
    std::string x_fileName;
	bool x_fromBelow;

    std::ofstream hOutFile;
	void writeBlock(boost::shared_ptr<StackDataSet> toWrite);

public:
    FileWriterComponent(std::string name);
    virtual void initialize();
    virtual void processMessageFromAbove(boost::shared_ptr<StackDataSet> set);
	virtual void processMessageFromBelow(boost::shared_ptr<StackDataSet> set);
};

} /* namespace iris */

#endif /* FILEWRITERCOMPONENT_H_ */
