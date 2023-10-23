#include "CMainWindow.h"
#include <QtWidgets/QApplication>

//����������ϵͳͷ�ļ�
#include "D:/rr6000/src/vc/include/dnet_all.h"
//������ʷ����ϵͳͷ�ļ�
#include "D:/rr6000/src/vc/include/hds_all.h"
//����ʵʱ����ϵͳͷ�ļ�
#include "D:/rr6000/src/vc/include/rdb_all.h"

//�����������
system_net_info dnet_obj;
//������ʷ����ʶ���
HAccess hds_obj;
//
//����ʵʱ����ʶ���
RDB_NET rdb_net;

int main(int argc, char *argv[])
{
	//��ʼ���������
	char str[60];
	strcpy(str, "zj");
	dnet_obj.set_system_net_info(str, DNET_NO);

	rdb_net.set_dnet_object(dnet_obj);
	//��ʼ����ʷ����ʶ���
	if (hds_obj.InitAccess(&dnet_obj, HDS_API_TYPE_SERVICE) != RETNO_SUCCESS)
	{
		return -1;
	}

    QApplication a(argc, argv);
    CMainWindow w;
    w.resize(1400, 900);
    w.show();
    return a.exec();
	return 0;
}
