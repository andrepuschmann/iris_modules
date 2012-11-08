/*
 * This file is part of Iris 2.
 * 
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the 
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin. 
 * All rights reserved.
 * 
 */



/**
 * \file TcpSocketRxComponent.h
 * A TCP socket receiver
 *
 *  Created on: 2-Feb-2009
 *  Created by: suttonp
 *  $Revision: 1165 $
 *  $LastChangedDate: 2010-11-01 15:11:15 +0000 (Mon, 01 Nov 2010) $
 *  $LastChangedBy: suttonp $
 *
 */

#ifndef TCPSOCKETRXCOMPONENT_H_
#define TCPSOCKETRXCOMPONENT_H_

#include "irisapi/PNComponent.h"

//For boost asio sockets
#include <boost/asio.hpp>

namespace iris
{

/*!
 * \brief A TCP socket receiver
 *
 * The TcpSocketRxComponent receives data from a TCP socket.
 */
class TcpSocketRxComponent: public PNComponent
{
private:
	//! Port number to bind to
    unsigned short x_port;
    //! Size of buffers to be generated
    unsigned int x_bufferSize;
    //! Data type of output
    std::string x_outputType;

    //! The ID of the output data type
    int d_outputTypeId;

    boost::asio::io_service d_ioService;
    boost::asio::ip::tcp::socket* d_socket;
    boost::asio::ip::tcp::acceptor* d_acceptor;

    char* d_buffer;
    bool d_connected;

    template<typename T> void writeOutput();
public:
	/*!
	 * Call the constructor on PNComponent and pass in all details
	 * about the component.	Register all parameters and events.
	 * \param name the name given to this component in the radio config
	 */
    TcpSocketRxComponent(std::string name);

	/*!
	 * Destructor - clean up
	 */
	~TcpSocketRxComponent();

	/*!
	 * Given the data-types associated with each input port, provide
	 * the data-types which will be produced on each output port.
	 * \param inputTypes the map of input port names and data-type identifiers
	 * \return map of output port names and data-type identifiers
	 */
    virtual std::map<std::string, int> calculateOutputTypes(std::map<std::string, int> inputTypes);

    /*!
	 * Register the input and output ports of this component
	 * by declaring them as input or output, naming them and
	 * providing a list of valid data types.
	 */
    virtual void registerPorts();

    /*!
	 * Do any initialization required by this component.
	 */
    virtual void initialize();

    /*!
     * Start this IO component
     */
    virtual void start();

    /*!
   	 * This is where the work of this component gets done.
   	 * Typically components will take DataSets from their input
   	 * ports, process them and write DataSets to their output ports.
   	 */
    virtual void process();

    /*!
	 * Stop this IO component
	 */
    virtual void stop();
};

} /* namespace iris */

#endif /* TCPSOCKETRXCOMPONENT_H_ */
