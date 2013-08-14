#include "ScatterplotWrapper.h"

#include "ScatterWidget.h"
#include "graphics/qt/common/Events.h"
#include <qapplication.h>
#include <QThread>
#include <complex>

using namespace std;


ScatterplotWrapper::ScatterplotWrapper()
    :widget_(NULL)
{
  if(QCoreApplication::instance() == NULL)
    return; //TODO: throw exception here in Iris
  if(QCoreApplication::instance()->thread() == QThread::currentThread())
  {
    connect( this, SIGNAL(createWidgetSignal()), this, SLOT(createWidgetSlot()) );
    connect( this, SIGNAL(destroyWidgetSignal()), this, SLOT(destroyWidgetSlot()) );
  }
  else
  {
    connect( this, SIGNAL(createWidgetSignal()), this, SLOT(createWidgetSlot()), Qt::BlockingQueuedConnection );
    connect( this, SIGNAL(destroyWidgetSignal()), this, SLOT(destroyWidgetSlot()), Qt::BlockingQueuedConnection );
    moveToThread(QCoreApplication::instance()->thread());
  }
  emit createWidgetSignal();
}

ScatterplotWrapper::~ScatterplotWrapper()
{
  emit destroyWidgetSignal();
}

void ScatterplotWrapper::createWidgetSlot()
{
  widget_ = new ScatterWidget;
  connect(this, SIGNAL(setWidgetXAxisScale(double,double)),
          widget_, SLOT(setWidgetXAxisScale(double,double)));
  connect(this, SIGNAL(setWidgetYAxisScale(double,double)),
          widget_, SLOT(setWidgetYAxisScale(double,double)));
  connect(this, SIGNAL(setWidgetXAxisAutoScale(bool)),
          widget_, SLOT(setWidgetXAxisAutoScale(bool)));
  connect(this, SIGNAL(setWidgetYAxisAutoScale(bool)),
          widget_, SLOT(setWidgetYAxisAutoScale(bool)));
  connect(this, SIGNAL(setWidgetTitle(QString)),
          widget_, SLOT(setWidgetTitle(QString)));
  connect(this, SIGNAL(setWidgetAxisLabels(QString, QString)),
          widget_, SLOT(setWidgetAxisLabels(QString, QString)));

  widget_->resize( 800, 600 );
  widget_->show();
}

void ScatterplotWrapper::destroyWidgetSlot()
{
  delete widget_;
}

void ScatterplotWrapper::setNewData(complex<double>* data, int numPoints)
{
  if(widget_ == NULL)
    return; //TODO: throw exception here in Iris
  qApp->postEvent(widget_, new ComplexDataEvent(data, numPoints));
}

void ScatterplotWrapper::setNewData(complex<float>* data, int numPoints)
{
  if(widget_ == NULL)
    return; //TODO: throw exception here in Iris
  qApp->postEvent(widget_, new ComplexDataEvent(data, numPoints));
}

void ScatterplotWrapper::setTitle(std::string title)
{
  if(widget_ == NULL)
    return;
  QString str = QString::fromUtf8(title.c_str());
  emit setWidgetTitle(str);
}

void ScatterplotWrapper::setXAxisScale(double xMin, double xMax)
{
  if(widget_ == NULL)
    return;
  emit setWidgetXAxisScale(xMin, xMax);
}

void ScatterplotWrapper::setYAxisScale(double yMin, double yMax)
{
  if(widget_ == NULL)
    return;
  emit setWidgetYAxisScale(yMin, yMax);
}

void ScatterplotWrapper::setXAxisAutoScale(bool on=true)
{
  if(widget_ == NULL)
    return;
  emit setWidgetXAxisAutoScale(on);
}

void ScatterplotWrapper::setYAxisAutoScale(bool on=true)
{
  if(widget_ == NULL)
    return;
  emit setWidgetYAxisAutoScale(on);
}

void ScatterplotWrapper::setAxisLabels(std::string xLabel, std::string yLabel)
{
  if(widget_ == NULL)
    return;
  QString xStr = QString::fromUtf8(xLabel.c_str());
  QString yStr = QString::fromUtf8(yLabel.c_str());
  emit setWidgetAxisLabels(xStr, yStr);
}

