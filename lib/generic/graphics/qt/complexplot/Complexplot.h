#ifndef COMPLEXPLOT_H
#define COMPLEXPLOT_H

#include <string>
#include <complex>

class ComplexplotWrapper;

class Complexplot
{
public:
  enum PlotId
  {
    I,
    Q,
    Magnitude,
    Phase
  };
  Complexplot();
  ~Complexplot();

  template<class Iterator>
  void plotNewData(Iterator begin, Iterator end);
  void plotNewData(std::complex<float>* data, int numPoints);
  void plotNewData(std::complex<double>* data, int numPoints);
  void setTitle(std::string title);
  void setAxes(PlotId id, double xMin, double xMax,
               double yMin, double yMax);
  void setXAxisRange(double xMin, double xMax);

private:
  ComplexplotWrapper* plot_;
};

template<class Iterator>
void Complexplot::plotNewData(Iterator begin, Iterator end)
{
  int numPoints = end-begin;
  std::complex<double>* data = new std::complex<double>[numPoints];

  for(int i=0;begin!=end;begin++,i++)
  {
    data[i] = *begin;
  }

  plotNewData(data, numPoints);

  delete[] data;
}

#endif // COMPLEXPLOT_H
