#include "CDelegateEdit.h"

CDelegateEdit::CDelegateEdit(QObject* parent)
{
}

CDelegateEdit::~CDelegateEdit()
{
}

QWidget* CDelegateEdit::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	//�༭���������
	QLineEdit* pLineEdit = NULL;

	pLineEdit = new QLineEdit(parent);

	return pLineEdit;
}

void CDelegateEdit::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	QString Strtmp;
	QLineEdit* pLineEdit = NULL;

	//�Ȱ�model������ݷ�����ʱ������
	Strtmp = index.model()->data(index, Qt::DisplayRole).toString();

	//��editģ�ͽӵ��������ʱ�༭���У�Ȼ��if�����������
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

	//���Ѿ��޸����ݵ�model��ӵ��������ʱ�༭��
	pLineEdit = static_cast<QLineEdit*>(editor);
	if (0 == pLineEdit)
		return;

	//�༭����ȡ���ݺ󣬰�����д��model�е�д���ڴ溯��
	Strtmp = pLineEdit->text();
	/*model.setdata��д�غ���*/
}

void CDelegateEdit::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	//�����Ԫ�����Ч��
	editor->setGeometry(option.rect);
}
