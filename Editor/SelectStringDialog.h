#ifndef SELECTSTRINGDIALOG_H
#define SELECTSTRINGDIALOG_H

#include <QDialog>
#include <QString>
#include <QStringList>

namespace Ui
{
class SelectStringDialog;
}

class SelectStringDialog : public QDialog
{
    Q_OBJECT

public:

    explicit SelectStringDialog(const QStringList& list,
                                const QString& defaultrs,
                                QWidget *parent = 0);
    ~SelectStringDialog();
    QString doExec()
    {
        QDialog::exec();
        return lastName;
    }

private:
    Ui::SelectStringDialog *ui;
    QString lastName;
};

#endif // SELECTSTRINGDIALOG_H
