#pragma once
#ifndef CTAOSMEASTAG_H
#define CTAOSMEASTAG_H

#include "CCommonHead.h"
class CTaosMeasTag
{
public:
	CTaosMeasTag();
	~CTaosMeasTag();
	void init();

public:
	//量测对象池中获取对象
	static CTaosMeasTag* getMeasTAGNodebyPool();
	//重置量测对象池
	static void resetMeasTAGPool();

public:
	QString m_tbname;
	QString m_meas_name;
	int		m_meas_type;

private:
	//taos量测TAG对象池
	static QList<CTaosMeasTag*>			m_listPoolMeasTAGNode;
	bool	m_IfUse;//对象池中对象是否占用
	
};
#endif // !CTAOSMEASTAG_H
