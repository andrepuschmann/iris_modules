#include "Lineplot.h"

#include <algorithm>

Lineplot::Lineplot(QWidget *parent)
  :QwtPlot(parent)
  ,xMin_(0)
  ,xMax_(0)
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

  enableAxis(QwtPlot::yRight);
  QwtScaleWidget *leftAxis = axisWidget(QwtPlot::yLeft);
  connect(leftAxis, SIGNAL(scaleDivChanged()), this, SLOT(linkScales()));

  setAxisScaleEngine(QwtPlot::xBottom, new QwtLinearScaleEngine);
  setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine);
  setAxisScaleEngine(QwtPlot::yRight, new QwtLinearScaleEngine);

  axisScaleEngine(QwtPlot::xBottom)->setAttribute(QwtScaleEngine::Floating,true);
  axisScaleEngine(QwtPlot::yLeft)->setAttribute(QwtScaleEngine::Floating,true);
  axisScaleEngine(QwtPlot::yRight)->setAttribute(QwtScaleEngine::Floating,true);

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
    if(xMin_==xMax_)
    {
      for(int i=0;i<=numPoints_;i++)
        indexPoints_[i] = i;
    }
    else
    {
      double step = (xMax_-xMin_)/numPoints_;
      double val = xMin_;
      for(int i=0;i<numPoints_;i++,val+=step)
        indexPoints_[i] = val;
    }
    curve_->setRawSamples(indexPoints_, dataPoints_, numPoints_);
  }

  memcpy(dataPoints_, data, numPoints_*sizeof(double));

  replot();
}

void Lineplot::setTitle(QString title)
{
  setTitle(title);
}

void Lineplot::setXLabel(QString label)
{
  setAxisTitle(QwtPlot::xBottom, label);
}

void Lineplot::setYLabel(QString label)
{
  setAxisTitle(QwtPlot::yLeft, label);
}

void Lineplot::setAxes(double xMin, double xMax,
                 double yMin, double yMax)
{
  setAxisScale(QwtPlot::xBottom, xMin, xMax);
  setAxisScale(QwtPlot::yLeft, yMin, yMax);
  setAxisScale(QwtPlot::yRight, yMin, yMax);
}

void Lineplot::setXAxisRange(double xMin, double xMax)
{
  xMin_ = xMin;
  xMax_ = xMax;
  double step = (xMax_-xMin_)/numPoints_;
  double val = xMin_;
  for(int i=0;i<numPoints_;i++,val+=step)
    indexPoints_[i] = val;
  curve_->setRawSamples(indexPoints_, dataPoints_, numPoints_);
}

void Lineplot::resetZoom()
{
  zoomer_->setZoomBase();
}

void Lineplot::linkScales()
{
  setAxisScaleDiv(QwtPlot::yRight, *axisScaleDiv(QwtPlot::yLeft));
}
