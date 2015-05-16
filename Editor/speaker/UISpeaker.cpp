#include "UISpeaker.h"
#include <PluginSpeaker.h>
#include "ui_UISpeaker.h"

UISpeaker::UISpeaker(ComponentSpeaker *cspeaker) :
    QWidget(0),
    ui(new Ui::UISpeaker)
{
    ui->setupUi(this);
    //set radius validetor
    ui->radius->setValidator(new QDoubleValidator());
    //set volume validetor
    ui->volume->setValidator(new QDoubleValidator());
    //connects
    auto lbSpeaker=[=](const QString&){
        cspeaker->doSpeaker();
    };
    connect(ui->radius,&QLineEdit::textChanged,this,lbSpeaker);
    connect(ui->volume,&QLineEdit::textChanged,lbSpeaker);

    auto cbSpeaker=[=](bool){
        cspeaker->doSpeaker();
    };
    connect(ui->cb2D,&QCheckBox::stateChanged,cbSpeaker);
    connect(ui->cbPlay,&QCheckBox::stateChanged,cbSpeaker);
    connect(ui->cbLoop,&QCheckBox::stateChanged,cbSpeaker);

    //attach a animation
    connect(ui->selectsound,&QPushButton::clicked,this,
    [=](bool)
    {
        cspeaker->selectSound();
    });
}

void UISpeaker::setSound(const QString& sound)
{
    if(sound=="") ui->selectsound->setText("...");
    else  ui->selectsound->setText(sound);
}
QString UISpeaker::getSound()
{
    return ui->selectsound->text();
}
void UISpeaker::setRadius(float rad)
{
    ui->radius->blockSignals(true);

    ui->radius->setText(QString::number(rad));
    ui->radius->setCursorPosition( 0 );

    ui->radius->blockSignals(false);
}
void UISpeaker::setVolume(float v)
{
    ui->volume->blockSignals(true);

    ui->volume->setText(QString::number(v));
    ui->volume->setCursorPosition( 0 );

    ui->volume->blockSignals(false);
}
void UISpeaker::set2D(bool enable)
{
    ui->cb2D->blockSignals(true);
    ui->cb2D->setChecked(enable);
    ui->cb2D->blockSignals(false);
}
void UISpeaker::setPlayOnAttach(bool enable)
{
    ui->cbPlay->blockSignals(true);
    ui->cbPlay->setChecked(enable);
    ui->cbPlay->blockSignals(false);
}
void UISpeaker::setLoop(bool enable)
{
    ui->cbLoop->blockSignals(true);
    ui->cbLoop->setChecked(enable);
    ui->cbLoop->blockSignals(false);
}


float UISpeaker::getRadius()
{
    return ui->radius->text().toFloat();
}
float UISpeaker::getVolume()
{
    return ui->volume->text().toFloat();
}
bool UISpeaker::get2D()
{
    return ui->cb2D->isChecked();
}
bool UISpeaker::getPlayOnAttach()
{
    return ui->cbPlay->isChecked();
}
bool UISpeaker::getLoop()
{
    return ui->cbLoop->isChecked();
}

UISpeaker::~UISpeaker()
{
    delete ui;
}
