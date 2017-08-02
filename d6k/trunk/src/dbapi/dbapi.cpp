#include "dbapi.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QMutexLocker>

QMutex CDB_API::m_mutex;

CDB_API::CDB_API(QString strConnName)
{
	QMutexLocker locker(&m_mutex);

	m_strConnName = strConnName;
}

CDB_API::~CDB_API()
{
	QMutexLocker locker(&m_mutex);
}

bool CDB_API::open(const QString &strDns, const QString &strUserName, const QString &strPassword, int nType, int timeout)
{
	QMutexLocker locker(&m_mutex);

	if (m_dbQt.contains(m_strConnName))
	{
		m_strError = "连接已存在!!!";
		qDebug() << "连接已存在";
		return false;
	}
	else
	{
		if (nType == ORACLE)
		{
			m_dbQt = QSqlDatabase::addDatabase("QOCI", m_strConnName);
			m_dbQt.setPort(1521);
		}
		else if (nType == MYSQL)
		{
			m_dbQt = QSqlDatabase::addDatabase("QMYSQL", m_strConnName);
			m_dbQt.setPort(3306);
		}

		m_dbQt.setDatabaseName(strDns);    // 库名		
		m_dbQt.setUserName(strUserName);   // 用户名
		m_dbQt.setPassword(strPassword);   // 密码

		if (!m_dbQt.open())
		{
			m_strError = m_dbQt.lastError().text();
			qDebug() << m_dbQt.lastError().text();
			return false;
		}
		else
		{
			m_dbQt.transaction();

			m_query = QSqlQuery(m_dbQt);

			qDebug() << "succfully";
		}
	}

	return true;
}

QString CDB_API::lastError()
{

	QMutexLocker locker(&m_mutex);

	return m_strError;
}

bool CDB_API::exec(const QString &strSql)
{
	QMutexLocker locker(&m_mutex);

	if (m_query.exec(strSql))
	{
		return true;
	}
	else
	{
		m_strError = m_query.lastError().text();

		return false;
	}
}

QSqlQuery& CDB_API::getSqlQuery()
{
	QMutexLocker locker(&m_mutex);

	return m_query;
}

int CDB_API::getFieldCounts()
{
	QMutexLocker locker(&m_mutex);

	return m_query.record().count();
}

int CDB_API::getFieldIndex(QString &strFieldName)
{
	QMutexLocker locker(&m_mutex);

	return m_query.record().indexOf(strFieldName);
}

bool CDB_API::next()
{
	QMutexLocker locker(&m_mutex);

	return m_query.next();
}

QVariant CDB_API::getValue(int nPos)
{
	QMutexLocker locker(&m_mutex);

	return m_query.value(nPos);
}

QVariant CDB_API::getValue(QString strFieldName)
{
	QMutexLocker locker(&m_mutex);

	int nIndex = m_query.record().indexOf(strFieldName);

	return m_query.value(nIndex);
}

bool CDB_API::transaction()
{
	QMutexLocker locker(&m_mutex);

	return m_dbQt.transaction();
}

bool CDB_API::commit()
{
	QMutexLocker locker(&m_mutex);

	return m_dbQt.commit();
}

bool CDB_API::rollback()
{
	QMutexLocker locker(&m_mutex);

	return m_dbQt.rollback();
}

bool CDB_API::prepare(QString &strSql)
{
	QMutexLocker locker(&m_mutex);

	return m_query.prepare(strSql);
}

void CDB_API::bindValue(QString strField, QVariant varValue)
{
	QMutexLocker locker(&m_mutex);

	m_query.bindValue(strField, varValue);
}

bool CDB_API::exec()
{
	QMutexLocker locker(&m_mutex);

	if (m_query.exec())
	{
		return true;
	}
	else
	{
		m_strError = m_query.lastError().text();

		return false;
	}
}

void CDB_API::addBindValue(QVariant varValue)
{
	QMutexLocker locker(&m_mutex);

	m_query.addBindValue(varValue);
}

void CDB_API::bindValue(int &nPos, QVariant varValue)
{
	QMutexLocker locker(&m_mutex);

	m_query.bindValue(nPos, varValue);
}

void CDB_API::addBatchBindValue(QVariantList &varList)
{
	QMutexLocker locker(&m_mutex);

	m_query.addBindValue(varList);
}

bool CDB_API::execBatch()
{
	QMutexLocker locker(&m_mutex);

	if (m_query.execBatch())
	{
		return true;
	}
	else
	{
		m_strError = m_query.lastError().text();

		return false;
	}
}

bool CDB_API::getFieldType(QString strTable, QString strFieldAlasName, QString strFieldName, CVarinfo &info)
{
	QMutexLocker locker(&m_mutex);

	QString strUser = UID;

	QString strSql = "SELECT * FROM all_tab_columns t WHERE t.OWNER='" + strUser + "'" +
		" AND t.TABLE_NAME='" + strTable + "' AND t.COLUMN_NAME='" + strFieldName + "'";

	if (m_query.exec(strSql))
	{
		while (m_query.next())
		{
			QString strFieldName = ALL_TAB_COLUMN_NAME;

			int nPos = m_query.record().indexOf(strFieldName);

			info.name = strFieldAlasName.toStdString();

			strFieldName = ALL_TAB_DATA_TYPE;
			nPos = m_query.record().indexOf(strFieldName);
			QString strTmp = m_query.value(nPos).toString();
			if (strTmp.contains(DB_TYPE_NUMBER) ||
				strTmp.contains(DB_TYPE_FLOAT))
			{
				info.dbtype = 2;
			}
			else if (strTmp.contains(DB_TYPE_CHAR, Qt::CaseInsensitive) ||
				strTmp.contains(DB_TYPE_VARCHAR, Qt::CaseInsensitive))
			{
				info.dbtype = 3;
			}

			strFieldName = ALL_TAB_COLUMN_DATA_LENGTH;
			nPos = m_query.record().indexOf(strFieldName);
			info.dbsize = m_query.value(nPos).toInt();

			strFieldName = ALL_TAB_COLUMN_DATA_PRECISION;
			nPos = m_query.record().indexOf(strFieldName);
			info.prec = m_query.value(nPos).toInt();

			strFieldName = ALL_TAB_COLUMN_DATA_SCALE;
			nPos = m_query.record().indexOf(strFieldName);

			if (strTmp.contains(DB_TYPE_FLOAT))
			{
				info.scale = 129;	//float
			}
			else
			{
				info.scale = m_query.value(nPos).toInt();
			}

		}
	}
	else
	{
		m_strError = m_query.lastError().text();

		return false;
	}

	return true;
}

QString CDB_API::trimmed(QString &strData) const
{
	QMutexLocker locker(&m_mutex);

	return strData.trimmed();
}

QString CDB_API::getFieldName(int nPos)
{
	QMutexLocker locker(&m_mutex);

	return m_query.record().fieldName(nPos);
}

bool CDB_API::getVec(QString strSql, TwoVec &vec)
{
	QMutexLocker locker(&m_mutex);

	vec.clear();

	if (m_query.exec(strSql))
	{
		int nCols = m_query.record().count();

		while (m_query.next())
		{
			QVector<QString> vecTmp;

			for (int i = 0; i < nCols; i++)
			{
				vecTmp.push_back(m_query.value(i).toString().trimmed());
			}

			vec.push_back(vecTmp);
		}
	}
	else
	{
		m_strError = m_query.lastError().text();

		return false;
	}

	return true;
}

#include <QStringList>

void CDB_API::close()
{
	QMutexLocker locker(&m_mutex);

	QStringList list = QSqlDatabase::connectionNames();

	m_dbQt.close();
}

bool CDB_API::getFieldsTypeFromTable(QString strTable, std::vector<CVarinfo> &info)
{
	QMutexLocker locker(&m_mutex);

	QString strUser = UID;

	QString strSql = "SELECT * FROM all_tab_columns t WHERE t.OWNER='" + strUser + "'" +
		" AND t.TABLE_NAME='" + strTable;

	info.clear();


	if (m_query.exec(strSql))
	{
		while (m_query.next())
		{
			CVarinfo infoTmp;

			QString strFieldName = ALL_TAB_COLUMN_NAME;

			int nPos = m_query.record().indexOf(strFieldName);

			infoTmp.name = m_query.value(nPos).toString().toStdString();

			strFieldName = ALL_TAB_DATA_TYPE;
			nPos = m_query.record().indexOf(strFieldName);
			QString strTmp = m_query.value(nPos).toString();
			if (strTmp.contains(DB_TYPE_NUMBER) ||
				strTmp.contains(DB_TYPE_FLOAT))
			{
				infoTmp.dbtype = 2;
			}

			strFieldName = ALL_TAB_COLUMN_DATA_LENGTH;
			nPos = m_query.record().indexOf(strFieldName);
			infoTmp.dbsize = m_query.value(nPos).toInt();

			strFieldName = ALL_TAB_COLUMN_DATA_PRECISION;
			nPos = m_query.record().indexOf(strFieldName);
			infoTmp.prec = m_query.value(nPos).toInt();

			strFieldName = ALL_TAB_COLUMN_DATA_SCALE;
			nPos = m_query.record().indexOf(strFieldName);

			if (strTmp.contains(DB_TYPE_FLOAT))
			{
				infoTmp.scale = 129;	//float
			}
			else
			{
				infoTmp.scale = m_query.value(nPos).toInt();
			}

			info.push_back(infoTmp);
		}
	}
	else
	{
		m_strError = m_query.lastError().text();

		return false;
	}

	return true;
}
