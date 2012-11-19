/**
 * \file TcpSocketRxComponent.cpp
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
 * Implementation of a source component which listens to a TCP socket.
 */

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "irisapi/TypeVectors.h"
#include "TcpSocketRxComponent.h"

#include <boost/thread/thread.hpp> //For sleep()

using namespace std;
using namespace boost::asio::ip;

namespace iris
{
	// export library symbols
	IRIS_COMPONENT_EXPORTS(PNComponent, TcpSocketRxComponent);

    TcpSocketRxComponent::TcpSocketRxComponent(string name):
            PNComponent(name, "tcpsocketrxpncomponent", "A TCP socket receiver", "Paul Sutton", "0.1"),
            d_buffer(NULL),
            d_connected(false)

    {
        //Register all parameters
        //format:        (name,   description,     default,   dynamic, parameter, list/Interval)
        registerParameter("port", "Port to listen on", "1234", false, x_port);
        registerParameter("bufferSize", "The size of the buffer used to receive datagrams", "1316", false, x_bufferSize);
        registerParameter("outputType", "The type of the output", "uint8_t", false, x_outputType);
    }

    void TcpSocketRxComponent::registerPorts()
    {
        //Register all ports
        //This component supports all data types
        vector<int> validTypes = convertToTypeIdVector<IrisDataTypes>();

        //format:        (name, vector of valid types)
        registerOutputPort("output1", validTypes);
    }

    map<string, int> TcpSocketRxComponent::calculateOutputTypes(map<string, int> inputTypes)
    {
        LOG(LINFO) << TypeInfo< complex<float> >::name();
        //Output type is set in the parameters
        map<string, int> outputTypes;
        if( x_outputType == TypeInfo< uint8_t >::name() )
            outputTypes["output1"] = TypeInfo< uint8_t >::identifier;
        if( x_outputType == TypeInfo< uint16_t >::name() )
            outputTypes["output1"] = TypeInfo< uint16_t >::identifier;
        if( x_outputType == TypeInfo< uint32_t >::name() )
            outputTypes["output1"] = TypeInfo< uint32_t >::identifier;
        if( x_outputType == TypeInfo< uint64_t >::name() )
            outputTypes["output1"] = TypeInfo< uint64_t >::identifier;
        if( x_outputType == TypeInfo< int8_t >::name() )
            outputTypes["output1"] = TypeInfo< int8_t >::identifier;
        if( x_outputType == TypeInfo< int16_t >::name() )
            outputTypes["output1"] = TypeInfo< int16_t >::identifier;
        if( x_outputType == TypeInfo< int32_t >::name() )
            outputTypes["output1"] = TypeInfo< int32_t >::identifier;
        if( x_outputType == TypeInfo< int64_t >::name() )
            outputTypes["output1"] = TypeInfo< int64_t >::identifier;
        if( x_outputType == TypeInfo< float >::name() )
            outputTypes["output1"] = TypeInfo< float >::identifier;
        if( x_outputType == TypeInfo< double >::name() )
            outputTypes["output1"] = TypeInfo< double >::identifier;
        if( x_outputType == TypeInfo< long double >::name() )
            outputTypes["output1"] = TypeInfo< long double >::identifier;
        if( x_outputType == TypeInfo< complex<float> >::name() )
            outputTypes["output1"] = TypeInfo< complex<float> >::identifier;
        if( x_outputType == TypeInfo< complex<double> >::name() )
            outputTypes["output1"] = TypeInfo< complex<double> >::identifier;
        if( x_outputType == TypeInfo< complex<long double> >::name() )
            outputTypes["output1"] = TypeInfo< complex<long double> >::identifier;

        d_outputTypeId = outputTypes["output1"];
        return outputTypes;

    }

    void TcpSocketRxComponent::initialize()
    {
        //Create our buffer
        d_buffer = new char[x_bufferSize];

        //Create socket and acceptor
        try{
            d_socket = new boost::asio::ip::tcp::socket(d_ioService);
            d_acceptor = new tcp::acceptor(d_ioService, tcp::endpoint(tcp::v4(), x_port));
        }
        catch(boost::system::system_error &e)
        {
            LOG(LERROR) << "Failed to create socket: " << e.what();
        }
    }

    void TcpSocketRxComponent::start()
    {
        //Open socket
        try{
            if(!d_acceptor->is_open())
                d_acceptor->open(tcp::v4());
        }
        catch(boost::system::system_error &e)
        {
            LOG(LERROR) << "Failed to open socket and accept a connection: " << e.what();
        }
    }

    void TcpSocketRxComponent::process()
    {
        if( outputBuffers.size() != 1 || inputBuffers.size() != 0)
        {
            //Need to throw an exception here
        }

        switch(d_outputTypeId)
        {
        case 0:
            writeOutput<uint8_t>();
            break;
        case 1:
            writeOutput<uint16_t>();
            break;
        case 2:
            writeOutput<uint32_t>();
            break;
        case 3:
            writeOutput<uint64_t>();
            break;
        case 4:
            writeOutput<int8_t>();
            break;
        case 5:
            writeOutput<int16_t>();
            break;
        case 6:
            writeOutput<int32_t>();
            break;
        case 7:
            writeOutput<int64_t>();
            break;
        case 8:
            writeOutput<float>();
            break;
        case 9:
            writeOutput<double>();
            break;
        case 10:
            writeOutput<long double>();
            break;
        case 11:
            writeOutput< complex<float> >();
            break;
        case 12:
            writeOutput< complex<double> >();
            break;
        case 13:
            writeOutput<complex< long double> >();
            break;
        default:
            break;
        }
    }

    template<typename T>
    void TcpSocketRxComponent::writeOutput()
    {
        std::size_t size;

        try{
            if(!d_connected)
            {
                //Listen for connection if necessary
                d_acceptor->accept(*d_socket);
                d_connected = true;
            }

            //Get data from socket
            //size = d_socket->receive(boost::asio::buffer(d_buffer, x_bufferSize));
            size = boost::asio::read(*d_socket, boost::asio::buffer(d_buffer, x_bufferSize));
        }catch(boost::system::system_error &e)
        {
            LOG(LERROR) << "Error listening on socket: " << e.what();
            boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
            return;
        }

        //Check that we've an integer number of data types in the datagram
        float f = size/(float)sizeof(T);
        float intpart, rem;
        rem = modf(f, &intpart);
        if(rem != 0)
        {
            LOG(LERROR) << "Did not receive an integer number of elements. Input size = " << size;
        }
        int numT = (int)intpart;

        //Get the output buffer
        WriteBuffer< T >* outBuf = castToType<T>(outputBuffers[0]);
        DataSet<T>* writeDataSet = NULL;
        outBuf->getWriteData(writeDataSet, numT);

        //Copy data into output
        T* bufT = (T*)d_buffer;
        copy(bufT, bufT+numT, writeDataSet->data.begin());

        //Release the buffer
        outBuf->releaseWriteData(writeDataSet);
    }

    void TcpSocketRxComponent::stop()
    {
        //Close acceptor
        try{
            d_acceptor->close();
            d_socket->close();
        }
        catch(boost::system::system_error &e)
        {
            LOG(LERROR) << "Failed to close socket: " << e.what();
        }
    }

	TcpSocketRxComponent::~TcpSocketRxComponent()
	{
		//Destroy our buffer
        delete [] d_buffer;

        //Destroy socket and acceptor
        delete d_socket;
        delete d_acceptor;
	}

} /* namespace iris */
