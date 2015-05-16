#ifndef UIEMITTER_H
#define UIEMITTER_H
#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <Easy2D.h>

namespace Ui
{
class UIEmitter;
}

class ComponentEmitter;
class UIEmitter : public QWidget
{
    Q_OBJECT

public:

    explicit UIEmitter(ComponentEmitter *cemitter);
    ~UIEmitter();

    //update ui
    void updateControllUI(const Easy2D::Emitter& emitter);
    void updateUI(const Easy2D::Emitter& emitter);
    void updateComponentGlobals(Easy2D::Emitter& emitter);
    void updateComponent(Easy2D::Emitter& emitter);


private:

    //utilities
    void setCBoxTF(QComboBox* cb,bool value);
    bool getCBoxTF(QComboBox* cb);
    void setSBoxColor(QSpinBox* r,
                      QSpinBox* g,
                      QSpinBox* b,
                      QSpinBox* a,
                      const Easy2D::Color& color);
    Easy2D::Color getSBoxColor(const QSpinBox* r,
                               const QSpinBox* g,
                               const QSpinBox* b,
                               const QSpinBox* a);

    void setLEditVec2(QLineEdit* x,
                      QLineEdit* y,
                      const Easy2D::Vec2& v);
    Easy2D::Vec2 getLEditVec2(QLineEdit* x,
                              QLineEdit* y);

    Ui::UIEmitter *ui;
    ComponentEmitter* cemitter;

};

#endif // UIEMITTER_H
