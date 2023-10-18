#pragma once

#include "CCommonHead.h"
#include <QtWidgets/QMainWindow>
#include <qdebug.h>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebEngineWidgets/QWebEnginePage>
#include <qfile.h>
#include <qfiledialog.h>
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <qjsondocument.h>

class CEchart : public QMainWindow
{
	Q_OBJECT
public:
	CEchart(QWidget* parent = Q_NULLPTR);
	~CEchart();

public:
	static CEchart* IntoInstance();					//����ʵ��
	static void ExitInstance();						//�˳�ʵ��
	void Init();									//��ʼ��

	
	void resizeEvent(QResizeEvent* event);

public slots:
	void Linechart();
	void on_inputDataBtn_clicked();			//�������ݲۺ���
	void on_lineChartBtn_clicked();			//����ͼ��ʾ�ۺ���
	void on_curvChartBtn_clicked();			//����ͼ��ʾ�ۺ���
	void on_barChartBtn_clicked();			//��״ͼ��ʾ�ۺ���
	void on_saveImgBtn_clicked();			//����ͼƬ�ۺ���
	void on_browseBtn_clicked();			//����ͼƬ����·���ۺ���

public:
	
	static CEchart* m_pChart;
	static QWebEngineView* m_pEngView;			//Web��ʾ����
	static QJsonArray m_dataLine;				//����Json����
	QString	m_saveImgName;				//ͼƬ��������
};

