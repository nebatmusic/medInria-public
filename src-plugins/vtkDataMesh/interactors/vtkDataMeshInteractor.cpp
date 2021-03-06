/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "vtkDataMeshInteractor.h"

#include <QVTKWidget.h>

#include <vtkCamera.h>
#include <vtkActor.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkImageView.h>
#include <vtkImageView2D.h>
#include <vtkImageView3D.h>
#include <vtkPointSet.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkBoundingBox.h>
#include <vtkImageFromBoundsSource.h>
#include <vtkImageData.h>
#include <vtkImageActor.h>
#include <vtkLookupTableManager.h>
#include <vtkLookupTable.h>
#include <vtkMetaDataSet.h>
#include <vtkMetaDataSetSequence.h>
#include <vtkDataArrayCollection.h>

#include <dtkLog/dtkLog.h>

#include <medAbstractData.h>
#include <medAbstractParameter.h>
#include <medBoolParameter.h>
#include <medDoubleParameter.h>
#include <medStringListParameter.h>
#include <medAbstractData.h>
#include <medViewFactory.h>
#include <medAbstractImageView.h>
#include <medIntParameter.h>
#include <medVtkViewBackend.h>
#include <vtkScalarBarActor.h>

#include <vector>

typedef QPair<vtkSmartPointer<vtkLookupTable>, QString> LutPair;

class vtkDataMeshInteractorPrivate
{
public:
    typedef vtkSmartPointer <vtkActor>  ActorSmartPointer;
    typedef vtkSmartPointer <vtkProperty>  PropertySmartPointer;
    typedef vtkSmartPointer <vtkDataArray>  AttributeSmartPointer;

    // views
    medAbstractImageView *view;
    vtkImageView2D *view2d;
    vtkImageView3D *view3d;
    medAbstractData *data;

    vtkMetaDataSet* metaDataSet;
    ActorSmartPointer actor2d;
    ActorSmartPointer actor3d;
    PropertySmartPointer actorProperty2D;
    PropertySmartPointer actorProperty3D;

    LutPair lut;
    AttributeSmartPointer attribute;
    double imageBounds[6];

    medStringListParameter *attributesParam;
    medStringListParameter *LUTParam;
    medBoolParameter *edgeVisibleParam;
    medStringListParameter *colorParam;
    medStringListParameter *renderingParam;
    medDoubleParameter *minRange,*maxRange;

    QPushButton * range_button;
    
    QList <medAbstractParameter*> parameters;

    QPointer<QWidget> toolbar;
    medIntParameter *slicingParameter;
};


vtkDataMeshInteractor::vtkDataMeshInteractor(medAbstractView *parent):
    medAbstractImageViewInteractor(parent), d(new vtkDataMeshInteractorPrivate)
{
    d->view = dynamic_cast<medAbstractImageView*>(parent);

    medVtkViewBackend* backend = static_cast<medVtkViewBackend*>(parent->backend());
    d->view2d = backend->view2D;
    d->view3d = backend->view3D;

    for (int i=0; i<6; i++)
        d->imageBounds[i] = 0;

    d->attributesParam = NULL;
    d->LUTParam = NULL;
    d->edgeVisibleParam = NULL;
    d->colorParam = NULL;
    d->renderingParam = NULL;
    d->minRange = 0;
    d->maxRange = 0;

    d->range_button = new QPushButton("Modify Range");
    d->range_button->setCheckable(true);
    connect(d->range_button,SIGNAL(toggled(bool)),this,SLOT(showRangeWidgets(bool)));

    d->slicingParameter = new medIntParameter("Slicing", this);
    d->slicingParameter->getSlider()->setOrientation(Qt::Horizontal);
    connect(d->slicingParameter, SIGNAL(valueChanged(int)), this, SLOT(moveToSlice(int)));
}


vtkDataMeshInteractor::~vtkDataMeshInteractor()
{
    delete d;
    d = NULL;
}


QString vtkDataMeshInteractor::description() const
{
    return tr("Interactor displaying Meshes");
}


QString vtkDataMeshInteractor::identifier() const
{
    return "vtkDataMeshInteractor";
}


QStringList vtkDataMeshInteractor::handled() const
{
    return vtkDataMeshInteractor::dataHandled();
}

QStringList vtkDataMeshInteractor::dataHandled()
{
    QStringList d = QStringList() << "vtkDataMesh";
    return  d;
}

bool vtkDataMeshInteractor::registered()
{
    medViewFactory *factory = medViewFactory::instance();
    return factory->registerInteractor<vtkDataMeshInteractor>("vtkDataMeshInteractor",
                                                              QStringList () << "medVtkView" <<
                                                              vtkDataMeshInteractor::dataHandled());
}


void vtkDataMeshInteractor::setInputData(medAbstractData *data)
{
    medAbstractInteractor::setInputData(data);

    vtkMetaDataSet * mesh = dynamic_cast<vtkMetaDataSet*>((vtkDataObject *)(data->data()));

    d->metaDataSet = mesh;
    d->lut = LutPair(NULL, "Default");

    updatePipeline();

    setupParameters();
}

void vtkDataMeshInteractor::setupParameters()
{
    d->parameters << this->opacityParameter();
    this->setOpacity(1.0);

    if(!(d->metaDataSet->GetType() != vtkMetaDataSet::VTK_META_SURFACE_MESH &&
         d->metaDataSet->GetType() != vtkMetaDataSet::VTK_META_VOLUME_MESH))
    {
        d->LUTParam = new medStringListParameter("LUT", this);
        d->LUTParam->addItem("Default");
        // TODO some LUT are missing for some attribute - RDE
        connect(d->LUTParam, SIGNAL(valueChanged(QString)), this, SLOT(setLut(QString)));
        d->LUTParam->setValue("Default");
        d->parameters << d->LUTParam;

        d->attributesParam = new medStringListParameter("Attributes", this);
        QStringList nameList("Default");

        for (int i = 0; i < d->metaDataSet->GetDataSet()->GetPointData()->GetNumberOfArrays(); i++)
            nameList << QString::fromUtf8(d->metaDataSet->GetDataSet()->GetPointData()->GetArrayName(i));

        for (int i = 0;  i < d->metaDataSet->GetDataSet()->GetCellData()->GetNumberOfArrays(); i++)
            nameList << QString::fromUtf8(d->metaDataSet->GetDataSet()->GetCellData()->GetArrayName(i));

        for (int i = 0;  i < d->metaDataSet->GetDataSet()->GetFieldData()->GetNumberOfArrays(); i++)
            nameList << QString::fromUtf8(d->metaDataSet->GetDataSet()->GetFieldData()->GetArrayName(i));

        foreach(QString name, nameList)
            d->attributesParam->addItem(name);

        connect(d->attributesParam, SIGNAL(valueChanged(QString)), this, SLOT(setAttribute(QString)));
        d->attributesParam->setValue("Default");
        d->parameters << d->attributesParam;
    }

    d->edgeVisibleParam = new medBoolParameter("Edge Visible", this);
    connect(d->edgeVisibleParam, SIGNAL(valueChanged(bool)), this, SLOT(setEdgeVisibility(bool)));
    d->edgeVisibleParam->setValue(false);
    d->parameters << d->edgeVisibleParam;

    d->colorParam = new medStringListParameter("Color", this);
    QStringList colors;

    colors << "#FFFFFF";
    colors << "#808080";
    colors << "#800000";
    colors << "#804040";
    colors << "#FF8080";
    colors << "#FF0000";
    colors << "#FFFF80";
    colors << "#FFFF00";
    colors << "#FF8040";
    colors << "#FF8000";
    colors << "#80FF80";
    colors << "#80FF00";
    colors << "#00FF00";
    colors << "#80FFFF";
    colors << "#00FFFF";
    colors << "#004080";
    colors << "#0000FF";
    colors << "#0080FF";
    colors << "#0080C0";

    foreach(QString color, colors)
        d->colorParam->addItem(color, medStringListParameter::createIconFromColor(color));

    connect(d->colorParam, SIGNAL(valueChanged(QString)), this, SLOT(setColor(QString)));
    if (!d->metaDataSet->GetActor(0))
    {
        d->colorParam->setValue("#FFFFFF");
    }
    else
    {
        // color and opacity defined in xml readers
        double * color = d->metaDataSet->GetActor(0)->GetProperty()->GetColor();

        int final_colors[3];
        final_colors[0] = int(color[0]*255.0);
        final_colors[1] = int(color[1]*255.0);
        final_colors[2] = int(color[2]*255.0);

        // decimal to hex color
        QString colorHex;
        colorHex += "#";
        for (int i=0; i<3; i++)
        {
            // Add a "0" to the hex number to follow "RRGGBBAA"
            if (final_colors[i] < 15)
            {
                colorHex += "0";
            }
            colorHex += QString::number(final_colors[i], 16).toUpper();
        }
        this->setColor(colorHex);

        this->setOpacity(d->metaDataSet->GetActor(0)->GetProperty()->GetOpacity());
    }

    d->parameters << d->colorParam;

    d->renderingParam = new medStringListParameter("Rendering", this);
    d->renderingParam->addItem("WireFrame");
    d->renderingParam->addItem("Surface");
    d->renderingParam->addItem("Points");
    connect(d->renderingParam, SIGNAL(valueChanged(QString)), this, SLOT(setRenderingType(QString)));
    d->renderingParam->setValue("Surface");
    d->parameters << d->renderingParam;

    connect(d->view->positionBeingViewedParameter(), SIGNAL(valueChanged(QVector3D)), this, SLOT(updateSlicingParam()));

    d->parameters << this->visibilityParameter();

    d->minRange = new medDoubleParameter("Min",this);
    d->minRange->setDecimals(4);
    d->parameters << d->minRange;
    d->maxRange = new medDoubleParameter("Max",this);
    d->maxRange->setDecimals(4);
    d->parameters << d->maxRange;

    connect(d->minRange,SIGNAL(valueChanged(double)),this,SLOT(updateRange()));
    connect(d->maxRange,SIGNAL(valueChanged(double)),this,SLOT(updateRange()));

    this->updateWidgets();
}

void vtkDataMeshInteractor::setOpacity(double value)
{
    d->actorProperty2D->SetOpacity(value);
    d->actorProperty3D->SetOpacity(value);

    this->opacityParameter()->setValue(value);
    d->view->render();
}

void vtkDataMeshInteractor::setVisibility(bool visible)
{
    int v = (visible) ? 1 : 0;
    d->actor2d->SetVisibility(v);
    d->actor3d->SetVisibility(v);

    d->view->render();
}

void vtkDataMeshInteractor::setEdgeVisibility(bool visible)
{
    int v = (visible) ? 1 : 0;
    d->actorProperty2D->SetEdgeVisibility(v);
    d->actorProperty3D->SetEdgeVisibility(v);

    d->view->render();
}


bool vtkDataMeshInteractor::edgeVisibility() const
{
    return (d->actorProperty3D->GetEdgeVisibility() == 1);
}


void vtkDataMeshInteractor::setColor(QColor color)
{
    if( ! color.isValid())
        color.setRgb(0,0,0);

    double r,g,b;
    color.getRgbF(&r, &g, &b);
    d->actorProperty2D->SetColor(r, g, b);
    d->actorProperty3D->SetColor(r, g, b);

    d->view->render();
}

void vtkDataMeshInteractor::setColor(const QString &color)
{
    d->colorParam->setValue(color);
    setColor(QColor(color));
}

QColor vtkDataMeshInteractor::color() const
{
    double r,g,b;
    d->actorProperty3D->GetColor(r, g, b);
    return QColor::fromRgbF(r, b, g);
}

void vtkDataMeshInteractor::setRenderingType(const QString & type)
{
    d->renderingParam->setValue(type);
    QString value = type.toLower();
    if (value == "wireframe")
    {
        d->actorProperty2D->SetRepresentationToWireframe ();
        d->actorProperty3D->SetRepresentationToWireframe ();
    }
    else if (value == "surface")
    {
        d->actorProperty2D->SetRepresentationToSurface ();
        d->actorProperty3D->SetRepresentationToSurface ();
    }
    else if (value == "points")
    {
        d->actorProperty2D->SetRepresentationToPoints ();
        d->actorProperty3D->SetRepresentationToPoints ();
    }

    d->view->render();
}


QString vtkDataMeshInteractor::renderingType() const
{
    return QString::fromStdString(d->actorProperty3D->GetRepresentationAsString()).toLower();
}

void vtkDataMeshInteractor::setMaxRange(double max)
{
    d->maxRange->setValue(max);
}

void vtkDataMeshInteractor::setMinRange(double min)
{
    d->minRange->setValue(min);
}

void vtkDataMeshInteractor::setAttribute(const QString & attributeName)
{
    d->attributesParam->setValue(attributeName);
    vtkPointSet * pointSet = vtkPointSet::SafeDownCast(d->metaDataSet->GetDataSet());
    if ( ! pointSet )
        return;

    vtkDataSetAttributes * attributes = NULL;
    vtkMapper * mapper2d = d->actor2d->GetMapper();
    vtkMapper * mapper3d = d->actor3d->GetMapper();
    if (pointSet->GetPointData()->HasArray(qPrintable(attributeName)))
    {
        attributes = pointSet->GetPointData();
        mapper2d->SetScalarModeToUsePointFieldData();
        mapper3d->SetScalarModeToUsePointFieldData();
    }
    else if (pointSet->GetCellData()->HasArray(qPrintable(attributeName)))
    {
        attributes = pointSet->GetCellData();
        mapper2d->SetScalarModeToUseCellFieldData();
        mapper3d->SetScalarModeToUseCellFieldData();
    }

    if (attributes)
    {

        if(d->colorParam)
            d->colorParam->hide();
        if(d->LUTParam)
        {
            QStringList luts;
            const std::vector<std::string> & vec = vtkLookupTableManager::GetAvailableLookupTables();
            for(std::vector<std::string>::const_iterator it = vec.begin(); it < vec.end(); ++it)
                luts.append(QString::fromStdString(*it));
            d->LUTParam->clear();
            foreach(QString lut, luts)
                d->LUTParam->addItem(lut);

            d->attribute = attributes->GetArray(qPrintable(attributeName));
            attributes->SetActiveScalars(qPrintable(attributeName));
            d->metaDataSet->SetCurrentActiveArray(attributes->GetArray(qPrintable(attributeName)));

            d->LUTParam->show();
        }

        mapper2d->SelectColorArray(qPrintable(attributeName));
        mapper3d->SelectColorArray(qPrintable(attributeName));

        d->range_button->show();
        double * range = d->metaDataSet->GetScalarRange(attributeName);
        d->minRange->setRange(range[0],range[1]);
        d->maxRange->setRange(range[0],range[1]);
        d->minRange->getSpinBox()->setRange(range[0],range[1]);
        d->maxRange->getSpinBox()->setRange(range[0],range[1]);
        d->minRange->setValue(range[0]);
        d->maxRange->setValue(range[1]);
        d->minRange->getSpinBox()->setValue(range[0]);
        d->maxRange->getSpinBox()->setValue(range[1]);
        d->view2d->SetColorRange(range);
        d->view3d->SetColorRange(range);
        
        this->setLut(d->lut.first);

        mapper2d->SetScalarVisibility(1);
        mapper3d->SetScalarVisibility(1);
    }
    else
    {
        d->range_button->setChecked(false);
        d->range_button->hide();
        if(d->LUTParam)
            d->LUTParam->hide();
        if(d->colorParam)
            d->colorParam->show();

        d->attribute = NULL;
        mapper2d->SetScalarVisibility(0);
        mapper3d->SetScalarVisibility(0);
    }
    d->view->render();
}


QString vtkDataMeshInteractor::attribute() const
{
    return QString::fromUtf8(d->actor2d->GetMapper()->GetArrayName());
}


void vtkDataMeshInteractor::setLut(const QString & lutName)
{
    d->LUTParam->setValue(lutName);
    vtkLookupTable * lut = NULL;

    if (lutName != "Default")
        lut = vtkLookupTableManager::GetLookupTable(lutName.toStdString());

    if ( ! d->attribute)
        return;

    d->lut = LutPair(lut, lutName);
    this->setLut(lut);

    d->view->render();
}


QString vtkDataMeshInteractor::lut() const
{
    return d->lut.second;
}


void vtkDataMeshInteractor::updatePipeline ()
{
    if (d->view && d->metaDataSet )
    {
        if(vtkPointSet * pointSet = vtkPointSet::SafeDownCast (d->metaDataSet->GetDataSet()))
        {
            d->actor2d = d->view2d->AddDataSet(pointSet);
            d->actor3d = d->view3d->AddDataSet(pointSet);

            d->metaDataSet->AddActor(d->actor2d);
            d->metaDataSet->AddActor(d->actor3d);

            d->actorProperty2D = vtkDataMeshInteractorPrivate::PropertySmartPointer::New();
            d->actorProperty3D = vtkDataMeshInteractorPrivate::PropertySmartPointer::New();

            d->actorProperty2D->SetLighting(false);
            d->actorProperty2D->SetLineWidth(3.0);
            d->actor2d->SetProperty(d->actorProperty2D);
            d->actor3d->SetProperty(d->actorProperty3D);

            d->view2d->UpdateBounds(pointSet->GetBounds(), d->view->layer(this->inputData()));
        }
    }
    d->view3d->ResetCamera();
}


void vtkDataMeshInteractor::setLut(vtkLookupTable * lut)
{
    if ( ! d->attribute )
        return;

    if ( ! lut )
    {
        lut = d->attribute->GetLookupTable();
        d->lut = LutPair(lut, "Default");
    }

    // remove the alpha channel from the LUT, it messes up the mesh
    if (lut)
    {
        double values[4];
        for(int i = 0; i < lut->GetNumberOfTableValues(); i++)
        {
            lut->GetTableValue(i, values);
            values[3] = 1.0;
            lut->SetTableValue(i, values);
        }
        double * range = d->metaDataSet->GetScalarRange(d->attributesParam->value());
        lut->SetRange(range);
    }

    vtkMapper * mapper2d = d->actor2d->GetMapper();
    vtkMapper * mapper3d = d->actor3d->GetMapper();
    mapper2d->InterpolateScalarsBeforeMappingOn();
    mapper3d->InterpolateScalarsBeforeMappingOn();

    d->view2d->SetLookupTable(lut);
    mapper2d->SetLookupTable(lut);
    mapper2d->UseLookupTableScalarRangeOn();
    mapper2d->InterpolateScalarsBeforeMappingOn();
    
    d->view3d->SetLookupTable(lut,d->view->layer(this->inputData()));
    mapper3d->SetLookupTable(lut);
    mapper3d->UseLookupTableScalarRangeOn();
    mapper3d->InterpolateScalarsBeforeMappingOn();
    
    updateRange();
}


void vtkDataMeshInteractor::removeData()
{
    if(d->metaDataSet)
    {
        if(vtkPointSet * pointSet = vtkPointSet::SafeDownCast (d->metaDataSet->GetDataSet()))
        {
            d->view2d->RemoveDataSet(pointSet);
            d->view3d->RemoveDataSet(pointSet);
            d->metaDataSet->RemoveActor(d->actor2d);
            d->metaDataSet->RemoveActor(d->actor3d);
            d->actor2d = 0;
            d->actor3d = 0;

            d->view->render();
        }
    }
}

void vtkDataMeshInteractor::moveToSlice  (int slice)
{
    //TODO find a way to get woorldCoordinate for slice from vtkInria.
    // instead of moving to the slice corresponding on the first layer dropped.
    if(d->view->is2D() && slice != d->view2d->GetSlice())
    {
        d->view2d->SetSlice(slice);
        d->view2d->Render();
    }
}

void vtkDataMeshInteractor::setWindowLevel (QHash<QString,QVariant>)
{
    // TODO
}

QWidget* vtkDataMeshInteractor::buildLayerWidget()
{
    QSlider *slider = this->opacityParameter()->getSlider();
    slider->setOrientation(Qt::Horizontal);
    return slider;
}

QWidget* vtkDataMeshInteractor::buildToolBoxWidget()
{
    QWidget *toolbox = new QWidget;
    QFormLayout *layout = new QFormLayout(toolbox);
    if(d->attributesParam)
        layout->addRow(d->attributesParam->getLabel(), d->attributesParam->getComboBox());

    layout->addRow(d->LUTParam->getLabel(), d->LUTParam->getComboBox());
    layout->addRow(d->edgeVisibleParam->getLabel(), d->edgeVisibleParam->getCheckBox());
    layout->addRow(d->colorParam->getLabel(), d->colorParam->getComboBox());
    layout->addRow(d->renderingParam->getLabel(), d->renderingParam->getComboBox());
    layout->addRow(d->range_button);
    d->minRange->getSlider()->setOrientation(Qt::Horizontal);
    d->maxRange->getSlider()->setOrientation(Qt::Horizontal);
    QHBoxLayout *minRangeLayout = new QHBoxLayout(toolbox);
    QHBoxLayout *maxRangeLayout = new QHBoxLayout(toolbox);
    minRangeLayout->addWidget(d->minRange->getSlider());
    minRangeLayout->addWidget(d->minRange->getSpinBox());
    maxRangeLayout->addWidget(d->maxRange->getSlider());
    maxRangeLayout->addWidget(d->maxRange->getSpinBox());
    layout->addRow(d->minRange->getLabel(),minRangeLayout);
    layout->addRow(d->maxRange->getLabel(),maxRangeLayout);
    showRangeWidgets(false);
    return toolbox;
}

QWidget* vtkDataMeshInteractor::buildToolBarWidget()
{
    d->toolbar = new QWidget();
    QHBoxLayout* toolbarLayout = new QHBoxLayout(d->toolbar);
    d->slicingParameter->getSlider()->setOrientation(Qt::Horizontal);
    toolbarLayout->addWidget(d->slicingParameter->getSlider());
    return d->toolbar;
}

QList<medAbstractParameter*> vtkDataMeshInteractor::linkableParameters()
{
    return d->parameters;
}

QList<medBoolParameter*> vtkDataMeshInteractor::mouseInteractionParameters()
{
    // no parameters related to mouse interactions
    return QList<medBoolParameter*>();
}

void vtkDataMeshInteractor::setUpViewForThumbnail()
{

    d->view->setOrientation(medImageView::VIEW_ORIENTATION_3D);
    d->view->reset();
    d->view3d->ShowAnnotationsOff();
}

void vtkDataMeshInteractor::updateWidgets()
{
    if (!d->toolbar.isNull())
    {
        if(!d->view->is2D())
        {
            d->slicingParameter->hide();
        }
        else
        {
            d->slicingParameter->show();
            this->updateSlicingParam();
        }
    }
}


void vtkDataMeshInteractor::updateSlicingParam()
{
    if(!d->view->is2D())
        return;
    //TODO Should be set according to the real number of slice of this data and
    // not according to vtkInria (ie. first layer droped) - RDE

    d->slicingParameter->blockSignals(true);
    d->slicingParameter->setRange(d->view2d->GetSliceMin(), d->view2d->GetSliceMax());
    d->slicingParameter->blockSignals(false);

    d->slicingParameter->setValue(d->view2d->GetSlice());
}

void vtkDataMeshInteractor::updateRange()
{
    if (!d->metaDataSet)
        return;
    
    vtkMapper * mapper2d = d->actor2d->GetMapper();
    vtkMapper * mapper3d = d->actor3d->GetMapper();

    vtkLookupTable * lut = vtkLookupTable::SafeDownCast(mapper3d->GetLookupTable());
    
    if (!lut)
        return;
    
    lut->SetRange(d->minRange->value(),d->maxRange->value());
    mapper2d->SetLookupTable(lut);
    mapper3d->SetLookupTable(lut);
    d->view3d->GetScalarBar()->SetLookupTable(lut);
    d->view2d->GetScalarBar()->SetLookupTable(lut);

    d->view->render();
}

void vtkDataMeshInteractor::showRangeWidgets(bool checked)
{
    if (checked)
    {
        d->maxRange->show();
        d->minRange->show();
    }
    else
    {
        d->maxRange->hide();
        d->minRange->hide();
    }
}

void vtkDataMeshInteractor::restoreParameters(QHash<QString, QString> parameters)
{
    if(parameters.contains("Attributes"))
        setAttribute(parameters["Attributes"]);
    if(parameters.contains("Opacity"))
        setOpacity(medDoubleParameter::fromString(parameters["Opacity"]));
    if(parameters.contains("Visibility"))
        setVisibility(medBoolParameter::fromString(parameters["Visibility"]));
    if(parameters.contains("LUT"))
        setLut(parameters["LUT"]);
    if(parameters.contains("Edge Visible"))
        setEdgeVisibility(medBoolParameter::fromString(parameters["Edge Visible"]));
    if(parameters.contains("Rendering"))
        setRenderingType(parameters["Rendering"]);
    if(parameters.contains("Color"))
        setColor(parameters["Color"]);
    if(parameters.contains("Max"))
        setMaxRange(medDoubleParameter::fromString(parameters["Max"]));
    if(parameters.contains("Min"))
        setMinRange(medDoubleParameter::fromString(parameters["Min"]));
}

QString vtkDataMeshInteractor::name() const
{
    return "vtkDataMeshInteractor";
}
