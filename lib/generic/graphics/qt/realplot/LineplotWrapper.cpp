#include "LineplotWrapper.h"

#include "Lineplot.h"
#include "LineplotEvents.h"
#include <qapplication.h>
#include <QThread>
#include <complex>


LineplotWrapper::LineplotWrapper()
    :plot_(NULL)
{
  if(QCoreApplication::instance() == NULL)
    return; //TODO: throw exception here in Iris
  if(QCoreApplication::instance()->thread() == QThread::currentThread())
  {
    connect( this, SIGNAL( createPlotSignal() ), this, SLOT(createPlotSlot()) );
  }
  else
  {
    connect( this, SIGNAL( createPlotSignal() ), this, SLOT(createPlotSlot()), Qt::BlockingQueuedConnection );
    moveToThread(QCoreApplication::instance()->thread());
  }
  emit createPlotSignal();
}

LineplotWrapper::~LineplotWrapper()
{
  delete plot_;
}

void LineplotWrapper::createPlotSlot()
{
  plot_ = new Lineplot;
  connect(this, SIGNAL(setPlotAxes(double,double,double,double)),
                       plot_, SLOT(setPlotAxes(double,double,double,double)));
  connect(this, SIGNAL(setPlotTitle(QString)),
          plot_, SLOT(setPlotTitle(QString)));
  connect(this, SIGNAL(setPlotXLabel(QString)),
          plot_, SLOT(setPlotXLabel(QString)));
  connect(this, SIGNAL(setPlotYLabel(QString)),
          plot_, SLOT(setPlotYLabel(QString)));

  plot_->resize( 800, 600 );
  plot_->show();
  plot_->moveToThread(QApplication::instance()->thread());
}

void LineplotWrapper::plotNewData(double* data, int numPoints)
{
  if(plot_ == NULL)
    return; //TODO: throw exception here in Iris
  qApp->postEvent(plot_, new PlotLineEvent(data, numPoints));
}

void LineplotWrapper::plotNewData(float* data, int numPoints)
{
  if(plot_ == NULL)
    return; //TODO: throw exception here in Iris
  qApp->postEvent(plot_, new PlotLineEvent(data, numPoints));
}

void LineplotWrapper::setAxes(double xMin, double xMax,
                        double yMin, double yMax)
{
  if(plot_ == NULL)
    return;
  emit setPlotAxes(xMin, xMax, yMin, yMax);
}

void LineplotWrapper::setTitle(std::string title)
{
  if(plot_ == NULL)
    return;
  QString str = QString::fromUtf8(title.c_str());
  emit setPlotTitle(str);
}

void LineplotWrapper::setXLabel(std::string label)
{
  if(plot_ == NULL)
    return;
  QString str = QString::fromUtf8(label.c_str());
  emit setPlotXLabel(str);
}

void LineplotWrapper::setYLabel(std::string label)
{
  if(plot_ == NULL)
    return;
  QString str = QString::fromUtf8(label.c_str());
  emit setPlotYLabel(str);
}

