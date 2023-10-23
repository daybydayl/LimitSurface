#include "CTaos.h"
#include <stdio.h>
#include <stdlib.h>

#include "CItemTree.h"
#include "CTreeModel.h"

CTaos* CTaos::m_pCTaos = 0;
TAOS* CTaos::m_taos = 0;

QList<CTaosMeasTag*>	CTaos::m_listMeasTagNode;
QMap<QString, QList<CTaosMeasTag*>>	CTaos::m_MeasTagMapSTNtoList;

QList<CTaosMeasTable*>	CTaos::m_listMeasTableNode;
QMap<QString, QList<CTaosMeasTable*>>	CTaos::m_MeasMapTNtoList;
QMap<QString, QMap<QString, QList<CTaosMeasTable*>>> CTaos::m_MeasMapSTNtoList;
QVector<QPointF> CTaos::m_pChartdata;
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

	//������Ŀ¼
	ReadSTTree();


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


	////Customplot����������������
	////CCustomPlot::m_Xtime.push_back(pointnum++);
	//int Xnum = pTaosMeasTable->m_ts;
	//CCustomPlot::m_Xtime.push_back(Xnum);
	//CCustomPlot::m_Yvalue.push_back(pTaosMeasTable->m_value);

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
	m_listMeasTableNode.clear();//�����ӱ������������
	m_MeasMapTNtoList[CGlobal::m_treeName].clear();//map�ӱ��������
	/*CCustomPlot::m_Xtime.clear();
	CCustomPlot::m_Yvalue.clear();*/

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
	
	//QChart��ʱ���ձ���
	qreal X = 0;
	qreal Y = 0;
	int x = 0;

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
		X = subchar.toLongLong();//����X������ֵ
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
		////Customplot����������������
		////CCustomPlot::m_Xtime.push_back(pointnum++);
		//CCustomPlot::m_Xtime.push_back(xAis/1000);
		//CCustomPlot::m_Yvalue.push_back(pTaosMeasTable->m_value);
		Y = pTaosMeasTable->m_value;//����Yʵֵ
		//m_pChartdata.append(QPointF(x++, Y));//�����ͼ�洢��
	}



	
	

}

void CTaos::STableDirectQueryData()
{
	CTaosMeasTag::resetMeasTAGPool();//��������
	m_listMeasTagNode.clear();//�����ӱ������������
	m_MeasTagMapSTNtoList[CGlobal::m_treeName].clear();//map�ӱ��������

	TAOS_RES* res = NULL;								//��ѯ�Ľ����
	TAOS_ROW data_row = NULL;							//һ����¼�Ľ������
	int records_num = 0;								//��ѯ����ļ�¼����
	QString str = "show table tags from rr6000.";
	str.append(CGlobal::m_treeName);

	res = taos_query(m_taos, str.toStdString().c_str());

	int fields_num = taos_field_count(res);				//��ѯ������������ͬtaos_num_fields
	TAOS_FIELD* taosfields_info = taos_fetch_fields(res);		//��ṹ��Ϣ

	QString subchar;
	char split = ' ';

	CTaosMeasTag* pTaosMeasTAG;//����ת�����������

	double pointnum = 0;
	while ((data_row = taos_fetch_row(res))) {				//���л�ȡ��ѯ������е����ݡ�
		char temp[1024] = { 0 };
		taos_print_row(temp, data_row, taosfields_info, fields_num);	//�ٷ�����һ����¼���
		records_num++;

		pTaosMeasTAG = CTaosMeasTag::getMeasTAGNodebyPool();

		subchar = strtok(temp, &split);//�ִ���ȡ

		pTaosMeasTAG->m_tbname = subchar;

		subchar = strtok(NULL, &split);
		pTaosMeasTAG->m_meas_name = subchar;

		subchar = strtok(NULL, &split);
		pTaosMeasTAG->m_meas_type = subchar.toInt();


		//�������޸ü�¼���ӱ�����map���о�append
		if (m_MeasTagMapSTNtoList.contains(CGlobal::m_treeName))
			m_MeasTagMapSTNtoList[CGlobal::m_treeName].append(pTaosMeasTAG);
		else
		{
			//���޾����������ٿ�ʼ���룬�ٰ���������ӱ�map
			m_listMeasTagNode.clear();
			m_listMeasTagNode.append(pTaosMeasTAG);
			m_MeasTagMapSTNtoList.insert(CGlobal::m_treeName, m_listMeasTagNode);
		}

	}
	
}

void CTaos::ReadSTTree()
{
	//���彨������Ŀ¼�Ľ�����Ϣ
	TAOS_RES* resST = NULL;								//��ѯ�Ľ����
	TAOS_ROW data_rowST = NULL;							//һ����¼�Ľ������
	int fields_numST = 0;								//��ѯ������������ͬtaos_num_fields
	TAOS_FIELD* taosfields_infoST = NULL;						//��ṹ��Ϣ

	//���峬�����ӱ�Ŀ¼�Ľ�����Ϣ
	TAOS_RES* resT = NULL;								//��ѯ�Ľ����
	TAOS_ROW data_rowT = NULL;							//һ����¼�Ľ������
	int fields_numT = 0;								//��ѯ������������ͬtaos_num_fields
	TAOS_FIELD* taosfields_infoT = NULL;						//��ṹ��Ϣ
	

	//��װ�鳬���sql���
	QString strST = "show rr6000.stables";
	resST = taos_query(m_taos, strST.toStdString().c_str());
	fields_numST = taos_field_count(resST);				//��ѯ������������ͬtaos_num_fields
	taosfields_infoST = taos_fetch_fields(resST);		//��ṹ��Ϣ

	while ((data_rowST = taos_fetch_row(resST))) {				//���л�ȡ��ѯ������е����ݡ�
		char tempST[64] = { 0 };
		taos_print_row(tempST, data_rowST, taosfields_infoST, fields_numST);	//�ٷ�����һ����¼���
		//������������һ��Ŀ¼
		CItemTree* pItemTree = CTreeModel::getItemTreebyPool();
		pItemTree->m_TreeName = tempST;
		pItemTree->m_TreeType = 1;//���Ϊ����������
		//pItemTree->m_TreeId = 1;
		CTreeModel::m_listSTree.append(pItemTree);

		//ͨ���յ��ĳ�����������ȡ�ӱ�Ŀ¼
		QString strT = "show table tags tbname from rr6000.";
		strT.append(tempST);
		resT = taos_query(m_taos, strT.toStdString().c_str());
		fields_numT = taos_field_count(resT);			//��ѯ������������ͬtaos_num_fields
		taosfields_infoT = taos_fetch_fields(resT);		//��ṹ��Ϣ
		while ((data_rowT = taos_fetch_row(resT))) {
			char tempT[64] = { 0 };
			taos_print_row(tempT, data_rowT, taosfields_infoT, fields_numT);	//�ٷ�����һ����¼���
			//���ӱ�����������Ŀ¼
			CItemTree* pItemTree = CTreeModel::getItemTreebyPool();
			pItemTree->m_TreeName = tempT;
			pItemTree->m_TreeType = 2;//���Ϊ�ӱ�����
			//pItemTree->m_TreeId = 1;

			if (CTreeModel::m_mapQStoQLTree.contains(QString(tempST)))
				CTreeModel::m_mapQStoQLTree[QString(tempST)].append(pItemTree);
			else
			{
				CTreeModel::m_listTree.clear();
				CTreeModel::m_listTree.append(pItemTree);
				CTreeModel::m_mapQStoQLTree.insert(QString(tempST), CTreeModel::m_listTree);
			}
		}

	}

}

void CTaos::ReadTTree()
{
	//CTreeModel::m_mapTree.clear();

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
		//CTreeModel::m_mapTree.append(pItemTree);
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
