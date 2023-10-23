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
	static CGlobal* IntoInstance();					//进入实例
	static void ExitInstance();						//退出实例
	void Init();									//初始化

public:

	//读厂站表，需要fac_id和类型匹配,通过这个来生成目录
	void readFacTable();
	
	//主读首次数据模型
	void ReadDataModel();

	//主更新数据函数
	void UpdateDataTable();

	//量测表部分,从量测链表获取结点数和获取对应索引结点


public:
	static CGlobal* m_pGlobal;						//单例指针
	static int		m_FacId;						//识别哪个厂站
	static int		m_TableType;					//识别是哪种表
	static int		m_TorS;							//识别普通表还是超级表

	static QString	m_treeName;						//目录名(表名)，用来存对应记录数据，通过QMap这个做key
	static QString	m_TreeType;						//目录类型，用来区分读哪种表
};

#endif // !CGLOBAL_H

