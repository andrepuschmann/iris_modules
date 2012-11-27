/**
 * \file components/gpp/phy/Example/ExampleComponent.h
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
 * An example PhyComponent to be used as a template for new PhyComponents.
 */

#ifndef PHY_EXAMPLECOMPONENT_H_
#define PHY_EXAMPLECOMPONENT_H_

#include "irisapi/PhyComponent.h"

namespace iris
{
namespace phy
{

/** An example PhyComponent to be used as a template for new PhyComponents.
 *
 * Copy this component, rename folder, .h and .cpp files.
 * Edit all files for your new component and build.
 */
class ExampleComponent
  : public PhyComponent
{
 public:
  ExampleComponent(std::string name);
  virtual void calculateOutputTypes(
        const std::map<std::string, int>& inputTypes,
        std::map<std::string, int>& outputTypes);
  virtual void registerPorts();
  virtual void initialize();
  virtual void process();

 private:
   /// Convenience pointer for incoming DataSet buffer
   ReadBuffer<uint32_t>* inBuf_;
   /// Convenience pointer for outgoing DataSet buffer
   WriteBuffer< uint32_t >* outBuf_;

   /// Example of a parameter which will be exposed by this component
   uint32_t example_x;
};

} // namespace phy
} // namespace iris

#endif // PHY_EXAMPLECOMPONENT_H_
