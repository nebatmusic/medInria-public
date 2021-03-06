#include "mscExportVideo.h"
#include "mscExportVideoPlugin.h"

namespace msc
{

ExportVideoPlugin::ExportVideoPlugin(QObject *parent) : medPlugin(parent)
{
}

bool ExportVideoPlugin::initialize()
{
    if (!ExportVideo::registered())
    {
        dtkWarn() << "Unable to register ExportVideo";
    }
    return true;
}

QString ExportVideoPlugin::name() const
{
    return "Export Video";
}

QString ExportVideoPlugin::description() const
{
    QString description = \
            tr("Export a 4D data as movie or JPEG images.\
               <br><br>This plugin uses the <a href=\"https://www.vtk.org/\" style=\"color: #cc0000\" >VTK library</a>.");
    return description;
}

QString ExportVideoPlugin::version() const
{
    return EXPORTVIDEOPLUGIN_VERSION;
}

QStringList ExportVideoPlugin::types() const
{
    return QStringList() << "ExportVideo";
}

Q_EXPORT_PLUGIN2(ExportVideoPlugin, ExportVideoPlugin)
}
