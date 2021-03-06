/*=========================================================================
 medInria
 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.
=========================================================================*/

#include <itkFiltersBinaryCloseProcess.h>
#include <dtkCore/dtkAbstractProcessFactory.h>

//-------------------------------------------------------------------------------------------

itkFiltersBinaryCloseProcess::itkFiltersBinaryCloseProcess(itkFiltersBinaryCloseProcess *parent)
    : itkMorphologicalFiltersProcessBase(parent)
{
}

bool itkFiltersBinaryCloseProcess::registered( void )
{
    return dtkAbstractProcessFactory::instance()->registerProcessType("itkBinaryCloseProcess", createitkFiltersBinaryCloseProcess);
}

QString itkFiltersBinaryCloseProcess::description() const
{
    return tr("Binary Close filter");
}

// /////////////////////////////////////////////////////////////////
// Type instantiation
// /////////////////////////////////////////////////////////////////

dtkAbstractProcess *createitkFiltersBinaryCloseProcess ( void )
{
    return new itkFiltersBinaryCloseProcess;
}
