#include <Editor.h>
#include <QtWidgets/QApplication>
#include <FirstDialog.h>
#include <Easy2DInit.h>
#include <QSettings>
#include <QFile>

int editorApplication(QApplication& a)
{
    //app data directory
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Easy2D", "Editor");
    settings.setValue("version","0.0.9");
    settings.sync();

#ifdef __APPLE__
    QFile file(":/darkorangeOSX.stylesheet");
#else
    QFile file(":/darkorangeOSX.stylesheet");
#endif

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        a.setStyleSheet(file.readAll());
        file.close();
    }

    //start up
    bool startUp=false;
    QString pathProject;
    QString location = QFileInfo(settings.fileName()).absolutePath();
    QString locationConf =location + "/config.e2d";

    //read
    if(QFile::exists(locationConf))
    {
        Easy2D::Table config;
        Easy2DInit::readTable(locationConf,config);
        startUp=config.getFloat("startUp")!=0.0;
        pathProject=config.getString("path").c_str();
        startUp=startUp && QFile::exists(pathProject);
    }

    //show dialog
    FirstDialog dialog;
    if(startUp || dialog.exec()==QDialog::Accepted)
    {
        if(!startUp)
        {
            startUp=dialog.startUp();
            pathProject=dialog.getProjectPath();
            Easy2D::Table config;
            config.set("startUp",(float)startUp);
            config.set("path",pathProject.toUtf8().data());
            Easy2DInit::saveTable(locationConf,config);
        }
        Editor editor(pathProject,locationConf);
        editor.show();
        return a.exec();
    }
    return 0;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Easy2DInit::initApplication();

    int returnCode=0;
    while((returnCode=editorApplication(a))==RESTART_APP_CODE);

    return returnCode;
}
