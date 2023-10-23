#pragma once
#ifndef CCHARTVIEW_H
#define CCHARTVIEW_H

#include <QtCharts/QChartView>
#include <QtCharts/QXYSeries>
QT_CHARTS_USE_NAMESPACE//加上这个命名空间才不会显示无定义基类

class CChartView : public QChartView {
    Q_OBJECT
public:
    CChartView(QChart* chart, QWidget* parent = nullptr);
    ~CChartView();

protected:
    //鼠标数据显示跟随
    void mouseMoveEvent(QMouseEvent* event) override;

    //使用滚轮放大和缩小
    void wheelEvent(QWheelEvent* event) override;
    //在选定区域显示数据
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    QGraphicsSimpleTextItem* m_coordX;
    QGraphicsSimpleTextItem* m_coordY;
};


#endif // ! CCHARTVIEW_H