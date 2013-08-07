#ifndef WATERFALLPLOT_H
#define WATERFALLPLOT_H

#include <string>

class WaterfallplotWrapper;

class Waterfallplot
{
public:
  Waterfallplot(int numDataPoints, int numRows);
  ~Waterfallplot();

  template<class Iterator>
  void plotNewData(Iterator begin, Iterator end);
  void plotNewData(float* data, int numPoints);
  void plotNewData(double* data, int numPoints);
  void setTitle(std::string title);
  void setAxes(double xMin, double xMax,
               double yMin, double yMax,
               double zMin, double zMax);

private:
  WaterfallplotWrapper* plot_;
};

template<class Iterator>
void Waterfallplot::plotNewData(Iterator begin, Iterator end)
{
  int numPoints = end-begin;
  double* data = new double[numPoints];

  for(int i=0;begin!=end;begin++,i++)
  {
    data[i] = *begin;
  }

  plotNewData(data, numPoints);
  delete[] data;
}

#endif // WATERFALLPLOT_H
