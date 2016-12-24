#include "plotter.h"
#include "ui_plotter.h"

#include <QStringList>
#include <QDebug>

plotter::plotter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::plotter)
{
    ui->setupUi(this);

    setupCanvas(ui->customPlot);
    setupPen();
    setupLegend();

    hide();
}

plotter::~plotter()
{
    delete ui;
}

void plotter::setupCanvas(QCustomPlot *customPlot)
{
//  demoName = "Real Time Data Demo";

  // include this section to fully disable antialiasing for higher performance:
/*
  customPlot->setNotAntialiasedElements(QCP::aeAll);
  QFont font;
  font.setStyleStrategy(QFont::NoAntialias);
  customPlot->xAxis->setTickLabelFont(font);
  customPlot->yAxis->setTickLabelFont(font);
  customPlot->legend->setFont(font);
*/
//  customPlot->addGraph(); // blue line
//  customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
//  customPlot->addGraph(); // red line
//  customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));
//  customPlot->addGraph(); // red line
//  customPlot->graph(2)->setPen(QPen(QColor(40, 255, 110)));

//  QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
//  timeTicker->setTimeFormat("%h:%m:%s");
//  customPlot->xAxis->setTicker(timeTicker);
  customPlot->axisRect()->setupFullAxesBox();
  customPlot->yAxis->setRange(-3, 3);
//  customPlot->yAxis->setRange(-yaf, yaf);

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
//  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
//  dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

void plotter::setupPen()
{
    QVector<QPen> penVector;

    penVector.append(QPen(QColor(Qt::red),2));
    penVector.append(QPen(QColor(Qt::green),2));
    penVector.append(QPen(QColor(Qt::blue),2));
    penVector.append(QPen(QColor(Qt::magenta),2));
    penVector.append(QPen(QColor(Qt::yellow),2));
    penVector.append(QPen(QColor(Qt::black),2));
    penVector.append(QPen(QColor(Qt::cyan),2));
//    penVector.append(QPen(QColor(Qt::red),2));
//    penVector.append(QPen(QColor(Qt::red),2));
//    penVector.append(QPen(QColor(Qt::red),2));

//    qDebug()<<tr("pen number")<<penVector.size();

    numberOfPens = penVector.size();

    //add pen to graph
    for(int i=0; i < penVector.size(); ++i)
    {
        ui->customPlot->addGraph();
        ui->customPlot->graph(i)->setPen(penVector.value(i));
    }
}

void plotter::setupLegend()
{
    ui->customPlot->legend->setVisible(true);
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    ui->customPlot->legend->setFont(legendFont);
    ui->customPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
}

void plotter::ingoreBrackets(QStringList& dataList)
{
    for(int i =0; i< dataList.size(); ++i)
    {
        //ignore data in the between brackets [ ]
        if(dataList.value(i).startsWith('[')&&dataList.value(i).endsWith(']'))
        {
            ;
        }

    }
}

void plotter::plotOneByOne(double key, QStringList& dataList)
{
    //numberOfLines are the smallest among...
    int numberOfPlots = numberOfPens<dataList.size()? numberOfPens : dataList.size();

    for(int i =0; i < numberOfPlots; ++i)
    {
        ui->customPlot->graph(i)->addData(key, dataList.value(i).toFloat());
    }

}

void plotter::updatePlot(QString& dataInCSV)
{
   static double timeStamp = 0;

   QStringList dataList = dataInCSV.split(',');

   plotOneByOne(timeStamp, dataList);

   ui->customPlot->xAxis->setRange(timeStamp, XRange, Qt::AlignRight);
   ui->customPlot->yAxis->setRange(YMin,YMax);
   ui->customPlot->replot();

   timeStamp+=XIncrement;
}

void plotter::setYMax(int YMax)
{
    this->YMax = YMax;
}

void plotter::setYMin(int YMin)
{
        this->YMin = YMin;
    }
void plotter::setXRange(int XRange)
{
    this->XRange= XRange;
}

void plotter::setXIncrement(double XIncrement)
{
    this->XIncrement = XIncrement;
}

#if 0

void plotter::updatePlot(QString dataInCSV)
{

  static double key = 0;

  QStringList singleData = dataInCSV.split(',');

//  qDebug()<<dataInCSV;
//  qDebug()<<singleData;
//  qDebug()<<singleData.value(1);

  static double lastPointKey = 0;
  if (1) // at most add point every 2 ms
  {
    // add data to lines:
    ui->customPlot->graph(0)->addData(key, singleData.value(0).toFloat());
    ui->customPlot->graph(1)->addData(key, singleData.value(1).toFloat());
    ui->customPlot->graph(2)->addData(key, singleData.value(2).toFloat());
//    ui->customPlot->graph(0)->addData(key, 1);
//    ui->customPlot->graph(1)->addData(key, -1);

    // rescale value (vertical) axis to fit the current data:
    //ui->customPlot->graph(0)->rescaleValueAxis();
    //ui->customPlot->graph(1)->rescaleValueAxis(true);
//    lastPointKey = key;
    key = key + 0.02;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
//  ui->customPlot->xAxis->setRange(key, 8, Qt::AlignRight);
  ui->customPlot->xAxis->setRange(key, 8, Qt::AlignRight);
  ui->customPlot->yAxis->setRange(-3,3);
  ui->customPlot->replot();

  // calculate frames per second:
//  static double lastFpsKey;
//  static int frameCount;
//  ++frameCount;
//  if (key-lastFpsKey > 2) // average fps over 2 seconds
//  {
//    ui->statusBar->showMessage(
//          QString("%1 FPS, Total Data points: %2")
//          .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
//          .arg(ui->customPlot->graph(0)->data()->size()+ui->customPlot->graph(1)->data()->size())
//          , 0);
//    lastFpsKey = key;
//    frameCount = 0;
//  }
}

#endif
