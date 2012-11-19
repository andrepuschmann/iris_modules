/**
 * @file ExampleController.h
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
 * An example controller to be used when creating new controllers.
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
