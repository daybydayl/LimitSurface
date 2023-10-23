#include "CMainWindow.h"
#include "CItemTree.h"
#include "CGlobal.h"

#include <QHeaderView>


CMainWindow::CMainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    //ui.setupUi(this);
    //��ʼ������
    initComponent();
}

CMainWindow::~CMainWindow()
{}

void CMainWindow::initComponent()
{
    m_pTaos = CTaos::Intoinstance();
    m_pTaos->Init();//��������Ŀ¼��

    //���������ģ�ͷ�������ͼ��
    m_pTreeModel = new CTreeModel(this);
    m_pTreeModel->setTreeData();                //���Ŀ¼��Ϣ
    m_pTreeView = new QTreeView(this);
    m_pTreeView->setHeaderHidden(true);         //����headerdata
    m_pTreeView->setModel(m_pTreeModel);

    //����ͼ���ӵ�startDo�Ĳۣ�˫��Ŀ¼��ȡ����
    connect(m_pTreeView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(startDo(const QModelIndex&)));	//�ź����Ӳۺ���



    //Table,������ı�ģ�ͷ������ͼ�У�����resize���еĿ��
    m_pTableModel = new CTableModel(this);
    m_pTableView = new QTableView(this);
    m_pTableView->setModel(m_pTableModel);//���ģ�ͷ�����ͼ
    m_pChart = new QChart();
    m_pLineseries = new QLineSeries();
    //��ͼ��󶨵���ͼChartView
    m_pChartView = new CChartView(m_pChart);
    /*m_pChartView = new QChartView();//ԭ����ֱ�ӷ�ʽ
    m_pChartView->setChart(m_pChart);*/
    
    m_pSplitter = new QSplitter(Qt::Horizontal, this);
    m_pVSplitter1 = new QSplitter(Qt::Vertical, this);	//�������������֣�����ˮƽ

    //�ұ߷��������¸����ͼ
    m_pVSplitter1->addWidget(m_pTableView);
    m_pVSplitter1->addWidget(m_pChartView);
    m_pVSplitter1->setStretchFactor(0, 1);				//������ͼ��������0�к͵�1����4:7
    m_pVSplitter1->setStretchFactor(1, 8);


    //��������ı���ͼ������ͼ�����������
    m_pSplitter->addWidget(m_pTreeView);
    m_pSplitter->addWidget(m_pVSplitter1);
    m_pSplitter->setStretchFactor(0, 2);				//������ͼ��������0�к͵�1����4:7
    m_pSplitter->setStretchFactor(1, 5);
    setCentralWidget(m_pSplitter);//���õ�����������


}


void CMainWindow::startDo(const QModelIndex& pQModel)
{
    //��ȡ��������Ϣ
    CItemTree* pItemTree;
    pItemTree = static_cast<CItemTree*>(pQModel.internalPointer());
    CGlobal::m_treeName = pItemTree->m_TreeName;//��ȫ��Ŀ¼��
    CGlobal::m_TreeType = pItemTree->m_TreeType;//��ȫ��Ŀ¼����(STorT)


    //�����Ӧ��ҵ
    if (pItemTree->m_TreeType == 1)
    {
        //����ǳ�����
        m_pChartView->hide();//���ػ�ͼ��ͼ
        m_pTaos->STableDirectQueryData();
    }
    else if (pItemTree->m_TreeType == 2)
    {
        //������ӱ�
        m_pTaos->TableDirectQueryData();//���ӱ�����

        m_pChartView->show();
        m_pLineseries->setUseOpenGL(true);//�����Կ����٣�Ч������

        //��������
        for (int i = 0; i < 100; i++)//����ʵ����ȥCTaos::TableDirectQueryDataȡ��ע��
            CTaos::m_pChartdata.append(QPointF(i, QRandomGenerator::global()->bounded(100)));//�����ͼ�洢��

        m_pLineseries->replace(CTaos::m_pChartdata);//������������������
        m_pChart->addSeries(m_pLineseries);//��������ӵ�ͼ����
        m_pChart->createDefaultAxes();//����Ĭ�ϵ���
        m_pChart->setTitle("meas1");//����ͼ�����
        //m_pChartView->setRenderHint(QPainter::Antialiasing);//���ÿ������Ⱦ��ʾ
        

    }

    
    m_pTableModel->SetHeader();//��ӱ��ͷ��
    //m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//�Զ������ֿ���п�
    //ˢ�´��ڣ�ֻˢ���޸ĵ�����
    //m_pTableView->viewport()->update();//Qt4�汾���˰汾������
    m_pTableModel->hasChanged();//ˢ�½���


}
