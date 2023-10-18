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

	//���ʽ������
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
	
	//������ݺ�ͷ��
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation,int role = Qt::DisplayRole) const;


	//��������Ϊ�����д����
	virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
	//����һ��combobox��editģ��
	Qt::ItemFlags flags(const QModelIndex& index) const;


public:
	QStringList		m_Listheader;

};

