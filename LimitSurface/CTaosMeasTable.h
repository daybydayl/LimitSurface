#pragma once
#include "CCommonHead.h"
class CTaosMeasTable
{
public:
	CTaosMeasTable();
	~CTaosMeasTable();
	void init();

public:
	//���������л�ȡ����
	static CTaosMeasTable* getMeasNodebyPool();
	//������������
	static void resetMeasPool();

public:
	double	m_ts;		//����ʱ���
	QString m_date;		//����ת�ɵ�����
	double	m_value;	//ʱ�����Ӧ����ֵ

	QString m_Smeas_name_TAG;//�������tag
	int m_Smeas_type_TAG;

	//taos�����¼�����
	static QList<CTaosMeasTable*>			m_listPoolMeasNode;
	bool	m_IfUse;//������ж����Ƿ�ռ��
};

