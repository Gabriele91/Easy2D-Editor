#ifndef LISTCOMPONENTS_H
#define LISTCOMPONENTS_H

#include <QAbstractItemModel.h>
#include <QTreeView.h>
#include <Easy2D.h>

class ListComponents : public QAbstractItemModel
{

	Q_OBJECT

public:
	ListComponents(QObject *parent = 0)
		:QAbstractItemModel(parent)
		,target(nullptr)
	{}

	void setTarget(Easy2D::Object* argTarget)
	{
		//new items
		beginInsertColumns(QModelIndex(),0,0);
		beginInsertRows(QModelIndex(),0,0);
		target=argTarget;		
		//reset ui
		endInsertRows();
		endInsertColumns();
	}
	QVariant data(const QModelIndex &index, int role) const
	{
		 if (!index.isValid())
			 return QVariant();

		 if (role != Qt::DisplayRole)
			 return QVariant();
		 
		//components
		Easy2D::Component *item = nullptr;
		item = static_cast<Easy2D::Component*>(index.internalPointer());

		return item ? QVariant(item->getComponentName()) : QVariant();
	}
	QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const
	{
		//no obj return a void index
		if (!hasIndex(row, column, parent) || !target )
			return QModelIndex();
		if (column > 1)
			return QModelIndex();
		//else
		Easy2D::Component* comp=nullptr;
		size_t row_i=0;
		auto comps=target->getComponents();
		for(auto itComp:comps)
		{
			if(row==row_i)
			{
				comp=itComp.second;
				break;
			}
			++row_i;
		}
		//
		if(comp)
			return createIndex(row, column, comp);
		else
			return QModelIndex();
	}
	QModelIndex parent(const QModelIndex &index) const
	{
		return QModelIndex();
	}
	int rowCount(const QModelIndex &parent = QModelIndex()) const
	{
		if (parent.column() > 0 || !target)
			return 0;
		return (int)target->getComponents().size();
	}
	int columnCount(const QModelIndex &parent = QModelIndex()) const
	{
		return 1;
	}

protected:

	Easy2D::Object* target;

};

#endif