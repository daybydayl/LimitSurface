#include "CEchart.h"
#include "QtWidgetsLearning.h"

//Echart
#include <cstdlib>//包含 srand() 和 rand() 函数的头文件
#include <ctime>

CEchart* CEchart::m_pChart = 0;
QWebEngineView* CEchart::m_pEngView = 0;
QJsonArray CEchart::m_dataLine;
CEchart::CEchart(QWidget* parent) : QMainWindow(parent)
{
	Init();
}

CEchart::~CEchart()
{
}

CEchart* CEchart::IntoInstance()
{
	if (m_pChart == 0)
		m_pChart = new CEchart();
	return m_pChart;
}

void CEchart::ExitInstance()
{
	if (m_pChart)
		delete m_pChart;
	m_pChart = 0;
}

void CEchart::Init()
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);                                    //设置应用程序用高分辨率显示
	this->setWindowTitle(QString::fromLocal8Bit("Qt5中用EChart进行数据可视化Demo"));
	//设置应用标题
	m_pEngView = new QWebEngineView(); 
	
	//ui.webLayout->addWidget(m_pEngView);                                                              //将QWebEngineView加入到Layout中
	

	//m_pEngView->page()->load(QUrl(QString::fromLocal8Bit("D:/rr6000/src/vc/my_program/QtWidgetsLearning/EChartTest.html")));
	m_pEngView->load(QUrl(QString::fromLocal8Bit("D:/My_program/QtWidgetsLearning/EChartTest.html"))); //载入EChart测试网页
	connect(m_pEngView, SIGNAL(loadFinished(bool)), this, SLOT(Linechart()));
	//m_pEngView->show();                                                                               //显示网页
	//ui.pathLEdt->setText(QCoreApplication::applicationDirPath());                                   //设置保存位置为项目所在位置
	//ui.lineChartBtn->setDisabled(true);                                                             //设置折线图按钮不可按
	//ui.curvChartBtn->setDisabled(true);                                                             //设置曲线图按钮不可按
	//ui.barChartBtn->setDisabled(true);
	//Linechart();
	
}

void CEchart::Linechart()
{
	//QJsonValue tv;
	//int i = i = static_cast<float>(rand()) / RAND_MAX + 2;
	int j;
	for (int i = 0; i < 2100; i++)
	{
		//j = static_cast<int>(rand()) % 10;
		m_dataLine.append(qExp(i / 400.0) * qCos(i / 10.0) * 100);
	}
	//ui.lagLab->clear();                                             //清空日志输出显示栏
	QJsonObject jsonObject;                                         //构建用于传输数据的json对象
	//m_saveImgName = QString::fromLocal8Bit("折线图");                 //设置保存图片名称
	jsonObject.insert("lineData", m_dataLine);                        //传入读取数据
	QString str = QString(QJsonDocument(jsonObject).toJson());      //将Json格式转为QString
	str = str.remove("\t").remove('\n');                            //移除多余的制表和换行
	str.replace(QRegExp("\""), "\\\"");                             //因为转为字符串后需要两次转译
	QString strVal = QString("lineChart(\"%1\");").arg(str);        //转为标准字符串
	//m_pEngView->page()->runJavaScript(strVal);                        //调用js中的函数
	QWebEnginePage* aa = m_pEngView->page();
	m_pEngView->page()->runJavaScript(strVal);

	//m_pEngView->show();                                               //显示网页
}

void CEchart::resizeEvent(QResizeEvent* event)
{
	if (m_pEngView)
	{
		m_pEngView->setGeometry(rect());
	}
}

void CEchart::on_inputDataBtn_clicked()
{
	////打开文件
	//ui.lagLab->clear();             //清空日志输出显示栏
	//QString     path = QFileDialog::getOpenFileName(this, "Open File", "", "TXT File(*.txt)");
	//QFile       textData(path);     //数据文件
	//QString     lineStr;            //读取行
	//QStringList dataTemp;           //临时数据
	//if (!textData.open(QIODevice::ReadOnly))
	//{
	//	ui.lagLab->setText(QString::fromLocal8Bit("导入文件失败！"));
	//	return;
	//}
	//while (!textData.atEnd())
	//{
	//	lineStr = textData.readLine();                  //按行读取数据
	//	dataTemp = lineStr.split(' ', QString::SplitBehavior::SkipEmptyParts);
	//	if (dataTemp.count() > 0)
	//	{
	//		m_dataLine.append(dataTemp.at(0).toFloat());  //将读取到的数据转为float加入到Json中
	//	}
	//}
	//
	//textData.close();                                   //关闭文件
	//ui.lineChartBtn->setDisabled(false);																//设置折线图按钮可按
	//ui.curvChartBtn->setDisabled(false);																//设置曲线图按钮可按
	//ui.barChartBtn->setDisabled(false);																	//设置柱状图按钮可按
	//ui.lagLab->setText(QString::fromLocal8Bit("导入文件成功！"));
}

void CEchart::on_lineChartBtn_clicked()
{
	//QJsonValue
	//ui.lagLab->clear();                                             //清空日志输出显示栏
	//QJsonObject jsonObject;                                         //构建用于传输数据的json对象
	//m_saveImgName = QString::fromLocal8Bit("折线图");                 //设置保存图片名称
	//jsonObject.insert("lineData", m_dataLine);                        //传入读取数据
	//QString str = QString(QJsonDocument(jsonObject).toJson());      //将Json格式转为QString
	//str = str.remove("\t").remove('\n');                            //移除多余的制表和换行
	//str.replace(QRegExp("\""), "\\\"");                             //因为转为字符串后需要两次转译
	//QString strVal = QString("lineChart(\"%1\");").arg(str);        //转为标准字符串
	//m_pEngView->page()->runJavaScript(strVal);                        //调用js中的函数
	//m_pEngView->show();                                               //显示网页
}

void CEchart::on_curvChartBtn_clicked()
{
}

void CEchart::on_barChartBtn_clicked()
{
}

void CEchart::on_saveImgBtn_clicked()
{
}

void CEchart::on_browseBtn_clicked()
{
}


