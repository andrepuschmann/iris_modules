/**
 * \file UsrpUhdTxComponent.cpp
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
 * Implementation of a sink component which writes to a USRP transmitter
 * using the Universal Hardware Driver (UHD).
 * This component supports streaming data by default. For bursty data,
 * set "streaming" to false. For bursty data, packets must be contained
 * in a single DataSet. If a timestamp is specified in a DataSet, the
 * packet will be transmitted at that time, if supported by the USRP.
 */

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "UsrpUhdTxComponent.h"
#include <uhd/utils/thread_priority.hpp>
#include <boost/thread.hpp>

using namespace std;
using namespace uhd;


namespace iris
{
	// export library symbols
	IRIS_COMPONENT_EXPORTS(PNComponent, UsrpUhdTxComponent);

	/*! Constructor
	*
	*	Call the constructor on PNComponent and pass in all details about the component.
	*	Register all parameters and events in the constructor.
  *
  * \param  name	The name assigned to this component when loaded
  */
	UsrpUhdTxComponent::UsrpUhdTxComponent(std::string name):
        PNComponent(name, "usrpuhdtxpncomponent", "A Usrp transmitter using the Universal Hardware Driver", "Paul Sutton", "0.1")
  {
	  //Format: registerParameter(name, description, default, dynamic?, parameter, allowed values);
    registerParameter("args", "A delimited string which may be used to specify a particular usrp", "", false, x_args);
    registerParameter("rate", "The transmit rate", "1000000", true, x_rate);
    registerParameter("frequency", "The transmit frequency", "2400000000", true, x_frequency);
    registerParameter("gain", "The transmit gain", "1", true, x_gain);
    registerParameter("streaming", "Whether we're streaming data to tx", "true", true, x_streaming);
    registerParameter("fixlooffset","Value to fix LO offset to in Hz - defaults to 2*rate", "-1", false, x_fixLoOffset);
    registerParameter("antenna","Daughterboard antenna selection", "", false, x_antenna);
    registerParameter("subdev","Daughterboard subdevice specification", "", false, x_subDev);
    registerParameter("bw","Daughterboard IF filter bandwidth (Hz)", "0", false, x_bw);
    registerParameter("ref","Reference waveform (internal, external, mimo)", "internal", false, x_ref);
    registerParameter("fmt", "Data format (fc64, fc32 or sc16)", "fc32", false, x_fmt);
  }

	/*! Destructor
	*
	*	Send an EOB packet to stop the Usrp
	*/
	UsrpUhdTxComponent::~UsrpUhdTxComponent()
	{
		//Send a mini EOB packet
		uhd::tx_metadata_t md;
		md.start_of_burst = false;
		md.end_of_burst   = true;
		vector< complex<float> > v;
		if(tx_stream != NULL)
		{
			tx_stream->send(&v.front(), 0, md);
		}
	}

	/*! Register the ports of this component
	*
	*	Ports are registered by name with a vector of valid data types permitted on those ports.
	*	This example has one input port with a single valid data type - complex<float>.
  */
	void UsrpUhdTxComponent::registerPorts()
  {
    //Register all ports
    vector<int> validTypes;
    validTypes.push_back(TypeInfo< complex<float> >::identifier);

    //format:        (name, vector of valid types)
    registerInputPort("input1", validTypes);
  }

	/*! Calculate output data types
	*
	*	Based on the input data types, tell the system what output data types will be provided.
	*	\param	inputTypes	The data types of the inputs which will be passed to this component
  */
	map<string, int> UsrpUhdTxComponent::calculateOutputTypes(std::map<std::string, int> inputTypes)
  {
    //No output types
    map<string, int> outputTypes;
    return outputTypes;
  }

	//! Do any initialization required
  void UsrpUhdTxComponent::initialize()
  {
    uhd::set_thread_priority_safe();

    //Set up the input DataBuffer
    d_inBuf = castToType< complex<float> >(inputBuffers.at(0));

    //Set up the usrp
    try
    {
      //Create the device
      LOG(LINFO) << "Creating the usrp device with args: " << x_args;
      usrp = uhd::usrp::multi_usrp::make(x_args);
      //Lock mboard clocks
      usrp->set_clock_source(x_ref);
      //always select the subdevice first, the channel mapping affects the other settings
      if (x_subDev!="") usrp->set_tx_subdev_spec(x_subDev);
      LOG(LINFO) << "Using Device: " << usrp->get_pp_string();

      //Set rate
      LOG(LINFO) << "Setting TX Rate: " << (x_rate/1e6) << "Msps...";
      usrp->set_tx_rate(x_rate);
      LOG(LINFO) << "Actual TX Rate: " << (usrp->get_tx_rate()/1e6) << "Msps...";

      //Set frequency
      LOG(LINFO) << "Setting TX Frequency: " << (x_frequency/1e6) << "MHz...";
      double lo_offset = 2*x_rate;	//Set LO offset to twice signal rate by default
      if(x_fixLoOffset >= 0)
      {
        lo_offset = x_fixLoOffset;
      }
      usrp->set_tx_freq(tune_request_t(x_frequency, lo_offset));
      LOG(LINFO) << "Actual TX Frequency: " << (usrp->get_tx_freq()/1e6) << "MHz";

      //We can only set the time on usrp2 devices
      if(usrp->get_mboard_name().find("usrp1") == string::npos)
      {
        LOG(LINFO) << "Setting device timestamp to 0...";
        usrp->set_time_now(uhd::time_spec_t((double)0));
      }

      //set the rf gain
      gain_range_t range = usrp->get_tx_gain_range();
      LOG(LINFO) << "Gain range: " << range.to_pp_string();
      LOG(LINFO) << "Setting TX Gain: " << x_gain << " dB...";
      usrp->set_tx_gain(x_gain);
      LOG(LINFO) << "Actual TX Gain: " <<  usrp->get_tx_gain() << " dB...";

      //set the IF filter bandwidth
      if(x_bw!=0){
        LOG(LINFO) << "Setting TX Bandwidth: " << x_bw << " MHz...";
        usrp->set_tx_bandwidth(x_bw);
        LOG(LINFO) << "Actual TX Bandwidth: " << usrp->get_tx_bandwidth() << " MHz...";
      }

      //Set the antenna
      if(x_antenna!="") usrp->set_tx_antenna(x_antenna);
      LOG(LINFO) << "Using TX Antenna: " << usrp->get_tx_antenna();

      boost::this_thread::sleep(boost::posix_time::seconds(1)); //allow for some setup time

      //Check Ref and LO Lock detect
      std::vector<std::string> sensor_names;
      sensor_names = usrp->get_tx_sensor_names(0);
      if (std::find(sensor_names.begin(), sensor_names.end(), "lo_locked") != sensor_names.end()) {
        uhd::sensor_value_t lo_locked = usrp->get_tx_sensor("lo_locked",0);
        LOG(LINFO) << "Checking TX: " << lo_locked.to_pp_string() << " ...";
        if(!lo_locked.to_bool())
          throw IrisException("Failed to lock LO");
      }
      sensor_names = usrp->get_mboard_sensor_names(0);
      if ((x_ref == "mimo") and (std::find(sensor_names.begin(), sensor_names.end(), "mimo_locked") != sensor_names.end())) {
        uhd::sensor_value_t mimo_locked = usrp->get_mboard_sensor("mimo_locked",0);
        LOG(LINFO) << "Checking TX: " << mimo_locked.to_pp_string() << " ...";
        if(!mimo_locked.to_bool())
          throw IrisException("Failed to lock LO");
      }
      if ((x_ref == "external") and (std::find(sensor_names.begin(), sensor_names.end(), "ref_locked") != sensor_names.end())) {
        uhd::sensor_value_t ref_locked = usrp->get_mboard_sensor("ref_locked",0);
        LOG(LINFO) << "Checking TX: " << ref_locked.to_pp_string() << " ...";
        if(!ref_locked.to_bool())
          throw IrisException("Failed to lock LO");
      }

      //create a transmit streamer
      uhd::stream_args_t stream_args(x_fmt);
      tx_stream = usrp->get_tx_stream(stream_args);
    }
    catch(std::exception& e)
    {
      throw IrisException(e.what());
    }
  }

	/*! The main work of the component is carried out here
	*
	*	Take a DataSet from the input buffer and send to the usrp
  */
  void UsrpUhdTxComponent::process()
  {
    //Get a DataSet from the input DataBuffer
    DataSet< complex<float> >* readDataSet = NULL;
    d_inBuf->getReadData(readDataSet);

    size_t size = readDataSet->data.size();

    //Set up metadata
    uhd::tx_metadata_t md;
    md.start_of_burst = true;
    if(x_streaming)
    {
      md.end_of_burst = false;
    }else{
      md.end_of_burst = true;
    }
    if(readDataSet->timeStamp > 0)
    {
      md.has_time_spec = true;
      md.time_spec = uhd::time_spec_t(readDataSet->timeStamp);
    }

    //Send the data
    size_t num_tx_samps = tx_stream->send(
      &readDataSet->data.front(), size, md
    );

    //Release the DataSet
    d_inBuf->releaseReadData(readDataSet);

  }

  //! This gets called whenever a parameter is reconfigured
  void UsrpUhdTxComponent::parameterHasChanged(std::string name)
  {
    try
    {
      if(name == "frequency")
      {
        LOG(LINFO) << "Setting TX Frequency: " << (x_frequency/1e6) << "MHz...";
        double lo_offset = 2*x_rate;  //Set LO offset to twice signal rate by default
        if(x_fixLoOffset >= 0)
        {
          lo_offset = x_fixLoOffset;
        }
        usrp->set_tx_freq(tune_request_t(x_frequency, lo_offset));
        LOG(LINFO) << "Actual TX Frequency: " << (usrp->get_tx_freq()/1e6) << "MHz";
      }
      else if(name == "rate")
      {
        LOG(LINFO) << "Setting TX Rate: " << (x_rate/1e6) << "Msps...";
        usrp->set_tx_rate(x_rate);
        LOG(LINFO) << "Actual TX Rate: " << (usrp->get_tx_rate()/1e6) << "Msps...";
      }
      else if(name == "gain")
      {
        gain_range_t range = usrp->get_tx_gain_range();
        LOG(LINFO) << "Gain range: " << range.to_pp_string();
        LOG(LINFO) << "Setting TX Gain: " << x_gain << " dB...";
        usrp->set_tx_gain(x_gain);
        LOG(LINFO) << "Actual TX Gain: " <<  usrp->get_tx_gain() << " dB...";
      }
    }
    catch(std::exception e)
    {
      throw IrisException(e.what());
    }
  }

} /* namespace iris */
