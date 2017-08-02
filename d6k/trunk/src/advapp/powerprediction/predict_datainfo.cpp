#include "predict_datainfo.h"
#include "predict_define.h"
#include "predict_module.h"

#include <QFile>
#include <QXmlStreamWriter>
#include <QMessageBox>

/*! \fn bool CInverterInfo::SaveADIData(QXmlStreamWriter& writer)
*****************************************************************
** \brief    CInverterInfo::SaveADIData
** \details  保存单台逆变器遥测、遥信数据
** \param    writer
** \return   bool
** \author   GuoHaijun
** \date     2017年5月3日
** \note
*****************************************************************/
bool CInverterInfo::SaveADIData(QXmlStreamWriter& writer)
{
	writer.writeStartElement("data");
	writer.writeAttribute("Count", QString("%1").arg(m_vecTableInfo.size()));
	int i = 1;
	for (auto const avec : m_vecTableInfo)
	{
		if ( i == 3 )
		{
			writer.writeStartElement("e");
		}
		else
		{
			writer.writeStartElement("e");
		}
		writer.writeAttribute("ID", QString("%1").arg(avec.m_nID));
		writer.writeAttribute("Name", QString("%1").arg(avec.m_szName));
		writer.writeAttribute("Description", QString("%1").arg(avec.m_strDescription));
		writer.writeAttribute("LinkedName", QString("%1").arg(avec.m_szLinkedTagName));
		writer.writeAttribute("Type", QString("%1").arg(avec.m_nType));
		writer.writeEndElement();
		i++;
	}
	writer.writeEndElement();
	return true;
}
/*! \fn bool CInverterInfo::SaveInverterData(QXmlStreamWriter& writer)
**********************************************************************
** \brief    CInverterInfo::SaveInverterData
** \details  保存单台逆变器信息
** \param    writer
** \return   bool
** \author   GuoHaijun
** \date     2017年5月3日
** \note
**********************************************************************/
bool CInverterInfo::SaveInverterData(QXmlStreamWriter& writer)
{
	writer.writeStartElement("inverter");
	writer.writeAttribute("Name", QString("%1").arg(m_szName));
	writer.writeAttribute("DeviceID", QString("%1").arg(m_nID));
	//writer.writeStartElement("data");
	SaveADIData(writer);
	//writer.writeEndElement();
	writer.writeEndElement();
	return true;
}
/*! \fn bool CInverterInfo::LoadData(QXmlStreamReader& reader)
**************************************************************
** \brief    CInverterInfo::LoadData
** \details  读取逆变器遥测数据
** \param    reader
** \return   bool
** \author   GuoHaijun
** \date     2017年5月16日
** \note
**************************************************************/
bool CInverterInfo::LoadData(QXmlStreamReader& reader, CInverterInfo* pInvterInfo)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "data")
			{
				ReadADIN(reader,pInvterInfo);
			}
		}
		else if (reader.isEndElement())
		{
			break;
		}
		reader.readNext();
	}
	return true;
}
/*! \fn bool CInverterInfo::ReadADIN(QXmlStreamReader& reader)
**************************************************************
** \brief    CInverterInfo::ReadADIN
** \details  读取逆变器遥测数据
** \param    reader
** \return   bool
** \author   GuoHaijun
** \date     2017年5月16日
** \note
**************************************************************/
bool CInverterInfo::ReadADIN(QXmlStreamReader& reader, CInverterInfo* pInvterInfo)
{
	m_vecTableInfo.clear();
	
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			//if (strTmp == "ai")
			{
				ReadAi(reader,pInvterInfo);
			}
			//else if (strTmp == "di")
			//{
			//	ReadDi(reader, pInvterInfo);
			//}
		}
		else if (reader.isEndElement() && strTmp == "data")
		{
			break;
		}

		reader.readNext();
	}

	return true;
}
/*! \fn bool CInverterInfo::ReadAi(QXmlStreamReader& reader)
**************************************************************
** \brief    CInverterInfo::ReadAi
** \details  读取逆变器遥测数据
** \param    reader
** \return   bool
** \author   GuoHaijun
** \date     2017年5月16日
** \note
**************************************************************/
bool CInverterInfo::ReadAi(QXmlStreamReader& reader, CInverterInfo* pInvterInfo)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();

			if (strTmp == "e")
			{
				CPPPointInfo pAnalog;
				int nID = reader.attributes().value("ID").toInt();
				pAnalog.m_nID = nID;
				QString szName = reader.attributes().value("Name").toString();
				pAnalog.m_szName = szName;

				pAnalog.m_strDescription = reader.attributes().value("Description").toString();

				QString szLinkedName = reader.attributes().value("LinkedName").toString();
				pAnalog.m_szLinkedTagName = szLinkedName;
				int nType = reader.attributes().value("Type").toInt();
				pAnalog.m_nType = nType;
				if (pAnalog.m_szName == pInvterInfo->m_ActPower.m_szName)
				{
					pInvterInfo->m_ActPower.m_nID = pAnalog.m_nID;
					pInvterInfo->m_ActPower.m_szName = pAnalog.m_szName;
					pInvterInfo->m_ActPower.m_strDescription = pAnalog.m_strDescription;
					pInvterInfo->m_ActPower.m_szLinkedTagName = pAnalog.m_szLinkedTagName;
					pInvterInfo->m_ActPower.m_nType = pAnalog.m_nType;
				}
				else if (pAnalog.m_szName == pInvterInfo->m_ReactPower.m_szName)
				{
					pInvterInfo->m_ReactPower.m_nID = pAnalog.m_nID;
					pInvterInfo->m_ReactPower.m_szName = pAnalog.m_szName;
					pInvterInfo->m_ReactPower.m_strDescription = pAnalog.m_strDescription;
					pInvterInfo->m_ReactPower.m_szLinkedTagName = pAnalog.m_szLinkedTagName;
					pInvterInfo->m_ReactPower.m_nType = pAnalog.m_nType;
				}
				else if (pAnalog.m_szName == pInvterInfo->m_OpenState.m_szName)
				{
					pInvterInfo->m_OpenState.m_nID = pAnalog.m_nID;
					pInvterInfo->m_OpenState.m_szName = pAnalog.m_szName;
					pInvterInfo->m_OpenState.m_strDescription = pAnalog.m_strDescription;
					pInvterInfo->m_OpenState.m_szLinkedTagName = pAnalog.m_szLinkedTagName;
					pInvterInfo->m_OpenState.m_nType = pAnalog.m_nType;
				}
				m_vecTableInfo.push_back(pAnalog);
			}
		}
		else if (reader.isEndElement())
		{
			break;
		}

		reader.readNext();
	}

	return true;
}
/*! \fn bool CInverterInfo::ReadDi(QXmlStreamReader& reader, CInverterInfo* pInvterInfo)
**************************************************************
** \brief    CInverterInfo::ReadDi
** \details  读取逆变器遥测数据
** \param    reader
** \return   bool
** \author   GuoHaijun
** \date     2017年6月9日
** \note
**************************************************************/
bool CInverterInfo::ReadDi(QXmlStreamReader& reader, CInverterInfo* pInvterInfo)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();

			if (strTmp == "di")
			{
				CPPPointInfo pAnalog;
				int nID = reader.attributes().value("ID").toInt();
				pAnalog.m_nID = nID;
				QString szName = reader.attributes().value("Name").toString();
				pAnalog.m_szName = szName;
				QString szLinkedName = reader.attributes().value("LinkedName").toString();
				pAnalog.m_szLinkedTagName = szLinkedName;
				int nType = reader.attributes().value("Type").toInt();
				pAnalog.m_nType = nType;
				if (pAnalog.m_szName == pInvterInfo->m_OpenState.m_szName)
				{
					pInvterInfo->m_OpenState.m_nID = pAnalog.m_nID;
					pInvterInfo->m_OpenState.m_szName = pAnalog.m_szName;
					pInvterInfo->m_OpenState.m_szLinkedTagName = pAnalog.m_szLinkedTagName;
					pInvterInfo->m_OpenState.m_nType = pAnalog.m_nType;
				}
				m_vecTableInfo.push_back(pAnalog);
			}
		}
		else if (reader.isEndElement())
		{
			break;
		}

		reader.readNext();
	}

	return true;
}

/*! \fn void CInverterInfo::Init()
***********************************
** \brief    CInverterInfo::Init()
** \details  单台逆变器信息初始化
** \param
** \return   void
** \author   GuoHaijun
** \date     2017年5月3日
** \note
***********************************/
void CInverterInfo::Init()
{
	m_szName = "inverter";
	m_vecTableInfo.append(m_ActPower);
	m_vecTableInfo.append(m_ReactPower);
	m_vecTableInfo.append(m_OpenState);
}

bool CPlantInfo::LogMsg(const char* szLogTxt, int nLevel)
{
	CPowerPredictApi * pApi = GetPredictModuleApi();
	Q_ASSERT(pApi);
	if (pApi == nullptr)
	{
		return false;
	}

	return pApi->LogMsg(szLogTxt, nLevel);
}
/*! \fn bool CPlantInfo::SaveADIData(QXmlStreamWriter& writer)
**************************************************************
** \brief    CPlantInfo::SaveADIData
** \details  保存整站遥测数据
** \param    writer
** \return   bool
** \author   GuoHaijun
** \date     2017年5月3日
** \note
**************************************************************/
bool CPlantInfo::SaveADIData(QXmlStreamWriter& writer)
{
	QString szLog;
	writer.writeStartElement("data");
	writer.writeAttribute("Count", QString("%1").arg(m_vecTableInfo.size()));
	for (auto const it : m_vecTableInfo)
	{
		writer.writeStartElement("e");
		writer.writeAttribute("ID", QString("%1").arg(it.m_nID));
		writer.writeAttribute("Name", QString("%1").arg(it.m_szName));
		writer.writeAttribute("Description", QString("%1").arg(it.m_strDescription));
		writer.writeAttribute("LinkedName", QString("%1").arg(it.m_szLinkedTagName));

		if (it.m_szLinkedTagName.isEmpty() == true)
		{
			szLog = QString(QObject::tr("[%1] point`s LinkedTagName is null, please link first!").arg(it.m_szName));
			LogMsg(szLog.toStdString().c_str(), 0);
		}
#if 0
// Fixed by LiJin 2017.7.14
		if (avec.m_szLinkedTagName == "")
		{
			QMessageBox warning(QMessageBox::Warning, "Warning", QStringLiteral("LinkedTagName is null,please link first!"));
			warning.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
			warning.setButtonText(QMessageBox::Ok, QStringLiteral("确定"));
			warning.setButtonText(QMessageBox::Cancel, QStringLiteral("取消"));

			if (warning.exec() == QMessageBox::Cancel)
			{
				return false;
			}
		}
#endif
		writer.writeAttribute("Type", QString("%1").arg(it.m_nType));
		writer.writeEndElement();
	}
	writer.writeEndElement();
	return true;
}
/*! \fn bool CPlantInfo::SavePlantData(QXmlStreamWriter& writer)
**************************************************************
** \brief    CPlantInfo::SavePlantData
** \details  保存整站信息
** \param    writer
** \return   bool
** \author   GuoHaijun
** \date     2017年5月3日
** \note
**************************************************************/
bool CPlantInfo::SavePlantData(QXmlStreamWriter& writer)
{
	writer.writeStartElement("plant");
	//writer.writeAttribute("deviceid", "1");
	
	writer.writeStartElement("staticdata");
	for each (auto var in m_vecTableStaticInfo)
	{
		writer.writeStartElement("sdata");
		writer.writeAttribute("key", QString("%1").arg(var.m_strKey));
		writer.writeAttribute("name", QString("%1").arg(var.m_szName));
		//Description

		writer.writeEndElement();
	}
	writer.writeEndElement();


	writer.writeStartElement("dynamicdata");
	writer.writeAttribute("count", QString("%1").arg(m_vecTableInfo.size()));
	//writer.writeStartElement("data");
	SaveADIData(writer);
	//writer.writeEndElement();
	writer.writeEndElement();




	writer.writeEndElement();
	return true;
}
/*! \fn bool CPlantInfo::LoadData(QXmlStreamReader& reader)
**************************************************************
** \brief    CPlantInfo::LoadData
** \details  读取整站遥测数据
** \param    reader
** \return   bool
** \author   GuoHaijun
** \date     2017年5月15日
** \note
**************************************************************/
bool CPlantInfo::LoadData(QXmlStreamReader& reader, CPlantInfo* pPlntInfo)
{
	QString strTmp1, strTmp2;
	while (!reader.atEnd())
	{
		strTmp1 = reader.name().toString();
		if (reader.isStartElement())
		{
			if (strTmp1 == "staticdata")
			{

			}
			else if (strTmp1 == "dynamicdata")
			{

			}

			strTmp2 = reader.name().toString();
			if (strTmp2 == "data")
			{
				ReadADIN(reader,pPlntInfo);
			}
		}
		else if (reader.isEndElement() && strTmp1 == "plant")
		{
			break;
		}
		reader.readNext();
	}
	return true;
}
/*! \fn bool CPlantInfo::ReadADIN(QXmlStreamReader& reader)
**************************************************************
** \brief    CPlantInfo::ReadADIN
** \details  读取整站遥测数据
** \param    reader
** \return   bool
** \author   GuoHaijun
** \date     2017年5月15日
** \note
**************************************************************/
bool CPlantInfo::ReadADIN(QXmlStreamReader& reader, CPlantInfo* pPlntInfo)
{
	QString strTmp1, strTmp2;

	m_vecTableInfo.clear();

	while (!reader.atEnd())
	{
		strTmp1 = reader.name().toString();
		if (reader.isStartElement())
		{
			strTmp2 = reader.name().toString();
			if (strTmp2 == "e")
			{
				ReadAi(reader, pPlntInfo);
			}
		}
		else if (reader.isEndElement() && strTmp1 == "data")
		{
			break;
		}
		reader.readNext();
	}

	return true;
}
/*! \fn bool CPlantInfo::ReadAi(QXmlStreamReader& reader)
**************************************************************
** \brief    CPlantInfo::ReadAi
** \details  读取整站遥测数据
** \param    reader
** \return   bool
** \author   GuoHaijun
** \date     2017年5月15日
** \note
**************************************************************/
bool CPlantInfo::ReadAi(QXmlStreamReader& reader, CPlantInfo* pPlntInfo)
{
	QString strTmp1, strTmp2;
	while(!reader.atEnd())
	{
		strTmp1 = reader.name().toString();
		if (reader.isStartElement())
		{
			strTmp2 = reader.name().toString();

			if (strTmp2 == "e")
			{
				CPPPointInfo pAnalog;
				int nID = reader.attributes().value("ID").toInt();
				pAnalog.m_nID = nID;
				QString szName = reader.attributes().value("Name").toString();
				pAnalog.m_szName = szName;

				pAnalog.m_strDescription = reader.attributes().value("Description").toString();

				QString szLinkedName = reader.attributes().value("LinkedName").toString();
				pAnalog.m_szLinkedTagName = szLinkedName;
				int nType = reader.attributes().value("Type").toInt();
				pAnalog.m_nType = nType;
				if (pAnalog.m_szName == pPlntInfo->m_ActPower.m_szName)
				{
					pPlntInfo->m_ActPower.m_nID = pAnalog.m_nID;
					pPlntInfo->m_ActPower.m_szName = pAnalog.m_szName;
					pPlntInfo->m_ActPower.m_strDescription = pAnalog.m_strDescription;
					pPlntInfo->m_ActPower.m_szLinkedTagName = pAnalog.m_szLinkedTagName;
					pPlntInfo->m_ActPower.m_nType = pAnalog.m_nType;
				}
				else if(pAnalog.m_szName == pPlntInfo->m_ReactPower.m_szName)
				{
					pPlntInfo->m_ReactPower.m_nID = pAnalog.m_nID;
					pPlntInfo->m_ReactPower.m_szName = pAnalog.m_szName;
					pPlntInfo->m_ReactPower.m_strDescription = pAnalog.m_strDescription;
					pPlntInfo->m_ReactPower.m_szLinkedTagName = pAnalog.m_szLinkedTagName;
					pPlntInfo->m_ReactPower.m_nType = pAnalog.m_nType;
				}
				m_vecTableInfo.push_back(pAnalog);
			}
		}
		else if (reader.isEndElement() && strTmp1 == "e")
		{
			break;
		}

		reader.readNext();
	}

	return true;
}

bool CPlantInfo::ReadStaticData(QXmlStreamReader& reader, CPlantInfo* pPlntInfo)
{
	return true;
}

/*! \fn void CPlantInfo::Init()
********************************
** \brief    CPlantInfo::Init
** \details  整站信息初始化
** \param    
** \return   void
** \author   GuoHaijun
** \date     2017年5月3日
** \note
********************************/
void CPlantInfo::Init()
{
	m_strName = "Plant Data";
	//添加动态数据
	m_vecTableInfo.clear();
	
	m_vecTableInfo.append(m_ActPower);
	m_vecTableInfo.append(m_ReactPower);

	//静态信息
	m_vecTableStaticInfo.clear();
}
/*! \fn bool CInverterGroup::SaveInverterGrp(QXmlStreamWriter& writer)
**********************************************************************
** \brief    CInverterGroup::SaveInverterGrp
** \details  保存逆变器组数据
** \param    writer
** \return   bool
** \author   GuoHaijun
** \date     2017年5月3日
** \note
**********************************************************************/
bool CInverterGroup::SaveInverterGrp(QXmlStreamWriter& writer, CInverterGroup& invertGrp)
{
	writer.writeStartElement("inverters");
	writer.writeAttribute("Count", QString("%1").arg(m_arrInverters.size()));
	writer.writeAttribute("Name", QString("%1").arg(m_strName));
	for (auto it : invertGrp.m_arrInverters)
	{
		it->SaveInverterData(writer);
	}
	writer.writeEndElement();
	return true;
}
/*! \fn bool CInverterGroup::LoadInvertersGrp(QXmlStreamReader& reader)
**************************************************************
** \brief    CInverterGroup::LoadInvertersGrp
** \details  读取逆变器组数据
** \param    reader
** \return   bool
** \author   GuoHaijun
** \date     2017年5月15日
** \note
**************************************************************/
bool CInverterGroup::LoadInvertersGrp(QXmlStreamReader& reader)
{
	QString strTmp1, strTmp2;
	while (!reader.atEnd())
	{
		strTmp1 = reader.name().toString();
		if (reader.isStartElement())
		{
			strTmp2 = reader.name().toString();
			if (strTmp2 == "inverter")
			{
				CInverterInfo *pInverter = new CInverterInfo;
				QString nName = reader.attributes().value("Name").toString();
				pInverter->m_szName = nName;
				int nID = reader.attributes().value("ID").toInt();
				pInverter->m_nID = nID;
				pInverter->m_nDeviceID = reader.attributes().value("DeviceID").toInt();

				pInverter->LoadData(reader, pInverter);
				m_arrInverters.push_back(pInverter);
				//m_mapInverters.insert(pInverter.m_szName, &pInverter);
			}
		}
		else if (reader.isEndElement() && strTmp1 == "inverters")
		{
			/*reader.readNext();
			QString strTmp1 = reader.name().toString();
			if (strTmp == "inverters")
			{
				break;
			}
			else
			{
				continue;
			}*/
			break;
		}
		reader.readNext();
	}
	return true;
}
/*! \fn bool CWeatherData::SaveADIData(QXmlStreamWriter& writer)
****************************************************************
** \brief    CWeatherData::SaveADIData
** \details  保存气象遥测、遥信数据
** \param    writer
** \return   bool
** \author   GuoHaijun
** \date     2017年5月4日
** \note
****************************************************************/
bool CWeatherData::SaveADIData(QXmlStreamWriter& writer)
{
	writer.writeStartElement("data");
	writer.writeAttribute("Count", QString("%1").arg(m_vecTableInfo.size()));
	for (auto const avec : m_vecTableInfo)
	{
		writer.writeStartElement("e");
		writer.writeAttribute("ID", QString("%1").arg(avec.m_nID));
		writer.writeAttribute("Name", QString("%1").arg(avec.m_szName));
		writer.writeAttribute("Description", QString("%1").arg(avec.m_strDescription));
		writer.writeAttribute("LinkedName", QString("%1").arg(avec.m_szLinkedTagName));
		writer.writeAttribute("Type", QString("%1").arg(avec.m_nType));
		writer.writeEndElement();
	}
	writer.writeEndElement();
	return true;
}
/*! \fn bool CWeatherData::SaveWeatherData(QXmlStreamWriter& writer)
********************************************************************
** \brief    CWeatherData::SaveWeatherData
** \details  保存气象信息
** \param    writer
** \return   bool
** \author   GuoHaijun
** \date     2017年5月4日
** \note
********************************************************************/
bool CWeatherData::SaveWeatherData(QXmlStreamWriter& writer)
{
	writer.writeStartElement("w");
	writer.writeAttribute("Name", QString("%1").arg(m_strName));
	//writer.writeAttribute("deviceid", QString("%1").arg(2));

	//writer.writeStartElement("data");
	SaveADIData(writer);
	//writer.writeEndElement();
	writer.writeEndElement();
	return true;
}


/*! \fn bool CWeatherData::LoadData(QXmlStreamReader& reader)
**************************************************************
** \brief    CWeatherData::LoadData
** \details  读取气象仪遥测数据
** \param    reader
** \return   bool
** \author   GuoHaijun
** \date     2017年5月15日
** \note
**************************************************************/
bool CWeatherData::LoadData(QXmlStreamReader& reader, CWeatherData* pWeaInfo)
{
	QString strTmp1, strTmp2;
	while (!reader.atEnd())
	{
		strTmp1 = reader.name().toString();
		if (reader.isStartElement())
		{
			strTmp2 = reader.name().toString();
			if (strTmp2 == "data")
			{
				ReadADIN(reader, pWeaInfo);

				break;
			}
		}
		else if (reader.isEndElement())
		{
			break;
		}

		reader.readNext();

	}
	return true;
}
/*! \fn bool CWeatherData::ReadADIN(QXmlStreamReader& reader)
**************************************************************
** \brief    CWeatherData::ReadADIN
** \details  读取气象仪遥测数据
** \param    reader
** \return   bool
** \author   GuoHaijun
** \date     2017年5月15日
** \note
**************************************************************/
bool CWeatherData::ReadADIN(QXmlStreamReader& reader, CWeatherData* pWeaInfo)
{
	QString strTmp1, strTmp2;
	pWeaInfo->m_vecTableInfo.clear();
	
	while (!reader.atEnd())
	{
		strTmp1 = reader.name().toString();
		if (reader.isStartElement())
		{
			strTmp2 = reader.name().toString();
			if (strTmp2 == "e")
			{
				ReadAi(reader, pWeaInfo);
			}
		}
		else if (reader.isEndElement() && strTmp1 == "data")
		{
			break;
		}

		reader.readNext();
	}

	return true;
}
/*! \fn bool CWeatherData::ReadAi(QXmlStreamReader& reader)
**************************************************************
** \brief    CWeatherData::ReadAi
** \details  读取气象仪遥测数据
** \param    reader
** \return   bool
** \author   GuoHaijun
** \date     2017年5月15日
** \note
**************************************************************/
bool CWeatherData::ReadAi(QXmlStreamReader& reader, CWeatherData* pWeaInfo)
{
	QString strTmp1, strTmp2;

	while (!reader.atEnd())
	{
		strTmp1 = reader.name().toString();
		if (reader.isStartElement())
		{
			strTmp2 = reader.name().toString();
			if (strTmp2 == "e")
			{
				CPPPointInfo pAnalog;
				int nID = reader.attributes().value("ID").toInt();
				pAnalog.m_nID = nID;
				QString szName = reader.attributes().value("Name").toString();
				pAnalog.m_szName = szName;

				pAnalog.m_strDescription = reader.attributes().value("Description").toString();

				QString szLinkedName = reader.attributes().value("LinkedName").toString();
				pAnalog.m_szLinkedTagName = szLinkedName;
				int nType = reader.attributes().value("Type").toInt();
				pAnalog.m_nType = nType;
				if (pAnalog.m_szName == pWeaInfo->m_AirPressure.m_szName)
				{
					pWeaInfo->m_AirPressure.m_nID = pAnalog.m_nID;
					pWeaInfo->m_AirPressure.m_szName = pAnalog.m_szName;
					pWeaInfo->m_AirPressure.m_strDescription = pAnalog.m_strDescription;
					pWeaInfo->m_AirPressure.m_szLinkedTagName = pAnalog.m_szLinkedTagName;
					pWeaInfo->m_AirPressure.m_nType = pAnalog.m_nType;

					pWeaInfo->m_vecTableInfo.push_back(pWeaInfo->m_AirPressure);
				}
				else if (pAnalog.m_szName == pWeaInfo->m_AirTemperature.m_szName)
				{
					pWeaInfo->m_AirTemperature.m_nID = pAnalog.m_nID;
					pWeaInfo->m_AirTemperature.m_szName = pAnalog.m_szName;
					pWeaInfo->m_AirTemperature.m_strDescription = pAnalog.m_strDescription;
					pWeaInfo->m_AirTemperature.m_szLinkedTagName = pAnalog.m_szLinkedTagName;
					pWeaInfo->m_AirTemperature.m_nType = pAnalog.m_nType;

					pWeaInfo->m_vecTableInfo.push_back(pWeaInfo->m_AirTemperature);
				}
				else if (pAnalog.m_szName == pWeaInfo->m_AvergWindDirect.m_szName)
				{
					pWeaInfo->m_AvergWindDirect.m_nID = pAnalog.m_nID;
					pWeaInfo->m_AvergWindDirect.m_szName = pAnalog.m_szName;
					pWeaInfo->m_AvergWindDirect.m_strDescription = pAnalog.m_strDescription;
					pWeaInfo->m_AvergWindDirect.m_szLinkedTagName = pAnalog.m_szLinkedTagName;
					pWeaInfo->m_AvergWindDirect.m_nType = pAnalog.m_nType;

					pWeaInfo->m_vecTableInfo.push_back(pWeaInfo->m_AvergWindDirect);
				}
				else if (pAnalog.m_szName == pWeaInfo->m_AvergWindSpeed.m_szName)
				{
					pWeaInfo->m_AvergWindSpeed.m_nID = pAnalog.m_nID;
					pWeaInfo->m_AvergWindSpeed.m_szName = pAnalog.m_szName;
					pWeaInfo->m_AvergWindSpeed.m_strDescription = pAnalog.m_strDescription;
					pWeaInfo->m_AvergWindSpeed.m_szLinkedTagName = pAnalog.m_szLinkedTagName;
					pWeaInfo->m_AvergWindSpeed.m_nType = pAnalog.m_nType;

					pWeaInfo->m_vecTableInfo.push_back(pWeaInfo->m_AvergWindSpeed);
				}
				else if (pAnalog.m_szName == pWeaInfo->m_DirectRadiation.m_szName)
				{
					pWeaInfo->m_DirectRadiation.m_nID = pAnalog.m_nID;
					pWeaInfo->m_DirectRadiation.m_szName = pAnalog.m_szName;
					pWeaInfo->m_DirectRadiation.m_strDescription = pAnalog.m_strDescription;
					pWeaInfo->m_DirectRadiation.m_szLinkedTagName = pAnalog.m_szLinkedTagName;
					pWeaInfo->m_DirectRadiation.m_nType = pAnalog.m_nType;

					pWeaInfo->m_vecTableInfo.push_back(pWeaInfo->m_DirectRadiation);
				}
				else if (pAnalog.m_szName == pWeaInfo->m_RelativeHumdty.m_szName)
				{
					pWeaInfo->m_RelativeHumdty.m_nID = pAnalog.m_nID;
					pWeaInfo->m_RelativeHumdty.m_szName = pAnalog.m_szName;
					pWeaInfo->m_RelativeHumdty.m_strDescription = pAnalog.m_strDescription;
					pWeaInfo->m_RelativeHumdty.m_szLinkedTagName = pAnalog.m_szLinkedTagName;
					pWeaInfo->m_RelativeHumdty.m_nType = pAnalog.m_nType;

					pWeaInfo->m_vecTableInfo.push_back(pWeaInfo->m_RelativeHumdty);
				}
				else if (pAnalog.m_szName == pWeaInfo->m_ScattRadiation.m_szName)
				{
					pWeaInfo->m_ScattRadiation.m_nID = pAnalog.m_nID;
					pWeaInfo->m_ScattRadiation.m_szName = pAnalog.m_szName;
					pWeaInfo->m_ScattRadiation.m_strDescription = pAnalog.m_strDescription;
					pWeaInfo->m_ScattRadiation.m_szLinkedTagName = pAnalog.m_szLinkedTagName;
					pWeaInfo->m_ScattRadiation.m_nType = pAnalog.m_nType;

					pWeaInfo->m_vecTableInfo.push_back(pWeaInfo->m_ScattRadiation);
				}
				else if (pAnalog.m_szName == pWeaInfo->m_TotalRadiation.m_szName)
				{
					pWeaInfo->m_TotalRadiation.m_nID = pAnalog.m_nID;
					pWeaInfo->m_TotalRadiation.m_szName = pAnalog.m_szName;
					pWeaInfo->m_TotalRadiation.m_strDescription = pAnalog.m_strDescription;
					pWeaInfo->m_TotalRadiation.m_szLinkedTagName = pAnalog.m_szLinkedTagName;
					pWeaInfo->m_TotalRadiation.m_nType = pAnalog.m_nType;

					pWeaInfo->m_vecTableInfo.push_back(pWeaInfo->m_TotalRadiation);
				}

				//m_vecTableInfo.push_back(pAnalog);
			}
		}
		else if (reader.isEndElement())
		{
			break;
		}

		reader.readNext();
	}

	return true;
}
/*! \fn void CWeatherData::Init()
*********************************
** \brief    CWeatherData::Init
** \details  气象信息初始化
** \param    
** \return   void
** \author   GuoHaijun
** \date     2017年5月4日
** \note
**********************************/
void CWeatherData::Init()
{
	m_strName = "Weather Data";
	m_vecTableInfo.append(m_TotalRadiation);
	m_vecTableInfo.append(m_DirectRadiation);
	m_vecTableInfo.append(m_ScattRadiation);
	m_vecTableInfo.append(m_AvergWindDirect);
	m_vecTableInfo.append(m_AvergWindSpeed);
	m_vecTableInfo.append(m_AirTemperature);
	m_vecTableInfo.append(m_RelativeHumdty);
	m_vecTableInfo.append(m_AirPressure);
}

/*! \fn bool CPredictData::SaveADIData(QXmlStreamWriter& writer)
****************************************************************
** \brief    CPredictData::SaveADIData
** \details  保存预测遥测数据
** \param    writer
** \return   bool
** \author   GuoHaijun
** \date     2017年5月4日
** \note
****************************************************************/
bool CPredictData::SaveADIData(QXmlStreamWriter& writer)
{
	writer.writeStartElement("data");
	writer.writeAttribute("Count", QString("%1").arg(m_vecTableInfo.size()));
	for (auto const it : m_vecTableInfo)
	{
		writer.writeStartElement("e");
		writer.writeAttribute("ID", QString("%1").arg(it.m_nID));
		writer.writeAttribute("Name", QString("%1").arg(it.m_szName));
		//Description
		writer.writeAttribute("Description", QString("%1").arg(it.m_strDescription));
		writer.writeAttribute("LinkedName", QString("%1").arg(it.m_szLinkedTagName));
		writer.writeAttribute("Type", QString("%1").arg(it.m_nType));
		writer.writeEndElement();
	}
	writer.writeEndElement();
	return true;
}
/*! \fn bool CPredictData::SavePredictData(QXmlStreamWriter& writer)
********************************************************************
** \brief    CPredictData::SavePredictData
** \details  保存预测信息
** \param    writer
** \return   bool
** \author   GuoHaijun
** \date     2017年5月4日
** \note
********************************************************************/
bool CPredictData::SavePredictData(QXmlStreamWriter& writer)
{
	writer.writeStartElement("prdt");
	writer.writeAttribute("Name", QString("%1").arg(m_strName));
	//writer.writeAttribute("deviceid", QString("%1").arg(2));

	writer.writeStartElement("data");
	SaveADIData(writer);
	writer.writeEndElement();
	writer.writeEndElement();
	return true;
}
/*! \fn bool CPredictData::LoadData(QXmlStreamReader& reader)
**************************************************************
** \brief    CPredictData::LoadData
** \details  读取预测遥测数据
** \param    reader
** \return   bool
** \author   GuoHaijun
** \date     2017年5月15日
** \note
**************************************************************/
bool CPredictData::LoadData(QXmlStreamReader& reader, CPredictData* pPrdtInfo)
{
	QString strTmp1, strTmp2;
	while (!reader.atEnd())
	{
		strTmp1 = reader.name().toString();
		if (reader.isStartElement())
		{
			strTmp2 = reader.name().toString();
			if (strTmp2 == "data")
			{
				ReadADIN(reader, pPrdtInfo);
			}
		}
		else if (reader.isEndElement())
		{
			break;
		}
		reader.readNext();
	}
	return true;
}
/*! \fn bool CPredictData::ReadADIN(QXmlStreamReader& reader)
**************************************************************
** \brief    CPredictData::ReadADIN
** \details  读取预测遥测数据
** \param    reader
** \return   bool
** \author   GuoHaijun
** \date     2017年5月15日
** \note
**************************************************************/
bool CPredictData::ReadADIN(QXmlStreamReader& reader, CPredictData* pPrdtInfo)
{
	m_vecTableInfo.clear();
	
	QString strTmp1, strTmp2;
	while (!reader.atEnd())
	{
		strTmp1 = reader.name().toString();
		if (reader.isStartElement())
		{
			strTmp2 = reader.name().toString();
			if (strTmp2 == "e")
			{
				ReadAO(reader, pPrdtInfo);
			}
		}
		else if (reader.isEndElement() && strTmp1 == "data")
		{
			break;
		}

		reader.readNext();
	}

	return true;
}
/*! \fn bool CPredictData::ReadAi(QXmlStreamReader& reader)
**************************************************************
** \brief    CPredictData::ReadAi
** \details  读取气象仪遥测数据
** \param    reader
** \return   bool
** \author   GuoHaijun
** \date     2017年5月15日
** \note
**************************************************************/
bool CPredictData::ReadAO(QXmlStreamReader& reader, CPredictData* pPrdtInfo)
{
	QString strTmp1, strTmp2;

	while (!reader.atEnd())
	{
		strTmp1 = reader.name().toString();
		if (reader.isStartElement())
		{
			strTmp2 = reader.name().toString();

			if (strTmp2 == "e")
			{
				CPPPointInfo pAnalog;
				int nID = reader.attributes().value("ID").toInt();
				pAnalog.m_nID = nID;
				QString szName = reader.attributes().value("Name").toString();
				pAnalog.m_szName = szName;
				
				pAnalog.m_strDescription = reader.attributes().value("Description").toString();

				QString szLinkedName = reader.attributes().value("LinkedName").toString();
				pAnalog.m_szLinkedTagName = szLinkedName;
				int nType = reader.attributes().value("Type").toInt();
				pAnalog.m_nType = nType;
				if (pAnalog.m_szName == pPrdtInfo->m_4Hour.m_szName)
				{
					pPrdtInfo->m_4Hour.m_nID = pAnalog.m_nID;
					pPrdtInfo->m_4Hour.m_szName = pAnalog.m_szName;
					pPrdtInfo->m_4Hour.m_strDescription = pAnalog.m_strDescription;
					pPrdtInfo->m_4Hour.m_szLinkedTagName = pAnalog.m_szLinkedTagName;
					pPrdtInfo->m_4Hour.m_nType = pAnalog.m_nType;
				}
				else if (pAnalog.m_szName == pPrdtInfo->m_72Hour.m_szName)
				{
					pPrdtInfo->m_72Hour.m_nID = pAnalog.m_nID;
					pPrdtInfo->m_72Hour.m_szName = pAnalog.m_szName;
					pPrdtInfo->m_4Hour.m_strDescription = pAnalog.m_strDescription;
					pPrdtInfo->m_72Hour.m_szLinkedTagName = pAnalog.m_szLinkedTagName;
					pPrdtInfo->m_72Hour.m_nType = pAnalog.m_nType;
				}

				m_vecTableInfo.push_back(pAnalog);
			}
		}
		else if (reader.isEndElement())
		{
			break;
		}

		reader.readNext();
	}

	return true;
}
/*! \fn void CPredictData::Init()
*********************************
** \brief    CPredictData::Init
** \details  气象信息初始化
** \param
** \return   void
** \author   GuoHaijun
** \date     2017年5月4日
** \note
**********************************/
void CPredictData::Init()
{
	m_strName = "Predict Data";
	m_vecTableInfo.append(m_4Hour);
	m_vecTableInfo.append(m_72Hour);
}
/*! \fn bool CStationData::SaveStationData(const QString& szRoot, int nOccNo)
********************************************************************
** \brief    CStationData::SaveStationData
** \details  保存厂站信息
** \param    szRoot  工程所在目录
** \return   bool
** \author   GuoHaijun
** \date     2017年5月4日
** \note
********************************************************************/
bool CStationData::SaveStationData(const QString& szRoot, CStationData* pStnData)
{	
	QString fileName = QString("%1%2").arg(m_strStationName).arg(".xml");
	QString strPath = szRoot + "/powerpredict/";
	strPath = strPath + fileName;
	QFile file(strPath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QString strTmp = QString(QObject::tr("open %1 fail!!").arg(fileName));

		return false;
	}
	QXmlStreamWriter writer(&file);
	writer.setAutoFormatting(true);
	writer.writeStartDocument();
	//修改层次结构
	//[station]
	writer.writeStartElement("station");
	writer.writeAttribute("StationID", QString("%1").arg(m_nStationID));
	writer.writeAttribute("Description", m_strDescrition);
	writer.writeAttribute("Algorithm", m_strAlgorithm);
	writer.writeAttribute("ReverseIsolationPath", m_strReverseIsolationPath);
	writer.writeAttribute("Name", m_strStationName);

	//writer.writeAttribute("Group", m_pParent->m_strGrpName);
	m_PlantValue->SavePlantData(writer);
	m_WeatherValue->SaveWeatherData(writer);
	m_PredictValue->SavePredictData(writer);
	m_Inverters->SaveInverterGrp(writer, *(pStnData->m_Inverters));
	//m_Inverters.SaveInverterGrp(writer);
	writer.writeEndElement();

	//[station]
	writer.writeEndElement();

	writer.writeEndDocument();
	return true;
}
/*! \fn bool CStationData::LoadStationData(QXmlStreamReader &reader)
********************************************************************
** \brief    CStationData::LoadStationData
** \details  加载厂站信息
** \param    reader  
** \return   bool
** \author   GuoHaijun
** \date     2017年5月12日
** \note
********************************************************************/
bool CStationData::LoadStationData(QXmlStreamReader &reader)
{
	QString strTmp1, strTmp2;

	while (!reader.atEnd())
	{
		reader.readNext();
		strTmp1 = reader.name().toString();

		if (reader.isStartElement())
		{
			strTmp2 = reader.name().toString();
			if (strTmp2 == "plant")
			{
				//LoadPlantData(reader);
;
				QString nName = reader.attributes().value("Name").toString();
				this->m_PlantValue->m_strName = nName;
				this->m_PlantValue->LoadData(reader, this->m_PlantValue);
				//m_vecPlantInfo.push_back(pPlant);
			}
			else if(strTmp2 == "w")
			{
				LoadWeatherData(reader);
			}
			else if (strTmp2 == "prdt")
			{
				LoadPredictData(reader);
			}
			else if (strTmp2 == "inverters")
			{
				LoadInvertersData(reader);
			}
		}
		else if (reader.isEndElement() && strTmp1 == "plant")
		{
			break;
		}
	}
	return true;
}
/*! \fn bool CStationData::ReadPlant(QXmlStreamReader &reader)
********************************************************************
** \brief    CStationData::ReadPlant
** \details  读取整站信息
** \param    reader
** \return   bool
** \author   GuoHaijun
** \date     2017年5月12日
** \note
********************************************************************/
bool CStationData::ReadPlant(QXmlStreamReader& reader)
{
	QString strTmp1, strTmp2;

	while (!reader.atEnd())
	{
		strTmp1 = reader.name().toString();
		if (reader.isStartElement())
		{
			strTmp2 = reader.name().toString();

			if (strTmp2 == "p")
			{
				Readp(reader);
			}
			else if (strTmp2 == "group")
			{
				
			}
			else if (strTmp2 == "channel")
			{
				//read channel next node group
				//reader.readNext();
			}
		}
		else if (reader.isEndElement())
		{
			if (strTmp1 == "channel")
			{
				return true;
			}
		}

		reader.readNext();


	}
	return true;
}
/*! \fn bool CStationData::Readp(QXmlStreamReader &reader)
********************************************************************
** \brief    CStationData::Readp
** \details  读取整站信息
** \param    reader
** \return   bool
** \author   GuoHaijun
** \date     2017年5月12日
** \note
********************************************************************/
bool CStationData::Readp(QXmlStreamReader& reader)
{
	CPlantInfo* pPlant = nullptr;
	QString strTmp,strName,strTmp2;

	while (!reader.atEnd())
	{
		strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			if (strTmp == "p")
			{
				strName = reader.attributes().value("Name").toString();
				if (strName.isEmpty() == true)
				{
					reader.readNext();
					continue;
				}

				pPlant = new CPlantInfo();
				
			}
			else if (strTmp == "data")
			{

			}
			else if (strTmp == "ADIN")
			{
				
				pPlant->m_strName = reader.attributes().value("Name").toString();

				//pPlant->LoadPlantData(reader);
			}

		}
		else if (reader.isEndElement())
		{
			strTmp2 = reader.name().toString();
			if (strTmp2 == "p")
			{
				break;
			}
		}
		reader.readNext();
	}

	return true;
}
/*! \fn bool CStationData::LoadPlantData(QXmlStreamReader& reader)
********************************************************************
** \brief    CStationData::LoadPlantData
** \details  读取整站信息
** \param    reader
** \return   bool
** \author   GuoHaijun
** \date     2017年5月15日
** \note
********************************************************************/
bool CStationData::LoadPlantData(QXmlStreamReader& reader)
{
	QString strTmp1, strTmp2;

	while (!reader.atEnd())
	{
		strTmp1 = reader.name().toString();
		if (reader.isStartElement())
		{
			strTmp2 = reader.name().toString();
			if (strTmp2 == "staticdata")
			{

			}
			else if (strTmp2 == "dynamicdata")
			{

			}


			if (strTmp2 == "p")
			{
				QString nName = reader.attributes().value("Name").toString();
				this->m_PlantValue->m_strName = nName;
				this->m_PlantValue->LoadData(reader, this->m_PlantValue);
				//m_vecPlantInfo.push_back(pPlant);
			}	
		}
		else if (reader.isEndElement() && strTmp1 == "p")
		{
			break;
		}
		reader.readNext();
	}
	return true;
}
/*! \fn bool CStationData::LoadWeatherData(QXmlStreamReader& reader)
********************************************************************
** \brief    CStationData::LoadWeatherData
** \details  读取整站信息
** \param    reader
** \return   bool
** \author   GuoHaijun
** \date     2017年5月15日
** \note
********************************************************************/
bool CStationData::LoadWeatherData(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "w")
			{
				QString nName = reader.attributes().value("Name").toString();
				this->m_WeatherValue->m_strName = nName;
				this->m_WeatherValue->LoadData(reader, this->m_WeatherValue);
				//m_vecWeatherData.push_back(pWeather);
			}
		}
		else if (reader.isEndElement() && strTmp == "w")
		{
			break;
		}
		reader.readNext();
	}
	return true;
}
/*! \fn bool CStationData::LoadPredictData(QXmlStreamReader& reader)
********************************************************************
** \brief    CStationData::LoadPredictData
** \details  读取预测信息
** \param    reader
** \return   bool
** \author   GuoHaijun
** \date     2017年5月15日
** \note
********************************************************************/
bool CStationData::LoadPredictData(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "prdt")
			{
				QString nName = reader.attributes().value("Name").toString();
				this->m_PredictValue->m_strName = nName;
				this->m_PredictValue->LoadData(reader, this->m_PredictValue);
				//m_vecPredictData.push_back(pPrdt);
			}
		}
		else if (reader.isEndElement() && strTmp == "prdt")
		{
			break;
		}
		reader.readNext();
	}
	return true;
}/*! \fn bool CStationData::LoadInvertersData(QXmlStreamReader& reader)
********************************************************************
** \brief    CStationData::LoadInvertersData
** \details  读取逆变器组信息
** \param    reader
** \return   bool
** \author   GuoHaijun
** \date     2017年5月15日
** \note
********************************************************************/
bool CStationData::LoadInvertersData(QXmlStreamReader& reader)
{
	while (!reader.atEnd())
	{
		QString strTmp = reader.name().toString();
		if (reader.isStartElement())
		{
			QString strTmp = reader.name().toString();
			if (strTmp == "inverters")
			{
				int nSixe = reader.attributes().value("Count").toInt();
				this->m_Inverters->m_nCount = nSixe;
				QString szName = reader.attributes().value("Name").toString();
				this->m_Inverters->m_strName = szName;
				this->m_Inverters->LoadInvertersGrp(reader);
				//m_vecInverterGroup.push_back(pInverters);
			}
		}
		else if (reader.isEndElement() && strTmp == "inverters")
		{
			break;
		}
		reader.readNext();
	}
	return true;
}
/*! \fn CPredictGroup::AddInverter()
****************************************
** \brief    CPredictGroup::AddInverter
** \details  添加逆变器
** \param
** \return   CInverterInfo*
** \author   GuoHaijun
** \date     2017年4月8日
** \note
*****************************************/
CInverterInfo* CInverterGroup::AddInverter()
{
	CInverterInfo* pData = new CInverterInfo;
	Q_ASSERT(pData);
	if (pData == nullptr)
	{
		return nullptr;
	}
	int nIndex = m_arrInverters.count();
	QString strName = "";

	int nDeviceIDMax = 1;
	for each (auto var in m_arrInverters)
	{
		if (var->m_nDeviceID < nDeviceIDMax)
		{
			var->m_nDeviceID = nDeviceIDMax;
		}
	}

	while (true)
	{
		strName = QString("%1%2").arg(PREDICT_INVERTER_NAME).arg(nDeviceIDMax);
		
		bool bFlag = false;
		for each (auto var in m_arrInverters)
		{
			if (var->m_szName == strName)
			{
				bFlag = true;
				break;
			}
		}

		if (bFlag)
		{
			nDeviceIDMax++;
			
			continue;
		}

		pData->m_szName = strName;
		pData->m_nID = nDeviceIDMax;
		m_arrInverters.append(pData);
		pData->m_nDeviceID = nDeviceIDMax;

		break;
	}

	return pData;
}
/*! \fn CPredictGroup::DeleteInverter(CInverterInfo *pInverter)
****************************************
** \brief    CPredictGroup::DeleteInverter
** \details  删除逆变器
** \param    pInverter
** \return   bool
** \author   GuoHaijun
** \date     2017年5月16日
** \note
*****************************************/
bool CInverterGroup::DeleteInverter(CInverterInfo *pInverter)
{
	Q_ASSERT(pInverter);
	if (pInverter == nullptr)
		return false;

	auto it_find = std::find(m_arrInverters.begin(), m_arrInverters.end(), pInverter);
	if (it_find != m_arrInverters.end())
	{
		delete pInverter;
		m_arrInverters.erase(it_find);
		return true;
	}

	delete pInverter;
	return false;
}
/*! \fn CPredictGroup::CPredictGroup()
************************************************************************
** \brief    CPredictGroup::CPredictGroup
** \details  预测组构造
** \param
** \return
** \author   GuoHaijun
** \date     2017年3月29日
** \note
***********************************************************************/
CPredictGroup::CPredictGroup()
{


}
/*! \fn CPredictGroup::~CPredictGroup()
************************************************************************
** \brief    CPredictGroup::~CPredictGroup
** \details  预测组析构
** \param
** \return
** \author   GuoHaijun
** \date     2017年3月29日
** \note
***********************************************************************/
CPredictGroup::~CPredictGroup()
{
	auto it = m_mapStations.begin();
	for (; it != m_mapStations.end(); ++it)
	{
		if (it.value() != nullptr)
		{
			delete it.value();
			it.value() = nullptr;
		}
	}
	m_mapStations.clear();
	auto iter = m_mapStrGrps.begin();
	for (; iter != m_mapStrGrps.end(); ++iter)
	{
		if (iter.value() != nullptr)
		{
			delete iter.value();
			iter.value() = nullptr;
		}
	}
	m_mapStrGrps.clear();
}
/*! \fn CPredictGroup* CPredictGroup::CreateGroup()
************************************************************************
** \brief   CPredictGroup::CreateGroup
** \details 创建组,分配内存
** \param
** \return  CPredictGroup*
** \author  GuoHaijun
** \date    2017年3月14日
** \note
***********************************************************************/
CPredictGroup* CPredictGroup::CreateGroup()
{
	int nIndex = 0;
	QString strName = "";
	while (true)
	{
		if (nIndex == 0)
		{
			strName = QString("%1%2").arg(PREDICT_GROUP_NAME).arg(nIndex);
		}
		else
		{
			strName = QString("%1%2").arg(PREDICT_GROUP_NAME).arg(nIndex);
		}
		auto it = m_mapStrGrps.find(strName);
		if (it == m_mapStrGrps.end())
		{
			auto pGroup = new CPredictGroup();
			pGroup->m_strGrpName = strName;
			m_mapStrGrps[strName] = pGroup;
			return pGroup;
		}
		nIndex++;
	}
	return nullptr;
}
/*! \fn CPredictGroup* CPredictGroup::CreateGroup(CPredictGroup* pPrdtGrp)
**************************************************************************
** \brief   CPredictGroup::CreateGroup
** \details 创建组,分配内存
** \param
** \return  CPredictGroup*
** \author  GuoHaijun
** \date    2017年3月14日
** \note
*************************************************************************/
CPredictGroup* CPredictGroup::CreateGroup(CPredictGroup* pPrdtGrp)
{
	Q_ASSERT(pPrdtGrp);
	if (pPrdtGrp == nullptr)
	{
		return nullptr;
	}
	int nIndex = 0;
	QString strName = "";
	while (true)
	{
		if (nIndex == 0)
		{
			strName = QString("%1%2").arg(PREDICT_GROUP_NAME).arg(nIndex);
		}
		else
		{
			strName = QString("%1%2").arg(PREDICT_GROUP_NAME).arg(nIndex);
		}
		auto it = m_mapStrGrps.find(strName);
		if (it == m_mapStrGrps.end())
		{
			auto pGroup = new CPredictGroup();
			pGroup->m_strGrpName = strName;
			m_mapStrGrps[strName] = pGroup;
			return pGroup;
		}
		nIndex++;
	}
	return nullptr;
}

CPredictGroup* CPredictGroup::CreateGroup(QString& grpName)
{
	auto it = m_mapStrGrps.find(grpName);
	if (it == m_mapStrGrps.end())
	{
		auto pGroup = new CPredictGroup();
		pGroup->m_strGrpName = grpName;
		m_mapStrGrps[grpName] = pGroup;
		return pGroup;
	}
	
	return nullptr;
}


/*! \fn bool CPredictGroup::DeleteGroup(CPredictGroup* pPrdtGroup, const QString& strLastTagName)
*************************************************************************************************
** \brief   CPredictGroup::DeleteGroup
** \details 删除组,释放内存
** \param   pPrdtGroup     子组指针
** \param   strLastTagName 子组名
** \return  bool
** \author  GuoHaijun
** \date    2017年3月30日
** \note
*************************************************************************************************/
bool CPredictGroup::DeleteGroup(CPredictGroup* pPrdtGroup, const QString& strLastTagName)
{
	Q_UNUSED(pPrdtGroup);

	auto iter = m_mapStrGrps.find(strLastTagName);
	if (iter == m_mapStrGrps.end())
	{
		return false;
	}

	delete iter.value();
	m_mapStrGrps.erase(iter);

	return true;
}
/*! \fn bool CPredictGroup::DeleteItem(const QString& strTagItem)
*****************************************************************
** \brief   CPredictGroup::DeleteItem
** \details 删除item
** \param   strTagItem     
** \return  bool
** \author  GuoHaijun
** \date    2017年3月30日
** \note
*****************************************************************/
bool CPredictGroup::DeleteItem(const QString& strTagItem)
{
	auto iter = m_mapStations.find(strTagItem);
	if (iter == m_mapStations.end())
	{
		return false;
	}
	delete iter.value();
	m_mapStations.erase(iter);
	return true;
}
/*! \fn bool CPredictGroup::DeleteItem(CStationData *pStation)
**************************************************************
** \brief   CPredictGroup::DeleteItem
** \details 删除item
** \param   pStation
** \return  bool
** \author  GuoHaijun
** \date    2017年3月30日
** \note
***************************************************************/
bool CPredictGroup::DeleteItem(CStationData *pStation)
{
	Q_ASSERT(pStation);
	if (pStation == nullptr)
		return false;

	auto it_find = m_mapStations.find(pStation->GetName());
	if (it_find != m_mapStations.end())
	{
		delete pStation;
		m_mapStations.erase(it_find);
		return true;
	}

	delete pStation;
	return false;
}
/*! \fn CPredictMgr::CPredictMgr()
*************************************
** \brief    CPredictMgr::CPredictMgr
** \details  预测管理构造
** \param
** \return
** \author   GuoHaijun
** \date     2017年3月29日
** \note
**************************************/
CPredictMgr::CPredictMgr()
{
	m_pRootPrdtGrp = nullptr;
	Init();
}
/*! \fn CPredictMgr::~CPredictMgr()
**************************************
** \brief    CPredictMgr::~CPredictMgr
** \details  预测管理析构
** \param
** \return 
** \author   GuoHaijun
** \date     2017年3月29日
** \note
***************************************/
CPredictMgr::~CPredictMgr()
{
	Reset();
}
/*! \fn void CPredictMgr::Init()
**************************************
** \brief    CPredictMgr::Init()
** \details  预测管理初始化
** \param
** \return
** \author   GuoHaijun
** \date     2017年3月29日
** \note
***************************************/
void CPredictMgr::Init()
{
	Q_ASSERT(m_pRootPrdtGrp == nullptr);
	m_pRootPrdtGrp = new CPredictGroup();
}
/*! \fn void CPredictMgr::Reset()
**************************************
** \brief    CPredictMgr::Reset
** \details  
** \param
** \return
** \author   GuoHaijun
** \date     2017年3月29日
** \note
***************************************/
void CPredictMgr::Reset()
{
	if (m_pRootPrdtGrp != nullptr)
	{
		delete m_pRootPrdtGrp;
		m_pRootPrdtGrp = nullptr;
	}

	m_pRootPrdtGrp = new CPredictGroup;

	m_mapRootGrpStrPrdtItem.clear();
}
/*! \fn bool CPredictMgr::LoadPredictNode(QDomElement& elm, const QString& szRoot)
**********************************************************************************
** \brief    CPredictMgr::LoadPredictNode
** \details  
** \param    elm        工程文件中预测子节点
** \param    szRoot     工程文件所在目录路径
** \return   bool
** \author   GuoHaijun
** \date     2017年5月15日
** \note
**********************************************************************************/
bool CPredictMgr::LoadPredictNode(QDomElement& elm, const QString& szRoot)
{
	QDomNode node = elm.firstChild();

	while (!node.isNull())
	{
		QDomElement e = node.toElement();

		auto strTmp = e.tagName();
		if (strTmp.compare("group") == 0)
		{
			QString strGroupName = e.attribute("name");
			if (strGroupName.isEmpty() == false)
			{
				CPredictGroup *pPrdtGroup = new CPredictGroup();
				pPrdtGroup->m_strGrpName = strGroupName;
				LoadChildGroup(pPrdtGroup, e, szRoot);
				m_pRootPrdtGrp->m_mapStrGrps.insert(strGroupName, pPrdtGroup);
			}
			else
			{

			}
		}
		else if (strTmp.compare("file") == 0)
		{
			QString strTagName = e.attribute("name");
			CStationData* pStn = new CStationData(m_pRootPrdtGrp);
			pStn->m_strStationName = strTagName;
			m_pRootPrdtGrp->m_mapStations.insert(strTagName, pStn);
			
			LoadPredictFile(pStn, pStn->m_strStationName, szRoot);
		}
		node = node.nextSibling();
	}

	return true;
}
/*! \fn bool CPredictMgr::LoadPredictFile(CStationData* pStnData, const QString& fileName, const QString& szRoot)
**********************************************************************************
** \brief    CPredictMgr::LoadPredictFile
** \details
** \param    pStnData   预测厂站指针
** \param    fileName   预测xml文件名
** \param    szRoot     工程文件所在目录路径
** \return   bool
** \author   GuoHaijun
** \date     2017年5月15日
** \note
**********************************************************************************/
bool CPredictMgr::LoadPredictFile(CStationData* pStnData, const QString& szFileName, const QString& szRoot)
{
	Q_ASSERT(pStnData);
	if (pStnData == nullptr)
		return false;

	QXmlStreamReader xml;
	QString strRoot = szRoot;
	strRoot += "/powerpredict";
	QString strTmp = strRoot + "/" + szFileName + ".xml";
	QFile file(strTmp);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		return false;
	}
	xml.setDevice(&file);

	if (xml.readNextStartElement())
	{
		QString strTmp = xml.name().toString();
		if (strTmp == "plant")
		{
			//QString strDesc = xml.attributes().value("Description").toString();
			//pStnData->m_strStationName = strDesc;
			//pStnData->LoadStationData(xml);

			////m_arrPrdtDatas.push_back(pStnData);
			//m_mapRootGrpStrPrdtItem.insert(pStnData->m_strStationName, pStnData);
		}
		if (strTmp == "station")
		{
			pStnData->m_nStationID = xml.attributes().value("StationID").toInt();
			pStnData->m_strStationName = xml.attributes().value("Name").toString();
			QString strDesc = xml.attributes().value("Description").toString();
			pStnData->m_strDescrition = strDesc;

			pStnData->m_strAlgorithm = xml.attributes().value("Algorithm").toString();
			pStnData->m_strReverseIsolationPath = xml.attributes().value("ReverseIsolationPath").toString();

			pStnData->LoadStationData(xml);

			//m_arrPrdtDatas.push_back(pStnData);
			m_mapRootGrpStrPrdtItem.insert(pStnData->m_strStationName, pStnData);
		}
	}
	file.close();

	return true;
}
/*! \fn ool CPredictMgr::LoadChildGroup(CPredictGroup* pPrdtGrp, QDomElement& elm, const QString& szRoot)
************************************************************************
** \brief    CPredictMgr::LoadChildGroup
** \details  加载组项
** \param    pPrdtGrp
** \param    elm
** \param    szRoot
** \return
** \author   GuoHaijun
** \date     2017年5月16日
** \note
***********************************************************************/
bool CPredictMgr::LoadChildGroup(CPredictGroup* pPrdtGrp, QDomElement& elm, const QString& szRoot)
{
	QDomNode n = elm.firstChild();

	while(!n.isNull())
	{
		QDomElement e = n.toElement();

		auto strTmp = e.tagName();
		if (strTmp.compare("group") == 0)
		{
			QString strGroupName = e.attribute("name");
			CPredictGroup* pPrdtChildGrp = new CPredictGroup();
			pPrdtChildGrp->m_strGrpName = strGroupName;
			pPrdtGrp->m_mapStrGrps.insert(strGroupName, pPrdtChildGrp);
			LoadChildGroup(pPrdtChildGrp, e, szRoot);
		}
		else if (strTmp.compare("file") == 0)
		{
			QString strTagName = e.attribute("name");
			CStationData* pStn = new CStationData(pPrdtGrp);
			pStn->m_strStationName = strTagName;
			pPrdtGrp->m_mapStations.insert(strTagName, pStn);			
			LoadPredictFile(pStn, pStn->m_strStationName, szRoot);
		}
		n = n.nextSibling();
	}


	return true;
}
/*! \fn bool CPredictMgr::DeleteHashPredictItem(const QString& strPredictTag)
************************************************************************
** \brief    CPredictMgr::DeleteHashPredictItem
** \details  删除预测项hash
** \param    strPredictTag
** \return
** \author   GuoHaijun
** \date     2017年3月30日
** \note
***********************************************************************/
bool CPredictMgr::DeleteHashPredictItem(const QString& strPredictTag)
{
	auto iter = m_mapRootGrpStrPrdtItem.find(strPredictTag);
	if (iter != m_mapRootGrpStrPrdtItem.end())
	{
		m_mapRootGrpStrPrdtItem.erase(iter);
		return true;
	}
	return false;
}
/*! \fn CPredictItem* CPredictMgr::AddNewPredictItem(CPredictGroup* pPredictGroup)
**********************************************************************************
** \brief    CPredictMgr::AddNewPredictItem
** \details  新建预测厂站，分配内存
** \param    pPredictGroup
** \return   CPredictItem*
** \author   GuoHaijun
** \date     2017年3月30日
** \note
**********************************************************************************/
CStationData* CPredictMgr::CreateNewPredictItem(CPredictGroup* pPredictGroup)
{
	Q_ASSERT(pPredictGroup);
	if (pPredictGroup == nullptr)
	{
		return nullptr;
	}
	CStationData* pData = new CStationData(pPredictGroup);
	int nIndex = 0;
	QString strName = "";
	while (true)
	{
		if (nIndex == 0)
		{
			strName = QString("%1%2").arg(PREDICT_ITEM_NAME).arg(nIndex);
		}
		else
		{
			strName = QString("%1%2").arg(PREDICT_ITEM_NAME).arg(nIndex);
		}
		auto iter = m_mapRootGrpStrPrdtItem.find(strName);
		if (iter == m_mapRootGrpStrPrdtItem.end())
		{
			m_mapRootGrpStrPrdtItem[strName] = pData;
			pData->SetName(strName);
			pPredictGroup->m_mapStations.insert(strName, pData);
			//pData->CreateEmptyStation();
			return pData;
		}
		else
		{
			nIndex++;
			if (nIndex > MAX_STATION_NUM)
			{
				delete pData;
				return nullptr;
			}
		}
	}
	return pData;
}
/*! \fn bool CPredictMgr::SaveProjectPredictNode(QDomDocument* pXml, QDomElement& pElement, const QString& szRoot)
******************************************************************************************************************
** \brief    CPredictMgr::SaveProjectPredictNode
** \details  
** \param    pXml
** \param    pElement
** \param    szRoot
** \return   bool
** \author   GuoHaijun
** \date     2017年5月2日
** \note
*****************************************************************************************************************/
bool CPredictMgr::SaveProjectPredictNode(QDomDocument* pXml, QDomElement& pElement, const QString& szRoot)
{
	Q_ASSERT(pXml);
	if (pXml == nullptr)
	{
		return false;
	}
	/*QDomElement rElm = pXml->createElement("powerpredict");
	rElm.setAttribute("name", "powerpredict");
	pElement.appendChild(rElm);*/
	SaveChildNode(pXml, pElement, m_pRootPrdtGrp);
	SaveChildItem(m_pRootPrdtGrp, szRoot);
	return true;
}
/*! \fn bool CPredictMgr::SaveChildNode(QDomDocument* pXml, QDomElement& pElement, CPredictGroup* pPrdtGroup)
******************************************************************************************************************
** \brief    CPredictMgr::SaveChildNode
** \details
** \param    pXml
** \param    pElement
** \param    szRoot
** \return   bool
** \author   GuoHaijun
** \date     2017年5月2日
** \note
*****************************************************************************************************************/
bool CPredictMgr::SaveChildNode(QDomDocument* pXml, QDomElement& pElement, CPredictGroup* pPrdtGroup)
{
	Q_ASSERT(pXml);
	Q_ASSERT(pPrdtGroup);
	if (pXml == nullptr || pPrdtGroup == nullptr)
	{
		return false;
	}
	auto it = pPrdtGroup->m_mapStrGrps.begin();
	for (; it != pPrdtGroup->m_mapStrGrps.end(); ++it)
	{
		QDomElement cElet = pXml->createElement("group");
		cElet.setAttribute("name", it.value()->m_strGrpName);
		pElement.appendChild(cElet);
		SaveChildNode(pXml, cElet, it.value());
	}
	auto iter = pPrdtGroup->m_mapStations.begin();
	for (; iter != pPrdtGroup->m_mapStations.end(); ++iter)
	{
		QDomElement cElet = pXml->createElement("file");
		auto strFile = QString("%1.xml").arg(iter.value()->m_strStationName);
		cElet.setAttribute("name", iter.value()->m_strStationName);
		pElement.appendChild(cElet);
	}
	return true;
}
/*! \fn bool CPredictMgr::SaveChildItem(CPredictGroup* pPrdtGroup, const QString & szRoot)
******************************************************************************************************************
** \brief    CPredictMgr::SaveChildItem
** \details
** \param    pPrdtGroup
** \param    szRoot
** \return   bool
** \author   GuoHaijun
** \date     2017年5月2日
** \note
*****************************************************************************************************************/
bool CPredictMgr::SaveChildItem(CPredictGroup* pPrdtGroup, const QString & szRoot)
{
	auto it = pPrdtGroup->m_mapStations.begin();
	for (; it != pPrdtGroup->m_mapStations.end(); ++it)
	{
		it.value()->SaveStationData(szRoot, it.value());
	}
	auto ite = pPrdtGroup->m_mapStrGrps.begin();
	for (; ite != pPrdtGroup->m_mapStrGrps.end(); ++ite)
	{
		SaveChildItem(ite.value(), szRoot);
	}

	return true;
}
/*! \fn CPredictItem::CPredictItem()
****************************************
** \brief    CPredictItem::CPredictItem
** \details  预测项构造
** \param
** \return
** \author   GuoHaijun
** \date     2017年3月29日
** \note
*****************************************/
CStationData::CStationData()
{
	// 	m_vecStnData.clear();
	// 	m_vecIntData.clear();
	// 	m_vecWeaData.clear();
	// 	m_vecPrdtData.clear();
	m_pParent = nullptr;
}
/*! \fn CPredictItem::CPredictItem()
*****************************************
** \brief    CPredictItem::~CPredictItem
** \details  预测项析构
** \param
** \return
** \author   GuoHaijun
** \date     2017年3月29日
** \note
*****************************************/
CStationData::~CStationData()
{
#if 0
	if (m_vecStnData[0] != nullptr)
	{
		delete m_vecStnData[0];
		m_vecStnData[0] = nullptr;
		m_vecStnData.clear();
	}

	if (m_vecIntData[0] != nullptr)
	{
		delete m_vecIntData[0];
		m_vecIntData[0] = nullptr;
		m_vecIntData.clear();
	}

	if (m_vecWeaData[0] != nullptr)
	{
		delete m_vecWeaData[0];
		m_vecWeaData[0] = nullptr;
		m_vecWeaData.clear();
	}

	if (m_vecPrdtData[0] != nullptr)
	{
		delete m_vecPrdtData[0];
		m_vecPrdtData[0] = nullptr;
		m_vecPrdtData.clear();
	}
#endif

	if (m_Inverters)
	{
		delete m_Inverters;
		m_Inverters = nullptr;
	}

	if (m_WeatherValue)
	{
		delete m_WeatherValue;
		m_WeatherValue = nullptr;
	}

	if (m_PlantValue)
	{
		delete m_PlantValue;
		m_PlantValue = nullptr;
	}

	if (m_PredictValue)
	{
		delete m_PredictValue;
		m_PredictValue = nullptr;
	}

}

#if 0

/*! \fn CStationData* CPredictItem::AddStationData()
************************************************************************
** \brief    CPredictItem::AddStationData
** \details  整站数据，分配内存
** \param
** \return   CStationData*
** \author   GuoHaijun
** \date     2017年3月31日
** \note
***********************************************************************/
CPlantInfo* CStationData::AddPlantData()
{
	CPlantInfo* pStnData = new CPlantInfo;
	Q_ASSERT(pStnData);
	if (pStnData == nullptr)
	{
		return nullptr;
	}

	m_vecStnData.push_back(pStnData);

	return pStnData;

}
/*! \fn CInverterData* CPredictItem::AddInverterData()
************************************************************************
** \brief    CPredictItem::AddInverterData
** \details  逆变器数据，分配内存
** \param
** \return   CInverterData*
** \author   GuoHaijun
** \date     2017年3月31日
** \note
***********************************************************************/
CInverterGroup* CStationData::AddInverterData()
{
	CInverterGroup* pInvData = new CInverterGroup;
	Q_ASSERT(pInvData);
	if (pInvData == nullptr)
	{
		return nullptr;
	}

	m_vecIntData.push_back(pInvData);

	return pInvData;
}
/*! \fn CWeatherData* CPredictItem::AddWeatherData()
************************************************************************
** \brief    CPredictItem::AddWeatherData
** \details  气象仪数据，分配内存
** \param
** \return   CWeatherData*
** \author   GuoHaijun
** \date     2017年3月31日
** \note
***********************************************************************/
CWeatherData* CStationData::AddWeatherData()
{
	CWeatherData* pWeaData = new CWeatherData;
	Q_ASSERT(pWeaData);
	if (pWeaData == nullptr)
	{
		return nullptr;
	}

	m_vecWeaData.push_back(pWeaData);

	return pWeaData;

}
/*! \fn CPredictData* CPredictItem::AddPredictData()
************************************************************************
** \brief    CPredictItem::AddPredictData
** \details  预测数据，分配内存
** \param
** \return   CPredictData*
** \author   GuoHaijun
** \date     2017年3月31日
** \note
***********************************************************************/
CPredictData* CStationData::AddPredictData()
{
	CPredictData* pPrdtData = new CPredictData;
	Q_ASSERT(pPrdtData);
	if (pPrdtData == nullptr)
	{
		return nullptr;
	}

	m_vecPrdtData.push_back(pPrdtData);

	return pPrdtData;
}

#endif