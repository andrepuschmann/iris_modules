/**
 * \file components/gpp/phy/OfdmDemodulator/OfdmDemodulatorComponent.h
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
 * An OFDM demodulation component. Takes blocks of received complex<float>
 * data and detects and demodulates OFDM frames. Received data is output
 * in blocks of uint8_t bytes. There is one output block for each received
 * frame. The demodulator expects frames with the following structure:     <br>                                           <br>
 *             -----------------------------------                         <br>
 *             | Preamble | Header | Data ...... |                         <br>
 *             -----------------------------------                         <br>
 *
 * The default parameter values expect the following spectral shape:
 *
 *  _______________||||||||||||||||____||||||||||||||||________________    <br>
 *  <  23 guards  >< 100 carriers ><DC>< 100 carriers ><  22 guards  >     <br>
 *
 * This corresponds to the default shape generated with the OfdmModulator
 * component when receiving with bandwidth X/2 where X is the transmit
 * bandwidth. See OfdmModulatorComponent.h for more information.
 */

#ifndef PHY_OFDMDEMODULATORCOMPONENT_H_
#define PHY_OFDMDEMODULATORCOMPONENT_H_

#include "irisapi/PhyComponent.h"
#include "modulation/OfdmPreambleDetector.h"

namespace iris
{
namespace phy
{

/**
 */
class OfdmDemodulatorComponent
  : public PhyComponent
{
public:
  typedef std::complex<float>   Cplx;
  typedef std::vector<Cplx>     CplxVec;
  typedef CplxVec::iterator     CplxVecIt;
  typedef std::vector<int>      IntVec;
  typedef IntVec::iterator      IntVecIt;
  typedef std::vector<uint8_t>  ByteVec;
  typedef ByteVec::iterator     ByteVecIt;

  OfdmDemodulatorComponent(std::string name);
  virtual void calculateOutputTypes(
      const std::map<std::string, int>& inputTypes,
      std::map<std::string, int>& outputTypes);
  virtual void registerPorts();
  virtual void initialize();
  virtual void process();

private:
  void setup();

  int numDataCarriers_x;      ///< Data subcarriers (default = 192)
  int numPilotCarriers_x;     ///< Pilot subcarriers (default = 8)
  int numGuardCarriers_x;     ///< Guard subcarriers (default = 55)
  int cyclicPrefixLength_x;   ///< Length of cyclic prefix (default = 32)
  float threshold_x;          ///< Frame detection threshold (default = 0.827)

  int numBins_;               ///< Number of bins for our FFT.

  IntVec pilotIndices_;       ///< Indices for our pilot carriers.
  IntVec dataIndices_;        ///< Indices for our data carriers.
  CplxVec preamble_;          ///< Contains our frame preamble.
  CplxVec pilotSequence_;     ///< Contains our pilot symbols.

  OfdmPreambleDetector detector_;

  template <typename T, size_t N>
  static T* begin(T(&arr)[N]) { return &arr[0]; }
  template <typename T, size_t N>
  static T* end(T(&arr)[N]) { return &arr[0]+N; }
};

} // namespace phy
} // namespace iris

#endif // PHY_OFDMDEMODULATORCOMPONENT_H_
