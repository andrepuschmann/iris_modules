/*
 * This file is part of Iris 2.
 * 
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the 
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin. 
 * All rights reserved.
 * 
 */

/**
 * \file UsrpUhdRxComponent.h
 * A Usrp receiver using the Universal Hardware Driver (UHD).
 * This component streams data from the Usrp and sets the timestamp and
 * sampleRate on the generated DataSet if supported.
 *
 *  Created on: 25-May-2010
 *  Created by: suttonp
 *  $Revision: 1489 $
 *  $LastChangedDate: 2012-05-28 10:55:22 +0100 (Mon, 28 May 2012) $
 *  $LastChangedBy: suttonp $
 *
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
