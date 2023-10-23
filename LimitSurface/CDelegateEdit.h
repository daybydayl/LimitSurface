#pragma once
#ifndef CDELEGATEEDIT_H
#define CDELEGATEEDIT_H

#include "CCommonHead.h"
#include <QtWidgets/QItemDelegate>
#include <QtWidgets/QLineEdit>
class CDelegateEdit : public QItemDelegate
{
public:
	CDelegateEdit(QObject* parent = nullptr);
	~CDelegateEdit();

public:
    // editing
    //����һ���ɱ༭�Ŀ�
    QWidget* createEditor(QWidget* parent,const QStyleOptionViewItem& option,const QModelIndex& index) const;
    //��model�������ȡ������༭����
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    //�ֶ�д�༭�����model���У��ڻ�д����Ҫ�������ڴ�
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    //���һ����ʾ���𣬲���Ҳ��
    void updateEditorGeometry(QWidget* editor,const QStyleOptionViewItem& option,const QModelIndex& index) const;

};
#endif // !CDELEGATE_H
