#include "CTaos.h"
#include <stdio.h>
#include <stdlib.h>

#include "CItemTree.h"
#include "CTreeModel.h"

CTaos* CTaos::m_pCTaos = 0;
TAOS* CTaos::m_taos = 0;

QList<CTaosMeasTable*>	CTaos::m_listMeasTableNode;
QMap<QString, QList<CTaosMeasTable*>>	CTaos::m_MeasMapTNtoList;
QMap<QString, QMap<QString, QList<CTaosMeasTable*>>> CTaos::m_MeasMapSTNtoList;
CTaos::CTaos()
{
	//m_taos = taos_connect("Linx.Linx-lzj", "root", "taosdata", NULL, 6030);//��˼���
	m_taos = taos_connect("lzj-VM", "root", "taosdata", NULL, 6030);          //unbuntu���
	if (m_taos == NULL) {
		printf("failed to connect to server\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("connect success����\n");
	}
}

CTaos::~CTaos()
{
	taos_close(m_taos);
	taos_cleanup();
}

CTaos* CTaos::Intoinstance()
{
	if (!m_pCTaos)
		m_pCTaos = new CTaos;
	return m_pCTaos;
}

void CTaos::Exitinstance()
{
	if (m_pCTaos)
		delete m_pCTaos;
	m_pCTaos = 0;
}

void CTaos::Init()
{
    
	

    /*�������ݿ�*/
	CommonExeDB();
	//SynReadTable();

	ReadSTTree();

	/*if (1 == CGlobal::m_TreeType)
		STableDirectQueryData();
	else if(2 == CGlobal::m_TreeType)
		TableDirectQueryData();*/
	//test();




}

int CTaos::Syn_executeSQL(TAOS* taos, const char* sql)
{
	int nRet = -1;
	TAOS_RES* res = taos_query(taos, sql);
	nRet = taos_errno(res);
	if (nRet != 0)
	{
		printf("Error code: %d; Message: %s\n", nRet, taos_errstr(res));
		taos_free_result(res);
		return nRet;
	}
	return nRet;
}

int CTaos::taos_analysis_row(TAOS_ROW row, TAOS_FIELD* fields, int num_fields)
{
	int offset = 0;
	CTaosMeasTable* pTaosMeasTable;//����ת�����������
	pTaosMeasTable = CTaosMeasTable::getMeasNodebyPool();
	char nstr[64] = { 0 };
	//char nstr1[64] = { 0 };
		for (int i = 0; i < num_fields; ++i) {

		switch (fields[i].type) {
		case TSDB_DATA_TYPE_TIMESTAMP:
			{
				sprintf_s(nstr, sizeof(nstr), "%lld", *(int64_t*)row[i]);
				pTaosMeasTable->m_ts = QString(nstr).toDouble();
				offset += 8;
			}
			break;

		case TSDB_DATA_TYPE_DOUBLE: 
			{
				/*dv = GET_DOUBLE_VAL(row[i]);
				len += sprintf_s(str + len, "%lf", dv);*/
				sprintf_s(nstr, sizeof(nstr), "%lf", *((double*)row[i]));
				pTaosMeasTable->m_value = QString(nstr).toDouble();
				offset += 8;
			} 
			break;
		default:
			break;
		}
	}
	if (m_MeasMapTNtoList.contains(CGlobal::m_treeName))
		m_MeasMapTNtoList[CGlobal::m_treeName].append(pTaosMeasTable);
	else
	{
		m_listMeasTableNode.append(pTaosMeasTable);
		m_MeasMapTNtoList.insert(CGlobal::m_treeName, m_listMeasTableNode);
	}


	//Customplot����������������
	//CCustomPlot::m_Xtime.push_back(pointnum++);
	int Xnum = pTaosMeasTable->m_ts;
	CCustomPlot::m_Xtime.push_back(Xnum);
	CCustomPlot::m_Yvalue.push_back(pTaosMeasTable->m_value);

	return offset;
}

int CTaos::Syn_QueryStructDataBySQL(TAOS* taos, const char* sql, TAOS_ROW &data_rows, TAOS_FIELD* &taosfields_info, int &fields_num, int &records_num)
{
	int nRet = -1;//0Ϊ����

	//ͨ��sql����ȡ�����
	TAOS_RES* res = taos_query(taos, sql);
	nRet = taos_errno(res);
	if (nRet != 0)
	{
		printf("Error code: %d; Message: %s\n", nRet, taos_errstr(res));
		taos_free_result(res);
		return nRet;
	}
	
	//ͨ���������ȡ����Ϣ
	taosfields_info = taos_fetch_fields(res);

	//ͨ���������ȡ����
	fields_num = taos_field_count(res);

	//ͨ���������ȡ��¼���ͼ�¼���ݼ�
	records_num = taos_fetch_block(res, &data_rows);//ִ�е����������⣡��������������������������������������������

	return nRet;
}

void CTaos::TableDirectQueryData()
{
	//CTaosMeasTable::resetMeasPool();//��������
	m_listMeasTableNode.clear();//�����ӱ������������
	m_MeasMapTNtoList[CGlobal::m_treeName].clear();//map�ӱ��������
	CCustomPlot::m_Xtime.clear();
	CCustomPlot::m_Yvalue.clear();

	TAOS_RES* res = NULL;								//��ѯ�Ľ����
	TAOS_ROW data_row = NULL;							//һ����¼�Ľ������
	TAOS_ROW* data_rows = new TAOS_ROW;				//���ݿ�
	int records_num = 0;								//��ѯ����ļ�¼����
	QString str = "select * from rr6000.";
	str.append(CGlobal::m_treeName);
	//str.append(" limit 10000");

	res = taos_query(m_taos, str.toStdString().c_str());

	int fields_num = taos_field_count(res);				//��ѯ������������ͬtaos_num_fields
	TAOS_FIELD* taosfields_info = taos_fetch_fields(res);		//��ṹ��Ϣ

	//���ؽ����ʱ����ֶεľ��ȣ�0 ������룬1 ����΢�룬2 �������롣
	int time_pre = taos_result_precision(res);
	double pointnum = 0;
	LONG64 xAis = 0;

	CTaosMeasTable* pTaosMeasTable;//����ת�����������

	//�ִ���ȡ
	QString subchar;
	char split = ' ';
	QString Smilli;
	//������ת��ʱ��
	char curDate[64];
	time_t curtime;
	struct tm* tminfo;//C++�Դ�
	while ((data_row = taos_fetch_row(res))) {				//���л�ȡ��ѯ������е����ݡ�
		char temp[1024] = { 0 };
		taos_print_row(temp, data_row, taosfields_info, fields_num);	//�ٷ�����һ����¼���
		records_num++;
		pTaosMeasTable = CTaosMeasTable::getMeasNodebyPool();
		subchar = strtok(temp, &split);//�ִ���ȡ
		xAis = subchar.toLongLong();
		Smilli = QString::number(subchar.toLongLong() % 1000);//ȡ�����ַ���
		curtime = subchar.toLongLong() / 1000;
		tminfo = localtime(&curtime);//���ĵõ�ʱ��ת�ɽṹ��tm����
		strftime(curDate, 64, "%Y-%m-%d %H:%M:%S", tminfo);//��tm�ṹ����ģ�ͷ����ַ�����
		pTaosMeasTable->m_date.append(curDate);//�����ִ�
		pTaosMeasTable->m_date.append(".");
		pTaosMeasTable->m_date.append(Smilli);//�����ִ�
		subchar = strtok(NULL, &split);
		pTaosMeasTable->m_value = subchar.toDouble();//����ֵ
		if (m_MeasMapTNtoList.contains(CGlobal::m_treeName))
			m_MeasMapTNtoList[CGlobal::m_treeName].append(pTaosMeasTable);
		else
		{
			m_listMeasTableNode.append(pTaosMeasTable);
			m_MeasMapTNtoList.insert(CGlobal::m_treeName, m_listMeasTableNode);
		}
		//Customplot����������������
		//CCustomPlot::m_Xtime.push_back(pointnum++);
		CCustomPlot::m_Xtime.push_back(xAis/1000);
		CCustomPlot::m_Yvalue.push_back(pTaosMeasTable->m_value);
	}

	/*int offset = 0;
	int ret = 0;
	ret = taos_fetch_block(res, data_rows);//block�������
	for (int i = 0; i < ret; i++)
	{
		taos_analysis_row(data_rows[i], taosfields_info, fields_num);
	}*/

	
	

}

void CTaos::STableDirectQueryData()
{
	CTaosMeasTable::resetMeasPool();//��������
	m_listMeasTableNode.clear();//�����ӱ������������
	m_MeasMapTNtoList[CGlobal::m_treeName].clear();//map�ӱ��������
	CCustomPlot::m_Xtime.clear();
	CCustomPlot::m_Yvalue.clear();

	TAOS_RES* res = NULL;								//��ѯ�Ľ����
	TAOS_ROW data_row = NULL;							//һ����¼�Ľ������
	int records_num = 0;								//��ѯ����ļ�¼����
	QString str = "select * from rr6000.";
	str.append(CGlobal::m_treeName);
	str.append(" limit 10000");

	res = taos_query(m_taos, str.toStdString().c_str());

	int fields_num = taos_field_count(res);				//��ѯ������������ͬtaos_num_fields
	TAOS_FIELD* taosfields_info = taos_fetch_fields(res);		//��ṹ��Ϣ

	//���ؽ����ʱ����ֶεľ��ȣ�0 ������룬1 ����΢�룬2 �������롣
	int time_pre = taos_result_precision(res);

	//�ִ���ȡ
	QString subchar;
	char split = ' ';
	
	//������ת��ʱ��
	char curDate[64];
	time_t curtime;
	struct tm* tminfo;//C++�Դ�
	QString Smilli;


	CTaosMeasTable* pTaosMeasTable;//����ת�����������

	double pointnum = 0;
	while ((data_row = taos_fetch_row(res))) {				//���л�ȡ��ѯ������е����ݡ�
		char temp[1024] = { 0 };
		taos_print_row(temp, data_row, taosfields_info, fields_num);	//�ٷ�����һ����¼���
		records_num++;

		pTaosMeasTable = CTaosMeasTable::getMeasNodebyPool();

		subchar = strtok(temp, &split);//�ִ���ȡ
		Smilli = QString::number(subchar.toLongLong() % 1000);//ȡ�����ַ���
		curtime = subchar.toLongLong() / 1000;
		tminfo = localtime(&curtime);//���ĵõ�ʱ��ת�ɽṹ��tm����
		strftime(curDate, 64, "%Y-%m-%d %H:%M:%S", tminfo);//��tm�ṹ����ģ�ͷ����ַ�����
		pTaosMeasTable->m_date.append(curDate);//�����ִ�
		pTaosMeasTable->m_date.append(".");
		pTaosMeasTable->m_date.append(Smilli);//�����ִ�

		subchar = strtok(NULL, &split);
		pTaosMeasTable->m_value = subchar.toDouble();//����ֵ

		subchar = strtok(NULL, &split);
		pTaosMeasTable->m_Smeas_name_TAG = subchar;//tag

		subchar = strtok(NULL, &split);
		pTaosMeasTable->m_Smeas_type_TAG = subchar.toInt();


		//�������޸ü�¼���ӱ�����map���о�append
		if (m_MeasMapTNtoList.contains(pTaosMeasTable->m_Smeas_name_TAG))
		{
			m_MeasMapTNtoList[pTaosMeasTable->m_Smeas_name_TAG].append(pTaosMeasTable);
			CItemTree* pItemTree = CTreeModel::getItemTreebyPool();
			pItemTree->m_TreeName = pTaosMeasTable->m_Smeas_name_TAG;
			pItemTree->m_TreeType = 2;
			CTreeModel::m_listTree.append(pItemTree);
		}
		else
		{
			//���޾����������ٿ�ʼ���룬�ٰ���������ӱ�map
			m_listMeasTableNode.clear();
			m_listMeasTableNode.append(pTaosMeasTable);
			m_MeasMapTNtoList.insert(pTaosMeasTable->m_Smeas_name_TAG, m_listMeasTableNode);
		}

		//���������map��û�и�st���Ӽ��Ͳ���
		if (!m_MeasMapSTNtoList.contains(CGlobal::m_treeName))
			m_MeasMapSTNtoList.insert(CGlobal::m_treeName, m_MeasMapTNtoList);

		//Customplot����������������
		//CCustomPlot::m_Xtime.push_back(pointnum++);
		//CCustomPlot::m_Xtime.push_back(pTaosMeasTable->m_date.toDouble());
		CCustomPlot::m_Yvalue.push_back(pTaosMeasTable->m_value);

	}
	
}

void CTaos::test()
{
	CTaosMeasTable::resetMeasPool();//��������
	m_listMeasTableNode.clear();
	m_MeasMapTNtoList[CGlobal::m_treeName].clear();
	CCustomPlot::m_Xtime.clear();
	CCustomPlot::m_Yvalue.clear();

	TAOS_RES* res = NULL;								//��ѯ�Ľ����
	TAOS_ROW data_row = NULL;								//һ����¼�Ľ������
	int records_num = 0;										//��ѯ����ļ�¼����
	QString str = "select * from rr6000.";
	str.append(CGlobal::m_treeName);
	str.append(" limit 10000");

	res = taos_query(m_taos, str.toStdString().c_str());

	int fields_num = taos_field_count(res);				//��ѯ������������ͬtaos_num_fields
	TAOS_FIELD* taosfields_info = taos_fetch_fields(res);		//��ṹ��Ϣ

	//���ؽ����ʱ����ֶεľ��ȣ�0 ������룬1 ����΢�룬2 �������롣
	int time_pre = taos_result_precision(res);

	//�ִ���ȡ
	/*char* subchar;
	char split = ' ';
	QString mid;*/

	//������ת��ʱ��
	char curDate[64];
	time_t curtime;
	struct tm* tminfo;//C++�Դ�

	QString last3B_ts;//���պ���λ����ʱ��
	QString ts;//��ʱ����һ����¼��ʱ���
	QString value;//��ʱ����һ����¼������ֵ
	QString measname_tag;
	QString meastype_tag;
	CTaosMeasTable* pTaosMeasTable;//����ת�����������

	double pointnum = 0;
	while ((data_row = taos_fetch_row(res))) {				//���л�ȡ��ѯ������е����ݡ�
		char temp[1024] = { 0 };
		taos_print_row(temp, data_row, taosfields_info, fields_num);	//�ٷ�����һ����¼���
		records_num++;

		pTaosMeasTable = CTaosMeasTable::getMeasNodebyPool();

		last3B_ts.clear();
		ts.clear();
		value.clear();//�����ʱ���յı���
		measname_tag.clear();
		meastype_tag.clear();

	
		/*subchar = strtok(temp, &split);//�ִ���ȡ
		pTaosMeasTable->m_date = subchar;
		mid = strtok(NULL, &split);
		pTaosMeasTable->m_value = mid.toDouble();*/
	

		int flag = 1;//�����ж�ȡ���Ĳ�������
		for (int i = 0; temp[i] != 0; i++)
		{
			if (1 == flag)//ȡʱ���
			{
				if (temp[i] != ' ')
					ts.append(temp[i]);
				else
				{
					flag = 2;//��⵽�ո񣬸�ȡ��һ������
					last3B_ts.append(temp[i - 3]);//ȡ����λ�ĺ���
					last3B_ts.append(temp[i - 2]);
					last3B_ts.append(temp[i - 1]);
				}
			}
			else if (2 == flag)
			{
				if (temp[i] != ' ')
					value.append(temp[i]);
				else
					flag = 3;
			}
			else if (3 == flag)
			{
				if (temp[i] != ' ')
					measname_tag.append(temp[i]);
				else
					flag = 4;
			}
			else if (4 == flag)
			{
				if (temp[i] != ' ')
					meastype_tag.append(temp[i]);

			}
				
		}
		pTaosMeasTable = CTaosMeasTable::getMeasNodebyPool();
		pTaosMeasTable->m_ts = ts.toDouble();
		//������ת��ʱ��
		char curDate[64];
		time_t curtime;
		struct tm* tminfo;//C++�Դ�
		curtime = pTaosMeasTable->m_ts/1000;
		tminfo = localtime(&curtime);//���ĵõ�ʱ��ת�ɽṹ��tm����
		strftime(curDate, 64, "%Y-%m-%d %H:%M:%S", tminfo);//��tm�ṹ����ģ�ͷ����ַ�����
		pTaosMeasTable->m_date = curDate;
		pTaosMeasTable->m_date.append(".");
		pTaosMeasTable->m_date.append(last3B_ts);
		pTaosMeasTable->m_value = value.toDouble();

	

		if (m_MeasMapTNtoList.contains(CGlobal::m_treeName))
			m_MeasMapTNtoList[CGlobal::m_treeName].append(pTaosMeasTable);
		else
		{
			m_listMeasTableNode.append(pTaosMeasTable);
			m_MeasMapTNtoList.insert(CGlobal::m_treeName, m_listMeasTableNode);
		}
	
		//Customplot����������������
		CCustomPlot::m_Xtime.push_back(pointnum++);
		CCustomPlot::m_Yvalue.push_back(pTaosMeasTable->m_value);
	
	
	}

			/*�������洢�������ظ���ţ����Ż�*/
}

void CTaos::ReadSTTree()
{
	TAOS_RES* res = NULL;								//��ѯ�Ľ����
	TAOS_ROW data_row = NULL;							//һ����¼�Ľ������
	int records_num = 0;								//��ѯ����ļ�¼����
	QString str = "show rr6000.stables";

	res = taos_query(m_taos, str.toStdString().c_str());

	int fields_num = taos_field_count(res);				//��ѯ������������ͬtaos_num_fields
	TAOS_FIELD* taosfields_info = taos_fetch_fields(res);		//��ṹ��Ϣ


	while ((data_row = taos_fetch_row(res))) {				//���л�ȡ��ѯ������е����ݡ�
		char temp[1024] = { 0 };
		taos_print_row(temp, data_row, taosfields_info, fields_num);	//�ٷ�����һ����¼���
		records_num++;

		CItemTree* pItemTree = CTreeModel::getItemTreebyPool();
		
		pItemTree->m_TreeName = temp;
		pItemTree->m_TreeType = 1;
		CTreeModel::m_listSTree.append(pItemTree);
	}

}

void CTaos::ReadTTree()
{
	CTreeModel::m_listTree.clear();

	TAOS_RES* res = NULL;								//��ѯ�Ľ����
	TAOS_ROW data_row = NULL;							//һ����¼�Ľ������
	int records_num = 0;								//��ѯ����ļ�¼����
	QString str = "show table tags tbname from rr6000.";
	str.append(CGlobal::m_treeName);

	res = taos_query(m_taos, str.toStdString().c_str());

	int fields_num = taos_field_count(res);				//��ѯ������������ͬtaos_num_fields
	TAOS_FIELD* taosfields_info = taos_fetch_fields(res);		//��ṹ��Ϣ


	while ((data_row = taos_fetch_row(res))) {				//���л�ȡ��ѯ������е����ݡ�
		char temp[1024] = { 0 };
		taos_print_row(temp, data_row, taosfields_info, fields_num);	//�ٷ�����һ����¼���
		records_num++;

		CItemTree* pItemTree = CTreeModel::getItemTreebyPool();

		pItemTree->m_TreeName = temp;
		pItemTree->m_TreeType = 2;
		CTreeModel::m_listTree.append(pItemTree);
	}

}


void CTaos::Get_TaosInfo(TAOS* taos)
{
	printf("�ͻ��˰汾Ϊ%s\n", taos_get_client_info());
	//printf("����˰汾Ϊ��%s\n", taos_get_server_info(taos));
}

void CTaos::CommonExeDB()
{
	Get_TaosInfo(m_taos);
	Syn_executeSQL(m_taos, "use rr6000");

}

void CTaos::SynReadTable()
{
	int nRet = -1;
	
	//TAOS_ROW* data_rows = (TAOS_ROW*)malloc(6);		//��¼������,���ȸ���ռ�
	TAOS_ROW data_rows = NULL;
	TAOS_FIELD* taosfields_info;					//����Ϣ����
	int fields_num;									//����
	int records_num;								//����

	nRet = Syn_QueryStructDataBySQL(
		m_taos,//���ݿ���
		"select * from rr6000.meas001",//sql��䣬const char*��
		data_rows,//����buf�������buf��֪����ô����
		taosfields_info,//����Ϣ
		fields_num,//������
		records_num);//��¼����


	double tstemp = 0;		//��ʱ����ʱ���
	double nmeas_value = 0;	//��ʱ��������ֵ

	int recordIdx = 0;
	int fieldIdx = 0;
	
	int tsOffset = 0;//�������ֶ���
	int measvalueOffset = 0 + taosfields_info[1].bytes;//�������ֶ���
	int Onerecordlen = taosfields_info[0].bytes + taosfields_info[1].bytes;
	
	//for (int i = 0; i < records_num; i++) {
	//	char temp[1024] = { 0 };
	//	taos_print_row(temp, data_rows + i, taosfields_info, fields_num);
	//	int j = 0;
	//	//puts(temp);
	//}

	//for (recordIdx = 0; recordIdx < records_num; recordIdx++)
	//{
	//	/*fieldIdx = 0;
	//	memcpy((char*) & tstemp, data_rows + Onerecordlen * recordIdx + tsOffset, taosfields_info[fieldIdx].bytes);
	//	fieldIdx++;
	//	memcpy((char*) & nmeas_value, data_rows + Onerecordlen * recordIdx + measvalueOffset, taosfields_info[fieldIdx].bytes);*/
	//	memcpy((char*)&nmeas_value, data_rows + Onerecordlen * recordIdx + measvalueOffset, taosfields_info[1].bytes);
	//}

	//char ts[1024] = { 0 };
	//for (recordIdx = 0; recordIdx < records_num; recordIdx++)
	//{
	//	taos_print_row(ts, data_rows[recordIdx], taosfields_info, fields_num);	//�ٷ�����һ����¼���
	//}


}
