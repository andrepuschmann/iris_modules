/**
 * \file lib/generic/modulation/OfdmPreambleGenerator.h
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
 * An OFDM preamble generator. Objects of this class can be used to
 * generate preambles with half-symbol repetitions.
 */

#ifndef MOD_OFDMPREAMBLEGENERATOR_H_
#define MOD_OFDMPREAMBLEGENERATOR_H_

#include <complex>
#include <vector>
#include <algorithm>
#include <boost/lambda/lambda.hpp>

#include "irisapi/Exceptions.h"
#include "irisapi/TypeInfo.h"
#include "irisapi/Logging.h"
#include "kissfft/kissfft.hh"
#include "utility/RawFileUtility.h"

namespace iris
{

/// Generate OFDM preambles with half-symbol repetitions.
class OfdmPreambleGenerator
{
 public:
  typedef std::complex<float>   Cplx;
  typedef std::vector<Cplx>     CplxVec;
  typedef CplxVec::iterator     CplxVecIt;

  /** Generate an OFDM preamble symbol with a half-symbol repetition.
   *
   * A time-domain OFDM preamble symbol is generated using the 802.16
   * preamble sequence. Pseudo-noise data is placed on carriers -100:2:100,
   * resulting in a half-symbol repetition.
   * The DC carrier is null and carrier symbols are qpsk with average
   * energy sqrt(2). The symbol does not include a cyclic prefix.
   *
   * @param numData   Number of data carriers (not including pilots).
   * @param numPilot  Number of pilot carriers.
   * @param numGuard  Number of guard carriers (not including DC).
   * @param outBegin  Iterator to first element of output vector.
   * @param outEnd    Iterator to one past last element of output.
   */
  static void generatePreamble(int numData,
                               int numPilot,
                               int numGuard,
                               CplxVecIt outBegin,
                               CplxVecIt outEnd)
  {
    using namespace boost::lambda;

    int numActive = numData + numPilot;
    int numBins = numData + numPilot + numGuard + 1;

    if(outEnd-outBegin < numBins)
      throw IrisException("Insufficient storage provided for generatePreamble output.");

    CplxVec bins(numBins);
    for(int i=2; i<numActive/2; i+=2)
      bins[i] = posPreambleSequence_[i%100];
    for(int i=1; i<numActive/2; i+=2)
      bins[numBins-1-i] = negPreambleSequence_[i%100];

    kissfft<float> fft(numBins,true);
    fft.transform(&bins[0], &(*outBegin));
    float scaleFactor = numActive/2.0;
    transform(outBegin, outEnd, outBegin, _1/scaleFactor);
  }

  /// Convenience function for logging.
  static std::string getName(){ return "OfdmPreambleGenerator"; }

  /// Static function used to build up posPreambleSequence_ vector.
  static CplxVec createPosPreambleSequence()
  {
    using namespace boost::lambda;
    typedef Cplx c;

    //Preamble sequence for positive frequency carriers defined in the IEEE 802.16 standard
    //(802.16 2004, Sec 8.3.3.6 - Preamble and structure for WirelessMAN OFDM PHY)
    c posSeq[] = {
      c(-1,-1),c( 1, 1),c(-1, 1),c(-1, 1),c(-1,-1),c( 1, 1),c( 1, 1),c( 1, 1),c(-1,-1),c( 1, 1),  \
      c( 1,-1),c( 1,-1),c( 1,-1),c(-1, 1),c(-1, 1),c(-1, 1),c(-1, 1),c( 1,-1),c(-1,-1),c(-1,-1),  \
      c(-1, 1),c( 1,-1),c( 1, 1),c( 1, 1),c(-1, 1),c( 1,-1),c( 1,-1),c( 1,-1),c(-1, 1),c( 1,-1),  \
      c(-1,-1),c(-1,-1),c(-1,-1),c( 1, 1),c( 1, 1),c( 1, 1),c( 1, 1),c(-1,-1),c(-1, 1),c(-1, 1),  \
      c( 1, 1),c(-1,-1),c( 1,-1),c( 1,-1),c( 1, 1),c(-1,-1),c(-1,-1),c(-1,-1),c( 1, 1),c(-1,-1),  \
      c(-1, 1),c(-1, 1),c(-1, 1),c( 1,-1),c( 1,-1),c( 1,-1),c( 1,-1),c(-1, 1),c( 1, 1),c( 1, 1),  \
      c(-1,-1),c( 1, 1),c(-1, 1),c(-1, 1),c(-1,-1),c( 1, 1),c( 1, 1),c( 1, 1),c(-1,-1),c( 1, 1),  \
      c( 1,-1),c( 1,-1),c( 1,-1),c(-1, 1),c(-1, 1),c(-1, 1),c(-1, 1),c( 1,-1),c(-1,-1),c(-1,-1),  \
      c( 1,-1),c(-1, 1),c(-1,-1),c(-1,-1),c( 1,-1),c(-1, 1),c(-1, 1),c(-1, 1),c( 1,-1),c(-1, 1),  \
      c( 1, 1),c( 1, 1),c( 1, 1),c(-1,-1),c(-1,-1),c(-1,-1),c(-1,-1),c( 1, 1),c( 1,-1),c( 1,-1)   \
    };

    CplxVec vec(begin(posSeq), end(posSeq));
    transform(vec.begin(), vec.end(), vec.begin(), _1/sqrtf(2.0f));
    return vec;
  }

  /// Static function used to build up negPreambleSequence_ vector.
  static CplxVec createNegPreambleSequence()
  {
    using namespace boost::lambda;
    typedef Cplx c;

    //Preamble sequence for negative frequency carriers defined in the IEEE 802.16 standard
    //(802.16 2004, Sec 8.3.3.6 - Preamble and structure for WirelessMAN OFDM PHY)
    c negSeq[] = {
      c( 1, 1),c(-1,-1),c(-1, 1),c(-1, 1),c( 1,-1),c(-1, 1),c( 1,-1),c( 1,-1),c(-1, 1),c( 1,-1),  \
      c(-1,-1),c(-1,-1),c(-1,-1),c( 1, 1),c(-1,-1),c(-1,-1),c( 1,-1),c(-1, 1),c(-1,-1),c(-1,-1),  \
      c(-1,-1),c( 1, 1),c( 1,-1),c( 1,-1),c(-1, 1),c( 1,-1),c(-1, 1),c(-1, 1),c( 1,-1),c(-1, 1),  \
      c( 1, 1),c( 1, 1),c( 1, 1),c(-1,-1),c( 1, 1),c( 1, 1),c(-1, 1),c( 1,-1),c( 1, 1),c( 1, 1),  \
      c(-1, 1),c( 1,-1),c(-1,-1),c(-1,-1),c( 1, 1),c(-1,-1),c( 1, 1),c( 1, 1),c(-1,-1),c( 1, 1),  \
      c( 1,-1),c( 1,-1),c( 1,-1),c(-1, 1),c( 1,-1),c( 1,-1),c( 1, 1),c(-1,-1),c( 1,-1),c( 1,-1),  \
      c(-1, 1),c( 1,-1),c(-1,-1),c(-1,-1),c( 1, 1),c(-1,-1),c( 1, 1),c( 1, 1),c(-1,-1),c( 1, 1),  \
      c( 1,-1),c( 1,-1),c( 1,-1),c(-1, 1),c( 1,-1),c( 1,-1),c( 1, 1),c(-1,-1),c( 1,-1),c( 1,-1),  \
      c(-1, 1),c( 1,-1),c(-1,-1),c(-1,-1),c( 1, 1),c(-1,-1),c( 1, 1),c( 1, 1),c(-1,-1),c( 1, 1),  \
      c( 1,-1),c( 1,-1),c( 1,-1),c(-1, 1),c( 1,-1),c( 1,-1),c( 1, 1),c(-1,-1),c( 1,-1),c( 1,-1)   \
    };

    CplxVec vec(begin(negSeq), end(negSeq));
    transform(vec.begin(), vec.end(), vec.begin(), _1/sqrtf(2.0f));
    return vec;
  }

  static const CplxVec posPreambleSequence_;
  static const CplxVec negPreambleSequence_;

 private:

  template <typename T, size_t N>
  static T* begin(T(&arr)[N]) { return &arr[0]; }
  template <typename T, size_t N>
  static T* end(T(&arr)[N]) { return &arr[0]+N; }

  OfdmPreambleGenerator(); // Disabled constructor
};

const OfdmPreambleGenerator::CplxVec OfdmPreambleGenerator::posPreambleSequence_ =
    OfdmPreambleGenerator::createPosPreambleSequence();

const OfdmPreambleGenerator::CplxVec OfdmPreambleGenerator::negPreambleSequence_ =
    OfdmPreambleGenerator::createNegPreambleSequence();

} // namespace iris

#endif // MOD_OFDMPREAMBLEGENERATOR_H_
