#ifndef ADDRESOURCEDIALOG_H
#define ADDRESOURCEDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QErrorMessage>
#include <QFileDialog>
#include <TreeScene.h>
#include <ui_AddResourceDialog.h>

class AddResourceDialog : public QDialog
{
	Q_OBJECT

public:

	AddResourceDialog(QWidget *parent = 0)
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
            else if(!ui.lineName->text().size()||
                     rsTable->exists(ui.lineName->text().toUtf8().data()))
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
            QString path=QFileDialog::getOpenFileName(this, this->title,
                                                      "",
                                                      this->filestype,
                                                      0,
                                                      QFileDialog::DontUseNativeDialog);
			ui.linePath->setText(path);
		});
	}
	~AddResourceDialog()
	{
	}

    int exec(const QString& title,
             const QString& filestype,
             Easy2D::Table* rsTable)
	{
		this->filestype=filestype;
		this->title=title;
        this->rsTable=rsTable;
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

    Easy2D::Table* rsTable;
	QString title;
	QString filestype;
	Ui::AddResourceDialog ui;
};


#endif
