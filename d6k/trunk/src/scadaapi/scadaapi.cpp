#include "scadaapi.h"
#include "../scdsvc/data_def.h"
#include "fesapi/fes_magicmem.h"
#include "../scdsvc/data_def.h"
#include "scadaapi/scdapp_def.h"
#include "mail/mail.h"
#include "log/log.h"
#include "tagname_pool.h"
#include "server_db.h"
#include "client_db.h"

#include <QtGlobal>
#include <QString>

CScadaApi::CScadaApi()
{
	m_nRefCount = 0;
	m_pMem = std::make_shared<CShareMem>();
	m_pDataMem = std::make_shared<CShareMem>();
	m_pScdMem = std::make_shared<CShareMem>();
	m_pTagAttr = std::make_shared<CTagAttMgr>();

	m_arrGetSAppRTDataFuncs[ATT_STATE] = std::bind(&CScadaApi::GetAppState, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	m_arrGetSAppRTDataFuncs[ATT_QUA] = std::bind(&CScadaApi::GetAppQuality, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}
CScadaApi::~CScadaApi()
{

}

bool CScadaApi::Initialize(const char *pszDataPath, const char * szAppName, unsigned int nMode)
{
	Q_ASSERT(pszDataPath && strlen(pszDataPath) > 0);
	if (!pszDataPath || strlen(pszDataPath) <= 0)
	{
		return false;
	}
	// 如果初始化过，则不再初始化
	if (m_nRefCount > 0)
	{
		m_nRefCount++;
		return true;
	}
	else
	{
		m_nRefCount++;
	}

	if (!BuildTagNamePool(pszDataPath))
	{
		return false;
	}
	if (!BuildFesDB(pszDataPath))
	{
		return false;
	}
	if (!CreateScdAndClientDB(pszDataPath))
	{
		return false;
	}

	bool bRet = OpenPostOffice("SCADA");

	int nId = 0;

	bRet = OpenMailBox("SCADA", szAppName, &nId);

	return true;
}

void CScadaApi::Run()
{

}

void CScadaApi::Shutdown()
{
	m_nRefCount--;

	if (m_nRefCount <= 0)
	{
		auto iter = m_MapTagName.begin();

		for (; iter != m_MapTagName.end(); ++iter)
		{
			iter->second->Shutdown();
		}
	}
}
/*! \fn bool CScadaApi::GetTagNameByOccNo(int32u nOccNo, std::string& tagName)
*********************************************************************************************************
** \brief CScadaApi::GetTagNameByOccNo
** \details 根据节点occno 获取节点tagname
** \param nOccNo
** \param tagName
** \return bool
** \author xingzhibing
** \date 2017年2月8日
** \note
********************************************************************************************************/
bool CScadaApi::GetNodeTagNameByOccNo(int32u nOccNo, std::string& tagName)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_NODE_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_NODE_OCCNO)
	{
		return false;
	}
	auto iter = m_MapString2ID.begin();
	for (; iter != m_MapString2ID.end(); ++iter)
	{
		if (iter->second == nOccNo)
		{
			tagName = iter->first;
			return true;
		}
	}
	return false;
}
/*! \fn int32u CScadaApi::GetNodeOccNoByTagName(const std::string& tagName)
*********************************************************************************************************
** \brief CScadaApi::GetOccNoByTagName
** \details 根据节点tagname 获取occno
** \param tagName
** \return int32u
** \author xingzhibing
** \date 2017年2月8日
** \note
********************************************************************************************************/
int32u CScadaApi::GetNodeOccNoByTagName(const std::string& tagName)const
{
	Q_ASSERT(!tagName.empty() && tagName.length() > 0);
	if (tagName.empty() || tagName.length() <= 0)
	{
		return INVALID_OCCNO;
	}

	auto iter = m_MapString2ID.find(tagName);
	if (iter == m_MapString2ID.end())
	{
		return INVALID_OCCNO;
	}
	else
	{
		return iter->second;
	}
}

bool CScadaApi::GetOccNoByTagName(int32u nNodeOccNo, const std::string& tagName, const std::string& szAttr, int32u* pOccNo, int32u* pIddType, int32u *pFiledID)const
{
	Q_ASSERT(nNodeOccNo != INVALID_OCCNO && nNodeOccNo <= MAX_NODE_OCCNO);
	if (nNodeOccNo == INVALID_OCCNO || nNodeOccNo > MAX_NODE_OCCNO)
	{
		return false;
	}

	Q_ASSERT(pOccNo && pIddType && pFiledID);
	if (pOccNo == nullptr || pIddType == nullptr || pFiledID == nullptr)
	{
		return false;
	}

	Q_ASSERT(!tagName.empty() && tagName.length() > 0);
	if (tagName.empty() || tagName.length() == 0)
	{
		return false;
	}
	// fixed by LiJin 2017.9.14, 考虑到部分TagName可能不需要属性
#if 0
	Q_ASSERT(!szAttr.empty() && szAttr.length() > 0);
	if (szAttr.empty() || szAttr.length() == 0)
	{
		return false;
	}
#endif

	std::shared_ptr<CTagNamePool> pTagNamePool = nullptr;

	auto iter = m_MapTagName.find(nNodeOccNo);
	if (iter == m_MapTagName.end())
	{
		return false;
	}
	else
	{
		pTagNamePool = iter->second;
	}
	Q_ASSERT(pTagNamePool);

	bool bRet = pTagNamePool->GetOccNoByTagName(tagName.c_str(), *pIddType, *pOccNo, *pOccNo);

	if (!bRet)
	{
		//todo log
		return false;
	}
	if (szAttr.length())
	{
		bRet = m_pTagAttr->GetAttIndex(*pIddType, szAttr.c_str(), *pFiledID);
		if (!bRet)
		{
			//todo log
			return false;
		}
	}	

	return true;
}

void CScadaApi::LogMsg(const char * pszText, int nLevel)
{
	::LogMsg("scddaapi", pszText, nLevel, nullptr);
}
/*! \fn bool CScadaApi::GetOccNoByTagName(const char *pszTagName, int32u nNodeOccNo, int32u &nIddType, int32u &nOccNo)const
********************************************************************************************************* 
** \brief CScadaApi::GetOccNoByTagName 
** \details 根据tagname或者occno，最初的需求是根据app的tagname，获取他的occno
** \param pszTagName 
** \param nNodeOccNo 
** \param nIddType 
** \param nOccNo 
** \return bool 
** \author LiJin 
** \date 2017年9月14日 
** \note 没有推敲清楚...... 需要仔细再琢磨
********************************************************************************************************/
bool CScadaApi::GetOccNoByTagName(const char *pszTagName, int32u nNodeOccNo, int32u &nIddType, int32u &nOccNo)const
{
	Q_ASSERT(pszTagName && strlen(pszTagName) > 0);
	if (pszTagName == nullptr || strlen(pszTagName) == 0)
	{
		return false;
	}
	int32u nFiledID = 0;
	return GetOccNoByTagName(nNodeOccNo,
		pszTagName,
		"",
		&nOccNo,
		&nIddType,
		&nFiledID); 
}

bool CScadaApi::GetOccNoByTagName(const char*pszTagName, int32u *pNodeOccNo, int32u *pIddType, int32u *pOccNo, int32u *pFiledID)const
{
	Q_ASSERT(pszTagName && strlen(pszTagName) > 0);
	if (!pszTagName || strlen(pszTagName) == 0)
	{
		return false;
	}
	QStringList szTagNameList = QString(pszTagName).split(".");

	if (szTagNameList.size() == 0)
	{
		//todo log
		return false;
	}

	//包含系统变量或者前置变量,暂时按照$或者结构数目区分
//	if (szTagNameList.size()==2 || QString(pszTagName).contains('$'))
//	{
	QString szNodeTagName = szTagNameList.at(0);
	int32u nNodeOccNo = GetNodeOccNoByTagName(szNodeTagName.toStdString());
	Q_ASSERT(nNodeOccNo != INVALID_OCCNO && nNodeOccNo <= MAX_NODE_OCCNO);
	if (nNodeOccNo == INVALID_OCCNO || nNodeOccNo > MAX_NODE_OCCNO)
	{
		//todo log
		return false;
	}
	*pNodeOccNo = nNodeOccNo;

	bool bRet = GetOccNoByTagName(nNodeOccNo,
		szTagNameList.at(1).toStdString().c_str(),
		szTagNameList.at(2).toStdString().c_str(),
		pOccNo,
		pIddType,
		pFiledID);
	if (!bRet)
	{
		return false;
	}
	//	}
	//	else if (szTagNameList.size()==3)
	/*
		{
			QString szNodeTagName = szTagNameList.at(0);

			int32u nNodeOccNo = GetNodeOccNoByTagName(szNodeTagName.toStdString());
			Q_ASSERT(nNodeOccNo!=INVALID_OCCNO && nNodeOccNo <=MAX_NODE_OCCNO);
			if (nNodeOccNo ==INVALID_OCCNO || nNodeOccNo >MAX_NODE_OCCNO)
			{
				//todo log
				return false;
			}
			*pNodeOccNo = nNodeOccNo;

			bool bRet = GetIOOccNoByTagName(nNodeOccNo,
									  szTagNameList.at(1).toStdString().c_str(),
									  szTagNameList.at(2).toStdString().c_str(),
										pOccNo,
										pIddType,
										pFiledID);
			if (!bRet)
			{
				return false;
			}
		}*/

	return true;
}

bool CScadaApi::GetRTData(int32u nIddType, int32u nNodeOccNo, int32u nOccNo, int32u nFiledID, IO_VARIANT *pRetData) 
{
	Q_ASSERT(nIddType != IDD_NULL  && nIddType <= MAX_IDD);
	if (nIddType == IDD_NULL || nIddType > MAX_IDD)
	{
		return false;
	}
	Q_ASSERT(nNodeOccNo != INVALID_OCCNO && nNodeOccNo <= MAX_NODE_OCCNO);
	if (nNodeOccNo == INVALID_OCCNO || nNodeOccNo > MAX_NODE_OCCNO)
	{
		return false;
	}
	Q_ASSERT(nOccNo != INVALID_OCCNO && nNodeOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
	{
		return false;
	}

	switch (nIddType)
	{
		case IDD_AIN:
		case IDD_DIN:
		case IDD_AOUT:
		case IDD_DOUT:
		case IDD_USERVAR:
		case IDD_SYSVAR:
		{
			NODE_TYPE nType = GetNodeType(nNodeOccNo);

			bool bRet = false;

			switch (nType)
			{
				case NODE_NULL:
				{
					return false;
				}
				case NODE_SVR:
				{
					CServerDB* pServer = GetScadaDbByOccNo(nNodeOccNo);

					Q_ASSERT(pServer);
					if (!pServer)
					{
						return false;
					}

					bRet = pServer->GetRTData(nIddType, nOccNo, nFiledID, *pRetData);
					break;
				}
				case NODE_FES:
				{
					CFesDB* pFes = GetFesDbByOccNo(nNodeOccNo);

					Q_ASSERT(pFes);
					if (!pFes)
					{
						return false;
					}

					bRet = pFes->GetRTData(nIddType, nOccNo, nFiledID, *pRetData);

					break;
				}
				case NODE_CLIENT:
				{
					CClientDB* pClient = GetClientDbByOccNo(nNodeOccNo);

					Q_ASSERT(pClient);
					if (!pClient)
					{
						return false;
					}

					bRet = pClient->GetRTData(nIddType, nOccNo, nFiledID, *pRetData);
					break;
				}
				default:
				{
					Q_ASSERT(false);
					return false;
				}
			}

			Q_ASSERT(bRet);
			if (!bRet)
			{
				return false;
			}
			return true;
		}
		case IDD_FAPP:
		case IDD_SAPP:
		{
			Q_ASSERT(m_arrGetSAppRTDataFuncs[nFiledID]);
			if (m_arrGetSAppRTDataFuncs[nFiledID])
			{
				m_arrGetSAppRTDataFuncs[nFiledID](nNodeOccNo, nOccNo, *pRetData);
			}
		}
		default:
			break;
	}
	return true;
}

bool CScadaApi::PutRTData(int32u nIddType, int32u nNodeOccNo, int32u nOccNo, int32u nFiledID, IO_VARIANT *pData, const char * pszAppTagName, void *pExt)
{
	Q_ASSERT(nIddType != IDD_NULL  && nIddType <= MAX_IDD);
	if (nIddType == IDD_NULL || nIddType > MAX_IDD)
	{
		return false;
	}
	Q_ASSERT(nNodeOccNo != INVALID_OCCNO && nNodeOccNo <= MAX_NODE_OCCNO);
	if (nNodeOccNo == INVALID_OCCNO || nNodeOccNo > MAX_NODE_OCCNO)
	{
		return false;
	}
	Q_ASSERT(nOccNo != INVALID_OCCNO && nNodeOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
	{
		return false;
	}

	// 先找到是哪个节点，是什么节点类型
	NODE_TYPE nType = GetNodeType(nNodeOccNo);

	bool bRet = false;

	switch (nType)
	{
		case NODE_NULL:
		{
			Q_ASSERT(false);
			return false;
		}
		case NODE_SVR:
		{
			CServerDB* pServer = GetScadaDbByOccNo(nNodeOccNo);

			Q_ASSERT(pServer);
			if (!pServer)
			{
				return false;
			}

			bRet = pServer->PutRtData(nIddType, nOccNo, nFiledID, pData, pszAppTagName,pExt);
			break;
		}
		case NODE_FES:
		{
			CFesDB* pFes = GetFesDbByOccNo(nNodeOccNo);

			Q_ASSERT(pFes);
			if (!pFes)
			{
				return false;
			}

			bRet = pFes->PutRtData(nIddType, nOccNo, nFiledID, pData, pszAppTagName, pExt);

			break;
		}
		case NODE_CLIENT:
		{
			CClientDB* pClient = GetClientDbByOccNo(nNodeOccNo);

			Q_ASSERT(pClient);
			if (!pClient)
			{
				return false;
			}

			bRet = pClient->PutRtData(nIddType, nOccNo, nFiledID, pData, pszAppTagName, pExt);
			break;
		}
		default:
		{
			Q_ASSERT(false);
			return false;
		}
	}

	Q_ASSERT(bRet);
	if (!bRet)
	{
		return false;
	}
	return true;
}

NODE_TYPE CScadaApi::GetNodeType(int32u nNodeOccNo) const
{
	Q_ASSERT(nNodeOccNo != INVALID_OCCNO && nNodeOccNo <= MAX_NODE_OCCNO);
	if (nNodeOccNo == INVALID_OCCNO || nNodeOccNo > MAX_OCCNO)
	{
		return NODE_NULL;
	}

	Q_ASSERT(m_mapType.empty() == false);
	if (m_mapType.empty())
	{
		return NODE_NULL;
	}
	auto iter = m_mapType.find(nNodeOccNo);
	if (iter == m_mapType.end())
	{
		return NODE_NULL;
	}
	else
	{
		return iter->second;
	}
}
/*! \fn NODE_STATE  CScadaApi::GetNodeHostState(int32u nNodeOccNo) const
*********************************************************************************************************
** \brief CScadaApi::GetNodeHostState
** \details 获取每个节点的工作状态
** \param nNodeOccNo
** \return NODE_STATE
** \author LiJin
** \date 2017年9月8日
** \note
********************************************************************************************************/
NODE_STATE  CScadaApi::GetNodeHostState(int32u nNodeOccNo) const
{
	Q_ASSERT(nNodeOccNo != INVALID_OCCNO && nNodeOccNo <= MAX_NODE_OCCNO);
	if (nNodeOccNo == INVALID_OCCNO || nNodeOccNo > MAX_OCCNO)
	{
		return STATE_UNKOWN;
	}

	Q_ASSERT(m_pNodes);
	if (m_pNodes == nullptr)
		return STATE_UNKOWN;

	if (m_pNodes[nNodeOccNo - 1].State == STATE_MAIN)
	{
		return STATE_MAIN;
	}
	else if (m_pNodes[nNodeOccNo - 1].State == STATE_SLAVE)
	{
		return STATE_SLAVE;
	}
	else
	{
		return STATE_UNKOWN;
	}
}

SAPP* CScadaApi::GetNodeAppInfoByName(int32u nNodeOccNo, const char* pszAppName)
{
	Q_ASSERT(nNodeOccNo != INVALID_OCCNO && nNodeOccNo <= MAX_NODE_OCCNO);
	if (nNodeOccNo == INVALID_OCCNO || nNodeOccNo > MAX_NODE_OCCNO)
	{
		return nullptr;
	}
	Q_ASSERT(pszAppName && strlen(pszAppName) > 0);
	if (!pszAppName || strlen(pszAppName) == 0)
	{
		return nullptr;
	}
	//未找到对应节点
	auto iter = m_arrAppInfos.find(nNodeOccNo);
	if (iter == m_arrAppInfos.end())
	{
		return nullptr;
	}
	for (auto it : iter->second)
	{
		if (strcmp(pszAppName, it->ProgramName) == 0)
		{
			return it;
		}
	}
	//未找到对应内存
	return nullptr;
}

bool CScadaApi::GetNodeAppInfoByOccNo(int32u nNodeOccNo, int32u nProOccNo, SAPP** pApp)  const
{
	Q_ASSERT(nNodeOccNo != INVALID_OCCNO && nNodeOccNo <= MAX_NODE_OCCNO);
	if (nNodeOccNo == INVALID_OCCNO || nNodeOccNo > MAX_NODE_OCCNO)
	{
		return false;
	}
	Q_ASSERT(nProOccNo != INVALID_OCCNO &&  nProOccNo <= MAX_OCCNO);
	if (nProOccNo == INVALID_OCCNO || nProOccNo > MAX_OCCNO)
	{
		return false;
	}
	//未找到对应节点
	auto iter = m_arrAppInfos.find(nNodeOccNo);

	if (iter == m_arrAppInfos.end())
	{
		return false;
	}
	for (auto it : iter->second)
	{
		if (it->OccNo == nProOccNo)
		{
			*pApp = it;
			return true;
		}
	}
	//未找到对应内存
	return false;
}

bool CScadaApi::GetAppState(int32u nNodeOccNo, int32u nOccNo, IO_VARIANT& pData) const
{
	Q_ASSERT(nNodeOccNo != INVALID_OCCNO && nNodeOccNo <= MAX_NODE_OCCNO);
	if (nNodeOccNo == INVALID_OCCNO || nNodeOccNo > MAX_NODE_OCCNO)
	{
		return false;
	}
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
	{
		return false;
	}
	SAPP* pApp = nullptr;
	bool bRet = GetNodeAppInfoByOccNo(nNodeOccNo, nOccNo, &pApp);

	Q_ASSERT(pApp);
	if (!pApp)
	{
		return false;
	}

	S_CHAR(&pData, &pApp->State);

	return true;
}

bool CScadaApi::GetAppQuality(int32u nNodeOccNo, int32u nOccNo, IO_VARIANT& pData)  const
{
	Q_ASSERT(nNodeOccNo != INVALID_OCCNO && nNodeOccNo <= MAX_NODE_OCCNO);
	if (nNodeOccNo == INVALID_OCCNO || nNodeOccNo > MAX_NODE_OCCNO)
	{
		return false;
	}

	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
	{
		return false;
	}

	SAPP* pApp = nullptr;

	bool bRet = GetNodeAppInfoByOccNo(nNodeOccNo, nOccNo, &pApp);

	Q_ASSERT(pApp);
	if (!pApp)
	{
		return false;
	}

	S_CHAR(&pData, &pApp->Quality);

	return true;
}

bool CScadaApi::GetAppHeatbeat(int32u nNodeOccNo, int32u nOccNo, IO_VARIANT& pData)  const
{
	return true;
}

/*
bool CScadaApi::GetDinValue(int32u nNodeOccNo, int32u nOccNo, CVariant & val, int8u &nQuality)
{
	Q_ASSERT(nNodeOccNo != INVALID_OCCNO && nNodeOccNo <= MAX_NODE_OCCNO);
	if (nNodeOccNo == INVALID_OCCNO || nNodeOccNo > MAX_NODE_OCCNO)
	{
		return false;
	}
	Q_ASSERT(nOccNo != INVALID_OCCNO && nNodeOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
	{
		return false;
	}
	CFesDB* pFes = GetFesDBByOccNO(nNodeOccNo);
	Q_ASSERT(pFes);
	if (!pFes)
	{
		return false;
	}
	bool bRet = pFes->GetAinValue(nOccNo, val, nQuality);

	return  bRet;
}

bool CScadaApi::GetAinValue(int32u nNodeOccNo, int32u nOccNo, CVariant & val, int8u &nQuality)
{
	Q_ASSERT(nNodeOccNo != INVALID_OCCNO && nNodeOccNo <= MAX_NODE_OCCNO);
	if (nNodeOccNo == INVALID_OCCNO || nNodeOccNo > MAX_NODE_OCCNO)
	{
		return false;
	}
	Q_ASSERT(nOccNo != INVALID_OCCNO && nNodeOccNo <= MAX_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_OCCNO)
	{
		return false;
	}
	CFesDB* pFes = GetFesDBByOccNO(nNodeOccNo);
	Q_ASSERT(pFes);
	if (!pFes)
	{
		return false;
	}
	bool bRet = pFes->GetAinValue(nOccNo, val, nQuality);

	return bRet;
}
*/
/*! \fn bool CScadaApi::BuildTagNamePool(const char* pszFilePath)
*********************************************************************************************************
** \brief CScadaApi::BuildTagNamePool
** \details 读取tagnamepool内存池数据
** \param pszFilePath
** \return bool
** \author xingzhibing
** \date 2017年2月8日
** \note
********************************************************************************************************/
bool CScadaApi::BuildTagNamePool(const char* pszFilePath)
{
	Q_ASSERT(pszFilePath && strlen(pszFilePath) > 0);
	if (!pszFilePath || strlen(pszFilePath) <= 0)
	{
		return false;
	}

	QString szName = QString(pszFilePath) + "-scada-tagname";
	char* pData = (char*)(m_pDataMem->GetShareMem(szName.toStdString().c_str()));

	Q_ASSERT(pData);
	if (!pData)
	{
		return false;
	}

	m_pMagicHead = (NODE_SCD_MAGIC*)(pData);
	Q_ASSERT(m_pMagicHead->MagicNum1 == MAGIC_HEAD && m_pMagicHead->MagicNum2 == MAGIC_HEAD);
	m_nNodeFesCount = m_pMagicHead->NodeFesCount;
	m_nNodeScdCount = m_pMagicHead->NodeServerCount;
	m_nNodeClientCount = m_pMagicHead->NodeClientCount;

	m_nTotalNodeCount = m_pMagicHead->AllNodeCount;


	pData += sizeof NODE_SCD_MAGIC;

	m_pNodeTag = reinterpret_cast<TAG_OCCNO *>(pData);

	for (int i = 0; i < m_nTotalNodeCount; ++i)
	{
		Q_ASSERT(m_pNodeTag[i].OccNo != INVALID_OCCNO && m_pNodeTag[i].OccNo <= MAX_NODE_OCCNO);
		if (m_pNodeTag[i].OccNo == INVALID_OCCNO || m_pNodeTag[i].OccNo > MAX_NODE_OCCNO)
		{
			//todo log
			continue;
		}
		m_MapString2ID.insert(std::make_pair(m_pNodeTag[i].TagName, m_pNodeTag[i].OccNo));
		m_arrNodeTagNames.push_back(&m_pNodeTag[i]);
	}

	pData += sizeof TAG_OCCNO * m_nTotalNodeCount;

	std::shared_ptr<CTagNamePool> pTagNamePool = nullptr;

	for (int i = 0; i < m_nNodeFesCount; ++i)
	{
		pTagNamePool = std::make_shared<CTagNamePool>();
		pTagNamePool->Initialize((unsigned char*)pData, 0);
		m_MapTagName.insert(std::make_pair(pTagNamePool->GetCurrentNodeOccNo(), pTagNamePool));
		pData += pTagNamePool->GetEstimateSize();
	}

	for (int i = 0; i < m_nNodeScdCount; ++i)
	{
		pTagNamePool = std::make_shared<CTagNamePool>();
		pTagNamePool->Initialize((unsigned char*)pData, 0);
		m_MapTagName.insert(std::make_pair(pTagNamePool->GetCurrentNodeOccNo(), pTagNamePool));
		pData += pTagNamePool->GetEstimateSize();
	}

	for (int i = 0; i < m_nNodeClientCount; ++i)
	{
		pTagNamePool = std::make_shared<CTagNamePool>();
		pTagNamePool->Initialize((unsigned char*)pData, 0);
		m_MapTagName.insert(std::make_pair(pTagNamePool->GetCurrentNodeOccNo(), pTagNamePool));
		pData += pTagNamePool->GetEstimateSize();
	}

	return true;
}

bool CScadaApi::BuildFesDB(const char* pszDataPath)
{
	Q_ASSERT(pszDataPath && strlen(pszDataPath) > 0);
	if (!pszDataPath || strlen(pszDataPath) == 0)
	{
		return false;
	}

	QString szName = QString(pszDataPath) + "-scada";
	char* pData = (char*)(m_pMem->GetShareMem(szName.toStdString().c_str()));

	Q_ASSERT(pData);
	if (!pData)
	{
		return false;
	}
	m_pNodeMagicHead = (NODE_SCD_MAGIC*)pData;
	Q_ASSERT(m_pNodeMagicHead->MagicNum1 == MAGIC_HEAD && m_pNodeMagicHead->MagicNum2 == MAGIC_HEAD);

	m_nNodeFesCount = m_pNodeMagicHead->NodeFesCount;
	m_nTotalNodeCount = m_pNodeMagicHead->AllNodeCount;

	Q_ASSERT(m_nNodeFesCount > 0 && m_nTotalNodeCount > 0);

	if (m_nNodeFesCount <= 0 || m_nNodeFesCount <= 0 || m_nNodeFesCount > m_nTotalNodeCount)
	{
		return false;
	}

	pData = (char*)pData + sizeof NODE_SCD_MAGIC;

	size_t nRet = 0;

	nRet = CreateNodeDB((unsigned char*)pData);
	pData += nRet;

	nRet = CreateNodeMem((unsigned char*)pData);
	pData += nRet;


	nRet = CreateFesMem((unsigned char*)pData);
	pData += nRet;

	return true;
}

bool CScadaApi::CreateScdAndClientDB(const char* pszFilePath)
{
	Q_ASSERT(pszFilePath && strlen(pszFilePath) > 0);
	if (!pszFilePath || strlen(pszFilePath) == 0)
	{
		return false;
	}

	QString szName = QString(pszFilePath) + "-scada-variable";
	char* pData = (char*)(m_pScdMem->GetShareMem(szName.toStdString().c_str()));

	Q_ASSERT(pData);
	if (!pData)
	{
		return false;
	}

	m_pScdHead = (NODE_SCD_MAGIC*)pData;
	Q_ASSERT(m_pScdHead->MagicNum1 == MAGIC_HEAD && m_pScdHead->MagicNum2 == MAGIC_HEAD);

	m_nNodeScdCount = m_pScdHead->NodeServerCount;
	m_nNodeClientCount = m_pScdHead->NodeClientCount;

	Q_ASSERT(m_nNodeScdCount);
	size_t nRet = 0;
	pData += sizeof NODE_SCD_MAGIC;

	nRet = CreateMemDB((unsigned char*)pData);
	pData += nRet;

	nRet = CreateScadaDB((unsigned char*)pData);
	pData += nRet;

	nRet = CreateClientDB((unsigned char*)pData);
	pData += nRet;

	nRet = CreateAppDB((unsigned char*)pData);
	return true;
}

size_t CScadaApi::CreateMemDB(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	m_pNodeScdMemHead = reinterpret_cast <NODE_MEM*>(pAddr);

	QString szLog;
	for (int i = 0; i < m_nNodeScdCount; ++i)
	{
		Q_ASSERT(m_pNodeScdMemHead[i].OccNo != INVALID_OCCNO && m_pNodeScdMemHead[i].OccNo <= MAX_OCCNO);

		if (m_pNodeScdMemHead[i].OccNo == INVALID_OCCNO || m_pNodeScdMemHead[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] node's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pNodeScdMemHead[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);

			m_arrScdMemDbs.push_back(&m_pNodeScdMemHead[i]);
			continue;
		}
		m_arrScdMemDbs.push_back(&m_pNodeScdMemHead[i]);
	}

	m_pNodeClientMemHead = reinterpret_cast<NODE_MEM*>(pAddr + (m_nNodeScdCount * sizeof NODE_MEM));
	for (int i = 0; i < m_nNodeClientCount; ++i)
	{
		Q_ASSERT(m_pNodeClientMemHead[i].OccNo != INVALID_OCCNO && m_pNodeClientMemHead[i].OccNo <= MAX_OCCNO);

		if (m_pNodeClientMemHead[i].OccNo == INVALID_OCCNO || m_pNodeClientMemHead[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] node's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pNodeClientMemHead[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);

			m_arrClientMemDbs.push_back(&m_pNodeClientMemHead[i]);
			continue;
		}
		m_arrClientMemDbs.push_back(&m_pNodeClientMemHead[i]);
	}

	return sizeof NODE_MEM *(m_nNodeScdCount + m_nNodeClientCount);
}

size_t CScadaApi::CreateScadaDB(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}

	size_t nRet = 0;

	m_pNodeScdDBHead = (CServerDB *)(pAddr);
	CServerDB* pServer = nullptr;
	for (int i = 0; i < m_nNodeScdCount; ++i)
	{
		pServer = new CServerDB(this);

		nRet = pServer->LoadMem(pAddr);
		m_arrScdDBs.push_back(pServer);
		m_mapScdDbs.insert(std::make_pair(pServer->GetNodeOccNo(), pServer));
		m_mapType.insert(std::make_pair(pServer->GetNodeOccNo(), NODE_SVR));
		pAddr += nRet;
	}

	nRet = 0;
	for (auto i : m_arrScdDBs)
	{
		nRet += i->GetEstimateSize();
	}

	return nRet;
}

size_t CScadaApi::CreateClientDB(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}

	size_t nRet = 0;

	m_pNodeClientHead = (CClientDB *)(pAddr);
	CClientDB* pClient = nullptr;
	for (int i = 0; i < m_nNodeClientCount; ++i)
	{
		pClient = new CClientDB(this);

		nRet = pClient->LoadMem(pAddr);
		m_arrClientDBs.push_back(pClient);
		m_mapClientDBs.insert(std::make_pair(pClient->GetNodeOccNo(), pClient));
		m_mapType.insert(std::make_pair(pClient->GetNodeOccNo(), NODE_CLIENT));
		pAddr += nRet;
	}

	nRet = 0;
	for (auto i : m_arrClientDBs)
	{
		nRet += i->GetEstimateSize();
	}

	return nRet;
}

size_t CScadaApi::CreateAppDB(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	NODE_APP_MAGIC * pHead = reinterpret_cast<NODE_APP_MAGIC*>(pAddr);
	Q_ASSERT(pHead->MagicNum1 == MAGIC_HEAD && pHead->MagicNum2 == MAGIC_HEAD);
	if (pHead->MagicNum1 != MAGIC_HEAD || pHead->MagicNum2 != MAGIC_HEAD)
	{
		return 0;
	}
	int nCount = pHead->nCount;

	pAddr += sizeof NODE_APP_MAGIC;

	size_t nSize = 0;

	for (int i = 0; i < nCount; ++i)
	{
		nSize += CreateNodeAppDB(pAddr);
		pAddr += nSize;
	}

	return sizeof NODE_APP_MAGIC + nSize;
}

size_t CScadaApi::CreateNodeAppDB(unsigned char* pAddr)
{
	Q_ASSERT(pAddr);
	if (!pAddr)
	{
		return 0;
	}
	NODE_APP_MAGIC * pHead = reinterpret_cast<NODE_APP_MAGIC*>(pAddr);
	Q_ASSERT(pHead->MagicNum1 == MAGIC_HEAD && pHead->MagicNum2 == MAGIC_HEAD);
	if (pHead->MagicNum1 != MAGIC_HEAD || pHead->MagicNum2 != MAGIC_HEAD)
	{
		return 0;
	}
	int nCount = pHead->nCount;
	int32u nOccNo = pHead->nOccNo;
	pAddr += sizeof NODE_APP_MAGIC;

	std::vector < SAPP* > arrApps;

	QString szLog;

	SAPP* ppApp = nullptr;

	ppApp = reinterpret_cast<SAPP*>(pAddr);

	m_mapAppAddr.insert(std::make_pair(nOccNo, ppApp));

	for (int i = 0; i < nCount; ++i)
	{
		Q_ASSERT(ppApp[i].OccNo != INVALID_OCCNO && ppApp[i].OccNo <= MAX_OCCNO);
		if (ppApp[i].OccNo == INVALID_OCCNO || ppApp[i].OccNo > MAX_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] nodeapp's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(ppApp[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			continue;
		}
		arrApps.push_back(&ppApp[i]);
	}
	m_arrAppInfos.insert(std::make_pair(nOccNo, arrApps));

	return sizeof NODE_APP_MAGIC + sizeof SAPP * arrApps.size();
}

size_t CScadaApi::CreateNodeDB(unsigned char* pHead)
{
	Q_ASSERT(pHead);
	if (!pHead)
	{
		return 0;
	}
	m_pNodes = reinterpret_cast<NODE*>(pHead);

	QString szLog;

	int32u i = 0;
	// 创建节点表
	for (i = 0; i < m_nTotalNodeCount; i++)
	{
		Q_ASSERT(m_pNodes[i].OccNo != INVALID_OCCNO && m_pNodes[i].OccNo <= MAX_NODE_OCCNO);
		if (m_pNodes[i].OccNo == INVALID_OCCNO || m_pNodes[i].OccNo > MAX_NODE_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] node's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pNodes[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			continue;
		}
		m_arrNodes.push_back(&m_pNodes[i]);
	}

	return sizeof(NODE)* m_nTotalNodeCount;
}

size_t CScadaApi::CreateNodeMem(unsigned char* pHead)
{
	Q_ASSERT(pHead);
	if (!pHead)
	{
		LogMsg(QObject::tr("read mem error,due to error head address").toStdString().c_str(), 0);
		return 0;
	}

	QString szLog;
	m_pNodeGIHead = reinterpret_cast<NODE_MEM*>(pHead);

	for (int i = 0; i < m_nNodeFesCount; ++i)
	{
		Q_ASSERT(m_pNodeGIHead[i].OccNo != INVALID_OCCNO && m_pNodeGIHead[i].OccNo < MAX_NODE_OCCNO);
		if (m_pNodeGIHead[i].OccNo == INVALID_OCCNO || m_pNodeGIHead[i].OccNo > MAX_NODE_OCCNO)
		{
			szLog = QString(QObject::tr("[%1] node's occno [ %2 ] in memory db is wrong ")).arg(i + 1).arg(m_pNodeGIHead[i].OccNo);
			LogMsg(szLog.toStdString().c_str(), 0);
			continue;
		}
		m_arrTempNodes.push_back(&m_pNodeGIHead[i]);
		m_MapNode.insert(std::make_pair(m_pNodeGIHead[i].OccNo, &m_pNodeGIHead[i]));
	}
	return sizeof NODE_MEM * m_nNodeFesCount;
}

size_t CScadaApi::CreateFesMem(unsigned char* pHead)
{
	Q_ASSERT(pHead);
	if (!pHead)
	{
		return 0;
	}
	m_pFesHead = (CFesDB *)(pHead);

	CFesDB* pFesDB = Q_NULLPTR;

	for (auto i : m_arrTempNodes)
	{
		pFesDB = new CFesDB(this);

		if (pFesDB)
		{
			pFesDB->LoadMem((unsigned char*)((char*)m_pFesHead + i->NodeOffSet));
			m_arrFesDbs.push_back(pFesDB);
			m_mapFesDBs.insert(std::make_pair(i->OccNo, pFesDB));
			m_mapType.insert(std::make_pair(i->OccNo, NODE_FES));
		}
	}
	return 0;
}

CFesDB* CScadaApi::GetFesDbByOccNo(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_NODE_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_NODE_OCCNO)
	{
		return nullptr;
	}
	auto it = m_mapFesDBs.find(nOccNo);
	if (it == m_mapFesDBs.end())
	{
		return nullptr;
	}
	else
	{
		return it->second;
	}
}

CServerDB* CScadaApi::GetScadaDbByOccNo(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_NODE_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_NODE_OCCNO)
	{
		return nullptr;
	}
	auto it = m_mapScdDbs.find(nOccNo);
	if (it == m_mapScdDbs.end())
	{
		return nullptr;
	}
	else
	{
		return it->second;
	}
}

CClientDB* CScadaApi::GetClientDbByOccNo(int32u nOccNo)
{
	Q_ASSERT(nOccNo != INVALID_OCCNO && nOccNo <= MAX_NODE_OCCNO);
	if (nOccNo == INVALID_OCCNO || nOccNo > MAX_NODE_OCCNO)
	{
		return nullptr;
	}
	auto it = m_mapClientDBs.find(nOccNo);
	if (it == m_mapClientDBs.end())
	{
		return nullptr;
	}
	else
	{
		return it->second;
	}
}

