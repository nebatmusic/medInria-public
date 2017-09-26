/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <itkMorphologicalFiltersProcessBase.h>

class ITKFILTERSPLUGIN_EXPORT itkFiltersErodeProcess : public itkMorphologicalFiltersProcessBase
{
    Q_OBJECT
    
public:
    itkFiltersErodeProcess(itkFiltersErodeProcess * parent = 0);
    itkFiltersErodeProcess(const itkFiltersErodeProcess& other);
    static bool registered ( void );
};

dtkAbstractProcess * createitkFiltersErodeProcess(void);
