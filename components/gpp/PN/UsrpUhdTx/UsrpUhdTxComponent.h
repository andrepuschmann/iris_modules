/**
 * \file UsrpUhdTxComponent.h
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
 * A sink component which writes to a USRP transmitter using the
 * Universal Hardware Driver (UHD).
 * This component supports streaming data by default. For bursty data,
 * set "streaming" to false. For bursty data, packets must be contained
 * in a single DataSet. If a timestamp is specified in a DataSet, the
 * packet will be transmitted at that time, if supported by the USRP.
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
