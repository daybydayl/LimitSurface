#include "CItemTree.h"

CItemTree::CItemTree()
{
	init();//初始化很多时候是务必的，养成初始化的习惯
}

CItemTree::~CItemTree()
{
	qDeleteAll(m_listChild);
	m_listChild.clear();
}

void CItemTree::init()
{
	m_IfUse = false;
	m_TreeType = 0;
	m_TreeId = -1;
	m_TreeName.clear();
	m_pParent = 0;
}

int CItemTree::countChild() const
{
	int nCount = 0;
	nCount = m_listChild.count();
	return nCount;
}

int CItemTree::indexOfParent()
{
	if(m_pParent)
	{
		int nCount = 0;

		//获取父目录下有多少个子目录，也就是获取当前子目录的链表成员个数
		//nCount = m_listChild.count();
		nCount = m_pParent->countChild();
		if (nCount > 0)
		{
			return m_pParent->m_listChild.indexOf(static_cast<CItemTree*>(this));
		}
	}

	return -1;
}

void CItemTree::addChild(CItemTree* pItemTree)
{
	if(pItemTree)
		m_listChild.append(pItemTree);

}

CItemTree* CItemTree::child(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_listChild.count())
		return 0;
	return m_listChild[nIndex];
}

void CItemTree::setParent(CItemTree* pItemTree)
{
	if (pItemTree)
		m_pParent = pItemTree;
}

CItemTree* CItemTree::parent() const
{
	return m_pParent;
}

