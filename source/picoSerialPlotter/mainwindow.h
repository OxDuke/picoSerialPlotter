#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTextBrowser>

namespace Ui {
class MainWindow;
}

class settingDialog;
class plotter;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow *ui;

	class settingDialog* connectionSettingsDialog;
    class plotter* plotterWindow;

	QSerialPort *serialPort;
	QTextBrowser* dataBrowser;

    QAction* actionPlot;
	QAction* actionRun;
	QAction* actionPause;
	QAction* actionStop;

	void setupCentralWidget(); //constructor helper
	void setupMenuBarAndToolBarAndConnection(); //constructor helper
	void setupStatusBar();  //constructor helper


//todo: what is the difference between "private slots" and "slots"
private slots:
	//slots for file menu
	void fileMenuNewTriggered();

	//slots for configure menu
//  void configureMenuShowTriggered();

	//slots for control menu
	void openSerialPort();
	void pauseSerialPort();
	void closeSerialPort();

	//slots for help menu
//  void helpMenuDocumentationTriggered();
//  void helpMenuLicenseTriggered();
	void helpMenuAboutTriggered();

    
	void readData();


};

#endif // MAINWINDOW_H
