/**
 * \file PskQamMod.h
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
 * A PSK QAM modulator class.
 */

#ifndef PSKQAMMOD_H_
#define PSKQAMMOD_H_

#include <complex>


namespace iris
{

/*!
 * \brief A PSKQam modulation class
 *
 * The PskQamMod class simply provides a
 * static function which modulates a vector of data
 */
class PskQamMod
{
private:
	PskQamMod(){}	//Disable the constructor

public:

	/*!
	 * Modulate a vector of data
	 * \param	inData	The vector of data to be modulated
	 * \param	outData	A vector to hold the modulated data
	 */
	static void modulate(std::vector<uint8_t>::iterator first,
			std::vector<uint8_t>::iterator last,
			std::vector< std::complex<float> > &outData,
			unsigned int M)
	{
		using namespace std;

		//Create our LUT for bpsk
		complex<float> BpskLUT[2];
		BpskLUT[0] = complex<float>(1,0);
		BpskLUT[1] = complex<float>(-1,0);

		//Create our LUT for qpsk. Gray coded. Unit average energy.
		complex<float> QpskLUT[4];
		QpskLUT[0] = complex<float>(-1.0f/sqrtf(2.0f),-1.0f/sqrtf(2.0f));
		QpskLUT[1] = complex<float>(-1.0f/sqrtf(2.0f),1.0f/sqrtf(2.0f));
		QpskLUT[2] = complex<float>(1.0f/sqrtf(2.0f),-1.0f/sqrtf(2.0f));
		QpskLUT[3] = complex<float>(1.0f/sqrtf(2.0f),1.0f/sqrtf(2.0f));

		//Create our LUT for 16 QAM. Gray coded. Unit average energy.
		complex<float> Qam16LUT[16];
		Qam16LUT[0] = complex<float>(-1.0f/sqrtf(10.0f),-1.0f/sqrtf(10.0f));
		Qam16LUT[1] = complex<float>(-1.0f/sqrtf(10.0f),-3.0f/sqrtf(10.0f));
		Qam16LUT[2] = complex<float>(-3.0f/sqrtf(10.0f),-1.0f/sqrtf(10.0f));
		Qam16LUT[3] = complex<float>(-3.0f/sqrtf(10.0f),-3.0f/sqrtf(10.0f));

		Qam16LUT[4] = complex<float>(-1.0f/sqrtf(10.0f),1.0f/sqrtf(10.0f));
		Qam16LUT[5] = complex<float>(-1.0f/sqrtf(10.0f),3.0f/sqrtf(10.0f));
		Qam16LUT[6] = complex<float>(-3.0f/sqrtf(10.0f),1.0f/sqrtf(10.0f));
		Qam16LUT[7] = complex<float>(-3.0f/sqrtf(10.0f),3.0f/sqrtf(10.0f));

		Qam16LUT[8] = complex<float>(1.0f/sqrtf(10.0f),-1.0f/sqrtf(10.0f));
		Qam16LUT[9] = complex<float>(1.0f/sqrtf(10.0f),-3.0f/sqrtf(10.0f));
		Qam16LUT[10] = complex<float>(3.0f/sqrtf(10.0f),-1.0f/sqrtf(10.0f));
		Qam16LUT[11] = complex<float>(3.0f/sqrtf(10.0f),-3.0f/sqrtf(10.0f));

		Qam16LUT[12] = complex<float>(1.0f/sqrtf(10.0f),1.0f/sqrtf(10.0f));
		Qam16LUT[13] = complex<float>(1.0f/sqrtf(10.0f),3.0f/sqrtf(10.0f));
		Qam16LUT[14] = complex<float>(3.0f/sqrtf(10.0f),1.0f/sqrtf(10.0f));
		Qam16LUT[15] = complex<float>(3.0f/sqrtf(10.0f),3.0f/sqrtf(10.0f));

		//Clear the output vector and reserve sufficient size
		outData.clear();
		outData.reserve((last-first)*8/M);

		switch (M)
		{
		case 2: //qpsk
			//Convert bytes into bit-sequences and use LUT
			for(; first != last; first++){
				for (int j = 3; j>=0; j--){
					outData.push_back( QpskLUT[(int)((*first >> (j*2)) & 0x3)] );
				}
			}
			break;
		case 4: //16 QAM
			//Convert bytes into bit-sequences and use LUT
			for(; first != last; first++){
				for (int j = 1; j>=0; j--){
					outData.push_back( Qam16LUT[(int)((*first >> (j*4)) & 0xF)] );
				}
			}
			break;
		default : //bpsk
			//Convert bytes into bit-sequences and use LUT
			for(; first != last; first++){
				for (int j = 7; j>=0; j--){
					outData.push_back( BpskLUT[(int)((*first >> j) & 0x1)] );
				}
			}
			break;
		}
	}
};

}	/* namespace iris */

#endif /* PSKQAMMOD_H_ */

