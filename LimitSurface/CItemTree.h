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

	//�õ����и���
	int countChild() const;

	//��ȡ��Ŀ¼�����Ǽ�������
	int indexOfParent();

	//��Ŀ¼�������Ŀ¼
	void addChild(CItemTree* pItemTree);

	//ͨ��������ȡĿ¼��
	CItemTree* child(int nIndex);

	//���ø�Ŀ¼
	void       setParent(CItemTree* pItemTree);

	//��ȡ��Ŀ¼
	CItemTree* parent() const;


public:
	bool				m_IfUse;		//�ڴ��ռ�����

	//Ŀ¼��Ϣ
	int					m_TreeType;		//�������𳬼������ӱ��,0�ǣ�1������2�ӱ�
	int					m_TreeId;		//�������ֱ�id
	QString				m_TreeName;		//Ŀ¼����

	CItemTree*			m_pParent;		//��Ŀ¼ָ��
	QList<CItemTree*>	m_listChild;	//����Ŀ¼����
};

#endif // !CITEMTREE_H