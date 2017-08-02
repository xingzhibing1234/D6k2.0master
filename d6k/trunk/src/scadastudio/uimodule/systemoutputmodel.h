#ifndef ULTRA_LIST_H
#define ULTRA_LIST_H

#include "logdefine.h"

#include <QAbstractTableModel>
#include <QColor>
#include <vector>
#include <string>
#include <memory>

class CMsgLog;

#if 0
class CDataItem
{
public:
	CDataItem(const MSG_LOG &log, QColor& color)
	{
		m_strType = log.ModuleName;
		m_strTag = log.LogText;
		m_color = color;

		//QString strTmp = QDateTime::currentDateTime().toString("hh:mm:ss");

		QString strTmp = QString("%1:%2:%3 %4").arg(log.Time.Hour).arg(log.Time.Minute).arg(log.Time.Second).arg(log.Time.Milliseconds);

		QString strDate = QString("%1-%2-%3").arg(log.Time.Year).arg(log.Time.Month).arg(log.Time.Day);

		m_strDate = strDate.toStdString();

		//char buf[32];
		//sprintf(buf,"%d:%d:%d:%d:%d:%d:%d\n",tvTime.year()%100,tvTime.month(),tvTime.day(),tvTime.hour(),tvTime.minute(),tvTime.second(),tvTime.microsec());
		//sprintf(buf,"%d:%d:%d %d:%d:%d\n",tvTime.year(),tvTime.month(),tvTime.day(),tvTime.hour(),tvTime.minute(),tvTime.second());
		m_strTime = strTmp.toStdString();

		m_nThreadID = log.ThreadID;
		m_nProcessID = log.ProcessID;
	}
	 
	std::string m_strType;
	std::string m_strTag;
	std::string m_strTime;
	std::string m_strDate;

	QColor m_color;

	int m_nThreadID;
	int m_nProcessID;
};
#endif

class CSystemOutputModel : public QAbstractTableModel
{
	enum{Date, Time, Tag, Type, ThreadId, ProcessID};
	
	enum
	{
		COL_LEVEL = 0,
		COL_INDEX,  //!< ���
		COL_DATE,
		COL_TIME,
		COL_MODULE,
		COL_COMMENT,
		COL_PROCESS,
		COL_THREAD,
		COL_MAX,
	};
public:
	explicit CSystemOutputModel(QObject *parent = 0, int rowCount = 1000000, int colCount = 6);
	~CSystemOutputModel();

	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	//bool InsertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
private:
	int m_nRowCount;
	int m_nColCount;

	QStringList m_lstHeader;

private: 
	std::vector<std::shared_ptr<CMsgLog>> m_arrLogs;
public: 	
	void AddItem(std::shared_ptr<CMsgLog> pLog);
	void ClearAll();
};

#endif // ULTRA_LIST_H