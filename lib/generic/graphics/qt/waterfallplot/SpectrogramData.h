#ifndef SPECTROGRAMDATA_H
#define SPECTROGRAMDATA_H

#include <qwt_raster_data.h>
#include <boost/circular_buffer.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include "irisapi/Exceptions.h"


class SpectrogramData
    :public QwtRasterData
{
public:
  typedef boost::shared_ptr< std::vector<double> >  VecPtr;
  typedef boost::circular_buffer< VecPtr >          VecPtrBuf;
  typedef VecPtrBuf::iterator                       VecPtrBufIt;

  SpectrogramData(int numDataPoints, int numRows)
    :QwtRasterData()
    ,nData_(numDataPoints)
    ,nRows_(numRows)
    ,data_(numRows)
  {
    for(int i=0;i<nRows_;i++)
    {
      data_.push_front(VecPtr(new std::vector<double>(nData_)));
      data_[0]->assign(nData_, 0.0);
    }
  }

  void appendData(double* data, int n)
  {
    if(n != nData_)
      throw iris::InvalidDataException("SpectrogramData: invalid data length");

    VecPtr v = data_.back();
    v->assign(data, data+n);
    data_.push_front(v);
  }

  double value(double x, double y) const
  {
    double bottom = interval(Qt::YAxis).minValue();
    double top = interval(Qt::YAxis).maxValue();
    double left = interval(Qt::XAxis).minValue();
    double right = interval(Qt::XAxis).maxValue();

    int ix = static_cast<int>((x-left)/(right-left) * (nData_-1));
    int iy = static_cast<int>((y-top)/(bottom-top) * (nRows_-1));
    double ret = (*data_[iy])[ix];
    return ret;
  }

private:
  VecPtrBuf data_;
  int nData_;
  int nRows_;
};

#endif // SPECTROGRAMDATA_H
