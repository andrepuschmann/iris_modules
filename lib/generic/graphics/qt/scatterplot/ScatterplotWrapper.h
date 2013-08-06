#ifndef SCATTERPLOTWRAPPER_H
#define SCATTERPLOTWRAPPER_H

#include <qapplication.h>
#include <complex>

class ScatterWidget;

class ScatterplotWrapper
    : QObject
{
  Q_OBJECT

public:
  ScatterplotWrapper();
  ~ScatterplotWrapper();

  void plotNewData(std::complex<float>* data, int numPoints);
  void plotNewData(std::complex<double>* data, int numPoints);
  void setTitle(std::string title);
  void setAxes(double xMin, double xMax,
               double yMin, double yMax);
  void setAxisLabels(std::string xLabel, std::string yLabel);

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

private:
  ScatterWidget* widget_;
};

#endif // SCATTERPLOTWRAPPER_H
