#ifndef FEGUICALLBACK_H
#define FEGUICALLBACK_H

class FEGuiCallback
{
public:
  virtual void setFrequency(double frequency) = 0;
  virtual void setBandwidth(double bandwidth) = 0;
  virtual void setGain(double gain) = 0;
};

#endif // FEGUICALLBACK_H
