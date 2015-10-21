#pragma once

#include <medDataIndex.h>
#include <QDialog>

class cartoPatientInfoDialogPrivate;


/**
 * @brief
 */
class cartoPatientInfoDialog: public QDialog
{
    Q_OBJECT

public:
    cartoPatientInfoDialog(QList<QString> attributes, QList<QString> data, QWidget *parent);

    virtual ~cartoPatientInfoDialog();

    QString value(QString attribute);

public slots:
    
    void setValue(const QString &value);
    
    void cancel();
    
    void validate();


private:

    cartoPatientInfoDialogPrivate *d;
};


