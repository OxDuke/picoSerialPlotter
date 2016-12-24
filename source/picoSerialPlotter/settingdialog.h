#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QWidget>
#include <QDialog>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QString>

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QSpacerItem>


class settingDialog : public QWidget
{
	Q_OBJECT
public:
	explicit settingDialog(QWidget *parent = 0);
#if 1
	struct serialSetting {
		QString name;
		qint32 baudRate;
		QString stringBaudRate;
		QSerialPort::DataBits dataBits;
		QString stringDataBits;
		QSerialPort::Parity parity;
		QString stringParity;
		QSerialPort::StopBits stopBits;
		QString stringStopBits;
		QSerialPort::FlowControl flowControl;
		QString stringFlowControl;
	};

	struct plotSetting
	{
		bool plotSettingChanged;
		int YMax;
		int YMin;
		int XRange;
		double XIncrement;
	};

	serialSetting getSetting();
	plotSetting getPlotSetting();
    void plotSettingReceived();


#endif
private:

	serialSetting currentSetting;
	plotSetting currentPlotSetting;

	//layout pointers
	QComboBox* serialPortInformationListBox;
	QLabel *descriptionLabel;
	QLabel *manufacturerLabel;
	QLabel *serialNumberLabel;
	QLabel *locationLabel;
	QLabel *vendorIDLabel;
	QLabel *productIDLabel;

	QComboBox* baudRateComboBox;
	QComboBox* dataBitsComboBox;
	QComboBox* parityComboBox;
	QComboBox* stopBitsComboBox;
	QComboBox* flowControlComboBox;

	QSpinBox* YMaxSpinBox;
	QSpinBox* YMinSpinBox;
	QSpinBox* XRangeSpinBox;
	QSpinBox* XIncrementSpinBox;

	QPushButton* applyButton;

	//functions
	void drawLayout();

	void fillParameterBox();

	void fillSelectBoxInfoList();

	void updateSetting();

signals:

public slots:

private slots:

	void fillSelectBoxLabels(int index);

	void applySetting();
};

#endif // SETTINGDIALOG_H
