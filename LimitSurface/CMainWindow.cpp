#include "CMainWindow.h"
#include "CItemTree.h"
#include "CGlobal.h"

#include <QHeaderView>


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
    m_pChart = new QChart();
    m_pLineseries = new QLineSeries();
    //将图表绑定到视图ChartView
    m_pChartView = new CChartView(m_pChart);
    /*m_pChartView = new QChartView();//原本的直接方式
    m_pChartView->setChart(m_pChart);*/
    
    m_pSplitter = new QSplitter(Qt::Horizontal, this);
    m_pVSplitter1 = new QSplitter(Qt::Vertical, this);	//分束器，两部分，这里水平

    //右边分束器上下给表和图
    m_pVSplitter1->addWidget(m_pTableView);
    m_pVSplitter1->addWidget(m_pChartView);
    m_pVSplitter1->setStretchFactor(0, 1);				//设置视图比例，第0列和第1列是4:7
    m_pVSplitter1->setStretchFactor(1, 8);


    //将定定义的表视图和树视图放入分束器中
    m_pSplitter->addWidget(m_pTreeView);
    m_pSplitter->addWidget(m_pVSplitter1);
    m_pSplitter->setStretchFactor(0, 2);				//设置视图比例，第0列和第1列是4:7
    m_pSplitter->setStretchFactor(1, 5);
    setCentralWidget(m_pSplitter);//设置到主窗口中央


}


void CMainWindow::startDo(const QModelIndex& pQModel)
{
    //获取点击项的信息
    CItemTree* pItemTree;
    pItemTree = static_cast<CItemTree*>(pQModel.internalPointer());
    CGlobal::m_treeName = pItemTree->m_TreeName;//给全局目录名
    CGlobal::m_TreeType = pItemTree->m_TreeType;//给全局目录类型(STorT)


    //点击对应作业
    if (pItemTree->m_TreeType == 1)
    {
        //如果是超级表
        m_pChartView->hide();//隐藏绘图视图
        m_pTaos->STableDirectQueryData();
    }
    else if (pItemTree->m_TreeType == 2)
    {
        //如果是子表
        m_pTaos->TableDirectQueryData();//读子表数据

        m_pChartView->show();
        m_pLineseries->setUseOpenGL(true);//利用显卡加速，效果不错

        //测试数据
        for (int i = 0; i < 100; i++)//用真实数据去CTaos::TableDirectQueryData取消注释
            CTaos::m_pChartdata.append(QPointF(i, QRandomGenerator::global()->bounded(100)));//放入绘图存储区

        m_pLineseries->replace(CTaos::m_pChartdata);//将数据向量赋给序列
        m_pChart->addSeries(m_pLineseries);//将序列添加到图表中
        m_pChart->createDefaultAxes();//创建默认的轴
        m_pChart->setTitle("meas1");//设置图表标题
        //m_pChartView->setRenderHint(QPainter::Antialiasing);//设置抗锯齿渲染提示
        

    }

    
    m_pTableModel->SetHeader();//添加表格头部
    //m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//自动设置字宽的列宽
    //刷新窗口，只刷新修改的数据
    //m_pTableView->viewport()->update();//Qt4版本，此版本不适用
    m_pTableModel->hasChanged();//刷新界面


}
