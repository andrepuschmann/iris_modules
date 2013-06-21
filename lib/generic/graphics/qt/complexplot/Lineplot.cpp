#include "Lineplot.h"

#include <algorithm>

Lineplot::Lineplot(QWidget *parent)
  :QwtPlot(parent)
{
  counter_ = 0;
  numPoints_ = 1;
  indexPoints_ = new double[numPoints_];
  dataPoints_ = new double[numPoints_];

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  QPalette palette;
  palette.setColor(canvas()->backgroundRole(), QColor("white"));
  canvas()->setPalette(palette);

  curve_ = new QwtPlotCurve("Curve");
  curve_->attach(this);
  curve_->setPen(QPen(Qt::blue, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  curve_->setStyle(QwtPlotCurve::Lines);
  curve_->setRawSamples(indexPoints_, dataPoints_, numPoints_);

  memset(dataPoints_, 0x0, numPoints_*sizeof(double));
  for(int i=0;i<numPoints_;i++)
    indexPoints_[i] = i;

  setAxisScaleEngine(QwtPlot::xBottom, new QwtLinearScaleEngine);
  setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine);

  axisScaleEngine(QwtPlot::xBottom)->setAttribute(QwtScaleEngine::Floating,true);
  axisScaleEngine(QwtPlot::yLeft)->setAttribute(QwtScaleEngine::Floating,true);

  zoomer_ = new QwtPlotZoomer(canvas());
  zoomer_->setMousePattern(QwtEventPattern::MouseSelect1, Qt::LeftButton);
  zoomer_->setMousePattern(QwtEventPattern::MouseSelect2, Qt::LeftButton,
                           Qt::ControlModifier);

  panner_ = new QwtPlotPanner(canvas());
  panner_->setMouseButton(Qt::RightButton);

  magnifier_ = new QwtPlotMagnifier(canvas());
  magnifier_->setMouseButton(Qt::NoButton);

}

Lineplot::~Lineplot()
{
  delete[] indexPoints_;
  delete[] dataPoints_;
}

void Lineplot::plotData(double* data, int n)
{
  if(numPoints_ != n)
  {
    numPoints_ = n;
    delete[] indexPoints_;
    delete[] dataPoints_;
    indexPoints_ = new double[numPoints_];
    dataPoints_ = new double[numPoints_];
    for(int i=0;i<numPoints_;i++)
      indexPoints_[i] = i;
    curve_->setRawSamples(indexPoints_, dataPoints_, numPoints_);
  }

  memcpy(dataPoints_, data, numPoints_*sizeof(double));

  replot();
}

void Lineplot::setPlotTitle(QString title)
{
  setTitle(title);
}

void Lineplot::setPlotXLabel(QString label)
{
  setAxisTitle(QwtPlot::xBottom, label);
}

void Lineplot::setPlotYLabel(QString label)
{
  setAxisTitle(QwtPlot::yLeft, label);
}

void Lineplot::setPlotAxes(double xMin, double xMax,
                 double yMin, double yMax)
{
  setAxisScale(QwtPlot::xBottom, xMin, xMax);
  setAxisScale(QwtPlot::yLeft, yMin, yMax);
}

void Lineplot::resetZoom()
{
  zoomer_->setZoomBase();
}
