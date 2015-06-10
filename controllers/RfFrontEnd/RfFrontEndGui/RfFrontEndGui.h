#ifndef RFFRONTENDGUI_H
#define RFFRONTENDGUI_H

#include <string>
#include "irisapi/Controller.h"

#include "FEGuiParams.h"

class RfFrontEndGuiWrapper;

class RfFrontEndGui
{
public:
  RfFrontEndGui(FEGuiParams params, iris::Controller* master=NULL);
  ~RfFrontEndGui();

private:
  RfFrontEndGuiWrapper* plot_;
  FEGuiParams params_;
  iris::Controller* master_;
};

#endif // RFFRONTENDGUI_H
