/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include "itkFiltersProcessBase.h"

class itkFiltersBinaryThresholdingProcessPrivate;
class medAbstractData;

class ITKFILTERSPLUGIN_EXPORT itkFiltersBinaryThresholdingProcess : public itkFiltersProcessBase
{
    Q_OBJECT
    
public:

    static const double defaultInsideValue;

    itkFiltersBinaryThresholdingProcess(itkFiltersBinaryThresholdingProcess * parent = 0);
    itkFiltersBinaryThresholdingProcess(const itkFiltersBinaryThresholdingProcess& other);
    virtual ~itkFiltersBinaryThresholdingProcess(void);
    static bool registered ( void );

    virtual QString description(void) const;
    
public slots:

    void setParameter (int data, int channel);
    void setParameter ( double  data, int channel );
    int tryUpdate();

protected:
    template <class ImageType> int updateProcess(medAbstractData* inputData);

private:
    itkFiltersBinaryThresholdingProcessPrivate *d;
};

dtkAbstractProcess * createitkFiltersBinaryThresholdingProcess(void);


