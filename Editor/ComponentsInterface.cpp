#include <Editor.h>
#include <ComponentsInterface.h>
#include <SelectStringDialog.h>

//get last object
Easy2D::Object* ComponentsInterface::getLastObject()
{
    return lastObject;
}
Editor* ComponentsInterface::getEditor()
{
    return editor;
}
WidgetScene* ComponentsInterface::getScene()
{
    if(editor) return editor->getWidgetScene();
    return nullptr;
}
//ops
void ComponentsInterface::lock()
{
    editor->lock();
}
void ComponentsInterface::unlock()
{
    editor->unlock();
}
QGLContext* ComponentsInterface::context()
{
    return Editor::context();
}
//dialogs
QString ComponentsInterface::openDialogStringList(const QStringList& list,
                                                  const QString& sdefault)
{
    SelectStringDialog dialog(list,sdefault);
    return dialog.doExec();
}
//resources
QString ComponentsInterface::resources(const QString& defaults,
                                       const QString& name)
{
    return editor->selectResource(defaults,name);
}
QString ComponentsInterface::resources(const QString& defaults,
                                       const QString& name,
                                       const QStringList& filters)
{
    return editor->selectResource(defaults,name,filters);
}
Easy2D::ResourcesGroup* ComponentsInterface::resources()
{
    return editor->getResourcesGroup();
}
