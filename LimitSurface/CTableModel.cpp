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

void CTableModel::setheader(QStringList	&Listheader)
{
	m_Listheader = Listheader;
}

int CTableModel::rowCount(const QModelIndex& parent) const
{
	int nCount;
	/*���Ϊ�������������ж�һ�³�������������*/
	
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
					return CTaos::m_MeasMapTNtoList[CGlobal::m_treeName][nRow]->m_date;
				}
			case 1:
				{
					return CTaos::m_MeasMapTNtoList[CGlobal::m_treeName][nRow]->m_value;
				}
			case 2:
				{
					return CTaos::m_MeasMapTNtoList[CGlobal::m_treeName][nRow]->m_Smeas_name_TAG;
				}
			break;
			case 3:
				{
					return CTaos::m_MeasMapTNtoList[CGlobal::m_treeName][nRow]->m_Smeas_type_TAG;
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