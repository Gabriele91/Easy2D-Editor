#include "UIEmitter.h"
#include "ui_UIEmitter.h"
#include <PluginEmitter.h>

UIEmitter::UIEmitter(ComponentEmitter *cemitter)
    :QWidget(nullptr)
    ,ui(new Ui::UIEmitter)
    ,cemitter(cemitter)
{
    ui->setupUi(this);

    //set validators globals
    ui->leMaxParticles->setValidator(new QIntValidator);
    ui->leEmissionRate->setValidator(new QDoubleValidator);
    ui->leDuration->setValidator(new QDoubleValidator);
    ui->leGravity_x->setValidator(new QDoubleValidator);
    ui->leGravity_y->setValidator(new QDoubleValidator);
    //set validetors defaults
    ui->lePosition_x->setValidator(new QDoubleValidator);
    ui->lePosition_y->setValidator(new QDoubleValidator);
    ui->leDirection_x->setValidator(new QDoubleValidator);
    ui->leDirection_y->setValidator(new QDoubleValidator);
    ui->leStartScale_x->setValidator(new QDoubleValidator);
    ui->leStartScale_y->setValidator(new QDoubleValidator);
    ui->leEndScale_x->setValidator(new QDoubleValidator);
    ui->leEndScale_y->setValidator(new QDoubleValidator);
    ui->leStartSpin->setValidator(new QDoubleValidator);
    ui->leEndSpin->setValidator(new QDoubleValidator);
    ui->leLifeTime->setValidator(new QDoubleValidator);
    //set validetors variance
    ui->leVPosition_x->setValidator(new QDoubleValidator);
    ui->leVPosition_y->setValidator(new QDoubleValidator);
    ui->leVDirection_x->setValidator(new QDoubleValidator);
    ui->leVDirection_y->setValidator(new QDoubleValidator);
    ui->leVStartScale_x->setValidator(new QDoubleValidator);
    ui->leVStartScale_y->setValidator(new QDoubleValidator);
    ui->leVEndScale_x->setValidator(new QDoubleValidator);
    ui->leVEndScale_y->setValidator(new QDoubleValidator);
    ui->leVStartSpin->setValidator(new QDoubleValidator);
    ui->leVEndSpin->setValidator(new QDoubleValidator);
    ui->leVLifeTime->setValidator(new QDoubleValidator);

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
    //no in dest
    //ui->cbBlendDst->addItem("SRC::ALPHA::SATURATE",Easy2D::BLEND::SRC::ALPHA::SATURATE);

    //connetcs
    connect(ui->pbTexture,&QPushButton::clicked,this,
    [this](bool)
    {
        QString name=this->cemitter->selectTexture();
        //set texture
        if(this->cemitter->setTexture(name))
        {
            //update name
            this->ui->pbTexture->setText(name);
        }
    });


    //update vent
    auto leGlobalsApplay=
    [this](const QString&)
    {
        this->cemitter->applayGlobalsChange();
    };
    //globals
    connect(ui->leMaxParticles,&QLineEdit::textChanged,leGlobalsApplay);
    connect(ui->leEmissionRate,&QLineEdit::textChanged,leGlobalsApplay);
    connect(ui->leDuration,&QLineEdit::textChanged,leGlobalsApplay);
    connect(ui->leGravity_x,&QLineEdit::textChanged,leGlobalsApplay);
    connect(ui->leGravity_y,&QLineEdit::textChanged,leGlobalsApplay);

    //cb box Globals
    //bools events
    auto cbGlobalsApplay=
    [this](int)
    {
        this->cemitter->applayGlobalsChange();
    };
    auto signalcb=static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged);

    connect(ui->cbRelative,signalcb,cbGlobalsApplay);
    connect(ui->cbBlendSrc,signalcb,cbGlobalsApplay);
    connect(ui->cbBlendDst,signalcb,cbGlobalsApplay);


    //manager
    connect(ui->pbRestart,&QPushButton::clicked,
    [this](bool)
    {
        this->cemitter->restart();
    });
    connect(ui->pbPausePlay,&QPushButton::clicked,
    [this](bool)
    {
        if(this->ui->pbPausePlay->text()=="Play")
        {
            this->cemitter->play();
            this->ui->pbPausePlay->setText("Play");
        }
        else
        if(this->ui->pbPausePlay->text()=="Pause")
        {
            this->cemitter->pause();
            this->ui->pbPausePlay->setText("Pause");
        }
    });

    //update vent
    auto leApplay=
    [this](const QString&)
    {
        this->cemitter->applayChange();
    };
    //default
    connect(ui->lePosition_x,&QLineEdit::textChanged,leApplay);
    connect(ui->lePosition_y,&QLineEdit::textChanged,leApplay);
    connect(ui->leDirection_x,&QLineEdit::textChanged,leApplay);
    connect(ui->leDirection_y,&QLineEdit::textChanged,leApplay);
    connect(ui->leStartScale_x,&QLineEdit::textChanged,leApplay);
    connect(ui->leStartScale_y,&QLineEdit::textChanged,leApplay);
    connect(ui->leEndScale_x,&QLineEdit::textChanged,leApplay);
    connect(ui->leEndScale_y,&QLineEdit::textChanged,leApplay);
    connect(ui->leStartSpin,&QLineEdit::textChanged,leApplay);
    connect(ui->leEndSpin,&QLineEdit::textChanged,leApplay);
    connect(ui->leLifeTime,&QLineEdit::textChanged,leApplay);

    //var
    connect(ui->leVPosition_x,&QLineEdit::textChanged,leApplay);
    connect(ui->leVPosition_y,&QLineEdit::textChanged,leApplay);
    connect(ui->leVDirection_x,&QLineEdit::textChanged,leApplay);
    connect(ui->leVDirection_y,&QLineEdit::textChanged,leApplay);
    connect(ui->leVStartScale_x,&QLineEdit::textChanged,leApplay);
    connect(ui->leVStartScale_y,&QLineEdit::textChanged,leApplay);
    connect(ui->leVEndScale_x,&QLineEdit::textChanged,leApplay);
    connect(ui->leVEndScale_y,&QLineEdit::textChanged,leApplay);
    connect(ui->leVStartSpin,&QLineEdit::textChanged,leApplay);
    connect(ui->leVEndSpin,&QLineEdit::textChanged,leApplay);
    connect(ui->leVLifeTime,&QLineEdit::textChanged,leApplay);

    //color
    auto sbApplay=
    [this](int)
    {
        this->cemitter->applayChange();
    };
    auto signalSb=static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged);
    //default
    connect(ui->sbStartColor_r,signalSb,sbApplay);
    connect(ui->sbStartColor_g,signalSb,sbApplay);
    connect(ui->sbStartColor_b,signalSb,sbApplay);
    connect(ui->sbStartColor_a,signalSb,sbApplay);
    connect(ui->sbEndColor_r,signalSb,sbApplay);
    connect(ui->sbEndColor_g,signalSb,sbApplay);
    connect(ui->sbEndColor_b,signalSb,sbApplay);
    connect(ui->sbEndColor_a,signalSb,sbApplay);
    //vars
    connect(ui->sbVStartColor_r,signalSb,sbApplay);
    connect(ui->sbVStartColor_g,signalSb,sbApplay);
    connect(ui->sbVStartColor_b,signalSb,sbApplay);
    connect(ui->sbVStartColor_a,signalSb,sbApplay);
    connect(ui->sbVEndColor_r,signalSb,sbApplay);
    connect(ui->sbVEndColor_g,signalSb,sbApplay);
    connect(ui->sbVEndColor_b,signalSb,sbApplay);
    connect(ui->sbVEndColor_a,signalSb,sbApplay);

}
//controll ui
void UIEmitter::updateControllUI(const Easy2D::Emitter& emitter)
{
    ui->pbPausePlay->setText(!emitter.getPause() ? "Pause" : "Play");
}

void UIEmitter::updateUI(const Easy2D::Emitter& emitter)
{
    //update globals
    //
    ui->pbTexture->blockSignals(true);
    ui->pbTexture->setText(cemitter->getTextureName());
    ui->pbTexture->blockSignals(false);

    ui->cbBlendSrc->setCurrentText(Easy2D::BLEND::toString(emitter.getBlendSrc()).c_str());
    ui->cbBlendDst->setCurrentText(Easy2D::BLEND::toString(emitter.getBlendDst()).c_str());
    //
    ui->leMaxParticles->setText(QString::number(emitter.getMaxParticles()));
    ui->leEmissionRate->setText(QString::number(emitter.getEmissionRate()));
    ui->leDuration->setText(QString::number(emitter.getDuration()));
    ui->leGravity_x->setText(QString::number(emitter.getGravity().x));
    ui->leGravity_y->setText(QString::number(emitter.getGravity().x));
    setCBoxTF(ui->cbRelative,emitter.getRelative());

    //update defaults
    ui->lePosition_x->setText(QString::number(emitter.getPosition().x));
    ui->lePosition_y->setText(QString::number(emitter.getPosition().y));
    ui->leDirection_x->setText(QString::number(emitter.getDirection().x));
    ui->leDirection_y->setText(QString::number(emitter.getDirection().y));
    ui->leStartScale_x->setText(QString::number(emitter.getStartScale().x));
    ui->leStartScale_y->setText(QString::number(emitter.getStartScale().y));;
    ui->leEndScale_x->setText(QString::number(emitter.getEndScale().x));
    ui->leEndScale_y->setText(QString::number(emitter.getEndScale().y));
    ui->leStartSpin->setText(QString::number(emitter.getStartSpin().valueDegrees()));
    ui->leEndSpin->setText(QString::number(emitter.getEndSpin().valueDegrees()));
    ui->leLifeTime->setText(QString::number(emitter.getLife()));

    setSBoxColor(ui->sbStartColor_r,
                 ui->sbStartColor_g,
                 ui->sbStartColor_b,
                 ui->sbStartColor_a,
                 emitter.getStartColor());

    setSBoxColor(ui->sbEndColor_r,
                 ui->sbEndColor_g,
                 ui->sbEndColor_b,
                 ui->sbEndColor_a,
                 emitter.getEndColor());

    //update varians
    ui->leVPosition_x->setText(QString::number(emitter.getPositionVar().x));
    ui->leVPosition_y->setText(QString::number(emitter.getPositionVar().y));
    ui->leVDirection_x->setText(QString::number(emitter.getDirectionVar().x));
    ui->leVDirection_y->setText(QString::number(emitter.getDirectionVar().y));
    ui->leVStartScale_x->setText(QString::number(emitter.getStartScaleVar().x));
    ui->leVStartScale_y->setText(QString::number(emitter.getStartScaleVar().y));;
    ui->leVEndScale_x->setText(QString::number(emitter.getEndScaleVar().x));
    ui->leVEndScale_y->setText(QString::number(emitter.getEndScaleVar().y));
    ui->leVStartSpin->setText(QString::number(emitter.getStartSpinVar().valueDegrees()));
    ui->leVEndSpin->setText(QString::number(emitter.getEndSpinVar().valueDegrees()));
    ui->leVLifeTime->setText(QString::number(emitter.getLifeVar()));

    setSBoxColor(ui->sbVStartColor_r,
                 ui->sbVStartColor_g,
                 ui->sbVStartColor_b,
                 ui->sbVStartColor_a,
                 emitter.getStartColorVar());

    setSBoxColor(ui->sbVEndColor_r,
                 ui->sbVEndColor_g,
                 ui->sbVEndColor_b,
                 ui->sbVEndColor_a,
                 emitter.getEndColorVar());

}


void UIEmitter::updateComponentGlobals(Easy2D::Emitter& emitter)
{
    //set globals
    emitter.setMaxParticles(ui->leMaxParticles->text().toInt());
    emitter.setEmissionRate(ui->leEmissionRate->text().toFloat());
    emitter.setDuration(ui->leDuration->text().toFloat());
    emitter.setGravity(getLEditVec2(ui->leGravity_x,ui->leGravity_y));
    emitter.setRelative(getCBoxTF(ui->cbRelative));
    emitter.setBlend(ui->cbBlendSrc->currentData().toUInt(),
                     ui->cbBlendDst->currentData().toUInt());
}

void UIEmitter::updateComponent(Easy2D::Emitter& emitter)
{
    //set defaults
    emitter.setPosition(getLEditVec2(ui->lePosition_x,ui->lePosition_y));
    emitter.setDirection(getLEditVec2(ui->leDirection_x,ui->leDirection_y));
    emitter.setStartScale(getLEditVec2(ui->leStartScale_x,ui->leStartScale_y));
    emitter.setEndScale(getLEditVec2(ui->leEndScale_x,ui->leEndScale_y));
    emitter.setStartSpin(Easy2D::Degree(ui->leStartSpin->text().toFloat()));
    emitter.setEndSpin(Easy2D::Degree(ui->leEndSpin->text().toFloat()));
    emitter.setLife(ui->leLifeTime->text().toFloat());
    emitter.setStartColor(getSBoxColor(ui->sbStartColor_r,
                                       ui->sbStartColor_g,
                                       ui->sbStartColor_b,
                                       ui->sbStartColor_a));
    emitter.setEndColor(getSBoxColor(ui->sbEndColor_r,
                                     ui->sbEndColor_g,
                                     ui->sbEndColor_b,
                                     ui->sbEndColor_a));
    //set variances
    emitter.setPositionVar(getLEditVec2(ui->leVPosition_x,ui->leVPosition_y));
    emitter.setDirectionVar(getLEditVec2(ui->leVDirection_x,ui->leVDirection_y));
    emitter.setStartScaleVar(getLEditVec2(ui->leVStartScale_x,ui->leVStartScale_y));
    emitter.setEndScaleVar(getLEditVec2(ui->leVEndScale_x,ui->leVEndScale_y));
    emitter.setStartSpinVar(Easy2D::Degree(ui->leVStartSpin->text().toFloat()));
    emitter.setEndSpinVar(Easy2D::Degree(ui->leVEndSpin->text().toFloat()));
    emitter.setLifeVar(ui->leVLifeTime->text().toFloat());
    emitter.setStartColorVar(getSBoxColor(ui->sbVStartColor_r,
                                       ui->sbVStartColor_g,
                                       ui->sbVStartColor_b,
                                       ui->sbVStartColor_a));
    emitter.setEndColorVar(getSBoxColor(ui->sbVEndColor_r,
                                     ui->sbVEndColor_g,
                                     ui->sbVEndColor_b,
                                     ui->sbVEndColor_a));


}

void UIEmitter::setSBoxColor(
                  QSpinBox* r,
                  QSpinBox* g,
                  QSpinBox* b,
                  QSpinBox* a,
                  const Easy2D::Color& color)
{
    r->setValue((int)color.r);
    g->setValue((int)color.g);
    b->setValue((int)color.b);
    a->setValue((int)color.a);
}

Easy2D::Color UIEmitter::getSBoxColor(
                           const QSpinBox* r,
                           const QSpinBox* g,
                           const QSpinBox* b,
                           const QSpinBox* a)
{
    return Easy2D::Color((unsigned char)r->value(),
                         (unsigned char)g->value(),
                         (unsigned char)b->value(),
                         (unsigned char)a->value());
}

void UIEmitter::setCBoxTF(QComboBox* cb,bool value)
{
    cb->setCurrentIndex((int)value);
}
bool UIEmitter::getCBoxTF(QComboBox* cb)
{
    return cb->currentIndex()!=0;
}

void UIEmitter::setLEditVec2(QLineEdit* x,
                             QLineEdit* y,
                             const Easy2D::Vec2& v)
{
    x->setText(QString::number(v.x));
    y->setText(QString::number(v.y));
}

Easy2D::Vec2 UIEmitter::getLEditVec2(QLineEdit* x,
                                     QLineEdit* y)
{
    return Easy2D::Vec2(x->text().toFloat(),
                        y->text().toFloat());
}

UIEmitter::~UIEmitter()
{
    delete ui;
}
