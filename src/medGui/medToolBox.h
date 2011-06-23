/* medToolBox.h ---
 *
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Oct  9 19:41:48 2009 (+0200)
 * Version: $Id$
 * Last-Updated: Mon Dec 20 12:51:04 2010 (+0100)
 *           By: Julien Wintz
 *     Update #: 83
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#ifndef MEDTOOLBOX_H
#define MEDTOOLBOX_H

#include "medGuiExport.h"

#include <QtGui>

class dtkAbstractView;
class dtkAbstractData;

class medToolBoxTab;
class medToolBoxPrivate;
class medToolBoxBody;
class medToolBoxHeader;



/**
 * @brief Toolbox that includes a title bar and a widget container.
 *
 * The toolboxes are displayed in the medToolBoxContainer.
 * They can be minimized, hidden, updated when a view is selected, etc...
 *
*/
class MEDGUI_EXPORT medToolBox : public QWidget
{
    Q_OBJECT

public:
    /**
    * @brief
    *
    * @param parent
    */
    medToolBox(QWidget *parent = 0);

    /**
     * @brief
     *
     * @param void
    */
    virtual ~medToolBox(void);

    /**
     * @brief Adds a widget to the toolbox.
     * This widget will be horizontally or vertically added to the layout given the toolbox's orientation.
     *
     * @param widget
    */
    void addWidget(QWidget *widget);

    /**
     * @brief Adds a medToolBoxTab to the toolbox.
     *
     * @param tab
    */
    void setTabWidget (medToolBoxTab* tab);

    /**
     * @brief Sets the toolbox's title.
     *
     * @param title
    */
    void setTitle(const QString& title);

    /**
     * @brief Sets the orientation of the toolbox.
     * Given the orientation, lays the widgets vertically or horizontally.
     * Usually used by the medViewerConfiguration.
     *
     * @param orientation
    */
    void setOrientation(Qt::Orientation orientation);

    /**
     * @brief Gets the toolbox's current orientation.
     *
     * @param void
     * @return Qt::Orientation
    */
    Qt::Orientation orientation(void) const;

    /**
     * @brief Gets the Toolbox's header.
     *
     * @param void
     * @return medToolBoxHeader *
    */
    medToolBoxHeader *header(void) const;

    /**
     * @brief Gets the toolbox's body.
     *
     * @param void
     * @return medToolBoxBody *
    */
    medToolBoxBody   *body(void)   const;

signals:
    /**
     * @brief Tells the world to add a new toolbox to the medToolboxContainer.
     * Typically used when a generic toolbox adds a custom toolbox.
     *
     * @param toolbox
    */
    void    addToolBox(medToolBox *toolbox);

    /**
     * @brief Tells the world to remove a toolbox from the medToolBoxContainer
     *
     * @param toolbox
    */
    void removeToolBox(medToolBox *toolbox);

    /**
     * @brief Emitted when an action from the toolbox succeeded.
     * Typically used when a dtkProcess returned.
     *
     * @param void
    */
    void success(void);

    /**
     * @brief Emitted when an action from the toolbox failed.
     *
     * Typically used when a dtkProcess returned.
     * @param void
    */
    void failure(void);

public slots:
    /**
     * @brief Clears the toolbox.
     *
     * Resets the parameters within the tolbox,
     * for instance when the current patient changed or the view.
     *
     * @param void
    */
    virtual void clear(void);

    /**
     * @brief Updates teh content of the toolbox when a new view is selected.
     *
     * @param view The view to extract inforamtion from.
    */
    virtual void update(dtkAbstractView *view);

    /**
     * @brief Switches from a minimized state to an extended one and vice versa.
     *
    */
    void switchMinimize();

    /**
     * @brief Remove not context meaningful toolboxes.
     *
    */
    void setValidDataTypes(const QStringList & dataTypes);
    void addValidDataType(const QString & dataType);

    void setContextVisibility(const QHash<QString, unsigned int> & viewDataTypes);
    void setContextVisible(bool contextVisibleFlag);
    bool ContextVisible();
    void show();

private:
    medToolBoxPrivate *d;
};

#endif
