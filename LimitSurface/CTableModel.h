#pragma once
#include "CCommonHead.h"
#include "CTaos.h"
#include "CGlobal.h"
#include "CTaosMeasTable.h"
#include "CTaosMeasTag.h"
class CTableModel:public QAbstractTableModel
{
public:
	CTableModel(QObject* parent = nullptr);
	~CTableModel();

public:
	void hasChanged();
	void setheader(QStringList &);

	//表格方式的行列
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
	
	//表格数据和头部
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;


	//下面两个为代理回写函数
	virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
	//启用一下combobox和edit模型
	Qt::ItemFlags flags(const QModelIndex& index) const;


public:
	QStringList		m_Listheader;

};

