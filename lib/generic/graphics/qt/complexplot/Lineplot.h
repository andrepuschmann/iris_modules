#ifndef LINEPLOT_H
#define LINEPLOT_H

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

class Lineplot
  : public QwtPlot
{
  Q_OBJECT

public:
  Lineplot(QWidget* parent = 0);
  virtual ~Lineplot();

  void plotData(double* data, int n);
  void setPlotTitle(QString title);
  void setPlotXLabel(QString title);
  void setPlotYLabel(QString title);
  void setPlotAxes(double xMin, double xMax,
                   double yMin, double yMax);
  void resetZoom();

private:
  QwtPlotCurve* curve_;

  QwtPlotPanner* panner_;
  QwtPlotZoomer* zoomer_;
  QwtPlotMagnifier* magnifier_;

  double* indexPoints_;
  double* dataPoints_;

  int64_t numPoints_;
  int counter_;
};

#endif // LINEPLOT_H
