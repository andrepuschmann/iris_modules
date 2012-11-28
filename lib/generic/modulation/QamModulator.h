/**
 * \file lib/generic/modulation/QamModulator.h
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
 * A Quadrature Amplitude Modulation class. Objects of this class
 * can be used to modulate using M-ary QAM with a constellation on
 * a rectangular lattice. Constellations are Gray coded with average
 * unit energy.
 */

#ifndef MOD_QAMMODULATOR_H_
#define MOD_QAMMODULATOR_H_

#include <complex>
#include <vector>
#include <cmath>

#include "irisapi/Exceptions.h"
#include "irisapi/TypeInfo.h"
#include "irisapi/Logging.h"

namespace iris
{

/** A Quadrature Amplitude Modulation class.
 *
 * Objects of this class provide M-ary QAM modulation. Constellations
 * are Gray coded with average unit energy.
 */
class QamModulator
{
 public:
  typedef std::complex<float>   Cplx;
  typedef std::vector<Cplx>     CplxVec;

  /** Modulate a sequence of uint8_t bytes to QAM complex<float>
   * symbols. Defaults to BPSK.
   *
   * @param inBegin   Iterator to first input byte.
   * @param inEnd     Iterator to one past last input byte.
   * @param outBegin  Iterator to first output QAM symbol.
   * @param outEnd    Iterator to one past last output QAM symbol.
   * @param M         Modulation depth (1=BPSK, 2=QPSK, 4=QAM16)
   */
  template <class InputInterator, class OutputIterator>
  static void modulate(InputInterator inBegin,
                       InputInterator inEnd,
                       OutputIterator outBegin,
                       OutputIterator outEnd,
                       unsigned int M)
  {
    // Check for sufficient output size
    if(outEnd-outBegin < (inEnd-inBegin)*8/M)
      throw IrisException("Insufficient storage provided for modulate output.");

    if(outEnd-outBegin > (inEnd-inBegin)*8/M)
      LOG(LWARNING) << "Output size larger than required for modulate.";

    switch (M)
    {
      case 2: //QPSK
        //Convert bytes into bit-sequences and use LUT
        for(; inBegin != inEnd; inBegin++)
        {
          for(int j = 3; j>=0; j--)
            *outBegin++ = QpskLut_[(int)((*inBegin >> (j*2)) & 0x3)];
        }
        break;
      case 4: //16 QAM
        //Convert bytes into bit-sequences and use LUT
        for(; inBegin != inEnd; inBegin++)
        {
          for (int j = 1; j>=0; j--)
            *outBegin++ = Qam16Lut_[(int)((*inBegin >> (j*4)) & 0xF)];
        }
        break;
      default : //BPSK
        //Convert bytes into bit-sequences and use LUT
        for(; inBegin != inEnd; inBegin++){
          for (int j = 7; j>=0; j--)
            *outBegin++ = BpskLut_[(int)((*inBegin >> j) & 0x1)];
        }
        break;
    }
  }

  /// Convenience function for logging.
  static std::string getName(){ return "QamModulator"; }

  static CplxVec createBpskLut()
  {
    using namespace std;
    CplxVec vec;
    vec.push_back(Cplx(1,0));
    vec.push_back(Cplx(-1,0));
    return vec;
  }

  static CplxVec createQpskLut()
  {
    using namespace std;
    CplxVec vec;
    vec.push_back(Cplx(-1.0f/sqrtf(2.0f),-1.0f/sqrtf(2.0f)));
    vec.push_back(Cplx(-1.0f/sqrtf(2.0f), 1.0f/sqrtf(2.0f)));
    vec.push_back(Cplx( 1.0f/sqrtf(2.0f),-1.0f/sqrtf(2.0f)));
    vec.push_back(Cplx( 1.0f/sqrtf(2.0f), 1.0f/sqrtf(2.0f)));
    return vec;
  }

  static CplxVec createQam16Lut()
  {
    using namespace std;
    CplxVec vec;
    vec.push_back(Cplx(-1.0f/sqrtf(10.0f),-1.0f/sqrtf(10.0f)));
    vec.push_back(Cplx(-1.0f/sqrtf(10.0f),-3.0f/sqrtf(10.0f)));
    vec.push_back(Cplx(-3.0f/sqrtf(10.0f),-1.0f/sqrtf(10.0f)));
    vec.push_back(Cplx(-3.0f/sqrtf(10.0f),-3.0f/sqrtf(10.0f)));

    vec.push_back(Cplx(-1.0f/sqrtf(10.0f), 1.0f/sqrtf(10.0f)));
    vec.push_back(Cplx(-1.0f/sqrtf(10.0f), 3.0f/sqrtf(10.0f)));
    vec.push_back(Cplx(-3.0f/sqrtf(10.0f), 1.0f/sqrtf(10.0f)));
    vec.push_back(Cplx(-3.0f/sqrtf(10.0f), 3.0f/sqrtf(10.0f)));

    vec.push_back(Cplx( 1.0f/sqrtf(10.0f),-1.0f/sqrtf(10.0f)));
    vec.push_back(Cplx( 1.0f/sqrtf(10.0f),-3.0f/sqrtf(10.0f)));
    vec.push_back(Cplx( 3.0f/sqrtf(10.0f),-1.0f/sqrtf(10.0f)));
    vec.push_back(Cplx( 3.0f/sqrtf(10.0f),-3.0f/sqrtf(10.0f)));

    vec.push_back(Cplx( 1.0f/sqrtf(10.0f), 1.0f/sqrtf(10.0f)));
    vec.push_back(Cplx( 1.0f/sqrtf(10.0f), 3.0f/sqrtf(10.0f)));
    vec.push_back(Cplx( 3.0f/sqrtf(10.0f), 1.0f/sqrtf(10.0f)));
    vec.push_back(Cplx( 3.0f/sqrtf(10.0f), 3.0f/sqrtf(10.0f)));
    return vec;
  }

 private:
  QamModulator(); // Disabled constructor

  static const CplxVec BpskLut_;
  static const CplxVec QpskLut_;
  static const CplxVec Qam16Lut_;
};

const QamModulator::CplxVec QamModulator::BpskLut_ = QamModulator::createBpskLut();
const QamModulator::CplxVec QamModulator::QpskLut_ = QamModulator::createQpskLut();
const QamModulator::CplxVec QamModulator::Qam16Lut_ = QamModulator::createQam16Lut();

} // namespace iris

#endif // MOD_QAMMODULATOR_H_
