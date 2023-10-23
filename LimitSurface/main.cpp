#include "CMainWindow.h"
#include <QtWidgets/QApplication>

//声明网络对象
system_net_info dnet_obj;
//声明历史库访问对象
HAccess hds_obj;
//
//声明实时库访问对象
RDB_NET rdb_net;

int main(int argc, char *argv[])
{
	//初始化网络对象
	char str[60];
	strcpy(str, "zj");
	dnet_obj.set_system_net_info(str, DNET_NO);

	rdb_net.set_dnet_object(dnet_obj);
	//初始化历史库访问对象
	if (hds_obj.InitAccess(&dnet_obj, HDS_API_TYPE_SERVICE) != RETNO_SUCCESS)
	{
		return -1;
	}

    QApplication a(argc, argv);
    CMainWindow w;
    w.resize(1920, 1080);
    w.show();
    return a.exec();
	return 0;
}
