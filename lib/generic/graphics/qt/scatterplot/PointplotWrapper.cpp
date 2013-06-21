#include "PointplotWrapper.h"

#include "Pointplot.h"
#include "PointplotEvents.h"
#include <qapplication.h>
#include <QThread>
#include <complex>


PointplotWrapper::PointplotWrapper()
    :plot_(NULL)
{
  if(QCoreApplication::instance() == NULL)
    return; //TODO: throw exception here in Iris
  if(QCoreApplication::instance()->thread() == QThread::currentThread())
  {
    connect( this, SIGNAL(createPlotSignal()), this, SLOT(createPlotSlot()) );
    connect( this, SIGNAL(destroyPlotSignal()), this, SLOT(destroyPlotSlot()) );
  }
  else
  {
    connect( this, SIGNAL(createPlotSignal()), this, SLOT(createPlotSlot()), Qt::BlockingQueuedConnection );
    connect( this, SIGNAL(destroyPlotSignal()), this, SLOT(destroyPlotSlot()), Qt::BlockingQueuedConnection );
    moveToThread(QCoreApplication::instance()->thread());
  }
  emit createPlotSignal();
}

PointplotWrapper::~PointplotWrapper()
{
  emit destroyPlotSignal();
}

void PointplotWrapper::createPlotSlot()
{
  plot_ = new Pointplot;
  connect(this, SIGNAL(setPlotAxes(double,double,double,double)),
                       plot_, SLOT(setPlotAxes(double,double,double,double)));
  connect(this, SIGNAL(setPlotTitle(QString)),
          plot_, SLOT(setPlotTitle(QString)));
  connect(this, SIGNAL(setPlotXLabel(QString)),
          plot_, SLOT(setPlotXLabel(QString)));
  connect(this, SIGNAL(setPlotYLabel(QString)),
          plot_, SLOT(setPlotYLabel(QString)));

  plot_->resize( 800, 600 );
  plot_->setPlotAxes(-2,2,-2,2);
  plot_->show();
  plot_->moveToThread(QApplication::instance()->thread());
}

void PointplotWrapper::destroyPlotSlot()
{
  delete plot_;
}

void PointplotWrapper::plotNewData(double* real, double* imag, int numPoints)
{
  if(plot_ == NULL)
    return; //TODO: throw exception here in Iris
  qApp->postEvent(plot_, new PlotDataEvent(real, imag, numPoints));
}

void PointplotWrapper::plotNewData(float* real, float* imag, int numPoints)
{
  if(plot_ == NULL)
    return; //TODO: throw exception here in Iris
  qApp->postEvent(plot_, new PlotDataEvent(real, imag, numPoints));
}

void PointplotWrapper::setAxes(double xMin, double xMax,
                        double yMin, double yMax)
{
  if(plot_ == NULL)
    return;
  emit setPlotAxes(xMin, xMax, yMin, yMax);
}

void PointplotWrapper::setTitle(std::string title)
{
  if(plot_ == NULL)
    return;
  QString str = QString::fromUtf8(title.c_str());
  emit setPlotTitle(str);
}

void PointplotWrapper::setXLabel(std::string label)
{
  if(plot_ == NULL)
    return;
  QString str = QString::fromUtf8(label.c_str());
  emit setPlotXLabel(str);
}

void PointplotWrapper::setYLabel(std::string label)
{
  if(plot_ == NULL)
    return;
  QString str = QString::fromUtf8(label.c_str());
  emit setPlotYLabel(str);
}

