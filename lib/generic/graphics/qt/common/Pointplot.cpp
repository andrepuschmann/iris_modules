#include "Pointplot.h"
#include <algorithm>

using namespace std;

class MyZoomer: public QwtPlotZoomer
{
public:
    MyZoomer(QwtPlotCanvas *canvas):
        QwtPlotZoomer(canvas)
    {
        setTrackerMode(AlwaysOn);
    }

    virtual QwtText trackerTextF(const QPointF &pos) const
    {
        QColor bg(Qt::white);
        bg.setAlpha(200);

        QwtText text = QwtPlotZoomer::trackerTextF(pos);
        text.setBackgroundBrush( QBrush( bg ));
        return text;
    }
};

Pointplot::Pointplot(QWidget *parent)
  :QwtPlot(parent)
{
  counter_ = 0;
  numPoints_ = 1;
  realPoints_ = new double[numPoints_];
  imagPoints_ = new double[numPoints_];

  QPalette palette;
  palette.setColor(canvas()->backgroundRole(), QColor("white"));
  canvas()->setPalette(palette);

  setAxisScaleEngine(QwtPlot::xBottom, new QwtLinearScaleEngine);
  setAxisTitle(QwtPlot::xBottom, "In-phase");

  setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine);
  setAxisTitle(QwtPlot::yLeft, "Quadrature");

  curve_ = new QwtPlotCurve("Constellation Points");
  curve_->attach(this);
  curve_->setPen(QPen(Qt::blue, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  curve_->setStyle(QwtPlotCurve::Dots);
  curve_->setRawSamples(realPoints_, imagPoints_, numPoints_);

  memset(realPoints_, 0x0, numPoints_*sizeof(double));
  memset(imagPoints_, 0x0, numPoints_*sizeof(double));

  zoomer_ = new MyZoomer(canvas());
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

void Pointplot::plotData(double* iData, double* qData, int n)
{
  if(numPoints_ != n)
  {
    numPoints_ = n;
    delete[] realPoints_;
    delete[] imagPoints_;
    realPoints_ = new double[numPoints_];
    imagPoints_ = new double[numPoints_];
    curve_->setRawSamples(realPoints_, imagPoints_, numPoints_);
  }

  copy(iData, iData+n, realPoints_);
  copy(qData, qData+n, imagPoints_);

  replot();
}

void Pointplot::setTitle(QString title)
{
  setTitle(title);
}

void Pointplot::setXLabel(QString label)
{
  setAxisTitle(QwtPlot::xBottom, label);
}

void Pointplot::setYLabel(QString label)
{
  setAxisTitle(QwtPlot::yLeft, label);
}

void Pointplot::setAxes(double xMin, double xMax,
                        double yMin, double yMax)
{
  setAxisScale(QwtPlot::xBottom, xMin, xMax);
  setAxisScale(QwtPlot::yLeft, yMin, yMax);
}
