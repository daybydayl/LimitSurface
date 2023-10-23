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
	//���������л�ȡ����
	static CTaosMeasTag* getMeasTAGNodebyPool();
	//������������
	static void resetMeasTAGPool();

public:
	QString m_tbname;
	QString m_meas_name;
	int		m_meas_type;

private:
	//taos����TAG�����
	static QList<CTaosMeasTag*>			m_listPoolMeasTAGNode;
	bool	m_IfUse;//������ж����Ƿ�ռ��
	
};
#endif // !CTAOSMEASTAG_H
