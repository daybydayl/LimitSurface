#pragma once
#include "CCommonHead.h"
#include "CTaos.h"
#include "CGlobal.h"
#include "CTaosMeasTable.h"
class CTableModel:public QAbstractTableModel
{
public:
	CTableModel(QObject* parent = nullptr);
	~CTableModel();

public:
	void hasChanged();
	void SetHeader();

	//表格方式的行列
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
	
	//表格数据和头部
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;


public:
	QStringList		m_Listheader;

};

