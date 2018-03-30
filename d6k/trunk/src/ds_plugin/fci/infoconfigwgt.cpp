﻿#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QFile>
#include <QMessageBox>
#include <QDir>
#include <QApplication>
#include <QGroupBox>
#include <QListWidget>
#include "infoconfigwgt.h"
#include "checkremotewgt.h"


CInfoConfigWgt::CInfoConfigWgt(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle(tr("Program Config"));
	setWindowFlags(Qt::WindowCloseButtonHint);

    m_pLineProjectName = new QLineEdit(this);
	//m_pLinECom = new QLineEdit(this);
	//m_pLineBps = new QLineEdit(this);

	m_pComID = new QComboBox(this);
	m_pComUseType = new QComboBox(this);
	m_pComBps = new QComboBox(this);
	m_pComParity = new QComboBox(this);
	m_pLineEditDataBit = new QLineEdit("8");
	m_pLineEditStopBit = new QLineEdit("0");
	m_pComProtocol = new QComboBox(this);

	m_pLinkAddrLen = new QLineEdit(this);
	m_pCotLen = new QLineEdit(this);
	m_pAsduAddrLen = new QLineEdit(this);
	m_pInfoAddrLen = new QLineEdit(this);
	m_pLinkAddr = new QLineEdit(this);
	m_pAsduAddr = new QLineEdit(this);
	m_pResendTimes = new QLineEdit(this);

	m_pComBoxDebiceType = new QComboBox(this);
    m_pDeviceAddress = new QLineEdit(this);
    m_pDeviceAddress->setText("1");

    m_pBinaryStartAddr = new QLineEdit(this);
    m_pBinaryStartAddr->setText("0");

	m_pDoubleBinaryStartAddr = new QLineEdit(this);
	m_pDoubleBinaryStartAddr->setText("8193");

    m_pAnalyStartAddr = new QLineEdit(this);
    m_pAnalyStartAddr->setText("16385");
    m_pKwhStartAddr = new QLineEdit(this);
    m_pKwhStartAddr->setText("25601");
    m_pControlAddr = new QLineEdit(this);
    m_pControlAddr->setText("0");

    m_pLineEdTotalCall = new QLineEdit(this);
    m_pLineEdTotalCall->setText("10");

    m_pLineEdCheckTimeCall = new QLineEdit(this);
    m_pLineEdCheckTimeCall->setText("10");

    m_pLineEdKwhTimeCall = new QLineEdit(this);
    m_pLineEdKwhTimeCall->setText("15");

    m_pLstProject = new QListWidget(this);

	InitWgt();
	if (!InitCurrentPointTable())
	{
		QMessageBox::warning(this, tr("Warning"), tr("Point Table is not exist!"));
		return;
	}
    //初始化已经存在的工程
    InitProjectList();

    //Slot_UpdateFtpConfig(m_pComBoxDebiceType->currentText());
    //
    connect(m_pLstProject, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(Slot_ClickListItem(QListWidgetItem *)));
	//
	connect(m_pLstProject, SIGNAL(itemDoubleClicked(QListWidgetItem *)),this,SLOT(Slot_lstDoubleClick(QListWidgetItem *)));
}

void  CInfoConfigWgt::InitWgt()
{
    //加载点表bin文件wgt
	QTabWidget *pTabWgt = new QTabWidget(this);

	//组装wgt
    QLabel * pLableProject = new QLabel(tr("ProgName:	"),this);
	QLabel * pLabComID     = new QLabel(tr("COM:		"), this);
	QLabel * pLabUseType   = new QLabel(tr("UseType:	"),this);
	QLabel * pLabBps       = new QLabel(tr("BPS:		"), this);
	QLabel * pLabParity    = new QLabel(tr("Parity:		"), this);
	QLabel * pLabStopBit   = new QLabel(tr("Stop:		"),this);
	QLabel * pLabDataBit   = new QLabel(tr("Data:		"),this);
	QLabel * pLabProtocol  = new QLabel(tr("Protocol:	"), this);

	QLabel *pLableDevice   = new QLabel(tr("Device Type	"), this);
	QLabel *pLabelDeviceAddr = new QLabel(QStringLiteral("设备地址			"), this);

	QHBoxLayout *pProjectlayout = new QHBoxLayout;
    pProjectlayout->addWidget(pLableProject);
    pProjectlayout->addWidget(m_pLineProjectName);
    pProjectlayout->addStretch();
    m_pLineProjectName->setFixedWidth(240);

	QHBoxLayout *pComLayout = new QHBoxLayout;
	pComLayout->addWidget(pLabComID);
	pComLayout->addWidget(m_pComID);
	pComLayout->addStretch();
	m_pComID->setFixedWidth(240);

	QHBoxLayout *pUseTypeLayout = new QHBoxLayout;
	pUseTypeLayout->addWidget(pLabUseType);
	pUseTypeLayout->addWidget(m_pComUseType);
	pUseTypeLayout->addStretch();
	m_pComUseType->setFixedWidth(240);

	QHBoxLayout *pBpsLayout = new QHBoxLayout;
	pBpsLayout->addWidget(pLabBps);
	pBpsLayout->addWidget(m_pComBps);
	pBpsLayout->addStretch();
	m_pComBps->setFixedWidth(240);

	QHBoxLayout *pParityLayout = new QHBoxLayout;
	pParityLayout->addWidget(pLabParity);
	pParityLayout->addWidget(m_pComParity);
	pParityLayout->addStretch();
	m_pComParity->setFixedWidth(240);

	QHBoxLayout *pDataBitLayout = new QHBoxLayout;
	pDataBitLayout->addWidget(pLabDataBit);
	pDataBitLayout->addWidget(m_pLineEditDataBit);
	pDataBitLayout->addStretch();
	m_pLineEditDataBit->setFixedWidth(240);

	QHBoxLayout *pStopBitLayout = new QHBoxLayout;
	pStopBitLayout->addWidget(pLabStopBit);
	pStopBitLayout->addWidget(m_pLineEditStopBit);
	pStopBitLayout->addStretch();
	m_pLineEditStopBit->setFixedWidth(240);

	QHBoxLayout *pProtocolLayout = new QHBoxLayout;
	pProtocolLayout->addWidget(pLabProtocol);
	pProtocolLayout->addWidget(m_pComProtocol);
	pProtocolLayout->addStretch();
	m_pComProtocol->setFixedWidth(240);

	QHBoxLayout * pDeviceLayout = new QHBoxLayout;
	pDeviceLayout->addWidget(pLableDevice);
	pDeviceLayout->addWidget(m_pComBoxDebiceType);
	pDeviceLayout->addStretch();
	m_pComBoxDebiceType->setFixedWidth(240);

	//装置地址
	QHBoxLayout *pDeviceAddrLayout = new QHBoxLayout;
	pDeviceAddrLayout->addWidget(pLabelDeviceAddr);
	pDeviceAddrLayout->addWidget(m_pDeviceAddress);
	pDeviceAddrLayout->addStretch();
	m_pDeviceAddress->setFixedWidth(240);

	QLabel *pLabLinkAddrLen    = new QLabel(QStringLiteral("链路地址长度		"), this);
	QLabel *pLabCotLen         = new QLabel(QStringLiteral("传送原因长度		"), this);
	QLabel *pLabAsduAddrLen    = new QLabel(QStringLiteral("公共信息地址长度		"), this);
	QLabel *pLablInfoAddrLen   = new QLabel(QStringLiteral("信息体地址长度		"), this);
	QLabel *pLabLinkAddr       = new QLabel(QStringLiteral("链路地址			"), this);
	QLabel *pLabAsduAddr       = new QLabel(QStringLiteral("公共信息地址		"), this);
	QLabel *pLabResendTimes    = new QLabel(QStringLiteral("重发次数			"), this);
    //遥信地址
    QLabel *pLableBinary       = new QLabel(QStringLiteral("单点遥信起始地址		"),this);
	//双点遥信地址
	QLabel *pLableDoubleBinary = new QLabel(QStringLiteral("双点遥信起始地址		"), this);
    //要测地址
    QLabel *pLableAnalog       = new QLabel(QStringLiteral("遥测起始地址		"),this);
    //遥脉地址
    QLabel *pLabelKwh          = new QLabel(QStringLiteral("遥脉起始地址		"),this);
    //遥控地址
    QLabel *pLabelControl      = new QLabel(QStringLiteral("遥控起始地址		"),this);
    //总召时间间隔
    QLabel *pLabelGeneralCall  = new QLabel(QStringLiteral("总召时间			"),this);
    //对时间隔
    QLabel *pLabelSyncTime     = new QLabel(QStringLiteral("同步时间			"),this);
    //电度量召唤
    QLabel *pLabelKwhTime      = new QLabel(QStringLiteral("电度时间			"),this);

	QHBoxLayout *pLinkAddrLenLayOut = new QHBoxLayout;
	pLinkAddrLenLayOut->addWidget(pLabLinkAddrLen);
	pLinkAddrLenLayOut->addWidget(m_pLinkAddrLen);
	pLinkAddrLenLayOut->addStretch();
	m_pLinkAddrLen->setFixedWidth(240);

	QHBoxLayout *pCotLenLayOut = new QHBoxLayout;
	pCotLenLayOut->addWidget(pLabCotLen);
	pCotLenLayOut->addWidget(m_pCotLen);
	pCotLenLayOut->addStretch();
	m_pCotLen->setFixedWidth(240);

	QHBoxLayout *pAsduAddrLenLayOut = new QHBoxLayout;
	pAsduAddrLenLayOut->addWidget(pLabAsduAddrLen);
	pAsduAddrLenLayOut->addWidget(m_pAsduAddrLen);
	pAsduAddrLenLayOut->addStretch();
	m_pAsduAddrLen->setFixedWidth(240);

	QHBoxLayout *pInfoAddrLenLayOut = new QHBoxLayout;
	pInfoAddrLenLayOut->addWidget(pLablInfoAddrLen);
	pInfoAddrLenLayOut->addWidget(m_pInfoAddrLen);
	pInfoAddrLenLayOut->addStretch();
	m_pInfoAddrLen->setFixedWidth(240);

	QHBoxLayout *pLinkAddrLayOut = new QHBoxLayout;
	pLinkAddrLayOut->addWidget(pLabLinkAddr);
	pLinkAddrLayOut->addWidget(m_pLinkAddr);
	pLinkAddrLayOut->addStretch();
	m_pLinkAddr->setFixedWidth(240);

	QHBoxLayout *pAsduAddrLayOut = new QHBoxLayout;
	pAsduAddrLayOut->addWidget(pLabAsduAddr);
	pAsduAddrLayOut->addWidget(m_pAsduAddr);
	pAsduAddrLayOut->addStretch();
	m_pAsduAddr->setFixedWidth(240);

	QHBoxLayout *pResendTimesLayOut = new QHBoxLayout;
	pResendTimesLayOut->addWidget(pLabResendTimes);
	pResendTimesLayOut->addWidget(m_pResendTimes);
	pResendTimesLayOut->addStretch();
	m_pResendTimes->setFixedWidth(240);

    QHBoxLayout *pGrop1 = new QHBoxLayout;
    pGrop1->addWidget(pLableBinary);
    pGrop1->addWidget(m_pBinaryStartAddr);
    pGrop1->addStretch();
    m_pBinaryStartAddr->setFixedWidth(240);

	QHBoxLayout *pGrop1_2 = new QHBoxLayout;
	pGrop1_2->addWidget(pLableDoubleBinary);
	pGrop1_2->addWidget(m_pDoubleBinaryStartAddr);
	pGrop1_2->addStretch();
	m_pDoubleBinaryStartAddr->setFixedWidth(240);

    QHBoxLayout *pGrop0 = new QHBoxLayout;
    pGrop0->addWidget(pLableAnalog);
    pGrop0->addWidget(m_pAnalyStartAddr);
    pGrop0->addStretch();
    m_pAnalyStartAddr->setFixedWidth(240);

    QHBoxLayout *pGrop2 = new QHBoxLayout;
    pGrop2->addWidget(pLabelKwh);
    pGrop2->addWidget(m_pKwhStartAddr);
    pGrop2->addStretch();
    m_pKwhStartAddr->setFixedWidth(240);

    QHBoxLayout *pGrop3 = new QHBoxLayout;
    pGrop3->addWidget(pLabelControl);
    pGrop3->addWidget(m_pControlAddr);
    pGrop3->addStretch();
    m_pControlAddr->setFixedWidth(240);

    //总召
    QHBoxLayout *pGrop4 = new QHBoxLayout;
    pGrop4->addWidget(pLabelGeneralCall);
    pGrop4->addWidget(m_pLineEdTotalCall);
    pGrop4->addStretch();
    m_pLineEdTotalCall->setFixedWidth(240);
    //时间同步
    QHBoxLayout *pGrop5 = new QHBoxLayout;
    pGrop5->addWidget(pLabelSyncTime);
    pGrop5->addWidget(m_pLineEdCheckTimeCall);
    pGrop5->addStretch();
    m_pLineEdCheckTimeCall->setFixedWidth(240);
    //遥脉召唤
    
    QHBoxLayout *pGrop6 = new QHBoxLayout;
    pGrop6->addWidget(pLabelKwhTime);
    pGrop6->addWidget(m_pLineEdKwhTimeCall);
    pGrop6->addStretch();
    m_pLineEdKwhTimeCall->setFixedWidth(240);

    //end
	//布局 
	QVBoxLayout *pPage1Layout = new QVBoxLayout;
    pPage1Layout->addLayout(pProjectlayout);
	pPage1Layout->addLayout(pComLayout);
	pPage1Layout->addLayout(pUseTypeLayout);
	pPage1Layout->addLayout(pBpsLayout);
	pPage1Layout->addLayout(pParityLayout);
	pPage1Layout->addLayout(pDataBitLayout);
	pPage1Layout->addLayout(pStopBitLayout);
	pPage1Layout->addLayout(pProtocolLayout);
	pPage1Layout->addLayout(pDeviceLayout);
	
    pPage1Layout->addStretch();


    QWidget *pAddrWgt = new QWidget;
    QVBoxLayout *pAddrlayout = new QVBoxLayout;

	pAddrlayout->addLayout(pLinkAddrLenLayOut);
	pAddrlayout->addLayout(pCotLenLayOut);
	pAddrlayout->addLayout(pAsduAddrLenLayOut);
	pAddrlayout->addLayout(pInfoAddrLenLayOut);
	pAddrlayout->addLayout(pLinkAddrLayOut);
	pAddrlayout->addLayout(pAsduAddrLayOut);
    pAddrlayout->addLayout(pResendTimesLayOut);
	pAddrlayout->addLayout(pDeviceAddrLayout);
	pAddrlayout->addLayout(pGrop1);
	pAddrlayout->addLayout(pGrop1_2);

    pAddrlayout->addLayout(pGrop0);
    pAddrlayout->addLayout(pGrop2);
    pAddrlayout->addLayout(pGrop3);
    pAddrlayout->addLayout(pGrop4);
    pAddrlayout->addLayout(pGrop5);
    pAddrlayout->addLayout(pGrop6);
    pAddrWgt->setLayout(pAddrlayout);

	QWidget *pbaseInfoWgt = new QWidget;
	pbaseInfoWgt->setLayout(pPage1Layout);

	//时间设置
	QLabel *pLabelTime0 = new QLabel(tr("Time0"),this);
	QLabel *pLabelTime1 = new QLabel(tr("Time1"), this);
	QLabel *pLabelTime2 = new QLabel(tr("Time2"), this);
	QLabel *pLabelTime3 = new QLabel(tr("Time3"), this);

	m_pLineEdTime0 = new QLineEdit("5000");
	m_pLineEdTime1 = new QLineEdit("5000");
	m_pLineEdTime2 = new QLineEdit("10000");
	m_pLineEdTime3 = new QLineEdit("20000");

	QHBoxLayout *pLayoutTime0 = new QHBoxLayout;
	pLayoutTime0->addWidget(pLabelTime0);
	pLayoutTime0->addWidget(m_pLineEdTime0);

	QHBoxLayout *pLayoutTime1 = new QHBoxLayout;
	pLayoutTime1->addWidget(pLabelTime1);
	pLayoutTime1->addWidget(m_pLineEdTime1);


	QHBoxLayout *pLayoutTime2 = new QHBoxLayout;
	pLayoutTime2->addWidget(pLabelTime2);
	pLayoutTime2->addWidget(m_pLineEdTime2);


	QHBoxLayout *pLayoutTime3 = new QHBoxLayout;
	pLayoutTime3->addWidget(pLabelTime3);
	pLayoutTime3->addWidget(m_pLineEdTime3);

	QWidget *pTimeWgt = new QWidget;
	QVBoxLayout *pTimetotalLayout = new QVBoxLayout(pTimeWgt);
	pTimetotalLayout->addLayout(pLayoutTime0);
	pTimetotalLayout->addLayout(pLayoutTime1);
	pTimetotalLayout->addLayout(pLayoutTime2);
	pTimetotalLayout->addLayout(pLayoutTime3);

	pTabWgt->addTab(pbaseInfoWgt, tr("Base Info"));
    pTabWgt->addTab(pAddrWgt, tr("Addr Config"));
	pTabWgt->addTab(pTimeWgt,QStringLiteral("时间设置"));


	QPushButton *pConformBtn = new QPushButton(tr("Confrom"), this);
	QPushButton *CancelBtn = new QPushButton(tr("Cancel"), this);

	QHBoxLayout *pBtnLayout = new QHBoxLayout;
	pBtnLayout->addStretch();
	pBtnLayout->addWidget(pConformBtn);
	pBtnLayout->addWidget(CancelBtn);
    //
    QHBoxLayout *pTotalLyout = new QHBoxLayout;
    pTotalLyout->addWidget(m_pLstProject);
    pTotalLyout->addWidget(pTabWgt);

	QVBoxLayout *pDlgLayout = new QVBoxLayout(this);
    pDlgLayout->addLayout(pTotalLyout);
	pDlgLayout->addLayout(pBtnLayout);
    //

	//确定  取消操作
	connect(pConformBtn, SIGNAL(clicked()), this, SLOT(Slot_Comform()));
	connect(CancelBtn, SIGNAL(clicked()), this, SLOT(Slot_Cancel()));
    //更新FTP配置信息
    connect(m_pComBoxDebiceType, SIGNAL(activated(const QString &)), this, SLOT(Slot_UpdateFtpConfig(const QString &)));
}

//加载点表
bool CInfoConfigWgt::InitCurrentPointTable()
{
	//查下点表
	QStringList files;
	QString strRunPath = qApp->applicationDirPath();
	QDir directory;

	directory.setPath(strRunPath + POINTTABLEPATH);
	files = directory.entryList(QDir::Files, QDir::Time);
	if (files.isEmpty())
	{
		return false;
	}
	else
	{
		for (int i = 0; i < files.count(); i++)
		{
			m_pComBoxDebiceType->addItem(files.at(i).split(".").at(0));
		}
	}
	return true;

}

void CInfoConfigWgt::Slot_Comform()
{
    if (m_pLineEdTotalCall->text().toInt() <= 0 || m_pLineEdCheckTimeCall->text().toInt() <= 0 || m_pLineEdKwhTimeCall->text().toInt() <= 0)
    {
        QMessageBox::warning(this, tr("Warning"), tr("General Call Time must greater than zero!"));
        return;
    }

    QString strRunPath = qApp->applicationDirPath();

    WriteXmlFile(strRunPath + PROJECTPATH + m_pLineProjectName->text() + ".prg");

	this->accept();
}

void CInfoConfigWgt::Slot_Cancel()
{
	this->close();
}

//获取IP地址
QString CInfoConfigWgt::GetIpAddress()
{
	return QString();
}

//获取端口号
int CInfoConfigWgt::GetPort()
{
	//return m_pLineBps->text().toInt();
	return 0;
}

//获取点表名称
QString CInfoConfigWgt::GetPointTable()
{
	return m_pComBoxDebiceType->currentText();
}

bool CInfoConfigWgt::InitComFile()
{
	bool bRet = true;

	QString dirPath = qApp->applicationFilePath();
	
	QString strFileName = dirPath + INITFILE + QString("band.txt");
	
	QFile file(strFileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	while (!file.atEnd()) {
		QByteArray line = file.readLine();
		QString str(line);
		m_pComBps->addItem(str);
	}

	file.close();

	strFileName = dirPath + INITFILE + QString("band.txt");

	file.setFileName(strFileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	while (!file.atEnd()) {
		QByteArray line = file.readLine();
		QString str(line);
		m_pComBps->addItem(str);
	}

	file.close();

	strFileName = dirPath + INITFILE + QString("parity.txt");

	file.setFileName(strFileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	while (!file.atEnd()) {
		QByteArray line = file.readLine();
		QString str(line);
		m_pComParity->addItem(str);
	}

	file.close();


	return bRet;
}

CInfoConfigWgt::~CInfoConfigWgt()
{
    
}



void CInfoConfigWgt::Slot_UpdateFtpConfig(const QString &strFilename)
{
    QString strRunPath = qApp->applicationDirPath();
    strRunPath = strRunPath + FTPCONFIGPATH + strFilename + "_trans.xml";
    emit Signal_UpdateFtpConfig(strRunPath);
}

QString CInfoConfigWgt::GetFtpConfigName()
{
    QString strRunPath = qApp->applicationDirPath();
    strRunPath = strRunPath + FTPCONFIGPATH + m_pComBoxDebiceType->currentText() + "_trans.xml";
    return strRunPath;
}

int CInfoConfigWgt::GetBinaryStartAddr()
{
    return m_pBinaryStartAddr->text().toInt();
}

int CInfoConfigWgt::GetDoubleBinaryStartAddr()
{
	return m_pDoubleBinaryStartAddr->text().toInt();
}

int CInfoConfigWgt::GetAnalogStartAddr()
{
    return m_pAnalyStartAddr->text().toInt();
}

int CInfoConfigWgt::GetKwhStartAddr()
{
    return m_pKwhStartAddr->text().toInt();
}

int CInfoConfigWgt::GetControlStart()
{
    return m_pControlAddr->text().toInt();
}

int CInfoConfigWgt::GetDeviceAddr()
{
    return m_pDeviceAddress->text().toInt();
}

QStringList CInfoConfigWgt::GetProjectFile()
{
    QString strRunPath = qApp->applicationDirPath();
    QDir directory;

    directory.setPath(strRunPath + PROJECTPATH);
    if (!directory.exists())
    {
        directory.mkdir(strRunPath + PROJECTPATH);
    }

    return directory.entryList(QDir::Files, QDir::Time);
}

void CInfoConfigWgt::InitProjectList()
{
    m_pLstProject->addItems(GetProjectFile());
}

void CInfoConfigWgt::Slot_ClickListItem(QListWidgetItem *pItem)
{
    //加载数据
    QString strRunPath = qApp->applicationDirPath();
    strRunPath = strRunPath + PROJECTPATH + pItem->text();
    m_pLineProjectName->setText(pItem->text().split(".").at(0));
    OpenXmlFile(strRunPath);
}

void CInfoConfigWgt::Slot_lstDoubleClick(QListWidgetItem *pItem)
{
	Slot_ClickListItem(pItem);
	Slot_Comform();
}

bool CInfoConfigWgt::OpenXmlFile(const QString &fileName)
{
    if (fileName.isEmpty())
    {
        return false;
    }
    if (!fileName.endsWith("prg"))
    {
        return true;
    }

    QFile xmlFile(fileName);

    if (xmlFile.open(QFile::ReadOnly | QFile::Text))
    {
        AnalyseXmlFile(&xmlFile);
        xmlFile.close();
    }
    else
    {
        return false;
    }

    return true;
}

void CInfoConfigWgt::AnalyseXmlFile(QIODevice *pDevice)
{
    QXmlStreamReader xmlReader(pDevice);

    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();

        if (xmlReader.isStartElement())
        {
            if (xmlReader.name().toString() == "ComID")
            {
                m_pComID->setCurrentText(xmlReader.readElementText());
            }
            else if (xmlReader.name().toString() == "UseType")
            {
				m_pComUseType->setCurrentText(xmlReader.readElementText());
            }
			else if (xmlReader.name().toString() == "UseType")
			{
				m_pComUseType->setCurrentText(xmlReader.readElementText());
			}
			else if (xmlReader.name().toString() == "UseType")
			{
				m_pComUseType->setCurrentText(xmlReader.readElementText());
			}
			else if (xmlReader.name().toString() == "UseType")
			{
				m_pComUseType->setCurrentText(xmlReader.readElementText());
			}
			else if (xmlReader.name().toString() == "UseType")
			{
				m_pComUseType->setCurrentText(xmlReader.readElementText());
			}
			else if (xmlReader.name().toString() == "Bps")
			{
				m_pComBps->setCurrentText(xmlReader.readElementText());
			}
			else if (xmlReader.name().toString() == "Parity")
			{
				m_pComParity->setCurrentText(xmlReader.readElementText());
			}
            else if (xmlReader.name().toString() == "StopBit")
            {
				m_pLineEditStopBit->setText(xmlReader.readElementText());
            }
			else if (xmlReader.name().toString() == "DataBit")
			{
				m_pLineEditDataBit->setText(xmlReader.readElementText());
			}
			else if (xmlReader.name().toString() == "Protocol")
			{
				m_pComProtocol->setCurrentText(xmlReader.readElementText());
			}
            else if (xmlReader.name().toString() == "DeviceType")
            {
                m_pComBoxDebiceType->setCurrentText(xmlReader.readElementText());
            }
			else if (xmlReader.name().toString() == "LinkAddrLen")
			{
				m_pLinkAddrLen->setText(xmlReader.readElementText());
			}
			else if (xmlReader.name().toString() == "CotLen")
			{
				m_pCotLen->setText(xmlReader.readElementText());
			}
			else if (xmlReader.name().toString() == "AsduAddrLen")
			{
				m_pAsduAddrLen->setText(xmlReader.readElementText());
			}
			else if (xmlReader.name().toString() == "InfoAddrLen")
			{
				m_pInfoAddrLen->setText(xmlReader.readElementText());
			}
			else if (xmlReader.name().toString() == "LinkAddr")
			{
				m_pLinkAddr->setText(xmlReader.readElementText());
			}
			else if (xmlReader.name().toString() == "AsduAddr")
			{
				m_pAsduAddr->setText(xmlReader.readElementText());
			}
			else if (xmlReader.name().toString() == "ResendTimes")
			{
				m_pResendTimes->setText(xmlReader.readElementText());
			}
			else if (xmlReader.name().toString() == "BinaryAddr")
			{
				m_pBinaryStartAddr->setText(xmlReader.readElementText());
			}
            else if (xmlReader.name().toString() == "DoubleBinaryAddr")
            {
                m_pDoubleBinaryStartAddr->setText(xmlReader.readElementText());
            }
			else if (xmlReader.name().toString() == "AnalogAddr")
			{
				m_pAnalyStartAddr->setText(xmlReader.readElementText());
			}
            else if (xmlReader.name().toString() == "KwhAddr")
            {
                m_pKwhStartAddr->setText(xmlReader.readElementText());
            }
            else if (xmlReader.name().toString() == "GeneralTime")
            {
                m_pLineEdTotalCall->setText(xmlReader.readElementText());
            }
            else if (xmlReader.name().toString() == "SyncTime")
            {
                m_pLineEdCheckTimeCall->setText(xmlReader.readElementText());
            }
            else if (xmlReader.name().toString() == "KwhTime")
            {
                m_pLineEdKwhTimeCall->setText(xmlReader.readElementText());
            }
            else if (xmlReader.name().toString() == "ControlAddr")
            {
                m_pControlAddr->setText(xmlReader.readElementText());
            }
			else if (xmlReader.name().toString() == "time0")
			{
				m_pLineEdTime0->setText(xmlReader.readElementText());
			}
			else if (xmlReader.name().toString() == "time1")
			{
				m_pLineEdTime1->setText(xmlReader.readElementText());
			}
			else if (xmlReader.name().toString() == "time2")
			{
				m_pLineEdTime2->setText(xmlReader.readElementText());
			}
			else if (xmlReader.name().toString() == "time3")
			{
				m_pLineEdTime3->setText(xmlReader.readElementText());
			}
        }
    }
}

void CInfoConfigWgt::AnalyseDownItemData(QXmlStreamReader &xmlReader)
{
   
}

bool CInfoConfigWgt::WriteXmlFile(const QString &strFilename)
{
    QFile xmlFile(strFilename);

    if (xmlFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QXmlStreamWriter writer(&xmlFile);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();
        writer.writeStartElement("conifg");
        //组装数据体
        WriteBaseInfo(writer);
 
        writer.writeEndElement();
        writer.writeEndDocument();
        xmlFile.close();
    }
    else
    {
        return false;
    }

    return true;
}

void CInfoConfigWgt::WriteBaseInfo(QXmlStreamWriter &xmlWriter)
{
    xmlWriter.writeStartElement("ComID");
    xmlWriter.writeCharacters(m_pComID->currentText());
    xmlWriter.writeEndElement();

	xmlWriter.writeStartElement("UseType");
	xmlWriter.writeCharacters(m_pComUseType->currentText());
	xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("Bps");
    xmlWriter.writeCharacters(m_pComBps->currentText());
    xmlWriter.writeEndElement();

	xmlWriter.writeStartElement("Bps");
	xmlWriter.writeCharacters(m_pComBps->currentText());
	xmlWriter.writeEndElement();

	xmlWriter.writeStartElement("Parity");
	xmlWriter.writeCharacters(m_pComParity->currentText());
	xmlWriter.writeEndElement();

	xmlWriter.writeStartElement("StopBit");
	xmlWriter.writeCharacters(m_pLineEditStopBit->text());
	xmlWriter.writeEndElement();

	xmlWriter.writeStartElement("DataBit");
	xmlWriter.writeCharacters(m_pLineEditDataBit->text());
	xmlWriter.writeEndElement();

	xmlWriter.writeStartElement("Protocol");
	xmlWriter.writeCharacters(m_pComProtocol->currentText());
	xmlWriter.writeEndElement();

	xmlWriter.writeStartElement("DeviceType");
	xmlWriter.writeCharacters(m_pComBoxDebiceType->currentText());
	xmlWriter.writeEndElement();

	xmlWriter.writeStartElement("LinkAddrLen");
	xmlWriter.writeCharacters(m_pLinkAddrLen->text());
	xmlWriter.writeEndElement();

	xmlWriter.writeStartElement("CotLen");
	xmlWriter.writeCharacters(m_pCotLen->text());
	xmlWriter.writeEndElement();

	xmlWriter.writeStartElement("AsduAddrLen");
	xmlWriter.writeCharacters(m_pAsduAddrLen->text());
	xmlWriter.writeEndElement();

	xmlWriter.writeStartElement("InfoAddrLen");
	xmlWriter.writeCharacters(m_pInfoAddrLen->text());
	xmlWriter.writeEndElement();

	xmlWriter.writeStartElement("LinkAddr");
	xmlWriter.writeCharacters(m_pLinkAddr->text());
	xmlWriter.writeEndElement();

	xmlWriter.writeStartElement("AsduAddr");
	xmlWriter.writeCharacters(m_pAsduAddr->text());
	xmlWriter.writeEndElement();

	xmlWriter.writeStartElement("ResendTimes");
	xmlWriter.writeCharacters(m_pResendTimes->text());
	xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("DeviceAddr");
    xmlWriter.writeCharacters(m_pDeviceAddress->text());
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("BinaryAddr");
    xmlWriter.writeCharacters(m_pBinaryStartAddr->text());
    xmlWriter.writeEndElement();

	xmlWriter.writeStartElement("DoubleBinaryAddr");
	xmlWriter.writeCharacters(m_pDoubleBinaryStartAddr->text());
	xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("AnalogAddr");
    xmlWriter.writeCharacters(m_pAnalyStartAddr->text());
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("KwhAddr");
    xmlWriter.writeCharacters(m_pKwhStartAddr->text());
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("ControlAddr");
    xmlWriter.writeCharacters(m_pControlAddr->text());
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("GeneralTime");
    xmlWriter.writeCharacters(m_pLineEdTotalCall->text());
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("SyncTime");
    xmlWriter.writeCharacters(m_pLineEdCheckTimeCall->text());
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("KwhTime");
    xmlWriter.writeCharacters(m_pLineEdKwhTimeCall->text());
    xmlWriter.writeEndElement();

	xmlWriter.writeStartElement("time0");
	xmlWriter.writeCharacters(m_pLineEdTime0->text());
	xmlWriter.writeEndElement();

	xmlWriter.writeStartElement("time1");
	xmlWriter.writeCharacters(m_pLineEdTime1->text());
	xmlWriter.writeEndElement();

	xmlWriter.writeStartElement("time2");
	xmlWriter.writeCharacters(m_pLineEdTime2->text());
	xmlWriter.writeEndElement();

	xmlWriter.writeStartElement("time3");
	xmlWriter.writeCharacters(m_pLineEdTime3->text());
	xmlWriter.writeEndElement();
}

void CInfoConfigWgt::WriteFtpInfo(QXmlStreamWriter &xmlWriter)
{
	
}

QString CInfoConfigWgt::GetProjectName()
{
    QString lstItem = m_pLstProject->currentItem()->text();
    lstItem = lstItem.split(".")[0];
    return lstItem;
}

int CInfoConfigWgt::GetGereralTime()
{
    return m_pLineEdTotalCall->text().toInt();
}

int CInfoConfigWgt::GetSyncGenertalTime()
{
    return m_pLineEdCheckTimeCall->text().toInt();
}

int CInfoConfigWgt::GetKwhCallTime()
{
    return m_pLineEdKwhTimeCall->text().toInt();
}
