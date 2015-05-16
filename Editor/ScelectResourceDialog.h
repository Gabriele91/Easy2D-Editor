#ifndef SCELECTRESOURCEDIALOG_H
#define SCELECTRESOURCEDIALOG_H

#include <QWidget>
#include <QDialog>
#include <ManagerResourcesList.h>

namespace Ui
{
class ScelectResourceDialog;
};

class ScelectResourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScelectResourceDialog(Easy2D::Table* table,
                                   const QString& defaultrs,
                                   QWidget *parent = 0);
    explicit ScelectResourceDialog(const QStringList &filter,
                                   Easy2D::Table* table,
                                   const QString& defaultrs,
                                   QWidget *parent = 0);
    ~ScelectResourceDialog();
    QString doExec()
    {
        QDialog::exec();
        return lastName;
    }

private:
    Ui::ScelectResourceDialog *ui;
    ManagerResourcesList* resources;
    QString lastName;
};

#endif // SCELECTRESOURCEDIALOG_H
