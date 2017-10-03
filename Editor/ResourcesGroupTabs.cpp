#include <ResourcesGroupTabs.h>



bool ResourcesGroupTabs::addReource(const QString& name,
									const QString& path,
									ManagerResourcesList* reources)
{
	//musto to not exist
	if(!name.size() || reources->getTable().exists(name.toUtf8().data()))
		return false;
	//get directory resource
    auto directory=QString::fromUtf8(reources->getTable().getPath().getDirectory());
	//path file
	auto pathFile=Easy2D::Utility::Path(path.toUtf8().data());
    auto filename=QString::fromUtf8(pathFile.getFilebasename());
    auto fileext=QString::fromUtf8(pathFile.getLeftExtension());
    auto finalpath=directory+'/'+filename+'.'+fileext;
	//copy file
	size_t i=0;
	while(QFile::exists(finalpath)) 
	{
		finalpath=(directory+'/'+filename)+QString::number(i++)+('.'+fileext);
	}
	//copy
	QFile::copy(path,finalpath);
    //calc correct path from table
    Easy2D::Utility::Path e2dPath(finalpath.toUtf8().data());
    e2dPath=e2dPath.getRelativePathTo(reources->getTable().getPath());
	//add to resource file
    reources->getTable().set(name.toUtf8().data(),e2dPath);
	//update ui
	reources->update();
	//return
	return true;
}

void ResourcesGroupTabs::update()
{
	textues->update();
    meshes->update();
	frameSets->update();
	tables->update();
	fonts->update();
	sounds->update();
	scripts->update();
    shaders->update();
}

///////////////////////////////////////////////////////////////////////
void objectHasTextureName(Easy2D::Object* node,
						  const Easy2D::String& name,
						  QList<Easy2D::Object*>& hasTextue,
						  bool noSub=true)
{
	if(node->hasComponent<Easy2D::Renderable>())
	{
		auto rable=node->getComponent<Easy2D::Renderable>();
		if(rable->getTexture()->getName()==name)
		{
			hasTextue.push_front(node);
			if(noSub) return;
		}
	}

	for(auto child:*node)
	{
		objectHasTextureName(child,name,hasTextue);
	}
}
void objectHasMeshName(Easy2D::Object* node,
					   const Easy2D::String& name,
					   QList<Easy2D::Object*>& hasMesh,
					   bool noSub=true)
{
	if(node->hasComponent<Easy2D::Renderable>())
	{
		auto rable=node->getComponent<Easy2D::Renderable>();
		if(rable->getMesh()->getName()==name)
		{
			hasMesh.push_front(node);
			if(noSub) return;
		}
	}

	for(auto child:*node)
	{
		objectHasMeshName(child,name,hasMesh);
	}

}
void objectHasFrameSetName(Easy2D::Object* node,
					       const Easy2D::String& name,
					       QList<Easy2D::Object*>& hasFrameSet,
						   bool noSub=true)
{
	if(node->hasComponent<Easy2D::AnimatedSprite>())
	{
		auto asprite=node->getComponent<Easy2D::AnimatedSprite>();
		std::vector<Easy2D::FrameSet::ptr> frames;
		asprite->getFrameSets(frames);
		for(auto aframe:frames)
		{
			if(aframe->getName()==name)
			{
				hasFrameSet.push_front(node);
				if(noSub) return;
			}
		}
	}

	for(auto child:*node)
	{
		objectHasFrameSetName(child,name,hasFrameSet);
	}

}
void objectHasScriptName(Easy2D::Object* node,
                         const Easy2D::String& name,
                         QList<Easy2D::Object*>& hasScript,
                         bool noSub=true)
{
    if(node->hasComponent<Easy2D::LuaScript>())
    {
        auto script=node->getComponent<Easy2D::LuaScript>();
        for(size_t i=0;i!=script->countScripts();++i)
        {
            if(script->getScript(i)->getName()==name)
            {
                hasScript.push_front(node);
                if(noSub) return;
            }
        }
    }

    for(auto child:*node)
    {
        objectHasScriptName(child,name,hasScript);
    }

}
void objectHasShaderName(Easy2D::Object* node,
                         const Easy2D::String& name,
                         QList<Easy2D::Object*>& hasShader,
                         bool noSub=true)
 {
    if(node->hasComponent<Easy2D::Renderable>())
    {
        auto rable=node->getComponent<Easy2D::Renderable>();
        if(rable->getShader()->getName()==name)
        {
            hasShader.push_front(node);
            if(noSub) return;
        }
    }

    for(auto child:*node)
    {
        objectHasShaderName(child,name,hasShader);
    }
 }
///////////////////////////////////////////////////////////////////////

bool ResourcesGroupTabs::canDeleteTexture(const QString& qName)
{
	if(!node) return true;
	Easy2D::String name(qName.toUtf8().data());
	//get list of all object to delete
	QList<Easy2D::Object*> hasTextue;
	for(auto child:*node)
	{
		objectHasTextureName(child,name,hasTextue);
	}
	return !hasTextue.size();
}
bool ResourcesGroupTabs::canDeleteMesh(const QString& qName)
{
	if(!node) return true;
	Easy2D::String name(qName.toUtf8().data());
	//get list of all object to delete
	QList<Easy2D::Object*> hasMesh;
	for(auto child:*node)
	{
		objectHasMeshName(child,name,hasMesh);
	}
	return !hasMesh.size();
}
bool ResourcesGroupTabs::canDeleteFrameSet(const QString& qName)
{
	if(!node) return true;
	Easy2D::String name(qName.toUtf8().data());
	//get list of all object to delete
	QList<Easy2D::Object*> hasFrameSet;
	for(auto child:*node)
	{
		objectHasFrameSetName(child,name,hasFrameSet);
	}
	return !hasFrameSet.size();
}
bool ResourcesGroupTabs::canDeleteTable(const QString& name)
{
	return true;
}
bool ResourcesGroupTabs::canDeleteFont(const QString& name)
{
	return true;
}
bool ResourcesGroupTabs::canDeleteSound(const QString& name)
{
	return true;
}
bool ResourcesGroupTabs::canDeleteScript(const QString& qName)
{
    if(!node) return true;
    Easy2D::String name(qName.toUtf8().data());
    //get list of all object to delete
    QList<Easy2D::Object*> hasScript;
    for(auto child:*node)
    {
        objectHasScriptName(child,name,hasScript);
    }
    return !hasScript.size();
}
bool ResourcesGroupTabs::canDeleteShader(const QString& qName)
{
    if(!node) return true;
    Easy2D::String name(qName.toUtf8().data());
    //get list of all object to delete
    QList<Easy2D::Object*> hasShader;
    for(auto child:*node)
    {
        objectHasShaderName(child,name,hasShader);
    }
    return !hasShader.size();
}

void ResourcesGroupTabs::deleteTexture(const QString& qName,bool deleteAllSubTree)
{
	//to Easy2D string
	Easy2D::String name(qName.toUtf8().data());
	//get list of all object to delete
	QList<Easy2D::Object*> hasTextue;
	for(auto child:*node)
	{
		objectHasTextureName(child,name,hasTextue,deleteAllSubTree);
	}
	//delete objects
	if(hasTextue.size())
	{
		deleteObjectList(hasTextue,deleteAllSubTree);
	}
	//delete from table
	textues->getTable().deleteElement(name);
	//update ui
	textues->update();
}
void ResourcesGroupTabs::deleteMesh(const QString& qName,bool deleteAllSubTree)
{
	//to Easy2D string
	Easy2D::String name(qName.toUtf8().data());
	//get list of all object to delete
	QList<Easy2D::Object*> hasMesh;
	for(auto child:*node)
	{
		objectHasMeshName(child,name,hasMesh,deleteAllSubTree);
	}
	//delete objects
	if(hasMesh.size())
	{
		deleteObjectList(hasMesh,deleteAllSubTree);
	}
	//delete from table
    meshes->getTable().deleteElement(name);
	//update ui
    meshes->update();
}
void ResourcesGroupTabs::deleteFrameSet(const QString& qName,bool deleteAllSubTree)
{
	//to Easy2D string
	Easy2D::String name(qName.toUtf8().data());
	//get list of all object to delete
	QList<Easy2D::Object*> hasFrameSet;
	for(auto child:*node)
	{
		objectHasFrameSetName(child,name,hasFrameSet,deleteAllSubTree);
	}
	//delete objects
	if(hasFrameSet.size())
	{
		deleteObjectList(hasFrameSet,deleteAllSubTree);
	}
	//delete from table
	frameSets->getTable().deleteElement(name);
	//update ui
	frameSets->update();
}
void ResourcesGroupTabs::deleteScript(const QString& qName,bool deleteAllSubTree)
{
	//to Easy2D string
	Easy2D::String name(qName.toUtf8().data());
	//get list of all object to delete
	QList<Easy2D::Object*> hasFrameSet;
	for(auto child:*node)
	{
		objectHasScriptName(child,name,hasFrameSet,deleteAllSubTree);
	}
	//delete objects
	if(hasFrameSet.size())
	{
		deleteObjectList(hasFrameSet,deleteAllSubTree);
	}
	//delete from table
	scripts->getTable().deleteElement(name);
	//update ui
	scripts->update();
}
void ResourcesGroupTabs::deleteShader(const QString& qName,bool deleteAllSubTree)
{
    //to Easy2D string
    Easy2D::String name(qName.toUtf8().data());
    //get list of all object to delete
    QList<Easy2D::Object*> hasShader;
    for(auto child:*node)
    {
        objectHasShaderName(child,name,hasShader,deleteAllSubTree);
    }
    //delete objects
    if(hasShader.size())
    {
        deleteObjectList(hasShader,deleteAllSubTree);
    }
    //delete from table
    shaders->getTable().deleteElement(name);
    //update ui
    shaders->update();
}

void ResourcesGroupTabs::deleteTable(const QString& name,bool deleteAllSubTree)
{
	deleteElement(tables,name);
}
void ResourcesGroupTabs::deleteFont(const QString& name,bool deleteAllSubTree)
{
	deleteElement(fonts,name);
}
void ResourcesGroupTabs::deleteSound(const QString& name,bool deleteAllSubTree)
{
	deleteElement(sounds,name);
}


void ResourcesGroupTabs::deleteElement(ManagerResourcesList* alist,const QString& qName)
{
	//to Easy2D string
	Easy2D::String name(qName.toUtf8().data());
	//delete from table
	alist->getTable().deleteElement(name);
	//update ui
	alist->update();

}
void ResourcesGroupTabs::deleteObjectList(const QList<Easy2D::Object*>& objects,bool deleteAllSubTree)
{
	for(auto obj:objects)
	{	
		if(obj->getParent() && !deleteAllSubTree)
		{
			auto parent=obj->getParent();
			//dt parent
			obj->getParent()->eraseChild(obj);
			//rename values
			Easy2D::String newName;
			int iname=0;
			//create a list and rename
			QList<Easy2D::Object*> listChild;
			for(auto child:*obj){
				listChild.push_back(child);
				//rename object
				newName=child->getName();
				iname=0;
				//rename
				while(parent->hasChild(newName))
				{
					newName=child->getName()+iname;
				}
				//set name
				child->setName(newName);
			}
			//now can pop all childs
			for(auto child:listChild) parent->addChild(child);
		}
		else if(obj->getParent())
		{
			obj->getParent()->eraseChild(obj);
		}

		delete obj;
	}
}


