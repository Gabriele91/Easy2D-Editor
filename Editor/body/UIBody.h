#ifndef UIBODY_H
#define UIBODY_H

#include <QWidget>
#include <Easy2D.h>
#include <QComboBox>

namespace Ui
{
    class UIBody;
}

class ComponentBody;
class UIBody : public QWidget
{
    Q_OBJECT

public:
    explicit UIBody(ComponentBody *cbody = 0);
    ~UIBody();

    //init ui
    void initUIBody(const Easy2D::Body* body);
    void updateListShape(const Easy2D::Body* body);
    void applayToBody(Easy2D::Body* body);
    //shape ui
    void enableUIShape(const Easy2D::Body* body,Easy2D::Shape index);
    void applayToShape(Easy2D::Body *body, Easy2D::Shape index);
    void disableUIShape();
    //vertex
    void enableUIVertex(const Easy2D::Vec2& v2e);
    void setUIVertex(const Easy2D::Vec2& v2e);
    void applayVertex(Easy2D::Vec2& v2e);
    void disableUIVertex();

private:

    void setCBoxTF(QComboBox* cb,bool value);
    bool getCBoxTF(QComboBox* cb);
    void setType(Easy2D::Body::Type type);
    Easy2D::Body::Type getBodyType();

    Ui::UIBody *ui;
    ComponentBody* cbody;
};

#endif // UIPARALLAX_H
