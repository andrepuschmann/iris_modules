/*
 * This file is part of Iris 2.
 * 
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the 
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin. 
 * All rights reserved.
 * 
 */


/**
 * \file UsrpUhdTxComponent.h
 * A Usrp transmitter using the Universal Hardware Driver (UHD).
 * This component supports streaming data by default. For bursty data, set "streaming"
 * to false. For bursty data, packets must be contained in a single DataSet.
 * If a timestamp is specified in a DataSet, the packet will be transmitted at that time,
 * if supported by the USRP.
 *
 *  Created on: 21-May-2010
 *  Created by: suttonp
 *  $Revision: 1489 $
 *  $LastChangedDate: 2012-05-28 10:55:22 +0100 (Mon, 28 May 2012) $
 *  $LastChangedBy: suttonp $
 *
 */

#ifndef USRPUHDTXCOMPONENT_H_
#define USRPUHDTXCOMPONENT_H_

#include "irisapi/PNComponent.h"
#include <uhd/usrp/multi_usrp.hpp>

namespace iris
{

class UsrpUhdTxComponent: public PNComponent
{
private:
	//Input and output DataBuffers
	ReadBuffer< std::complex<float> >* d_inBuf;

	//Exposed parameters
	std::string x_args;   //See http://www.ettus.com/uhd_docs/manual/html/identification.html
	double x_rate;        //Rate of outgoing samples
	double x_frequency;   //Tx frequency
	double x_fixLoOffset; //Fix the local oscillator offset (defaults to 2*rate)
	float x_gain;         //Overall tx gain
	std::string x_antenna;//Daughterboard antenna selection
	std::string x_subDev; //Daughterboard subdevice specification
	double x_bw;          //Daughterboard IF filter bandwidth (Hz)
	std::string x_ref;    //Reference waveform (internal, external, mimo)
	bool x_streaming;     //Streaming or bursty traffic?
	std::string x_fmt;    //Data format (fc64, fc32 or sc16)

	//The device
  uhd::usrp::multi_usrp::sptr usrp;
  uhd::tx_streamer::sptr tx_stream;

public:
    UsrpUhdTxComponent(std::string name);
    virtual ~UsrpUhdTxComponent();
    virtual std::map<std::string, int> calculateOutputTypes(std::map<std::string, int> inputTypes);
    virtual void registerPorts();
    virtual void initialize();
    virtual void process();
    virtual void parameterHasChanged(std::string name);
};

} /* namespace iris */

#endif /* USRPUHDTXCOMPONENT_H_ */
