#ifndef EASY2DINIT_H
#define EASY2DINIT_H

#include <Easy2D.h>
#include <QWidget>

#define FRAMES 60.0f
#define DELTATIME (1.0f/FRAMES)

class Easy2DInit
{
public:
    static void initApplication();
    static void initOpenGL();
    static void releaseOpenGL();
	static void setQTWidget(QWidget* obj);
	static void saveTable(const QString& path,const Easy2D::Table& table);
	static void readTable(const QString& path,Easy2D::Table& table);
};

#endif
