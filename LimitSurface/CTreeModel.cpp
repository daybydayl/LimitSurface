#include "CTreeModel.h"

QList<CItemTree*>	CTreeModel::m_listPoolItemTree;
QList<CItemTree*>	CTreeModel::m_listSTree;
QList<CItemTree*>	CTreeModel::m_listTree;
QMap<QString, QList<CItemTree*>>	CTreeModel::m_mapQStoQLTree;
CTreeModel::CTreeModel(QObject* parent)
{
    m_pItemRoot = new CItemTree();
    m_pItemRoot->m_TreeType = 0;
    m_pItemRoot->m_TreeId = 0;
    m_pItemRoot->m_TreeName = QString("TD时序库");
    m_pItemRoot->setParent(0);
}
CTreeModel::~CTreeModel()
{
    m_listPoolItemTree.clear();
}
void CTreeModel::setTreeData()
{
    
    //建目录
    for (int i = 0; i < m_listSTree.count(); i++)
    {
        m_listSTree[i]->setParent(m_pItemRoot);
        m_pItemRoot->addChild(m_listSTree[i]);
        for (int j = 0; j < m_mapQStoQLTree[m_listSTree[i]->m_TreeName].count(); j++)
        {
            m_mapQStoQLTree[m_listSTree[i]->m_TreeName][j]->setParent(m_listSTree[i]);
            m_listSTree[i]->addChild(m_mapQStoQLTree[m_listSTree[i]->m_TreeName][j]);
        }
    }

}
QModelIndex CTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    CItemTree* pItemParent;//接收当前树节点做父类
    CItemTree* pItemChild;//添加该树目录的子节点

    //把父类转换为当前类型
    pItemParent = static_cast<CItemTree*>(parent.internalPointer());

    //如果是根部了，就建了个主目录索引，m_pItemRoot虽然是根，但他自己也得建个索引
    if (0 == pItemParent)
        return createIndex(0, 0, m_pItemRoot);

    //如果不是根部，就通过父类在该目录下建立对应行列的目录索引
    pItemChild = pItemParent->child(row);
    if (pItemChild)
        return createIndex(row, column, pItemChild);

    return QModelIndex();
}

QModelIndex CTreeModel::parent(const QModelIndex& child) const
{
    CItemTree* pItemChild;//接收该树目录节点
    CItemTree* pItemParent;//通过child获取树目录的父节点
    
    //把传进来的孩类转换为当前类型，这里孩类不定是哪级
    pItemChild = static_cast<CItemTree*>(child.internalPointer());

    if (0 == pItemChild)
        return QModelIndex();

    //如果父类是根部的话，就直接插入首个目录索引就好
    pItemParent = pItemChild->m_pParent;
    if (m_pItemRoot == pItemParent)
        return createIndex(0, 0, m_pItemRoot);

    //如果不是根部的话，通过函数获取父类所在的目录行次添入
    if (pItemParent)
    {
        return createIndex(pItemParent->indexOfParent(), 0, pItemParent);
    }

    return QModelIndex();

}

int CTreeModel::rowCount(const QModelIndex& parent) const
{
    CItemTree* pItemParent;
    pItemParent = static_cast<CItemTree*>(parent.internalPointer());
    if (pItemParent)
    {
        int nCount = 0;
        nCount = pItemParent->countChild();
        return nCount;
    }
    return 1;

}

int CTreeModel::columnCount(const QModelIndex& parent) const
{
    //目录一般都是1列
    return 1;
}

QVariant CTreeModel::data(const QModelIndex& index, int role) const
{
    CItemTree* pItemTree;
    pItemTree = static_cast<CItemTree*>(index.internalPointer());

    switch (role)
    {
    case Qt::DisplayRole:
        {
            return pItemTree->m_TreeName;
        }
        break;
    default:
        break;
    }


    return QVariant();
}

QVariant CTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //这里目录不用头
    return QVariant();
}

void CTreeModel::setheadData(QStringList& header)
{
    m_listheader = header;
}

void CTreeModel::resetItemTreePool()
{
    for (int i = 0; i < m_listPoolItemTree.count(); i++)
    {
        m_listPoolItemTree[i]->init();
    }
}

CItemTree* CTreeModel::getItemTreebyPool()
{
    int nCount = 0;
    CItemTree* pItemTree;
    nCount = m_listPoolItemTree.count();

    for (int i = 0; i < nCount; i++)
    {
        pItemTree = m_listPoolItemTree[i];
        if (false == pItemTree->m_IfUse)
        {
            pItemTree->m_IfUse = true;
            return pItemTree;
        }

    }

    pItemTree = new CItemTree;
    pItemTree->m_IfUse = true;
    m_listPoolItemTree.append(pItemTree);

    return pItemTree;
}
