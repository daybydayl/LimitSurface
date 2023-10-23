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

public://重写继承函数
	//在该目录下添加一行目录,返回索引
	virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
	//父类在它级目录下加一行目录,返回索引
	virtual QModelIndex parent(const QModelIndex& child) const;

	//给父类地址，父类找孩子多少个，也就是子菜单下多少行
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
	//找多少列，这里目录一般都是1了
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;

	//串数据树型目录
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	//这里不像table那里，一般不用头部，但也要复写返回空对象
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

public:

	void setheadData(QStringList& header);
	//重置对象池
	static void       resetItemTreePool();
	//从对象池获取对象
	static CItemTree* getItemTreebyPool();

protected:
	CItemTree* m_pItemRoot;							//根目录

public:
	static QList<CItemTree*>	m_listSTree;			//存超级表树目录队列
	static QList<CItemTree*>	m_listTree;				//存单个子表目录
	static QMap<QString, QList<CItemTree*>>	m_mapQStoQLTree;			//存超级表下的子表目录

	QStringList	m_listheader;
private:
	static QList<CItemTree*>	m_listPoolItemTree;	//对象池
};

#endif // ! CTREEMODEL_H