#include "CTaosMeasTable.h"

QList<CTaosMeasTable*> CTaosMeasTable::m_listPoolMeasNode;
CTaosMeasTable::CTaosMeasTable()
{
	init();
}

CTaosMeasTable::~CTaosMeasTable()
{
}

void CTaosMeasTable::init()
{
	m_ts = 0;
	m_date.clear();
	m_value = 0;
	m_IfUse = false;
}

CTaosMeasTable* CTaosMeasTable::getMeasNodebyPool()
{
	int nCount = m_listPoolMeasNode.count();
	CTaosMeasTable* pTaosMeasTable;
	
	for (int i = 0; i < nCount; i++)
	{
		pTaosMeasTable = m_listPoolMeasNode[i];
		if (false == pTaosMeasTable->m_IfUse)
		{
			pTaosMeasTable->m_IfUse = true;
			return pTaosMeasTable;
		}
	}

	pTaosMeasTable = new CTaosMeasTable();
	pTaosMeasTable->m_IfUse = true;
	m_listPoolMeasNode.append(pTaosMeasTable);

	return pTaosMeasTable;
}

void CTaosMeasTable::resetMeasPool()
{
	foreach(CTaosMeasTable * pTaosMeasTable, m_listPoolMeasNode)
	{
		pTaosMeasTable->init();
	}

}
