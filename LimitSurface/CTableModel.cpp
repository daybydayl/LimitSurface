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

void CTableModel::SetHeader()
{
	m_Listheader.clear();

	if(1 == CGlobal::m_TreeType)
		m_Listheader << QString("表名") << QString("量测名") << QString("类型");
	else if(2 == CGlobal::m_TreeType)
		m_Listheader << QString("时间戳") << QString("量测值");
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

bool CTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	/*===================反填函数需要时应该接到回填内存的地方=======================*/
	int      nRow    = 0;
	int      nCol    = 0;
	CTaosMeasTag* pTaosMeasTag = NULL;
	

	nRow = index.row();
	nCol = index.column();

	pTaosMeasTag = CTaos::m_MeasTagMapSTNtoList[CGlobal::m_treeName][nRow];

	if(0 == pTaosMeasTag)
	{
		return false;
	}

	switch(nCol)
	{
	case 0:
		{
			pTaosMeasTag->m_tbname = value.toString();
		}
		break;

	case 1:
		{
			int nValue = 0;
			nValue = value.toInt();
			//pNodeSw->m_chValue = (char)nValue;
			pTaosMeasTag->m_meas_name = value.toString();
		}
		break;
	case 2:
		{
			pTaosMeasTag->m_meas_type = value.toInt();
		}
		break;

	default:
		break;
	}

	return true;
}

Qt::ItemFlags CTableModel::flags(const QModelIndex& index) const
{
	Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

	if (index.isValid())
	{
		return defaultFlags | Qt::ItemIsEditable | Qt::ItemIsEnabled;
	}

	return defaultFlags;
}
