#ifndef SCATTERPLOT_H
#define SCATTERPLOT_H

#include <string>
#include <complex>

class ScatterplotWrapper;

class Scatterplot
{
public:
  Scatterplot();
  ~Scatterplot();

  template<class Iterator>
  void plotNewData(Iterator begin, Iterator end);
  void plotNewData(std::complex<float>* data, int numPoints);
  void plotNewData(std::complex<double>* data, int numPoints);
  void setTitle(std::string title);
  void setAxes(double xMin, double xMax,
               double yMin, double yMax);
  void setAxisLabels(std::string xLabel, std::string yLabel);

private:
  ScatterplotWrapper* plot_;
};

template<class Iterator>
void Scatterplot::plotNewData(Iterator begin, Iterator end)
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

#endif // SCATTERPLOT_H
