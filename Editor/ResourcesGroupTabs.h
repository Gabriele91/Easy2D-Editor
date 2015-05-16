#ifndef RESOURCESGROUPTAB_H
#define RESOURCESGROUPTAB_H

#include <Easy2DInit.h>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <ManagerResourcesList.h>
 
class ResourcesGroupTabs
{
public:
	
	ResourcesGroupTabs(QListWidget* listTextures,
					   QListWidget* listMeshes,
					   QListWidget* listFrameSets,
					   QListWidget* listTables,
					   QListWidget* listFonts,
					   QListWidget* listSounds,
					   QListWidget* listScripts,
                       QListWidget* listShaders,
					   Easy2D::Table& rbRes)
					   :node(nullptr)
	{
		textues=new	  ManagerResourcesList(listTextures,&rbRes.getTable("textures"));
        meshes=new	  ManagerResourcesList(listMeshes,&rbRes.getTable("meshes"));
		frameSets=new ManagerResourcesList(listFrameSets,&rbRes.getTable("frameSets"));
		tables=new	  ManagerResourcesList(listTables,&rbRes.getTable("tables"));
		fonts=new	  ManagerResourcesList(listFonts,&rbRes.getTable("fonts"));
		sounds=new	  ManagerResourcesList(listSounds,&rbRes.getTable("sounds"));
        scripts=new	  ManagerResourcesList(listScripts,&rbRes.getTable("scripts"));
        shaders=new	  ManagerResourcesList(listShaders,&rbRes.getTable("shaders"));
	}
	~ResourcesGroupTabs()
	{
		delete textues;
        delete meshes;
		delete frameSets;
		delete tables;
		delete fonts;
		delete sounds;
		delete scripts;
        delete shaders;
	}
	
    void changeResources(Easy2D::Table& rbRes)
    {
        textues->changeTable(&rbRes.getTable("textures"));
        meshes->changeTable(&rbRes.getTable("meshes"));
        frameSets->changeTable(&rbRes.getTable("frameSets"));
        tables->changeTable(&rbRes.getTable("tables"));
        fonts->changeTable(&rbRes.getTable("fonts"));
        sounds->changeTable(&rbRes.getTable("sounds"));
        scripts->changeTable(&rbRes.getTable("scripts"));
        shaders->changeTable(&rbRes.getTable("shaders"));
    }

	void update();
	bool addTexture(const QString& name,const QString& path)
	{
		return addReource(name,path,textues);
	}
	bool addMesh(const QString& name,const QString& path)
	{
        return addReource(name,path,meshes);
	}
	bool addFrameSet(const QString& name,const QString& path)
	{
		return addReource(name,path,frameSets);
	}
	bool addTable(const QString& name,const QString& path)
	{
		return addReource(name,path,tables);
	}
	bool addFont(const QString& name,const QString& path)
	{
		return addReource(name,path,fonts);
	}
	bool addSound(const QString& name,const QString& path)
	{
		return addReource(name,path,sounds);
    }
    bool addScript(const QString& name,const QString& path)
    {
        return addReource(name,path,scripts);
    }
    bool addShader(const QString& name,const QString& path)
    {
        return addReource(name,path,shaders);
    }
	
	void setNodeTarget(Easy2D::Object* argNode)
	{
		node=argNode;
	}
	bool canDeleteTexture(const QString& name);
	bool canDeleteMesh(const QString& name);
	bool canDeleteFrameSet(const QString& name);
	bool canDeleteTable(const QString& name);
	bool canDeleteFont(const QString& name);
    bool canDeleteSound(const QString& name);
    bool canDeleteScript(const QString& name);
    bool canDeleteShader(const QString& name);
	
	void deleteTexture(const QString& name,bool deleteAllSubTree=true);
	void deleteMesh(const QString& name,bool deleteAllSubTree=true);
	void deleteFrameSet(const QString& name,bool deleteAllSubTree=true);
	void deleteTable(const QString& name,bool deleteAllSubTree=true);
	void deleteFont(const QString& name,bool deleteAllSubTree=true);
    void deleteSound(const QString& name,bool deleteAllSubTree=true);
    void deleteScript(const QString& name,bool deleteAllSubTree=true);
    void deleteShader(const QString& name,bool deleteAllSubTree=true);

private:
	
	void deleteObjectList(const QList<Easy2D::Object*>& objects,bool deleteAllSubTree);
	bool addReource(const QString& name,
					const QString& path,
					ManagerResourcesList* reources);
	void deleteElement(ManagerResourcesList* alist,const QString& name);

	Easy2D::Object* node;
	ManagerResourcesList* textues;
    ManagerResourcesList* meshes;
	ManagerResourcesList* frameSets;
	ManagerResourcesList* tables;
	ManagerResourcesList* fonts;
    ManagerResourcesList* sounds;
    ManagerResourcesList* scripts;
    ManagerResourcesList* shaders;

};

#endif
