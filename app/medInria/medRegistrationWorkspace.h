/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <QtCore>

#include <medSelectorWorkspace.h>

class medTabbedViewContainers;
class medRegistrationWorkspacePrivate;
class medAbstractData;

class medRegistrationWorkspace : public medSelectorWorkspace
{
    Q_OBJECT
    MED_WORKSPACE_INTERFACE("Registration",
                            "Workspace to compute the spatial transformation between two images in order to register them.",
                            "Methodology")

enum ContainerIndex
{
    Fixed = 0,
    Moving,
    Fuse
};

public:
    medRegistrationWorkspace(QWidget *parent);
    ~medRegistrationWorkspace();

    static bool isUsable();
    void setupViewContainerStack ();
    void setInitialGroups();

protected slots:
    void updateFromFixedContainer();
    void updateFromMovingContainer();
    void updateFromContainer(enum ContainerIndex);
    void updateUserLayerClosable(int tabIndex);
    void updateFromRegistrationSuccess(medAbstractData *output);

    void resetDefaultWidgetFixedContainer();
    void resetDefaultWidgetMovingContainer();
    void resetDefaultWidgetFuseContainer();

private:
    medRegistrationWorkspacePrivate *d;
};


