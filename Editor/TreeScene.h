#ifndef TREESCENE_H
#define TREESCENE_H

#include <QAbstractItemModel.h>
#include <QTreeView.h>
#include <Easy2D.h>

class TreeScene : public QAbstractItemModel
{
	Q_OBJECT

public:

	TreeScene(QObject *parent = 0);
    void safeFreeRoot();
	~TreeScene();

    /* MY METHODS */
    void setReourceGroup(Easy2D::ResourcesGroup& rsource);
    void changeSceneTable(const Easy2D::Table& sctable);
	QString currentName();
    Easy2D::Object* currentObject();
    bool selectObject(Easy2D::Object* obj);
	bool pushObject(const Easy2D::String& name);
	bool renameObject(const Easy2D::String& name);
	bool deleteObject();
    void setTreeView(QTreeView* const tree);
	Easy2D::Object* getSceneNode();
	Easy2D::Object* getRootNode();
    Easy2D::Object* getObjectSelected();
    void serialize(Easy2D::Table& outtable);
	void reset();

	/* STANDARD METHOS */
	Qt::DropActions supportedDropActions() const;
	
	bool removeRows(int row, int count, const QModelIndex &parent);
	bool insertRows(int row, int count, const QModelIndex &parent);

	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;

	QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;

private:

	QTreeView*		myTreeView;
	Easy2D::Object* rootItem;
	Easy2D::Object* sceneItem;

};

#endif
