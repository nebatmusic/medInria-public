/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medSegmentationAbstractToolBox.h>

#include <msegPluginExport.h>

#include <medAbstractData.h>

#include <medDataIndex.h>
#include <medViewEventFilter.h>
#include <medImageMaskAnnotationData.h>

#include <QVector3D>
#include <QTextEdit>

#include <vector>

#include <itkImage.h>
#include <itkImageRegionIterator.h>

class medAbstractData;
class medAbstractImageView;
class medAnnotationData;

class dtkAbstractProcessFactory;
class medSeedPointAnnotationData;
class ClickAndMoveEventFilter;

struct PaintState {
    enum E{ None, Wand, Stroke, DeleteStroke };
};

typedef itk::Image <unsigned char, 3>              MaskType;
typedef itk::Image <float, 3>                      MaskFloatType;
typedef itk::ImageRegionIterator <MaskType>        MaskIterator;
typedef itk::ImageRegionIterator <MaskFloatType>   MaskFloatIterator;

typedef itk::Image <unsigned char, 2>              Mask2dType;
typedef itk::ImageRegionIterator<Mask2dType>       Mask2dIterator;
typedef itk::Image <float, 2>                      Mask2dFloatType;
typedef itk::ImageRegionIterator <Mask2dFloatType> Mask2dFloatIterator;

//! Segmentation toolbox to allow manual painting of pixels
class MEDVIEWSEGMENTATIONPLUGIN_EXPORT AlgorithmPaintToolbox : public medSegmentationAbstractToolBox
{
    Q_OBJECT
    MED_TOOLBOX_INTERFACE("Paint Segmentation", "Paint Tool",
                          <<"segmentation")
public:
    
    typedef QPair<Mask2dType::Pointer,unsigned int> SlicePair;
    typedef QPair<QList<SlicePair>,unsigned char> PairListSlicePlaneId;

    AlgorithmPaintToolbox( QWidget *parent );
    ~AlgorithmPaintToolbox();

    inline void setPaintState( PaintState::E value){m_paintState = value;}
    inline PaintState::E paintState(){return m_paintState;}

    bool getSeedPlanted();
    void setSeedPlanted(bool,MaskType::IndexType,unsigned int,double);
    void setSeed(QVector3D);

    inline bool getCursorOn(){return cursorOn;};
    void setCursorOn(bool value);
    inline void setCurrentIdSlice(unsigned int id){currentIdSlice = id;};
    inline unsigned int getCurrentIdSlice(){return currentIdSlice;};
    inline void setCurrentPlaneIndex(unsigned int index){currentPlaneIndex = index;};
    inline unsigned int getCurrentPlaneIndex(){return currentPlaneIndex;};
    void setParameter(int channel, int value);
    void setCurrentView(medAbstractImageView* view);

    bool isData(Mask2dType::Pointer input,unsigned char label);
    Mask2dType::Pointer extract2DImageSlice(MaskType::Pointer input,int plane,int slice,MaskType::SizeType size,MaskType::IndexType start);
    Mask2dFloatType::Pointer computeDistanceMap(Mask2dType::Pointer img);
    void computeIntermediateSlice(Mask2dFloatType::Pointer distanceMapImg0,Mask2dFloatType::Pointer distanceMapImg1,int slice0,
                                                              int slice1,int j,MaskFloatIterator ito,MaskIterator itMask,double *vec);
    void computeCentroid(Mask2dIterator itmask,unsigned int *coord);
    Mask2dType::Pointer translateImageByVec(Mask2dType::Pointer img,int *vec);

    dtkPlugin* plugin();

    medAbstractData* processOutput();

public slots:
    void activateStroke();
    void activateMagicWand();
    void updateMagicWandComputationSpeed();

    void import();
    void clearMask();

    void setLabel(int newVal);
    void setLabelColor();

    void synchronizeWandSpinBoxesAndSliders(void);
    void updateMagicWandComputation();

    void updateStroke(ClickAndMoveEventFilter * filter, medAbstractImageView * view);
    void updateWandRegion(medAbstractImageView * view, QVector3D &vec);
    void updateMouseInteraction();

    void undo();
    void redo();
    void addSliceToStack(medAbstractView * view,const unsigned char planeIndex,QList<int> listIdSlice);
    //void saveCurrentStateForCursor(medAbstractView * view,const unsigned char planeIndex,unsigned int idSlice);
    void onViewClosed();

    void newSeed();
    void removeSeed();

    void copySliceMask();
    void pasteSliceMask();

    void increaseBrushSize();
    void reduceBrushSize();

    void interpolate();

protected:
    friend class ClickAndMoveEventFilter;

    void addStroke( medAbstractImageView *view, const QVector3D &vec );
    void setData( medAbstractData *data );

    // update with seed point data.
    void updateTableRow(int row);

    void initializeMaskData( medAbstractData * imageData, medAbstractData * maskData );

    void setOutputMetadata(const medAbstractData * inputData, medAbstractData * outputData);

    void updateFromGuiItems();

    void showButtons( bool value);

    void generateLabelColorMap(unsigned int numLabels);

    void updateButtons();

    void addBrushSize(int size);

    char computePlaneIndex(const QVector3D &,MaskType::IndexType & ,bool& isInside);

    void copySliceFromMask3D(itk::Image<unsigned char,2>::Pointer copy,const char planeIndex,const char * direction,const unsigned int slice);
    void pasteSliceToMask3D(itk::Image<unsigned char,2>::Pointer image2D,const char planeIndex,const char * direction,const unsigned int slice);

    void removeCursorDisplay();

signals:
    void installEventFilterRequest(medViewEventFilter *filter);

private:
    typedef dtkSmartPointer<medSeedPointAnnotationData> SeedPoint;

    // Stroke's objects
    QPushButton *m_strokeButton;
    QSlider *m_brushSizeSlider;
    QSpinBox *m_brushSizeSpinBox;
    QLabel *m_brushRadiusLabel;
    QShortcut *addBrushSize_shortcut, *reduceBrushSize_shortcut;
    double m_strokeRadius;
    //

    // Magic Wand's objects
    // The slider works on percentages of a linear scale between min and max values, i.e.
    // wandradius = (max - min) * sliderPerc / 2.0
    QPushButton *m_magicWandButton;
    QFormLayout * magicWandLayout;
    QLabel * m_wandUpperThresholdLabel, *m_wandLowerThresholdLabel;
    QSlider *m_wandUpperThresholdSlider, *m_wandLowerThresholdSlider;
    QDoubleSpinBox *m_wandUpperThresholdSpinBox , * m_wandLowerThresholdSpinBox;
    QPushButton * m_removeSeedButton;
    QShortcut *removeSeed_shortcut;
    QCheckBox *m_wand3DCheckbox, *m_wand3DRealTime;
    QLabel * m_wandInfo;
    QTime wandTimer;
    
    bool seedPlanted;
    bool initiateRegionGrowth;
    QVector3D m_seed;
    double m_wandRadius, m_wandUpperThreshold, m_wandLowerThreshold;
    //

    // Common objects
    QPushButton *m_interpolateButton;
    QPushButton *m_labelColorWidget;
    QSpinBox *m_strokeLabelSpinBox;
    QLabel *m_colorLabel;
    QShortcut *undo_shortcut, *redo_shortcut, *copy_shortcut, *paste_shortcut;
    unsigned int m_strokeLabel;
    //

    double m_MinValueImage;
    double m_MaxValueImage;

    QPushButton *m_applyButton;

    QPushButton *m_clearMaskButton;

    dtkSmartPointer< ClickAndMoveEventFilter > m_viewFilter;

    dtkSmartPointer<medImageMaskAnnotationData> m_maskAnnotationData;
    //TODO smartPointing have to be managed only in abstraction -rde

    dtkSmartPointer<medAbstractData> m_maskData;
    medAbstractData* m_imageData;

    medImageMaskAnnotationData::ColorMapType m_labelColorMap;

    MaskType::Pointer m_itkMask;

    QPair<Mask2dType::Pointer,char> m_copy;
    
    // undo_redo_feature's attributes
    QHash<medAbstractView*,QStack<PairListSlicePlaneId>*> * m_undoStacks,*m_redoStacks;
    medAbstractImageView * currentView;
    medAbstractImageView * viewCopied;

    bool cursorOn;
    QList<QPair<MaskType::IndexType,unsigned char> > * cursorPixels;
    unsigned int currentPlaneIndex; //plane Index of the current/last operation
    unsigned int currentIdSlice; // current slice;
    bool undoRedoCopyPasteModeOn;
    bool cursorJustReactivated;

    template <typename IMAGE> void RunConnectedFilter (MaskType::IndexType &index, unsigned int planeIndex);
    template <typename IMAGE> void GenerateMinMaxValuesFromImage ();

    QVector3D m_lastVup;
    QVector3D m_lastVpn;
    double m_sampleSpacing[2];

    PaintState::E m_paintState;
};
  

