#ifndef UIPARALLAX_H
#define UIPARALLAX_H

#include <QWidget>
#include <Easy2D.h>

namespace Ui{ class UIParallax; };
class ComponentParallax;
class UIParallax : public QWidget
{
    Q_OBJECT

public:
    explicit UIParallax(ComponentParallax *cparallax = 0);
    ~UIParallax();

    void setParallax(const Easy2D::Vec2& pos);
    Easy2D::Vec2 getParallax();

private:
    Ui::UIParallax *ui;
};

#endif // UIPARALLAX_H
