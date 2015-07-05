#include <Easy2DInit.h>
#include <AudioAL.h>
#include <QDir>
#include <QFile>
#include <QDebug>


#if defined( PLATFORM_IOS )
#elif defined( PLATFORM_OSX )
#include <CocoaApp.h>
#elif defined( PLATFORM_WINDOW )
#include <WindowsApp.h>
#elif defined( PLATFORM_LINUX )
#include <LinuxApp.h>
#elif defined( PLATFORM_ANDROID )
#include <AndroidApp.h>
#elif defined( PLATFORM_EMSCRIPTEN )
#include <EmscriptenApp.h>
#endif

class QTScreen : Easy2D::Screen
{

public:

	QTScreen(QWidget* mainWidget=nullptr)
			 :mainWidget(mainWidget)
	{
		
	}
	void setQTWidget(QWidget* obj)
	{
		mainWidget=obj;
	}

    virtual ~QTScreen()
	{
	}
    /**
    * return screen width
    */
	virtual Easy2D::uint getWidth(){
		if(mainWidget)
			return (Easy2D::uint)mainWidget->width();
		else 
			return 0;
	}
    /**
    * return screen height
    */
    virtual Easy2D::uint getHeight()
	{	
		if(mainWidget)
			return (Easy2D::uint)mainWidget->height();
		else 
			return 0;
	}
    /**
    * return screen native width
    */
    virtual Easy2D::uint getNativeWidth()
	{
		return getWidth();
	}
    /**
    * return screen  native height
    */
    virtual Easy2D::uint getNativeHeight()
	{
		return getHeight();
	}
    /**
    * return screen orientation
    */
	virtual Orientation getOrientation()
	{
		return Orientation();
	}
    /**
    * set screen orientation
    */
    virtual void setOrientation(Orientation orientation)
    {
        //void
    }
    /**
    * return frame rate
    */
    virtual Easy2D::uint getFrameRate()
	{
		//todo
		return FRAMES;
	}

    /**
    * set the specified thread's current rendering context
    */
    virtual void acquireContext(){ /* NONE */ }
    /**
    * swap the buffers
    */
    virtual void swap(){ /* NONE */ }
    /**
    * show or hide mouse cursor
    * @param show, set true if you want show cursor otherwise false
    */
    virtual void setCursor(bool show=true){ /* NONE */ }
    /**
    * set position cursor
    */
    virtual void setPositionCursor(const Easy2D::Vec2& pos){ /* NONE */ }
    /**
    * return if cursor is shown or hidden
    */
    virtual bool getCursor(){ return false; }
    /**
    * enable or disable full screen
    */
    virtual void setFullscreen(bool fullscreen=true){ /* NONE */ }
    /**
    * return if fullscreen is enable return true
    */
    virtual bool isFullscreen(){ return false; }

protected:
	/**
    * create window
    */
    virtual void createWindow(const char* appname,
                              uint width,
                              uint height,
                              uint freamPerSecond,
                              bool fullscreen,
                              TypeBuffers type=COLOR32DEPTH24STENCIL8,
                              Screen::AntiAliasing dfAA=NOAA)
	{ /* NONE */ }

    /**
    * close window
    */
    virtual void closeWindow(){ /* NONE */ }

	//values
	QWidget* mainWidget;

};

class QTApplication : Easy2D::Application
{

public:

	QTApplication(QWidget* mainWidget=nullptr)
				 :mainWidget(mainWidget)
    {
        screen=(Easy2D::Screen*)new QTScreen(mainWidget);
        audio=(Easy2D::Audio*)new Easy2D::AudioAL();
		lastDeltaTime=DELTATIME;
	}
	void setQTWidget(QWidget* obj)
	{
		mainWidget=obj;
		((QTScreen*)screen)->setQTWidget(obj);
	}

	/**
    * destroy an window application
    */
	virtual ~QTApplication()
    {
        delete screen;
        delete audio;
	}
    /**
    * load a binary file
    * @return succes
    */
	virtual bool loadData(const Easy2D::String& path,void*& ptr,size_t &len)
	{
	    //open
		FILE *pfile=fopen(path,"rb");
		QByteArray output(("load file: "+path).c_str());
		qDebug(output);
		Q_ASSERT_X(pfile,"Load file",QByteArray(("error load file: "+path).c_str()));
		//get size
		fseek(pfile,0,SEEK_END);
		len=ftell(pfile);
		fseek(pfile,0,SEEK_SET);
		//read
        ptr=Easy2D::malloc(len*sizeof(char)+1);
		fread(ptr,len,1,pfile);
		(*((char*)ptr+len))='\0';
		//close
		fclose(pfile);
		return pfile!=NULL;
	}
    /**
    * where you can save files data
    * @return path
    */
    virtual Easy2D::String appDataDirectory(){	return appWorkingDirectory(); }
    /**
    * application root (read only)
    * @return path
    */
    virtual Easy2D::String appWorkingDirectory()
	{
		return QDir::currentPath().toUtf8().data();
	}
    /**
    * resources directory (read only)
    * @return path
    */
    virtual Easy2D::String appResourcesDirectory(){	return appWorkingDirectory(); }
    /**
    * application exit method
    */
    virtual void exit(){ /* NONE */ }
    /**
    * application loop
    */
    virtual void loop(){ /* NONE */ }
    /**
    * execute a instance application
    */
    virtual void exec(Easy2D::Game *game){ /* NONE */ }
    /**
    * application update
    */
    virtual void update(float dt){ /* NONE */ }
    /**
    * return true if device supports only power of two texture
    */
    virtual bool onlyPO2(){ return false; }

protected:

	QWidget* mainWidget;

};

void Easy2DInit::initApplication()
{
	Easy2D::Application::create("Easy2DEditor",(Easy2D::Application*)new QTApplication());
}
void Easy2DInit::initOpenGL()
{
    #if defined( PLATFORM_WINDOW  )|| defined( PLATFORM_LINUX )
        Easy2D::initOpenGL2();
    #endif
    //enable default state
    Easy2D::RenderContext::initContext();
}
void Easy2DInit::releaseOpenGL()
{
    //enable default state
    Easy2D::RenderContext::releaseContext();
}
void Easy2DInit::setQTWidget(QWidget* obj)
{
	((QTApplication*)Easy2D::Application::instance())->setQTWidget(obj);
}

void Easy2DInit::saveTable(const QString& path,const Easy2D::Table& table)
{
	QFile file(path);
	if(file.open(QIODevice::WriteOnly))
	{
		file.write(table.serialize().c_str());
		file.close();
	}
}
void Easy2DInit::readTable(const QString& path,Easy2D::Table& table)
{
	QFile file(path);
	if(file.open(QIODevice::ReadOnly))
	{
		Easy2D::String readall(file.readAll().data());
		table.deserialize(readall);
		file.close();
	}
}
