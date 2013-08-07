#ifndef POINTPLOT_H
#define POINTPLOT_H

#include <qapplication.h>
#include <qwt_plot.h>
#include <qwt_painter.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_engine.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <complex>

class Pointplot
  : public QwtPlot
{
  Q_OBJECT

public:
  Pointplot(QWidget* parent = 0);
  virtual ~Pointplot();

  void plotData(double* iData, double* qData, int n);
  void setTitle(QString title);
  void setXLabel(QString title);
  void setYLabel(QString title);
  void setAxes(double xMin, double xMax,
               double yMin, double yMax);

private:
  QwtPlotCurve* curve_;

  QwtPlotPanner* panner_;
  QwtPlotZoomer* zoomer_;
  QwtPlotMagnifier* magnifier_;

  struct opReal{double operator()(std::complex<double> i) const{return real(i);}};
  struct opImag{double operator()(std::complex<double> i) const{return imag(i);}};

  double* realPoints_;
  double* imagPoints_;

  int64_t numPoints_;
  int counter_;
};

#endif // POINTPLOT_H
