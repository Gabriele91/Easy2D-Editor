#include <TreeScene.h>
#include <Debug.h>
#include <RenameObjectDialog.h>

TreeScene::TreeScene (QObject * parent)
  : QAbstractItemModel (parent)
  , myTreeView (nullptr)
{
	setObjectName("Scene Tree");
	rootItem = new Easy2D::Object("Root");
	sceneItem = new Easy2D::Object("Scene");
	//append scene to root
	rootItem->addChild(sceneItem);
}
void TreeScene::safeFreeRoot()
{
    if(rootItem)
    {
        delete rootItem;
        rootItem=nullptr;
    }
}

TreeScene::~ TreeScene ()
{
    safeFreeRoot();
}

/* MY METHODS */
void TreeScene::setReourceGroup (Easy2D::ResourcesGroup & rsource)
{
	//scene table
    changeSceneTable(*rsource.load<Easy2D::Table>("scene"));
}
void TreeScene::changeSceneTable(const Easy2D::Table& tbscebe)
{
    //delete items
    beginRemoveRows(QModelIndex(),0,0);
    //delete scene
    rootItem->eraseChild(sceneItem);
    delete sceneItem;
    //reset ui
    endRemoveRows();
    //new items
    beginInsertRows(QModelIndex(),0,0);
    //new scene
    sceneItem = new Easy2D::Object("Scene");
    sceneItem->setName("Scene");
    //have a objects?
    if( tbscebe.existsAsType("Objects",Easy2D::Table::TABLE) )
    {
        //get objects
        const Easy2D::Table& objects=tbscebe.getConstTable("Objects");
        //deserialize
        for(auto obj:objects)
        {
            if(obj.first.isString() &&
                obj.second->asType(Easy2D::Table::TABLE))
            {
                auto newobj=new Easy2D::Object();
                newobj->deserialize(obj.second->get<Easy2D::Table>());
                sceneItem->addChild(newobj);
            }
        }
    }
    //scene to root
    rootItem->addChild(sceneItem);
    //reset ui
    endInsertRows();
    //expand
    if(myTreeView)
        myTreeView->expand(createIndex(0,0,sceneItem));
}
QString TreeScene::currentName ()
{
    //get selected
    QModelIndex iname=myTreeView->currentIndex();
    //
    if(!iname.isValid())
        return "";
    //get object
    auto nameItem = static_cast<Easy2D::Object*>(iname.internalPointer());
    //is a object?
    //no root or scene
    if(!nameItem || nameItem==sceneItem || nameItem==rootItem)
        return "";
    //name
    return 	QString::fromUtf8(nameItem->getName());
}
Easy2D::Object* TreeScene::currentObject ()
{
    //get selected
    QModelIndex iname=myTreeView->currentIndex();
    //
    if(!iname.isValid())
        return nullptr;
    //get object
    auto nameItem = static_cast<Easy2D::Object*>(iname.internalPointer());
    //is a object?
    //no root or scene
    if(!nameItem || nameItem==sceneItem || nameItem==rootItem)
        return nullptr;
    //name
    return 	nameItem;
}
bool TreeScene::selectObject(Easy2D::Object* obj)
{
    if(!obj) return false;
    if(!obj->getParent()) return false;
    ///////////////////////////////////////////////////////
    size_t i_row = 0;
    for (auto achild : *obj->getParent())
    {
        if (achild == obj)
        {
            break;
        }
        ++i_row;
    }
    myTreeView->setCurrentIndex(createIndex(i_row,0,obj));
    ///////////////////////////////////////////////////////
    return true;
}
bool TreeScene::pushObject (Easy2D::String const & name)
{
	if(name.size()==0) return false;
	//create a canocical name
	Easy2D::String newname(name);
	newname.replaceAll(" ","_");
	//where? 
	QModelIndex pusin=createIndex(0, 0, sceneItem);
	if(myTreeView) 
	{
		QModelIndex selected=myTreeView->currentIndex();
		if(selected.isValid())
			pusin=selected;
	}
	//get object
	auto pusinItem = static_cast<Easy2D::Object*>(pusin.internalPointer());
	//seach a equals names
	for(auto achild:*pusinItem)
	{
		if(achild->getName()==newname)
			return false;
	}
	//I can add a new object
	beginInsertRows(parent(pusin),0,0);
	pusinItem->addChild(new Easy2D::Object(newname));
	endInsertRows();
	//expand 
	if(myTreeView)
		myTreeView->expand(pusin);
	//return true
	return true;
}
bool TreeScene::renameObject (Easy2D::String const & name)
{
	if(name.size()==0) return false;
	//create a canocical name
	Easy2D::String newname(name);
	newname.replaceAll(" ","_");
	//where? 
	if(!myTreeView) return false;
	QModelIndex rename=myTreeView->currentIndex();
	if(!rename.isValid()) return false;
	//get object
	auto renameItem = static_cast<Easy2D::Object*>(rename.internalPointer());
	//no root or scene
	if(!renameItem || renameItem==sceneItem || renameItem==rootItem) 
		return false;
	//seach a equals names
	for(auto achild:*(renameItem->getParent()))
	{
		if(achild->getName()==newname)
			return false;
	}
	//change name
	renameItem->setName(newname);
	//return true
	return true;
}
bool TreeScene::deleteObject ()
{
	//where? 
	QModelIndex goout=myTreeView->currentIndex();
	//get object
	Easy2D::Object* gooutItem = static_cast<Easy2D::Object*>(goout.internalPointer());
	//if is not a scene node or root node
	if(!gooutItem || gooutItem==sceneItem || gooutItem==rootItem) 
		return false;
	//update
	beginRemoveRows(parent(goout),goout.row(),goout.row());
	//dt
	gooutItem->getParent()->eraseChild(gooutItem);
	//ui update
	endRemoveRows();
	//delete
	delete gooutItem;
	//return true
	return true;
}
void TreeScene::setTreeView (QTreeView * const tree)
{
	myTreeView=tree;
}
Easy2D::Object* TreeScene::getSceneNode ()
{
	return sceneItem;
}
Easy2D::Object* TreeScene::getRootNode ()
{
	return rootItem;
}
Easy2D::Object* TreeScene::getObjectSelected()
{
	//where? 
	QModelIndex goout=myTreeView->currentIndex();
	//get object
	Easy2D::Object* gooutItem = static_cast<Easy2D::Object*>(goout.internalPointer());
	//if is not a scene node or root node
	if(!gooutItem || gooutItem==sceneItem || gooutItem==rootItem) 
		return nullptr;
	//return pointer
	return gooutItem;
}
void TreeScene::reset()
{
	//fake remove first element of root (aka scene)
	beginRemoveRows(createIndex(0, 0,rootItem),0,0);
	//update ui
	endRemoveRows();
}
void TreeScene::serialize(Easy2D::Table &outtable)
{
    for(Easy2D::Object* child: *sceneItem)
    {
        child->serialize(outtable);
    }
}

/* STANDARD METHOS */
Qt::DropActions TreeScene::supportedDropActions () const
{
	return  Qt::MoveAction;
}
bool TreeScene::removeRows (int row, int count, QModelIndex const & parent)
{
	return true;
}
bool TreeScene::insertRows (int row, int count, QModelIndex const & parent)
{
	if(!parent.isValid() || !myTreeView)
		return false;
		
	QModelIndex selected=myTreeView->currentIndex();
	if(!selected.isValid())
		return false;
		
	Easy2D::Object *parentItem = nullptr;
	Easy2D::Object *selectedItem = nullptr;		
	parentItem = static_cast<Easy2D::Object*>(parent.internalPointer());
	selectedItem = static_cast<Easy2D::Object*>(selected.internalPointer());

	if(!parentItem||!selectedItem)
		return false;

    //the new parent already contain a object with this name?
    if(parentItem->getChild(selectedItem->getName()))
    {
        RenameObjectDialog rename;
        if(rename.exec(selectedItem)==QDialog::Rejected)
        {
            return false;
        }
    }

    //save global values
    auto position=selectedItem->getPosition(true);
    auto rotation=selectedItem->getRotation(true);
    auto scale=selectedItem->getScale(true);
    auto parentMode=selectedItem->getParentMode();

    //remove item
    beginRemoveRows(TreeScene::parent(selected), selected.row(), selected.row());
    selectedItem->getParent()->eraseChild(selectedItem);
    endRemoveRows();

	//add item
	beginInsertRows(parent, row, row);
    parentItem->addChild(selectedItem,parentMode);
	endInsertRows();

    //restore values
    selectedItem->setPosition(position,true);
    selectedItem->setRotation(rotation,true);
    selectedItem->setScale(scale,true);

	//expand where is added
	myTreeView->expand(parent);

    return true;
}
QVariant TreeScene::data (QModelIndex const & index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();
		 
	//object
	Easy2D::Object *item = nullptr;
	item = static_cast<Easy2D::Object*>(index.internalPointer());

	return item ? QVariant(item->getName()) : QVariant();
}
Qt::ItemFlags TreeScene::flags (QModelIndex const & index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	//can't change root and scene
	auto indexItem = static_cast<Easy2D::Object*>(index.internalPointer());
	if (indexItem==rootItem)
		return Qt::ItemIsEnabled;
	if(indexItem==sceneItem)
		return Qt::ItemIsEnabled | Qt::ItemIsDropEnabled;
	//others items
	return Qt::ItemIsSelectable | 
			Qt::ItemIsEnabled | 
			Qt::ItemIsDragEnabled |
			Qt::ItemIsDropEnabled;
}
QModelIndex TreeScene::index (int row, int column, QModelIndex const & parent) const
{
	//no obj return a void index
	if (!hasIndex(row, column, parent))
		return QModelIndex();
	//object
	Easy2D::Object *parentItem = rootItem;
	//get parent
	if (parent.isValid())
		parentItem = static_cast<Easy2D::Object*>(parent.internalPointer());
	//get child
	Easy2D::Object *childItem = nullptr;
	if(parentItem)
	{
		size_t i = 0;
		for (auto achild : *parentItem)
		{
			if ((i++) == row)
			{
				childItem = achild;
				break;
			}
			
		}
	}
	//
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}
QModelIndex TreeScene::parent (QModelIndex const & index) const
{
	if (!index.isValid())
		return QModelIndex();

	Easy2D::Object *childItem = static_cast<Easy2D::Object*>(index.internalPointer());
	Easy2D::Object *parentItem = childItem->getParent();

	if (!childItem || !parentItem)
		return QModelIndex();

	if (parentItem == rootItem)
		return QModelIndex();
	//get id parent
	size_t i_row = 0;
	for (auto achild : *parentItem->getParent())
	{
		if (achild == parentItem)
		{
			break;
		}
		++i_row;
	}

	return createIndex(i_row, 0, parentItem);
}
int TreeScene::rowCount (QModelIndex const & parent) const
{
	Easy2D::Object* parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<Easy2D::Object*>(parent.internalPointer());

	return (int)parentItem->countChilds();
}
int TreeScene::columnCount (QModelIndex const & parent) const
{
	return 1;
}
