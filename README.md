# Iris_Modules Project

Iris is a software architecture for building highly reconfigurable radio networks using a component-based design. The architecture comprises two repositories - Iris_Core and Iris_Modules. Iris_Core contains the core part of the architecture such as parsers, managers and engines. Iris_Modules contain the components which can be used to create your software radio such as PHY-layer components and radio controllers.

## Description

The Iris architecture offers support for all layers of the network stack and provides a platform for the development of not only reconfigurable point-to-point radio links but complete networks of reconfigurable radios. Individual radios are described using an XML document. This lists the components which comprise the radio, gives the values to be used for their parameters and describes the connections between them.

Iris was originally developed by CTVR, The Telecommunications Research Centre, based at University of Dublin, Trinity College. In 2013, it was released under the LGPL v3 license and is currently managed by Software Radio Systems (http://www.softwareradiosystems.com).

## Getting Started

The installation guide can be found here: http://www.softwareradiosystems.com/redmine/projects/iris/wiki/Installing_Iris

## Requirements

Required:
* CMake 2.6 or later - http://www.cmake.org/
* Boost 1.46 or later - http://www.boost.org/
* Iris_Core

Optional:
* Qt 4.8 - http://qt-project.org/ (For graphical widgets)
* Qwt 6 - http://qwt.sourceforge.net/ (For graphical widgets)
* UHD - http://code.ettus.com/redmine/ettus/projects/uhd/wiki (For using USRP hardware)
* FFTW - http://www.fftw.org/ (For FFT transforms)
* Liquid-DSP - https://github.com/jgaeddert/liquid-dsp (For some PHY components)
* Google Protocol Buffers - https://developers.google.com/protocol-buffers/ (For some Stack components)
* Python - http://www.python.org/ (For PythonPlotter widget)
* Matlab (For MatlabTemplate PHY component, MatlabPlotter widget)

## Where To Get Help

* Redmine: http://www.softwareradiosystems.com/redmine/projects/iris
* Iris-discuss mailing list: http://www.softwareradiosystems.com/mailman/listinfo/iris-discuss
* Iris blog: http://irissoftwareradio.wordpress.com

## License

Iris is licensed under LGPL v3. For more information see LICENSE.

[![githalytics.com alpha](https://cruel-carlota.pagodabox.com/7bc7ae4f73d901c2cb1590fbe0c49cd9 "githalytics.com")](http://githalytics.com/softwareradiosystems/iris_modules)


[![Bitdeli Badge](https://d2weczhvl823v0.cloudfront.net/softwareradiosystems/iris_modules/trend.png)](https://bitdeli.com/free "Bitdeli Badge")

