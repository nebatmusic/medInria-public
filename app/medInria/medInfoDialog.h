#pragma once

#include <medDataIndex.h>
#include <QDialog>

class medInfoDialogPrivate;


/**
 * @brief
 */
class medInfoDialog: public QDialog
{
    Q_OBJECT

public:
    medInfoDialog(QWidget *parent = NULL);

    virtual ~medInfoDialog();

    QString value(QString attribute);

public slots:
    
    void setValue(const QString &value);
    
    void cancel();
    
    void validate();


private:

    medInfoDialogPrivate *d;
};


