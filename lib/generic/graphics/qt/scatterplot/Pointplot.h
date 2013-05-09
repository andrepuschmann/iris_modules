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

class PlotDataEvent;

class Pointplot
  : public QwtPlot
{
  Q_OBJECT

public:
  Pointplot(QWidget* parent = 0);
  virtual ~Pointplot();

public slots:
  void customEvent( QEvent * e );
  void setPlotTitle(QString title);
  void setPlotXLabel(QString title);
  void setPlotYLabel(QString title);
  void setPlotAxes(double xMin, double xMax,
                   double yMin, double yMax);

private:
  void plotData(PlotDataEvent* e);

  QwtPlotCurve* curve_;

  QwtPlotPanner* panner_;
  QwtPlotZoomer* zoomer_;
  QwtPlotMagnifier* magnifier_;

  double* realPoints_;
  double* imagPoints_;

  int64_t numPoints_;
  int counter_;
};

#endif // POINTPLOT_H
