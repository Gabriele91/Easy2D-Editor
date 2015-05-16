#ifndef RENAMEOBJECTDIALOG_H
#define RENAMEOBJECTDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QErrorMessage>
#include <ui_RenameObjectDialog.h>
#include <TreeScene.h>

class RenameObjectDialog : public QDialog
{
	Q_OBJECT

public:
    RenameObjectDialog(QWidget *parent = 0)
        :QDialog(parent)
        ,target(nullptr)
    {
        ui.setupUi(this);
        //connections
        connect(ui.buttonRename, &QPushButton::clicked,
        [this](bool)
        {
            if(!this->target) return;
            //get name
            Easy2D::String newName=
            this->ui.editName->text().toUtf8().constData();
            //create a canocical name
            newName.replaceAll(" ","_");
            //controll
            if(newName.size() && !this->target->getParent()->getChild(newName))
            {
                this->target->setName(newName);
                this->accept();
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

	~RenameObjectDialog()
	{
	}

    int exec(Easy2D::Object* obj)
	{
        //must have a name and a parent
        if(!obj || !obj->getParent() || obj->getName().size()==0)
            return QDialog::Rejected;
        //setup ui
        ui.editName->setText(QString(obj->getName().c_str()));
        //save target
        target=obj;
        //exec
        return QDialog::exec();
	}

private:

	Ui::RenameObjectDialog ui;
    Easy2D::Object* target;
};


#endif
