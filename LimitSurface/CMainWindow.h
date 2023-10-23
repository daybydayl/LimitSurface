#pragma once
#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QtWidgets/QMainWindow>
//#include "ui_CMainWindow.h"

#include <qtwidgetsglobal.h>

#include <QTableView>
#include <QTreeView>
#include <QSplitter>

//图部分包含的文件
#include <QtCharts/QChart>
//#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView> 
#include <QtCharts/QLegend>
#include "CChartView.h"
QT_CHARTS_USE_NAMESPACE//加上这个命名空间才不会显示无定义基类


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
    void initComponent();//初始化界面布局和类对象

protected slots:
    //目录点击的进出表函数
    void startDo(const QModelIndex&);



private:
    //Ui::CMainWindow  ui;
    CTableModel* m_pTableModel;     //自定义的table模型
    QTableView* m_pTableView;       //Qt的表视图模型

    CTreeModel* m_pTreeModel;       //自定义的Tree模型
    QTreeView* m_pTreeView;         //Qt的树视图

    QSplitter* m_pSplitter;	        //主分束器

    CTaos* m_pTaos;                 //taos句柄

    //QChart
    QSplitter* m_pVSplitter1;	    //分束器
    QChart* m_pChart;
    QLineSeries* m_pLineseries;//QLineSeries对象
    //QChartView* m_pChartView;
    CChartView* m_pChartView;//自定义继承QChartView的类




};
#endif // !CMAINWINDOW_H