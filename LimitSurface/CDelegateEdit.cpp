#include "CDelegateEdit.h"

CDelegateEdit::CDelegateEdit(QObject* parent)
{
}

CDelegateEdit::~CDelegateEdit()
{
}

QWidget* CDelegateEdit::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	//编辑条的类对象
	QLineEdit* pLineEdit = NULL;

	pLineEdit = new QLineEdit(parent);

	return pLineEdit;
}

void CDelegateEdit::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	QString Strtmp;
	QLineEdit* pLineEdit = NULL;

	//先把model项的数据放入临时变量中
	Strtmp = index.model()->data(index, Qt::DisplayRole).toString();

	//把edit模型接到定义的临时编辑条中，然后if里把数据添入
	pLineEdit = static_cast<QLineEdit*>(editor);

	if (pLineEdit)
	{
		pLineEdit->setText(Strtmp);
		return;
	}
	QItemDelegate::setEditorData(editor, index);
}

void CDelegateEdit::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QLineEdit* pLineEdit = NULL;
	QString Strtmp;

	//把已经修改数据的model项接到定义的临时编辑条
	pLineEdit = static_cast<QLineEdit*>(editor);
	if (0 == pLineEdit)
		return;

	//编辑条获取数据后，把数据写到model中的写回内存函数
	Strtmp = pLineEdit->text();
	/*model.setdata的写回函数*/
}

void CDelegateEdit::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	//起个单元格浮起的效果
	editor->setGeometry(option.rect);
}
