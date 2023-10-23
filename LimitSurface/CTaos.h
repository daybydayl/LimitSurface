#pragma once
#ifndef CTAOS_H
#define CTAOS_H

#include "CTaosMeasTable.h"
#include "CTaosMeasTag.h"
#include "CGlobal.h"

class CTaos
{
public:
	CTaos();
	~CTaos();

public:
	static CTaos* Intoinstance();					//进入实例
	static void Exitinstance();						//退出实例
	void Init();									//初始化

public://自定义
	int Syn_executeSQL(TAOS* taos, const char* sql);//执行操作语句更好
	//自封装的通过一条 查询sql语句，获取对应结构数据
	// 
	int taos_analysis_row(TAOS_ROW row, TAOS_FIELD* fields, int num_fields);
	//[in]数据库句柄，[in]sql语句，[out]记录数据，[out]域信息数组，[out]域数，[out]行数，返回状态值0为正常
	int Syn_QueryStructDataBySQL(TAOS* taos, const char* sql, TAOS_ROW &data_rows, TAOS_FIELD* &taosfields_info, int &fields_num, int &records_num);	//执行sql语句

	void TableDirectQueryData();//直接读子表

	void STableDirectQueryData();//读超级表tag

	void ReadSTTree();

	void ReadTTree();


public:
	void Get_TaosInfo(TAOS* taos);					//获取taos相关信息
	void CommonExeDB();								//公共库、表操作
	void SynReadTable();							//同步方式读表

public:
	static CTaos* m_pCTaos;							//单例指针
	static TAOS* m_taos;							//声明成全局，方便跨文件使用

	//超级表数据
	static QList<CTaosMeasTag*>						m_listMeasTagNode;
	static QMap<QString, QList<CTaosMeasTag*>>		m_MeasTagMapSTNtoList;

	//子表数据
	static QList<CTaosMeasTable*>					m_listMeasTableNode;//子表接收的数据
	static QMap<QString, QList<CTaosMeasTable*>>	m_MeasMapTNtoList;//接收taos量测结点链表,子表名和数据
	static QMap<QString, QMap<QString, QList<CTaosMeasTable*>>> m_MeasMapSTNtoList;//接收超级表命，子表名和数据

	//QChart绘图变量
	static QVector<QPointF> m_pChartdata;	//QVector<QPointF>对象来存储数据(x,y)
	
};
											
#endif // ! CTAOS_H