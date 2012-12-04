/**
 * \file components/gpp/phy/OfdmModulator/OfdmModulatorComponent.cpp
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
 * Implementation of the OfdmModulator component.
 */

#include "OfdmModulatorComponent.h"

#include <cmath>
#include <algorithm>
#include <boost/lambda/lambda.hpp>

#include "kissfft/kissfft.hh"
#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "modulation/OfdmIndexGenerator.h"
#include "modulation/OfdmPreambleGenerator.h"
#include "modulation/Crc.h"
#include "modulation/Whitener.h"
#include "modulation/QamModulator.h"

using namespace std;
using namespace boost::lambda;

namespace iris
{
namespace phy
{

// export library symbols
IRIS_COMPONENT_EXPORTS(PhyComponent, OfdmModulatorComponent);

OfdmModulatorComponent::OfdmModulatorComponent(std::string name)
  : PhyComponent(name,                          // component name
                "ofdmmodulator",                // component type
                "An OFDM modulation component", // description
                "Paul Sutton",                  // author
                "1.0")                          // version
{
/*
  registerParameter(
      "exampleparameter",                   // name
      "An example parameter",               // description
      "0",                                  // default value
      true,                                 // dynamic?
      example_x,                            // parameter
      Interval<uint32_t>(0,5));             // allowed values

  registerEvent(
      "exampleevent",                       // name
      "An example event",                   // description
      TypeInfo< uint32_t >::identifier);    // data type
*/
}

void OfdmModulatorComponent::registerPorts()
{
  registerInputPort("input1", TypeInfo< uint8_t >::identifier);
  registerOutputPort("output1", TypeInfo< complex<float> >::identifier);
}

void OfdmModulatorComponent::calculateOutputTypes(
    const std::map<std::string,int>& inputTypes,
    std::map<std::string,int>& outputTypes)
{
  //One output type - always complex<float>
  outputTypes["output1"] = TypeInfo< complex<float> >::identifier;
}

void OfdmModulatorComponent::initialize()
{
  setup();
}

void OfdmModulatorComponent::process()
{
  DataSet< uint8_t >* in = NULL;
  getInputDataSet("input1", in);
  double time = in->timeStamp;
  int size = (int)in->data.size();
  int numSymbols = ceil(size/(float)bytesPerSymbol_);

  ByteVecIt it = in->data.begin();
  do
  {
    int sizeThisFrame;
    if(numSymbols >= maxSymbolsPerFrame_x)
      sizeThisFrame = maxSymbolsPerFrame_x * bytesPerSymbol_;
    else
      sizeThisFrame = size;

    createHeader(it, it+sizeThisFrame);
    createFrame(it, it+sizeThisFrame);

    numSymbols -= maxSymbolsPerFrame_x;
    size -= (maxSymbolsPerFrame_x*bytesPerSymbol_);
    it += sizeThisFrame;

  }while(numSymbols > 0);

  releaseInputDataSet("input1", in);
}

/// Set up all our index vectors and containers.
void OfdmModulatorComponent::setup()
{
  pilotIndices_.clear();
  pilotIndices_.resize(numPilotCarriers_x);
  dataIndices_.clear();
  dataIndices_.resize(numDataCarriers_x);
  OfdmIndexGenerator::generateIndices(numDataCarriers_x,
                                      numPilotCarriers_x,
                                      numGuardCarriers_x,
                                      pilotIndices_.begin(), pilotIndices_.end(),
                                      dataIndices_.begin(), dataIndices_.end());

  numBins_ = numDataCarriers_x + numPilotCarriers_x + numGuardCarriers_x;
  fftBins_.clear();
  fftBins_.resize(numBins_);
  symbol_.clear();
  symbol_.resize(numBins_);
  preamble_.clear();
  preamble_.resize(numBins_);
  OfdmPreambleGenerator::generatePreamble(numDataCarriers_x,
                                          numPilotCarriers_x,
                                          numGuardCarriers_x,
                                          preamble_.begin(), preamble_.end());

  bytesPerSymbol_ = (numDataCarriers_x * modulationDepth_x)/8;
  header_.resize(bytesPerSymbol_);  //TODO: Check that header can be contained in one symbol
  Whitener::whiten(header_.begin(), header_.end());
}

/** Create a header for the current frame.
 *
 * The header will occupy a single OFDM symbol and will be BPSK modulated.
 * The header structure is as follows:                                     <br>
 *       ---------------------------------------------------------         <br>
 * bits  |    32|                16|            8| bits/symbol-56|         <br>
 * data  |   CRC| Frame size(bytes)| QAM encoding|        padding|         <br>
 *       ---------------------------------------------------------         <br>
 *
 * @param begin Iterator to first byte of tx data.
 * @param end   Iterator to one past last byte of tx data.
 */
void OfdmModulatorComponent::createHeader(ByteVecIt begin, ByteVecIt end)
{
  //Add the CRC
  uint32_t crc = Crc::generate(begin,end);
  header_[0] = (crc>>24) & 0xFF;
  header_[1] = (crc>>16) & 0xFF;
  header_[2] = (crc>>8) & 0xFF;
  header_[3] = crc & 0xFF;

  //Add frame size
  int size = end-begin;
  header_[4] = (size>>8) & 0xFF;
  header_[5] = size & 0xFF;

  //Add the QAM encoding
  header_[6] = modulationDepth_x & 0xFF;
}

/** Create an OFDM frame and write it to the output.
 *
 * The frame structure is as follows:                                     <br>
 *          --------------------------------------------------------      <br>
 * symbols  |        1 |       1|           variable |           1 |      <br>
 * data     | Preamble | Header |       Data Symbols | Frame Guard |      <br>
 *          --------------------------------------------------------      <br>
 *
 * @param begin   Iterator to first input data byte.
 * @param end     Iterator to one past last input data byte.
 */
void OfdmModulatorComponent::createFrame(ByteVecIt begin, ByteVecIt end)
{
  int numQamSymbols = (end-begin)*8/modulationDepth_x;
  int numOfdmSymbols = ceil((end-begin)/(float)bytesPerSymbol_);
  int ofdmSymLength = numBins_+cyclicPrefixLength_x;

  // Whiten
  Whitener::whiten(header_.begin(), header_.end());
  Whitener::whiten(begin, end);

  // Modulate
  modHeader_.resize(bytesPerSymbol_);
  modData_.resize(numQamSymbols);
  QamModulator::modulate(header_.begin(), header_.end(),
                         modHeader_.begin(), modHeader_.end(),1);
  QamModulator::modulate(begin, end,
                         modData_.begin(), modData_.end(), modulationDepth_x);

  // Get a DataSet
  int frameLength = (1+1+numOfdmSymbols+1) * (ofdmSymLength);
  DataSet< complex<float> >* out = NULL;
  getOutputDataSet("output1", out, frameLength);
  CplxVecIt it = out->data.begin();

  // Copy preamble
  it = copyWithCp(preamble_.begin(), preamble_.end(), it, it+ofdmSymLength);

  // Create and copy header symbol
  createSymbol(modHeader_.begin(), modHeader_.end(), symbol_.begin(),symbol_.end());
  it = copyWithCp(symbol_.begin(), symbol_.end(), it, it+ofdmSymLength);

  // Create and copy data symbols
  CplxVecIt inIt = modData_.begin();
  for(int i=0;i<numOfdmSymbols;i++)
  {
    createSymbol(inIt, inIt+numDataCarriers_x, symbol_.begin(), symbol_.end());
    inIt += numDataCarriers_x;
    it = copyWithCp(symbol_.begin(), symbol_.end(), it, it+ofdmSymLength);
  }
}

/** Create a single OFDM symbol.
 *
 * Our pilot and data index vectors are used to map QAM symbols onto carriers
 * and an FFT is used to create the time-domain OFDM symbol.
 *
 * @param inBegin   Iterator to first input QAM symbol.
 * @param inEnd     Iterator to one past last input QAM symbol.
 * @param outBegin  Iterator to first sample of the output OFDM symbol.
 * @param outEnd    Iterator to one past last sample of the output symbol.
 */
void OfdmModulatorComponent::createSymbol(CplxVecIt inBegin, CplxVecIt inEnd,
                                          CplxVecIt outBegin, CplxVecIt outEnd)
{
  if(outEnd-outBegin < numBins_)
    throw IrisException("Insufficient storage provided for createSymbol output.");
  //TODO: Check we have enough input symbols, pad if not.

  int i = 0;
  IntVecIt it = pilotIndices_.begin();
  for(; it!=pilotIndices_.end(); it++, i++)
    fftBins_[*it] = pilotSequence_[i%pilotSequence_.size()];
  for(it=dataIndices_.begin(); it!= dataIndices_.end(); it++)
    fftBins_[*it] = *inBegin++;

  kissfft<float> fft(numBins_,true);  // Precreate this for speed?
  fft.transform(&fftBins_[0], &(*outBegin));
  transform(outBegin, outEnd, outBegin, _1/(float)numBins_);
}

OfdmModulatorComponent::CplxVecIt
OfdmModulatorComponent::copyWithCp(CplxVecIt inBegin, CplxVecIt inEnd,
                                   CplxVecIt outBegin, CplxVecIt outEnd)
{
  if(outEnd-outBegin < (inEnd-inBegin)+cyclicPrefixLength_x)
    throw IrisException("Insufficient storage provided for copyWithCp output.");

  CplxVecIt it = copy(inEnd-cyclicPrefixLength_x, inEnd, outBegin);
  copy(inBegin, inEnd, it);
  return outEnd;
}

} // namesapce phy
} // namespace iris
