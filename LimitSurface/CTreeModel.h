#pragma once
#ifndef CTREEMODEL_H
#define CTREEMODEL_H




#include "CCommonHead.h"
#include "CItemTree.h"
#include "CGlobal.h"

class CTreeModel:public QAbstractItemModel
{
public:
	CTreeModel(QObject* parent = nullptr);
	virtual ~CTreeModel();

public:
	void setTreeData();

public://��д�̳к���
	//�ڸ�Ŀ¼�����һ��Ŀ¼,��������
	virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
	//����������Ŀ¼�¼�һ��Ŀ¼,��������
	virtual QModelIndex parent(const QModelIndex& child) const;

	//�������ַ�������Һ��Ӷ��ٸ���Ҳ�����Ӳ˵��¶�����
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
	//�Ҷ����У�����Ŀ¼һ�㶼��1��
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;

	//����������Ŀ¼
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	//���ﲻ��table���һ�㲻��ͷ������ҲҪ��д���ؿն���
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

public:

	void setheadData(QStringList& header);
	//���ö����
	static void       resetItemTreePool();
	//�Ӷ���ػ�ȡ����
	static CItemTree* getItemTreebyPool();

protected:
	CItemTree* m_pItemRoot;							//��Ŀ¼

public:
	static QList<CItemTree*>	m_listSTree;			//�泬������Ŀ¼����
	static QList<CItemTree*>	m_listTree;				//�浥���ӱ�Ŀ¼
	static QMap<QString, QList<CItemTree*>>	m_mapQStoQLTree;			//�泬�����µ��ӱ�Ŀ¼

	QStringList	m_listheader;
private:
	static QList<CItemTree*>	m_listPoolItemTree;	//�����
};

#endif // ! CTREEMODEL_H