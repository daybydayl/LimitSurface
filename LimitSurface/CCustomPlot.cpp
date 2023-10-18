#include "CCustomPlot.h"
#include "CTaos.h"


CCustomPlot* CCustomPlot::m_pCCustomPlot = 0;
QVector<double>	CCustomPlot::m_Xtime;
QVector<double> CCustomPlot::m_Yvalue;
CCustomPlot::CCustomPlot()
{
    m_pCustomPlot = new QCustomPlot();
    m_pCPBars = new QCPBars(m_pCustomPlot->xAxis, m_pCustomPlot->yAxis);//在画布种new柱状图类
}

CCustomPlot::~CCustomPlot()
{
}

void CCustomPlot::init()
{
    //m_pCustomPlot->addGraph();//添加一条数据
    //m_pCustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);//可拖拽可缩放可点击数据
    //m_pCustomPlot->legend->setVisible(true);//显示图例,默认在右上角
    //m_pCustomPlot->xAxis->setLabel(QString("X轴文字"));//X轴文字显示
    //m_pCustomPlot->yAxis->setLabel(QString("Y轴文字"));//Y轴文字显示
    ////m_pCustomPlot->xAxis->setRange(0, 100);//当前X轴显示的范围，rescaleAxes设置了就会自动给
    ////m_pCustomPlot->yAxis->setRange(-10, 10);//当前Y轴显示的范围

    //m_pCustomPlot->graph(0)->setPen(QPen(Qt::red));//绘制曲线0的画刷颜色为红色
    //m_pCustomPlot->graph(0)->rescaleAxes();//重置规模轴，让范围自行缩放，使图0完全适合于可见区域.加了这，setRange就不生效，会动态帮设置了


}

void CCustomPlot::Draw()
{
    // 创建一个QCPAxisTickerDateTime对象
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    // 设置日期时间格式
    dateTicker->setDateTimeFormat("yyyy-MM-dd hh:mm:ss.zzz");
    // 将X轴的刻度标签设置为日期时间类型
    //m_pCustomPlot->xAxis->setTicker(dateTicker);

    
    
    //m_pCustomPlot->xAxis->setRange(0, 100000);//当前X轴显示的范围，rescaleAxes设置了就会自动给
    //m_pCustomPlot->yAxis->setRange(0, 100000);//当前Y轴显示的范围

    //折线图绘制
    /*m_pCustomPlot->addGraph();
    m_pCustomPlot->graph(0)->setData(CCustomPlot::m_Xtime, CCustomPlot::m_Yvalue);*/
    //m_pCustomPlot->graph(0)->rescaleAxes();

    //柱状图绘制
    m_pCPBars->setData(CCustomPlot::m_Xtime, CCustomPlot::m_Yvalue);
    m_pCPBars->rescaleAxes();

    m_pCustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);//可拖拽可缩放可点击数据
    //刷新CustomPlot图
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
