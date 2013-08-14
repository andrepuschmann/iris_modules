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
  void appendNewData(Iterator begin, Iterator end);
  void appendNewData(float* data, int numPoints);
  void appendNewData(double* data, int numPoints);
  void setTitle(std::string title);
  void setPlotXAxisScale(double xMin, double xMax);
  void setSpectrogramXAxisScale(double xMin, double xMax);
  void setPlotYAxisScale(double yMin, double yMax);
  void setSpectrogramYAxisScale(double yMin, double yMax);
  void setSpectrogramZAxisScale(double zMin, double zMax);

private:
  WaterfallplotWrapper* plot_;
};

template<class Iterator>
void Waterfallplot::appendNewData(Iterator begin, Iterator end)
{
  int numPoints = end-begin;
  double* data = new double[numPoints];

  for(int i=0;begin!=end;begin++,i++)
  {
    data[i] = *begin;
  }

  appendNewData(data, numPoints);
  delete[] data;
}

#endif // WATERFALLPLOT_H
