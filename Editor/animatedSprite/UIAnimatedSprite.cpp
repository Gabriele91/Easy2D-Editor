#include <QPalette>
#include <QColorDialog>
#include <QCheckBox>
#include <QFormLayout>
#include "PluginAnimatedSprite.h"
#include "UIAnimatedSprite.h"
#include "ui_UIAnimatedSprite.h"

UIAnimatedSprite::UIAnimatedSprite(ComponentAnimatedSprite* csprite) :
    QWidget(nullptr),
    ui(new Ui::UIAnimatedSprite),
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

    connect(ui->colorR,SIGNAL(valueChanged(int)),SLOT(updateFrameColor()));
    connect(ui->colorG,SIGNAL(valueChanged(int)),SLOT(updateFrameColor()));
    connect(ui->colorB,SIGNAL(valueChanged(int)),SLOT(updateFrameColor()));
    connect(ui->colorA,SIGNAL(valueChanged(int)),SLOT(updateFrameColor()));

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

    //attach a animation
    connect(ui->pbAddAnimation,&QPushButton::clicked,this,
    [this](bool)
    {
        Animation anim=this->csprite->selectAnimation();
        this->pushAnimation(anim);
        this->applayFrames();
    });
}

void UIAnimatedSprite::initAnimations(const Animations& animations,
                                      int animDefault,
                                      int frameDefault)
{
    //save defaults
    vdefault.animation=animDefault;
    vdefault.frame=frameDefault;
    //add animations
    for(size_t i=0;i!=animations.frames.size();++i)
    {
        //add some pages
        QString name(animations.frames[i]->getName().c_str());
        addFrameSet(name,animations.times[i],
                    animations.loops[i]);
    }
}
void UIAnimatedSprite::pushAnimation(const Animation& anim)
{
    if(anim.frame)
    {
        QString name(anim.frame->getName().c_str());
        addFrameSet(name,
                    anim.time,
                    anim.loop);
    }
}

void UIAnimatedSprite::getAnimations(Animations& anim)
{
    for(auto group : frameSets)
    {
       auto pb=group->findChild<QPushButton*>("pbFrameSet");
       anim.frames.push_back(csprite->
                             resources()->
                             load<Easy2D::FrameSet>(
                                 pb->text().toUtf8().data()
                                 ));
       auto dsb=group->findChild<QDoubleSpinBox*>("defaultTime");
       anim.times.push_back(dsb->value());
       auto cb=group->findChild<QCheckBox*>("cbLoop");
       anim.loops.push_back(cb->isChecked());
    }
}

void UIAnimatedSprite::clearAnimations()
{
    for(auto group : frameSets)
    {
        delete group;
    }
    frameSets.clear();
}

void UIAnimatedSprite::addFrameSet(const QString &name,
                                   float vDefaultTime,
                                   bool loop)
{
    /* animation id */
    int animId=frameSets.size();
    /* box */
    auto horizontalGroupBox = new QGroupBox(ui->frameContents);
    horizontalGroupBox->setObjectName(QStringLiteral("gb")+QString::number(frameSets.size()));
    {
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(horizontalGroupBox->sizePolicy().hasHeightForWidth());
    horizontalGroupBox->setSizePolicy(sizePolicy);
    }
    horizontalGroupBox->setMinimumSize(QSize(0, 80));
    horizontalGroupBox->setMaximumSize(QSize(16777215, 16777215));

    //attributes box
    {
        ////////////////////////////////////////////////////
        auto layoutBox = new QVBoxLayout(horizontalGroupBox);
        layoutBox->setObjectName(QStringLiteral("layoutBox"));
        /* Button */
        auto pbFrameSet = new QPushButton(horizontalGroupBox);
        pbFrameSet->setObjectName(QStringLiteral("pbFrameSet"));
        pbFrameSet->setMaximumSize(QSize(16777215, 16777215));
        pbFrameSet->setText(name);
        //add
        layoutBox->addWidget(pbFrameSet);
        ////////////////////////////////////////////////////

        ////////////////////////////////////////////////////
        //layouts
        auto layoutAttributes=new QFormLayout(horizontalGroupBox);
        layoutAttributes->setObjectName(QStringLiteral("layoutAttributes"));
        layoutAttributes->setLabelAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        layoutAttributes->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        layoutBox->addLayout(layoutAttributes);
        ////////////////////////////////////////////////////

        ////////////////////////////////////////////////////
        /* Button */
        auto pbFSDelete = new QPushButton(horizontalGroupBox);
        pbFSDelete->setObjectName(QStringLiteral("pbFSDelete"));
        pbFSDelete->setMaximumSize(QSize(16777215, 16777215));
        pbFSDelete->setText("delete");
        //add
        layoutBox->addWidget(pbFSDelete);
        ////////////////////////////////////////////////////

        ////////////////////////////////////////////////////
        /* default time */
        auto lbDefaultTime = new QLabel(horizontalGroupBox);
        lbDefaultTime->setObjectName(QStringLiteral("lbDefaultTime"));
        {
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lbDefaultTime->sizePolicy().hasHeightForWidth());
        lbDefaultTime->setSizePolicy(sizePolicy);
        }
        lbDefaultTime->setText("time");

        auto defaultTime = new QDoubleSpinBox(horizontalGroupBox);
        defaultTime->setObjectName(QStringLiteral("defaultTime"));
        {
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(defaultTime->sizePolicy().hasHeightForWidth());
        defaultTime->setSizePolicy(sizePolicy);
        }
        defaultTime->setValue(vDefaultTime);
        //add
        layoutAttributes->addRow(lbDefaultTime,defaultTime);

        /* loop */
        auto lbLoop = new QLabel(horizontalGroupBox);
        lbLoop->setObjectName(QStringLiteral("lbLoop"));
        {
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lbDefaultTime->sizePolicy().hasHeightForWidth());
        lbLoop->setSizePolicy(sizePolicy);
        }
        lbLoop->setText("loop");

        auto cbLoop = new QCheckBox(horizontalGroupBox);
        cbLoop->setObjectName(QStringLiteral("cbLoop"));
        {
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(cbLoop->sizePolicy().hasHeightForWidth());
        cbLoop->setSizePolicy(sizePolicy);
        }
        cbLoop->setChecked(loop);
        //add
        layoutAttributes->addRow(lbLoop,cbLoop);

        /* start frame */
        auto lbSFrame = new QLabel(horizontalGroupBox);
        lbSFrame->setObjectName(QStringLiteral("lbSFrame"));
        {
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lbDefaultTime->sizePolicy().hasHeightForWidth());
        lbSFrame->setSizePolicy(sizePolicy);
        }
        lbSFrame->setText("start");

        auto sbSFrame = new QSpinBox(horizontalGroupBox);
        sbSFrame->setObjectName(QStringLiteral("sbSFrame"));
        {
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(defaultTime->sizePolicy().hasHeightForWidth());
        sbSFrame->setSizePolicy(sizePolicy);
        }
        //if is first enable
        if(vdefault.animation==animId)
        {
            sbSFrame->setValue(vdefault.frame);
            sbSFrame->setEnabled(true);
        }
        else
        {
            sbSFrame->setValue(0);
            sbSFrame->setEnabled(false);
        }
        //add
        layoutAttributes->addRow(lbSFrame,sbSFrame);

        /* default animation */
        auto lbDefault = new QLabel(horizontalGroupBox);
        lbDefault->setObjectName(QStringLiteral("lbDefault"));
        {
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lbDefaultTime->sizePolicy().hasHeightForWidth());
        lbDefault->setSizePolicy(sizePolicy);
        }
        lbDefault->setText("default");

        auto cbDefault = new QCheckBox(horizontalGroupBox);
        cbDefault->setObjectName(QStringLiteral("cbDefault"));
        {
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(cbLoop->sizePolicy().hasHeightForWidth());
        cbDefault->setSizePolicy(sizePolicy);
        }
        //if is first check
        if(vdefault.animation==animId)
        {
            cbDefault->setChecked(true);
            cbDefault->setDisabled(true);
        }
        else
        {
            cbDefault->setChecked(false);
            cbDefault->setDisabled(false);
        }
        //add
        layoutAttributes->addRow(lbDefault,cbDefault);
        ////////////////////////////////////////////////////
        //add into list
        frameSets.push_back(horizontalGroupBox);
        //add into layout
        ui->verticalLayoutContents->addWidget(horizontalGroupBox);
        ////////////////////////////////////////////////////
        //attachs
        //change frame set signal
        connect(pbFrameSet,&QPushButton::clicked,this,
        [this,pbFrameSet](bool)
        {
            //get frame
            Animation anim=this->csprite->selectAnimation(pbFrameSet->text());
            if(!anim.frame) return;
            //set anim
            pbFrameSet->setText(anim.frame->getName().c_str());
            //applay
            this->applayFrames();
        });
        //change time signal
        connect(defaultTime,SIGNAL(valueChanged(double)), this,SLOT(applayFrames()));
        //change loop signal
        connect(cbLoop,&QCheckBox::stateChanged,this,&UIAnimatedSprite::applayFrames);
        //change start frame signal
        connect(sbSFrame,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this,
        [this,sbSFrame](int v){
            int frame=this->csprite->setFrame(v);
            if(v!=frame)
            {
                vdefault.frame=frame;
                sbSFrame->blockSignals(true);
                sbSFrame->setValue(frame);
                sbSFrame->blockSignals(false);
            }
        });
        //change default signal
        cbDefault->setProperty("animId",animId);
        //connect
        connect(cbDefault,&QCheckBox::stateChanged,this,
        [this,horizontalGroupBox,cbDefault,sbSFrame,animId](int)
        {
            if(cbDefault->isChecked())
            {
                //get id
                vdefault.animation=cbDefault->property("animId").toInt();
                this->applayDefaultAnim(horizontalGroupBox);
                cbDefault->setDisabled(true);
                sbSFrame->setEnabled(true);
            }
        });
        //delete frameset signal
        pbFSDelete->setProperty("animId",animId);
        //connect
        connect(pbFSDelete,&QPushButton::clicked,this,
        [this,horizontalGroupBox,pbFSDelete](bool)
        {
            //get id
            int animId=pbFSDelete->property("animId").toInt();
            //erase
            frameSets.erase(
            std::find(frameSets.begin(),frameSets.end(),horizontalGroupBox)
            );
            //close
            horizontalGroupBox->close();
            horizontalGroupBox->destroyed();
            //delete
            delete horizontalGroupBox;
            //applay
            this->applayFrames();
            //update delete
            deleteDefault(animId);
        });
    }


}

//update frame color
void UIAnimatedSprite::updateFrameColor()
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
//update change
void UIAnimatedSprite::applayChange()
{
    csprite->applayChange();
}
void UIAnimatedSprite::applayFrames()
{
    csprite->applayFrames();
}
void UIAnimatedSprite::applayDefaultAnim(QWidget* widget)
{
    int i=0;
    for(auto group : frameSets)
    {
        if(group==widget)
        {
            //set default animation
            csprite->setDefaultAnimation(i);
            //set frame
            QSpinBox* sb=group->findChild<QSpinBox*>("sbSFrame");
            sb->setValue(csprite->setFrame(sb->value()));
        }
        else
        {
            QCheckBox* cb=group->findChild<QCheckBox*>("cbDefault");
            QSpinBox* sb=group->findChild<QSpinBox*>("sbSFrame");
            cb->blockSignals(true);
            cb->setChecked(false);
            cb->blockSignals(false);
            cb->setDisabled(false);
            sb->setEnabled(false);
        }
        ++i;
    }
}

//update default value
void UIAnimatedSprite::deleteDefault(int animDelete)
{
    if(animDelete<vdefault.animation)
    {
        //update animation
        vdefault.animation-=1;
        //clamp
        if(vdefault.animation<0)
            vdefault.animation=0;
    }
    else if(animDelete==vdefault.animation)
    {
        vdefault.animation=0;
    }

    //update ids
    for(int i=animDelete;i<frameSets.size();++i)
    {
        QCheckBox*   cb=frameSets[i]->findChild<QCheckBox*>("cbDefault");
        QPushButton* pb=frameSets[i]->findChild<QPushButton*>("pbFSDelete");
        int animId= cb->property("animId").toInt()-1;
            animId= animId < 0 ? 0 : animId ;
        cb->setProperty("animId",animId);
        pb->setProperty("animId",animId);
    }

    updateDefaultUI();
}
void UIAnimatedSprite::updateDefaultUI()
{
    int i=0;
    for(auto group : frameSets)
    {
        if(i==vdefault.animation)
        {
            //set default animation
            csprite->setDefaultAnimation(i);
            //set anim ui
            QCheckBox* cb=group->findChild<QCheckBox*>("cbDefault");
            cb->blockSignals(true);
            cb->setChecked(true);
            cb->blockSignals(false);
            cb->setDisabled(true);
            //set frame
            QSpinBox* sb=group->findChild<QSpinBox*>("sbSFrame");
            sb->setValue(csprite->setFrame(sb->value()));
            sb->setEnabled(true);
        }
        else
        {
            QCheckBox* cb=group->findChild<QCheckBox*>("cbDefault");
            QSpinBox* sb=group->findChild<QSpinBox*>("sbSFrame");
            cb->blockSignals(true);
            cb->setChecked(false);
            cb->blockSignals(false);
            cb->setDisabled(false);
            sb->setEnabled(false);
        }
        ++i;
    }
}
//cullface
void UIAnimatedSprite::setCullface(Easy2D::CullFace type)
{
    switch (type)
    {
        case Easy2D::DISABLE: ui->selectFaceCulling->setCurrentText("DISABLE"); break;
        case Easy2D::BACK: ui->selectFaceCulling->setCurrentText("BACK"); break;
        case Easy2D::FRONT: ui->selectFaceCulling->setCurrentText("FRONT"); break;
        default: break;
    }
}
Easy2D::CullFace UIAnimatedSprite::getCullface()
{
    if(ui->selectFaceCulling->currentText()=="BACK") return Easy2D::BACK;
    if(ui->selectFaceCulling->currentText()=="FRONT") return Easy2D::FRONT;
    return Easy2D::DISABLE;
}
//colors
void UIAnimatedSprite::setColor(const Easy2D::Color& color)
{
    ui->colorR->setValue(color.r);
    ui->colorG->setValue(color.g);
    ui->colorB->setValue(color.b);
    ui->colorA->setValue(color.a);
}
Easy2D::Color UIAnimatedSprite::getColor()
{
   return Easy2D::Color(ui->colorR->value(),
                        ui->colorG->value(),
                        ui->colorB->value(),
                        ui->colorA->value());
}
//blends
void UIAnimatedSprite::setBlend(int blendsrc,int blenddst)
{
    ui->cbBlendSrc->setCurrentText(Easy2D::BLEND::toString(blendsrc).c_str());
    ui->cbBlendDst->setCurrentText(Easy2D::BLEND::toString(blenddst).c_str());
}
int UIAnimatedSprite::getBlendSrc()
{
    return ui->cbBlendSrc->currentData().toInt();
}
int UIAnimatedSprite::getBlendDst()
{
    return ui->cbBlendDst->currentData().toInt();
}

UIAnimatedSprite::~UIAnimatedSprite()
{
    delete ui;
}
