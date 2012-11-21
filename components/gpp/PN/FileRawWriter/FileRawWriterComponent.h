/**
 * \file components/gpp/PN/FileRawWriter/FileRawWriterComponent.h
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
 * Sink component to write data to file.
 */

#ifndef PN_FILERAWWRITERCOMPONENT_H_
#define PN_FILERAWWRITERCOMPONENT_H_

#include <fstream>

#include "irisapi/PNComponent.h"

namespace iris
{
namespace pn
{

/** A PNComponent to write data to a named file.
 *
 * The name of the file to write and the endianness of the data can
 * be specified using parameters.
 */
class FileRawWriterComponent: public PNComponent
{
 public:
  FileRawWriterComponent(std::string name);
	~FileRawWriterComponent();
  virtual std::map<std::string, int> calculateOutputTypes(std::map<std::string, int> inputTypes);
  virtual void registerPorts();
  virtual void initialize();
  virtual void process();

 private:
  /// template function to write data
  template<typename T> void writeBlock();

  std::string fileName_x;   ///< Name of file to write to
  std::string endian_x;     ///< Endianness of data

  std::ofstream hOutFile_;  ///< The output file stream
};

} // namespace pn
} // namespace iris

#endif // PN_FILERAWWRITERCOMPONENT_H_
