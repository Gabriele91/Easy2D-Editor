#include <UIBody.h>
#include <PluginBody.h>
#include <ui_UIBody.h>

UIBody::UIBody(ComponentBody *cbody) :
    QWidget(0),
    ui(new Ui::UIBody),
    cbody(cbody)
{
    ui->setupUi(this);
    //setup ui
    ui->leLVelocity_x->setValidator(new QDoubleValidator);
    ui->leLVelocity_y->setValidator(new QDoubleValidator);
    ui->leAVelocity->setValidator(new QDoubleValidator);
    ui->leLDemping->setValidator(new QDoubleValidator);
    ui->leADemping->setValidator(new QDoubleValidator);
    ui->leGScale->setValidator(new QDoubleValidator);

    //disable shape editor
    ui->ggbShape->setEnabled(false);
    ui->leDensity->setValidator(new QDoubleValidator);
    ui->leFriction->setValidator(new QDoubleValidator);
    ui->leRestitution->setValidator(new QDoubleValidator);

    //disable vertex ui
    ui->ggbVertex->setEnabled(false);
    ui->ggbVertex->setVisible(false);
    ui->leVertex_x->blockSignals(true);
    ui->leVertex_y->blockSignals(true);
    ui->leVertex_x->setValidator(new QDoubleValidator);
    ui->leVertex_y->setValidator(new QDoubleValidator);

    //disable push buttons/list shape
    ui->pbDisableShape->setEnabled(false);
    ui->pbDeleteShape->setEnabled(false);

    //connests

    //values events
    auto leApplay=
    [this](const QString&)
    {
        this->cbody->applayChange();
    };
    connect(ui->leLVelocity_x,&QLineEdit::textChanged,leApplay);
    connect(ui->leLVelocity_y,&QLineEdit::textChanged,leApplay);
    connect(ui->leAVelocity,&QLineEdit::textChanged,leApplay);
    connect(ui->leLDemping,&QLineEdit::textChanged,leApplay);
    connect(ui->leADemping,&QLineEdit::textChanged,leApplay);
    connect(ui->leGScale,&QLineEdit::textChanged,leApplay);

    //bools events
    auto cbApplay=
    [this](int)
    {
        this->cbody->applayChange();
    };
    auto signalcb=static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged);
    connect(ui->cbActive,signalcb,cbApplay);
    connect(ui->cbAwake,signalcb,cbApplay);
    connect(ui->cbASleep,signalcb,cbApplay);
    connect(ui->cbBullet,signalcb,cbApplay);
    connect(ui->cbFRotation,signalcb,cbApplay);
    connect(ui->cbEnableScale,signalcb,cbApplay);

    //type event
    connect(ui->cbType,signalcb,cbApplay);

    //shape connests
    connect(ui->leDensity,&QLineEdit::textChanged,leApplay);
    connect(ui->leFriction,&QLineEdit::textChanged,leApplay);
    connect(ui->leRestitution,&QLineEdit::textChanged,leApplay);
    connect(ui->cbSensor,signalcb,cbApplay);

    //list shape event
    auto lwEditMode=
    [this](QListWidgetItem* item)
    {
        this->cbody->enabeEditMode((Easy2D::Shape)item->data(Qt::UserRole).toInt());
    };
    connect(ui->listShapes,&QListWidget::itemDoubleClicked,lwEditMode);

    //applay vertex
    auto leApplayVertex=
    [this](const QString&)
    {
        this->cbody->applayVertex();
    };
    connect(ui->leVertex_x,&QLineEdit::textChanged,leApplayVertex);
    connect(ui->leVertex_y,&QLineEdit::textChanged,leApplayVertex);

    //add/delete shape
    connect(ui->pbAddShape,&QPushButton::clicked,
    [this]()
    {
       this->cbody->addShape();
    });
    connect(ui->pbDisableShape,&QPushButton::clicked,
    [this]()
    {
        this->cbody->disableEditMode();
    });
    connect(ui->pbDeleteShape,&QPushButton::clicked,
    [this]()
    {
        this->cbody->deleteCurrentShape();
    });

}


void UIBody::initUIBody(const Easy2D::Body *body)
{
    //set numbers
    ui->leLVelocity_x->setText(QString::number(body->getLinearVelocity().x));
    ui->leLVelocity_y->setText(QString::number(body->getLinearVelocity().y));
    ui->leAVelocity->setText(QString::number(body->getAngularVelocity()));
    ui->leLDemping->setText(QString::number(body->getLinearDamping()));
    ui->leADemping->setText(QString::number(body->getAngularDamping()));
    ui->leGScale->setText(QString::number(body->getGravityScale()));
    //set bools
    setCBoxTF(ui->cbActive,body->getActive());
    setCBoxTF(ui->cbAwake,body->getAwake());
    setCBoxTF(ui->cbASleep,body->getSleepingAllowed());
    setCBoxTF(ui->cbBullet,body->getBullet());
    setCBoxTF(ui->cbFRotation,body->getFixedAngle());
    setCBoxTF(ui->cbEnableScale,body->getEnableScale());
    //set type
    setType(body->getType());
    //add shape list
    updateListShape(body);
}
void UIBody::updateListShape(const Easy2D::Body* body)
{
    //clear list
    ui->listShapes->clear();
    //add shape list
    for(const Easy2D::Shape sp : *body)
    {
       //geometry type
       switch (body->getShapeType(sp))
       {
           case Easy2D::Body::CIRCLE:
           {
                QListWidgetItem* item=new QListWidgetItem("Circle");
                item->setData(Qt::UserRole,sp);
                ui->listShapes->addItem(item);
           }
           break;
           case Easy2D::Body::POLYGON:
           {
                QListWidgetItem* item=new QListWidgetItem("Polygon");
                item->setData(Qt::UserRole,sp);
                ui->listShapes->addItem(item);
           }
           break;
           case Easy2D::Body::CHAIN:
           {
                QListWidgetItem* item=new QListWidgetItem("Chain");
                item->setData(Qt::UserRole,sp);
                ui->listShapes->addItem(item);
           }
           break;
           case Easy2D::Body::EDGE:
           {
                QListWidgetItem* item=new QListWidgetItem("Edge");
                item->setData(Qt::UserRole,sp);
                ui->listShapes->addItem(item);
           }
           break;
           default:
           break;
       }
    }
}

void UIBody::applayToBody(Easy2D::Body* body)
{
    //get numbers
    body->setLinearVelocity(Easy2D::Vec2(ui->leLVelocity_x->text().toFloat(),
                                         ui->leLVelocity_y->text().toFloat()));
    body->setAngularVelocity(ui->leAVelocity->text().toFloat());
    body->setLinearDamping(ui->leLDemping->text().toFloat());
    body->setAngularDamping(ui->leADemping->text().toFloat());
    body->setGravityScale(ui->leGScale->text().toFloat());
    //get bools
    body->setActive(getCBoxTF(ui->cbActive));
    body->setAwake(getCBoxTF(ui->cbAwake));;
    body->setSleepingAllowed(getCBoxTF(ui->cbASleep));;
    body->setBullet(getCBoxTF(ui->cbBullet));
    body->setFixedAngle(getCBoxTF(ui->cbFRotation));
    body->setEnableScale(getCBoxTF(ui->cbEnableScale));
    //get type
    body->setType(getBodyType());
}


void UIBody::enableUIShape(const Easy2D::Body* body, Easy2D::Shape index)
{
    //enable ui
    ui->pbDisableShape->setEnabled(true);
    ui->pbDeleteShape->setEnabled(true);
    ui->ggbShape->setEnabled(true);
    ui->leDensity->setText(QString::number(body->getShapeDensity(index)));
    ui->leFriction->setText(QString::number(body->getShapeFriction(index)));
    ui->leRestitution->setText(QString::number(body->getShapeRestitution(index)));
    setCBoxTF(ui->cbSensor,body->getShapeIsSensor(index));
}
void UIBody::applayToShape(Easy2D::Body* body, Easy2D::Shape index)
{
    body->setShapeDensity(index,ui->leDensity->text().toFloat());
    body->setShapeFriction(index,ui->leFriction->text().toFloat());
    body->setShapeRestitution(index,ui->leRestitution->text().toFloat());
    body->setShapeIsSensor(index,getCBoxTF(ui->cbSensor));
}
void UIBody::disableUIShape()
{
    //disable ui
    ui->pbDisableShape->setEnabled(false);
    ui->pbDeleteShape->setEnabled(false);
    ui->ggbShape->setEnabled(false);
}


//vertex
void UIBody::enableUIVertex(const Easy2D::Vec2& v2e)
{
    setUIVertex(v2e);
    ui->ggbVertex->setEnabled(true);
    ui->ggbVertex->setVisible(true);
}
void UIBody::setUIVertex(const Easy2D::Vec2& v2e)
{
    ui->leVertex_x->blockSignals(true);
    ui->leVertex_y->blockSignals(true);

    ui->leVertex_x->setText(QString::number(v2e.x));
    ui->leVertex_y->setText(QString::number(v2e.y));

    ui->leVertex_x->blockSignals(false);
    ui->leVertex_y->blockSignals(false);
}
void UIBody::applayVertex(Easy2D::Vec2& v2e)
{
    v2e.x=ui->leVertex_x->text().toFloat();
    v2e.y=ui->leVertex_y->text().toFloat();
}
void UIBody::disableUIVertex()
{
    ui->leVertex_x->blockSignals(true);
    ui->leVertex_y->blockSignals(true);
    ui->ggbVertex->setEnabled(false);
    ui->ggbVertex->setVisible(false);
}



void UIBody::setCBoxTF(QComboBox* cb,bool value)
{
    cb->setCurrentIndex((int)value);
}
bool UIBody::getCBoxTF(QComboBox* cb)
{
    return cb->currentIndex()!=0;
}


void UIBody::setType(Easy2D::Body::Type type)
{
    switch (type)
    {
        case Easy2D::Body::STATIC: ui->cbType->setCurrentIndex(0); break;
        case Easy2D::Body::DINAMIC: ui->cbType->setCurrentIndex(1); break;
        case Easy2D::Body::KINEMATIC: ui->cbType->setCurrentIndex(2); break;
        default:break;
    }
}
Easy2D::Body::Type UIBody::getBodyType()
{
    if(ui->cbType->currentText()=="Dynamic")
    {
        return Easy2D::Body::DINAMIC;
    }
    else if(ui->cbType->currentText()=="Kinematic")
    {
        return Easy2D::Body::KINEMATIC;
    }

    return Easy2D::Body::STATIC;
}

UIBody::~UIBody()
{
    delete ui;
}
