/*
 * This file is part of Iris 2.
 * 
 * Copyright (C) 2009 The Provost, Fellows and Scholars of the 
 * College of the Holy and Undivided Trinity of Queen Elizabeth near Dublin. 
 * All rights reserved.
 * 
 */



/*
 * file TestMain.cpp
 * Main test file for Component component
 *
 *  Created on: 24-Nov-2008
 *  Created by: jlotze
 *  $Revision: 714 $
 *  $LastChangedDate: 2009-10-20 20:14:02 +0100 (Tue, 20 Oct 2009) $
 *  $LastChangedBy: lotzej $
 */


#define BOOST_TEST_MODULE Component
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <irisapi/Logging.h>
#include <boost/test/unit_test.hpp>
#include <algorithm>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <complex>
#include <cstdio>

#include "EndianConversion.h"
#include "DataBufferTrivial.h"
#include "FileRawReaderComponent.h"

using namespace std;
using namespace iris;
using namespace boost;

BOOST_AUTO_TEST_SUITE (Component)

// an object which generates a testfile and deletes it at the end
struct FileGenerator
{
    FileGenerator()
    {
        ofstream testfile("testfile.bin");
        testvec.resize(20);
        for (size_t i = 0; i < testvec.size(); ++i)
            testvec[i] = complex<float> (i, -i);

        testfile.write(reinterpret_cast<char*> (&testvec[0]), testvec.size()
                * sizeof(complex<float> ));
    }

    ~FileGenerator()
    {
        remove("testfile.bin");
    }
    vector<complex<float> > testvec;

} x;

BOOST_AUTO_TEST_CASE(NativeRead)
{
    // instantiate & setup the component
    PNComponent* comp = new FileRawReaderComponent("filerawreadertest");
    comp->setLoggingPolicy(Logger::getPolicy());
    comp->setValue("filename", "testfile.bin");
    comp->setValue("blockSize", "10");
    comp->setValue("datatype", "complex<float>");
    comp->setValue("endian", "native");
    comp->registerPorts();

    map<string, int> outputTypes = comp->calculateOutputTypes(map<string, int>());
    DataBufferTrivial<complex<float> > write_buf;
    comp->setBuffers(vector<ReadBufferBase*>(), vector<WriteBufferBase*>(1, &write_buf));

    comp->initialize();

    comp->start();

    comp->doProcess();

    // now look at the data that has been read
    DataSet<complex<float> >* data = NULL;
    write_buf.getReadData(data);

    vector<complex<float> >::iterator itin, itref;
    itin = data->data.begin();
    itref = x.testvec.begin();
    while (itin != data->data.end())
        BOOST_CHECK_EQUAL(*itin++, *itref++);
    write_buf.releaseReadData(data);

    comp->doProcess();

    // check second block of data
    data = NULL;
    write_buf.getReadData(data);
    itin = data->data.begin();
    while (itin != data->data.end())
        BOOST_CHECK_EQUAL(*itin++, *itref++);
    write_buf.releaseReadData(data);

    comp->doProcess();

    // see if third block is at the beginning again
    data = NULL;
    write_buf.getReadData(data);
    itin = data->data.begin();
    itref = x.testvec.begin();
    while (itin != data->data.end())
        BOOST_CHECK_EQUAL(*itin++, *itref++);
    write_buf.releaseReadData(data);

    // stop & destroy
    comp->stop();
    delete comp;

}

BOOST_AUTO_TEST_CASE(BigRead)
{
    // instantiate & setup the component
    PNComponent* comp = new FileRawReaderComponent("filerawreadertest");
    comp->setLoggingPolicy(Logger::getPolicy());
    comp->setValue("filename", "testfile.bin");
    comp->setValue("blockSize", "10");
    comp->setValue("datatype", "complex<float>");
    comp->setValue("endian", "big");
    comp->registerPorts();

    map<string, int> outputTypes = comp->calculateOutputTypes(map<string, int>());
    DataBufferTrivial<complex<float> > write_buf;
    comp->setBuffers(vector<ReadBufferBase*>(), vector<WriteBufferBase*>(1, &write_buf));

    comp->initialize();

    comp->start();

    comp->doProcess();

    // now look at the data that has been read
    DataSet<complex<float> >* data = NULL;
    write_buf.getReadData(data);

    vector<complex<float> >::iterator itin, itref;
    itin = data->data.begin();
    itref = x.testvec.begin();
    while (itin != data->data.end())
        BOOST_CHECK_EQUAL(*itin++, sys2big(*itref++));
    write_buf.releaseReadData(data);

    comp->doProcess();

    // check second block of data
    data = NULL;
    write_buf.getReadData(data);
    itin = data->data.begin();
    while (itin != data->data.end())
        BOOST_CHECK_EQUAL(*itin++, sys2big(*itref++));
    write_buf.releaseReadData(data);

    comp->doProcess();

    // see if third block is at the beginning again
    data = NULL;
    write_buf.getReadData(data);
    itin = data->data.begin();
    itref = x.testvec.begin();
    while (itin != data->data.end())
        BOOST_CHECK_EQUAL(*itin++, sys2big(*itref++));
    write_buf.releaseReadData(data);

    // stop & destroy
    comp->stop();
    delete comp;

}

BOOST_AUTO_TEST_CASE(LittleRead)
{
    // instantiate & setup the component
    PNComponent* comp = new FileRawReaderComponent("filerawreadertest");
    comp->setLoggingPolicy(Logger::getPolicy());
    comp->setValue("filename", "testfile.bin");
    comp->setValue("blockSize", "10");
    comp->setValue("datatype", "complex<float>");
    comp->setValue("endian", "little");
    comp->registerPorts();

    map<string, int> outputTypes = comp->calculateOutputTypes(map<string, int>());
    DataBufferTrivial<complex<float> > write_buf;
    comp->setBuffers(vector<ReadBufferBase*>(), vector<WriteBufferBase*>(1, &write_buf));

    comp->initialize();

    comp->start();

    comp->doProcess();

    // now look at the data that has been read
    DataSet<complex<float> >* data = NULL;
    write_buf.getReadData(data);

    vector<complex<float> >::iterator itin, itref;
    itin = data->data.begin();
    itref = x.testvec.begin();
    while (itin != data->data.end())
        BOOST_CHECK_EQUAL(*itin++, sys2lit(*itref++));
    write_buf.releaseReadData(data);

    comp->doProcess();

    // check second block of data
    data = NULL;
    write_buf.getReadData(data);
    itin = data->data.begin();
    while (itin != data->data.end())
        BOOST_CHECK_EQUAL(*itin++, sys2lit(*itref++));
    write_buf.releaseReadData(data);

    comp->doProcess();

    // see if third block is at the beginning again
    data = NULL;
    write_buf.getReadData(data);
    itin = data->data.begin();
    itref = x.testvec.begin();
    while (itin != data->data.end())
        BOOST_CHECK_EQUAL(*itin++, sys2lit(*itref++));
    write_buf.releaseReadData(data);

    // stop & destroy
    comp->stop();
    delete comp;

}

BOOST_AUTO_TEST_CASE(longRead)
{
    // instantiate & setup the component
    PNComponent* comp = new FileRawReaderComponent("filerawreadertest");
    comp->setLoggingPolicy(Logger::getPolicy());
    comp->setValue("filename", "testfile.bin");
    comp->setValue("blockSize", "30");
    comp->setValue("datatype", "complex<float>");
    comp->setValue("endian", "native");
    comp->registerPorts();

    map<string, int> outputTypes = comp->calculateOutputTypes(map<string, int>());
    DataBufferTrivial<complex<float> > write_buf;
    comp->setBuffers(vector<ReadBufferBase*>(), vector<WriteBufferBase*>(1, &write_buf));

    comp->initialize();

    comp->start();

    comp->doProcess();

    // now look at the data that has been read
    DataSet<complex<float> >* data = NULL;
    write_buf.getReadData(data);

    vector<complex<float> >::iterator itin, itref;
    itin = data->data.begin();
    itref = x.testvec.begin();
    while (itref != x.testvec.end())
        BOOST_CHECK_EQUAL(*itin++, *itref++);
    itref = x.testvec.begin();
    while (itin != data->data.end())
        BOOST_CHECK_EQUAL(*itin++, *itref++);

    write_buf.releaseReadData(data);

    comp->stop();

    delete comp;
}

BOOST_AUTO_TEST_CASE(byteRead)
{
    // instantiate & setup the component
    PNComponent* comp = new FileRawReaderComponent("filerawreadertest");
    comp->setLoggingPolicy(Logger::getPolicy());
    comp->setValue("filename", "testfile.bin");
    comp->setValue("blockSize", "30");
    comp->setValue("datatype", "uint8_t");
    comp->setValue("endian", "native");
    comp->registerPorts();

    map<string, int> outputTypes = comp->calculateOutputTypes(map<string, int>());
    DataBufferTrivial<uint8_t > write_buf;
    comp->setBuffers(vector<ReadBufferBase*>(), vector<WriteBufferBase*>(1, &write_buf));

    comp->initialize();

    comp->start();

    comp->doProcess();

    // now look at the data that has been read
    DataSet<uint8_t >* data = NULL;
    write_buf.getReadData(data);

    vector<uint8_t >::iterator itin;
    const uint8_t* itref;
    itin = data->data.begin();
    itref = reinterpret_cast<const uint8_t*>(&x.testvec[0]);
    while (itin != data->data.end())
        BOOST_CHECK_EQUAL(*itin++, *itref++);
    write_buf.releaseReadData(data);

    comp->stop();

    delete comp;
}

BOOST_AUTO_TEST_SUITE_END()
