#pragma once
#ifndef CCHARTVIEW_H
#define CCHARTVIEW_H

#include <QtCharts/QChartView>
#include <QtCharts/QXYSeries>
QT_CHARTS_USE_NAMESPACE//������������ռ�Ų�����ʾ�޶������

class CChartView : public QChartView {
    Q_OBJECT
public:
    CChartView(QChart* chart, QWidget* parent = nullptr);
    ~CChartView();

protected:
    //���������ʾ����
    void mouseMoveEvent(QMouseEvent* event) override;

    //ʹ�ù��ַŴ����С
    void wheelEvent(QWheelEvent* event) override;
    //��ѡ��������ʾ����
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    QGraphicsSimpleTextItem* m_coordX;
    QGraphicsSimpleTextItem* m_coordY;
};


#endif // ! CCHARTVIEW_H