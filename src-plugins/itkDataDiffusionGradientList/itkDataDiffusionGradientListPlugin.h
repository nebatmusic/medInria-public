/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medPlugin.h>
#include <itkDataDiffusionGradientListPluginExport.h>

class ITKDATADIFFUSIONGRADIENTLISTPLUGIN_EXPORT itkDataDiffusionGradientListPluginPrivate;

class ITKDATADIFFUSIONGRADIENTLISTPLUGIN_EXPORT itkDataDiffusionGradientListPlugin : public medPlugin
{
    Q_OBJECT
    Q_INTERFACES(dtkPlugin)

public:
     itkDataDiffusionGradientListPlugin(QObject *parent = 0);
    ~itkDataDiffusionGradientListPlugin();

    virtual bool initialize();

    virtual QString name() const;
    virtual QString description() const;
    virtual QString version() const;
    virtual QStringList tags() const;
    virtual QStringList types() const;

private:
     itkDataDiffusionGradientListPluginPrivate *d;
};


