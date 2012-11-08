/*
 * This file is part of Iris 2.
 *
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin.
 * All rights reserved.
 *
 */

/** \file FirFilter.h
 * FIR Filter classes
 *
 *  Created on: 2009-11-19
 *  Created by: lotzej
 *  $Revision: 727 $
 *  $LastChangedDate: 2009-11-19 17:54:43 +0000 (Thu, 19 Nov 2009) $
 *  $LastChangedBy: lotzej $

 */

#ifndef _FIRFILTER_H_
#define _FIRFILTER_H_

#include <vector>
#include <deque>
#include <algorithm>

#include <boost/lambda/lambda.hpp>

namespace iris
{

//! Filters input data by applying an FIR filter with the 
//! given coefficients
template<class InT, class CoeffT = InT, class OutT = InT>
class FirFilter
{
public:
    //! default constructor - no coefficients are set
    FirFilter()
    {
    }

    //! constructor setting the filter coefficients
    template<class It>
    FirFilter(It coeff_start, It coeff_end) :
        d_coeffs(coeff_start, coeff_end), d_sums(d_coeffs.size() + 1)
    {
    }

    //! set the filter coefficients
    template<class It>
    void setCoeffs(It coeff_start, It coeff_end)
    {
        d_coeffs.clear();
        d_coeffs.assign(coeff_start, coeff_end);
        d_sums.clear();
        d_sums.resize(d_coeffs.size() + 1);
    }

    //! \brief Apply filter to given input sequence, writing output to output iterator.
    //! Make sure that output has enough capacity to hold input.size() values.
    //! @return Iterator pointing to one past the end of the output sequence.
    template<class InIt, class OutIt>
    OutIt filter(InIt istart, InIt iend, OutIt ostart)
    {
        while (istart != iend)
        {
            // multiply with each coefficient & add
            std::transform(d_coeffs.begin(), d_coeffs.end(), ++d_sums.begin(), d_sums.begin(),
                    *istart++ * boost::lambda::_1 + boost::lambda::_2);
            *ostart++ = d_sums.front();
        }
        return ostart;
    }
private:
    std::vector<CoeffT> d_coeffs; //!< coefficient vector
    std::vector<OutT> d_sums; //!< keeps sums in a delay line
};

//! \brief Upsampling & interpolating filter.
//! Upsamples input data and applies filter to the upsampled data
//! avoiding all the multiply by zero operations (filter works at the input
//! rate)
template<class InT, class CoeffT = InT, class OutT = InT>
class FirFilterUpsamp
{
public:
    //! Default constructor -- no coefficients are set
    FirFilterUpsamp()
    {
    }

    //! Constructor initialising the coefficients.
    //! Also sets the upsampling factor
    template<class It>
    FirFilterUpsamp(unsigned factor, It start, It end) :
        d_coeffs(start, end), d_delayLine(d_coeffs.size()), d_factor(factor)
    {
    }

    //! initialises the coefficients
    template<class It>
    void setCoeffs(It start, It end)
    {
        d_coeffs.clear();
        d_coeffs.assign(start, end);
        d_delayLine.clear();
        d_delayLine.resize(d_coeffs.size());
    }

    std::vector<CoeffT> getCoeffs() { return d_coeffs; }

    //! set the upsampling factor of the filter
    void setUpsamplingFactor(unsigned factor)
    {
        d_factor = factor;
    }

    //! \brief Apply filter to given input sequence, writing output to output iterator.
    //! Make sure that output has enough capacity to hold input.size() * factor values.
    //! @return Iterator pointing to one past the end of the output sequence.
    template<class InIt, class OutIt>
    OutIt filter(InIt istart, InIt iend, OutIt ostart)
    {
        while (istart != iend)
        {
            d_delayLine.pop_back();
            d_delayLine.push_front(*istart++);

            OutIt oend = ostart;
            advance(oend, d_factor);
            coeff_iterator coeff_start = d_coeffs.begin();
            coeff_iterator cend = d_coeffs.end();
            while (ostart != oend)
            {
                *ostart = OutT();
                delay_iterator di = d_delayLine.begin();
                coeff_iterator ci = coeff_start++;
                while (ci < cend)
                {
                    *ostart += *ci * *di++;
                    ci += d_factor;
                }
                ostart++;
            }
        }
        return ostart;
    }

private:
    std::vector<CoeffT> d_coeffs; //!< vector holding the coefficients
    std::deque<InT> d_delayLine; //!< vector holding the delay line
    unsigned d_factor; //!< upsampling factor

    //! iterator type for the coefficients vector
    typedef typename std::vector<CoeffT>::iterator coeff_iterator;
    //! iterator type for the delay line deque
    typedef typename std::deque<InT>::iterator delay_iterator;
};

} // end of iris namespace

#endif

