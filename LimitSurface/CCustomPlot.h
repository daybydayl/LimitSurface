#pragma once
#include "qcustomplot.h"
#include "CCommonHead.h"
class CCustomPlot
{
public:
	CCustomPlot();
	~CCustomPlot();
	void init();

	void Draw();
	
public:
	static CCustomPlot* intoinstance();
	static void exitinstance();


public:
	QCustomPlot*			m_pCustomPlot;	//主画布
	QCPBars*				m_pCPBars;		//柱状图数据体
	static QVector<double>	m_Xtime;		//横坐标数据
	static QVector<double>	m_Yvalue;		//纵坐标数据
private:
	static CCustomPlot*		m_pCCustomPlot;	//自定义取画布的类
};

