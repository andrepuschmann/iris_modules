/*
 * This file is part of Iris 2.
 *
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin.
 * All rights reserved.
 *
 */



/**
 * \file ExampleComponent.h
 * An example StackComponent to be used as a template for new StackComponents
 *
 *  Created on: 2-Mar-2010
 *  Created by: suttonp
 *  $Revision: 1095 $
 *  $LastChangedDate: 2010-10-22 10:36:32 +0100 (Fri, 22 Oct 2010) $
 *  $LastChangedBy: suttonp $
 *
 */

#ifndef EXAMPLECOMPONENT_H_
#define EXAMPLECOMPONENT_H_

#include "irisapi/StackComponent.h"

namespace iris
{

class ExampleComponent: public StackComponent
{
private:
	//Exposed parameters
	uint32_t x_example;

public:
    ExampleComponent(std::string name);
    virtual void initialize();
    virtual void start();
    virtual void stop();
    virtual void processMessageFromAbove(boost::shared_ptr<StackDataSet> set);
	virtual void processMessageFromBelow(boost::shared_ptr<StackDataSet> set);
};

} /* namespace iris */

#endif /* EXAMPLECOMPONENT_H_ */
