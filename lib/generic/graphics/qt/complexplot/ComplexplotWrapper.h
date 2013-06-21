#ifndef COMPLEXPLOTWRAPPER_H
#define COMPLEXPLOTWRAPPER_H

#include <qapplication.h>
#include <complex>

class ComplexWidget;

class ComplexplotWrapper
    : QObject
{
  Q_OBJECT

public:
  ComplexplotWrapper();
  ~ComplexplotWrapper();

  void plotNewData(std::complex<float>* data, int numPoints);
  void plotNewData(std::complex<double>* data, int numPoints);
  void setTitle(std::string title);
  void setAxes(int id, double xMin, double xMax,
               double yMin, double yMax);

public slots:
  void createWidgetSlot();
  void destroyWidgetSlot();

signals:
  void createWidgetSignal();
  void destroyWidgetSignal();
  void setWidgetTitle(QString title);
  void setWidgetAxes(int id, double xMin, double xMax,
                   double yMin, double yMax);

private:
  ComplexWidget* widget_;
};

#endif // COMPLEXPLOTWRAPPER_H
