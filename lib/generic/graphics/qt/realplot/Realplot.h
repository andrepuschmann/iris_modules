#ifndef REALPLOT_H
#define REALPLOT_H

#include <string>

class LineplotWrapper;

class Realplot
{
public:
  Realplot();
  ~Realplot();

  template<class Iterator>
  void plotNewData(Iterator begin, Iterator end);
  void plotNewData(float* data, int numPoints);
  void plotNewData(double* data, int numPoints);
  void setAxes(double xMin, double xMax,
               double yMin, double yMax);
  void setTitle(std::string title);
  void setXLabel(std::string label);
  void setYLabel(std::string label);

private:
  LineplotWrapper* plot_;
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
