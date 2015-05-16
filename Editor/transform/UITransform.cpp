#include "UITransform.h"
#include <PluginTransform.h>
#include "ui_UITransform.h"

UITransform::UITransform(ComponentTransform *ctransform) :
    QWidget(0),
    ui(new Ui::UITransform)
{
    ui->setupUi(this);
    //set z validetor
    ui->order_z->setValidator(new QIntValidator());
    //set pos validetor
    ui->pos_x->setValidator(new QDoubleValidator());
    ui->pos_y->setValidator(new QDoubleValidator());
    //set rotation validetor
    ui->rotation->setValidator(new QDoubleValidator());
    //set scale validetor
    ui->scale_x->setValidator(new QDoubleValidator());
    ui->scale_y->setValidator(new QDoubleValidator());



    //connects
    auto lbTransform=[=](const QString&){
        ctransform->doTransform();
    };
    connect(ui->order_z,&QLineEdit::textChanged,this,lbTransform);
    connect(ui->pos_x,&QLineEdit::textChanged,lbTransform);
    connect(ui->pos_y,&QLineEdit::textChanged,lbTransform);
    connect(ui->rotation,&QLineEdit::textChanged,lbTransform);
    connect(ui->scale_x,&QLineEdit::textChanged,lbTransform);
    connect(ui->scale_y,&QLineEdit::textChanged,lbTransform);

    auto cbTransform=[=](bool){
        ctransform->doTransform();
    };
    connect(ui->cbPosition,&QCheckBox::stateChanged,cbTransform);
    connect(ui->cbRotation,&QCheckBox::stateChanged,cbTransform);
    connect(ui->cbScale,&QCheckBox::stateChanged,cbTransform);
}

void UITransform::setPosition(const Easy2D::Vec2& pos)
{
    ui->pos_x->blockSignals(true);
    ui->pos_y->blockSignals(true);

    ui->pos_x->setText(QString::number(pos.x));
    ui->pos_y->setText(QString::number(pos.y));
    //left
    ui->pos_x->setCursorPosition( 0 );
    ui->pos_y->setCursorPosition( 0 );

    ui->pos_x->blockSignals(false);
    ui->pos_y->blockSignals(false);
}

void UITransform::setScale(const Easy2D::Vec2& scale)
{    
    ui->scale_x->blockSignals(true);
    ui->scale_y->blockSignals(true);

    ui->scale_x->setText(QString::number(scale.x));
    ui->scale_y->setText(QString::number(scale.y));
    //left
    ui->scale_x->setCursorPosition( 0 );
    ui->scale_y->setCursorPosition( 0 );

    ui->scale_x->blockSignals(false);
    ui->scale_y->blockSignals(false);
}
void UITransform::setRotation(Easy2D::Angle rot)
{
    ui->rotation->blockSignals(true);

    ui->rotation->setText(QString::number(rot.valueDegrees()));
    //left
    ui->rotation->setCursorPosition( 0 );

    ui->rotation->blockSignals(false);
}

void UITransform::setOrder(int val_z)
{
    ui->order_z->blockSignals(true);

    ui->order_z->setText(QString::number(val_z));
    //left
    ui->order_z->setCursorPosition( 0 );

    ui->order_z->blockSignals(false);
}


Easy2D::Vec2 UITransform::getPosition()
{
    return Easy2D::Vec2(
    ui->pos_x->text().toFloat(),
    ui->pos_y->text().toFloat()
    );
}
Easy2D::Vec2 UITransform::getScale()
{
    return Easy2D::Vec2(
    ui->scale_x->text().toFloat(),
    ui->scale_y->text().toFloat()
    );
}
Easy2D::Angle UITransform::getRotation()
{
    return Easy2D::Degree( ui->rotation->text().toFloat() );
}
int UITransform::getOrder()
{
    return ui->order_z->text().toInt();
}

int UITransform::getParentMode()
{
    int out=Easy2D::Object::DISABLE_PARENT;
    out|=Easy2D::Object::ENABLE_POSITION*ui->cbPosition->isChecked();
    out|=Easy2D::Object::ENABLE_ROTATION*ui->cbRotation->isChecked();
    out|=Easy2D::Object::ENABLE_SCALE*ui->cbScale->isChecked();
    return out;
}
void UITransform::setParentMode(int mode)
{
    ui->cbPosition->setChecked((mode&Easy2D::Object::ENABLE_POSITION) != 0);
    ui->cbRotation->setChecked((mode&Easy2D::Object::ENABLE_ROTATION) != 0);
    ui->cbScale->setChecked((mode&Easy2D::Object::ENABLE_SCALE) != 0);
}


UITransform::~UITransform()
{
    delete ui;
}
