#pragma once

#include <medDataIndex.h>
#include <QDialog>

class mscExportVideoDialogPrivate;

class mscExportVideoDialog: public QDialog
{
    Q_OBJECT

public:
    enum ExportVideoName
    {
        TIME,
        ROTATION,
    };

    mscExportVideoDialog(QWidget *parent, int numberOfFrames );
    virtual ~mscExportVideoDialog();

    QVector<int> value();

public slots:      
    void cancel();
    void validate();

protected slots:
    /**
     * @brief adaptWidgetForMethod adapt widgets according to the chosen exporting method
     * @param int: method chosen
     */
    void adaptWidgetForMethod(int method);

private:
    mscExportVideoDialogPrivate *d;
};
