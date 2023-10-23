#pragma once
#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QtWidgets/QMainWindow>
//#include "ui_CMainWindow.h"

#include <qtwidgetsglobal.h>

#include <QTableView>
#include <QTreeView>
#include <QSplitter>

//ͼ���ְ������ļ�
#include <QtCharts/QChart>
//#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView> 
#include <QtCharts/QLegend>
#include "CChartView.h"
QT_CHARTS_USE_NAMESPACE//������������ռ�Ų�����ʾ�޶������


#include "CTableModel.h"
#include "CTreeModel.h"
#include "CTaos.h"


class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CMainWindow(QWidget* parent = nullptr);
    ~CMainWindow();

public:
    void initComponent();//��ʼ�����沼�ֺ������

protected slots:
    //Ŀ¼����Ľ�������
    void startDo(const QModelIndex&);



private:
    //Ui::CMainWindow  ui;
    CTableModel* m_pTableModel;     //�Զ����tableģ��
    QTableView* m_pTableView;       //Qt�ı���ͼģ��

    CTreeModel* m_pTreeModel;       //�Զ����Treeģ��
    QTreeView* m_pTreeView;         //Qt������ͼ

    QSplitter* m_pSplitter;	        //��������

    CTaos* m_pTaos;                 //taos���

    //QChart
    QSplitter* m_pVSplitter1;	    //������
    QChart* m_pChart;
    QLineSeries* m_pLineseries;//QLineSeries����
    //QChartView* m_pChartView;
    CChartView* m_pChartView;//�Զ���̳�QChartView����




};
#endif // !CMAINWINDOW_H