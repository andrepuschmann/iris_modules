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

  void appendNewData(float* data, int numPoints);
  void appendNewData(double* data, int numPoints);
  void setTitle(std::string title);
  void setPlotXAxisScale(double xMin, double xMax);
  void setSpectrogramXAxisScale(double xMin, double xMax);
  void setPlotYAxisScale(double yMin, double yMax);
  void setSpectrogramYAxisScale(double yMin, double yMax);
  void setSpectrogramZAxisScale(double zMin, double zMax);

public slots:
  void createWidgetSlot(int numDataPoints, int numRows);
  void destroyWidgetSlot();

signals:
  void createWidgetSignal(int numDataPoints, int numRows);
  void destroyWidgetSignal();
  void setWidgetTitle(QString title);
  void setWidgetPXAxisScale(double xMin, double xMax);
  void setWidgetSXAxisScale(double xMin, double xMax);
  void setWidgetPYAxisScale(double yMin, double yMax);
  void setWidgetSYAxisScale(double yMin, double yMax);
  void setWidgetSZAxisScale(double zMin, double zMax);

private:
  WaterfallWidget* widget_;
};

#endif // WATERFALLPLOTWRAPPER_H
