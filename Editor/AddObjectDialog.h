#ifndef ADDOBJECTDIALOG_H
#define ADDOBJECTDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QErrorMessage>
#include <ui_AddObjectDialog.h>
#include <TreeScene.h>

class AddObjectDialog : public QDialog
{
	Q_OBJECT

public:
	AddObjectDialog(TreeScene* treeModelScene,QWidget *parent = 0)
		:QDialog(parent)
	{
		ui.setupUi(this);
		connect(ui.buttonAdd, &QPushButton::clicked, 
		[=]() {
			if(treeModelScene->pushObject(
				this->ui.editName->text().toUtf8().constData()
			  ))
			{
				this->close();
			}
			else
			{
				QMessageBox messageBox(this);
				messageBox.setFixedSize(200,200);
				messageBox.critical(0,"Error","invalid name");
				messageBox.open();
			}
		});
	}
	~AddObjectDialog()
	{
	}

private:

	Ui::AddObjectDialog ui;
};


#endif