/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medAbstractWorkspace.h>
#include "reformatPluginExport.h"

class medSelectorToolBox;
class medReformatWorkspacePrivate;
    
class REFORMATPLUGIN_EXPORT medReformatWorkspace : public medAbstractWorkspace
{
    Q_OBJECT
    MED_WORKSPACE_INTERFACE("Reformat",
                            "Reformat workspace.",
                            "Methodology")
    
public:
    medReformatWorkspace(QWidget *parent = NULL);
    virtual ~medReformatWorkspace();

    static bool isUsable();
    static bool registered();

    medSelectorToolBox *selectorToolBox();

protected slots:
    void onSuccess();

private:
    medReformatWorkspacePrivate *d;
};
