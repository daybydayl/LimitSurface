#include "CTableModel.h"


CTableModel::CTableModel(QObject* parent) : QAbstractTableModel(parent)
{
	
}

CTableModel::~CTableModel()
{
	
}

void CTableModel::hasChanged()
{
	//reset();Qt4�е�reset�Ѳ����ã����������������Դﵽһ����Ч��

	beginResetModel();
	/*����ǰ�����ı��һЩ����*/
	endResetModel();
}

void CTableModel::SetHeader()
{
	m_Listheader.clear();

	if(1 == CGlobal::m_TreeType)
		m_Listheader << QString("����") << QString("������") << QString("����");
	else if(2 == CGlobal::m_TreeType)
		m_Listheader << QString("ʱ���") << QString("����ֵ");
}

int CTableModel::rowCount(const QModelIndex& parent) const
{
	int nCount;
	/*���Ϊ�����������ж�һ�³�������������*/
	if (CGlobal::m_TreeType == 1)
		nCount = CTaos::m_MeasTagMapSTNtoList[CGlobal::m_treeName].count();
	else if(CGlobal::m_TreeType == 2)
		nCount = CTaos::m_MeasMapTNtoList[CGlobal::m_treeName].count();

	return nCount;
}

int CTableModel::columnCount(const QModelIndex& parent) const
{
	int nCount;

	nCount = m_Listheader.count();//Ӧ ��������
	

	return nCount;
}

QVariant CTableModel::data(const QModelIndex& index, int role) const
{
	

	//���յ���������ʾ
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
	case Qt::DisplayRole://role�������ʾ��ɫ
		{
			if(orientation == Qt::Horizontal)//�����header
			{
				int nCount = m_Listheader.count();
				if (section < nCount)
					return m_Listheader[section];
				return QVariant();
			}
			else if (orientation == Qt::Vertical)//��������������
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
	/*===================�������ҪʱӦ�ýӵ������ڴ�ĵط�=======================*/
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
