#ifndef FIRSTDIALOG_H
#define FIRSTDIALOG_H


#include <QDialog>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QErrorMessage>
#include <CreateSceneDialog.h>
#include <ui_FirstDialog.h>
#include <ProjectUtility.h>

class FirstDialog : public QDialog
{
	Q_OBJECT

public:
	FirstDialog(QWidget *parent = 0)
		:QDialog(parent)
	{
		ui.setupUi(this);
		//attach buttons events
		connect(ui.openProject, &QPushButton::clicked, 
        [=]() {
            QString file=QFileDialog::getOpenFileName(this,
                                                      "Open File...",
                                                      QString(),
                                                      "Easy2D (*.rs.e2d)",
                                                      0,
                                                      QFileDialog::DontUseNativeDialog);
			if(QFile::exists(file))
			{
				//save
				projectPath=file;
				//done
				done(QDialog::Accepted);
			}
			else
			{
				QMessageBox messageBox(this);
				messageBox.setFixedSize(200,200);
				messageBox.critical(0,"Error","invalid file");
				messageBox.open();
			}
		});
		connect(ui.newProject, &QPushButton::clicked, 
		[=]() {
			//none
			CreateSceneDialog dialog;
			dialog.exec("New Scene");
			QDir path(dialog.getLastPath());

			if(!path.exists())
			{
				QMessageBox messageBox(this);
				messageBox.setFixedSize(200,200);
				messageBox.critical(0,"Error","The path doesn't exist");
				messageBox.open();
			}
			else 
			{
				if(!path.mkdir(dialog.getLastName()))
				{
					QMessageBox messageBox(this);
					messageBox.setFixedSize(200,200);
					messageBox.critical(0,"Error","Invalid project name");
					messageBox.open();
				}
				else
                {
                    if(ProjectUtility::createProject(dialog.getLastPath(),
                                                     dialog.getLastName(),
                                                     projectPath))
					{
						//done
						done(QDialog::Accepted);
					}
					else
					{
                        //rejected
						done(QDialog::Rejected);
					}
				}
			}

		});
	}
	~FirstDialog()
	{
	}
	const QString& getProjectPath() const
	{
		return projectPath;
	}
	bool startUp() const
	{
		return ui.loadLastScene->isChecked();
	}

private:
	
	QString projectPath;
	Ui::FirstDialog ui;

};

#endif
