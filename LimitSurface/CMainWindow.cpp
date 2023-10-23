#include "CMainWindow.h"
#include "CItemTree.h"
#include "CGlobal.h"


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
    m_pCCustom = CCustomPlot::intoinstance();//ͨ���Զ���Ķ����ȡQCustomPlot����
    
    m_pSplitter = new QSplitter(Qt::Horizontal, this);
    m_pVSplitter1 = new QSplitter(Qt::Vertical, this);	//�������������֣�����ˮƽ
    m_pHSplitter3 = new QSplitter(Qt::Horizontal, this);



    addPushButton();
    //addTimeEdit();
    m_pHSplitter3->addWidget(m_pCCustom->m_pCustomPlot);
    m_pHSplitter3->addWidget(m_pButtonContainer);
    m_pHSplitter3->setStretchFactor(0, 7);				//������ͼ��������0�к͵�1����4:7
    m_pHSplitter3->setStretchFactor(1, 1);

    //��������ı���ͼ������ͼ�����������
    m_pVSplitter1->addWidget(m_pTableView);
    m_pVSplitter1->addWidget(m_pHSplitter3);
    m_pVSplitter1->setStretchFactor(0, 4);				//������ͼ��������0�к͵�1����4:7
    m_pVSplitter1->setStretchFactor(1, 6);

    m_pSplitter->addWidget(m_pTreeView);
    m_pSplitter->addWidget(m_pVSplitter1);
    m_pSplitter->setStretchFactor(0, 2);				//������ͼ��������0�к͵�1����4:7
    m_pSplitter->setStretchFactor(1, 7);
    setCentralWidget(m_pSplitter);//���õ�����������


}


void CMainWindow::addPushButton()
{
    // ����һ������С�������������Splitter
    m_pButtonContainer = new QWidget(this);
    // ����һ����ֱ���ֲ���ӵ�����С������
    m_pVBoxLayot = new QVBoxLayout(m_pButtonContainer);
    m_pPushButton1 = new QPushButton(this);
    m_pPushButton1->setText("����ͼƬ");
    //m_pPushButton2 = new QPushButton(this);
    m_pVBoxLayot->addWidget(m_pPushButton1);
    //m_pVBoxLayot->addWidget(m_pPushButton2);

    connect(m_pPushButton1, SIGNAL(clicked()), this, SLOT(Exportphoto()));

}

void CMainWindow::addTimeEdit()
{
    m_startTimeEdit = new QDateTimeEdit(this);
    m_endTimeEdit = new QDateTimeEdit(this);
    m_startTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    m_endTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");

    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(m_startTimeEdit);
    layout->addWidget(m_endTimeEdit);

    m_pButtonContainer = new QWidget(this);
    m_pButtonContainer->setLayout(layout);

    // �����źźͲ�
    connect(m_startTimeEdit, SIGNAL(QDateTimeEdit::dateTimeChanged()), this, SLOT(updatePlotByTimeEdit()));
    //connect(endTimeEdit, &QDateTimeEdit::dateTimeChanged, this, SLOT(updatePlotByTimeEdit));

}

void CMainWindow::Exportphoto()
{

    bool ret = false;
    ret = m_pCCustom->m_pCustomPlot->savePng("test.png", 800, 500);//����ͼƬ
    if (ret)
    {
        QMessageBox msgBox;
        msgBox.setStyleSheet("QLabel{min-width: 300px; font-size: 24px;} QPushButton{width: 120px; font-size: 18px;}");
        msgBox.setText("ͼƬ�ѳɹ�������");
        msgBox.exec();
    }
}

void CMainWindow::updatePlotByTimeEdit()
{
    QDateTime startTime = m_startTimeEdit->dateTime();
    QDateTime endTime = m_endTimeEdit->dateTime();

    // ʹ��startTime��endTime�������ͼ��
}

void CMainWindow::updateTracer(QMouseEvent* event)
{
    double x = m_pCCustom->m_pCustomPlot->xAxis->pixelToCoord(event->pos().x());
    double y = m_pCCustom->m_pCustomPlot->yAxis->pixelToCoord(event->pos().y());

    m_tracer->setGraphKey(x);
    m_tracer->updatePosition();

    m_textLabel->setText(QString("X: %1\nY: %2").arg(x).arg(y));
    m_textLabel->position->setCoords(QPointF(x, y));
    m_textLabel->setFont(QFont(font().family(), 10));

    m_pCCustom->m_pCustomPlot->replot();
}

void CMainWindow::startDo(const QModelIndex& pQModel)
{
    //��ȡ��������Ϣ
    CItemTree* pItemTree;
    pItemTree = static_cast<CItemTree*>(pQModel.internalPointer());
    CGlobal::m_treeName = pItemTree->m_TreeName;
    CGlobal::m_TreeType = pItemTree->m_TreeType;

    //�����Ӧ��ҵ
    if (pItemTree->m_TreeType == 1)
    {
        //����ǳ�����,��ST�����ӱ����ݣ���������Ŀ¼
        m_pHSplitter3->hide();
        m_pTaos->STableDirectQueryData();
    }
    else if (pItemTree->m_TreeType == 2)
    {
        m_pHSplitter3->show();
        m_pTaos->TableDirectQueryData();//���ӱ�����
        

        // �������QCustomPlot������ΪcustomPlot
        m_tracer = new QCPItemTracer(m_pCCustom->m_pCustomPlot);
        m_tracer->setStyle(QCPItemTracer::tsCrosshair);

        m_textLabel = new QCPItemText(m_pCCustom->m_pCustomPlot);
        m_textLabel->setPositionAlignment(Qt::AlignTop | Qt::AlignHCenter);

        connect(m_pCCustom->m_pCustomPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(updateTracer(QMouseEvent*)));
        //�Զ�������л��ƻ�ͼ����ͼ��
        m_pCCustom->Draw();
    }

    m_pTableModel->SetHeader();
    m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//�Զ������п�
    //ˢ�´��ڣ�ֻˢ���޸ĵ�����
    //m_pTableView->viewport()->update();//Qt4�汾���˰汾������
    m_pTableModel->hasChanged();


}
