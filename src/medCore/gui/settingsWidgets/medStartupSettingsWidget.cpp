/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medStartupSettingsWidget.h>
#include <QWidget>
#include <QtGui>
#include <QtCore>

#include <medSettingsManager.h>
#include <medWorkspaceFactory.h>

class medStartupSettingsWidgetPrivate {

public:
  QWidget* parent;
  QCheckBox* startInFullScreen;
  QComboBox* defaultStartingArea;
  QComboBox* theme;

  medStartupSettingsWidgetPrivate();
  ~medStartupSettingsWidgetPrivate();
};

medStartupSettingsWidgetPrivate::medStartupSettingsWidgetPrivate()
{
}

medStartupSettingsWidgetPrivate::~medStartupSettingsWidgetPrivate()
{
}


medStartupSettingsWidget::medStartupSettingsWidget(QWidget *parent) :
        medSettingsWidget(parent),
        d(new medStartupSettingsWidgetPrivate())
{
    setTabName(tr("Startup"));
    d->startInFullScreen = new QCheckBox(this);
    d->startInFullScreen->setToolTip(tr("Start application in full screen mode?"));

    QList<medWorkspaceFactory::Details*> workspaceDetails = medWorkspaceFactory::instance()->workspaceDetailsSortedByName();

    d->defaultStartingArea = new QComboBox(this);
    d->defaultStartingArea->addItem(tr("Search"));
    d->defaultStartingArea->setItemData(0, 0, Qt::UserRole - 1); // Search is disabled
    d->defaultStartingArea->addItem(tr("Homepage"));
    d->defaultStartingArea->addItem(tr("Browser"));
    foreach ( medWorkspaceFactory::Details* detail, workspaceDetails )
        d->defaultStartingArea->addItem(detail->name);

    d->theme = new QComboBox(this);
    d->theme->addItem(tr("Dark"));
    d->theme->addItem(tr("Light"));
    d->theme->setToolTip(tr("Choose a theme displayed at start-up"));

    QFormLayout* layout = new QFormLayout;
    layout->addRow(tr("Fullscreen"),d->startInFullScreen);
    layout->addRow(tr("Starting area"), d->defaultStartingArea);
    layout->addRow(tr("Theme"), d->theme);
    this->setLayout(layout);
}

/**
 * @brief Performs the validation of each control inside the widget.
 *
 * @param void
 * @return true is the validation is successful, false otherwise.
*/
bool medStartupSettingsWidget::validate()
{
    return true;
}

void medStartupSettingsWidget::read()
{
    medSettingsManager * mnger = medSettingsManager::instance();
    d->startInFullScreen->setChecked(mnger->value("startup", "fullscreen").toBool());

    //if nothing is configured then Homepage is the default area
    int index = mnger->value("startup", "default_starting_area", 1).toInt();

    // clamp range
    if (index < 0)
        index = 0;
    if (index > d->defaultStartingArea->count() -1)
        index = d->defaultStartingArea->count() -1;

    d->defaultStartingArea->setCurrentIndex(index);

    // Theme Settings
    int indexTheme = mnger->value("startup", "theme", 0).toInt();
    if (indexTheme < 0)
        indexTheme = 0;
    if (indexTheme > d->theme->count() -1)
        indexTheme = d->theme->count() -1;
    d->theme->setCurrentIndex(indexTheme);

}

bool medStartupSettingsWidget::write()
{
    medSettingsManager * mnger = medSettingsManager::instance();
    mnger->setValue("startup","fullscreen", d->startInFullScreen->isChecked());
    mnger->setValue("startup","default_starting_area",
                    d->defaultStartingArea->currentIndex());

    mnger->setValue("startup","theme",
                    d->theme->currentIndex());
    return true;
}
