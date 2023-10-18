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
    void initComponent();//��ʼ�����沼�ֺ������

    void addPushButton();//���1��ť

    void addTimeEdit();//���ʱ������༭�ؼ�

protected slots:
    //Ŀ¼����Ľ�������
    void startDo(const QModelIndex&);

    //������ͼͼƬ�ۺ���
    void Exportphoto();

    //����ʱ������ˢ��
    void updatePlotByTimeEdit();

    //ָ��ѡ����ʾֵ
    void updateTracer(QMouseEvent*);


private:
    //Ui::CMainWindow  ui;
    CTableModel* m_pTableModel;  //�Զ����tableģ��
    QTableView* m_pTableView;   //Qt�ı���ͼģ��

    CTreeModel* m_pTreeModel;   //�Զ����Treeģ��
    QTreeView* m_pTreeView;    //Qt������ͼ

    QSplitter* m_pSplitter;	//������

    CTaos* m_pTaos;//taos���
    //Qcustomplot
    QSplitter* m_pSplitter1;	//������
    QSplitter* m_pSplitter2;	//������
    CCustomPlot* m_pCCustom;

    //��Ӱ�ť
    QWidget* m_pButtonContainer;
    QVBoxLayout* m_pVBoxLayot;//��ֱ���ֵ�С����
    QSplitter* m_pSplitter3;
    QPushButton* m_pPushButton1;//��ť1����
    QPushButton* m_pPushButton2;

    //���ʱ������
    QDateTimeEdit* m_startTimeEdit;
    QDateTimeEdit* m_endTimeEdit;

    //����ƶ�
    QCPItemTracer* m_tracer;
    QCPItemText* m_textLabel;


};
