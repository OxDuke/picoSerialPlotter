#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QAction>
#include <QLabel>
#include <QMessageBox>
#include <QDialog>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QComboBox>

#include <QByteArray>
#include <QTime>

#include "settingdialog.h"
#include "plotter.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

    connectionSettingsDialog = new settingDialog();  //dialog created for connection settings

    plotterWindow = new plotter();

    serialPort = new QSerialPort();  //instance of QSeriaPort object

	setupCentralWidget(); //central widget setup

	setupMenuBarAndToolBarAndConnection(); //setup peripherals

	this->setWindowIcon(QIcon(":/resource/Icons/picoSerialPlotter.png"));

	setupStatusBar();

}

void MainWindow::setupStatusBar()
{
	ui->statusBar->showMessage(tr("Welcome to picoSerialPlotter!"), 2000);
//	ui->statusBar->addPermanentWidget(new QLabel(tr("picoSerialPlotter V0.41")));
	ui->statusBar->addPermanentWidget(new QLabel(tr("<span style=\"color: #00ff00\">pico</span><span style=\"color: #0000ff\">Serial</span><span style=\"color: #ff0000\">Plotter</span>")));
}

void MainWindow::setupCentralWidget()
{
	dataBrowser = new QTextBrowser();
//    QCustomPlot* plot
	this->setCentralWidget(dataBrowser);
}

void MainWindow::setupMenuBarAndToolBarAndConnection()
{

// ===================set up menubar starts===============================

	QMenu* fileMenu = ui->menuBar->addMenu(tr("File(&F)"));
	QMenu* editMenu = ui->menuBar->addMenu(tr("Edit(&E)"));
	QMenu* configureMenu = ui->menuBar->addMenu(tr("Configuration(&O)"));
	QMenu* controlMenu = ui->menuBar->addMenu(tr("Control(&C)"));
	QMenu* helpMenu = ui->menuBar->addMenu(tr("Help(&H)"));

	//file menu
	QAction* actionNew = fileMenu ->addAction(QIcon(":/resource/Icons/menuBar/File/new.png"), tr("New Configuration"));
	actionNew->setShortcut(QKeySequence("Ctrl+N"));

	QAction* actionOpen = fileMenu ->addAction(QIcon(":/resource/Icons/menuBar/File/open.png"), tr("Open Configuration"));
	actionOpen->setShortcut(QKeySequence("Ctrl+O"));

	QAction* actionSave = fileMenu ->addAction(QIcon(":/resource/Icons/menuBar/File/save.png"), tr("Save Configuration"));
	actionSave->setShortcut(QKeySequence("Ctrl+S"));

	fileMenu ->addAction(QIcon(":/resource/Icons/menuBar/File/save.png"), tr("Save as..."));

	fileMenu -> addSeparator();

	QAction* actionExit = fileMenu ->addAction(QIcon(":/resource/Icons/menuBar/File/exit.png"), tr("Exit"));
	actionExit->setShortcut(QKeySequence("Ctrl+E"));

	//edit menu
	;

	//configure menu
    QAction* actionSetting = configureMenu->addAction(QIcon(":/resource/Icons/menuBar/Configure/setting.png"), tr("Settings"));
    actionSetting->setShortcut(QKeySequence("F4"));

	//control menu
	actionRun = controlMenu ->addAction(QIcon(":/resource/Icons/menuBar/Control/run.png"), tr("Run"));
	actionRun->setShortcut(QKeySequence("F5"));

	actionPause = controlMenu ->addAction(QIcon(":/resource/Icons/menuBar/Control/pause.png"), tr("Pause"));
	actionPause->setShortcut(QKeySequence("F6"));

	actionStop = controlMenu ->addAction(QIcon(":/resource/Icons/menuBar/Control/stop.png"), tr("Stop"));
	actionStop->setShortcut(QKeySequence("F7"));

    actionPlot = controlMenu ->addAction(QIcon(":/resource/Icons/menuBar/Control/plot.png"), tr("Plot"));
    actionPlot->setShortcut(QKeySequence("F8"));

	actionRun->setEnabled(true);
	actionPause->setEnabled(false);
	actionStop->setEnabled(false);

	//help menu
    QAction* actionDocumentation = helpMenu->addAction(tr("Documentation"));

	QAction* actionLicense = helpMenu->addAction(tr("License"));

	QAction* actionAbout = helpMenu->addAction(tr("About"));

// ===================set up menubar ends===============================


// ===================set up tool bar starts===============================

	ui->mainToolBar->addAction(actionOpen);

	ui->mainToolBar->addAction(actionRun);
	ui->mainToolBar->addAction(actionPause);
	ui->mainToolBar->addAction(actionStop);

    ui->mainToolBar->addAction(actionSetting);

    ui->mainToolBar->addAction(actionPlot);

// ===================set up tool bar ends===============================


// ===================set up connections starts===============================

//	connect(actionOpen, SIGNAL(triggered()), this, SLOT(fileNewTriggered()));
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(helpMenuAboutTriggered()));
    connect(actionSetting, SIGNAL(triggered()), connectionSettingsDialog, SLOT(show()));

	connect(actionRun, SIGNAL(triggered(bool)), this, SLOT(openSerialPort()));
	connect(actionPause, SIGNAL(triggered(bool)), this, SLOT(pauseSerialPort()));
	connect(actionStop, SIGNAL(triggered(bool)), this, SLOT(closeSerialPort()));

    connect(actionPlot, SIGNAL(triggered(bool)), plotterWindow, SLOT(show()));

    connect(serialPort, SIGNAL(readyRead()), this, SLOT(readData()));

// ===================set up connections ends===============================

}


MainWindow::~MainWindow()
{
	delete ui;
}

//================all the definition for slots starts here===============

void MainWindow::openSerialPort()
{
	settingDialog::serialSetting p = connectionSettingsDialog->getSetting();
	serialPort->setPortName(p.name);
	serialPort->setBaudRate(p.baudRate);
	serialPort->setDataBits(p.dataBits);
	serialPort->setParity(p.parity);
	serialPort->setStopBits(p.stopBits);
	serialPort->setFlowControl(p.flowControl);
    serialPort->setReadBufferSize(35);
	if (serialPort->open(QIODevice::ReadWrite))
	{
		actionRun->setEnabled(false);
		actionPause->setEnabled(true);
		actionStop->setEnabled(true);

		ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
		                           .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
		                           .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl), 0);
	}
	else
	{
		QMessageBox::critical(this, tr("Error"), serialPort->errorString());
		ui->statusBar->showMessage(tr("Open error"), 0);
	}
}

void MainWindow::pauseSerialPort()
{
	if (serialPort->isOpen())
    {
        serialPort->clear();
		serialPort->close();
    }

	actionRun->setEnabled(true);
	actionPause->setEnabled(false);
	actionStop->setEnabled(true);

	ui->statusBar->showMessage(tr("Paused"), 0);
}

void MainWindow::closeSerialPort()
{
	if (serialPort->isOpen())
    {
        serialPort->clear();
        serialPort->close();
    }

	actionRun->setEnabled(true);
	actionPause->setEnabled(false);
	actionStop->setEnabled(false);

	ui->statusBar->showMessage(tr("Disconnected"), 0);
}

void MainWindow::readData()
{
    static QString string = QString();

    QByteArray data = serialPort->readAll();

    static QTime timer;

    timer.start();

    string.append(data);

    int indexOfCR = string.indexOf('\n');

//    qDebug()<<tr("size:")<<string.size();

    while(indexOfCR != -1) //print all '\n'-terminated strings
    {
        QString temp = string.left(indexOfCR);
        QString& rtemp = temp;

        timer.restart();

        dataBrowser->append(rtemp);

        //plot it
        settingDialog::plotSetting ps = connectionSettingsDialog->getPlotSetting();
        if(ps.plotSettingChanged == true)
        {
            plotterWindow->setYMax(ps.YMax);
            plotterWindow->setYMin(ps.YMin);
            plotterWindow->setXRange(ps.XRange);
            plotterWindow->setXIncrement(ps.XIncrement);
            connectionSettingsDialog->plotSettingReceived();
        }

        plotterWindow->updatePlot(rtemp);//do not append '\n', because the append already add a '\n'

        qDebug()<<tr("plot time: ")<<timer.restart();

        string.remove(0,indexOfCR+1);

        indexOfCR = string.indexOf('\n');
    }

    if(string.size()>200) //prevent string from being too long
        string.clear();

#if 0
    if(string.size()>200) //print strings that are too long
    {
        dataBrowser->append(string);
        string.clear();
    }

#endif
}


//slots for file menu
void MainWindow::fileMenuNewTriggered()
{
//	qDebug() << "pushed" << endl;
    ;
}

void MainWindow::helpMenuAboutTriggered()
{
	QMessageBox aboutBox;

	aboutBox.setWindowTitle(tr("About"));
	aboutBox.setWindowIcon(QIcon(":/resource/Icons/picoSerialPlotter.png"));

	aboutBox.setIconPixmap(QPixmap(":/resource/Icons/menuBar/Help/about.png"));
	aboutBox.setTextFormat(Qt::AutoText);
	aboutBox.setText("<div style=\"font-size: large; \">\
                     <h1 style=\"font-size: xx-large; \"><span style=\"color: #00ff00\">pico</span><span style=\"color: #0000ff\">Serial</span><span style=\"color: #ff0000\">Plotter</span> V0.41</h1>\
                     <p>\
                     <br>\
	                 This software was developed by Weidong Sun for educational and research purposes and was released under GPL.\
	                 </p>\
	                 <p>\
	                 Copyright (C) 2016 Weidong Sun. All right reserved.\
	                 <br>\
	                 <br> Contact information\
	                 <br> -------------------\
	                 <br> Weidong Sun, China\
	                 <br> Website : under construction yet\
	                 <br> e-mail : swdswd28@gmail.com\
	                 <br>\
	                 </p>\
	                 </div>\
	                 ");

	aboutBox.exec();

}

