#include "CMainWindow.h"
#include "CItemTree.h"
#include "CGlobal.h"


CMainWindow::CMainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    //ui.setupUi(this);
    //初始化窗口
    initComponent();
}

CMainWindow::~CMainWindow()
{}

void CMainWindow::initComponent()
{
    m_pTaos = CTaos::Intoinstance();
    m_pTaos->Init();//读出超表目录树

    //将定义的树模型放入树视图中
    m_pTreeModel = new CTreeModel(this);
    m_pTreeModel->setTreeData();                //添加目录信息
    m_pTreeView = new QTreeView(this);
    m_pTreeView->setHeaderHidden(true);         //隐藏headerdata
    m_pTreeView->setModel(m_pTreeModel);

    //树视图连接到startDo的槽，双击目录获取索引
    connect(m_pTreeView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(startDo(const QModelIndex&)));	//信号连接槽函数



    //Table,将定义的表模型放入表视图中，并且resize行列的宽度
    m_pTableModel = new CTableModel(this);
    m_pTableView = new QTableView(this);
    m_pTableView->setModel(m_pTableModel);//表格模型放入视图
    m_pCCustom = CCustomPlot::intoinstance();//通过自定义的对象获取QCustomPlot绘制
    
    m_pSplitter = new QSplitter(Qt::Horizontal, this);
    m_pVSplitter1 = new QSplitter(Qt::Vertical, this);	//分束器，两部分，这里水平
    m_pHSplitter3 = new QSplitter(Qt::Horizontal, this);



    addPushButton();
    //addTimeEdit();
    m_pHSplitter3->addWidget(m_pCCustom->m_pCustomPlot);
    m_pHSplitter3->addWidget(m_pButtonContainer);
    m_pHSplitter3->setStretchFactor(0, 7);				//设置视图比例，第0列和第1列是4:7
    m_pHSplitter3->setStretchFactor(1, 1);

    //将定定义的表视图和树视图放入分束器中
    m_pVSplitter1->addWidget(m_pTableView);
    m_pVSplitter1->addWidget(m_pHSplitter3);
    m_pVSplitter1->setStretchFactor(0, 4);				//设置视图比例，第0列和第1列是4:7
    m_pVSplitter1->setStretchFactor(1, 6);

    m_pSplitter->addWidget(m_pTreeView);
    m_pSplitter->addWidget(m_pVSplitter1);
    m_pSplitter->setStretchFactor(0, 2);				//设置视图比例，第0列和第1列是4:7
    m_pSplitter->setStretchFactor(1, 7);
    setCentralWidget(m_pSplitter);//设置到主窗口中央


}


void CMainWindow::addPushButton()
{
    // 创建一个容器小部件，方便放入Splitter
    m_pButtonContainer = new QWidget(this);
    // 创建一个垂直布局并添加到容器小部件中
    m_pVBoxLayot = new QVBoxLayout(m_pButtonContainer);
    m_pPushButton1 = new QPushButton(this);
    m_pPushButton1->setText("导出图片");
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

    // 连接信号和槽
    connect(m_startTimeEdit, SIGNAL(QDateTimeEdit::dateTimeChanged()), this, SLOT(updatePlotByTimeEdit()));
    //connect(endTimeEdit, &QDateTimeEdit::dateTimeChanged, this, SLOT(updatePlotByTimeEdit));

}

void CMainWindow::Exportphoto()
{

    bool ret = false;
    ret = m_pCCustom->m_pCustomPlot->savePng("test.png", 800, 500);//导出图片
    if (ret)
    {
        QMessageBox msgBox;
        msgBox.setStyleSheet("QLabel{min-width: 300px; font-size: 24px;} QPushButton{width: 120px; font-size: 18px;}");
        msgBox.setText("图片已成功导出！");
        msgBox.exec();
    }
}

void CMainWindow::updatePlotByTimeEdit()
{
    QDateTime startTime = m_startTimeEdit->dateTime();
    QDateTime endTime = m_endTimeEdit->dateTime();

    // 使用startTime和endTime更新你的图表
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
    //获取点击项的信息
    CItemTree* pItemTree;
    pItemTree = static_cast<CItemTree*>(pQModel.internalPointer());
    CGlobal::m_treeName = pItemTree->m_TreeName;
    CGlobal::m_TreeType = pItemTree->m_TreeType;

    //点击对应作业
    if (pItemTree->m_TreeType == 1)
    {
        //如果是超级表,存ST所有子表数据，并建立子目录
        m_pHSplitter3->hide();
        m_pTaos->STableDirectQueryData();
    }
    else if (pItemTree->m_TreeType == 2)
    {
        m_pHSplitter3->show();
        m_pTaos->TableDirectQueryData();//读子表数据
        

        // 假设你的QCustomPlot对象名为customPlot
        m_tracer = new QCPItemTracer(m_pCCustom->m_pCustomPlot);
        m_tracer->setStyle(QCPItemTracer::tsCrosshair);

        m_textLabel = new QCPItemText(m_pCCustom->m_pCustomPlot);
        m_textLabel->setPositionAlignment(Qt::AlignTop | Qt::AlignHCenter);

        connect(m_pCCustom->m_pCustomPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(updateTracer(QMouseEvent*)));
        //自定义的类中绘制绘图所需图像
        m_pCCustom->Draw();
    }

    m_pTableModel->SetHeader();
    m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//自动设置列宽
    //刷新窗口，只刷新修改的数据
    //m_pTableView->viewport()->update();//Qt4版本，此版本不适用
    m_pTableModel->hasChanged();


}
