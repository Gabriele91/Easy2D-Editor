
#CPP FLAGS
CONFIG += c++11
QMAKE_CXXFLAGS+=-DDISABLE_SIMD\
                -DORDERED_TABLE\
                -DUSE_LUA

#-DDISABLE_SIMD -> I don't want overload new and delete
#-DNEW_DELETE_OVERLOADABLE -> shared lib not used...
CONFIG(debug, debug|release){
    QMAKE_CXXFLAGS += -D_DEBUG
}

#includes
INCLUDEPATH+= $$EASY2D_PATH/Easy2D/
INCLUDEPATH+= $$EASY2D_PATH/Easy2D/include/
INCLUDEPATH+= $$EASY2D_PATH/Easy2D/include/audio/
INCLUDEPATH+= $$EASY2D_PATH/Easy2D/include/gui/
INCLUDEPATH+= $$EASY2D_PATH/Easy2D/include/script/
INCLUDEPATH+= $$EASY2D_PATH/Easy2D/include/thread/

#only window
win32 {

    #define window
    QMAKE_CXXFLAGS += -D_CRT_NONSTDC_NO_WARNINGS -D_MBCS
    #includes
    INCLUDEPATH+= $$EASY2D_PATH/Easy2D/include/window/
    INCLUDEPATH+= $$EASY2D_PATH/Easy2D/dependencies-win32/include/
    #links
    CONFIG(debug, debug|release){
        LIBS+= $$EASY2D_PATH/Easy2D/lib/Easy2D_EDITOR_D.lib
        LIBS+= $$EASY2D_PATH/Easy2D/dependencies-win32/lib/Box2D_D.lib
        LIBS+= $$EASY2D_PATH/Easy2D/dependencies-win32/lib/freetype250_D.lib
    }
    CONFIG(release, debug|release){
        LIBS+= $$EASY2D_PATH/Easy2D/lib/Easy2D.lib
        LIBS+= $$EASY2D_PATH/Easy2D/dependencies-win32/lib/Box2D.lib
        LIBS+= $$EASY2D_PATH/Easy2D/dependencies-win32/lib/freetype250.lib
    }
    LIBS+= $$EASY2D_PATH/Easy2D/dependencies-win32/lib/lua51.lib
    LIBS+= $$EASY2D_PATH/Easy2D/dependencies-win32/lib/OpenAL32.lib
    LIBS+= Shlwapi.lib
    LIBS+= ShFolder.lib
}
#only mac os x
macx {
    #includes
    INCLUDEPATH+= $$EASY2D_PATH/Easy2D/include/cocoa/
    INCLUDEPATH+= $$EASY2D_PATH/Easy2D/dependencies-osx64/include/
    #LIBS
    LIBS+= -L$$EASY2D_PATH/Easy2D/build/Debug/ -lEasy2DEditor
    LIBS+= -L$$EASY2D_PATH/Easy2D/dependencies-osx64/lib/ -lBox2D
    LIBS+= -L$$EASY2D_PATH/Easy2D/dependencies-osx64/lib/ -lfreetype
   #LIBS+= -L$$EASY2D_PATH/Easy2D/dependencies-osx64/lib/ -llua
    LIBS+= -L$$EASY2D_PATH/Easy2D/dependencies-osx64/lib/ -lclua
    #fameworks
    LIBS += -framework Cocoa
    LIBS += -framework AppKit
    LIBS += -framework OpenAL
    LIBS += -lz
    LIBS += -lbz2
}
#only linux
linux-g++ {
    #includes
    INCLUDEPATH+= $$PWD/../../Easy2D/include/linux/
    INCLUDEPATH+= $$PWD/../../Easy2D/dependencies-linux/include/
    #LIBS
    LIBS+= -L$$EASY2D_PATH/Easy2D/build/Debug/ -lEasy2D
    LIBS+= -L$$EASY2D_PATH/Easy2D/dependencies-linux/lib/ -lBox2D
    LIBS+= -L$$EASY2D_PATH/Easy2D/dependencies-linux/lib/ -lfreetype
    LIBS+= -L$$EASY2D_PATH/Easy2D/dependencies-linux/lib/ -llua
    LIBS+= -L$$EASY2D_PATH/Easy2D/dependencies-linux/lib/ -lopenal
}
