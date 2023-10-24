#include "CMainWindow.h"
#include "CItemTree.h"
#include "CGlobal.h"

#include <QtWidgets/QHeaderView>


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
    m_pTableView->setModel(m_pTableModel);//表格模型放入视
    
    m_pSplitter = new QSplitter(Qt::Horizontal, this);

    m_pSplitter->addWidget(m_pTreeView);
    m_pSplitter->addWidget(m_pTableView);
    m_pSplitter->setStretchFactor(0, 2);				//设置视图比例，第0列和第1列是4:7
    m_pSplitter->setStretchFactor(1, 7);
    setCentralWidget(m_pSplitter);//设置到主窗口中央


}

void CMainWindow::startDo(const QModelIndex& pQModel)
{
    //获取点击项的信息
    CItemTree* pItemTree;
    pItemTree = static_cast<CItemTree*>(pQModel.internalPointer());
    CGlobal::m_treeName = pItemTree->m_TreeName;
    CGlobal::m_TreeType = pItemTree->m_TreeType;


    if (pItemTree->m_TreeType == 1)
    {
        //如果是超级表
        m_pTaos->STableDirectQueryData();
    }
    else if (pItemTree->m_TreeType == 2)
    {
        //如果是子表
        m_pTaos->TableDirectQueryData();//读子表数据
        
    }

    m_pTableModel->SetHeader();
    m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//自动设置列宽
    //刷新窗口，只刷新修改的数据
    //m_pTableView->viewport()->update();//Qt4版本，此版本不适用
    m_pTableModel->hasChanged();


}
