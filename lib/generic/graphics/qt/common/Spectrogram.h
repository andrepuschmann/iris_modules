#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <qwt_plot_spectrogram.h>

class Spectrogram: public QwtPlotSpectrogram
{
public:
  Spectrogram(const QString &title=QString::null);
  virtual ~Spectrogram();
  virtual QImage renderImage(const QwtScaleMap &xMap,
                             const QwtScaleMap &yMap,
                             const QRectF &area,
                             const QSize &imageSize) const;

private:
  mutable QImage cacheImage_;
};

#endif // SPECTROGRAM_H
