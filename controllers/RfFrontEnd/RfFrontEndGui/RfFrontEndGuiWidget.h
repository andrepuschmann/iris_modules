#ifndef RFFRONTENDGUIWIDGET_H
#define RFFRONTENDGUIWIDGET_H

#include <qapplication.h>
#include <qwidget.h>

#include "FEGuiParams.h"
#include "FEGuiCallback.h"

class KnobSpin;

class RfFrontEndGuiWidget
  : public QWidget
{
  Q_OBJECT

public:
  RfFrontEndGuiWidget(FEGuiParams params,
                      FEGuiCallback* callback = NULL,
                      QWidget* parent = NULL);
  virtual ~RfFrontEndGuiWidget();

public slots:
  void setFrequency(double frequency);
  void setBandwidth(double bandwidth);
  void setGain(double gain);

protected:

private:
  void updateGradient();
  KnobSpin* knob1_;
  KnobSpin* knob2_;
  KnobSpin* knob3_;
  FEGuiParams params_;
  FEGuiCallback* callback_;
  double frequency_;
  double bandwidth_;
  double gain_;
};

#endif // RFFRONTENDGUIWIDGET_H
