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
	WriteBuffer< std::complex<float> >* outBuf_;

	//The device
	uhd::usrp::multi_usrp::sptr usrp_;
	uhd::rx_streamer::sptr rxStream_;

	bool isStreaming_;
	bool isUsrp1_;
	uhd::time_spec_t currentTimestamp_;
	bool gotFirstPacket_;

	//Exposed parameters
	std::string args_x;   //See http://www.ettus.com/uhd_docs/manual/html/identification.html
	double frequency_x;   //Receive frequency
	double rate_x;      //Receive rate
	double fixLoOffset_x;   //Local oscillator offset
	float gain_x;       //Receive gain
	int outputBlockSize_x;  //Output block size
	std::string antenna_x;  //Daughterboard antenna selection
	std::string subDev_x;   //Daughterboard subdevice specification
	double bw_x;      //Daughterboard IF filter bandwidth in Hz
	std::string ref_x;    //Reference clock(internal, external, mimo)
	std::string wireFmt_x;  //Wire format (sc8 or sc16)
	
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
