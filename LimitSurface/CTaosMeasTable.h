#pragma once
#include "CCommonHead.h"
class CTaosMeasTable
{
public:
	CTaosMeasTable();
	~CTaosMeasTable();
	void init();

public:
	//量测对象池中获取对象
	static CTaosMeasTable* getMeasNodebyPool();
	//重置量测对象池
	static void resetMeasPool();

public:
	double	m_ts;		//毫秒时间戳
	QString m_date;		//毫秒转成的日期
	double	m_value;	//时间戳对应量测值

	QString m_Smeas_name_TAG;//超级表的tag
	int m_Smeas_type_TAG;

	//taos量测记录对象池
	static QList<CTaosMeasTable*>			m_listPoolMeasNode;
	bool	m_IfUse;//对象池中对象是否占用
};

