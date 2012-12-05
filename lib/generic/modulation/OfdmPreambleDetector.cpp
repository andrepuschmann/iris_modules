/**
 * \file lib/generic/modulation/OfdmPreambleDetector.cpp
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
 * Implementation of an OFDM preamble detector using Schmidl & Cox.
 */

#include "OfdmPreambleDetector.h"

#include "utility/MathDefines.h"

namespace iris
{

OfdmPreambleDetector::OfdmPreambleDetector(int symbolLen,
                                           int cyclicPrefixLen,
                                           float threshold)
  :sLen_(symbolLen)
  ,cpLen_(cyclicPrefixLen)
  ,thresh_(threshold*cpLen_)
  ,symbolBuffer_(sLen_+cpLen_)
  ,halfBuffer_(sLen_/2, Cplx(0,0))
  ,eBuffer_(sLen_, Cplx(0,0))
  ,pBuffer_(sLen_/2, Cplx(0,0))
  ,vBuffer_(cpLen_, 0.0)
  ,vDebugBuffer_(sLen_, 0.0)
  ,currentP_(0,0)
  ,currentE_(0,0)
  ,vMovingAve_(0.0)
  ,lastVma_(0.0)
{}

template <class Iterator>
Iterator OfdmPreambleDetector::search(Iterator inBegin,
                                      Iterator inEnd,
                                      Iterator preambleBegin,
                                      Iterator preambleEnd,
                                      float &freqOffset,
                                      float &snr)
{
  int counter=-1;

  for(; inBegin != inEnd; ++inBegin)
  {
    counter++;

    //Add current sample to symbolBuffer_
    symbolBuffer_.push_back(*inBegin);

    Cplx  nextSymbol = *inBegin;
    Cplx  midSymbol = halfBuffer_.front();

    //Add current sample to the halfBuffer_
    halfBuffer_.push_back(*inBegin);

    //Iterate for P
    Cplx  nextP = conj(nextSymbol)*midSymbol;
    Cplx  tailP = pBuffer_.front();
    currentP_ += (nextP - tailP);
    pBuffer_.push_back(nextP);

    //Iterate for E
    Cplx  nextE = conj(nextSymbol)*nextSymbol;
    Cplx  tailE = eBuffer_.front();
    currentE_ += (nextE - tailE);
    eBuffer_.push_back(nextE);

    //Use P and E values to calculate V
    float magP = abs(currentP_);
    float magE = abs(currentE_);
    float den = magE*magE;
    float v = (den == 0) ? 0 : ((2*magP)*(2*magP))/den;

    //Estimate the SNR
    snr = 10*log10(sqrtf(v)/(1-sqrtf(v)));

    //Iterate for the moving average value of V
    lastVma_ = vMovingAve_;
    vMovingAve_ += v;
    vMovingAve_ -= vBuffer_.front();
    vBuffer_.push_back(v);
    vDebugBuffer_.push_back(v);

    //Check the moving average V value for threshold and peak
    if(vMovingAve_ > thresh_ && vMovingAve_ < lastVma_)
    {
      //RawFileUtility::write(vArrayBuffer.begin(), vArrayBuffer.end(), "Output_Data//vArrayBuffer");

      //We've detected the peak - copy the preamble into output vector
      if((preambleEnd-preambleBegin) < sLen_+cpLen_)
        throw IrisException("Insufficient storage provided for preamble output");
      for(CplxBufIt it=symbolBuffer_.begin(); it!=symbolBuffer_.end(); ++it)
        *preambleBegin++ = *it;

      //RawFileUtility::write(preamble.begin(), preamble.end(), "Output_Data//preamble");

      //Phase of P gives the fractional frequency offset
      freqOffset = arg(currentP_);
      freqOffset = -(freqOffset)/(float)IRIS_PI;

      reset();
      return ++inBegin;
    }
  }

  return inBegin;
}

void OfdmPreambleDetector::reset(int symbolLen,
                                 int cyclicPrefixLen,
                                 float threshold)
{
  sLen_ = symbolLen;
  cpLen_ = cyclicPrefixLen;
  thresh_ = threshold;
  symbolBuffer_.assign(sLen_+cpLen_, Cplx(0,0));
  halfBuffer_.assign(sLen_/2, Cplx(0,0));
  eBuffer_.assign(sLen_, Cplx(0,0));
  pBuffer_.assign(sLen_/2, Cplx(0,0));
  vBuffer_.assign(cpLen_, 0);
  vDebugBuffer_.assign(sLen_, 0);
  currentP_ = Cplx(0,0);
  currentE_ = Cplx(0,0);
  vMovingAve_ = 0;
}

} // namespace iris
