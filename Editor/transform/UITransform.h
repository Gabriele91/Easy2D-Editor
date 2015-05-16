#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QWidget>
#include <Easy2D.h>

namespace Ui{ class UITransform; };
class ComponentTransform;
class UITransform : public QWidget
{
    Q_OBJECT

public:
    explicit UITransform(ComponentTransform *parent = 0);
    ~UITransform();
    Ui::UITransform *ui;

    void setPosition(const Easy2D::Vec2& pos);
    Easy2D::Vec2 getPosition();

    void setScale(const Easy2D::Vec2& scale);
    Easy2D::Vec2 getScale();

    void setRotation(Easy2D::Angle rot);
    Easy2D::Angle getRotation();

    void setOrder(int val_z);
    int getOrder();

    int getParentMode();
    void setParentMode(int mode);


};

#endif // TRANSFORM_H
