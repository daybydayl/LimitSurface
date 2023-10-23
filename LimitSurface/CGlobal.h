#pragma once
#ifndef CGLOBAL_H
#define CGLOABL_H

#include "CCommonHead.h"
#include "CTaos.h"

class CGlobal
{
public:
	CGlobal();
	~CGlobal();

public:
	static CGlobal* IntoInstance();					//����ʵ��
	static void ExitInstance();						//�˳�ʵ��
	void Init();									//��ʼ��

public:

	//����վ����Ҫfac_id������ƥ��,ͨ�����������Ŀ¼
	void readFacTable();
	
	//�����״�����ģ��
	void ReadDataModel();

	//���������ݺ���
	void UpdateDataTable();

	//�������,�����������ȡ������ͻ�ȡ��Ӧ�������


public:
	static CGlobal* m_pGlobal;						//����ָ��
	static int		m_FacId;						//ʶ���ĸ���վ
	static int		m_TableType;					//ʶ�������ֱ�
	static int		m_TorS;							//ʶ����ͨ���ǳ�����

	static QString	m_treeName;						//Ŀ¼��(����)���������Ӧ��¼���ݣ�ͨ��QMap�����key
	static QString	m_TreeType;						//Ŀ¼���ͣ��������ֶ����ֱ�
};

#endif // !CGLOBAL_H

