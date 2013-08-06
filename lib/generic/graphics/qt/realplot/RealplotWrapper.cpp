#include "RealplotWrapper.h"

#include "RealWidget.h"
#include "common/Events.h"
#include <qapplication.h>
#include <QThread>


RealplotWrapper::RealplotWrapper()
    :widget_(NULL)
{
  if(QCoreApplication::instance() == NULL)
    return; //TODO: throw exception here in Iris
  if(QCoreApplication::instance()->thread() == QThread::currentThread())
  {
    connect( this, SIGNAL( createWidgetSignal() ), this, SLOT(createWidgetSlot()) );
    connect( this, SIGNAL( destroyWidgetSignal() ), this, SLOT(destroyWidgetSlot()) );
  }
  else
  {
    connect( this, SIGNAL( createWidgetSignal() ), this, SLOT(createWidgetSlot()), Qt::BlockingQueuedConnection );
    connect( this, SIGNAL( destroyWidgetSignal() ), this, SLOT(destroyWidgetSlot()), Qt::BlockingQueuedConnection );
    moveToThread(QCoreApplication::instance()->thread());
  }
  emit createWidgetSignal();
}

RealplotWrapper::~RealplotWrapper()
{
  emit destroyWidgetSignal();
}

void RealplotWrapper::createWidgetSlot()
{
  widget_ = new RealWidget;
  connect(this, SIGNAL(setWidgetTitle(QString)),
          widget_, SLOT(setWidgetTitle(QString)));
  connect(this, SIGNAL(setWidgetAxisLabels(QString, QString)),
          widget_, SLOT(setWidgetAxisLabels(QString, QString)));
  connect(this, SIGNAL(setWidgetAxes(double,double,double,double)),
          widget_, SLOT(setWidgetAxes(double,double,double,double)));
  connect(this, SIGNAL(setWidgetXAxisRange(double,double)),
          widget_, SLOT(setWidgetXAxisRange(double,double)));

  widget_->resize( 800, 600 );
  widget_->show();
  widget_->moveToThread(QApplication::instance()->thread());
}

void RealplotWrapper::destroyWidgetSlot()
{
  delete widget_;
}

void RealplotWrapper::plotNewData(double* data, int numPoints)
{
  if(widget_ == NULL)
    return; //TODO: throw exception here in Iris
  qApp->postEvent(widget_, new RealDataEvent(data, numPoints));
}

void RealplotWrapper::plotNewData(float* data, int numPoints)
{
  if(widget_ == NULL)
    return; //TODO: throw exception here in Iris
  qApp->postEvent(widget_, new RealDataEvent(data, numPoints));
}

void RealplotWrapper::setTitle(std::string title)
{
  if(widget_ == NULL)
    return;
  QString str = QString::fromUtf8(title.c_str());
  emit setWidgetTitle(str);
}

void RealplotWrapper::setAxisLabels(std::string xLabel, std::string yLabel)
{
  if(widget_ == NULL)
    return;
  QString xStr = QString::fromUtf8(xLabel.c_str());
  QString yStr = QString::fromUtf8(yLabel.c_str());
  emit setWidgetAxisLabels(xStr, yStr);
}

void RealplotWrapper::setAxes(double xMin, double xMax,
                        double yMin, double yMax)
{
  if(widget_ == NULL)
    return;
  emit setWidgetAxes(xMin, xMax, yMin, yMax);
}

void RealplotWrapper::setXAxisRange(double xMin, double xMax)
{
  if(widget_ == NULL)
    return;
  emit setWidgetXAxisRange(xMin, xMax);
}
