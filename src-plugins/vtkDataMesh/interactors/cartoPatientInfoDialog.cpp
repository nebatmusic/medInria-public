
#include "cartoPatientInfoDialog.h"

#include <medDataIndex.h>
#include <medAbstractDatabaseItem.h>

#include <QtGui>


class cartoPatientInfoDialogPrivate
{
public:

    QList<QString> values;

    QList<QString> attributes;
};


cartoPatientInfoDialog::cartoPatientInfoDialog(QList<QString> attributes, QList<QString> dataList, QWidget *parent): QDialog(parent,
            Qt::Dialog | Qt::WindowCloseButtonHint), d (new cartoPatientInfoDialogPrivate)

{
    QVBoxLayout *dialogLayout = new QVBoxLayout;
    QFormLayout *formLayout = new QFormLayout;
    QHBoxLayout *buttonLayout = new QHBoxLayout;

    QPushButton *okButton = new QPushButton(tr("OK"));
    QPushButton *cancelButton = new QPushButton(tr("Cancel"));
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    dialogLayout->addLayout(formLayout);
    dialogLayout->addStretch();
    dialogLayout->addLayout(buttonLayout);

    int i=0;

    d->attributes = attributes;
    d->values = dataList;

    foreach(QString attrib, attributes)
    {
        QString data = dataList[i];
        i++;

        if(attrib.isEmpty())
            continue;
        
        QLineEdit *textEdit = NULL;

        textEdit = new QLineEdit(this);
        textEdit->setMaxLength(30);
        QRegExp allowedChars(".*[a-zA-Z0-9_ -].*");
        QValidator * validator = new QRegExpValidator(allowedChars);
        textEdit->setValidator(validator);
        textEdit->setObjectName(attrib);
        textEdit->setText(data);
        formLayout->addRow(attrib, textEdit);
        connect(textEdit, SIGNAL(textChanged(const QString &)), this, SLOT(setValue(const QString &)));

    }

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(validate()));

    setLayout(dialogLayout);
    setModal(true);
}

cartoPatientInfoDialog::~cartoPatientInfoDialog()
{
    delete d;
    d = NULL;
}


void cartoPatientInfoDialog::setValue(const QString & text)
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

void cartoPatientInfoDialog::cancel()
{
    this->reject();
}


void cartoPatientInfoDialog::validate()
{
    this->accept();
}

QString cartoPatientInfoDialog::value(QString attribute)
{
    QString res;
    int index = d->attributes.indexOf(attribute);

    if(index>-1 && index<d->values.length()+1)
        res = d->values[index];

    if(res.isNull())
        res="";

    return res;
}