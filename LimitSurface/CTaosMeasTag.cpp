#include "CTaosMeasTag.h"

QList<CTaosMeasTag*> CTaosMeasTag::m_listPoolMeasTAGNode;
CTaosMeasTag::CTaosMeasTag()
{
	init();
}

CTaosMeasTag::~CTaosMeasTag()
{
}

void CTaosMeasTag::init()
{
	m_tbname.clear();
	m_meas_name.clear();
	m_meas_type = -1;
	m_IfUse = false;
}

CTaosMeasTag* CTaosMeasTag::getMeasTAGNodebyPool()
{
	int nCount = m_listPoolMeasTAGNode.count();
	CTaosMeasTag* pTaosMeasTag;

	for (int i = 0; i < nCount; i++)
	{
		pTaosMeasTag = m_listPoolMeasTAGNode[i];
		if (pTaosMeasTag->m_IfUse == false)
		{
			pTaosMeasTag->m_IfUse = true;
			return pTaosMeasTag;
		}
	}

	pTaosMeasTag = new CTaosMeasTag();
	pTaosMeasTag->m_IfUse = true;
	m_listPoolMeasTAGNode.append(pTaosMeasTag);
	return pTaosMeasTag;
}

void CTaosMeasTag::resetMeasTAGPool()
{
	foreach(CTaosMeasTag * pTaosMeasTag, m_listPoolMeasTAGNode)
	{
		pTaosMeasTag->init();
	}
}
