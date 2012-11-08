/*
 * This file is part of Iris 2.
 * 
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the 
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin. 
 * All rights reserved.
 * 
 */


/**
 * \file RawFileUtility.h
 *
 * 	This class provides static functions for reading and writing signal data to files.
 *
 *  Created on: Apr-2008
 *  Created by: suttonp
 *  $Revision: 865 $
 *  $LastChangedDate: 2010-03-17 10:33:49 +0000 (Wed, 17 Mar 2010) $
 *  $LastChangedBy: suttonp $
 *
 */

#ifndef RAWFILEUTILITY_H
#define RAWFILEUTILITY_H

#include <fstream>
#include "EndianConversion.h"

/*! \class RawFileUtility Class
* \brief This class provides generic functionality for reading and writing signals to and from files.
*/
class RawFileUtility
{
public:

	/*! Function Description.
	* \brief Write anything to file
	* \param first An iterator to the first element 
	* \param last An iterator to the last element
	* \param filename Name of file to write data to
	* \param endianness	The endianness with which data should be written
	*/
	template <typename Iter>
	static bool write(Iter first, Iter last, std::string filename, std::string endianness = "native")
	{
		using namespace std;

		string fileName(filename);
		fileName += ".bin";

		ofstream hOutFile(fileName.c_str(), ios::binary | ios::trunc);
		// file will be closed during deconstruction

		if(hOutFile.is_open())
		{
			return write(first, last, hOutFile, endianness);
		}
		else
			return false;
	};

	/*! Function Description.
	* \brief Write anything to file
	* \param first		An iterator to the first element 
	* \param last		An iterator to the last element
	* \param hOutFile	An ofstream to write to
	* \param endianness	The endianness with which data should be written
	*/
	template <typename Iter>
	static bool write(Iter first, Iter last, std::ostream &hOutFile, std::string endianness = "native")
	{
		using namespace std;

		for(;first != last;++first)
		{
			typename iterator_traits<Iter>::value_type temp = *first;
			if(sizeof(temp) > 1)
			{
				if(endianness == "big")
				{
					temp = sys2big(temp);
				}
				if(endianness == "little")
				{
					temp = sys2lit(temp);
				}
			}
			hOutFile.write(reinterpret_cast<char*>(&temp), sizeof(temp));
		}
		return true;
	};
};

#endif

