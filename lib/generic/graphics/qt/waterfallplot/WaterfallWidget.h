#ifndef WATERFALLWIDGET_H
#define WATERFALLWIDGET_H

#include <qapplication.h>
#include <qwidget.h>
#include <complex>

class RealDataEvent;
class Lineplot;
class Spectrogramplot;

class WaterfallWidget
  : public QWidget
{
  Q_OBJECT

public:
  WaterfallWidget(int numDataPoints, int numRows, QWidget* parent = 0);
  virtual ~WaterfallWidget();

public slots:
  void customEvent( QEvent * e );
  void setWidgetTitle(QString title);
  void setPlotAxes(double xMin, double xMax,
                   double yMin, double yMax,
                   double zMin, double zMax);

private:
  void plotData(RealDataEvent* e);
  Lineplot* p_;
  Spectrogramplot* s_;

  double* data_;
  int numPoints_;
};

#endif // WATERFALLWIDGET_H
