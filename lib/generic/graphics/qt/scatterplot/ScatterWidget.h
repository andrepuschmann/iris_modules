#ifndef SCATTERWIDGET_H
#define SCATTERWIDGET_H

#include <qapplication.h>
#include <qwidget.h>
#include <complex>

class ComplexDataEvent;
class Pointplot;

class ScatterWidget
  : public QWidget
{
  Q_OBJECT

public:
  ScatterWidget(QWidget* parent = 0);
  virtual ~ScatterWidget();

public slots:
  void customEvent( QEvent * e );
  void setWidgetTitle(QString title);
  void setWidgetAxisLabels(QString xLabel, QString yLabel);
  void setWidgetAxes(double xMin, double xMax,
                     double yMin, double yMax);

private:
  void plotData(ComplexDataEvent* e);
  Pointplot* plot_;

  struct opReal{double operator()(std::complex<double> i) const{return real(i);}};
  struct opImag{double operator()(std::complex<double> i) const{return imag(i);}};

  double* iData_;
  double* qData_;
  int numPoints_;

};

#endif // SCATTERWIDGET_H
