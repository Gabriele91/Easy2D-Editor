#ifndef CREATESCENEDIALOG_H
#define CREATESCENEDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QErrorMessage>
#include <QFileDialog>
#include <TreeScene.h>
#include <ui_CreateSceneDialog.h>

class CreateSceneDialog : public QDialog
{
	Q_OBJECT

public:

	CreateSceneDialog(QWidget *parent = 0)
		:QDialog(parent)
	{
		ui.setupUi(this);
		connect(ui.buttonAdd, &QPushButton::clicked, 
		[=]() {
			
			if(!QFile::exists(ui.linePath->text()))
			{
				QMessageBox messageBox(this);
				messageBox.setFixedSize(200,200);
				messageBox.critical(0,"Error","invalid path");
				messageBox.open();
			}
			else if(!ui.lineName->text().size())
			{
				QMessageBox messageBox(this);
				messageBox.setFixedSize(200,200);
				messageBox.critical(0,"Error","invalid name");
				messageBox.open();
			}
			else 			
			{
				this->accept();
			}

		});
		connect(ui.buttonDir, &QPushButton::clicked, 
		[=]() {
            QString path=QFileDialog::getExistingDirectory(this,
                                                           this->title,
                                                           QString(),
                                                           QFileDialog::DontUseNativeDialog);
			ui.linePath->setText(path);
		});
	}
	~CreateSceneDialog()
	{
	}

	int exec(const QString& title)
	{
		this->title=title;
		setWindowTitle(title);
		ui.lineName->setText("");
		ui.linePath->setText("");
		return QDialog::exec();
	}

	QString getLastName()
	{
		return ui.lineName->text();
	}
	QString getLastPath()
	{
		return ui.linePath->text();
	}

private:

	QString title;
	Ui::CreateSceneDialog ui;
};


#endif
