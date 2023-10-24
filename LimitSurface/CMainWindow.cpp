#include "CMainWindow.h"
#include "CItemTree.h"
#include "CGlobal.h"

#include <QtWidgets/QHeaderView>


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
    m_pTableView->setModel(m_pTableModel);//���ģ�ͷ�����
    
    m_pSplitter = new QSplitter(Qt::Horizontal, this);

    m_pSplitter->addWidget(m_pTreeView);
    m_pSplitter->addWidget(m_pTableView);
    m_pSplitter->setStretchFactor(0, 2);				//������ͼ��������0�к͵�1����4:7
    m_pSplitter->setStretchFactor(1, 7);
    setCentralWidget(m_pSplitter);//���õ�����������


}

void CMainWindow::startDo(const QModelIndex& pQModel)
{
    //��ȡ��������Ϣ
    CItemTree* pItemTree;
    pItemTree = static_cast<CItemTree*>(pQModel.internalPointer());
    CGlobal::m_treeName = pItemTree->m_TreeName;
    CGlobal::m_TreeType = pItemTree->m_TreeType;


    if (pItemTree->m_TreeType == 1)
    {
        //����ǳ�����
        m_pTaos->STableDirectQueryData();
    }
    else if (pItemTree->m_TreeType == 2)
    {
        //������ӱ�
        m_pTaos->TableDirectQueryData();//���ӱ�����
        
    }

    m_pTableModel->SetHeader();
    m_pTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//�Զ������п�
    //ˢ�´��ڣ�ֻˢ���޸ĵ�����
    //m_pTableView->viewport()->update();//Qt4�汾���˰汾������
    m_pTableModel->hasChanged();


}
