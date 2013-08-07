#include "Spectrogram.h"

Spectrogram::Spectrogram(const QString &title)
  :QwtPlotSpectrogram(title)
{

}

Spectrogram::~Spectrogram()
{}

QImage Spectrogram::renderImage(const QwtScaleMap &xMap,
                                const QwtScaleMap &yMap,
                                const QRectF &area,
                                const QSize &imageSize) const
{
  cacheImage_ = QwtPlotSpectrogram::renderImage(xMap, yMap, area, imageSize);
  return cacheImage_;
}

