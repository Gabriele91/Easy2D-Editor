#ifndef WIDGETSCENE_H
#define WIDGETSCENE_H

#include <Easy2D.h>
#include <QGLWidget>
#include <QTimer>
#include <QString>
#include <TreeScene.h>
#include <functional>

///////////////////////
class Editor;
class ComponentsTabs;

class SceneComponentCallBack
{
  public:
    //object manage
    virtual Easy2D::Object* getSelectObject()=0;
    virtual void goToTarget()=0;
    //camera
    virtual Easy2D::Camera& getCamera()=0;
    virtual void setZoom(float)=0;
    //editor object mode
    virtual void editorObjectMode(bool eom)=0;
    virtual bool isInEditorObjectMode()=0;
    //GUI UI DRAW
    virtual void drawPoint2D(const Easy2D::Vec2& p,
                             const Easy2D::Color& colorE=Easy2D::Color(255,255,255,128),
                             const Easy2D::Color& colorI=Easy2D::Color(0,0,255,255))=0;

    virtual void drawLine(const Easy2D::Vec2& p1,
                          const Easy2D::Vec2& p2,
                          const Easy2D::Color& color=Easy2D::Color(255,255,255,128))=0;

    virtual void drawAABox(const Easy2D::AABox2& box,
                           const Easy2D::Color& color=Easy2D::Color(255,255,255,128))=0;

    virtual void drawCircle(const Easy2D::Vec2& p,float r,
                            const Easy2D::Color& color=Easy2D::Color(255,255,255,128))=0;

    virtual void drawPath(const std::vector<Easy2D::Vec2>& points,
                          bool close=false,
                          const Easy2D::Color& color=Easy2D::Color(255,255,255,128))=0;

    virtual void drawPath(const std::vector<Easy2D::Vec2>& points,
                          const Easy2D::Vec2& position,
                          bool close=false,
                          const Easy2D::Color& color=Easy2D::Color(255,255,255,128))=0;

    virtual void drawPathPoints(const std::vector<Easy2D::Vec2>& points,
                                bool close=false,
                                const Easy2D::Color& color=Easy2D::Color(255,255,255,128),
                                const Easy2D::Color& colorE=Easy2D::Color(255,255,255,128),
                                const Easy2D::Color& colorI=Easy2D::Color(0,0,255,255))=0;
    virtual void drawPathPoints(const std::vector<Easy2D::Vec2>& points,
                                const Easy2D::Vec2& position,
                                bool close=false,
                                const Easy2D::Color& color=Easy2D::Color(255,255,255,128),
                                const Easy2D::Color& colorE=Easy2D::Color(255,255,255,128),
                                const Easy2D::Color& colorI=Easy2D::Color(0,0,255,255))=0;


};

///////////////////////
class WidgetScene : public QGLWidget,
                    public Easy2D::Scene,
                    public SceneComponentCallBack
{

    Q_OBJECT

public:

    WidgetScene(QWidget *parent=0);
	~WidgetScene();

    //OpenGL loops
	void initializeGL();
	void paintGL();
    void resizeGL(int w, int h);
    //Objects managment
    void setSceneNode(Easy2D::Object* argscene);
    void setSelectObject(Easy2D::Object* target);
    void unselectObject();
    Easy2D::Object* getSelectObject();
    bool isInSelectedObject(Easy2D::Object* obj);
    void goToTarget();
    //camera
    Easy2D::Camera& getCamera()
    {
        return *camera;
    }
    void setZoom(float value);

	//events
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent* event);
    void paintEvent(QPaintEvent *event);

    //init editor
    void setEditor(Editor* argeditor);
    //onStart callback method
    void setOnStart(std::function<void(void)> callback)
    {
        cbOnStart=callback;
    }
    //onRun callback method
    void setOnRun(std::function<void(float)> callback)
    {
        cbOnRun=callback;
    }
    //onEnd callback method
    void setOnEnd(std::function<void(void)> callback)
    {
        cbOnEnd=callback;
    }
    //onEnd callback method
    void setOnDrawGui(std::function<void(void)> callback)
    {
        cbOnDrawGui=callback;
    }
    //input callbacks
    void setOnMousePress(std::function<void(QMouseEvent *event)> callback)
    {
        cbOnMousePress=callback;
    }
    void setOnMouseMove(std::function<void(QMouseEvent *event)> callback)
    {
        cbOnMouseMove=callback;
    }
    void setOnMouseRelease(std::function<void(QMouseEvent *event)> callback)
    {
        cbOnMouseRelease=callback;
    }
    void setOnMouseWell(std::function<void(QWheelEvent *event)> callback)
    {
        cbOnMouseWell=callback;
    }
    void setOnDoubleClick(std::function<void(QMouseEvent *event)> callback)
    {
        cbOnDoubleClick=callback;
    }

    //lock update
    void lock()
    {
        timer.stop();
    }
    //unlock update
    void unlock()
    {
        timer.start();
    }
    //callbacks
    void setOnEnterPlayMode(std::function<void(void)> callback)
    {
        cbOnEnterPlayMode=callback;
    }
    void setOnExitPlayMode(std::function<void(void)> callback)
    {
        cbOnExitPlayMode=callback;
    }

    //editor object mode
    void editorObjectMode(bool eom);
    bool isInEditorObjectMode();

    //GUI UI DRAW
     void drawPoint2D(const Easy2D::Vec2& p,
                      const Easy2D::Color& colorE=Easy2D::Color(255,255,255,128),
                      const Easy2D::Color& colorI=Easy2D::Color(0,0,255,255));

    void drawLine(const Easy2D::Vec2& p1,
                  const Easy2D::Vec2& p2,
                  const Easy2D::Color& color=Easy2D::Color(255,255,255,128));

    void drawAABox(const Easy2D::AABox2& box,
                   const Easy2D::Color& color=Easy2D::Color(255,255,255,128));

    void drawCircle(const Easy2D::Vec2& p,float r,
                    const Easy2D::Color& color=Easy2D::Color(255,255,255,128));

    void drawPath(const std::vector<Easy2D::Vec2>& points,
                  bool close=false,
                  const Easy2D::Color& color=Easy2D::Color(255,255,255,128));

    void drawPath(const std::vector<Easy2D::Vec2>& points,
                  const Easy2D::Vec2& position,
                  bool close=false,
                  const Easy2D::Color& color=Easy2D::Color(255,255,255,128));

    void drawPathPoints(const std::vector<Easy2D::Vec2>& points,
                        bool close=false,
                        const Easy2D::Color& color=Easy2D::Color(255,255,255,128),
                        const Easy2D::Color& colorE=Easy2D::Color(255,255,255,128),
                        const Easy2D::Color& colorI=Easy2D::Color(0,0,255,255));
    void drawPathPoints(const std::vector<Easy2D::Vec2>& points,
                        const Easy2D::Vec2& position,
                        bool close=false,
                        const Easy2D::Color& color=Easy2D::Color(255,255,255,128),
                        const Easy2D::Color& colorE=Easy2D::Color(255,255,255,128),
                        const Easy2D::Color& colorI=Easy2D::Color(0,0,255,255));

    void setClearColor(const Easy2D::Color& color);
    Easy2D::Color getClearColor();

public slots:

    //update OpenGL thread
    void updateThreadGL();

protected:

	enum State2DAction
	{
		OBJECT_UNSELECTED,
		OBJECT_SELECTED_TRANSLATE,
		OBJECT_SELECTED_ROTATION,
		OBJECT_SELECTED_SCALE,
        OBJECT_SELECTED_EDITOR
	};
    //editor state
    int lastEditorState;
	//scene states
	State2DAction state;
	//pos piked mouse
	Easy2D::Vec2 pikCam;
	//start rotation of object
    Easy2D::Angle pikRot;
	//start pos of object
	Easy2D::Vec2 pikPos;
	//start scale of object
	Easy2D::Vec2 pikScale;
    //fake clear color
    Easy2D::Color clearColor;
    //callback methods
    std::function<void(void)> cbOnStart;
    std::function<void(float)> cbOnRun;
    std::function<void(void)> cbOnEnd;
    std::function<void(void)> cbOnEnterPlayMode;
    std::function<void(void)> cbOnExitPlayMode;
    std::function<void(void)> cbOnDrawGui;
    //callback input methods
    std::function<void(QMouseEvent *event)> cbOnMousePress;
    std::function<void(QMouseEvent *event)> cbOnMouseMove;
    std::function<void(QMouseEvent *event)> cbOnMouseRelease;
    std::function<void(QWheelEvent *event)> cbOnMouseWell;
    std::function<void(QMouseEvent *event)> cbOnDoubleClick;
	//virtual methos
	virtual void onStart();
	virtual void onRun(float dt);
	virtual void onEnd();
	
    //private methods
    void drawEditorObjects();
    void updateParticles(Easy2D::Object* obj,float dt);
    bool getTargetAABox(Easy2D::AABox2 &box);
    bool getAABox(Easy2D::Object* obj,Easy2D::AABox2& box);
    bool getGlobalAABox(Easy2D::Object* obj,Easy2D::AABox2& box);

	//dev vars
	QTimer timer;
    Editor* editor;
	Easy2D::Camera* camera;
    Easy2D::Object* sceneNode;
	Easy2D::Object* target;
	Easy2D::Object* grid;
};



#endif
