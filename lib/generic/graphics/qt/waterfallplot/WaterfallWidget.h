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
  void setPlotXAxisScale(double xMin, double xMax);
  void setSpectrogramXAxisScale(double xMin, double xMax);
  void setPlotYAxisScale(double yMin, double yMax);
  void setSpectrogramYAxisScale(double yMin, double yMax);
  void setSpectrogramZAxisScale(double zMin, double zMax);

protected:
  virtual void timerEvent(QTimerEvent *event);

private:
  void appendData(RealDataEvent* e);
  Lineplot* p_;
  Spectrogramplot* s_;

  double* data_;
  int numPoints_;
  int timerId_;
  bool haveNewData_;
};

#endif // WATERFALLWIDGET_H
