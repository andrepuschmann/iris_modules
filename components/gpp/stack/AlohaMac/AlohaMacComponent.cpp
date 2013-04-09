/**
 * \file components/gpp/stack/AlohaMac/AlohaMacComponent.cpp
 * \version 1.0
 *
 * \section COPYRIGHT
 *
 * Copyright 2013 Andre Puschmann <andre.puschmann@tu-ilmenau.de>
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
 * Implementation of a simple Aloha MAC component.
 *
 */

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "AlohaMacComponent.h"
#include "utility/StackHelper.h"

using namespace std;

namespace iris
{
// export library symbols
IRIS_COMPONENT_EXPORTS(StackComponent, AlohaMacComponent);

AlohaMacComponent::AlohaMacComponent(std::string name)
  : StackComponent(name,
                   "alohamacstackcomponent",
                   "A simple Aloha MAC component",
                   "Andre Puschmann",
                   "0.1")
  ,txSeqNo_(1)
  ,rxSeqNo_(0)
  ,rxPktBuffer_(100)
  ,txPktBuffer_(100)
{
  //Format: registerParameter(name, description, default, dynamic?, parameter, allowed values);
  registerParameter("localaddress", "Address of this client", "f009e090e90e", false, localAddress_x);
  registerParameter("destinationaddress", "Address of the destination client", "00f0f0f0f0f0", false, destinationAddress_x);
  registerParameter("acktimeout", "Time to wait for ACK packets in ms", "100", false, ackTimeout_x);
  registerParameter("maxretry", "Number of retransmissions", "100", false, maxRetry_x);

}


AlohaMacComponent::~AlohaMacComponent()
{
}


void AlohaMacComponent::initialize()
{
  maxRetry_x++; // first attempt does not count as retransmission
}


void AlohaMacComponent::processMessageFromAbove(boost::shared_ptr<StackDataSet> incomingFrame)
{
  //StackHelper::printDataset(incomingFrame, "vanilla from above");
  txPktBuffer_.pushDataSet(incomingFrame);
}


void AlohaMacComponent::processMessageFromBelow(boost::shared_ptr<StackDataSet> incomingFrame)
{
  //StackHelper::printDataset(incomingFrame, "vanilla from below");
  rxPktBuffer_.pushDataSet(incomingFrame);
}


void AlohaMacComponent::start()
{
  rxThread_.reset(new boost::thread(boost::bind( &AlohaMacComponent::rxThreadFunction, this)));
  txThread_.reset(new boost::thread(boost::bind( &AlohaMacComponent::txThreadFunction, this)));
}

void AlohaMacComponent::stop()
{
  // stop threads
  rxThread_->interrupt();
  rxThread_->join();
  txThread_->interrupt();
  txThread_->join();
}


void AlohaMacComponent::registerPorts()
{
  std::vector<int> types;
  types.push_back( int(TypeInfo< uint8_t >::identifier) );

  //The ports on top of the component
  registerOutputPort("topoutputport",types);
  registerInputPort("topinputport", types);

  //The ports below the component
  registerInputPort("bottominputport", types);
  registerOutputPort("bottomoutputport", types);
}


void AlohaMacComponent::rxThreadFunction()
{
  boost::this_thread::sleep(boost::posix_time::seconds(1));
  LOG(LINFO) << "Rx thread started.";

  try
  {
    while(true)
    {
      boost::this_thread::interruption_point();

      shared_ptr<StackDataSet> frame = rxPktBuffer_.popDataSet();

      AlohaPacket newPacket;
      StackHelper::deserializeAndStripDataset(frame, newPacket);

      if (localAddress_x == newPacket.destination()) {
        switch(newPacket.type()) {
        case AlohaPacket::DATA:
        {
          LOG(LINFO) << "Got DATA " << newPacket.seqno() << " from " << newPacket.source();
          sendAckPacket(newPacket.source(), newPacket.seqno());

          // check if packet contains new data
          if (newPacket.seqno() > rxSeqNo_ || newPacket.seqno() == 1) {
            // send new data packet up
            sendDownwards("topoutputport", frame);
            rxSeqNo_ = newPacket.seqno(); // update seqno
            if (newPacket.seqno() == 1) LOG(LINFO) << "Receiver restart detected.";
          }
          break;
        }
        case AlohaPacket::ACK:
        {
          LOG(LINFO) << "Got ACK  " << newPacket.seqno();
          boost::unique_lock<boost::mutex> lock(seqNoMutex_);
          if (newPacket.seqno() == txSeqNo_) {
            // received right ACK
            lock.unlock();
            ackArrivedCond_.notify_one();
          } else if (newPacket.seqno() > txSeqNo_) {
            LOG(LERROR) << "Received future ACK.";
          } else {
            LOG(LERROR) << "Received too old ACK";
          }
          break;
        }
        default:
          LOG(LERROR) << "Undefined packet type.";
          break;
        }
      }
    } // while
  }
  catch(IrisException& ex)
  {
    LOG(LFATAL) << "Error in AlohaMac component: " << ex.what() << " - Rx thread exiting.";
  }
  catch(boost::thread_interrupted)
  {
    LOG(LINFO) << "Thread " << boost::this_thread::get_id() << " in stack component interrupted.";
  }
}


void AlohaMacComponent::txThreadFunction()
{
  boost::this_thread::sleep(boost::posix_time::seconds(1));
  LOG(LINFO) << "Tx thread started.";

  try
  {
    while(true)
    {
      boost::this_thread::interruption_point();

      shared_ptr<StackDataSet> frame = txPktBuffer_.popDataSet();

      boost::unique_lock<boost::mutex> lock(seqNoMutex_);
      AlohaPacket dataPacket;
      dataPacket.set_source(localAddress_x);
      dataPacket.set_destination(destinationAddress_x);
      dataPacket.set_type(AlohaPacket::DATA);
      dataPacket.set_seqno(txSeqNo_);
      StackHelper::mergeAndSerializeDataset(frame, dataPacket);

      bool stop_signal = false;
      int txCounter = 1;
      while (not stop_signal) {
        // send packet to PHY
        LOG(LINFO) << "Tx DATA  " << txSeqNo_;
        sendDownwards(frame);

        // wait for ACK
        if (ackArrivedCond_.timed_wait(lock, boost::posix_time::milliseconds(ackTimeout_x)) == false) {
          // returns false if timeout was reached
          LOG(LINFO) << "ACK time out for " << txCounter << ". transmission of " << txSeqNo_;
          // wait random time before trying again, here between ackTimeout and 2*ackTimeout
          int collisionTimeout = rand() % ackTimeout_x;
          collisionTimeout = std::min(ackTimeout_x + collisionTimeout, 2 * ackTimeout_x);
          boost::this_thread::sleep(boost::posix_time::milliseconds(collisionTimeout));
        } else {
          // ACK received before timeout
          stop_signal = true;
        }

        if (++txCounter > maxRetry_x) stop_signal = true;
      }

      // increment seqno for next data packet and release lock
      txSeqNo_++;
      if (txSeqNo_ == std::numeric_limits<uint32_t>::max()) txSeqNo_ = 1;
      lock.unlock();
    }
  }
  catch(IrisException& ex)
  {
    LOG(LFATAL) << "Error in AlohaMac component: " << ex.what() << " - Tx thread exiting.";
  }
  catch(boost::thread_interrupted)
  {
    LOG(LINFO) << "Thread " << boost::this_thread::get_id() << " in stack component interrupted.";
  }
}


void AlohaMacComponent::sendAckPacket(const string destination, uint32_t seqno)
{
  AlohaPacket ackPacket;
  ackPacket.set_source(localAddress_x);
  ackPacket.set_destination(destination);
  ackPacket.set_type(AlohaPacket::ACK);
  ackPacket.set_seqno(seqno);

  shared_ptr<StackDataSet> buffer(new StackDataSet);
  StackHelper::mergeAndSerializeDataset(buffer, ackPacket);
  //StackHelper::printDataset(buffer, "ACK Tx");

  sendDownwards("bottomoutputport", buffer);
  LOG(LINFO) << "Tx  ACK  " << seqno;
}

} /* namespace iris */
