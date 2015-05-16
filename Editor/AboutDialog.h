#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H
#include <QDialog>
#include <TreeScene.h>
#include <ui_AboutDialog.h>

class AboutDialog : public QDialog
{
	Q_OBJECT

public:
	AboutDialog(QWidget *parent = 0):QDialog(parent)
	{
		ui.setupUi(this);
	}
	~AboutDialog()
	{
	}

private:

	Ui::AboutDialog ui;
};

#endif
