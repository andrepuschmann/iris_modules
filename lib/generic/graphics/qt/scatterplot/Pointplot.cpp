#include "Pointplot.h"
#include "PointplotEvents.h"

Pointplot::Pointplot(QWidget *parent)
  :QwtPlot(parent)
{
  counter_ = 0;
  numPoints_ = 1024;
  realPoints_ = new double[numPoints_];
  imagPoints_ = new double[numPoints_];

  QPalette palette;
  palette.setColor(canvas()->backgroundRole(), QColor("white"));
  canvas()->setPalette(palette);

  setAxisScaleEngine(QwtPlot::xBottom, new QwtLinearScaleEngine);
  setAxisScale(QwtPlot::xBottom, -2.0, 2.0);
  setAxisTitle(QwtPlot::xBottom, "In-phase");

  setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine);
  setAxisScale(QwtPlot::yLeft, -2.0, 2.0);
  setAxisTitle(QwtPlot::yLeft, "Quadrature");

  curve_ = new QwtPlotCurve("Constellation Points");
  curve_->attach(this);
  curve_->setPen(QPen(Qt::blue, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  curve_->setStyle(QwtPlotCurve::Dots);
  curve_->setRawSamples(realPoints_, imagPoints_, numPoints_);

  memset(realPoints_, 0x0, numPoints_*sizeof(double));
  memset(imagPoints_, 0x0, numPoints_*sizeof(double));

  zoomer_ = new QwtPlotZoomer(canvas());
  zoomer_->setMousePattern(QwtEventPattern::MouseSelect1, Qt::LeftButton);
  zoomer_->setMousePattern(QwtEventPattern::MouseSelect2, Qt::LeftButton,
                           Qt::ControlModifier);

  panner_ = new QwtPlotPanner(canvas());
  panner_->setMouseButton(Qt::RightButton);

  magnifier_ = new QwtPlotMagnifier(canvas());
  magnifier_->setMouseButton(Qt::NoButton);

}

Pointplot::~Pointplot()
{
  delete[] realPoints_;
  delete[] imagPoints_;
}

void Pointplot::customEvent( QEvent * e )
{
  if(e->type() == PlotDataEvent::type)
  {
    PlotDataEvent* dataEvent = (PlotDataEvent*)e;
    plotData(dataEvent);
  }
}

void Pointplot::plotData(PlotDataEvent* e)
{
  if(numPoints_ != e->numPoints_)
  {
    numPoints_ = e->numPoints_;
    delete[] realPoints_;
    delete[] imagPoints_;
    realPoints_ = new double[numPoints_];
    imagPoints_ = new double[numPoints_];
    curve_->setRawSamples(realPoints_, imagPoints_, numPoints_);
  }

  memcpy(realPoints_, e->realPoints_, numPoints_*sizeof(double));
  memcpy(imagPoints_, e->imagPoints_, numPoints_*sizeof(double));

  replot();
}

void Pointplot::setPlotTitle(QString title)
{
  setTitle(title);
}

void Pointplot::setPlotXLabel(QString label)
{
  setAxisTitle(QwtPlot::xBottom, label);
}

void Pointplot::setPlotYLabel(QString label)
{
  setAxisTitle(QwtPlot::yLeft, label);
}

void Pointplot::setPlotAxes(double xMin, double xMax,
                 double yMin, double yMax)
{
  setAxisScale(QwtPlot::xBottom, xMin, xMax);
  setAxisScale(QwtPlot::yLeft, yMin, yMax);
}
