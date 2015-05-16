#include <UIParallax.h>
#include <PluginParallax.h>
#include <ui_UIParallax.h>

UIParallax::UIParallax(ComponentParallax *cparallax) :
    QWidget(0),
    ui(new Ui::UIParallax)
{
    ui->setupUi(this);
    //set parallax validetor
    ui->par_x->setValidator(new QDoubleValidator());
    ui->par_y->setValidator(new QDoubleValidator());

    //connects
    auto lbParallax=[=](const QString&){
        cparallax->applayChange();
    };
    connect(ui->par_x,&QLineEdit::textChanged,lbParallax);
    connect(ui->par_y,&QLineEdit::textChanged,lbParallax);
}


void UIParallax::setParallax(const Easy2D::Vec2& scale)
{
    ui->par_x->setText(QString::number(scale.x));
    ui->par_y->setText(QString::number(scale.y));
    //left
    ui->par_x->setCursorPosition( 0 );
    ui->par_y->setCursorPosition( 0 );
}
Easy2D::Vec2 UIParallax::getParallax()
{
    return Easy2D::Vec2(
    ui->par_x->text().toFloat(),
    ui->par_y->text().toFloat()
    );
}

UIParallax::~UIParallax()
{
    delete ui;
}
