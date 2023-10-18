#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CMainWindow.h"

#include "CCommonHead.h"
#include <QTableView>
#include <QTreeView>
#include <QSplitter>
#include "CTableModel.h"
#include "CTreeModel.h"
#include "CItemTree.h"

#include "CCustomPlot.h"

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CMainWindow(QWidget* parent = nullptr);
    ~CMainWindow();

public:
    void initComponent();//初始化界面布局和类对象

    void addPushButton();//添加1按钮

    void addTimeEdit();//添加时间区间编辑控件

protected slots:
    //目录点击的进出表函数
    void startDo(const QModelIndex&);

    //导出绘图图片槽函数
    void Exportphoto();

    //更新时间区间刷新
    void updatePlotByTimeEdit();

    //指针选中显示值
    void updateTracer(QMouseEvent*);


private:
    //Ui::CMainWindow  ui;
    CTableModel* m_pTableModel;  //自定义的table模型
    QTableView* m_pTableView;   //Qt的表视图模型

    CTreeModel* m_pTreeModel;   //自定义的Tree模型
    QTreeView* m_pTreeView;    //Qt的树视图

    QSplitter* m_pSplitter;	//分束器

    CTaos* m_pTaos;//taos句柄
    //Qcustomplot
    QSplitter* m_pSplitter1;	//分束器
    QSplitter* m_pSplitter2;	//分束器
    CCustomPlot* m_pCCustom;

    //添加按钮
    QWidget* m_pButtonContainer;
    QVBoxLayout* m_pVBoxLayot;//垂直布局的小部件
    QSplitter* m_pSplitter3;
    QPushButton* m_pPushButton1;//按钮1尝试
    QPushButton* m_pPushButton2;

    //添加时间区间
    QDateTimeEdit* m_startTimeEdit;
    QDateTimeEdit* m_endTimeEdit;

    //鼠标移动
    QCPItemTracer* m_tracer;
    QCPItemText* m_textLabel;


};
