#pragma once
#ifndef CITEMTREE_H
#define CITEMTREE_H

#include "CCommonHead.h"
#include <QAbstractItemView>
class CItemTree
{
public:
	CItemTree();
	~CItemTree();

	
public:
	void init();

	//得到子行个数
	int countChild() const;

	//获取父目录在它那级索引号
	int indexOfParent();

	//在目录下添加子目录
	void addChild(CItemTree* pItemTree);

	//通过索引获取目录项
	CItemTree* child(int nIndex);

	//设置父目录
	void       setParent(CItemTree* pItemTree);

	//获取父目录
	CItemTree* parent() const;


public:
	bool				m_IfUse;		//内存池占用情况

	//目录信息
	int					m_TreeType;		//用于区别超级表还是子表的,0非，1超级表，2子表
	int					m_TreeId;		//用于区分表id
	QString				m_TreeName;		//目录名字

	CItemTree*			m_pParent;		//父目录指针
	QList<CItemTree*>	m_listChild;	//孩子目录链表
};

#endif // !CITEMTREE_H