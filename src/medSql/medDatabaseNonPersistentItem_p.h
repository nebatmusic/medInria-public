/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medDataIndex.h>

#include <dtkCore/dtkAbstractData.h>
#include <dtkCore/dtkSmartPointer.h>

#include <QtCore/QString>
#include <QtGui/QImage>

class dtkAbstractData;

class medDatabaseNonPersistentItemPrivate
{
public:
    QString name;
    QString birthdate;
    QString patientId;
    QString studyName;
    QString studyId;
    QString studyUid;
    QString seriesName;
    QString seriesId;
    QString seriesUid;
    QString file;
    QImage thumb;

    medDataIndex index;

    dtkSmartPointer<dtkAbstractData> data;
};


