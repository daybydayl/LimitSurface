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
    //创建一个可编辑的框
    QWidget* createEditor(QWidget* parent,const QStyleOptionViewItem& option,const QModelIndex& index) const;
    //从model数据项获取数据填到编辑条中
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    //手动写编辑框会回填到model项中，在回写到想要的数据内存
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;
    //框的一个显示浮起，不加也行
    void updateEditorGeometry(QWidget* editor,const QStyleOptionViewItem& option,const QModelIndex& index) const;

};
#endif // !CDELEGATE_H
