/*
 * This file is part of Iris 2.
 * 
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the 
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin. 
 * All rights reserved.
 * 
 */



/**
 * \file UdpSocketRxComponent.cpp
 *
 *  Created on: 9-Jan-2009
 *  Created by: suttonp
 *  $Revision: 714 $
 *  $LastChangedDate: 2009-10-20 20:14:02 +0100 (Tue, 20 Oct 2009) $
 *  $LastChangedBy: lotzej $
 */

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "UdpSocketRxComponent.h"

using namespace std;
using namespace boost::asio::ip;

namespace iris
{
	// export library symbols
	IRIS_COMPONENT_EXPORTS(PNComponent, UdpSocketRxComponent);

    UdpSocketRxComponent::UdpSocketRxComponent(string name):
        PNComponent(name, "udpsocketrxpncomponent", "A udp socket rx", "Paul Sutton", "0.1")
		,bStopping(false)
    {
        //Register all parameters
        //format:        (name,   description,     default,   dynamic, parameter, list/Interval)
        registerParameter("port", "Port to listen on", "1234", false, x_port);
        registerParameter("bufferSize", "The size of the buffer used to receive datagrams", "1316", false, x_bufferSize);
        registerParameter("outputType", "The type of the output", "uint8_t", false, x_outputType);
    }

    void UdpSocketRxComponent::registerPorts()
    {
        //Register all ports
        vector<int> validTypes; //This component supports all data types
        validTypes.push_back( int(TypeInfo< uint8_t >::identifier) );
        validTypes.push_back( int(TypeInfo< uint16_t >::identifier) );
        validTypes.push_back( int(TypeInfo< uint32_t >::identifier) );
        validTypes.push_back( int(TypeInfo< uint64_t >::identifier) );
        validTypes.push_back( int(TypeInfo< int8_t >::identifier) );
        validTypes.push_back( int(TypeInfo< int16_t >::identifier) );
        validTypes.push_back( int(TypeInfo< int32_t >::identifier) );
        validTypes.push_back( int(TypeInfo< int64_t >::identifier) );
        validTypes.push_back( int(TypeInfo< float >::identifier) );
        validTypes.push_back( int(TypeInfo< double >::identifier) );
        validTypes.push_back( int(TypeInfo< long double >::identifier) );
        validTypes.push_back( int(TypeInfo< complex<float> >::identifier) );
        validTypes.push_back( int(TypeInfo< complex<double> >::identifier) );
        validTypes.push_back( int(TypeInfo< complex<long double> >::identifier) );

        //format:        (name, vector of valid types)
        registerOutputPort("output1", validTypes);
    }

    map<string, int> UdpSocketRxComponent::calculateOutputTypes(map<string, int> inputTypes)
    {
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

    void UdpSocketRxComponent::initialize()
    {
        //Create our buffer
        buffer = new char[x_bufferSize];

        //Create socket
        try{
            d_socket = new boost::asio::ip::udp::socket(d_ioService);
        }
        catch(boost::system::system_error &e)
        {
            LOG(LERROR) << "Failed to create socket: " << e.what();
        }

    }

    void UdpSocketRxComponent::start()
    {
        //Open socket
        try{
            d_socket->open(udp::v4());
            d_socket->bind(udp::endpoint(udp::v4(), x_port));
        }
        catch(boost::system::system_error &e)
        {
            LOG(LERROR) << "Failed to open socket: " << e.what();
        }
    }

    void UdpSocketRxComponent::process()
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
    void UdpSocketRxComponent::writeOutput()
    {
		if(!bStopping)
		{
			//Get data from socket
			std::size_t size;
			try{
				udp::endpoint sender_endpoint;
				size = d_socket->receive_from(boost::asio::buffer(buffer, x_bufferSize), sender_endpoint);
			}catch(boost::system::system_error &e)
			{
				if(!bStopping)
				{
					LOG(LERROR) << "Error receiving from socket: " << e.what();
				}
				return;
			}

			//Check that we've an integer number of data types in the datagram
			float f = size/(float)sizeof(T);
			float intpart, rem;
			rem = modf(f, &intpart);
			if(rem != 0)
			{
				LOG(LERROR) << "Did not receive an integer number of elements - data will be lost";
			}
			int numT = (int)intpart;

			//Get the output buffer
			WriteBuffer< T >* outBuf = castToType<T>(outputBuffers[0]);
			DataSet<T>* writeDataSet = NULL;
			outBuf->getWriteData(writeDataSet, numT);

			//Copy data into output
			T* bufT = (T*)buffer;
			copy(bufT, bufT+numT, writeDataSet->data.begin());

			//Release the buffer
			outBuf->releaseWriteData(writeDataSet);
		}
    }

    void UdpSocketRxComponent::stop()
    {
        //Close socket
        try{
			bStopping = true;
            d_socket->shutdown(udp::socket::shutdown_receive);
            d_socket->close();
        }
        catch(boost::system::system_error &e)
        {
            LOG(LERROR) << "Failed to close socket: " << e.what();
        }
    }

	UdpSocketRxComponent::~UdpSocketRxComponent()
	{
		//Destroy our buffer
        delete [] buffer;

        //Destroy socket
        delete d_socket;
	}

} /* namespace iris */
