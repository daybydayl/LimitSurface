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
	//m_taos = taos_connect("Linx.Linx-lzj", "root", "taosdata", NULL, 6030);//凝思句柄
	m_taos = taos_connect("lzj-VM", "root", "taosdata", NULL, 6030);          //unbuntu句柄
	if (m_taos == NULL) {
		printf("failed to connect to server\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("connect success！！\n");
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
    
	

    /*操作数据库*/
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
	CTaosMeasTable* pTaosMeasTable;//接收转换后的类数据
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


	//Customplot，给数据线填数据
	//CCustomPlot::m_Xtime.push_back(pointnum++);
	int Xnum = pTaosMeasTable->m_ts;
	CCustomPlot::m_Xtime.push_back(Xnum);
	CCustomPlot::m_Yvalue.push_back(pTaosMeasTable->m_value);

	return offset;
}

int CTaos::Syn_QueryStructDataBySQL(TAOS* taos, const char* sql, TAOS_ROW &data_rows, TAOS_FIELD* &taosfields_info, int &fields_num, int &records_num)
{
	int nRet = -1;//0为正常

	//通过sql语句获取结果集
	TAOS_RES* res = taos_query(taos, sql);
	nRet = taos_errno(res);
	if (nRet != 0)
	{
		printf("Error code: %d; Message: %s\n", nRet, taos_errstr(res));
		taos_free_result(res);
		return nRet;
	}
	
	//通过结果集获取域信息
	taosfields_info = taos_fetch_fields(res);

	//通过结果集获取域数
	fields_num = taos_field_count(res);

	//通过结果集获取记录数和记录数据集
	records_num = taos_fetch_block(res, &data_rows);//执行到这里有问题！！！！！！！！！！！！！！！！！！！！！！！

	return nRet;
}

void CTaos::TableDirectQueryData()
{
	//CTaosMeasTable::resetMeasPool();//清楚对象池
	m_listMeasTableNode.clear();//单个子表链表数据清空
	m_MeasMapTNtoList[CGlobal::m_treeName].clear();//map子表集链表清空
	CCustomPlot::m_Xtime.clear();
	CCustomPlot::m_Yvalue.clear();

	TAOS_RES* res = NULL;								//查询的结果集
	TAOS_ROW data_row = NULL;							//一条记录的结果数据
	TAOS_ROW* data_rows = new TAOS_ROW;				//数据块
	int records_num = 0;								//查询结果的记录条数
	QString str = "select * from rr6000.";
	str.append(CGlobal::m_treeName);
	//str.append(" limit 10000");

	res = taos_query(m_taos, str.toStdString().c_str());

	int fields_num = taos_field_count(res);				//查询表的域个数，等同taos_num_fields
	TAOS_FIELD* taosfields_info = taos_fetch_fields(res);		//域结构信息

	//返回结果集时间戳字段的精度，0 代表毫秒，1 代表微秒，2 代表纳秒。
	int time_pre = taos_result_precision(res);
	double pointnum = 0;
	LONG64 xAis = 0;

	CTaosMeasTable* pTaosMeasTable;//接收转换后的类数据

	//字串截取
	QString subchar;
	char split = ' ';
	QString Smilli;
	//将毫秒转成时间
	char curDate[64];
	time_t curtime;
	struct tm* tminfo;//C++自带
	while ((data_row = taos_fetch_row(res))) {				//按行获取查询结果集中的数据。
		char temp[1024] = { 0 };
		taos_print_row(temp, data_row, taosfields_info, fields_num);	//官方解析一条记录输出
		records_num++;
		pTaosMeasTable = CTaosMeasTable::getMeasNodebyPool();
		subchar = strtok(temp, &split);//字串截取
		xAis = subchar.toLongLong();
		Smilli = QString::number(subchar.toLongLong() % 1000);//取毫秒字符串
		curtime = subchar.toLongLong() / 1000;
		tminfo = localtime(&curtime);//将的得到时间转成结构体tm类型
		strftime(curDate, 64, "%Y-%m-%d %H:%M:%S", tminfo);//把tm结构体已模型放入字符串中
		pTaosMeasTable->m_date.append(curDate);//日期字串
		pTaosMeasTable->m_date.append(".");
		pTaosMeasTable->m_date.append(Smilli);//毫秒字串
		subchar = strtok(NULL, &split);
		pTaosMeasTable->m_value = subchar.toDouble();//量测值
		if (m_MeasMapTNtoList.contains(CGlobal::m_treeName))
			m_MeasMapTNtoList[CGlobal::m_treeName].append(pTaosMeasTable);
		else
		{
			m_listMeasTableNode.append(pTaosMeasTable);
			m_MeasMapTNtoList.insert(CGlobal::m_treeName, m_listMeasTableNode);
		}
		//Customplot，给数据线填数据
		//CCustomPlot::m_Xtime.push_back(pointnum++);
		CCustomPlot::m_Xtime.push_back(xAis/1000);
		CCustomPlot::m_Yvalue.push_back(pTaosMeasTable->m_value);
	}

	/*int offset = 0;
	int ret = 0;
	ret = taos_fetch_block(res, data_rows);//block不会解析
	for (int i = 0; i < ret; i++)
	{
		taos_analysis_row(data_rows[i], taosfields_info, fields_num);
	}*/

	
	

}

void CTaos::STableDirectQueryData()
{
	CTaosMeasTable::resetMeasPool();//清楚对象池
	m_listMeasTableNode.clear();//单个子表链表数据清空
	m_MeasMapTNtoList[CGlobal::m_treeName].clear();//map子表集链表清空
	CCustomPlot::m_Xtime.clear();
	CCustomPlot::m_Yvalue.clear();

	TAOS_RES* res = NULL;								//查询的结果集
	TAOS_ROW data_row = NULL;							//一条记录的结果数据
	int records_num = 0;								//查询结果的记录条数
	QString str = "select * from rr6000.";
	str.append(CGlobal::m_treeName);
	str.append(" limit 10000");

	res = taos_query(m_taos, str.toStdString().c_str());

	int fields_num = taos_field_count(res);				//查询表的域个数，等同taos_num_fields
	TAOS_FIELD* taosfields_info = taos_fetch_fields(res);		//域结构信息

	//返回结果集时间戳字段的精度，0 代表毫秒，1 代表微秒，2 代表纳秒。
	int time_pre = taos_result_precision(res);

	//字串截取
	QString subchar;
	char split = ' ';
	
	//将毫秒转成时间
	char curDate[64];
	time_t curtime;
	struct tm* tminfo;//C++自带
	QString Smilli;


	CTaosMeasTable* pTaosMeasTable;//接收转换后的类数据

	double pointnum = 0;
	while ((data_row = taos_fetch_row(res))) {				//按行获取查询结果集中的数据。
		char temp[1024] = { 0 };
		taos_print_row(temp, data_row, taosfields_info, fields_num);	//官方解析一条记录输出
		records_num++;

		pTaosMeasTable = CTaosMeasTable::getMeasNodebyPool();

		subchar = strtok(temp, &split);//字串截取
		Smilli = QString::number(subchar.toLongLong() % 1000);//取毫秒字符串
		curtime = subchar.toLongLong() / 1000;
		tminfo = localtime(&curtime);//将的得到时间转成结构体tm类型
		strftime(curDate, 64, "%Y-%m-%d %H:%M:%S", tminfo);//把tm结构体已模型放入字符串中
		pTaosMeasTable->m_date.append(curDate);//日期字串
		pTaosMeasTable->m_date.append(".");
		pTaosMeasTable->m_date.append(Smilli);//毫秒字串

		subchar = strtok(NULL, &split);
		pTaosMeasTable->m_value = subchar.toDouble();//量测值

		subchar = strtok(NULL, &split);
		pTaosMeasTable->m_Smeas_name_TAG = subchar;//tag

		subchar = strtok(NULL, &split);
		pTaosMeasTable->m_Smeas_type_TAG = subchar.toInt();


		//看该有无该记录的子表链表map，有就append
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
			//若无就先清链表再开始添入，再把链表放入子表map
			m_listMeasTableNode.clear();
			m_listMeasTableNode.append(pTaosMeasTable);
			m_MeasMapTNtoList.insert(pTaosMeasTable->m_Smeas_name_TAG, m_listMeasTableNode);
		}

		//如果超级表map里没有该st的子集就插入
		if (!m_MeasMapSTNtoList.contains(CGlobal::m_treeName))
			m_MeasMapSTNtoList.insert(CGlobal::m_treeName, m_MeasMapTNtoList);

		//Customplot，给数据线填数据
		//CCustomPlot::m_Xtime.push_back(pointnum++);
		//CCustomPlot::m_Xtime.push_back(pTaosMeasTable->m_date.toDouble());
		CCustomPlot::m_Yvalue.push_back(pTaosMeasTable->m_value);

	}
	
}

void CTaos::test()
{
	CTaosMeasTable::resetMeasPool();//清楚对象池
	m_listMeasTableNode.clear();
	m_MeasMapTNtoList[CGlobal::m_treeName].clear();
	CCustomPlot::m_Xtime.clear();
	CCustomPlot::m_Yvalue.clear();

	TAOS_RES* res = NULL;								//查询的结果集
	TAOS_ROW data_row = NULL;								//一条记录的结果数据
	int records_num = 0;										//查询结果的记录条数
	QString str = "select * from rr6000.";
	str.append(CGlobal::m_treeName);
	str.append(" limit 10000");

	res = taos_query(m_taos, str.toStdString().c_str());

	int fields_num = taos_field_count(res);				//查询表的域个数，等同taos_num_fields
	TAOS_FIELD* taosfields_info = taos_fetch_fields(res);		//域结构信息

	//返回结果集时间戳字段的精度，0 代表毫秒，1 代表微秒，2 代表纳秒。
	int time_pre = taos_result_precision(res);

	//字串截取
	/*char* subchar;
	char split = ' ';
	QString mid;*/

	//将毫秒转成时间
	char curDate[64];
	time_t curtime;
	struct tm* tminfo;//C++自带

	QString last3B_ts;//接收后三位毫秒时间
	QString ts;//临时接收一条记录的时间戳
	QString value;//临时接收一条记录的量测值
	QString measname_tag;
	QString meastype_tag;
	CTaosMeasTable* pTaosMeasTable;//接收转换后的类数据

	double pointnum = 0;
	while ((data_row = taos_fetch_row(res))) {				//按行获取查询结果集中的数据。
		char temp[1024] = { 0 };
		taos_print_row(temp, data_row, taosfields_info, fields_num);	//官方解析一条记录输出
		records_num++;

		pTaosMeasTable = CTaosMeasTable::getMeasNodebyPool();

		last3B_ts.clear();
		ts.clear();
		value.clear();//清空临时接收的变量
		measname_tag.clear();
		meastype_tag.clear();

	
		/*subchar = strtok(temp, &split);//字串截取
		pTaosMeasTable->m_date = subchar;
		mid = strtok(NULL, &split);
		pTaosMeasTable->m_value = mid.toDouble();*/
	

		int flag = 1;//用来判断取出哪部分数据
		for (int i = 0; temp[i] != 0; i++)
		{
			if (1 == flag)//取时间戳
			{
				if (temp[i] != ' ')
					ts.append(temp[i]);
				else
				{
					flag = 2;//检测到空格，该取下一个数据
					last3B_ts.append(temp[i - 3]);//取后三位的毫秒
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
		//将毫秒转成时间
		char curDate[64];
		time_t curtime;
		struct tm* tminfo;//C++自带
		curtime = pTaosMeasTable->m_ts/1000;
		tminfo = localtime(&curtime);//将的得到时间转成结构体tm类型
		strftime(curDate, 64, "%Y-%m-%d %H:%M:%S", tminfo);//把tm结构体已模型放入字符串中
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
	
		//Customplot，给数据线填数据
		CCustomPlot::m_Xtime.push_back(pointnum++);
		CCustomPlot::m_Yvalue.push_back(pTaosMeasTable->m_value);
	
	
	}

			/*按表名存储，数据重复存放，待优化*/
}

void CTaos::ReadSTTree()
{
	TAOS_RES* res = NULL;								//查询的结果集
	TAOS_ROW data_row = NULL;							//一条记录的结果数据
	int records_num = 0;								//查询结果的记录条数
	QString str = "show rr6000.stables";

	res = taos_query(m_taos, str.toStdString().c_str());

	int fields_num = taos_field_count(res);				//查询表的域个数，等同taos_num_fields
	TAOS_FIELD* taosfields_info = taos_fetch_fields(res);		//域结构信息


	while ((data_row = taos_fetch_row(res))) {				//按行获取查询结果集中的数据。
		char temp[1024] = { 0 };
		taos_print_row(temp, data_row, taosfields_info, fields_num);	//官方解析一条记录输出
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

	TAOS_RES* res = NULL;								//查询的结果集
	TAOS_ROW data_row = NULL;							//一条记录的结果数据
	int records_num = 0;								//查询结果的记录条数
	QString str = "show table tags tbname from rr6000.";
	str.append(CGlobal::m_treeName);

	res = taos_query(m_taos, str.toStdString().c_str());

	int fields_num = taos_field_count(res);				//查询表的域个数，等同taos_num_fields
	TAOS_FIELD* taosfields_info = taos_fetch_fields(res);		//域结构信息


	while ((data_row = taos_fetch_row(res))) {				//按行获取查询结果集中的数据。
		char temp[1024] = { 0 };
		taos_print_row(temp, data_row, taosfields_info, fields_num);	//官方解析一条记录输出
		records_num++;

		CItemTree* pItemTree = CTreeModel::getItemTreebyPool();

		pItemTree->m_TreeName = temp;
		pItemTree->m_TreeType = 2;
		CTreeModel::m_listTree.append(pItemTree);
	}

}


void CTaos::Get_TaosInfo(TAOS* taos)
{
	printf("客户端版本为%s\n", taos_get_client_info());
	//printf("服务端版本为：%s\n", taos_get_server_info(taos));
}

void CTaos::CommonExeDB()
{
	Get_TaosInfo(m_taos);
	Syn_executeSQL(m_taos, "use rr6000");

}

void CTaos::SynReadTable()
{
	int nRet = -1;
	
	//TAOS_ROW* data_rows = (TAOS_ROW*)malloc(6);		//记录的数据,需先给点空间
	TAOS_ROW data_rows = NULL;
	TAOS_FIELD* taosfields_info;					//域信息数组
	int fields_num;									//域数
	int records_num;								//行数

	nRet = Syn_QueryStructDataBySQL(
		m_taos,//数据库句柄
		"select * from rr6000.meas001",//sql语句，const char*型
		data_rows,//数据buf，这里的buf不知道怎么解析
		taosfields_info,//域信息
		fields_num,//域数量
		records_num);//记录数量


	double tstemp = 0;		//临时接收时间戳
	double nmeas_value = 0;	//临时接收量测值

	int recordIdx = 0;
	int fieldIdx = 0;
	
	int tsOffset = 0;//这里先手动给
	int measvalueOffset = 0 + taosfields_info[1].bytes;//这里先手动给
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
	//	taos_print_row(ts, data_rows[recordIdx], taosfields_info, fields_num);	//官方解析一条记录输出
	//}


}
