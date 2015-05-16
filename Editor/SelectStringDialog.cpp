#include "SelectStringDialog.h"
#include "ui_SelectStringDialog.h"

SelectStringDialog::SelectStringDialog(const QStringList &list, const QString &defaultrs, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectStringDialog)
{
    //create ui
    ui->setupUi(this);
    //add elements
    ui->listString->addItems(list);
    //set defaultdefaultrs
    for (int i = 0; i < ui->listString->count(); ++i)
    {
        auto item=ui->listString->item(i);
        QString itemData = item->text();
        if(itemData==defaultrs)
        {
            ui->listString->setCurrentItem(item);
            break;
        }
    }
    //connects
    connect(ui->pbCancel,&QPushButton::clicked,this,
    [this](bool){
        this->lastName="";
        this->reject();
    });
    connect(ui->listString,&QListWidget::itemDoubleClicked,this,
    [this](QListWidgetItem*){
        this->lastName=ui->listString->currentItem()->text();
        this->accept();
    });
}

SelectStringDialog::~SelectStringDialog()
{
    delete ui;
}
