#include "ScatterplotWrapper.h"

#include "ScatterWidget.h"
#include "common/Events.h"
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
  connect(this, SIGNAL(setWidgetAxes(double,double,double,double)),
                       widget_, SLOT(setWidgetAxes(double,double,double,double)));
  connect(this, SIGNAL(setWidgetTitle(QString)),
          widget_, SLOT(setWidgetTitle(QString)));
  connect(this, SIGNAL(setWidgetAxisLabels(QString, QString)),
          widget_, SLOT(setWidgetAxisLabels(QString, QString)));

  widget_->resize( 800, 600 );
  widget_->setWidgetAxes(-2,2,-2,2);
  widget_->show();
  widget_->moveToThread(QApplication::instance()->thread());
}

void ScatterplotWrapper::destroyWidgetSlot()
{
  delete widget_;
}

void ScatterplotWrapper::plotNewData(complex<double>* data, int numPoints)
{
  if(widget_ == NULL)
    return; //TODO: throw exception here in Iris
  qApp->postEvent(widget_, new ComplexDataEvent(data, numPoints));
}

void ScatterplotWrapper::plotNewData(complex<float>* data, int numPoints)
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

void ScatterplotWrapper::setAxes(double xMin, double xMax,
                        double yMin, double yMax)
{
  if(widget_ == NULL)
    return;
  emit setWidgetAxes(xMin, xMax, yMin, yMax);
}

void ScatterplotWrapper::setAxisLabels(std::string xLabel, std::string yLabel)
{
  if(widget_ == NULL)
    return;
  QString xStr = QString::fromUtf8(xLabel.c_str());
  QString yStr = QString::fromUtf8(yLabel.c_str());
  emit setWidgetAxisLabels(xStr, yStr);
}

