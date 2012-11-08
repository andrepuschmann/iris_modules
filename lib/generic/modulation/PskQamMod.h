/*
 * This file is part of Iris 2.
 * 
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the 
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin. 
 * All rights reserved.
 * 
 */



/**
 * \file PskQamMod.h
 * A PSK QAM modulator class
 *
 *  Created on: 14-Sept-2009
 *  Created by: ozgulb
 *  $Revision: 1533 $
 *  $LastChangedDate: 2012-07-20 17:26:11 +0100 (Fri, 20 Jul 2012) $
 *  $LastChangedBy: suttonp $
 *
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

