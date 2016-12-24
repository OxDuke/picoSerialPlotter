#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QString>
#include <QVector>
#include <QPen>

#include "qcustomplot.h"

namespace Ui {
class plotter;
}

class plotter : public QWidget
{
    Q_OBJECT

public:
    explicit plotter(QWidget *parent = 0);
    ~plotter();

    void updatePlot(QString& dataInCSV);

    void setYMax(int YMax);
    void setYMin(int YMin);
    void setXRange(int XRange);
    void setXIncrement(double XIncrement);

private:
    Ui::plotter *ui;

    void setupCanvas(QCustomPlot *customPlot);
    void setupPen();
    void setupLegend();

    void ingoreBrackets(QStringList &dataList);
    void plotOneByOne(double key, QStringList& dataList);

    int numberOfPens;

    //plot parameters
    int YMin;
    int YMax;
    int XRange;
    double XIncrement;
};

#endif // PLOTTER_H
