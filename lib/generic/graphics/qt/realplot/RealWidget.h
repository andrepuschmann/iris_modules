#ifndef REALWIDGET_H
#define REALWIDGET_H

#include <qapplication.h>
#include <qwidget.h>
#include <complex>

class RealDataEvent;
class Lineplot;

class RealWidget
  : public QWidget
{
  Q_OBJECT

public:
  RealWidget(QWidget* parent = 0);
  virtual ~RealWidget();

public slots:
  void customEvent( QEvent * e );
  void setWidgetTitle(QString title);
  void setWidgetAxisLabels(QString xLabel, QString yLabel);
  void setWidgetAxes(double xMin, double xMax,
                     double yMin, double yMax);
  void setWidgetXAxisRange(double xMin, double xMax);

private:
  void plotData(RealDataEvent* e);
  Lineplot* l_; //The line plot

  double* dataPoints_;
  int numPoints_;

};

#endif // REALWIDGET_H
