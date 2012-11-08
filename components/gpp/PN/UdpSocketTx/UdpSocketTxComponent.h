/*
 * This file is part of Iris 2.
 * 
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the 
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin. 
 * All rights reserved.
 * 
 */



/**
 * \file UdpSocketTxComponent.h
 * A UDP Socket transmitter
 *
 *  Created on: 30-Jan-2009
 *  Created by: suttonp
 *  $Revision: 1169 $
 *  $LastChangedDate: 2010-11-01 15:12:01 +0000 (Mon, 01 Nov 2010) $
 *  $LastChangedBy: suttonp $
 *
 */

#ifndef UDPSOCKETTXCOMPONENT_H_
#define UDPSOCKETTXCOMPONENT_H_

#include "irisapi/PNComponent.h"

//For boost asio sockets
#include <boost/asio.hpp>

namespace iris
{

/*!
 * \brief Transmit data over a UDP socket
 *
 * The UdpSocketTxComponent transmits data over a UDP socket
 * to a specified IP address and port.
 */
class UdpSocketTxComponent: public PNComponent
{
private:
	//! The IP address to send to
    std::string x_address;
    //! The destination port number
    unsigned short x_port;

    boost::asio::io_service d_ioService;
    boost::asio::ip::udp::socket* d_socket;
    boost::asio::ip::udp::endpoint* d_endPoint;
    template<typename T> void writeOutput();
public:
	/*!
	 * Call the constructor on PNComponent and pass in all details
	 * about the component.	Register all parameters and events.
	 * \param name the name given to this component in the radio config
	 */
    UdpSocketTxComponent(std::string name);

	/*!
	 * Destructor - clean up
	 */
	~UdpSocketTxComponent();

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
  	 * This is where the work of this component gets done.
  	 * Typically components will take DataSets from their input
  	 * ports, process them and write DataSets to their output ports.
  	 */
    virtual void process();
};

} /* namespace iris */

#endif /* UDPSOCKETTXCOMPONENT_H_ */
