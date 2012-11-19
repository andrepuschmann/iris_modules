/**
 * \file UsrpUhdRxComponent.h
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
 * A source component which reads from a USRP receiver using the
 * Universal Hardware Driver (UHD).
 * This component streams data from the USRP and sets the timestamp and
 * sampleRate on the generated DataSet if supported.
 */

#ifndef USRPUHDRXCOMPONENT_H_
#define USRPUHDRXCOMPONENT_H_

#include "irisapi/PNComponent.h"
#include <uhd/usrp/multi_usrp.hpp>

namespace iris
{

class UsrpUhdRxComponent: public PNComponent
{
private:
	//Output DataBuffer
	WriteBuffer< std::complex<float> >* d_outBuf;

	//The device
	uhd::usrp::multi_usrp::sptr usrp;
	uhd::rx_streamer::sptr rx_stream;

	bool isStreaming;
	bool isUsrp1;
	uhd::time_spec_t d_currentTimestamp;
	bool d_gotFirstPacket;

	//Exposed parameters
	std::string x_args;     //See http://www.ettus.com/uhd_docs/manual/html/identification.html
	double x_frequency;     //Receive frequency
	double x_rate;          //Receive rate
	double x_fixLoOffset;   //Local oscillator offset
	float x_gain;           //Receive gain
	int x_outputBlockSize;  //Output block size
	std::string x_antenna;  //Daughterboard antenna selection
	std::string x_subDev;   //Daughterboard subdevice specification
	double x_bw;            //Daughterboard IF filter bandwidth in Hz
	std::string x_ref;      //Reference clock(internal, external, mimo)
	std::string x_wireFmt;  //Wire format (sc8 or sc16)
	
	void setStreaming(bool s);

public:
    UsrpUhdRxComponent(std::string name);
    ~UsrpUhdRxComponent();
    virtual std::map<std::string, int> calculateOutputTypes(std::map<std::string, int> inputTypes);
    virtual void registerPorts();
    virtual void initialize();
    virtual void process();
    virtual void parameterHasChanged(std::string name);
};

} /* namespace iris */

#endif /* USRPUHDRXCOMPONENT_H_ */
