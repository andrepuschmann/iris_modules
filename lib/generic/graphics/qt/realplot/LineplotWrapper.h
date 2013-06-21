#ifndef LINEPLOTWRAPPER_H
#define LINEPLOTWRAPPER_H

#include <qapplication.h>

class Lineplot;

class LineplotWrapper
    : QObject
{
  Q_OBJECT

public:
  LineplotWrapper();
  ~LineplotWrapper();

  void plotNewData(float* data, int numPoints);
  void plotNewData(double* data, int numPoints);
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
  Lineplot* plot_;
};

#endif // LINEPLOTWRAPPER_H
