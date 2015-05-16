#include "ScelectResourceDialog.h"
#include "ui_ScelectResourceDialog.h"

ScelectResourceDialog::ScelectResourceDialog(Easy2D::Table *table,
                                             const QString &defaultrs,
                                             QWidget *parent)
                                            :ScelectResourceDialog(QStringList(),
                                                                   table,
                                                                   defaultrs,
                                                                   parent)
{
}

ScelectResourceDialog::ScelectResourceDialog(const QStringList &filter,
                                             Easy2D::Table *table,
                                             const QString &defaultrs,
                                             QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScelectResourceDialog)
{
    //init ui
    ui->setupUi(this);
    //init manager
    resources=new ManagerResourcesList(ui->listResources,table);
    //update
    resources->update(filter);
    //set defaultdefaultrs
    for (int i = 0; i < ui->listResources->count(); ++i)
    {
        auto item=ui->listResources->item(i);
        QString itemData = item->text();
        if(itemData==defaultrs)
        {
            ui->listResources->setCurrentItem(item);
            break;
        }
    }
    //connect
    connect(ui->pbCancel,&QPushButton::clicked,this,
    [this](bool){
        this->lastName="";
        this->reject();
    });
    connect(ui->listResources,&QListWidget::itemDoubleClicked,this,
    [this](QListWidgetItem*){
        this->lastName=ui->listResources->currentItem()->text();
        this->accept();
    });
}

ScelectResourceDialog::~ScelectResourceDialog()
{
    delete resources;
    delete ui;
}
