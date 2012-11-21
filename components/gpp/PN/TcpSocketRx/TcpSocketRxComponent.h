/**
 * \file components/gpp/PN/TcpSocketRx/TcpSocketRxComponent.h
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
 * A source component which listens to a TCP socket.
 */

#ifndef PN_TCPSOCKETRXCOMPONENT_H_
#define PN_TCPSOCKETRXCOMPONENT_H_

#include "irisapi/PNComponent.h"

//For boost asio sockets
#include <boost/asio.hpp>

namespace iris
{
namespace pn
{

/** A PNComponent which receives from a TCP socket.
 *
 * The TcpSocketRxComponent receives data from a TCP socket. The port number,
 * buffer size and data type can be specified using parameters.
 */
class TcpSocketRxComponent
  : public PNComponent
{
 public:
  TcpSocketRxComponent(std::string name);
	~TcpSocketRxComponent();
  virtual std::map<std::string, int> calculateOutputTypes(std::map<std::string, int> inputTypes);
  virtual void registerPorts();
  virtual void initialize();
  virtual void start();
  virtual void process();
  virtual void stop();

 private:
  /// Template function used to write the output.
  template<typename T> void writeOutput();

  unsigned short port_x;      ///< Port number to bind to.
  unsigned int bufferSize_x;  ///< Size of buffers to be generated.
  std::string outputType_x;   ///< Data type of output.

  int outputTypeId_;          ///< The ID of the output data type

  boost::asio::io_service ioService_;
  boost::asio::ip::tcp::socket* socket_;
  boost::asio::ip::tcp::acceptor* acceptor_;

  char* buffer_;
  bool connected_;
};

} // namespace pn
} // namespace iris

#endif // PN_TCPSOCKETRXCOMPONENT_H_
