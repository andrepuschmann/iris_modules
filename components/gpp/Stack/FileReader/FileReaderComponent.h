/**
 * \file FileReaderComponent.h
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
 * A source stack component which reads data from file.
 */

#ifndef STACK_FILEREADERCOMPONENT_H_
#define STACK_FILEREADERCOMPONENT_H_

#include <fstream>
#include "irisapi/StackComponent.h"


namespace iris
{

class FileReaderComponent
  : public StackComponent
{
public:
  /*! Constructor
  *
  *    Call the constructor on StackComponent and pass in all details about the component.
  *    Register all parameters and events in the constructor.
  *
  *   \param  name    The name assigned to this component when loaded
  */
  FileReaderComponent(std::string name);

  virtual void initialize();
  virtual void start();
  virtual void stop();
  virtual void processMessageFromAbove(boost::shared_ptr<StackDataSet> set);
  virtual void processMessageFromBelow(boost::shared_ptr<StackDataSet> set);

private:
  //Private functions
  void readBlock(boost::shared_ptr<StackDataSet> readDataBuffer);
  void fileReadingLoop();

  //Exposed parameters
  uint32_t blockSize_x;
  std::string fileName_x;
  bool sendBelow_x;
  bool enabled_x;
  uint32_t delay_x;
  uint32_t intermittentPauseLength_x;
  int packets_x;

  int count_;

  //\todo Remove these filthy hacks
  uint32_t counter_;

  //! Handle for file reading thread
  boost::scoped_ptr< boost::thread > thread_;
  
  //The file stream
  std::ifstream hInFile_;
};

} // namespace iris

#endif // STACK_FILEREADERCOMPONENT_H_
