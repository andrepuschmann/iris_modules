/*
 * This file is part of Iris 2.
 * 
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the 
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin. 
 * All rights reserved.
 * 
 */



/**
 * \file UdpSocketRxComponent.h
 * A UDP socket receiver
 *
 *  Created on: 30-Jan-2009
 *  Created by: suttonp
 *  $Revision: 1168 $
 *  $LastChangedDate: 2010-11-01 15:11:49 +0000 (Mon, 01 Nov 2010) $
 *  $LastChangedBy: suttonp $
 *
 */

#ifndef UDPSOCKETRXCOMPONENT_H_
#define UDPSOCKETRXCOMPONENT_H_

#include "irisapi/PNComponent.h"

//For boost asio sockets
#include <boost/asio.hpp>

namespace iris
{

/*!
 * \brief A UDP socket receiver
 *
 * The UdpSocketRxComponent receives data from a UDP socket.
 */
class UdpSocketRxComponent: public PNComponent
{
private:
	//! The port to receive from
    unsigned short x_port;
    //! The size of the buffer used to receive datagrams
    unsigned int x_bufferSize;
    //! The data type of output data
    std::string x_outputType;

    int d_outputTypeId;
    boost::asio::io_service d_ioService;
    boost::asio::ip::udp::socket* d_socket;
    char* buffer;
	bool bStopping;

    template<typename T> void writeOutput();
public:
	/*!
	 * Call the constructor on PNComponent and pass in all details
	 * about the component.	Register all parameters and events.
	 * \param name the name given to this component in the radio config
	 */
    UdpSocketRxComponent(std::string name);

	/*!
	 * Destructor - clean up
	 */
	~UdpSocketRxComponent();

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

#endif /* UDPSOCKETRXCOMPONENT_H_ */
