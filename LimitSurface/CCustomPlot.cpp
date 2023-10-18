#include "CCustomPlot.h"
#include "CTaos.h"


CCustomPlot* CCustomPlot::m_pCCustomPlot = 0;
QVector<double>	CCustomPlot::m_Xtime;
QVector<double> CCustomPlot::m_Yvalue;
CCustomPlot::CCustomPlot()
{
    m_pCustomPlot = new QCustomPlot();
    m_pCPBars = new QCPBars(m_pCustomPlot->xAxis, m_pCustomPlot->yAxis);//�ڻ�����new��״ͼ��
}

CCustomPlot::~CCustomPlot()
{
}

void CCustomPlot::init()
{
    //m_pCustomPlot->addGraph();//���һ������
    //m_pCustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);//����ק�����ſɵ������
    //m_pCustomPlot->legend->setVisible(true);//��ʾͼ��,Ĭ�������Ͻ�
    //m_pCustomPlot->xAxis->setLabel(QString("X������"));//X��������ʾ
    //m_pCustomPlot->yAxis->setLabel(QString("Y������"));//Y��������ʾ
    ////m_pCustomPlot->xAxis->setRange(0, 100);//��ǰX����ʾ�ķ�Χ��rescaleAxes�����˾ͻ��Զ���
    ////m_pCustomPlot->yAxis->setRange(-10, 10);//��ǰY����ʾ�ķ�Χ

    //m_pCustomPlot->graph(0)->setPen(QPen(Qt::red));//��������0�Ļ�ˢ��ɫΪ��ɫ
    //m_pCustomPlot->graph(0)->rescaleAxes();//���ù�ģ�ᣬ�÷�Χ�������ţ�ʹͼ0��ȫ�ʺ��ڿɼ�����.�����⣬setRange�Ͳ���Ч���ᶯ̬��������


}

void CCustomPlot::Draw()
{
    // ����һ��QCPAxisTickerDateTime����
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    // ��������ʱ���ʽ
    dateTicker->setDateTimeFormat("yyyy-MM-dd hh:mm:ss.zzz");
    // ��X��Ŀ̶ȱ�ǩ����Ϊ����ʱ������
    //m_pCustomPlot->xAxis->setTicker(dateTicker);

    
    
    //m_pCustomPlot->xAxis->setRange(0, 100000);//��ǰX����ʾ�ķ�Χ��rescaleAxes�����˾ͻ��Զ���
    //m_pCustomPlot->yAxis->setRange(0, 100000);//��ǰY����ʾ�ķ�Χ

    //����ͼ����
    /*m_pCustomPlot->addGraph();
    m_pCustomPlot->graph(0)->setData(CCustomPlot::m_Xtime, CCustomPlot::m_Yvalue);*/
    //m_pCustomPlot->graph(0)->rescaleAxes();

    //��״ͼ����
    m_pCPBars->setData(CCustomPlot::m_Xtime, CCustomPlot::m_Yvalue);
    m_pCPBars->rescaleAxes();

    m_pCustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);//����ק�����ſɵ������
    //ˢ��CustomPlotͼ
    m_pCustomPlot->replot();

}


CCustomPlot* CCustomPlot::intoinstance()
{
	if (m_pCCustomPlot == 0)
		m_pCCustomPlot = new CCustomPlot();
	return m_pCCustomPlot;
}

void CCustomPlot::exitinstance()
{
	if (m_pCCustomPlot)
		delete m_pCCustomPlot;
	m_pCCustomPlot = 0;
}
