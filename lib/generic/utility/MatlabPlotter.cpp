/**
 * \file lib/generic/utility/MatlabPlotter.cpp
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
 * Implementation of the MatlabPlotter class.
 * MatlabPlotter is built as a static library - this allows us to
 * use an rpath to specify the directory containing matlab-specific
 * libraries and avoid clashes with system libraries.
 */

#include "MatlabPlotter.h"
#include <irisapi/Exceptions.h>

MatlabPlotter::MatlabPlotter()
{
  if(!matlab_.open(""))
  {
    std::cout << "[ERROR] MatlabPlotter: Failed to open matlab engine." << std::endl; //TODO: throw exception
    throw iris::ResourceNotFoundException("Failed to start Matlab engine");
  }
  complexCommand_ ="figure; \
                    subplot(4,1,1);plot(real(data));title('Real'); \
                    subplot(4,1,2);plot(imag(data));title('Imag'); \
                    subplot(4,1,3);plot(abs(data));title('Magnitude'); \
                    subplot(4,1,4);plot(angle(data));title('Phase');";
  scalarCommand_ = "figure; \
                    plot(data);";
};
