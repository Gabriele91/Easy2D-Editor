#ifndef PLUGINBODY_H
#define PLUGINBODY_H
#include <UIBody.h>
#include <ComponentsInterface.h>
#include <Editor.h>
#include <SelectStringDialog.h>
#include <WidgetScene.h>
#include <QWheelEvent>
#include <QDebug>


class ComponentBody : public ComponentsInterface
{


    inline bool collisionLine(const Easy2D::Vec2& start,
                              const Easy2D::Vec2& end,
                              const Easy2D::Vec2& center,
                              float r);
public:
    //name
    virtual QString getName();
    //get widget ui
    virtual QWidget* getWidget();
    //build ui
    virtual QWidget* buildGUI(QWidget*);
    //on load dll
    virtual void onCreate();
    //get body
    Easy2D::Body* getBody();

    //on run
    virtual void onRun(Easy2D::Object* obj,float dt);

    //on draw ui
    virtual void onDrawGui();

    //destroy dll
    virtual void onDestoy();

    //uibody change
    void applayChange();

    //void applay vertex slected
    void applayVertex();

    //update corrent shape vertexs
    void updateVertexs();

    //add a shape
    void addShape();

    //enable edit mode
    void enabeEditMode(Easy2D::Shape shape);

    //disable edit mode
    void disableEditMode();

    //is editable
    inline bool isEditable(const Easy2D::Body* body) const;

    //delete selected shape
    void deleteCurrentShape();

    //input event
    void onWheelEvent(QWheelEvent* event);

    //edit shape
    void selectOrAddAPoint(const Easy2D::Vec2& pikCam);
    void deleteAPoint(const Easy2D::Vec2& pikCam);

    //input managment
    void onMousePressEvent(QMouseEvent* event);
    void onMouseMoveEvent(QMouseEvent* event);
    void onMouseReleaseEvent(QMouseEvent* event);

    //get matrix
    Easy2D::Mat4 getRTMatrix();

    //draw
    void drawBodyCircle(const Easy2D::Body* body,
                        Easy2D::Shape index,
                        const Easy2D::Color& lineColor=Easy2D::Color(0,255,0,128),
                        const Easy2D::Color& pointColor=Easy2D::Color(0,255,0,255));
    void drawBodyPolygon(const Easy2D::Body* body,
                         Easy2D::Shape index,
                         const Easy2D::Color& lineColor=Easy2D::Color(0,255,0,128),
                         const Easy2D::Color& pointColor=Easy2D::Color(0,255,0,255));
    void drawBodyChain(const Easy2D::Body* body,
                       Easy2D::Shape index,
                       const Easy2D::Color& lineColor=Easy2D::Color(0,255,0,128),
                       const Easy2D::Color& pointColor=Easy2D::Color(0,255,0,255));

    //utility
    inline void drawCircle(const Easy2D::Vec2& pos,
                           float radius,
                           const Easy2D::Color& lineColor=Easy2D::Color(0,255,0,128),
                           const Easy2D::Color& pointColor=Easy2D::Color(0,255,0,255));
    inline void drawPolygon(std::vector<Easy2D::Vec2> points,
                            bool close=true,
                            const Easy2D::Color& lineColor=Easy2D::Color(0,255,0,128),
                            const Easy2D::Color& pointColor=Easy2D::Color(0,255,0,255));
    inline void drawChain(std::vector<Easy2D::Vec2> points
                         ,bool bStart
                         ,const Easy2D::Vec2& start
                         ,bool bEnd
                         ,const Easy2D::Vec2& end
                         ,const Easy2D::Color& lineColor=Easy2D::Color(0,255,0,128)
                         ,const Easy2D::Color& pointColor=Easy2D::Color(0,255,0,255));
private:

    bool applay;
    bool initUi;
    UIBody* uibody;

    //edit state
    struct EditShape
    {
        //////////////////////////////////////
        //shape info
        bool isEditable;
        Easy2D::Shape shape;
        //point
        int pointSelect;
        bool radiusSelect;
        //shape info
        bool hasPrev;
        bool hasNext;
        Easy2D::Vec2 prev,next;
        std::vector<Easy2D::Vec2> points;
        //////////////////////////////////////
        EditShape()
        {
            isEditable=false;
            shape=-1;
            pointSelect=-1;
            radiusSelect=false;
            hasPrev=false;
            hasNext=false;
        }
    }
    editShape;

};

#endif // PLUGINPARALLAX_H
