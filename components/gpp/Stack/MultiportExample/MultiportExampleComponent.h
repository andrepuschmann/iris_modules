/*
 * This file is part of Iris 2.
 *
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin.
 * All rights reserved.
 *
 */

/**
 * \file MultiportExampleComponent.h
 * An example StackComponent which uses multiple ports
 *
 *  Created on: 2-Mar-2010
 *  Created by: suttonp
 *  $Revision: 845 $
 *  $LastChangedDate: 2010-03-04 13:21:23 +0000 (Thu, 04 Mar 2010) $
 *  $LastChangedBy: suttonp $
 *
 */

#ifndef MULTIPORTEXAMPLECOMPONENT_H_
#define MULTIPORTEXAMPLECOMPONENT_H_

#include "irisapi/StackComponent.h"

namespace iris
{

class MultiportExampleComponent: public StackComponent
{
private:
	//Exposed parameters
	uint32_t x_example;

public:
    MultiportExampleComponent(std::string name);
    virtual void registerPorts(); //We're overriding this to specify multiple ports
    virtual void initialize();
    virtual void processMessageFromAbove(boost::shared_ptr<StackDataSet> set);
	virtual void processMessageFromBelow(boost::shared_ptr<StackDataSet> set);
};

} /* namespace iris */

#endif /* EXAMPLECOMPONENT_H_ */
