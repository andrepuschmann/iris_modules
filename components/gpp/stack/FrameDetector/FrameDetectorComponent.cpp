/**
 * \file components/gpp/stack/FrameDetector/FrameDetectorComponent.cpp
 * \version 1.0
 *
 * \section COPYRIGHT
 *
 * Copyright 2012-2013 The Iris Project Developers. See the
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
 * Implementation of a simple detector frame component.
 * This allows to trigger an event if a dataset pushed through
 * the component matches a certain pattern, e.g. contains
 * a UDP header.
 *
 */

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "FrameDetectorComponent.h"

#include "utility/StackHelper.h"

using namespace std;

namespace iris
{
namespace stack
{

/// Export library symbols
IRIS_COMPONENT_EXPORTS(StackComponent, FrameDetectorComponent);

FrameDetectorComponent::FrameDetectorComponent(std::string name)
  : StackComponent(name,
                   "framedetector",
                   "An stack to manage multiple flows",
                   "Andre Puschmann",
                   "0.1")
{
    registerParameter("matchUdp", "Whether to detect UDP frames", "false", false, matchUdp_x);
    registerParameter("matchIcmp", "Whether to detect ICMP frames", "false", false, matchIcmp_x);
    registerEvent("EvFrameDetected", "An event to signal that a matching frame has been received", TypeInfo< uint8_t >::identifier);
}

void FrameDetectorComponent::initialize()
{}

void FrameDetectorComponent::processMessageFromAbove(boost::shared_ptr<StackDataSet> set)
{
    // just pass data through
    sendDownwards(set);
}


void FrameDetectorComponent::processMessageFromBelow(boost::shared_ptr<StackDataSet> set)
{
    runFrameDetector(set);
    sendUpwards(set);
}


void FrameDetectorComponent::runFrameDetector(boost::shared_ptr<StackDataSet> set)
{
    //StackHelper::printDataset(set, "before FrameMatcher");
    const int maxBufSize = ETHER_HDR_LEN + (int)sizeof(struct iphdr);
    unsigned char packetBuffer[maxBufSize];
    const struct ether_header* ethernetHeader = (struct ether_header*)packetBuffer;
    const struct ip* ipHeader;


    // serialize deque first
    int noBytes = std::min((int)set->data.size(), (int)maxBufSize);
    for (int i = 0; i < noBytes; i++)
        packetBuffer[i] = set->data[i];

    // check of ethernet header (tap device)
    if (ntohs(ethernetHeader->ether_type) == ETHERTYPE_IP) {
        ipHeader = (struct ip*)(packetBuffer + sizeof(struct ether_header));
    } else {
        ipHeader = (struct ip*)packetBuffer; // if not, skip ethernet header
    }

    bool triggerEvent = false;
    if (matchUdp_x) {
        if (ipHeader->ip_p == IPPROTO_UDP ||
            ipHeader->ip_p == IPPROTO_UDPLITE) {
            triggerEvent = true;
        }
    }
    if (matchIcmp_x) {
        if (ipHeader->ip_p == IPPROTO_ICMP) {
            triggerEvent = true;
        }
    }

    if (triggerEvent) {
        uint8_t dummyParam;
        LOG(LDEBUG) << "Trigger EvFrameDetected!";
        activateEvent("EvFrameDetected", dummyParam);
    }
}


} // namespace stack
} // namespace iris
