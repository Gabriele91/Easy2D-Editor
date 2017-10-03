#include <QPalette>
#include <QColorDialog>
#include "PluginSprite.h"
#include "UISprite.h"
#include "ui_UISprite.h"

UISprite::UISprite(ComponentSprite* csprite) :
    QWidget(nullptr),
    ui(new Ui::UISprite),
    csprite(csprite)
{
    //create ui
    ui->setupUi(this);
    //add cullface values
    ui->selectFaceCulling->addItem("DISABLE");
    ui->selectFaceCulling->addItem("BACK");
    ui->selectFaceCulling->addItem("FRONT");

    //setup combo box blend src
    ui->cbBlendSrc->addItem("ONE",Easy2D::BLEND::ONE);
    ui->cbBlendSrc->addItem("ZERO",Easy2D::BLEND::ZERO);

    ui->cbBlendSrc->addItem("ONE::MINUS::DST::COLOR",Easy2D::BLEND::ONE::MINUS::DST::COLOR);
    ui->cbBlendSrc->addItem("ONE::MINUS::DST::ALPHA",Easy2D::BLEND::ONE::MINUS::DST::ALPHA);

    ui->cbBlendSrc->addItem("ONE::MINUS::SRC::COLOR",Easy2D::BLEND::ONE::MINUS::SRC::COLOR);
    ui->cbBlendSrc->addItem("ONE::MINUS::SRC::ALPHA",Easy2D::BLEND::ONE::MINUS::SRC::ALPHA);

    ui->cbBlendSrc->addItem("DST::COLOR",Easy2D::BLEND::DST::COLOR);
    ui->cbBlendSrc->addItem("DST::ALPHA",Easy2D::BLEND::DST::ALPHA);

    ui->cbBlendSrc->addItem("SRC::COLOR",Easy2D::BLEND::SRC::COLOR);
    ui->cbBlendSrc->addItem("SRC::ALPHA",Easy2D::BLEND::SRC::ALPHA);
    ui->cbBlendSrc->addItem("SRC::ALPHA::SATURATE",Easy2D::BLEND::SRC::ALPHA::SATURATE);

    //setup combo box blend dst
    ui->cbBlendDst->addItem("ONE",Easy2D::BLEND::ONE);
    ui->cbBlendDst->addItem("ZERO",Easy2D::BLEND::ZERO);

    ui->cbBlendDst->addItem("ONE::MINUS::DST::COLOR",Easy2D::BLEND::ONE::MINUS::DST::COLOR);
    ui->cbBlendDst->addItem("ONE::MINUS::DST::ALPHA",Easy2D::BLEND::ONE::MINUS::DST::ALPHA);

    ui->cbBlendDst->addItem("ONE::MINUS::SRC::COLOR",Easy2D::BLEND::ONE::MINUS::SRC::COLOR);
    ui->cbBlendDst->addItem("ONE::MINUS::SRC::ALPHA",Easy2D::BLEND::ONE::MINUS::SRC::ALPHA);

    ui->cbBlendDst->addItem("DST::COLOR",Easy2D::BLEND::DST::COLOR);
    ui->cbBlendDst->addItem("DST::ALPHA",Easy2D::BLEND::DST::ALPHA);

    ui->cbBlendDst->addItem("SRC::COLOR",Easy2D::BLEND::SRC::COLOR);
    ui->cbBlendDst->addItem("SRC::ALPHA",Easy2D::BLEND::SRC::ALPHA);

    //connects
    connect(ui->colorR,SIGNAL(valueChanged(int)),SLOT(updateFrameColor()));
    connect(ui->colorG,SIGNAL(valueChanged(int)),SLOT(updateFrameColor()));
    connect(ui->colorB,SIGNAL(valueChanged(int)),SLOT(updateFrameColor()));
    connect(ui->colorA,SIGNAL(valueChanged(int)),SLOT(updateFrameColor()));

    connect(ui->selectTexture,&QPushButton::clicked,this,
    [this](bool)
    {
        this->setTexture(this->csprite->selectTexture());
        //applay change
        this->applayChange();
    });

    connect(ui->selectFaceCulling,
            SIGNAL(currentIndexChanged(int)),
            SLOT(applayChange()));


    auto cbApplay=
    [this](int)
    {
        this->applayChange();
    };
    auto signalcb=static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged);
    connect(ui->cbBlendSrc,signalcb,cbApplay);
    connect(ui->cbBlendDst,signalcb,cbApplay);

    connect(ui->selectColor,&QPushButton::clicked,this,
    [this](bool)
    {
        QColor acolor;
        acolor.setRgb(  ui->colorR->value(),
                        ui->colorG->value(),
                        ui->colorB->value(),
                        ui->colorA->value() );
        acolor=QColorDialog::getColor(acolor,
                                      0,
                                      "Sprite Color",
                                      QColorDialog::DontUseNativeDialog|
                                      QColorDialog::ShowAlphaChannel);
        ui->colorR->setValue(acolor.red());
        ui->colorG->setValue(acolor.green());
        ui->colorB->setValue(acolor.blue());
        ui->colorA->setValue(acolor.alpha());
        //applay change
        this->applayChange();
    });
    //update frame color
    updateFrameColor();
}

void UISprite::applayChange()
{
    this->csprite->applayChange();
}


void UISprite::setTexture(const QString& name)
{
    ui->selectTexture->setText(name);
}
QString UISprite::getTexture()
{
    return ui->selectTexture->text();
}

void UISprite::updateFrameColor()
{
    //color frame
    QColor acolor;
    acolor.setRgb(  ui->colorR->value(),
                    ui->colorG->value(),
                    ui->colorB->value(),
                    ui->colorA->value() );
    QString scolor=acolor.name();
    ui->widgetColor->setStyleSheet("background-color: "+scolor+";");
    //applay change
    applayChange();
}

//cullface
void UISprite::setCullface(Easy2D::CullFace type)
{
    switch (type)
    {
        case Easy2D::DISABLE: ui->selectFaceCulling->setCurrentText("DISABLE"); break;
        case Easy2D::BACK: ui->selectFaceCulling->setCurrentText("BACK"); break;
        case Easy2D::FRONT: ui->selectFaceCulling->setCurrentText("FRONT"); break;
        default: break;
    }
}
Easy2D::CullFace UISprite::getCullface()
{
    if(ui->selectFaceCulling->currentText()=="BACK") return Easy2D::BACK;
    if(ui->selectFaceCulling->currentText()=="FRONT") return Easy2D::FRONT;
    return Easy2D::DISABLE;
}
//colors
void UISprite::setColor(const Easy2D::Color& color)
{
    ui->colorR->setValue(color.r);
    ui->colorG->setValue(color.g);
    ui->colorB->setValue(color.b);
    ui->colorA->setValue(color.a);
}
Easy2D::Color UISprite::getColor()
{
   return Easy2D::Color(ui->colorR->value(),
                        ui->colorG->value(),
                        ui->colorB->value(),
                        ui->colorA->value());
}
//blends
void UISprite::setBlend(int blendsrc,int blenddst)
{
    ui->cbBlendSrc->setCurrentText(QString::fromUtf8(Easy2D::BLEND::toString(blendsrc)));
    ui->cbBlendDst->setCurrentText(QString::fromUtf8(Easy2D::BLEND::toString(blenddst)));
}
int UISprite::getBlendSrc()
{
    return ui->cbBlendSrc->currentData().toInt();
}
int UISprite::getBlendDst()
{
    return ui->cbBlendDst->currentData().toInt();
}

UISprite::~UISprite()
{
    delete ui;
}
