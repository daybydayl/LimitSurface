#include "CEchart.h"
#include "QtWidgetsLearning.h"

//Echart
#include <cstdlib>//���� srand() �� rand() ������ͷ�ļ�
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
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);                                    //����Ӧ�ó����ø߷ֱ�����ʾ
	this->setWindowTitle(QString::fromLocal8Bit("Qt5����EChart�������ݿ��ӻ�Demo"));
	//����Ӧ�ñ���
	m_pEngView = new QWebEngineView(); 
	
	//ui.webLayout->addWidget(m_pEngView);                                                              //��QWebEngineView���뵽Layout��
	

	//m_pEngView->page()->load(QUrl(QString::fromLocal8Bit("D:/rr6000/src/vc/my_program/QtWidgetsLearning/EChartTest.html")));
	m_pEngView->load(QUrl(QString::fromLocal8Bit("D:/My_program/QtWidgetsLearning/EChartTest.html"))); //����EChart������ҳ
	connect(m_pEngView, SIGNAL(loadFinished(bool)), this, SLOT(Linechart()));
	//m_pEngView->show();                                                                               //��ʾ��ҳ
	//ui.pathLEdt->setText(QCoreApplication::applicationDirPath());                                   //���ñ���λ��Ϊ��Ŀ����λ��
	//ui.lineChartBtn->setDisabled(true);                                                             //��������ͼ��ť���ɰ�
	//ui.curvChartBtn->setDisabled(true);                                                             //��������ͼ��ť���ɰ�
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
	//ui.lagLab->clear();                                             //�����־�����ʾ��
	QJsonObject jsonObject;                                         //�������ڴ������ݵ�json����
	//m_saveImgName = QString::fromLocal8Bit("����ͼ");                 //���ñ���ͼƬ����
	jsonObject.insert("lineData", m_dataLine);                        //�����ȡ����
	QString str = QString(QJsonDocument(jsonObject).toJson());      //��Json��ʽתΪQString
	str = str.remove("\t").remove('\n');                            //�Ƴ�������Ʊ�ͻ���
	str.replace(QRegExp("\""), "\\\"");                             //��ΪתΪ�ַ�������Ҫ����ת��
	QString strVal = QString("lineChart(\"%1\");").arg(str);        //תΪ��׼�ַ���
	//m_pEngView->page()->runJavaScript(strVal);                        //����js�еĺ���
	QWebEnginePage* aa = m_pEngView->page();
	m_pEngView->page()->runJavaScript(strVal);

	//m_pEngView->show();                                               //��ʾ��ҳ
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
	////���ļ�
	//ui.lagLab->clear();             //�����־�����ʾ��
	//QString     path = QFileDialog::getOpenFileName(this, "Open File", "", "TXT File(*.txt)");
	//QFile       textData(path);     //�����ļ�
	//QString     lineStr;            //��ȡ��
	//QStringList dataTemp;           //��ʱ����
	//if (!textData.open(QIODevice::ReadOnly))
	//{
	//	ui.lagLab->setText(QString::fromLocal8Bit("�����ļ�ʧ�ܣ�"));
	//	return;
	//}
	//while (!textData.atEnd())
	//{
	//	lineStr = textData.readLine();                  //���ж�ȡ����
	//	dataTemp = lineStr.split(' ', QString::SplitBehavior::SkipEmptyParts);
	//	if (dataTemp.count() > 0)
	//	{
	//		m_dataLine.append(dataTemp.at(0).toFloat());  //����ȡ��������תΪfloat���뵽Json��
	//	}
	//}
	//
	//textData.close();                                   //�ر��ļ�
	//ui.lineChartBtn->setDisabled(false);																//��������ͼ��ť�ɰ�
	//ui.curvChartBtn->setDisabled(false);																//��������ͼ��ť�ɰ�
	//ui.barChartBtn->setDisabled(false);																	//������״ͼ��ť�ɰ�
	//ui.lagLab->setText(QString::fromLocal8Bit("�����ļ��ɹ���"));
}

void CEchart::on_lineChartBtn_clicked()
{
	//QJsonValue
	//ui.lagLab->clear();                                             //�����־�����ʾ��
	//QJsonObject jsonObject;                                         //�������ڴ������ݵ�json����
	//m_saveImgName = QString::fromLocal8Bit("����ͼ");                 //���ñ���ͼƬ����
	//jsonObject.insert("lineData", m_dataLine);                        //�����ȡ����
	//QString str = QString(QJsonDocument(jsonObject).toJson());      //��Json��ʽתΪQString
	//str = str.remove("\t").remove('\n');                            //�Ƴ�������Ʊ�ͻ���
	//str.replace(QRegExp("\""), "\\\"");                             //��ΪתΪ�ַ�������Ҫ����ת��
	//QString strVal = QString("lineChart(\"%1\");").arg(str);        //תΪ��׼�ַ���
	//m_pEngView->page()->runJavaScript(strVal);                        //����js�еĺ���
	//m_pEngView->show();                                               //��ʾ��ҳ
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


