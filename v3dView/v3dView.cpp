// /////////////////////////////////////////////////////////////////
// Generated by dtkPluginGenerator
// /////////////////////////////////////////////////////////////////

#include <vtkINRIA3DConfigure.h>

#ifdef vtkINRIA3D_USE_ITK
#include "medItk.h"
#endif

#include "v3dView.h"

#include <dtkCore/dtkAbstractViewFactory.h>
#include <dtkCore/dtkAbstractDataImage.h>

#include <vtkCommand.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkLookupTableManager.h>
#include <vtkImageActor.h>

#include <vtkImageView2D.h>
#include <vtkImageView3D.h>
#include <vtkInteractorStyleImageView2D.h>
#include <vtkInteractorStyleTrackballCamera2.h>
#include <vtkImageViewCollection.h>

#include <QVTKWidget.h>

#include <QtGui>
#include <QMenu>
#include <QMouseEvent>

// /////////////////////////////////////////////////////////////////////////////////////////////////////////
// v3dViewObserver: links a QSlider with the ImageViewPositionChangeEvent of a vtkImageView instance.
// /////////////////////////////////////////////////////////////////////////////////////////////////////////

class v3dViewObserver : public vtkCommand
{
public:
    static v3dViewObserver* New(void) { return new v3dViewObserver; }

    void Execute(vtkObject *caller, unsigned long event, void *callData);

    void SetSlider(QSlider *slider) {
        this->slider = slider;
    }

    inline void   Lock(void) { this->lock = 1; }
    inline void UnLock(void) { this->lock = 0; }

protected:
     v3dViewObserver(void);
    ~v3dViewObserver(void);

private:
    int lock;
    QSlider *slider;
};

v3dViewObserver::v3dViewObserver(void)
{
    this->slider = 0;
    this->lock = 0;
}

v3dViewObserver::~v3dViewObserver(void)
{

}

void v3dViewObserver::Execute(vtkObject *caller, unsigned long event, void *callData)
{
    vtkImageView2D* view = vtkImageView2D::SafeDownCast (caller);

    if(!view )
        return;

    if(event == vtkImageView::ImageViewPositionChangeEvent && !this->lock) {
        if(this->slider ) {
            unsigned int zslice = view->GetSlice();
            this->slider->blockSignals (true);
            this->slider->setValue (zslice);
            this->slider->blockSignals (false);
        }
    }
}

// /////////////////////////////////////////////////////////////////
// v3dViewPrivate
// /////////////////////////////////////////////////////////////////

class v3dViewPrivate
{
public:
    vtkRenderer *renderer2DAxial;
    vtkRenderer *renderer2DSagittal;
    vtkRenderer *renderer2DCoronal;
    vtkRenderer *renderer3D;
    vtkImageView2D *view2DAxial;
    vtkImageView2D *view2DSagittal;
    vtkImageView2D *view2DCoronal;
    vtkImageView3D *view3D;

    vtkImageView *currentView;
  
    vtkImageViewCollection *collection;
    v3dViewObserver *observer;

    QWidget *widget;
    QSlider *slider;
    QVTKWidget *vtkWidget;
    QMenu *menu;
    QString orientation;

    dtkAbstractData *data;
};

// /////////////////////////////////////////////////////////////////
// v3dView
// /////////////////////////////////////////////////////////////////

v3dView::v3dView(void) : dtkAbstractView(), d(new v3dViewPrivate)
{
    d->data = 0;
    d->orientation = "Axial";

    // Setting up 2D view
    
    d->renderer2DAxial = vtkRenderer::New();
    d->view2DAxial = vtkImageView2D::New();    
    d->view2DAxial->SetRenderer(d->renderer2DAxial);
    d->view2DAxial->SetBackgroundColor(0.0, 0.0, 0.0);
    d->view2DAxial->GetInteractorStyleImageView2D()->SetLeftButtonInteractionStyle(vtkInteractorStyleImageView2D::MOUSE_INTERACTION_ZOOMING);
    d->view2DAxial->GetInteractorStyleImageView2D()->SetMiddleButtonInteractionStyle(vtkInteractorStyleImageView2D::MOUSE_INTERACTION_SLICENAVIGATION);
    d->view2DAxial->GetInteractorStyleImageView2D()->SetRightButtonInteractionStyle(vtkInteractorStyleImageView2D::MOUSE_INTERACTION_NONE);
    d->view2DAxial->SetSliceOrientation(vtkImageView2D::SLICE_ORIENTATION_AXIAL);
    d->view2DAxial->CursorFollowMouseOn();
    d->view2DAxial->ImageAxisVisibilityOff();
    d->view2DAxial->ScalarBarVisibilityOff();

    
    d->renderer2DSagittal = vtkRenderer::New();
    d->view2DSagittal = vtkImageView2D::New();    
    d->view2DSagittal->SetRenderer(d->renderer2DSagittal);
    d->view2DSagittal->SetBackgroundColor(0.0, 0.0, 0.0);
    d->view2DSagittal->GetInteractorStyleImageView2D()->SetLeftButtonInteractionStyle(vtkInteractorStyleImageView2D::MOUSE_INTERACTION_ZOOMING);
    d->view2DSagittal->GetInteractorStyleImageView2D()->SetMiddleButtonInteractionStyle(vtkInteractorStyleImageView2D::MOUSE_INTERACTION_SLICENAVIGATION);
    d->view2DSagittal->GetInteractorStyleImageView2D()->SetRightButtonInteractionStyle(vtkInteractorStyleImageView2D::MOUSE_INTERACTION_NONE);
    d->view2DSagittal->SetSliceOrientation(vtkImageView2D::SLICE_ORIENTATION_SAGITTAL);
    d->view2DSagittal->CursorFollowMouseOn();
    d->view2DSagittal->ImageAxisVisibilityOff();
    d->view2DSagittal->ScalarBarVisibilityOff();
    

    d->renderer2DCoronal = vtkRenderer::New();
    d->view2DCoronal = vtkImageView2D::New();    
    d->view2DCoronal->SetRenderer(d->renderer2DCoronal);
    d->view2DCoronal->SetBackgroundColor(0.0, 0.0, 0.0);
    d->view2DCoronal->GetInteractorStyleImageView2D()->SetLeftButtonInteractionStyle(vtkInteractorStyleImageView2D::MOUSE_INTERACTION_ZOOMING);
    d->view2DCoronal->GetInteractorStyleImageView2D()->SetMiddleButtonInteractionStyle(vtkInteractorStyleImageView2D::MOUSE_INTERACTION_SLICENAVIGATION);
    d->view2DCoronal->GetInteractorStyleImageView2D()->SetRightButtonInteractionStyle(vtkInteractorStyleImageView2D::MOUSE_INTERACTION_NONE);
    d->view2DCoronal->SetSliceOrientation(vtkImageView2D::SLICE_ORIENTATION_CORONAL);
    d->view2DCoronal->CursorFollowMouseOn();
    d->view2DCoronal->ImageAxisVisibilityOff();
    d->view2DCoronal->ScalarBarVisibilityOff();

    d->currentView = d->view2DAxial;


    // Setting up 3D view
    d->renderer3D = vtkRenderer::New();
    d->view3D = vtkImageView3D::New();
    d->view3D->SetRenderer(d->renderer3D);
    d->view3D->SetBoxWidgetVisibility(0);
    d->view3D->CroppingOff();
    d->view3D->ImageAxisVisibilityOff();
    d->view3D->ScalarBarVisibilityOff();
    d->view3D->SetCroppingModeModeToOutside();
    d->view3D->ShadeOn();
    vtkInteractorStyleTrackballCamera2 *interactorStyle = vtkInteractorStyleTrackballCamera2::New();
    d->view3D->SetInteractorStyle ( interactorStyle );
    interactorStyle->Delete();

    d->widget = new QWidget;

    d->slider = new QSlider(Qt::Horizontal, d->widget);
    d->slider->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    d->slider->setFocusPolicy(Qt::NoFocus);

    d->vtkWidget = new QVTKWidget(d->widget);
    d->vtkWidget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    d->vtkWidget->setFocusPolicy(Qt::NoFocus);

    QVBoxLayout *layout = new QVBoxLayout(d->widget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->slider);
    layout->addWidget(d->vtkWidget);

    d->view3D->SetRenderWindow(d->vtkWidget->GetRenderWindow());
    //d->view3D->SetRenderWindowInteractor(d->vtkWidget->GetRenderWindow()->GetInteractor());
    d->view3D->UninitializeInteractor();
	d->vtkWidget->GetRenderWindow()->RemoveRenderer(d->renderer3D);
    
    d->view2DCoronal->SetRenderWindow(d->vtkWidget->GetRenderWindow());
    //d->view2DCoronal->SetRenderWindowInteractor(d->vtkWidget->GetRenderWindow()->GetInteractor());
	d->vtkWidget->GetRenderWindow()->RemoveRenderer(d->renderer2DCoronal);
    d->view2DCoronal->UninitializeInteractor();

    d->view2DSagittal->SetRenderWindow(d->vtkWidget->GetRenderWindow());
    //d->view2DSagittal->SetRenderWindowInteractor(d->vtkWidget->GetRenderWindow()->GetInteractor());
	d->vtkWidget->GetRenderWindow()->RemoveRenderer(d->renderer2DSagittal);
    d->view2DSagittal->UninitializeInteractor();

    d->view2DAxial->SetRenderWindow(d->vtkWidget->GetRenderWindow());
    //d->view2DAxial->SetRenderWindowInteractor(d->vtkWidget->GetRenderWindow()->GetInteractor());

    d->collection = vtkImageViewCollection::New();
    d->collection->AddItem (d->view2DAxial);
    d->collection->AddItem (d->view2DCoronal);
    d->collection->AddItem (d->view2DSagittal);
    d->collection->AddItem (d->view3D);

    d->view2DAxial->SetLinkPosition (0);
    d->view2DCoronal->SetLinkPosition (0);
    d->view2DSagittal->SetLinkPosition (0);
    d->view3D->SetLinkPosition (0);

    d->view2DAxial->SetLinkColorWindowLevel (0);
    d->view2DCoronal->SetLinkColorWindowLevel (0);
    d->view2DSagittal->SetLinkColorWindowLevel (0);
    d->view3D->SetLinkColorWindowLevel (0);

    d->view2DAxial->SetLinkCamera (0);
    d->view2DCoronal->SetLinkCamera (0);
    d->view2DSagittal->SetLinkCamera (0);
    d->view3D->SetLinkCamera (0);    

    d->observer = v3dViewObserver::New();
    d->observer->SetSlider(d->slider);
    d->view2DAxial->AddObserver(vtkImageView::ImageViewPositionChangeEvent, d->observer);

    QAction *axialAct = new QAction(tr("Axial"), d->vtkWidget);
    connect(axialAct, SIGNAL(triggered()), this, SLOT(onMenuAxialTriggered()));

    QAction *coronalAct = new QAction(tr("Coronal"), d->vtkWidget);
    connect(coronalAct, SIGNAL(triggered()), this, SLOT(onMenuCoronalTriggered()));

    QAction *sagittalAct = new QAction(tr("Sagittal"), d->vtkWidget);
    connect(sagittalAct, SIGNAL(triggered()), this, SLOT(onMenuSagittalTriggered()));

    QAction *vrAct = new QAction(tr("VR"), d->vtkWidget);
    connect(vrAct, SIGNAL(triggered()), this, SLOT(onMenu3DVRTriggered()));

    QAction *maxipAct = new QAction(tr("MIP - Max"), d->vtkWidget);
    connect(maxipAct, SIGNAL(triggered()), this, SLOT(onMenu3DMaxIPTriggered()));

    QAction *minipAct = new QAction(tr("MIP - Min"), d->vtkWidget);
    connect(minipAct, SIGNAL(triggered()), this, SLOT(onMenu3DMinIPTriggered()));
    
    QAction *mprAct = new QAction(tr("MPR"), d->vtkWidget);
    connect(mprAct, SIGNAL(triggered()), this, SLOT(onMenu3DMPRTriggered()));
    
    QAction *zoomAct = new QAction(tr("Zoom"), d->vtkWidget);
    connect(zoomAct, SIGNAL(triggered()), this, SLOT(onMenuZoomTriggered()));

    QAction *wlAct = new QAction(tr("Window / Level"), d->vtkWidget);
    connect(wlAct, SIGNAL(triggered()), this, SLOT(onMenuWindowLevelTriggered()));

    QActionGroup *group = new QActionGroup(d->vtkWidget);
    group->addAction(zoomAct);
    group->addAction(wlAct);
    wlAct->setChecked(true);
    
    d->menu = new QMenu(d->vtkWidget );
    d->menu->addAction(axialAct);
    d->menu->addAction(coronalAct);
    d->menu->addAction(sagittalAct);

    QMenu *tridMenu = d->menu->addMenu (tr ("3D"));
    tridMenu->addAction (vrAct);
    tridMenu->addAction (maxipAct);
    tridMenu->addAction (minipAct);
    tridMenu->addAction (mprAct);

    d->menu->addSeparator();
    d->menu->addAction(zoomAct);
    d->menu->addAction(wlAct);

    QStringList lut;
    lut << "Default"
	<< "Black&White"
	<< "Black&WhiteInversed"
	<< "Spectrum"
	<< "HotMetal"
	<< "GE"
	<< "Flow"
	<< "Loni"
	<< "Loni2"
	<< "Asymmetry"
	<< "PValue"
	<< "blueBlackAlpha"
	<< "greenBlackAlpha"
	<< "redBlackAlpha"
	<< "Muscles&Bones"
      	<< "Red Vessels"
      	<< "Bones"
	<< "Stern";
    
    
    this->addProperty ("Orientation",           QStringList() << "Axial" << "Sagittal" << "Coronal" << "3D");
    this->addProperty ("ScalarBarVisibility",   QStringList() << "true" << "false");
    this->addProperty ("LookupTable",           lut);
    this->addProperty ("BackgroundLookupTable", lut);
    this->addProperty ("Opacity",               QStringList() << "1.0");
    this->addProperty ("ShowAxis",              QStringList() << "true" << "false");
    this->addProperty ("LeftClickInteraction",  QStringList() << "Zooming" << "Windowing" << "Slicing" << "Measuring");
    this->addProperty ("Mode",                  QStringList() << "VR" << "MPR" << "MIP - Maximum" << "MIP - Minimum");
    this->addProperty ("Cropping",              QStringList() << "true" << "false");
    this->addProperty ("Preset",                QStringList() << "None" << "VR Muscles&Bones"
		                                              << "Vascular I" << "Vascular II" << "Vascular III" << "Vascular IV"
		                                              << "Standard" << "Soft" << "Soft on White" << "Soft on Blue"
		                                              << "Red on White" << "Glossy");

    // set default properties
    this->setProperty ("Orientation", "Axial");
    this->setProperty ("ScalarBarVisibility", "false");
    this->setProperty ("ShowAxis", "false");
    this->setProperty ("LookupTable", "Default");
    this->setProperty ("BackgroundLookupTable", "Default");
    this->setProperty ("Opacity", "1.0");
    this->setProperty ("LeftClickInteraction", "Zooming");
    this->setProperty ("Mode", "VR");
    this->setProperty ("Preset", "None");

    connect(d->vtkWidget, SIGNAL(mouseEvent(QMouseEvent*)), this, SLOT(onMousePressEvent(QMouseEvent*)));
    connect(d->slider,    SIGNAL(valueChanged(int)),        this, SLOT(onZSliderValueChanged(int)));
}

v3dView::~v3dView(void)
{
    d->vtkWidget->GetRenderWindow()->RemoveRenderer(d->renderer2DAxial);
    d->vtkWidget->GetRenderWindow()->RemoveRenderer(d->renderer2DCoronal);
    d->vtkWidget->GetRenderWindow()->RemoveRenderer(d->renderer2DSagittal);
    d->vtkWidget->GetRenderWindow()->RemoveRenderer(d->renderer3D);

    /*
      d->view2D->SetRenderWindow(0);
      d->view2D->SetRenderWindowInteractor(0);
      d->view3D->SetRenderWindow(0);
      d->view3D->SetRenderWindowInteractor(0);
    */

    d->view2DAxial->Delete();
    d->view2DSagittal->Delete();
    d->view2DCoronal->Delete();
    d->renderer2DAxial->Delete();
    d->renderer2DSagittal->Delete();
    d->renderer2DCoronal->Delete();
    d->view3D->UninitializeInteractor();
    d->view3D->Delete();
    d->renderer3D->Delete();
    d->observer->Delete();

    delete d;

    d = NULL;
}

bool v3dView::registered(void)
{
    return dtkAbstractViewFactory::instance()->registerViewType("v3dView", createV3dView);
}

QString v3dView::description(void) const
{
    return "v3dView";
}

// /////////////////////////////////////////////////////////////////
// 
// /////////////////////////////////////////////////////////////////

void v3dView::clear(void)
{
    d->collection->SyncSetInput (0); // to be tested
}

void v3dView::reset(void)
{
    if(!d->collection)
	return;
    
    d->collection->SyncReset();
}

void v3dView::update(void)
{
    if( d->currentView )
        d->currentView->Render();
    d->vtkWidget->update();
}

void v3dView::link(dtkAbstractView *other)
{
    if(!d->collection)
        return;

    if(!other)
        return;

    if(vtkImageView *view = dynamic_cast<vtkImageView*>((vtkObject*)(other->view()))) {
        d->collection->AddItem (view);
    }
}

void v3dView::unlink(dtkAbstractView *other)
{
    Q_UNUSED(other);
}

void *v3dView::view(void)
{
    return d->currentView;
}

void v3dView::setData(dtkAbstractData *data)
{
    if(!data)
        return;

    d->data = data;

    if (data->hasMetaData("PatientName")){
        const QString patientName = data->metaDataValues(tr("PatientName"))[0];	
        d->view2DAxial->SetPatientName (patientName.toAscii().constData());
		d->view2DSagittal->SetPatientName (patientName.toAscii().constData());
		d->view2DCoronal->SetPatientName (patientName.toAscii().constData());
        d->view3D->SetPatientName (patientName.toAscii().constData());
    }
    
    if( data->hasMetaData("StudyDescription")){
        const QString studyName = data->metaDataValues(tr("StudyDescription"))[0];
        d->view2DAxial->SetStudyName (studyName.toAscii().constData());
		d->view2DSagittal->SetStudyName (studyName.toAscii().constData());
		d->view2DCoronal->SetStudyName (studyName.toAscii().constData());
        d->view3D->SetStudyName (studyName.toAscii().constData());
    }
    
    if (data->hasMetaData("SeriesDescription")){
        const QString seriesName = data->metaDataValues(tr("SeriesDescription"))[0];
        d->view2DAxial->SetSeriesName (seriesName.toAscii().constData());
		d->view2DSagittal->SetSeriesName (seriesName.toAscii().constData());
		d->view2DCoronal->SetSeriesName (seriesName.toAscii().constData());
        d->view3D->SetSeriesName (seriesName.toAscii().constData());
    }

#ifdef vtkINRIA3D_USE_ITK
    if( itk::Image<char, 3>* image = dynamic_cast<itk::Image<char, 3>*>( (itk::Object*)( data->data() ) ) ) {
        d->view2DAxial->SetITKInput(image);
		d->view2DSagittal->SetITKInput(image);
		d->view2DCoronal->SetITKInput(image);
        d->view3D->SetITKInput(image);
    }
    else if( itk::Image<unsigned char, 3>* image = dynamic_cast<itk::Image<unsigned char, 3>*>( (itk::Object*)( data->data() ) ) ) {
        d->view2DAxial->SetITKInput(image);
		d->view2DSagittal->SetITKInput(image);
		d->view2DCoronal->SetITKInput(image);
        d->view3D->SetITKInput(image);
    }
    else if( itk::Image<short, 3>* image = dynamic_cast<itk::Image<short, 3>*>( (itk::Object*)( data->data() ) ) ) {
        d->view2DAxial->SetITKInput(image);
		d->view2DSagittal->SetITKInput(image);
		d->view2DCoronal->SetITKInput(image);
        d->view3D->SetITKInput(image);
    }
    else if( itk::Image<unsigned short, 3>* image = dynamic_cast<itk::Image<unsigned short, 3>*>( (itk::Object*)( data->data() ) ) ) {
        d->view2DAxial->SetITKInput(image);
		d->view2DSagittal->SetITKInput(image);
		d->view2DCoronal->SetITKInput(image);
        d->view3D->SetITKInput(image);
    }
    else if( itk::Image<int, 3>* image = dynamic_cast<itk::Image<int, 3>*>( (itk::Object*)( data->data() ) ) ) {
        d->view2DAxial->SetITKInput(image);
		d->view2DSagittal->SetITKInput(image);
		d->view2DCoronal->SetITKInput(image);
        d->view3D->SetITKInput(image);
    }
    else if( itk::Image<unsigned int, 3>* image = dynamic_cast<itk::Image<unsigned int, 3>*>( (itk::Object*)( data->data() ) ) ) {
        d->view2DAxial->SetITKInput(image);
		d->view2DSagittal->SetITKInput(image);
		d->view2DCoronal->SetITKInput(image);
        d->view3D->SetITKInput(image);
    }
    else if( itk::Image<long, 3>* image = dynamic_cast<itk::Image<long, 3>*>( (itk::Object*)( data->data() ) ) ) {
        d->view2DAxial->SetITKInput(image);
		d->view2DSagittal->SetITKInput(image);
		d->view2DCoronal->SetITKInput(image);
        d->view3D->SetITKInput(image);
    }
    else if( itk::Image<unsigned long, 3>* image = dynamic_cast<itk::Image<unsigned long, 3>*>( (itk::Object*)( data->data() ) ) ) {
        d->view2DAxial->SetITKInput(image);
		d->view2DSagittal->SetITKInput(image);
		d->view2DCoronal->SetITKInput(image);
        d->view3D->SetITKInput(image);
    }
    else if( itk::Image<float, 3>* image = dynamic_cast<itk::Image<float, 3>*>( (itk::Object*)( data->data() ) ) ) {
        d->view2DAxial->SetITKInput(image);
		d->view2DSagittal->SetITKInput(image);
		d->view2DCoronal->SetITKInput(image);
        d->view3D->SetITKInput(image);
    }
    else if( itk::Image<double, 3>* image = dynamic_cast<itk::Image<double, 3>*>( (itk::Object*)( data->data() ) ) ) {
        d->view2DAxial->SetITKInput(image);
		d->view2DSagittal->SetITKInput(image);
		d->view2DCoronal->SetITKInput(image);
        d->view3D->SetITKInput(image);
    }
    else if( itk::Image<itk::RGBPixel<unsigned char>, 3> *image = dynamic_cast<itk::Image<itk::RGBPixel<unsigned char>, 3>*>( (itk::Object*)( data->data() ) ) ) {
        d->view2DAxial->SetITKInput(image);
		d->view2DSagittal->SetITKInput(image);
		d->view2DCoronal->SetITKInput(image);
        d->view3D->SetITKInput(image);
    }
    else if( itk::Image<itk::Vector<unsigned char, 3>, 3> *image = dynamic_cast<itk::Image<itk::Vector<unsigned char, 3>, 3>*>( (itk::Object*)( data->data() ) ) ) {
        d->view2DAxial->SetITKInput(image);
		d->view2DSagittal->SetITKInput(image);
		d->view2DCoronal->SetITKInput(image);
        d->view3D->SetITKInput(image);
    }
    else
        qDebug() << "Cannot cast ITK image";
#endif


    if(vtkImageData *dataset = dynamic_cast<vtkImageData*>((vtkDataObject *)(data->data()))) {
        d->collection->SyncSetInput(dataset);
    }
    
    if(dtkAbstractDataImage* imData = dynamic_cast<dtkAbstractDataImage*>(data) )
      if( d->orientation=="Axial")
        d->slider->setRange(0, imData->zDimension()-1);
      else if( d->orientation=="Sagittal")
		d->slider->setRange(0, imData->xDimension()-1);
      else if( d->orientation=="Coronal")
		d->slider->setRange(0, imData->yDimension()-1);

    dtkAbstractView::setData(data);

    this->update();
}

void *v3dView::data (void)
{
    if (d->data)
        return d->data->output();

    return NULL;
}

QWidget *v3dView::widget(void)
{
    return d->widget;
}

void v3dView::onPropertySet(QString key, QString value)
{
    if(key == "Orientation")
	this->onOrientationPropertySet(value);

    if(key == "ScalarBarVisibility")
	this->onScalarBarVisibilityPropertySet(value);

    if(key == "LookupTable")
	this->onLookupTablePropertySet(value);

    if(key == "BackgroundLookupTable")
	this->onBackgroundLookupTablePropertySet(value);

    if(key == "Opacity")
	this->onOpacityPropertySet(value);

    if(key == "ShowAxis")
	this->onShowAxisPropertySet(value);

    if(key == "LeftClickInteraction")
	this->onLeftClickInteractionPropertySet(value);

    if(key == "Mode")
	this->onModePropertySet(value);

    if(key == "Preset")
	this->onPresetPropertySet(value);

    if(key == "Cropping")
	this->onCroppingPropertySet(value);

    this->widget()->update();
}

void v3dView::onOrientationPropertySet(QString value)
{
    if (value==d->orientation)
         return;
    
    double pos[3], window = 0.0, level = 0.0;
    if( d->currentView ) {
        d->currentView->GetCurrentPoint (pos);
        window = d->currentView->GetColorWindow();
        level = d->currentView->GetColorLevel();
    }
    
    if (value=="3D") {
        d->orientation = "3D";
        d->view2DAxial->UninitializeInteractor();
	d->view2DSagittal->UninitializeInteractor();
	d->view2DCoronal->UninitializeInteractor();
        d->vtkWidget->GetRenderWindow()->RemoveRenderer(d->renderer2DAxial);
	d->vtkWidget->GetRenderWindow()->RemoveRenderer(d->renderer2DSagittal);
	d->vtkWidget->GetRenderWindow()->RemoveRenderer(d->renderer2DCoronal);
        d->vtkWidget->GetRenderWindow()->AddRenderer(d->renderer3D);
	d->view3D->InitializeInteractor();

	d->currentView = d->view3D;	
    }
    else {
        d->view3D->UninitializeInteractor();
        d->vtkWidget->GetRenderWindow()->RemoveRenderer(d->renderer3D);        
    }
    
    
    if (value == "Axial") {
        d->view2DSagittal->UninitializeInteractor();
	d->view2DCoronal->UninitializeInteractor();
	d->vtkWidget->GetRenderWindow()->RemoveRenderer(d->renderer2DSagittal);
	d->vtkWidget->GetRenderWindow()->RemoveRenderer(d->renderer2DCoronal);
	
        d->vtkWidget->GetRenderWindow()->AddRenderer(d->renderer2DAxial);	
        d->view2DAxial->InitializeInteractor();
        d->orientation = "Axial";

	if ( dtkAbstractDataImage* imData = dynamic_cast<dtkAbstractDataImage*>(d->data) )
	    d->slider->setRange (0, imData->zDimension()-1);
	
	d->view2DSagittal->RemoveObserver(d->observer);
	d->view2DCoronal->RemoveObserver(d->observer);
	d->view2DAxial->AddObserver(vtkImageView::ImageViewPositionChangeEvent, d->observer);	
	
	d->currentView = d->view2DAxial;
    }

    if (value == "Sagittal") {
        d->view2DAxial->UninitializeInteractor();
	d->view2DCoronal->UninitializeInteractor();
	d->vtkWidget->GetRenderWindow()->RemoveRenderer(d->renderer2DAxial);
	d->vtkWidget->GetRenderWindow()->RemoveRenderer(d->renderer2DCoronal);
	
        d->vtkWidget->GetRenderWindow()->AddRenderer(d->renderer2DSagittal);
        d->view2DSagittal->InitializeInteractor();
        d->orientation = "Sagittal";

	if ( dtkAbstractDataImage* imData = dynamic_cast<dtkAbstractDataImage*>(d->data) )
	    d->slider->setRange (0, imData->xDimension()-1);

	d->view2DAxial->RemoveObserver(d->observer);
	d->view2DCoronal->RemoveObserver(d->observer);
	d->view2DSagittal->AddObserver(vtkImageView::ImageViewPositionChangeEvent, d->observer);	
	
	d->currentView = d->view2DSagittal;
    }

    if (value == "Coronal") {
        d->view2DSagittal->UninitializeInteractor();
	d->view2DAxial->UninitializeInteractor();
	d->vtkWidget->GetRenderWindow()->RemoveRenderer(d->renderer2DSagittal);
	d->vtkWidget->GetRenderWindow()->RemoveRenderer(d->renderer2DAxial);

        d->vtkWidget->GetRenderWindow()->AddRenderer(d->renderer2DCoronal);
        d->view2DCoronal->InitializeInteractor();
        d->orientation = "Coronal";

	if ( dtkAbstractDataImage* imData = dynamic_cast<dtkAbstractDataImage*>(d->data) )
            d->slider->setRange (0, imData->yDimension()-1);

	d->view2DAxial->RemoveObserver(d->observer);
	d->view2DSagittal->RemoveObserver(d->observer);
	d->view2DCoronal->AddObserver(vtkImageView::ImageViewPositionChangeEvent, d->observer);	
	
	d->currentView = d->view2DCoronal;
    }

    if (!d->currentView)
      return;

    d->currentView->SetWorldCoordinates (pos);
    d->currentView->SetColorWindow (window);
    d->currentView->SetColorLevel (level);	

    // force a correct display of the 2D axis for planar views
    d->currentView->InvokeEvent (vtkImageView::ImageViewPositionChangeEvent, NULL);
}

void v3dView::onModePropertySet (QString value)
{
  if (value=="VR") {
      d->view3D->SetRenderingModeToVR();
      d->view3D->SetVolumeRayCastFunctionToComposite();
  }

  if (value=="MPR") {
      d->view3D->SetRenderingModeToPlanar();
  }

  if (value=="MIP - Maximum") {
      d->view3D->SetRenderingModeToVR();
      d->view3D->SetVolumeRayCastFunctionToMaximumIntensityProjection();
  }

  if (value=="MIP - Minimum") {
      d->view3D->SetRenderingModeToVR();
      d->view3D->SetVolumeRayCastFunctionToMinimumIntensityProjection();
  }
  
}

void v3dView::onScalarBarVisibilityPropertySet(QString value)
{
    if (value == "true") {
      d->collection->SyncSetScalarBarVisibility(true);
    }

    if (value == "false") {
      d->collection->SyncSetScalarBarVisibility(false);
    }
    /*
    if (d->currentView)
        d->currentView->Render();
    */
}

void v3dView::onLookupTablePropertySet(QString value)
{
    if (value == "Default") {
        d->collection->SyncSetLookupTable(vtkLookupTableManager::GetBWLookupTable());
    }

    if (value == "Black&White") {
	d->collection->SyncSetLookupTable(vtkLookupTableManager::GetBWLookupTable());
    }

    if (value == "Black&WhiteInversed") {
	d->collection->SyncSetLookupTable(vtkLookupTableManager::GetBWInverseLookupTable());
    }

    if (value == "Spectrum") {
	d->collection->SyncSetLookupTable(vtkLookupTableManager::GetSpectrumLookupTable());
    }

    if (value == "HotMetal") {
	d->collection->SyncSetLookupTable(vtkLookupTableManager::GetHotMetalLookupTable());
    }

    if (value == "GE") {
	d->collection->SyncSetLookupTable(vtkLookupTableManager::GetGEColorLookupTable());
    }

    if (value == "Loni") {
	d->collection->SyncSetLookupTable(vtkLookupTableManager::GetLONILookupTable());
    }

    if (value == "Loni2") {
	d->collection->SyncSetLookupTable(vtkLookupTableManager::GetLONI2LookupTable());
    }

    if (value == "Asymmetry") {
	d->collection->SyncSetLookupTable(vtkLookupTableManager::GetAsymmetryLookupTable());
    }

    if (value == "PValue") {
	d->collection->SyncSetLookupTable(vtkLookupTableManager::GetPValueLookupTable());
    }

    if (value == "blueBlackAlpha") {
	d->collection->SyncSetLookupTable(vtkLookupTableManager::GetBlueBlackAlphaLookupTable());
    }

    if( value == "greenBlackAlpha") {
	d->collection->SyncSetLookupTable(vtkLookupTableManager::GetGreenBlackAlphaLookupTable());
    }

    if (value == "redBlackAlpha") {
	d->collection->SyncSetLookupTable(vtkLookupTableManager::GetRedBlackAlphaLookupTable());
    }

    if (value == "Muscles&Bones") {
	d->collection->SyncSetLookupTable(vtkLookupTableManager::GetVRMusclesBonesLookupTable());
    }

    if (value == "Stern") {
	d->collection->SyncSetLookupTable(vtkLookupTableManager::GetSternLookupTable());
    }

    if (value == "Red Vessels") {
	d->collection->SyncSetLookupTable(vtkLookupTableManager::GetVRRedVesselsLookupTable());
    }

    if (value == "Bones") {
	d->collection->SyncSetLookupTable(vtkLookupTableManager::GetVRBonesLookupTable());
    }
    /*
    if (d->currentView)
        d->currentView->Render();
    */
}


void v3dView::onBackgroundLookupTablePropertySet(QString value)
{
  /*
    if (value == "Default")
	d->view2D->SetBGLookupTable(vtkLookupTableManager::GetBWLookupTable());

    if (value == "Black&White")
	d->view2D->SetBGLookupTable(vtkLookupTableManager::GetBWLookupTable());

    if (value == "Black&WhiteInversed")
	d->view2D->SetBGLookupTable(vtkLookupTableManager::GetBWInverseLookupTable());

    if (value == "Spectrum")
	d->view2D->SetBGLookupTable(vtkLookupTableManager::GetSpectrumLookupTable());

    if (value == "HotMetal")
	d->view2D->SetBGLookupTable(vtkLookupTableManager::GetHotMetalLookupTable());

    if (value == "GE")
	d->view2D->SetBGLookupTable(vtkLookupTableManager::GetGEColorLookupTable());

    if (value == "Loni")
	d->view2D->SetBGLookupTable(vtkLookupTableManager::GetLONILookupTable());

    if (value == "Loni2")
	d->view2D->SetBGLookupTable(vtkLookupTableManager::GetLONI2LookupTable());

    if (value == "Asymmetry")
	d->view2D->SetBGLookupTable(vtkLookupTableManager::GetAsymmetryLookupTable());

    if (value == "PValue")
	d->view2D->SetBGLookupTable(vtkLookupTableManager::GetPValueLookupTable());

    if (value == "blueBlackAlpha")
	d->view2D->SetBGLookupTable(vtkLookupTableManager::GetBlueBlackAlphaLookupTable());

    if( value == "greenBlackAlpha")
	d->view2D->SetBGLookupTable(vtkLookupTableManager::GetGreenBlackAlphaLookupTable());

    if (value == "redBlackAlpha")
	d->view2D->SetBGLookupTable(vtkLookupTableManager::GetRedBlackAlphaLookupTable());
  */
}


void v3dView::onOpacityPropertySet(QString value)
{
    bool ok; double opacity = value.toDouble(&ok);

    if (ok) {
	d->view2DAxial->GetImageActor()->SetOpacity(opacity);
	d->view2DSagittal->GetImageActor()->SetOpacity(opacity);
	d->view2DCoronal->GetImageActor()->SetOpacity(opacity);
    }
    else
	qDebug("Error: cannot convert QString value to a double");
    /*
    if (d->currentView)
        d->currentView->Render();
    */
}

void v3dView::onShowAxisPropertySet(QString value)
{
    if (value == "true")
	d->collection->SyncSetImageAxisVisibility(1);

    if (value == "false")
	d->collection->SyncSetImageAxisVisibility(0);

    /*
    if (d->currentView)
        d->currentView->Render();
    */
}

void v3dView::onLeftClickInteractionPropertySet(QString value)
{
    if (value == "Zooming") {
        d->view2DAxial->GetInteractorStyleImageView2D()->SetLeftButtonInteractionStyle(vtkInteractorStyleImageView2D::MOUSE_INTERACTION_ZOOMING);
	d->view2DSagittal->GetInteractorStyleImageView2D()->SetLeftButtonInteractionStyle(vtkInteractorStyleImageView2D::MOUSE_INTERACTION_ZOOMING);
	d->view2DCoronal->GetInteractorStyleImageView2D()->SetLeftButtonInteractionStyle(vtkInteractorStyleImageView2D::MOUSE_INTERACTION_ZOOMING);
    }

    if (value == "Windowing") {
        d->view2DAxial->GetInteractorStyleImageView2D()->SetLeftButtonInteractionStyle(vtkInteractorStyleImageView2D::MOUSE_INTERACTION_WINDOWING);
	d->view2DSagittal->GetInteractorStyleImageView2D()->SetLeftButtonInteractionStyle(vtkInteractorStyleImageView2D::MOUSE_INTERACTION_WINDOWING);
	d->view2DCoronal->GetInteractorStyleImageView2D()->SetLeftButtonInteractionStyle(vtkInteractorStyleImageView2D::MOUSE_INTERACTION_WINDOWING);
    }

    if (value == "Slicing") {
        d->view2DAxial->GetInteractorStyleImageView2D()->SetLeftButtonInteractionStyle(vtkInteractorStyleImageView2D::MOUSE_INTERACTION_SLICENAVIGATION);
	d->view2DSagittal->GetInteractorStyleImageView2D()->SetLeftButtonInteractionStyle(vtkInteractorStyleImageView2D::MOUSE_INTERACTION_SLICENAVIGATION);
	d->view2DCoronal->GetInteractorStyleImageView2D()->SetLeftButtonInteractionStyle(vtkInteractorStyleImageView2D::MOUSE_INTERACTION_SLICENAVIGATION);
    }

    if (value == "Measuring") {
      /*
        d->view2DAxial->GetInteractorStyleImageView2D()->SetLeftButtonInteractionStyle(vtkInteractorStyleImageView2D::MOUSE_INTERACTION_WINDOWING);
	d->view2DSagittal->GetInteractorStyleImageView2D()->SetLeftButtonInteractionStyle(vtkInteractorStyleImageView2D::MOUSE_INTERACTION_WINDOWING);
	d->view2DCoronal->GetInteractorStyleImageView2D()->SetLeftButtonInteractionStyle(vtkInteractorStyleImageView2D::MOUSE_INTERACTION_WINDOWING);
      */
    }
}

void v3dView::onPresetPropertySet (QString value)
{
  //double white[3] = {1.0,1.0,1.0};
    
    if( value == "VR Muscles&Bones" ) {

      this->setProperty ("LookupTable", "Muscles&Bones");

      double color[3] = {0.0, 0.0, 0.0};

      if ( d->currentView ) {
	d->currentView->SetBackgroundColor( color );
	d->currentView->SetColorWindow (337.0);
	d->currentView->SetColorLevel (1237.0);
	//d->collection->SyncSetTextColor ( white );
	//d->collection->SyncSetAboutData ("VR Muscles - Bones - Powered by magic Pedro");
      }
    }

    if( value == "Vascular I" ) {

      this->setProperty ("LookupTable", "Stern");

      double color[3] = {0.0, 0.0, 0.0};
      
      d->collection->SyncSetBackgroundColor( color );
      d->collection->SyncSetColorWindow (388.8);
      d->collection->SyncSetColorLevel (362.9);
      //d->collection->SyncSetTextColor ( white );
      //d->view->SetAboutData ("Vascular - Powered by magic Pedro");
    }

    if( value == "Vascular II" ) {

      this->setProperty ("LookupTable", "Red Vessels");

      double color[3] = {0.0, 0.0, 0.0};
      
      d->collection->SyncSetBackgroundColor( color );
      d->collection->SyncSetColorWindow (189.6);
      d->collection->SyncSetColorLevel (262.3);

      //d->collection->SyncSetTextColor ( white );
      //d->view->SetAboutData ("Vascular II - Powered by magic Pedro");
    }

    if( value == "Vascular III" ) {

      this->setProperty ("LookupTable", "Red Vessels");

      double color[3] = {0.0, 0.0, 0.0};
      
      d->collection->SyncSetBackgroundColor( color );
      d->collection->SyncSetColorWindow (284.4);
      d->collection->SyncSetColorLevel (341.7);
      //d->collection->SyncSetTextColor ( white );
      //d->view->SetAboutData ("Vascular III - Powered by magic Pedro");
    }
    
    if( value == "Vascular IV" ) {

      this->setProperty ("LookupTable", "Red Vessels");

      double color[3] = {0.0, 0.0, 0.0};
      
      d->collection->SyncSetBackgroundColor( color );
      d->collection->SyncSetColorWindow (272.5);
      d->collection->SyncSetColorLevel (310.9);
      //d->collection->SyncSetTextColor ( white );
      //d->view->SetAboutData ("Vascular IV - Powered by magic Pedro");
    }

    if( value == "Standard" ) {

      this->setProperty ("LookupTable", "Muscles&Bones");

      double color[3] = {0.0, 0.0, 0.0};
      
      d->collection->SyncSetBackgroundColor( color );
      d->collection->SyncSetColorWindow (243.7);
      d->collection->SyncSetColorLevel (199.6);
      //d->collection->SyncSetTextColor ( white );
      //d->view->SetAboutData ("Standard - Powered by magic Pedro");
    }

    if( value == "Soft" ) {

      this->setProperty ("LookupTable", "Bones");

      double color[3] = {0.0, 0.0, 0.0};
      
      d->collection->SyncSetBackgroundColor( color );
      d->collection->SyncSetColorWindow (133.5);
      d->collection->SyncSetColorLevel (163.4);
      //d->collection->SyncSetTextColor ( white );
      //d->view->SetAboutData ("Soft - Powered by magic Pedro");
    }

    if( value == "Soft on White" ) {

      this->setProperty ("LookupTable", "Muscles&Bones");

      double color[3] = {1.0,0.98820477724075317,0.98814374208450317};
      
      d->collection->SyncSetBackgroundColor( color );
      d->collection->SyncSetColorWindow (449.3);
      d->collection->SyncSetColorLevel (372.8);
      //d->view->SetAboutData ("Soft on White - Powered by magic Pedro");
    }

    if( value == "Soft on Blue" ) {

      this->setProperty ("LookupTable", "Muscles&Bones");

      double color[3]={0.0, 0.27507439255714417, 0.26398107409477234};      
      
      d->collection->SyncSetBackgroundColor( color );
      d->collection->SyncSetColorWindow (449.3);
      d->collection->SyncSetColorLevel (372.8);
      //d->collection->SetAboutData ("Soft on Blue - Powered by magic Pedro");
    }

    if( value == "Red on White" ) {

      this->setProperty ("LookupTable", "Red Vessels");

      double color[3]={1.0, 0.98820477724075317, 0.98814374208450317};
	
      d->collection->SyncSetBackgroundColor( color );
      d->collection->SyncSetColorWindow (449.3);
      d->collection->SyncSetColorLevel (372.8);
      //d->view->SetAboutData ("Red on White - Powered by magic Pedro");
    }

    if( value == "Glossy" ) {

      this->setProperty ("LookupTable", "Bones");

      double color[3] = {0.0, 0.0, 0.0};
      
      d->collection->SyncSetBackgroundColor( color );
      d->collection->SyncSetColorWindow (133.5);
      d->collection->SyncSetColorLevel (163.4);
      //d->collection->SyncSetTextColor ( white );
      //d->view->SetAboutData ("Glossy - Powered by magic Pedro");
    }

}

void v3dView::onCroppingPropertySet (QString value)
{
    if ( value=="true" ) {
        d->view3D->SetCropping ( 1 );
	d->view3D->SetBoxWidgetVisibility ( 1 );
    }
    else {
      d->view3D->SetCropping ( 0 );
      d->view3D->SetBoxWidgetVisibility ( 0 );
    }
}

void v3dView::onMousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton) {
        d->menu->popup (event->globalPos());
    }
}

void v3dView::onZSliderValueChanged (int value)
{
    if (d->orientation=="3D" || !d->currentView)
        return;

    d->observer->Lock();
    if( vtkImageView2D *view = vtkImageView2D::SafeDownCast(d->currentView) ) {
        view->SetSlice (value);
        d->currentView->Render();
    }
    d->observer->UnLock();
}

void v3dView::onMenuAxialTriggered (void)
{
    this->setProperty("Orientation", "Axial");
    d->view2DAxial->Render();
}


void v3dView::onMenuCoronalTriggered (void)
{
    this->setProperty("Orientation", "Coronal");
    d->view2DCoronal->Render();
}


void v3dView::onMenuSagittalTriggered (void)
{
    this->setProperty("Orientation", "Sagittal");
    d->view2DSagittal->Render();
}

void v3dView::onMenu3DVRTriggered (void)
{
    this->setProperty ("Orientation", "3D");
    this->setProperty ("Mode", "VR");    
    d->view3D->Render();
}

void v3dView::onMenu3DMPRTriggered (void)
{    
    this->setProperty("Orientation", "3D");
    this->setProperty("Mode", "MPR");
    d->view3D->Render();
}

void v3dView::onMenu3DMaxIPTriggered (void)
{
    this->setProperty("Orientation", "3D");
    this->setProperty("Mode", "MIP - Maximum");
    d->view3D->Render();
}

void v3dView::onMenu3DMinIPTriggered (void)
{
    this->setProperty("Orientation", "3D");
    this->setProperty("Mode", "MIP - Minimum");
    d->view3D->Render();
}

void v3dView::onMenuZoomTriggered (void)
{
    this->setProperty ("LeftClickInteraction", "Zooming");
    //this->onPropertySet("LeftClickInteraction", "Zooming");
}

void v3dView::onMenuWindowLevelTriggered (void)
{
    this->setProperty ("LeftClickInteraction", "Windowing");
    //this->onPropertySet("LeftClickInteraction", "Windowing");
}

// /////////////////////////////////////////////////////////////////
// Type instanciation
// /////////////////////////////////////////////////////////////////

dtkAbstractView *createV3dView(void)
{
    return new v3dView;
}
