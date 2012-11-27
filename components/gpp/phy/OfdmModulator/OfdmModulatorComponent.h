/**
 * \file components/gpp/phy/OfdmModulator/OfdmModulatorComponent.h
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
 * An OFDM modulation component. Takes a block of data bytes in uint8_t
 * format and outputs a full OFDM frame of complex<float> symbols with
 * the following structure:                                                <br>
 *             -----------------------------------                         <br>
 *             | Preamble | Header | Data ...... |                         <br>
 *             -----------------------------------                         <br>
 *
 * The default parameter values give the following spectral shape:
 *
 *  ________________||||||||||||||||____||||||||||||||||________________   <br>
 *  <  156 guards  >< 100 carriers ><DC>< 100 carriers ><  155 guards  >   <br>
 *
 * This default shape is designed with a large number of guard subcarriers
 * to account for the roll-off of the half-band filters in the USRP front-ends.
 * When transmitted with bandwidth X, this default waveform can be demodulated
 * by receiving bandwidth X/2 and using the OfdmDemodulator component with its
 * default parameter values.
 */

#ifndef PHY_OFDMMODULATORCOMPONENT_H_
#define PHY_OFDMMODULATORCOMPONENT_H_

#include <fftw3.h>

#include "irisapi/PhyComponent.h"

namespace iris
{
namespace phy
{

/** An OFDM modulation component. Takes a block of data bytes in uint8_t
 * format and outputs a full OFDM frame of complex<float> symbols with
 * the following structure:                                                <br>
 *             -----------------------------------                         <br>
 *             | Preamble | Header | Data ...... |                         <br>
 *             -----------------------------------                         <br>
 */
class OfdmModulatorComponent
  : public PhyComponent
{
 public:
  OfdmModulatorComponent(std::string name);
  virtual void calculateOutputTypes(
        const std::map<std::string, int>& inputTypes,
        std::map<std::string, int>& outputTypes);
  virtual void registerPorts();
  virtual void initialize();
  virtual void process();

 private:

  typedef std::vector< std::complex<float> > ComplexVector;

  void generatePreamble();
  void generatePilotIndices();
  void generateDataIndices();

  void createFrame();
  void createHeader();
  void createSymbol();

  int numDataCarriers_x;     ///< Data subcarriers (default = 192)
  int numPilotCarriers_x;    ///< Pilot subcarriers (default = 8)
  int numGuardCarriers_x;    ///< Guard subcarriers (default = 55+256)
  int modulationDepth_x;     ///< 1=BPSK, 2=QPSK, 4=QAM16 (default = 1)
  int cyclicPrefixLength_x;  ///< Length of cyclic prefix (default = 32)
  int maxSymbolsPerFrame_x;  ///< Max OFDM symbols per frame (default = 32)

  ComplexVector fftBins_;
  ComplexVector preamble_;
  fftwf_plan fftPlan_;

};

} // namespace phy
} // namespace iris

#endif // PHY_OFDMMODULATORCOMPONENT_H_
