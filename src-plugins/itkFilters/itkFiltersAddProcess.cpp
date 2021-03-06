/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "itkFiltersAddProcess.h"

#include <dtkCore/dtkAbstractProcessFactory.h>

#include <itkAddImageFilter.h>
#include <itkImage.h>

#include <medUtilitiesITK.h>

class itkFiltersAddProcessPrivate
{
public:
    double addValue;
};

const double itkFiltersAddProcess::defaultAddValue = 100.0;

//-------------------------------------------------------------------------------------------

itkFiltersAddProcess::itkFiltersAddProcess(itkFiltersAddProcess *parent) 
    : itkFiltersProcessBase(parent), d(new itkFiltersAddProcessPrivate)
{  
    d->addValue = defaultAddValue;
}

itkFiltersAddProcess::itkFiltersAddProcess(const itkFiltersAddProcess& other)
     : itkFiltersProcessBase(other), d(other.d)
{
}

//-------------------------------------------------------------------------------------------

itkFiltersAddProcess::~itkFiltersAddProcess( void )
{
    delete d;
}

//-------------------------------------------------------------------------------------------

bool itkFiltersAddProcess::registered( void )
{
    return dtkAbstractProcessFactory::instance()->registerProcessType("itkAddProcess", createitkFiltersAddProcess);
}

//-------------------------------------------------------------------------------------------

QString itkFiltersAddProcess::description() const
{
    return tr("ITK add constant filter");
}

//-------------------------------------------------------------------------------------------

void itkFiltersAddProcess::setParameter(double data, int channel)
{
    if (channel != 0)
        return;
    
    d->addValue = data;
}

//-------------------------------------------------------------------------------------------

int itkFiltersAddProcess::tryUpdate()
{
    int res = DTK_FAILURE;

    if ( getInputData() )
    {
        res = DISPATCH_ON_3D_PIXEL_TYPE(&itkFiltersAddProcess::updateProcess, this, getInputData());
    }

    return res;
}

template <class ImageType>
int itkFiltersAddProcess::updateProcess(medAbstractData* inputData)
{
    typename ImageType::Pointer inputImage = static_cast<ImageType*>(inputData->data());

    typedef itk::AddImageFilter<ImageType, itk::Image<double, ImageType::ImageDimension>, ImageType> AddFilterType;
    typename AddFilterType::Pointer addFilter = AddFilterType::New();

    addFilter->SetInput(inputImage);
    addFilter->SetConstant ( d->addValue );

    itk::CStyleCommand::Pointer callback = itk::CStyleCommand::New();
    callback->SetClientData ( ( void * ) this );
    callback->SetCallback ( itkFiltersProcessBase::eventCallback );
    addFilter->AddObserver ( itk::ProgressEvent(), callback );

    addFilter->Update();

    getOutputData()->setData ( addFilter->GetOutput() );

    QString newSeriesDescription = "add filter " + QString::number(d->addValue);
    medUtilities::setDerivedMetaData(getOutputData(), inputData, newSeriesDescription);

    return DTK_SUCCEED;
}

// /////////////////////////////////////////////////////////////////
// Type instanciation
// /////////////////////////////////////////////////////////////////

dtkAbstractProcess * createitkFiltersAddProcess ( void )
{
    return new itkFiltersAddProcess;
}
