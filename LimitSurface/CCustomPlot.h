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
	QCustomPlot*			m_pCustomPlot;	//������
	QCPBars*				m_pCPBars;		//��״ͼ������
	static QVector<double>	m_Xtime;		//����������
	static QVector<double>	m_Yvalue;		//����������
private:
	static CCustomPlot*		m_pCCustomPlot;	//�Զ���ȡ��������
};

