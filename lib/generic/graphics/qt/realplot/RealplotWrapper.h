#ifndef REALPLOTWRAPPER_H
#define REALPLOTWRAPPER_H

#include <qapplication.h>

class RealWidget;

class RealplotWrapper
    : QObject
{
  Q_OBJECT

public:
  RealplotWrapper();
  ~RealplotWrapper();

  void plotNewData(float* data, int numPoints);
  void plotNewData(double* data, int numPoints);
  void setTitle(std::string title);
  void setAxisLabels(std::string xLabel, std::string yLabel);
  void setAxes(double xMin, double xMax,
               double yMin, double yMax);
  void setXAxisRange(double xMin, double xMax);

public slots:
  void createWidgetSlot();
  void destroyWidgetSlot();

signals:
  void createWidgetSignal();
  void destroyWidgetSignal();
  void setWidgetTitle(QString title);
  void setWidgetAxisLabels(QString xLabel, QString yLabel);
  void setWidgetAxes(double xMin, double xMax,
                     double yMin, double yMax);
  void setWidgetXAxisRange(double xMin, double xMax);

private:
  RealWidget* widget_;
};

#endif // REALPLOTWRAPPER_H
