#include "CGlobal.h"

////声明网络对象
//extern system_net_info dnet_obj;
////声明历史库访问对象
//extern HAccess hds_obj;
////声明实时库访问对象
//extern RDB_NET rdb_net;

CGlobal* CGlobal::m_pGlobal = 0;

int		CGlobal::m_FacId = 0;						//识别哪个厂站
int		CGlobal::m_TableType= 0 ;					//识别是哪种表
int		CGlobal::m_TorS = 0;							//识别普通表还是超级表
QString	CGlobal::m_treeName;						//目录id
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
