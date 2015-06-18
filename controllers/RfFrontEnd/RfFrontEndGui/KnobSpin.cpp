#include "KnobSpin.h"
#include <qwt_math.h>
#include <qpen.h>
#include <qwt_knob.h>
#include <qwt_round_scale_draw.h>
#include <qwt_scale_engine.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qevent.h>

KnobSpin::KnobSpin(const QString &title, double min, double max, double step, QWidget *parent):
    QWidget(parent)
{
  QFont font("Helvetica", 10);

  knob_ = new QwtKnob(this);
  knob_->setFont(font);
  knob_->setRange(min, max);
  QwtScaleDiv scaleDiv =
      knob_->scaleEngine()->divideScale(min, max, 5, 3);
  QList<double> ticks = scaleDiv.ticks(QwtScaleDiv::MajorTick);
  if ( ticks.size() > 0 && ticks[0] > min )
  {
      if ( ticks.first() > min )
          ticks.prepend(min);
      if ( ticks.last() < max )
          ticks.append(max);
  }
  scaleDiv.setTicks(QwtScaleDiv::MajorTick, ticks);
  knob_->setScale(scaleDiv);

  knob_->setKnobWidth(150);
  knob_->setStep(step);

  spin_ = new QDoubleSpinBox(this);
  spin_->setRange(min, max);
  spin_->setSingleStep(step);

  font.setBold(true);
  label_ = new QLabel(title, this);
  label_->setFont(font);
  label_->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

  setSizePolicy(QSizePolicy::MinimumExpanding,
      QSizePolicy::MinimumExpanding);

  connect(spin_, SIGNAL(valueChanged(double)),
          this, SIGNAL(valueChanged(double)));

  connect(knob_, SIGNAL(valueChanged(double)),
          spin_, SLOT(setValue(double)) );
  connect(spin_, SIGNAL(valueChanged(double)),
          knob_, SLOT(setValue(double)) );
}

QSize KnobSpin::sizeHint() const
{
  QSize sz1 = knob_->sizeHint();
  QSize sz2 = spin_->sizeHint();
  QSize sz3 = label_->sizeHint();

  int w = qMax(sz1.width(), sz2.width());
  w = qMax(w, sz3.width());
  const int h = sz1.height() + sz2.height() + sz3.height();

  int off = knob_->scaleDraw()->extent(knob_->font());
  off -= 10; // spacing

  return QSize(w, h - off);
}

void KnobSpin::setValue(double value)
{
  knob_->setValue(value);
  spin_->setValue(value);
}

double KnobSpin::value() const
{
  return knob_->value();
}

void KnobSpin::resizeEvent(QResizeEvent *e)
{
  const QSize sz = e->size();

  int lh = label_->sizeHint().height();
  label_->setGeometry(0, sz.height() - lh,
      sz.width(), lh);

  int sh = spin_->sizeHint().height();
  spin_->setGeometry(0, sz.height() - lh - sh,
      sz.width(), sh);

  int kh = knob_->sizeHint().height();
  int off = knob_->scaleDraw()->extent(knob_->font());
  off -= 10; // spacing
  knob_->setGeometry(0, spin_->pos().y() - kh + off,
      sz.width(), kh);
}
