#include "CTableModel.h"


CTableModel::CTableModel(QObject* parent) : QAbstractTableModel(parent)
{
	
}

CTableModel::~CTableModel()
{
	
}

void CTableModel::hasChanged()
{
	//reset();Qt4中的reset已不能用，这里下面两条可以达到一样的效果

	beginResetModel();
	/*重置前想做的别的一些操作*/
	endResetModel();
}

void CTableModel::setheader(QStringList	&Listheader)
{
	m_Listheader = Listheader;
}

int CTableModel::rowCount(const QModelIndex& parent) const
{
	int nCount;
	/*如果为超级表，这里判断一下超级表所有行数*/
	if (CGlobal::m_TreeType == 1)
		nCount = CTaos::m_MeasTagMapSTNtoList[CGlobal::m_treeName].count();
	else if(CGlobal::m_TreeType == 2)
		nCount = CTaos::m_MeasMapTNtoList[CGlobal::m_treeName].count();

	return nCount;
}

int CTableModel::columnCount(const QModelIndex& parent) const
{
	int nCount;

	nCount = m_Listheader.count();//应 数据域数
	

	return nCount;
}

QVariant CTableModel::data(const QModelIndex& index, int role) const
{
	

	//用收到的数据显示
	switch(role)
	{
	case Qt::DisplayRole:
		{
			int nRow = index.row();
			int nCol = index.column();
			
			switch(nCol)
			{
			case 0:
				{
				if (CGlobal::m_TreeType == 1)
					return CTaos::m_MeasTagMapSTNtoList[CGlobal::m_treeName][nRow]->m_tbname;
				else if(CGlobal::m_TreeType == 2)
					return CTaos::m_MeasMapTNtoList[CGlobal::m_treeName][nRow]->m_date;
				}
			case 1:
				{
				if (CGlobal::m_TreeType == 1)
					return CTaos::m_MeasTagMapSTNtoList[CGlobal::m_treeName][nRow]->m_meas_name;
				else if (CGlobal::m_TreeType == 2)
					return CTaos::m_MeasMapTNtoList[CGlobal::m_treeName][nRow]->m_value;
				}
			case 2:
				{
				if (CGlobal::m_TreeType == 1)
					return CTaos::m_MeasTagMapSTNtoList[CGlobal::m_treeName][nRow]->m_meas_type;
				else if (CGlobal::m_TreeType == 2)
					return 0;
				}
			break;
			case 3:
				{
					return 0;
				}
				break;
			default:
				break;
			}

		}
		break;
	default:
		break;
	}

	return QVariant();
}

QVariant CTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	switch (role)
	{
	case Qt::DisplayRole://role如果是显示角色
		{
			if(orientation == Qt::Horizontal)//行输出header
			{
				int nCount = m_Listheader.count();
				if (section < nCount)
					return m_Listheader[section];
				return QVariant();
			}
			else if (orientation == Qt::Vertical)//列输出项序号自增
			{
				return QString::number(section + 1);
			}
		}
		break;
	default:
		break;
	}

	return QVariant();
}
