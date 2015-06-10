#include "RfFrontEndGui.h"
#include "RfFrontEndGuiWrapper.h"

using namespace std;

RfFrontEndGui::RfFrontEndGui(FEGuiParams params, iris::Controller* master)
{
  params_ = params;
  master_ = master;
  plot_ = new RfFrontEndGuiWrapper(params_, master_);
}

RfFrontEndGui::~RfFrontEndGui()
{
  if(plot_)
    delete plot_;
}
