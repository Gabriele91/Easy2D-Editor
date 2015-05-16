QT = core opengl gui


#path easy2d
EASY2D_PATH = $$PWD/../..
#easy2d configure
include(../LibEasy2D.pro)

INCLUDEPATH += ./
INCLUDEPATH += transform/
INCLUDEPATH += parallax/
INCLUDEPATH += sprite/
INCLUDEPATH += animatedSprite/
INCLUDEPATH += speaker/
INCLUDEPATH += emitter/
INCLUDEPATH += body/

release: DESTDIR = $$PWD/../release
debug:   DESTDIR = $$PWD/../debug
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

RESOURCES += \
    Editor.qrc

FORMS += \
    AboutDialog.ui \
    AddObjectDialog.ui \
    AddResourceDialog.ui \
    CreateSceneDialog.ui \
    Editor.ui \
    FirstDialog.ui \
    RenameObjectDialog.ui \
    ScelectResourceDialog.ui \
    SelectStringDialog.ui \
    transform/UITransform.ui \
    sprite/UISprite.ui \
    parallax/UIParallax.ui\
    emitter/UIEmitter.ui \
    body/UIBody.ui \
    animatedSprite/UIAnimatedSprite.ui \
    speaker/UISpeaker.ui

HEADERS += \
    AboutDialog.h \
    AddObjectDialog.h \
    AddResourceDialog.h \
    ComponentsTabs.h \
    CreateSceneDialog.h \
    Easy2DInit.h \
    Editor.h \
    FirstDialog.h \
    ListComponents.h \
    ManagerResourcesList.h \
    RenameObjectDialog.h \
    ResourcesGroupTabs.h \
    TreeScene.h \
    WidgetScene.h \
    ComponentsInterface.h \
    ComponentsManager.h \
    ProjectUtility.h \
    ScelectResourceDialog.h \
    SelectStringDialog.h \
    transform/PluginTransform.h \
    transform/UITransform.h \
    sprite/PluginSprite.h \
    sprite/UISprite.h \
    parallax/PluginParallax.h \
    parallax/UIParallax.h\
    emitter/PluginEmitter.h\
    emitter/UIEmitter.h \
    body/PluginBody.h \
    body/UIBody.h \
    animatedSprite/PluginAnimatedSprite.h \
    animatedSprite/UIAnimatedSprite.h \
    speaker/PluginSpeaker.h \
    speaker/UISpeaker.h

SOURCES += \
    Easy2DInit.cpp \
    Editor.cpp \
    main.cpp \
    ResourcesGroupTabs.cpp \
    TreeScene.cpp \
    WidgetScene.cpp \
    ComponentsManager.cpp \
    ScelectResourceDialog.cpp \
    SelectStringDialog.cpp \
    ComponentsTabs.cpp \
    transform/PluginTransform.cpp \
    transform/UITransform.cpp \
    sprite/PluginSprite.cpp \
    sprite/UISprite.cpp \
    parallax/PluginParallax.cpp \
    parallax/UIParallax.cpp \
    emitter/PluginEmitter.cpp \
    emitter/UIEmitter.cpp \
    ComponentsInterface.cpp \
    body/PluginBody.cpp \
    body/UIBody.cpp \
    animatedSprite/PluginAnimatedSprite.cpp \
    animatedSprite/UIAnimatedSprite.cpp \
    speaker/PluginSpeaker.cpp \
    speaker/UISpeaker.cpp

#mac os x assets
macx {
    #icon
    ICON = assets/images/e2d.icns
    #resources
    APP_RS_FILES.files =  assets/template/resources.rs.e2d
    APP_RS_FILES.path = Contents/MacOS/assets/template/
    QMAKE_BUNDLE_DATA += APP_RS_FILES
    #textures
    APP_TX_FILES.files = assets/template/textures/textures.e2d
    APP_TX_FILES.path = Contents/MacOS/assets/template/textures
    QMAKE_BUNDLE_DATA += APP_TX_FILES
    #meshes
    APP_ME_FILES.files = assets/template/meshes/meshes.e2d
    APP_ME_FILES.path = Contents/MacOS/assets/template/meshes
    QMAKE_BUNDLE_DATA += APP_ME_FILES
    #frameSets
    APP_FS_FILES.files = assets/template/frameSets/frameSets.e2d
    APP_FS_FILES.path = Contents/MacOS/assets/template/frameSets
    QMAKE_BUNDLE_DATA += APP_FS_FILES
    #fonts
    APP_FT_FILES.files = assets/template/fonts/fonts.e2d
    APP_FT_FILES.path = Contents/MacOS/assets/template/fonts
    QMAKE_BUNDLE_DATA += APP_FT_FILES
    #scripts
    APP_SC_FILES.files = assets/template/scripts/scripts.e2d
    APP_SC_FILES.path = Contents/MacOS/assets/template/scripts
    QMAKE_BUNDLE_DATA += APP_SC_FILES
    #sounds
    APP_SD_FILES.files = assets/template/sounds/sounds.e2d
    APP_SD_FILES.path = Contents/MacOS/assets/template/sounds
    QMAKE_BUNDLE_DATA += APP_SD_FILES
    #tables
    APP_TB_FILES.files = assets/template/tables/tables.e2d
    APP_TB_FILES.files+= assets/template/tables/scene.table.e2d
    APP_TB_FILES.path = Contents/MacOS/assets/template/tables
    QMAKE_BUNDLE_DATA += APP_TB_FILES
    #shaders
    APP_SA_FILES.files = assets/template/shaders/shaders.e2d
    APP_SA_FILES.path = Contents/MacOS/assets/template/shaders
    QMAKE_BUNDLE_DATA += APP_SA_FILES
}
win32 {
    #icon
    ICON = assets/images/e2d.icns
    #resources
    APP_RS_FILES.files =  assets/template/resources.rs.e2d
    APP_RS_FILES.path = $${DESTDIR}/assets/template/
    INSTALLS  += APP_RS_FILES
    #textures
    APP_TX_FILES.files = assets/template/textures/textures.e2d
    APP_TX_FILES.path = $${DESTDIR}/assets/template/textures
    INSTALLS  += APP_TX_FILES
    #meshes
    APP_ME_FILES.files = assets/template/meshes/meshes.e2d
    APP_ME_FILES.path = $${DESTDIR}/assets/template/meshes
    INSTALLS  += APP_ME_FILES
    #frameSets
    APP_FS_FILES.files = assets/template/frameSets/frameSets.e2d
    APP_FS_FILES.path = $${DESTDIR}/assets/template/frameSets
    INSTALLS  += APP_FS_FILES
    #fonts
    APP_FT_FILES.files = assets/template/fonts/fonts.e2d
    APP_FT_FILES.path = $${DESTDIR}/assets/template/fonts
    INSTALLS  += APP_FT_FILES
    #scripts
    APP_SC_FILES.files = assets/template/scripts/scripts.e2d
    APP_SC_FILES.path = $${DESTDIR}/assets/template/scripts
    INSTALLS  += APP_SC_FILES
    #sounds
    APP_SD_FILES.files = assets/template/sounds/sounds.e2d
    APP_SD_FILES.path = $${DESTDIR}/assets/template/sounds
    INSTALLS  += APP_SD_FILES
    #tables
    APP_TB_FILES.files = assets/template/tables/tables.e2d
    APP_TB_FILES.files+= assets/template/tables/scene.table.e2d
    APP_TB_FILES.path = $${DESTDIR}/assets/template/tables
    INSTALLS  += APP_TB_FILES
    #shaders
    APP_SA_FILES.files = assets/template/shaders/shaders.e2d
    APP_SA_FILES.path = $${DESTDIR}/assets/template/shaders
    INSTALLS  += APP_SA_FILES
}
