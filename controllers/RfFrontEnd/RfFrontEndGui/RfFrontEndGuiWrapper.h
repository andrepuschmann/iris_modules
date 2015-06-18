#ifndef RFFRONTENDGUIWRAPPER_H
#define RFFRONTENDGUIWRAPPER_H

#include <qapplication.h>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829
#include "irisapi/Controller.h"
#endif

#include "FEGuiParams.h"
#include "FEGuiCallback.h"

class RfFrontEndGuiWidget;

class RfFrontEndGuiWrapper
    : QObject, public FEGuiCallback
{
  Q_OBJECT

public:
  RfFrontEndGuiWrapper(FEGuiParams params, iris::Controller* master=NULL);
  ~RfFrontEndGuiWrapper();

  virtual void setFrequency(double frequency);
  virtual void setBandwidth(double bandwidth);
  virtual void setGain(double gain);

public slots:
  void createWidgetSlot();
  void destroyWidgetSlot();
  void widgetDestroyed();

signals:
  void createWidgetSignal();
  void destroyWidgetSignal();
  void destroyWidgetSignalBlocking();

private:
  RfFrontEndGuiWidget* widget_;
  FEGuiParams params_;
  iris::Controller* master_;
  bool destroyed_;
};

#endif // RFFRONTENDGUIWRAPPER_H
