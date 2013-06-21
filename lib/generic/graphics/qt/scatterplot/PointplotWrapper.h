#ifndef POINTPLOTWRAPPER_H
#define POINTPLOTWRAPPER_H

#include <qapplication.h>

class Pointplot;

class PointplotWrapper
    : QObject
{
  Q_OBJECT

public:
  PointplotWrapper();
  ~PointplotWrapper();

  void plotNewData(float* real, float* imag, int numPoints);
  void plotNewData(double* real, double* imag, int numPoints);
  void setAxes(double xMin, double xMax,
               double yMin, double yMax);
  void setTitle(std::string title);
  void setXLabel(std::string label);
  void setYLabel(std::string label);

public slots:
  void createPlotSlot();
  void destroyPlotSlot();

signals:
  void createPlotSignal();
  void destroyPlotSignal();
  void setPlotTitle(QString title);
  void setPlotXLabel(QString title);
  void setPlotYLabel(QString title);
  void setPlotAxes(double xMin, double xMax,
                   double yMin, double yMax);

private:
  Pointplot* plot_;
};

#endif // POINTPLOTWRAPPER_H
