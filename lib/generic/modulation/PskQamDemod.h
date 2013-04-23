/**
 * \file PskQamDemod.h
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
 * A PSK QAM demodulator class.
 */

#ifndef PSKQAMDEMOD_H_
#define PSKQAMDEMOD_H_

#include <complex>


namespace iris
{

/*!
 * \brief A PSKQam demodulation class
 *
 * The PskQamDemod class simply provides a static function
 * for demodulating a vector of samples
 */
class PskQamDemod
{
private:
	//! Disable the constructor
	PskQamDemod(){}

public:

	/*!
	 * Demodulate a vector of samples
	 * \param	inData	The vector of samples to be demodulated
	 * \param	outIt	An iterator into the output vector where we write the demodulated data
	 */
	static void demodulate(std::vector< std::complex<float> >::iterator first,
			std::vector< std::complex<float> >::iterator last,
			std::vector<uint8_t>::iterator outIt,
			unsigned int M)
	{
		int count=0;
		int symIndex,pointsPerQuadrant;
		std::complex<float> tempV;
		float biasValues[2]={2.0f/sqrtf(10.0f),0.0f};
		int qam16ConstellationLUT[16];

		qam16ConstellationLUT[0] = 15;
		qam16ConstellationLUT[1] = 13;
		qam16ConstellationLUT[2] = 12;
		qam16ConstellationLUT[3] = 14;
		
		qam16ConstellationLUT[4] = 5;
		qam16ConstellationLUT[5] = 7;
		qam16ConstellationLUT[6] = 6;
		qam16ConstellationLUT[7] = 4;
		
		qam16ConstellationLUT[8] = 0;
		qam16ConstellationLUT[9] = 2;
		qam16ConstellationLUT[10] = 3;
		qam16ConstellationLUT[11] = 1;
		
		qam16ConstellationLUT[12] = 10;
		qam16ConstellationLUT[13] = 8;
		qam16ConstellationLUT[14] = 9;
		qam16ConstellationLUT[15] = 11;
		
		switch (M)
		{
		case 2: //qpsk
			for(;first!=last;first++)
			{
				if(count%4 == 0 && count!=0)
					outIt++;
				//slice to demodulate				 
				if ((*first).real() > 0)
				{
					if ((*first).imag() > 0)
					{
						*outIt = *outIt<<2 | 0x3;
					}
					else
					{
						*outIt = *outIt<<2 | 0x2;
					}
				}
				else
				{
					if ((*first).imag() > 0)
					{
						*outIt = *outIt<<2 | 0x1;
					}
					else
					{
						*outIt = *outIt<<2 | 0x0;
					}
				}
				count++;
			}
			break;
		case 4:
			for(;first!=last;first++)
			{
				if(count%2 == 0 && count!=0)
					outIt++;

				symIndex = 0;
				pointsPerQuadrant = 4;
				tempV = *first;
				for(int j=0;j<2;j++)
				{
					if (tempV.real() > 0)
					{
						if (tempV.imag() > 0)
						{
							tempV = tempV + std::complex<float>(-biasValues[j],-biasValues[j]);
						}
						else
						{
							tempV = tempV + std::complex<float>(-biasValues[j],biasValues[j]);
							symIndex += 3*pointsPerQuadrant;
						}
					}
					else
					{
						if (tempV.imag() > 0)
						{
							tempV = tempV + std::complex<float>(biasValues[j],-biasValues[j]);
							symIndex += pointsPerQuadrant;
						}
						else
						{
							tempV = tempV + std::complex<float>(biasValues[j],biasValues[j]);
							symIndex += 2*pointsPerQuadrant;
						}
					}
					pointsPerQuadrant = pointsPerQuadrant >> 2;
				}
				*outIt = *outIt<<4 | qam16ConstellationLUT[symIndex];
				count++;
			}
			break;
		default:
			for(;first!=last;first++)
			{
				if(count%8 == 0 && count!=0)
					outIt++;
				(*first).real() > 0 ? *outIt = *outIt<<1 | 0: *outIt = *outIt<<1 | 1;
				count++;
			}
			break;
		}
	}
};

}	/* namespace iris */

#endif /* PSKQAMDEMOD_H_ */

