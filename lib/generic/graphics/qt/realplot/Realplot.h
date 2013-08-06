#ifndef REALPLOT_H
#define REALPLOT_H

#include <string>

class RealplotWrapper;

class Realplot
{
public:
  Realplot();
  ~Realplot();

  template<class Iterator>
  void plotNewData(Iterator begin, Iterator end);
  void plotNewData(float* data, int numPoints);
  void plotNewData(double* data, int numPoints);
  void setTitle(std::string title);
  void setAxes(double xMin, double xMax,
               double yMin, double yMax);
  void setXAxisRange(double xMin, double xMax);
  void setLabels(std::string xLabel, std::string yLabel);

private:
  RealplotWrapper* plot_;
};

template<class Iterator>
void Realplot::plotNewData(Iterator begin, Iterator end)
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

#endif // REALPLOT_H
