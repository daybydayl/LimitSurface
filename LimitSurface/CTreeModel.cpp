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
    m_pItemRoot->m_TreeName = QString("TDʱ���");
    m_pItemRoot->setParent(0);
}
CTreeModel::~CTreeModel()
{
    m_listPoolItemTree.clear();
}
void CTreeModel::setTreeData()
{
    
    //��Ŀ¼
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
    CItemTree* pItemParent;//���յ�ǰ���ڵ�������
    CItemTree* pItemChild;//��Ӹ���Ŀ¼���ӽڵ�

    //�Ѹ���ת��Ϊ��ǰ����
    pItemParent = static_cast<CItemTree*>(parent.internalPointer());

    //����Ǹ����ˣ��ͽ��˸���Ŀ¼������m_pItemRoot��Ȼ�Ǹ��������Լ�Ҳ�ý�������
    if (0 == pItemParent)
        return createIndex(0, 0, m_pItemRoot);

    //������Ǹ�������ͨ�������ڸ�Ŀ¼�½�����Ӧ���е�Ŀ¼����
    pItemChild = pItemParent->child(row);
    if (pItemChild)
        return createIndex(row, column, pItemChild);

    return QModelIndex();
}

QModelIndex CTreeModel::parent(const QModelIndex& child) const
{
    CItemTree* pItemChild;//���ո���Ŀ¼�ڵ�
    CItemTree* pItemParent;//ͨ��child��ȡ��Ŀ¼�ĸ��ڵ�
    
    //�Ѵ������ĺ���ת��Ϊ��ǰ���ͣ����ﺢ�಻�����ļ�
    pItemChild = static_cast<CItemTree*>(child.internalPointer());

    if (0 == pItemChild)
        return QModelIndex();

    //��������Ǹ����Ļ�����ֱ�Ӳ����׸�Ŀ¼�����ͺ�
    pItemParent = pItemChild->m_pParent;
    if (m_pItemRoot == pItemParent)
        return createIndex(0, 0, m_pItemRoot);

    //������Ǹ����Ļ���ͨ��������ȡ�������ڵ�Ŀ¼�д�����
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
    //Ŀ¼һ�㶼��1��
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
    //����Ŀ¼����ͷ
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
