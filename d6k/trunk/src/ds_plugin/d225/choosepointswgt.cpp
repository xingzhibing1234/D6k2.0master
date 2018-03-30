﻿/*! @file
<PRE>
********************************************************************************
模块名       :	选择需要测的点
文件名       :	choosepointswgt.cpp
文件实现功能 :
作者         :  ww
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief
*  @author ww
*  @version 1.0
*  @date    2016.9.27
*/
#include <QTableWidget>
#include <QHBoxLayout>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QComboBox>
#include <QApplication>
#include <QDir>
#include "choosepointswgt.h"
#include "dragtablewgt.h"
#include "remoteponittableanalyse.h"
#include "pointinfo.h"
#include "analysepointbin.h"
#include "virtualtablewgt.h"
#include "analogdragtablewgt.h"
#include "remoteponittableanalyse.h"
#include "infoconfigwgt.h"

/*! \fn CChoosePointsWgt::CChoosePointsWgt(CPointInfo *pPointInfo, QWidget *parent)
*********************************************************************************************************
** \brief CChoosePointsWgt::CChoosePointsWgt(CPointInfo *pPointInfo, QWidget *parent)
** \details   构造函数
** \param 点信息 parent
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
CChoosePointsWgt::CChoosePointsWgt(CPointInfo *pPointInfo, QString strProjecName, QWidget *parent)
	: QWidget(parent),
	m_pDescTableWgt(new CDragTableWgt(this)),
	m_pSourceTableWgt(new CDragTableWgt(this)),
	m_pDoubleDescTableWgt(new CDragTableWgt(this)),
	m_pDoubleSourceTableWgt(new CDragTableWgt(this)),
	m_pAnalogDescTableWgt(new CAnalogDragTableWgt(this)),
	m_pAnalogSourceTableWgt(new CAnalogDragTableWgt(this)),
	m_pKwhDescTableWgt(new CDragTableWgt(this)),
	m_pKwhSourceTableWgt(new CDragTableWgt(this)),
	m_pControlDescTableWgt(new CDragTableWgt(this)),
	m_pControlSourceTableWgt(new CDragTableWgt(this)),
    m_pVirtualDescTableWgt(new CVirtualTableWgt(this)),
    m_pVirtualSourceTableWgt(new CDragTableWgt(this)),
	m_pTabWgt(new QTabWidget(this)),
    m_pCurrentPoints(new CAnalysePointBin(this)),
	m_pComCooseTable(new QComboBox(this))
{

	m_strProjectName = strProjecName;
	m_pComCooseTable->setFixedWidth(280);

	m_pPointInfo = pPointInfo;
	
	m_pDescTableWgt->SetDescTableWgt();
	m_pSourceTableWgt->SetSourceTableWgt();

	//双点
	m_pDoubleDescTableWgt->SetDescTableWgt();
	m_pDoubleSourceTableWgt->SetSourceTableWgt();

	m_pAnalogDescTableWgt->SetDescTableWgt();
	m_pAnalogSourceTableWgt->SetSourceTableWgt();

	m_pKwhDescTableWgt->SetDescTableWgt();
	m_pKwhSourceTableWgt->SetSourceTableWgt();

	m_pControlDescTableWgt->SetDescTableWgt();
	m_pControlSourceTableWgt->SetSourceTableWgt();

    //虚遥信
    m_pVirtualSourceTableWgt->SetSourceTableWgt();
    m_pVirtualDescTableWgt->SetDescTableWgt();

	//更新数据  信号  槽
	connect(m_pDescTableWgt, SIGNAL(Signal_UpdateTableInfo()), this, SLOT(Slot_UpdateBinaryPointData()));
	connect(m_pDoubleDescTableWgt, SIGNAL(Signal_UpdateTableInfo()), this, SLOT(Slot_UpdateDoubleBinaryPointData()));

	connect(m_pAnalogDescTableWgt, SIGNAL(Signal_UpdateTableInfo()), this, SLOT(Slot_UpdateAnalogPointData()));
	connect(m_pKwhDescTableWgt, SIGNAL(Signal_UpdateTableInfo()), this, SLOT(Slot_UpdateKwhPointData()));
	connect(m_pControlDescTableWgt, SIGNAL(Signal_UpdateTableInfo()), this, SLOT(Slot_UpdateControlPointData()));

	//更新选中状态
	connect(m_pDescTableWgt, SIGNAL(Signal_ExistListID(QList<unsigned int>)), m_pSourceTableWgt, SLOT(Slot_SetSelectedState(QList<unsigned int>)));

	connect(m_pDoubleDescTableWgt, SIGNAL(Signal_ExistListID(QList<unsigned int>)), m_pDoubleSourceTableWgt, SLOT(Slot_SetSelectedState(QList<unsigned int>)));

	connect(m_pAnalogDescTableWgt, SIGNAL(Signal_ExistListID(QList<unsigned int>)), m_pAnalogSourceTableWgt, SLOT(Slot_SetSelectedState(QList<unsigned int>)));
	connect(m_pKwhDescTableWgt, SIGNAL(Signal_ExistListID(QList<unsigned int>)), m_pKwhSourceTableWgt, SLOT(Slot_SetSelectedState(QList<unsigned int>)));
	connect(m_pControlDescTableWgt, SIGNAL(Signal_ExistListID(QList<unsigned int>)), m_pControlSourceTableWgt, SLOT(Slot_SetSelectedState(QList<unsigned int>)));
    //虚遥信
    connect(m_pVirtualDescTableWgt, SIGNAL(Signal_ExistListID(QList<unsigned int>)), m_pVirtualSourceTableWgt, SLOT(Slot_SetSelectedState(QList<unsigned int>)));

	//复原状态
	connect(m_pSourceTableWgt, SIGNAL(Signal_ExistListID(QList<unsigned int>)), m_pDescTableWgt, SLOT(Slot_setRecoverState(QList<unsigned int>)));

	connect(m_pDoubleSourceTableWgt, SIGNAL(Signal_ExistListID(QList<unsigned int>)), m_pDoubleDescTableWgt, SLOT(Slot_setRecoverState(QList<unsigned int>)));

	connect(m_pAnalogSourceTableWgt, SIGNAL(Signal_ExistListID(QList<unsigned int>)), m_pAnalogDescTableWgt, SLOT(Slot_setRecoverState(QList<unsigned int>)));
	connect(m_pKwhSourceTableWgt, SIGNAL(Signal_ExistListID(QList<unsigned int>)), m_pKwhDescTableWgt, SLOT(Slot_setRecoverState(QList<unsigned int>)));
	connect(m_pControlSourceTableWgt, SIGNAL(Signal_ExistListID(QList<unsigned int>)), m_pControlDescTableWgt, SLOT(Slot_setRecoverState(QList<unsigned int>)));
    //虚遥信
    connect(m_pVirtualSourceTableWgt, SIGNAL(Signal_ExistListID(QList<unsigned int>)), m_pVirtualDescTableWgt, SLOT(Slot_setRecoverState(QList<unsigned int>)));

	SetWgtLayout();
	InitConboboxInfo();

	connect(m_pComCooseTable,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(Slot_ChoosePointTable(const QString &)));
}

/*! \fn CChoosePointsWgt::SetWgtLayout()
*********************************************************************************************************
** \brief CChoosePointsWgt::SetWgtLayout()
** \details   设置页面布局
** \param 
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
void CChoosePointsWgt::SetWgtLayout()
{
	//创建wgt布局
	QHBoxLayout *pWgtLayout = new QHBoxLayout(this);
	pWgtLayout->addWidget(m_pTabWgt);
	//创建遥信widget
	QWidget *pBinaryWgt = new QWidget(this);
	QHBoxLayout *pBinayLayout = new QHBoxLayout(pBinaryWgt);

	pBinayLayout->addWidget(m_pDescTableWgt);
	pBinayLayout->addWidget(m_pSourceTableWgt);
	//创建双点遥信
	QWidget *pDoubleBinaryWgt = new QWidget(this);
	QHBoxLayout *pDoubleBinayLayout = new QHBoxLayout(pDoubleBinaryWgt);

	pDoubleBinayLayout->addWidget(m_pDoubleDescTableWgt);
	pDoubleBinayLayout->addWidget(m_pDoubleSourceTableWgt);

	//创建遥测widget
	QWidget *pAnalogWgt = new QWidget(this);
	QHBoxLayout *pAnalogLayout = new QHBoxLayout(pAnalogWgt);

	pAnalogLayout->addWidget(m_pAnalogDescTableWgt);
	pAnalogLayout->addWidget(m_pAnalogSourceTableWgt);

	//创建遥脉widget
	QWidget *pKwhWgt = new QWidget(this);
	QHBoxLayout *pKwhLayout = new QHBoxLayout(pKwhWgt);

	pKwhLayout->addWidget(m_pKwhDescTableWgt);
	pKwhLayout->addWidget(m_pKwhSourceTableWgt);

	//创建遥控widget
	QWidget *pControlWgt = new QWidget(this);
	QHBoxLayout *pControlLayout = new QHBoxLayout(pControlWgt);

	pControlLayout->addWidget(m_pControlDescTableWgt);
	pControlLayout->addWidget(m_pControlSourceTableWgt);

    //创建虚遥信
    QWidget *pVirutalWgt = new QWidget(this);
    QHBoxLayout *pVirutallayout = new QHBoxLayout(pVirutalWgt);

    pVirutallayout->addWidget(m_pVirtualDescTableWgt);
    pVirutallayout->addWidget(m_pVirtualSourceTableWgt);


	m_pTabWgt->addTab(pBinaryWgt, tr("Binary Point"));
	m_pTabWgt->addTab(pDoubleBinaryWgt, QStringLiteral("双点遥信"));
	m_pTabWgt->addTab(pAnalogWgt, tr("Analog Point"));
	m_pTabWgt->addTab(pKwhWgt, tr("Kwh Point"));
	m_pTabWgt->addTab(pControlWgt, tr("Control Point"));
    m_pTabWgt->addTab(pVirutalWgt,tr("Virtual Binary"));
}

/*! \fn InitWgt(CRemotePonitTableAnalyse *pPointAnalyse)
*********************************************************************************************************
** \brief InitWgt(CRemotePonitTableAnalyse *pPointAnalyse)
** \details   初始化点表内容
** \param
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
void CChoosePointsWgt::InitWgt(CRemotePonitTableAnalyse *pPointAnalyse)
{
	//清空数据
	m_pDescTableWgt->ClearAllData();
	m_pSourceTableWgt->ClearAllData();

	m_pDoubleDescTableWgt->ClearAllData();
	m_pDoubleSourceTableWgt->ClearAllData();

	m_pAnalogDescTableWgt->ClearAllData();
	m_pAnalogSourceTableWgt->ClearAllData();
	m_pKwhDescTableWgt->ClearAllData();
	m_pKwhSourceTableWgt->ClearAllData();
	m_pControlDescTableWgt->ClearAllData();
	m_pControlSourceTableWgt->ClearAllData();

    m_pVirtualSourceTableWgt->ClearAllData();
    m_pVirtualDescTableWgt->ClearAllData();

	m_pPointTableAnalyse = pPointAnalyse;

	m_pPointTableAnalyse->SetTableFlag(0);

	if (!InsertDataToTable())
	{
		return;
	}

	Slot_UpdateBinaryPointData();
	Slot_UpdateAnalogPointData();
	Slot_UpdateKwhPointData();
	Slot_UpdateControlPointData();


//     m_pCurrentPoints->InitData("./ini/f220/remotetable/dbBase.bin");
//     Slot_UpdateAllChoosePoints("");

}


/*! \fn Slot_UpdateBinaryPointData()
*********************************************************************************************************
** \brief Slot_UpdateBinaryPointData()
** \details   更新遥信测点
** \param
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
void CChoosePointsWgt::Slot_UpdateBinaryPointData()
{
	Q_ASSERT(m_pPointInfo);
	if (m_pPointInfo == NULL)
	{
		return;
	}

	m_pPointInfo->SetBinaryMap(m_pDescTableWgt->GetCurrentTableData());
}

void CChoosePointsWgt::Slot_UpdateDoubleBinaryPointData()
{
	Q_ASSERT(m_pPointInfo);
	if (m_pPointInfo == NULL)
	{
		return;
	}

	m_pPointInfo->SetDoubleBinaryMap(m_pDoubleDescTableWgt->GetCurrentTableData());

}

/*! \fn Slot_UpdateAnalogPointData()
*********************************************************************************************************
** \brief Slot_UpdateAnalogPointData()
** \details   更新遥测测点
** \param
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
void CChoosePointsWgt::Slot_UpdateAnalogPointData()
{
	Q_ASSERT(m_pPointInfo);
	if (m_pPointInfo == NULL)
	{
		return;
	}

	m_pPointInfo->SetAnalogMap(m_pAnalogDescTableWgt->GetCurrentTableData());

}

/*! \fn Slot_UpdateKwhPointData()
*********************************************************************************************************
** \brief Slot_UpdateKwhPointData()
** \details   更新遥脉测点信息
** \param
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
void CChoosePointsWgt::Slot_UpdateKwhPointData()
{
	Q_ASSERT(m_pPointInfo);
	if (m_pPointInfo == NULL)
	{
		return;
	}

	m_pPointInfo->SetKwhMap(m_pKwhDescTableWgt->GetCurrentTableData());
}

/*! \fn Slot_UpdateControlPointData()
*********************************************************************************************************
** \brief Slot_UpdateControlPointData()
** \details   更新遥控测点信息
** \param
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
void CChoosePointsWgt::Slot_UpdateControlPointData()
{
	Q_ASSERT(m_pPointInfo);
	if (m_pPointInfo == NULL)
	{
		return;
	}

	m_pPointInfo->SetControlMap(m_pControlDescTableWgt->GetCurrentTableData());
}

/*! \fn InsertDataToTable()
*********************************************************************************************************
** \brief InsertDataToTable()
** \details   往表格中插入数据
** \param
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
bool CChoosePointsWgt::InsertDataToTable()
{
// 	if (!m_pPointTableAnalyse->OpenPointTableFile(POINTTABLENAME))
// 	{
// 		return false;
// 	}
	//清空数据
	m_pDescTableWgt->clearContents();
	m_pDescTableWgt->setRowCount(0);

	m_pDoubleDescTableWgt->clearContents();
	m_pDoubleDescTableWgt->setRowCount(0);

	m_pDoubleSourceTableWgt->clearContents();
	m_pDoubleSourceTableWgt->setRowCount(0);

	m_pSourceTableWgt->clearContents();
	m_pSourceTableWgt->setRowCount(0);

    m_pVirtualSourceTableWgt->clearContents();
    m_pVirtualSourceTableWgt->setRowCount(0);

    m_pVirtualDescTableWgt->clearContents();
    m_pVirtualDescTableWgt->setRowCount(0);

	m_pAnalogDescTableWgt->clearContents();
	m_pAnalogDescTableWgt->setRowCount(0);

	m_pAnalogSourceTableWgt->clearContents();
	m_pAnalogSourceTableWgt->setRowCount(0);

	m_pKwhDescTableWgt->clearContents();
	m_pKwhDescTableWgt->setRowCount(0);

	m_pKwhSourceTableWgt->clearContents();
	m_pKwhSourceTableWgt->setRowCount(0);

	m_pControlDescTableWgt->clearContents();
	m_pControlDescTableWgt->setRowCount(0);

	m_pControlSourceTableWgt->clearContents();
	m_pControlSourceTableWgt->setRowCount(0);

	if (!GetBinaryPoints(m_pPointTableAnalyse))
	{
		return false;
	}

	if (!GetDoubleBinaryPoints(m_pPointTableAnalyse))
	{
		return false;
	}

	if (!GetAnalogPoints(m_pPointTableAnalyse))
	{
		return false;
	}

	if (!GetKwhPoints(m_pPointTableAnalyse))
	{
		return false;
	}

	if (!GetControlPoints(m_pPointTableAnalyse))
	{
		return false;
	}

	return true;
}

/*! \fn GetBinaryPoints()
*********************************************************************************************************
** \brief GetBinaryPoints()
** \details   获取遥信点表信息
** \param
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
bool CChoosePointsWgt::GetBinaryPoints(CRemotePonitTableAnalyse *pPointAnalyse)
{
	if (pPointAnalyse == nullptr)
	{
		return false;
	}

	//获取遥信数据
	QList<RPT> lstBinaryRpt = pPointAnalyse->GetBinaryData();

	//填充数据  遥信
	for (int i = 0; i < lstBinaryRpt.count(); i++)
	{
		QTableWidgetItem *item0 = new QTableWidgetItem;
		item0->setText(QString::number(lstBinaryRpt.at(i).NUM2));
		item0->setData(Qt::UserRole + 1, pPointAnalyse->GetTableFlag());

		QTableWidgetItem *item1 = new QTableWidgetItem;
		item1->setText(tr("Binary"));

		QTableWidgetItem *item2 = new QTableWidgetItem;
		item2->setText(lstBinaryRpt.at(i).Destriber);

		QTableWidgetItem *item3 = new QTableWidgetItem;

		m_pSourceTableWgt->insertRow(i);
		m_pSourceTableWgt->setItem(i, 0, item0);
		m_pSourceTableWgt->setItem(i, 1, item1);
		m_pSourceTableWgt->setItem(i, 2, item2);
		m_pSourceTableWgt->setItem(i, 3, item3);

        //虚遥信内容和遥信相同
        QTableWidgetItem *item4 = new QTableWidgetItem;
        item4->setText(QString::number(i + 1));

        QTableWidgetItem *item5 = new QTableWidgetItem;
        item5->setText(tr("Binary"));

        QTableWidgetItem *item6 = new QTableWidgetItem;
        item6->setText(lstBinaryRpt.at(i).Destriber);

        QTableWidgetItem *item7 = new QTableWidgetItem;

        m_pVirtualSourceTableWgt->insertRow(i);
        m_pVirtualSourceTableWgt->setItem(i, 0, item4);
        m_pVirtualSourceTableWgt->setItem(i, 1, item5);
        m_pVirtualSourceTableWgt->setItem(i, 2, item6);
        m_pVirtualSourceTableWgt->setItem(i, 3, item7);
	}

	return true;
}

//双点遥信
bool CChoosePointsWgt::GetDoubleBinaryPoints(CRemotePonitTableAnalyse *pPointAnalyse)
{
	QList<RPT> lstBinaryRpt = pPointAnalyse->GetBinaryData();

	//填充数据  遥信
	for (int i = 0; i < lstBinaryRpt.count(); i++)
	{
		QTableWidgetItem *item0 = new QTableWidgetItem;
		item0->setText(QString::number(lstBinaryRpt.at(i).NUM2));
		item0->setData(Qt::UserRole + 1, pPointAnalyse->GetTableFlag());

		QTableWidgetItem *item1 = new QTableWidgetItem;
		item1->setText(tr("Binary"));

		QTableWidgetItem *item2 = new QTableWidgetItem;
		item2->setText(lstBinaryRpt.at(i).Destriber);

		QTableWidgetItem *item3 = new QTableWidgetItem;

		m_pDoubleSourceTableWgt->insertRow(i);
		m_pDoubleSourceTableWgt->setItem(i, 0, item0);
		m_pDoubleSourceTableWgt->setItem(i, 1, item1);
		m_pDoubleSourceTableWgt->setItem(i, 2, item2);
		m_pDoubleSourceTableWgt->setItem(i, 3, item3);

	}

	return true;
}

/*! \fn GetAnalogPoints()
*********************************************************************************************************
** \brief GetAnalogPoints()
** \details   获取遥测点表信息
** \param
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
bool CChoosePointsWgt::GetAnalogPoints(CRemotePonitTableAnalyse *pPointAnalyse)
{
	//获取遥信数据
	QList<RPT> lstAnalogRpt = pPointAnalyse->GetAnaloyData();

	//填充数据  遥信
	for (int i = 0; i < lstAnalogRpt.count(); i++)
	{
		QTableWidgetItem *item0 = new QTableWidgetItem;
		item0->setText(QString::number(lstAnalogRpt.at(i).NUM2));
		item0->setData(Qt::UserRole + 1, pPointAnalyse->GetTableFlag());

		QTableWidgetItem *item1 = new QTableWidgetItem;
		item1->setText(tr("Analog"));

		QTableWidgetItem *item2 = new QTableWidgetItem;
		item2->setText(lstAnalogRpt.at(i).Destriber);

		QTableWidgetItem *item3 = new QTableWidgetItem;
		item3->setText(lstAnalogRpt.at(i).Unit);

		QTableWidgetItem *item4 = new QTableWidgetItem;
		item4->setText(QString::number(lstAnalogRpt.at(i).fBanalog));

		QTableWidgetItem *item5 = new QTableWidgetItem;
		item5->setText(QString::number(lstAnalogRpt.at(i).fKanalog));

		QTableWidgetItem *item6 = new QTableWidgetItem;
		item6->setText(QString::number(lstAnalogRpt.at(i).fUpperLimit));

		QTableWidgetItem *item7 = new QTableWidgetItem;
		item7->setText(QString::number(lstAnalogRpt.at(i).fDownLimit));

		QTableWidgetItem *item8 = new QTableWidgetItem;
		item8->setText(QString::number(lstAnalogRpt.at(i).fThreshold));

		m_pAnalogSourceTableWgt->insertRow(i);
		m_pAnalogSourceTableWgt->setItem(i, 0, item0);
		m_pAnalogSourceTableWgt->setItem(i, 1, item1);
		m_pAnalogSourceTableWgt->setItem(i, 2, item2);
		m_pAnalogSourceTableWgt->setItem(i, 3, item3);

		m_pAnalogSourceTableWgt->setItem(i, 4, item4);
		m_pAnalogSourceTableWgt->setItem(i, 5, item5);
		m_pAnalogSourceTableWgt->setItem(i, 6, item6);
		m_pAnalogSourceTableWgt->setItem(i, 7, item7);
		m_pAnalogSourceTableWgt->setItem(i, 8, item8);
	}

	return true;
}

/*! \fn GetKwhPoints()
*********************************************************************************************************
** \brief GetKwhPoints()
** \details   获取遥脉点表信息
** \param
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
bool CChoosePointsWgt::GetKwhPoints(CRemotePonitTableAnalyse *pPointAnalyse)
{
	//获取遥信数据
	QList<RPT> lstKwhRpt = pPointAnalyse->GetKwhData();

	//填充数据  遥信
	for (int i = 0; i < lstKwhRpt.count(); i++)
	{
		QTableWidgetItem *item0 = new QTableWidgetItem;
		item0->setText(QString::number(lstKwhRpt.at(i).NUM2));
		item0->setData(Qt::UserRole + 1, pPointAnalyse->GetTableFlag());

		QTableWidgetItem *item1 = new QTableWidgetItem;
		item1->setText(tr("Kwh"));

		QTableWidgetItem *item2 = new QTableWidgetItem;
		item2->setText(lstKwhRpt.at(i).Destriber);

		QTableWidgetItem *item3 = new QTableWidgetItem;
		item3->setText(lstKwhRpt.at(i).Unit);

		m_pKwhSourceTableWgt->insertRow(i);
		m_pKwhSourceTableWgt->setItem(i, 0, item0);
		m_pKwhSourceTableWgt->setItem(i, 1, item1);
		m_pKwhSourceTableWgt->setItem(i, 2, item2);
		m_pKwhSourceTableWgt->setItem(i, 3, item3);
	}

	return true;
}

/*! \fn GetControlPoints()
*********************************************************************************************************
** \brief GetControlPoints()
** \details   获取遥控点表信息
** \param
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
bool CChoosePointsWgt::GetControlPoints(CRemotePonitTableAnalyse *pPointAnalyse)
{
	//获取遥信数据
	QList<RPT> lstControlRpt = pPointAnalyse->GetControlData();

	//填充数据  遥信
	for (int i = 0; i < lstControlRpt.count(); i++)
	{
		QTableWidgetItem *item0 = new QTableWidgetItem;
		item0->setText(QString::number(lstControlRpt.at(i).NUM2));
		item0->setData(Qt::UserRole + 1, pPointAnalyse->GetTableFlag());

		QTableWidgetItem *item1 = new QTableWidgetItem;
		item1->setText(tr("Control"));

		QTableWidgetItem *item2 = new QTableWidgetItem;
		item2->setText(lstControlRpt.at(i).Destriber);

		QTableWidgetItem *item3 = new QTableWidgetItem;

		m_pControlSourceTableWgt->insertRow(i);
		m_pControlSourceTableWgt->setItem(i, 0, item0);
		m_pControlSourceTableWgt->setItem(i, 1, item1);
		m_pControlSourceTableWgt->setItem(i, 2, item2);
		m_pControlSourceTableWgt->setItem(i, 3, item3);
	}

	return true;
}

CChoosePointsWgt::~CChoosePointsWgt()
{

	for (auto item : m_pIdTPointInfo)
	{
		item->deleteLater();
	}
}

/*! \fn Slot_UpdateAllChoosePoints(const QString &strFilename)
*********************************************************************************************************
** \brief Slot_UpdateAllChoosePoints(const QString &strFilename)
** \details   更新所有的点
** \param
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
void CChoosePointsWgt::Slot_UpdateAllChoosePoints(const QString &strFilename)
{
    //m_pCurrentPoints->InitData("./ini/f220/remotetable/dbBase.bin");
    m_pCurrentPoints->InitData(strFilename);

    m_pDescTableWgt->clearContents();
    m_pDescTableWgt->setRowCount(0);

	m_pDoubleDescTableWgt->clearContents();
	m_pDoubleDescTableWgt->setRowCount(0);

    m_pAnalogDescTableWgt->clearContents();
    m_pAnalogDescTableWgt->setRowCount(0);

    m_pKwhDescTableWgt->clearContents();
    m_pKwhDescTableWgt->setRowCount(0);

    m_pControlDescTableWgt->clearContents();
    m_pControlDescTableWgt->setRowCount(0);

    //QMap<unsigned int, QList<int>> maplstInfo = m_pCurrentPoints->GetNeedPoints();
    //QMap<int, QMap<int, RPT> > mapTableInfo = m_pPointTableAnalyse->GetAllPointInfo();

    //顺便遍历所有数据
   // QMap<unsigned int, QList<int>>::const_iterator it = maplstInfo.constBegin();

    //QStringList lstCodes;
    //lstCodes.append(m_pPointTableAnalyse->GetBinaryCodes());
    //lstCodes.append(m_pPointTableAnalyse->GetAnalogCodes());
    //lstCodes.append(m_pPointTableAnalyse->GetKwhCodes());
    //lstCodes.append(m_pPointTableAnalyse->GetControlCodes());

	//遥测
	QMap<int, int> mapIndex;
	for (int i=0; i<m_pCurrentPoints->GetAnalogTableSort().count(); i++)
	{

		if (!mapIndex.contains(m_pCurrentPoints->GetAnalogTableSort()[i]))
		{
			mapIndex[m_pCurrentPoints->GetAnalogTableSort()[i]] = 0;
		}

		//遥测
		CRemotePonitTableAnalyse * tRemote = nullptr;
		if (m_pCurrentPoints->GetAnalogTableSort()[i] == 0)
		{
			tRemote = m_pPointTableAnalyse;
		}
		else
		{
			tRemote = m_pIdTPointInfo[m_pCurrentPoints->GetAnalogTableSort()[i]];
		}

		Q_ASSERT(tRemote);
		if (tRemote == nullptr)
		{
			return;
		}

		if (tRemote->GetAnalogIdGroup().contains(m_pCurrentPoints->GetAnaloglst()[m_pCurrentPoints->GetAnalogTableSort()[i]][mapIndex[m_pCurrentPoints->GetAnalogTableSort()[i]]]))
		{
			//m_pCurrentPoints->GetAnaloglst()[m_pCurrentPoints->GetAnalogTableSort()[i]][i];
			UpdateAnalogToDesTables(tRemote->GetAnalogIdGroup()[m_pCurrentPoints->GetAnaloglst()[m_pCurrentPoints->GetAnalogTableSort()[i]][mapIndex[m_pCurrentPoints->GetAnalogTableSort()[i]]]], m_pAnalogDescTableWgt, m_pCurrentPoints->GetAnalogTableSort()[i]);

			mapIndex[m_pCurrentPoints->GetAnalogTableSort()[i]] = mapIndex[m_pCurrentPoints->GetAnalogTableSort()[i]] + 1;
		}
	}


	//遥信
	QMap<int, int> mapBinaryIndex;
	for (int i = 0; i<m_pCurrentPoints->GetBinaryTableSort().count(); i++)
	{

		if (!mapBinaryIndex.contains(m_pCurrentPoints->GetBinaryTableSort()[i]))
		{
			mapBinaryIndex[m_pCurrentPoints->GetBinaryTableSort()[i]] = 0;
		}

		//遥
		CRemotePonitTableAnalyse * tRemote = nullptr;
		if (m_pCurrentPoints->GetBinaryTableSort()[i] == 0)
		{
			tRemote = m_pPointTableAnalyse;
		}
		else
		{
			tRemote = m_pIdTPointInfo[m_pCurrentPoints->GetBinaryTableSort()[i]];
		}

		Q_ASSERT(tRemote);
		if (tRemote == nullptr)
		{
			return;
		}

		if (tRemote->GetBinaryIdGroup().contains(m_pCurrentPoints->GetBinarylst()[m_pCurrentPoints->GetBinaryTableSort()[i]][mapBinaryIndex[m_pCurrentPoints->GetBinaryTableSort()[i]]]))
		{
			UpdateToDesTables(tRemote->GetBinaryIdGroup()[m_pCurrentPoints->GetBinarylst()[m_pCurrentPoints->GetBinaryTableSort()[i]][mapBinaryIndex[m_pCurrentPoints->GetBinaryTableSort()[i]]]], m_pDescTableWgt, m_pCurrentPoints->GetBinaryTableSort()[i]);

			mapBinaryIndex[m_pCurrentPoints->GetBinaryTableSort()[i]] = mapBinaryIndex[m_pCurrentPoints->GetBinaryTableSort()[i]] + 1;
		}
	}


	//双点遥信
	QMap<int, int> mapDoubleBinaryIndex;
	for (int i = 0; i<m_pCurrentPoints->GetDoubleBinaryTableSort().count(); i++)
	{

		if (!mapDoubleBinaryIndex.contains(m_pCurrentPoints->GetDoubleBinaryTableSort()[i]))
		{
			mapDoubleBinaryIndex[m_pCurrentPoints->GetDoubleBinaryTableSort()[i]] = 0;
		}

		//遥
		CRemotePonitTableAnalyse * tRemote = nullptr;
		if (m_pCurrentPoints->GetDoubleBinaryTableSort()[i] == 0)
		{
			tRemote = m_pPointTableAnalyse;
		}
		else
		{
			tRemote = m_pIdTPointInfo[m_pCurrentPoints->GetDoubleBinaryTableSort()[i]];
		}

		Q_ASSERT(tRemote);
		if (tRemote == nullptr)
		{
			return;
		}

		if (tRemote->GetBinaryIdGroup().contains(m_pCurrentPoints->GetDoubleBinarylst()[m_pCurrentPoints->GetDoubleBinaryTableSort()[i]][mapDoubleBinaryIndex[m_pCurrentPoints->GetDoubleBinaryTableSort()[i]]]))
		{
			UpdateToDesTables(tRemote->GetBinaryIdGroup()[m_pCurrentPoints->GetDoubleBinarylst()[m_pCurrentPoints->GetDoubleBinaryTableSort()[i]][mapDoubleBinaryIndex[m_pCurrentPoints->GetDoubleBinaryTableSort()[i]]]], m_pDoubleDescTableWgt, m_pCurrentPoints->GetDoubleBinaryTableSort()[i]);

			mapDoubleBinaryIndex[m_pCurrentPoints->GetDoubleBinaryTableSort()[i]] = mapDoubleBinaryIndex[m_pCurrentPoints->GetDoubleBinaryTableSort()[i]] + 1;
		}
	}

	//遥脉
	QMap<int, int> mapKwhIndex;
	for (int i = 0; i<m_pCurrentPoints->GetKwhTableSort().count(); i++)
	{

		if (!mapKwhIndex.contains(m_pCurrentPoints->GetKwhTableSort()[i]))
		{
			mapKwhIndex[m_pCurrentPoints->GetKwhTableSort()[i]] = 0;
		}

		//遥
		CRemotePonitTableAnalyse * tRemote = nullptr;
		if (m_pCurrentPoints->GetKwhTableSort()[i] == 0)
		{
			tRemote = m_pPointTableAnalyse;
		}
		else
		{
			tRemote = m_pIdTPointInfo[m_pCurrentPoints->GetKwhTableSort()[i]];
		}

		Q_ASSERT(tRemote);
		if (tRemote == nullptr)
		{
			return;
		}

		if (tRemote->GetKwhIdGroup().contains(m_pCurrentPoints->GetKwhlst()[m_pCurrentPoints->GetKwhTableSort()[i]][mapKwhIndex[m_pCurrentPoints->GetKwhTableSort()[i]]]))
		{
			UpdateToDesTables(tRemote->GetKwhIdGroup()[m_pCurrentPoints->GetKwhlst()[m_pCurrentPoints->GetKwhTableSort()[i]][mapKwhIndex[m_pCurrentPoints->GetKwhTableSort()[i]]]], m_pKwhDescTableWgt, m_pCurrentPoints->GetKwhTableSort()[i]);

			mapKwhIndex[m_pCurrentPoints->GetKwhTableSort()[i]] = mapKwhIndex[m_pCurrentPoints->GetKwhTableSort()[i]] + 1;
		}
	}

	//遥控
	QMap<int, int> mapControlIndex;
	for (int i = 0; i<m_pCurrentPoints->GetControlTableSort().count(); i++)
	{

		if (!mapControlIndex.contains(m_pCurrentPoints->GetControlTableSort()[i]))
		{
			mapControlIndex[m_pCurrentPoints->GetControlTableSort()[i]] = 0;
		}

		//遥
		CRemotePonitTableAnalyse * tRemote = nullptr;
		if (m_pCurrentPoints->GetControlTableSort()[i] == 0)
		{
			tRemote = m_pPointTableAnalyse;
		}
		else
		{
			tRemote = m_pIdTPointInfo[m_pCurrentPoints->GetControlTableSort()[i]];
		}

		Q_ASSERT(tRemote);
		if (tRemote == nullptr)
		{
			return;
		}

		if (tRemote->GetControlIdGroup().contains(m_pCurrentPoints->GetControllst()[m_pCurrentPoints->GetControlTableSort()[i]][mapControlIndex[m_pCurrentPoints->GetControlTableSort()[i]]]))
		{
			UpdateToDesTables(tRemote->GetControlIdGroup()[m_pCurrentPoints->GetControllst()[m_pCurrentPoints->GetControlTableSort()[i]][mapControlIndex[m_pCurrentPoints->GetControlTableSort()[i]]]], m_pControlDescTableWgt, m_pCurrentPoints->GetControlTableSort()[i]);

			mapControlIndex[m_pCurrentPoints->GetControlTableSort()[i]] = mapControlIndex[m_pCurrentPoints->GetControlTableSort()[i]] + 1;
		}
	}

	/*
	for (int i = 0; i < m_pCurrentPoints->GetAnaloglst().count(); i++)
	{
		//遥测
		if (m_pPointTableAnalyse->GetAnalogIdGroup().contains(m_pCurrentPoints->GetAnaloglst()[i]))
		{
			UpdateAnalogToDesTables(m_pPointTableAnalyse->GetAnalogIdGroup()[m_pCurrentPoints->GetAnaloglst()[i]], m_pAnalogDescTableWgt);
		}
	}
	*/

	/*
	for (int i = 0; i < m_pCurrentPoints->GetBinarylst().count(); i++)
	{
		//遥信
		if (m_pPointTableAnalyse->GetBinaryIdGroup().contains(m_pCurrentPoints->GetBinarylst()[i]))
		{
			UpdateToDesTables(m_pPointTableAnalyse->GetBinaryIdGroup()[m_pCurrentPoints->GetBinarylst()[i]], m_pDescTableWgt);
		}
	}

	//双点遥信
	for (int i = 0; i < m_pCurrentPoints->GetDoubleBinarylst().count(); i++)
	{
		//
		if (m_pPointTableAnalyse->GetBinaryIdGroup().contains(m_pCurrentPoints->GetDoubleBinarylst()[i]))
		{
			UpdateToDesTables(m_pPointTableAnalyse->GetBinaryIdGroup()[m_pCurrentPoints->GetDoubleBinarylst()[i]], m_pDoubleDescTableWgt);
		}
	}

	for (int i = 0; i < m_pCurrentPoints->GetKwhlst().count(); i++)
	{
		//遥脉
		if (m_pPointTableAnalyse->GetKwhIdGroup().contains(m_pCurrentPoints->GetKwhlst()[i]))
		{
			UpdateToDesTables(m_pPointTableAnalyse->GetKwhIdGroup()[m_pCurrentPoints->GetKwhlst()[i]], m_pKwhDescTableWgt);
		}
	}

	for (int i = 0; i < m_pCurrentPoints->GetControllst().count(); i++)
	{
		//遥控
		if (m_pPointTableAnalyse->GetControlIdGroup().contains(m_pCurrentPoints->GetControllst()[i]))
		{
			UpdateToDesTables(m_pPointTableAnalyse->GetControlIdGroup()[m_pCurrentPoints->GetControllst()[i]], m_pControlDescTableWgt);
		}
	}

	*/
	/*
    for (int i = 0; i < m_pCurrentPoints->GetBtData().count(); i = i + 2)
    {
        unsigned int tCodeType = (unsigned char)m_pCurrentPoints->GetBtData().at(i + 1);
        unsigned int tDeviceNum = (unsigned char)m_pCurrentPoints->GetBtData().at(i);

        if (lstCodes.contains(QString::number(tCodeType)))
        {
            if (m_pPointTableAnalyse->GetBinaryCodes().contains((QString::number(tCodeType))))
            {
                //遥信
				if (mapTableInfo.contains(tCodeType) && mapTableInfo[tCodeType].contains(tDeviceNum))
				{
					UpdateToDesTables(mapTableInfo[tCodeType][tDeviceNum], m_pDescTableWgt);
				}
                
            }
            else if (m_pPointTableAnalyse->GetAnalogCodes().contains((QString::number(tCodeType))))
            {
                //遥测
				if (mapTableInfo.contains(tCodeType) && mapTableInfo[tCodeType].contains(tDeviceNum))
				{
					UpdateToDesTables(mapTableInfo[tCodeType][tDeviceNum], m_pAnalogDescTableWgt);
				}
                
            }
            else if (m_pPointTableAnalyse->GetKwhCodes().contains((QString::number(tCodeType))))
            {
                //遥脉
				if (mapTableInfo.contains(tCodeType) && mapTableInfo[tCodeType].contains(tDeviceNum))
				{
					UpdateToDesTables(mapTableInfo[tCodeType][tDeviceNum], m_pKwhDescTableWgt);
				}
                
            }
            else if (m_pPointTableAnalyse->GetControlCodes().contains((QString::number(tCodeType))))
            {
                //遥控
				if (mapTableInfo.contains(tCodeType) && mapTableInfo[tCodeType].contains(tDeviceNum))
				{
					UpdateToDesTables(mapTableInfo[tCodeType][tDeviceNum], m_pControlDescTableWgt);
				}
                
            }

            //mapTableInfo[(unsigned char)m_pCurrentPoints->GetBtData().at(i + 1)][(unsigned char)m_pCurrentPoints->GetBtData().at(i)];
        }
    }
	*/
//     for (int i = 0; i < lstCodes.count(); i++)
//     {
//         if (maplstInfo.contains(lstCodes.at(i).toInt()))
//         {
//             for (int j = 0; j < maplstInfo[lstCodes.at(i).toInt()].count(); j++)
//             {
// 
//                 if (m_pPointTableAnalyse->GetBinaryCodes().contains(lstCodes.at(i)))
//                 {
//                     //遥信
//                     UpdateToDesTables(mapTableInfo[lstCodes.at(i).toInt()][maplstInfo[lstCodes.at(i).toInt()].at(j)], m_pDescTableWgt);
//                 }
//                 else if (m_pPointTableAnalyse->GetAnalogCodes().contains(lstCodes.at(i)))
//                 {
//                     //遥测
//                     UpdateToDesTables(mapTableInfo[lstCodes.at(i).toInt()][maplstInfo[lstCodes.at(i).toInt()].at(j)], m_pAnalogDescTableWgt);
//                 }
//                 else if (m_pPointTableAnalyse->GetKwhCodes().contains(lstCodes.at(i)))
//                 {
//                     //遥脉
//                     UpdateToDesTables(mapTableInfo[lstCodes.at(i).toInt()][maplstInfo[lstCodes.at(i).toInt()].at(j)], m_pKwhDescTableWgt);
//                 }
//                 if (m_pPointTableAnalyse->GetControlCodes().contains(lstCodes.at(i)))
//                 {
//                     //遥控
//                     UpdateToDesTables(mapTableInfo[lstCodes.at(i).toInt()][maplstInfo[lstCodes.at(i).toInt()].at(j)], m_pControlDescTableWgt);
//                 }
//             }
//         }
// 
//     }
    
//     while (it != maplstInfo.constEnd())
//     {
//         for (int i = 0; i < it.value().count(); i++)
//         {
//             if (mapTableInfo.contains(it.key()) && mapTableInfo[it.key()].contains(it.value().at(i)))
//             {
//                 //存在类型  并且存在点
//                 //开始写入
//                 //确定往哪个表里面写
//                 if (m_pPointTableAnalyse->GetBinaryCodes().contains(QString::number(it.key())))
//                 {
//                     //遥信
//                     UpdateToDesTables(mapTableInfo[it.key()][it.value().at(i)], m_pDescTableWgt);
//                 }
//                 else if (m_pPointTableAnalyse->GetAnalogCodes().contains(QString::number(it.key())))
//                 {
//                     //遥测
//                     UpdateToDesTables(mapTableInfo[it.key()][it.value().at(i)], m_pAnalogDescTableWgt);
//                 }
//                 else if (m_pPointTableAnalyse->GetKwhCodes().contains(QString::number(it.key())))
//                 {
//                     //遥脉
//                     UpdateToDesTables(mapTableInfo[it.key()][it.value().at(i)], m_pKwhDescTableWgt);
//                 }
//                 if (m_pPointTableAnalyse->GetControlCodes().contains(QString::number(it.key())))
//                 {
//                     //遥控
//                     UpdateToDesTables(mapTableInfo[it.key()][it.value().at(i)], m_pControlDescTableWgt);
//                 }
//             }
//         }
//         
//         it++;
//     }

    Slot_UpdateBinaryPointData();
	//双点遥信
	Slot_UpdateDoubleBinaryPointData();
    Slot_UpdateAnalogPointData();
    Slot_UpdateKwhPointData();
    Slot_UpdateControlPointData();
    
}

void CChoosePointsWgt::Slot_ChoosePointTable(const QString &itemName)
{
	//加载点表
	//加载点表数据
	/*
	QString strRunPath = qApp->applicationDirPath();

	if (!m_pPointTableAnalyse->OpenPointTableFile(strRunPath + POINTTABLEPATH + itemName + ".txt"))
	{
		return;
	}
	*/

	//InitWgt(m_pPointTableAnalyse);

	if (m_strProjectName == itemName)
	{
		m_pDoubleSourceTableWgt->clearContents();
		m_pDoubleSourceTableWgt->setRowCount(0);

		m_pSourceTableWgt->clearContents();
		m_pSourceTableWgt->setRowCount(0);

		m_pVirtualSourceTableWgt->clearContents();
		m_pVirtualSourceTableWgt->setRowCount(0);

		m_pAnalogSourceTableWgt->clearContents();
		m_pAnalogSourceTableWgt->setRowCount(0);

		m_pKwhSourceTableWgt->clearContents();
		m_pKwhSourceTableWgt->setRowCount(0);

		m_pControlSourceTableWgt->clearContents();
		m_pControlSourceTableWgt->setRowCount(0);

		if (!GetBinaryPoints(m_pPointTableAnalyse))
		{
			return;
		}

		if (!GetDoubleBinaryPoints(m_pPointTableAnalyse))
		{
			return;
		}

		if (!GetAnalogPoints(m_pPointTableAnalyse))
		{
			return;
		}

		if (!GetKwhPoints(m_pPointTableAnalyse))
		{
			return;
		}

		if (!GetControlPoints(m_pPointTableAnalyse))
		{
			return;
		}
		return;
	}

	QString strItem = itemName;
	int nNumber = strItem.remove("DEV_").toInt();

	if (!m_pIdTPointInfo.contains(nNumber))
	{
		QMessageBox::warning(this, QStringLiteral("告警"), QStringLiteral("%1点表不存在").arg(itemName));
		return;
	}


	m_pDoubleSourceTableWgt->clearContents();
	m_pDoubleSourceTableWgt->setRowCount(0);

	m_pSourceTableWgt->clearContents();
	m_pSourceTableWgt->setRowCount(0);

	m_pVirtualSourceTableWgt->clearContents();
	m_pVirtualSourceTableWgt->setRowCount(0);

	m_pAnalogSourceTableWgt->clearContents();
	m_pAnalogSourceTableWgt->setRowCount(0);

	m_pKwhSourceTableWgt->clearContents();
	m_pKwhSourceTableWgt->setRowCount(0);

	m_pControlSourceTableWgt->clearContents();
	m_pControlSourceTableWgt->setRowCount(0);

	if (!GetBinaryPoints(m_pIdTPointInfo[nNumber]))
	{
		return;
	}

	if (!GetDoubleBinaryPoints(m_pIdTPointInfo[nNumber]))
	{
		return;
	}

	if (!GetAnalogPoints(m_pIdTPointInfo[nNumber]))
	{
		return;
	}

	if (!GetKwhPoints(m_pIdTPointInfo[nNumber]))
	{
		return;
	}

	if (!GetControlPoints(m_pIdTPointInfo[nNumber]))
	{
		return;
	}
}

void CChoosePointsWgt::resizeEvent(QResizeEvent * event)
{
	m_pComCooseTable->move(this->width()-300,10);
	QWidget::resizeEvent(event);
}

/*! \fn UpdateToDesTables(RPT rptInfo, CDragTableWgt *pDesTableWgt)
*********************************************************************************************************
** \brief UpdateToDesTables(RPT rptInfo, CDragTableWgt *pDesTableWgt)
** \details   更新表中的值
** \param
** \return
** \author ww
** \date 2016年09月27日
** \note
********************************************************************************************************/
void CChoosePointsWgt::UpdateToDesTables(RPT rptInfo, CDragTableWgt *pDesTableWgt, int nPointTableIndex)
{
    if (pDesTableWgt == nullptr)
    {
        return;
    }

    int nRowCount = pDesTableWgt->rowCount();

    QTableWidgetItem *item0 = new QTableWidgetItem;

	item0->setData(Qt::UserRole + 1, nPointTableIndex);
//    if (rptInfo.GroupNum == CONTROLGROUP)
//    {
        item0->setText(QString::number(nRowCount));
        
//     }
//     else
//     {
//         item0->setText(QString::number(nRowCount + 1));
//     }
    

    QTableWidgetItem *item1 = new QTableWidgetItem;
    item1->setText(QString::number(rptInfo.NUM2));

    QTableWidgetItem *item2 = new QTableWidgetItem;
    QString strGroupName;
    if (rptInfo.GroupNum == BINARYGROUP || rptInfo.GroupNum == TBINARYGROUP || rptInfo.GroupNum == TABINARYGROUP)
    {
        strGroupName = tr("Binary");
    }
    else if (rptInfo.GroupNum == ANALOGGROUP || rptInfo.GroupNum == TANALOGGROUP || rptInfo.GroupNum == TBANALOGGROUP
        || rptInfo.GroupNum == TBCANALOGGROUP || rptInfo.GroupNum == TBCBNALOGGROUP || rptInfo.GroupNum == TBCCNALOGGROUP)
    {
        strGroupName = tr("Analog");
    }
    else if (rptInfo.GroupNum == KWHGROUP)
    {
        strGroupName = tr("Kwh");
    }
    else if (rptInfo.GroupNum == CONTROLGROUP)
    {
        strGroupName = tr("Control");
    }

    item2->setText(strGroupName);

    QTableWidgetItem *item3 = new QTableWidgetItem;
    item3->setText(rptInfo.Destriber);

    QTableWidgetItem *item4 = new QTableWidgetItem;
    item4->setText(rptInfo.Unit);

    pDesTableWgt->insertRow(nRowCount);
    pDesTableWgt->setItem(nRowCount, 0, item0);
    pDesTableWgt->setItem(nRowCount, 1, item1);
    pDesTableWgt->setItem(nRowCount, 2, item2);
    pDesTableWgt->setItem(nRowCount, 3, item3);
    pDesTableWgt->setItem(nRowCount, 4, item4);
}

void CChoosePointsWgt::UpdateAnalogToDesTables(RPT rptInfo, CAnalogDragTableWgt * pDesTableWgt,int nPointTableIndex)
{
	if (pDesTableWgt == nullptr)
	{
		return;
	}

	int nRowCount = pDesTableWgt->rowCount();

	QTableWidgetItem *item0 = new QTableWidgetItem;
	//    if (rptInfo.GroupNum == CONTROLGROUP)
	//    {
	item0->setText(QString::number(nRowCount));
	item0->setData(Qt::UserRole + 1, nPointTableIndex);

	//     }
	//     else
	//     {
	//         item0->setText(QString::number(nRowCount + 1));
	//     }


	QTableWidgetItem *item1 = new QTableWidgetItem;
	item1->setText(QString::number(rptInfo.NUM2));

	QTableWidgetItem *item2 = new QTableWidgetItem;
	QString strGroupName = tr("Analog");


	item2->setText(strGroupName);

	QTableWidgetItem *item3 = new QTableWidgetItem;
	item3->setText(rptInfo.Destriber);

	QTableWidgetItem *item4 = new QTableWidgetItem;
	item4->setText(rptInfo.Unit);

	QTableWidgetItem *item5 = new QTableWidgetItem;
	

	QTableWidgetItem *item6 = new QTableWidgetItem;
	

	QTableWidgetItem *item7 = new QTableWidgetItem;
	

	QTableWidgetItem *item8 = new QTableWidgetItem;
	

	QTableWidgetItem *item9 = new QTableWidgetItem;
	

	if (m_pCurrentPoints->GetDoorDatas()[nPointTableIndex].contains(rptInfo.NUM2))
	{
		item5->setText(QString::number(m_pCurrentPoints->GetDoorDatas()[nPointTableIndex][rptInfo.NUM2].fBanalog));
		item6->setText(QString::number(m_pCurrentPoints->GetDoorDatas()[nPointTableIndex][rptInfo.NUM2].fKanalog));
		item7->setText(QString::number(m_pCurrentPoints->GetDoorDatas()[nPointTableIndex][rptInfo.NUM2].fUpperLimit));
		item8->setText(QString::number(m_pCurrentPoints->GetDoorDatas()[nPointTableIndex][rptInfo.NUM2].fDownLimit));
		item9->setText(QString::number(m_pCurrentPoints->GetDoorDatas()[nPointTableIndex][rptInfo.NUM2].fThreshold));
	}
	else
	{
		item5->setText(QString::number(rptInfo.fBanalog));
		item6->setText(QString::number(rptInfo.fKanalog));
		item7->setText(QString::number(rptInfo.fUpperLimit));
		item8->setText(QString::number(rptInfo.fDownLimit));
		item9->setText(QString::number(rptInfo.fThreshold));

	}

	pDesTableWgt->insertRow(nRowCount);
	pDesTableWgt->setItem(nRowCount, 0, item0);
	pDesTableWgt->setItem(nRowCount, 1, item1);
	pDesTableWgt->setItem(nRowCount, 2, item2);
	pDesTableWgt->setItem(nRowCount, 3, item3);
	pDesTableWgt->setItem(nRowCount, 4, item4);

	pDesTableWgt->setItem(nRowCount, 5, item5);
	pDesTableWgt->setItem(nRowCount, 6, item6);
	pDesTableWgt->setItem(nRowCount, 7, item7);
	pDesTableWgt->setItem(nRowCount, 8, item8);
	pDesTableWgt->setItem(nRowCount, 9, item9);

}

QMap<int, QString> CChoosePointsWgt::GetRemoteToDesc()
{
    QMap<int, QString> mapInfo;
    for (int i = 0; i < m_pDescTableWgt->rowCount(); i++)
    {
        mapInfo.insert(m_pDescTableWgt->item(i, 0)->text().toInt(), m_pDescTableWgt->item(i, 3)->text());
    }
    return mapInfo;
}

void CChoosePointsWgt::SetProjectPath(const QString &strPath)
{
    m_pVirtualDescTableWgt->SetSaveFileName(strPath);
}

void CChoosePointsWgt::SaveChooseBinFile(const QString &strFilename)
{
    if (strFilename.isEmpty())
    {
        return;
    }

    QFile file(strFilename);

    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Warning"), file.errorString());
        return;
    }

    //遥信
    for (int i = 0; i < m_pDescTableWgt->rowCount(); i++)
    {
		if (i == 0)
		{
			file.write("引用表类型=YX\n");
			file.write("引用表数目=" + QByteArray(QString::number(m_pDescTableWgt->rowCount()).toStdString().c_str()) + "\n");
		}


        int nDeviceNum = m_pDescTableWgt->item(i, 1)->text().toInt();
       // int nDeviceGroup = m_pPointTableAnalyse->GetBinaryIdGroup()[nDeviceNum].GroupNum;
        //nDeviceNum = m_pPointTableAnalyse->GetBinaryIdGroup()[nDeviceNum].NUM;


		file.write(QByteArray(QString::number(i+1).toStdString().c_str()) + "," + 
			QByteArray(QString::number(nDeviceNum).toStdString().c_str()) + "," +
			QByteArray(m_pDescTableWgt->item(i, 0)->data(Qt::UserRole+1).toByteArray()) + QByteArray("\n"));
    }

	//双点遥信
	for (int i = 0; i < m_pDoubleDescTableWgt->rowCount(); i++)
	{
		if (i == 0)
		{
			file.write("引用表类型=DYX\n");
			file.write("引用表数目=" + QByteArray(QString::number(m_pDoubleDescTableWgt->rowCount()).toStdString().c_str()) + "\n");
		}


		int nDeviceNum = m_pDoubleDescTableWgt->item(i, 1)->text().toInt();
		// int nDeviceGroup = m_pPointTableAnalyse->GetBinaryIdGroup()[nDeviceNum].GroupNum;
		//nDeviceNum = m_pPointTableAnalyse->GetBinaryIdGroup()[nDeviceNum].NUM;


		file.write(QByteArray(QString::number(i + 1).toStdString().c_str()) + "," + 
			QByteArray(QString::number(nDeviceNum).toStdString().c_str()) + "," +
			QByteArray(m_pDoubleDescTableWgt->item(i, 0)->data(Qt::UserRole + 1).toByteArray()) + QByteArray("\n"));
	}

    //遥测
    for (int i = 0; i < m_pAnalogDescTableWgt->rowCount(); i++)
    {
		if (i == 0)
		{
			file.write("引用表类型=YC\n");
			file.write("引用表数目=" + QByteArray(QString::number(m_pAnalogDescTableWgt->rowCount()).toStdString().c_str()) + "\n");
		}

		int nDeviceNum = m_pAnalogDescTableWgt->item(i, 1)->text().toInt();

		float fB = m_pAnalogDescTableWgt->item(i, 5)->text().toFloat();
		float fk = m_pAnalogDescTableWgt->item(i, 6)->text().toFloat();
		float fUp = m_pAnalogDescTableWgt->item(i, 7)->text().toFloat();
		float fDown = m_pAnalogDescTableWgt->item(i, 8)->text().toFloat();
		float fThreshold = m_pAnalogDescTableWgt->item(i, 9)->text().toFloat();

		file.write(QByteArray(QString::number(i + 1).toStdString().c_str()) + "," +
			QByteArray(QString::number(nDeviceNum).toStdString().c_str()) +  "," + 
			QByteArray(QString::number(fB).toStdString().c_str()) + "," +
			QByteArray(QString::number(fk).toStdString().c_str()) + "," +
			QByteArray(QString::number(fUp).toStdString().c_str()) + "," +
			QByteArray(QString::number(fDown).toStdString().c_str()) + "," +
			QByteArray(QString::number(fThreshold).toStdString().c_str()) + "," +
			QByteArray(m_pAnalogDescTableWgt->item(i, 0)->data(Qt::UserRole + 1).toByteArray()) + "\n");
	}

    //遥脉
    for (int i = 0; i < m_pKwhDescTableWgt->rowCount(); i++)
    {
		if (i == 0)
		{
			file.write("引用表类型=YM\n");
			file.write("引用表数目=" + QByteArray(QString::number(m_pKwhDescTableWgt->rowCount()).toStdString().c_str()) + QByteArray("\n"));
		}

		int nDeviceNum = m_pKwhDescTableWgt->item(i, 1)->text().toInt();

		file.write(QByteArray(QString::number(i + 1).toStdString().c_str()) + "," +
			QByteArray(QString::number(nDeviceNum).toStdString().c_str()) + "," +
			QByteArray(m_pKwhDescTableWgt->item(i, 0)->data(Qt::UserRole + 1).toByteArray()) + "\n");
	}

    //遥控
    for (int i = 0; i < m_pControlDescTableWgt->rowCount(); i++)
    {
		if (i == 0)
		{
			file.write("引用表类型=YK\n");
			file.write("引用表数目=" + QByteArray(QString::number(m_pControlDescTableWgt->rowCount()).toStdString().c_str()) + QByteArray("\n"));
		}

		int nDeviceNum = m_pControlDescTableWgt->item(i, 1)->text().toInt();

		file.write(QByteArray(QString::number(i + 1).toStdString().c_str()) + "," +
			QByteArray(QString::number(nDeviceNum).toStdString().c_str()) + "," +
			QByteArray(m_pControlDescTableWgt->item(i, 0)->data(Qt::UserRole + 1).toByteArray()) + "\n");
	}

    //file.write(btRead);

    file.close();
    QMessageBox::information(this, tr("Success"), tr("Save Success!"));
}

void CChoosePointsWgt::InitConboboxInfo()
{
	//查找目录下的所有点表文件
	QString strPaht = qApp->applicationDirPath() + "/ini/d225/pointtable";

	QDir tDir(strPaht);

	QFileInfoList tFileList = tDir.entryInfoList(QDir::Files);

	m_pComCooseTable->addItem(m_strProjectName);

	for (int i = 0; i < tFileList.count(); i++)
	{
		if (tFileList.at(i).fileName().contains("DEV_") && tFileList.at(i).fileName().endsWith(".txt"))
		{
			m_pComCooseTable->addItem(tFileList.at(i).fileName().remove(".txt"));
			//
			CRemotePonitTableAnalyse* pTable = new CRemotePonitTableAnalyse;

			if (!pTable->OpenPointTableFile(strPaht + "/" + tFileList.at(i).fileName()))
			{
				return;
			}

			int nTableNum = tFileList.at(i).fileName().remove("DEV_").remove(".txt").toInt();

			pTable->SetTableFlag(nTableNum);
			m_pIdTPointInfo.insert(nTableNum, pTable);

		}
	}

}