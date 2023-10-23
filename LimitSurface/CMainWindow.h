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
    void initComponent();//��ʼ�����沼�ֺ������

protected slots:
    //Ŀ¼����Ľ�������
    void startDo(const QModelIndex&);


private:
    //Ui::CMainWindow  ui;
    CTableModel* m_pTableModel;  //�Զ����tableģ��
    QTableView* m_pTableView;   //Qt�ı���ͼģ��

    CTreeModel* m_pTreeModel;   //�Զ����Treeģ��
    QTreeView* m_pTreeView;    //Qt������ͼ

    QSplitter* m_pSplitter;	//������

    CTaos* m_pTaos;//taos���

};
