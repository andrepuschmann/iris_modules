#ifndef SCATTERPLOT_H
#define SCATTERPLOT_H

#include <string>

class PointplotWrapper;

class Scatterplot
{
public:
  Scatterplot();
  ~Scatterplot();

  template<class Iterator>
  void plotNewData(Iterator begin, Iterator end);
  void plotNewData(float* real, float* imag, int numPoints);
  void plotNewData(double* real, double* imag, int numPoints);
  void setAxes(double xMin, double xMax,
               double yMin, double yMax);
  void setTitle(std::string title);
  void setXLabel(std::string label);
  void setYLabel(std::string label);

private:
  PointplotWrapper* plot_;
};

template<class Iterator>
void Scatterplot::plotNewData(Iterator begin, Iterator end)
{
  int numPoints = end-begin;
  double* real = new double[numPoints];
  double* imag = new double[numPoints];

  for(int i=0;begin!=end;begin++,i++)
  {
    real[i] = begin->real();
    imag[i] = begin->imag();
  }

  plotNewData(real, imag, numPoints);

  delete[] real;
  delete[] imag;
}

#endif // SCATTERPLOT_H
