#include "CGlobal.h"

CGlobal* CGlobal::m_pGlobal = 0;

int		CGlobal::m_FacId = 0;						//ʶ���ĸ���վ
int		CGlobal::m_TableType= 0 ;					//ʶ�������ֱ�
int		CGlobal::m_TorS = 0;							//ʶ����ͨ���ǳ�����
QString	CGlobal::m_treeName;						//Ŀ¼id
QString	CGlobal::m_TreeType;
CGlobal::CGlobal()
{
}

CGlobal::~CGlobal()
{
}

CGlobal* CGlobal::IntoInstance()
{
	if (!m_pGlobal)
		m_pGlobal = new CGlobal;
	return m_pGlobal;
}

void CGlobal::ExitInstance()
{
	if (m_pGlobal)
		delete m_pGlobal;
	m_pGlobal = 0;
}

void CGlobal::Init()
{

}

void CGlobal::readFacTable()
{

}

void CGlobal::ReadDataModel()
{

}

void CGlobal::UpdateDataTable()
{
}
