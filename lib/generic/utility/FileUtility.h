/*
 * This file is part of Iris 2.
 * 
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the 
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin. 
 * All rights reserved.
 * 
 */


/**
 * \file FileUtility.h
 *
 * 	This class provides static functions for reading and writing signal data to files.
 *
 *  Created on: April-2008
 *  Created by: suttonp
 *  $Revision: 1525 $
 *  $LastChangedDate: 2012-07-20 10:40:43 +0100 (Fri, 20 Jul 2012) $
 *  $LastChangedBy: suttonp $
 *
 */

#ifndef FILEUTILITY_H
#define FILEUTILITY_H

#include <fstream>
#include "tml/tml.h"

using namespace std;

/*! \class FileUtility Class
* \brief This class provides generic functionality for reading and writing signals to and from files.
*/
class FileUtility
{
public:

	/*! Function Description.
	* \brief Write arrays of complex samples to file
	* \param input An array of complex samples to be written to file
	* \param size Number of complex samples in input
	* \param filename Name of file to write data to
	* \param scalefactor Factor by which to scale the data
	*/
	static bool write_complex_float(SCplx* input, int size, string filename, float scalefactor)
	{
		string realFileName(filename);
		string imagFileName(filename);
		realFileName += "_real.bin";
		imagFileName += "_imag.bin";

		ofstream hOutFile1(realFileName.c_str(), ios::binary | ios::trunc);
		ofstream hOutFile2(imagFileName.c_str(), ios::binary | ios::trunc);

		float real = 0;
		float imag = 0;

		if(hOutFile1.is_open() && hOutFile2.is_open())
		{
			for(int i=0;i<size;i++)
			{
				real = input[i].re * scalefactor;
				imag = input[i].im * scalefactor;
				hOutFile1.write((char*)&real, sizeof(float));
				hOutFile2.write((char*)&imag, sizeof(float));
			}
			hOutFile1.close();
			hOutFile2.close();
			return true;
		}
		return false;
	};

	/*! Function Description.
	* \brief Write arrays of floats to file
	* \param input An array of floats to be written to file
	* \param size Number of floats in input
	* \param filename Name of file to write data to
	*/
	static bool write_float(float* input, int size, string filename, float scalefactor)
	{
		string fileName(filename);
		fileName += ".bin";

		ofstream hOutFile(fileName.c_str(), ios::binary | ios::trunc);
		float output = 0;

		if(hOutFile.is_open())
		{
			for(int i=0;i<size;i++)
			{
				output = input[i] * scalefactor;
				hOutFile.write((char*)&output, sizeof(float));
			}
			hOutFile.close();
			return true;
		}

		return false;
	};

	/*! Function Description.
	* \brief Write arrays of ints to file
	* \param input An array of ints to be written to file
	* \param size Number of ints in input
	* \param filename Name of file to write data to
	*/
	static bool write_int(int* input, int size, string filename)
	{
		string fileName(filename);
		fileName += ".bin";

		ofstream hOutFile(fileName.c_str(), ios::binary | ios::trunc);

		if(hOutFile.is_open())
		{
			for(int i=0;i<size;i++)
			{
				hOutFile.write((char*)&input[i], sizeof(int));
			}
			hOutFile.close();
			return true;
		}

		return false;
	};

	/*! Function Description.
	* \brief Read arrays of complex samples from file
	* \param output An array of to hold the complex samples
	* \param size Number of complex samples to read
	* \param filename Name of file to read data from
	*/
	static bool read_complex_float(SCplx* output, int size, string filename)
	{
		string realFileName(filename);
		string imagFileName(filename);
		realFileName += "_real.bin";
		imagFileName += "_imag.bin";

		ifstream hInFile1(realFileName.c_str(), ios::binary);
		ifstream hInFile2(imagFileName.c_str(), ios::binary);

		float real = 0;
		float imag = 0;

		if(hInFile1.is_open() && hInFile2.is_open())
		{
			for(int i=0;i<size;i++)
			{
				hInFile1.read((char*)&real, sizeof(float));
				hInFile2.read((char*)&imag, sizeof(float));
				output[i].re = real;
				output[i].im = imag;
			}
			hInFile1.close();
			hInFile2.close();
			return true;
		}
		return false;
	};

	/*! Function Description.
	* \brief Read arrays of floats from file
	* \param output An array of to hold the floats
	* \param size Number of floats to read
	* \param filename Name of file to read data from
	*/
	static bool read_float(float* output, int size, string filename)
	{
		string fileName(filename);
		fileName += ".bin";

		ifstream hInFile(fileName.c_str(), ios::binary);

		if(hInFile.is_open()){
			hInFile.read((char*)output, sizeof(float)*size);
			if(hInFile.good()){
				hInFile.close();
				return true;
			}
		}
		return false;
	};
};

#endif

