/*
 * This file is part of Iris 2.
 * 
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the 
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin. 
 * All rights reserved.
 * 
 */



/**
 * \file ExampleController.h
 * An example controller to be used when creating new controllers
 *
 *  Created on: 06-Aug-2009
 *  Created by: suttonp
 *  $Revision: 763 $
 *  $LastChangedDate: 2009-12-16 13:57:33 +0000 (Wed, 16 Dec 2009) $
 *  $LastChangedBy: suttonp $
 *
 */

#ifndef EXAMPLECONTROLLER_H_
#define EXAMPLECONTROLLER_H_

#include "irisapi/Controller.h"

namespace iris
{

class ExampleController: public Controller
{
private:
    uint32_t counter;

public:
    ExampleController();
	virtual void subscribeToEvents();
	virtual void initialize();
    virtual void processEvent(Event &e);
	virtual void destroy();
};

} /* namespace iris */

#endif /* EXAMPLECONTROLLER_H_ */
