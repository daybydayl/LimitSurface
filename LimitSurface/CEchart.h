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
	static CEchart* IntoInstance();					//进入实例
	static void ExitInstance();						//退出实例
	void Init();									//初始化

	
	void resizeEvent(QResizeEvent* event);

public slots:
	void Linechart();
	void on_inputDataBtn_clicked();			//导入数据槽函数
	void on_lineChartBtn_clicked();			//折线图显示槽函数
	void on_curvChartBtn_clicked();			//曲线图显示槽函数
	void on_barChartBtn_clicked();			//柱状图显示槽函数
	void on_saveImgBtn_clicked();			//保存图片槽函数
	void on_browseBtn_clicked();			//设置图片保存路径槽函数

public:
	
	static CEchart* m_pChart;
	static QWebEngineView* m_pEngView;			//Web显示引擎
	static QJsonArray m_dataLine;				//传入Json数据
	QString	m_saveImgName;				//图片保存名称
};

