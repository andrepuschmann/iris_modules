#include <qprinter.h>
#include <qprintdialog.h>
#include <qwt_color_map.h>
#include <qwt_plot_spectrogram.h>
#include <qwt_scale_widget.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_renderer.h>
#include <qwt_matrix_raster_data.h>
#include "Spectrogramplot.h"

class MyZoomer: public QwtPlotZoomer
{
public:
    MyZoomer(QwtPlotCanvas *canvas):
        QwtPlotZoomer(canvas)
    {
        setTrackerMode(AlwaysOn);
    }

    virtual QwtText trackerTextF(const QPointF &pos) const
    {
        QColor bg(Qt::white);
        bg.setAlpha(200);

        QwtText text = QwtPlotZoomer::trackerTextF(pos);
        text.setBackgroundBrush( QBrush( bg ));
        return text;
    }
};

//Set up a colormap to use the "jet" colormap from matlab
class ColorMap
  :public QwtLinearColorMap
{
public:
  ColorMap()
    :QwtLinearColorMap(QColor(0,0,189), QColor(132,0,0))
  {
    double pos;
    pos = 1.0/13.0*1.0; addColorStop(pos, QColor(0,0,255));
    pos = 1.0/13.0*2.0; addColorStop(pos, QColor(0,66,255));
    pos = 1.0/13.0*3.0; addColorStop(pos, QColor(0,132,255));
    pos = 1.0/13.0*4.0; addColorStop(pos, QColor(0,189,255));
    pos = 1.0/13.0*5.0; addColorStop(pos, QColor(0,255,255));
    pos = 1.0/13.0*6.0; addColorStop(pos, QColor(66,255,189));
    pos = 1.0/13.0*7.0; addColorStop(pos, QColor(132,255,132));
    pos = 1.0/13.0*8.0; addColorStop(pos, QColor(189,255,66));
    pos = 1.0/13.0*9.0; addColorStop(pos, QColor(255,255,0));
    pos = 1.0/13.0*10.0; addColorStop(pos, QColor(255,189,0));
    pos = 1.0/13.0*12.0; addColorStop(pos, QColor(255,66,0));
    pos = 1.0/13.0*13.0; addColorStop(pos, QColor(189,0,0));
  }
};

Spectrogramplot::Spectrogramplot(int numDataPoints, int numRows, QWidget *parent)
  :QwtPlot(parent)
  ,nData_(numDataPoints)
  ,nRows_(numRows)
{
    spectrogram_ = new Spectrogram();
    spectrogram_->setRenderThreadCount(1); // set system specific thread count
    data_ = new SpectrogramData(nData_, nRows_);
    spectrogram_->attach(this);
    //setPlotAxes(0, 100, 0, 100, -1.0, 1.0); //PROBLEM WHEN THIS IS REMOVED

    // LeftButton for the zooming
    // MidButton for the panning
    // RightButton: zoom out by 1
    // Ctrl+RighButton: zoom out to full size

    zoomer_ = new MyZoomer(canvas());
    zoomer_->setMousePattern(QwtEventPattern::MouseSelect1,
        Qt::LeftButton);
    zoomer_->setMousePattern(QwtEventPattern::MouseSelect2,
        Qt::LeftButton, Qt::ControlModifier);

    panner_ = new QwtPlotPanner(canvas());
    panner_->setAxisEnabled(QwtPlot::yRight, false);
    panner_->setMouseButton(Qt::RightButton);

    magnifier_ = new QwtPlotMagnifier(canvas());
    magnifier_->setMouseButton(Qt::NoButton);

    // Avoid jumping when labels with more/less digits
    // appear/disappear when scrolling vertically

    const QFontMetrics fm(axisWidget(QwtPlot::yLeft)->font());
    QwtScaleDraw *sd = axisScaleDraw(QwtPlot::yLeft);
    sd->setMinimumExtent( fm.width("100.00") );

    const QColor c(Qt::darkBlue);
    zoomer_->setRubberBandPen(c);
    zoomer_->setTrackerPen(c);
}

void Spectrogramplot::plotData(double* data, int n)
{
  data_->appendData(data, n);
  replot();
}

void Spectrogramplot::setPlotAxes(double xMin, double xMax,
                                  double yMin, double yMax,
                                  double zMin, double zMax)
{
  xMin_ = xMin;
  xMax_ = xMax;
  yMin_ = yMin;
  yMax_ = yMax;
  zMin_ = zMin;
  zMax_ = zMax;

  spectrogram_->setColorMap(new ColorMap());

  data_->setInterval( Qt::XAxis, QwtInterval( xMin_, xMax_ ) );
  data_->setInterval( Qt::YAxis, QwtInterval( yMin_, yMax_ ) );
  data_->setInterval( Qt::ZAxis, QwtInterval( zMin_, zMax_ ) );
  spectrogram_->setData(data_);

  const QwtInterval zInterval = spectrogram_->data()->interval( Qt::ZAxis );
  QwtScaleWidget *rightAxis = axisWidget(QwtPlot::yRight);
  //rightAxis->setTitle("Intensity");
  rightAxis->setColorBarEnabled(true);
  rightAxis->setColorMap( zInterval, new ColorMap());

  setAxisScale(QwtPlot::yRight, zInterval.minValue(), zInterval.maxValue() );
  enableAxis(QwtPlot::yRight);

  plotLayout()->setAlignCanvasToScales(true);
  replot();
}
