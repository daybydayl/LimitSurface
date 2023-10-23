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
    CTableModel* m_pTableModel;  //自定义的table模型
    QTableView* m_pTableView;   //Qt的表视图模型

    CTreeModel* m_pTreeModel;   //自定义的Tree模型
    QTreeView* m_pTreeView;    //Qt的树视图

    QSplitter* m_pSplitter;	//分束器

    CTaos* m_pTaos;//taos句柄

};
