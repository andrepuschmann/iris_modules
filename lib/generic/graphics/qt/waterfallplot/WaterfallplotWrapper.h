#ifndef WATERFALLPLOTWRAPPER_H
#define WATERFALLPLOTWRAPPER_H

#include <qapplication.h>

class WaterfallWidget;

class WaterfallplotWrapper
    : QObject
{
  Q_OBJECT

public:
  WaterfallplotWrapper(int numDataPoints, int numRows);
  ~WaterfallplotWrapper();

  void plotNewData(float* data, int numPoints);
  void plotNewData(double* data, int numPoints);
  void setTitle(std::string title);
  void setAxes(double xMin, double xMax,
               double yMin, double yMax,
               double zMin, double zMax);

public slots:
  void createWidgetSlot(int numDataPoints, int numRows);
  void destroyWidgetSlot();

signals:
  void createWidgetSignal(int numDataPoints, int numRows);
  void destroyWidgetSignal();
  void setWidgetTitle(QString title);
  void setWidgetAxes(double xMin, double xMax,
                     double yMin, double yMax,
                     double zMin, double zMax);

private:
  WaterfallWidget* widget_;
};

#endif // WATERFALLPLOTWRAPPER_H
