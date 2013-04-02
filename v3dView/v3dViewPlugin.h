// /////////////////////////////////////////////////////////////////
// Generated by dtkPluginGenerator
// /////////////////////////////////////////////////////////////////

#pragma once

#include <dtkCore/dtkPlugin.h>

#include "v3dViewPluginExport.h"

class V3DVIEWPLUGIN_EXPORT v3dViewPluginPrivate;

class V3DVIEWPLUGIN_EXPORT v3dViewPlugin : public dtkPlugin
{
    Q_OBJECT
    Q_INTERFACES(dtkPlugin)

public:
     v3dViewPlugin(QObject *parent = 0);
    ~v3dViewPlugin(void);

    virtual bool initialize(void);
    virtual bool uninitialize(void);

    virtual QString name(void) const;
    virtual QString description(void) const;
    virtual QString version(void) const;

    virtual QStringList authors(void) const;
    virtual QString contact(void) const;
    virtual QStringList contributors(void) const;


    virtual QStringList tags(void) const;
    virtual QStringList types(void) const;

private:
     v3dViewPluginPrivate *d;
};


