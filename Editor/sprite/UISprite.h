#ifndef UISPRITE_H
#define UISPRITE_H

#include <Easy2D.h>
#include <QWidget>

namespace Ui
{
    class UISprite;
}

class ComponentSprite;
class UISprite : public QWidget
{
    Q_OBJECT

public:
    explicit UISprite(ComponentSprite *csprite);
    ~UISprite();
    //texture
    void setTexture(const QString& name);
    QString getTexture();
    //cullface
    void setCullface(Easy2D::CullFace type);
    Easy2D::CullFace getCullface();
    //colors
    void setColor(const Easy2D::Color& color);
    Easy2D::Color getColor();
    //blend
    void setBlend(int blendsrc,int blenddst);
    int getBlendSrc();
    int getBlendDst();

public slots:

    //update frame color
    void updateFrameColor();
    void applayChange();

private:
    Ui::UISprite *ui;
    ComponentSprite* csprite;
};

#endif // UISPRITE_H
