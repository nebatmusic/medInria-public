
#include "medInfoDialog.h"

#include <medDataIndex.h>
#include <medAbstractDatabaseItem.h>

#include <QtGui>


class medInfoDialogPrivate
{
public:

    QList<QString> values;

    QList<QString> attributes;
};


medInfoDialog::medInfoDialog(QWidget *parent): QDialog(parent,
            Qt::Dialog | Qt::WindowCloseButtonHint), d (new medInfoDialogPrivate)

{
    QVBoxLayout *dialogLayout = new QVBoxLayout;

    QTableWidget* softwareShortcutsTable = new QTableWidget(2,2);
    softwareShortcutsTable->setAlternatingRowColors(true);
    softwareShortcutsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    softwareShortcutsTable->setFocusPolicy(Qt::NoFocus);
    softwareShortcutsTable->setSelectionMode(QAbstractItemView::NoSelection);
    softwareShortcutsTable->setHorizontalHeaderLabels(QStringList()<<"Shortcut"<<"Action");

    softwareShortcutsTable->setItem(0, 0, new QTableWidgetItem("Ctrl + 0"));
    softwareShortcutsTable->setItem(0, 1, new QTableWidgetItem("Opens a scene"));

    softwareShortcutsTable->setItem(1, 0, new QTableWidgetItem("Ctrl + W"));
    softwareShortcutsTable->setItem(1, 1, new QTableWidgetItem("Resets the viewer"));

    softwareShortcutsTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    softwareShortcutsTable->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    QTableWidget* meshManipShortcutsTable = new QTableWidget(4,2, this) ;
    meshManipShortcutsTable->setAlternatingRowColors(true);
    meshManipShortcutsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    meshManipShortcutsTable->setFocusPolicy(Qt::NoFocus);
    meshManipShortcutsTable->setSelectionMode(QAbstractItemView::NoSelection);
    meshManipShortcutsTable->setHorizontalHeaderLabels(QStringList()<<"Shortcut"<<"Action");

    meshManipShortcutsTable->setItem(0, 0, new QTableWidgetItem("Left"));
    QTableWidgetItem* tableWidget = new QTableWidgetItem("Rotation");
    tableWidget->setToolTip("Rotation of the camera around its focal point");
    meshManipShortcutsTable->setItem(0, 1, tableWidget);

    meshManipShortcutsTable->setItem(1, 0, new QTableWidgetItem("Right"));
    meshManipShortcutsTable->setItem(1, 1, new QTableWidgetItem("Zooming"));

    meshManipShortcutsTable->setItem(2, 0, new QTableWidgetItem("Middle \n(or Shift+Left)"));
    meshManipShortcutsTable->setItem(2, 1, new QTableWidgetItem("Panning"));

    meshManipShortcutsTable->setItem(3, 0, new QTableWidgetItem("Ctrl + Left"));
    meshManipShortcutsTable->setItem(3, 1, new QTableWidgetItem("Spinning"));

    meshManipShortcutsTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    meshManipShortcutsTable->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);


    QPushButton *okButton = new QPushButton(tr("OK"));

    dialogLayout->addWidget(new QLabel("Software shortcuts: "));
    dialogLayout->addWidget(softwareShortcutsTable);
    dialogLayout->addWidget(new QLabel("Mesh manipulation shortcuts (using mouse buttons): "));
    dialogLayout->addWidget(meshManipShortcutsTable);
    dialogLayout->addWidget(new QLabel("* for Mac users, use Cmd instead of Ctrl"));
    dialogLayout->addWidget(okButton);

    connect(okButton, SIGNAL(clicked()), this, SLOT(validate()));

    setLayout(dialogLayout);
    setModal(true);
}

medInfoDialog::~medInfoDialog()
{
    delete d;
    d = NULL;
}


void medInfoDialog::setValue(const QString & text)
{
    QWidget *currentWidget = QApplication::focusWidget();
    QString objectName = currentWidget->objectName();

    if(!objectName.isEmpty())
    {
        int index = d->attributes.indexOf(objectName);

        if(index>-1 && index<d->values.length())
        {
            d->values[index] = text;
        }
    }
}

void medInfoDialog::cancel()
{
    this->reject();
}


void medInfoDialog::validate()
{
    this->accept();
}

QString medInfoDialog::value(QString attribute)
{
    QString res;
    int index = d->attributes.indexOf(attribute);

    if(index>-1 && index<d->values.length()+1)
        res = d->values[index];

    if(res.isNull())
        res="";

    return res;
}