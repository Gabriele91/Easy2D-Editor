#ifndef MANAGERRESOURCESLIST_H
#define MANAGERRESOURCESLIST_H

#include <QList>
#include <QPushButton>
#include <QListWidget>
#include <Easy2DInit.h>

class ManagerResourcesList
{

public:

	ManagerResourcesList(QListWidget* list,
						 Easy2D::Table* table)
		:list(list),
		 table(table)
	{
		update();
	}
	
	QListWidget& getListWidget()
	{
		return *list;
	}
	Easy2D::Table& getTable()
	{
		return *table;
	}

    void changeTable(Easy2D::Table* argTable)
    {
        table=argTable;
        update();
    }

    void update(const QStringList& filter=QStringList())
	{
		list->clear();
		for(auto element:*table)
		{
            QString name=QString::fromUtf8(element.first.string());
            if(filter.indexOf(name)!=-1) break;
			list->addItem(name);
		}
	}

private:

	QListWidget* list;
	Easy2D::Table* table;

};


#endif
