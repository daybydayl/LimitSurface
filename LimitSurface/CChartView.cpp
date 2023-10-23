#include "CChartView.h"

CChartView::CChartView(QChart* chart, QWidget* parent):QChartView(chart, parent)
{
    // ����QGraphicsSimpleTextItem����ʾ����
    m_coordX = new QGraphicsSimpleTextItem(chart);
    m_coordY = new QGraphicsSimpleTextItem(chart);

    // ������Ƥ��ѡ��
    setRubberBand(QChartView::RectangleRubberBand);
}

CChartView::~CChartView()
{

}

void CChartView::mouseMoveEvent(QMouseEvent* event)
{
    QPointF point = chart()->mapToValue(event->pos());
    m_coordX->setText(QString("X: %1").arg(point.x()));
    m_coordY->setText(QString("Y: %1").arg(point.y()));

    // ����������ʾ��λ��
    m_coordX->setPos(event->pos() + QPoint(25, 0));
    m_coordY->setPos(event->pos() + QPoint(25, 25));  // ��Y������ʾ��X�����·�
}

void CChartView::wheelEvent(QWheelEvent* event)
{
    // ʹ�ù��ַŴ����С
    if (event->delta() > 0) {
        chart()->zoomIn();
    }
    else {
        chart()->zoomOut();
    }
}

void CChartView::mouseReleaseEvent(QMouseEvent* event)
{
    // ��ѡ��������ʾ����
    if (rubberBand() != QChartView::NoRubberBand) {
        QRectF rect = rubberBandRect();
        //qDebug() << "Selected area:" << rect;

        // �����������Ĵ���������ѡ�����������

        setRubberBand(QChartView::NoRubberBand);
    }

    QChartView::mouseReleaseEvent(event);
}

