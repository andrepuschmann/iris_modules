#ifndef KNOBSPIN_H
#define KNOBSPIN_H

#include <qwidget.h>

class QwtKnob;
class QDoubleSpinBox;
class QLabel;

class KnobSpin
  : public QWidget
{
    Q_OBJECT

public:
    KnobSpin(const QString &title, double min, double max, double step,
             QWidget *parent = NULL);
    virtual QSize sizeHint() const;
    QwtKnob* knob(){return knob_;}
    QDoubleSpinBox* spin(){return spin_;}

public slots:
    void setValue(double value);
    double value() const;

signals:
    double valueChanged(double);

protected:
    virtual void resizeEvent(QResizeEvent *);

private:
    QwtKnob *knob_;
    QDoubleSpinBox *spin_;
    QLabel *label_;
};

#endif // KNOBSPIN_H
