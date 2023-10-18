#pragma once
#include "CCommonHead.h"
#include "CTaosMeasTable.h"
#include "CGlobal.h"

#include "CCustomPlot.h"

class CTaos
{
public:
	CTaos();
	~CTaos();

public:
	static CTaos* Intoinstance();					//����ʵ��
	static void Exitinstance();						//�˳�ʵ��
	void Init();									//��ʼ��

public:
	int Syn_executeSQL(TAOS* taos, const char* sql);//ִ�в���������
	//�Է�װ��ͨ��һ�� ��ѯsql��䣬��ȡ��Ӧ�ṹ����
	// 
	int taos_analysis_row(TAOS_ROW row, TAOS_FIELD* fields, int num_fields);
	//[in]���ݿ�����[in]sql��䣬[out]��¼���ݣ�[out]����Ϣ���飬[out]������[out]����������״ֵ̬0Ϊ����
	int Syn_QueryStructDataBySQL(TAOS* taos, const char* sql, TAOS_ROW &data_rows, TAOS_FIELD* &taosfields_info, int &fields_num, int &records_num);	//ִ��sql���

	void TableDirectQueryData();//ֱ�Ӷ��ӱ�

	void STableDirectQueryData();//���������ֱ�Ӱ������ӱ��ó���ã�֮���ӱ���Ҫ��

	void test();

	void ReadSTTree();

	void ReadTTree();


public:
	void Get_TaosInfo(TAOS* taos);					//��ȡtaos�����Ϣ
	void CommonExeDB();								//�����⡢�����
	void SynReadTable();							//ͬ����ʽ����

public:
	static CTaos* m_pCTaos;							//����ָ��
	static TAOS* m_taos;							//������ȫ�֣�������ļ�ʹ��

	static QList<CTaosMeasTable*>					m_listMeasTableNode;//�ӱ���յ�����
	static QMap<QString, QList<CTaosMeasTable*>>	m_MeasMapTNtoList;//����taos����������,�ӱ���������
	static QMap<QString, QMap<QString, QList<CTaosMeasTable*>>> m_MeasMapSTNtoList;//���ճ����������ӱ���������
	
};
											
