#include "settingdialog.h"

#include <QDebug>
#include <QMessageBox>

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

settingDialog::settingDialog(QWidget *parent) : QWidget(parent)
{
	drawLayout();

	connect(applyButton, SIGNAL(clicked(bool)), this, SLOT(applySetting()));

	/*====== fill info starts here======*/
	connect(serialPortInformationListBox, SIGNAL(currentIndexChanged(int)), this, SLOT(fillSelectBoxLabels(int)));
	fillParameterBox();
	fillSelectBoxInfoList();

	/*====== fill info ends here======*/

	updateSetting();

	hide();
}

void settingDialog::drawLayout()
{
	//layout construction
	//select box
	QGroupBox *selectBox = new QGroupBox(tr("Select a port"));

	this->serialPortInformationListBox = new QComboBox();

	descriptionLabel = new QLabel(tr("Description:"));
	manufacturerLabel = new QLabel(tr("Manufacturer:"));
	serialNumberLabel = new QLabel(tr("Serial Number:"));
	locationLabel = new QLabel(tr("Location:"));
	vendorIDLabel = new QLabel(tr("Vendor ID:"));
	productIDLabel = new QLabel(tr("Product ID:"));

	QVBoxLayout *vBoxLayout = new QVBoxLayout;
	vBoxLayout->addWidget(serialPortInformationListBox);
	vBoxLayout->addWidget(descriptionLabel);
	vBoxLayout->addWidget(manufacturerLabel);
	vBoxLayout->addWidget(serialNumberLabel);
	vBoxLayout->addWidget(locationLabel);
	vBoxLayout->addWidget(vendorIDLabel);
	vBoxLayout->addWidget(productIDLabel);
	selectBox->setLayout(vBoxLayout);

	//parameter box
	QGroupBox *parameterBox = new QGroupBox(tr("Choose parameters"));

	this->baudRateComboBox = new QComboBox();
	QLabel* baudRateLabel = new QLabel(tr("Baud Rate"));
	this->dataBitsComboBox = new QComboBox();
	QLabel* dataBitsLabel = new QLabel(tr("Data bits"));
	this->parityComboBox = new QComboBox();
	QLabel* parityLabel = new QLabel(tr("Parity"));
	this->stopBitsComboBox = new QComboBox();
	QLabel* stopBitsLabel = new QLabel(tr("Stop bits"));
	this->flowControlComboBox = new QComboBox();
	QLabel* flowControlLabel = new QLabel(tr("Flow control"));

	QGridLayout* gridLayout1 = new QGridLayout;
	gridLayout1->addWidget(baudRateLabel, 0, 0);
	gridLayout1->addWidget(baudRateComboBox, 0, 1);
	gridLayout1->addWidget(dataBitsLabel, 1, 0);
	gridLayout1->addWidget(dataBitsComboBox, 1, 1);
	gridLayout1->addWidget(parityLabel, 2, 0);
	gridLayout1->addWidget(parityComboBox, 2, 1);
	gridLayout1->addWidget(stopBitsLabel, 3, 0);
	gridLayout1->addWidget(stopBitsComboBox, 3, 1);
	gridLayout1->addWidget(flowControlLabel, 4, 0);
	gridLayout1->addWidget(flowControlComboBox, 4, 1);
	parameterBox->setLayout(gridLayout1);

#if 1
	//plot setting box
	QGroupBox *plotSettingBox = new QGroupBox(tr("Plot settings"));

	QLabel* YMaxLabel = new QLabel(tr("Y Axis Max:"));
	YMaxSpinBox = new QSpinBox();
	YMaxSpinBox->setRange(-999, 999);
	YMaxSpinBox->setValue(5);

	QLabel* YMinLabel = new QLabel(tr("Y Axis Min:"));
	YMinSpinBox = new QSpinBox();
	YMinSpinBox->setRange(-999, 999);
	YMinSpinBox->setValue(-5);

	QLabel* XRangeLabel = new QLabel(tr("X Axis Range:"));
	XRangeSpinBox = new QSpinBox();
	XRangeSpinBox->setRange(0, 99);
	XRangeSpinBox->setValue(8);

	QLabel* XIncrementLabel = new QLabel(tr("X Axis Increment *1000:"));
	XIncrementSpinBox = new QSpinBox();
	XIncrementSpinBox->setRange(0, 1000);
	XIncrementSpinBox->setValue(2);

	QGridLayout* gridLayout3 = new QGridLayout();
	gridLayout3->addWidget(YMaxLabel, 0, 0);
	gridLayout3->addWidget(YMaxSpinBox, 0, 1);
	gridLayout3->addWidget(YMinLabel, 1, 0);
	gridLayout3->addWidget(YMinSpinBox, 1, 1);
	gridLayout3->addWidget(XRangeLabel, 2, 0);
	gridLayout3->addWidget(XRangeSpinBox, 2, 1);
	gridLayout3->addWidget(XIncrementLabel, 3, 0);
	gridLayout3->addWidget(XIncrementSpinBox, 3, 1);

	plotSettingBox->setLayout(gridLayout3);
#endif

	//apply button layout
	applyButton = new QPushButton(tr("Apply"));
//    QSpacerItem* spacer = new QSpacerItem();

	QHBoxLayout* applyButtonLayer = new QHBoxLayout;
	applyButtonLayer->addStretch();
	applyButtonLayer->addWidget(applyButton);

	QGridLayout* gridLayout2 = new QGridLayout;

	gridLayout2->addWidget(selectBox, 0, 0);
	gridLayout2->addWidget(parameterBox, 0, 1);
	gridLayout2->addWidget(plotSettingBox, 0, 2);
	gridLayout2->addLayout(applyButtonLayer, 1, 2);
	this->setLayout(gridLayout2);

	this->setWindowIcon(QIcon(":/resource/Icons/menuBar/Configure/setting.png"));
}


void settingDialog::fillParameterBox()
{
	baudRateComboBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
	baudRateComboBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
	baudRateComboBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
	baudRateComboBox->addItem(QStringLiteral("57600"), QSerialPort::Baud57600);
	baudRateComboBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
	baudRateComboBox->setCurrentIndex(4);

	dataBitsComboBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
	dataBitsComboBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
	dataBitsComboBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
	dataBitsComboBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
	dataBitsComboBox->setCurrentIndex(3);

	parityComboBox->addItem(tr("None"), QSerialPort::NoParity);
	parityComboBox->addItem(tr("Even"), QSerialPort::EvenParity);
	parityComboBox->addItem(tr("Odd"), QSerialPort::OddParity);
	parityComboBox->addItem(tr("Mark"), QSerialPort::MarkParity);
	parityComboBox->addItem(tr("Space"), QSerialPort::SpaceParity);

	stopBitsComboBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
	stopBitsComboBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
	stopBitsComboBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

	flowControlComboBox->addItem(tr("None"), QSerialPort::NoFlowControl);
	flowControlComboBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
	flowControlComboBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);

}

void settingDialog::fillSelectBoxInfoList()
{

	this->serialPortInformationListBox->clear();

//	const char blankString[] = QT_TRANSLATE_NOOP("SettingDialog", "N/A");

	QString description;
	QString manufacturer;
	QString serialNumber;
	const auto infos = QSerialPortInfo::availablePorts();
	for (const QSerialPortInfo &info : infos) {
		QStringList list;
		description = info.description();
		manufacturer = info.manufacturer();
		serialNumber = info.serialNumber();
		list << info.portName()
		     << (!description.isEmpty() ? description : blankString)
		     << (!manufacturer.isEmpty() ? manufacturer : blankString)
		     << (!serialNumber.isEmpty() ? serialNumber : blankString)
		     << info.systemLocation()
		     << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
		     << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

		this->serialPortInformationListBox->addItem(list.first(), list);
	}
}

void settingDialog::fillSelectBoxLabels(int index)
{
	if (index == -1)
		return;

	QStringList list = this->serialPortInformationListBox->itemData(index).toStringList();

	descriptionLabel->setText(tr("Description: %1").arg(list.count() > 1 ? list.at(1) : tr(blankString)));
	manufacturerLabel->setText(tr("Manufacturer: %1").arg(list.count() > 2 ? list.at(2) : tr(blankString)));
	serialNumberLabel->setText(tr("Serial number: %1").arg(list.count() > 3 ? list.at(3) : tr(blankString)));
	locationLabel->setText(tr("Location: %1").arg(list.count() > 4 ? list.at(4) : tr(blankString)));
	vendorIDLabel->setText(tr("Vendor Identifier: %1").arg(list.count() > 5 ? list.at(5) : tr(blankString)));
	productIDLabel->setText(tr("Product Identifier: %1").arg(list.count() > 6 ? list.at(6) : tr(blankString)));
}

void settingDialog::applySetting()
{
	updateSetting();
	qDebug() << tr("apply");
//	close();
	hide();
}

void settingDialog::updateSetting()
{
	//port settings
	currentSetting.name = serialPortInformationListBox->currentText();

	currentSetting.baudRate = baudRateComboBox->currentText().toInt();

	currentSetting.stringBaudRate = QString::number(currentSetting.baudRate);

	currentSetting.dataBits = static_cast<QSerialPort::DataBits>(
	                              dataBitsComboBox->itemData(dataBitsComboBox->currentIndex()).toInt());
	currentSetting.stringDataBits = dataBitsComboBox->currentText();

	currentSetting.parity = static_cast<QSerialPort::Parity>(
	                            parityComboBox->itemData(parityComboBox->currentIndex()).toInt());
	currentSetting.stringParity = parityComboBox->currentText();

	currentSetting.stopBits = static_cast<QSerialPort::StopBits>(
	                              stopBitsComboBox->itemData(stopBitsComboBox->currentIndex()).toInt());
	currentSetting.stringStopBits = stopBitsComboBox->currentText();

	currentSetting.flowControl = static_cast<QSerialPort::FlowControl>(
	                                 flowControlComboBox->itemData(flowControlComboBox->currentIndex()).toInt());
	currentSetting.stringFlowControl = flowControlComboBox->currentText();

	//plot settings
	currentPlotSetting.plotSettingChanged = true;

	if (YMaxSpinBox->value() <= YMinSpinBox->value())
	{
        QMessageBox::critical(this, tr("Error"), tr("YMax is smaller than YMin"));
	}
    else
    {
        currentPlotSetting.YMax = YMaxSpinBox->value();

        currentPlotSetting.YMin = YMinSpinBox->value();
    }

	currentPlotSetting.XRange = XRangeSpinBox->value();

    currentPlotSetting.XIncrement = (double)XIncrementSpinBox->value()/100;


}

settingDialog::serialSetting settingDialog::getSetting()
{
	return currentSetting;
}

settingDialog::plotSetting settingDialog::getPlotSetting()
{
    return currentPlotSetting;
}


void settingDialog::plotSettingReceived()
{
    currentPlotSetting.plotSettingChanged =false;
}
