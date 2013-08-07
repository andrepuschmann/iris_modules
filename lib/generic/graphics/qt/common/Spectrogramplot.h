#ifndef SPECTROGRAMPLOT_H
#define SPECTROGRAMPLOT_H

#include <qwt_plot.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include "Spectrogram.h"
#include "SpectrogramData.h"


class SpectrogramData;

class Spectrogramplot: public QwtPlot
{
    Q_OBJECT

public:
    Spectrogramplot(int numDataPoints, int numRows, QWidget * = NULL);
    void plotData(double* data, int n);
    void setPlotAxes(double xMin, double xMax,
                     double yMin, double yMax,
                     double zMin, double zMax);

private:
    QwtPlotZoomer* zoomer_;
    QwtPlotPanner *panner_;
    QwtPlotMagnifier *magnifier_;
    Spectrogram *spectrogram_;
    SpectrogramData* data_;
    int nData_;
    int nRows_;
    double xMin_;
    double xMax_;
    double yMin_;
    double yMax_;
    double zMin_;
    double zMax_;
};

#endif // SPECTROGRAMPLOT_H
