/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Friends-array decorator
*
*/



#ifndef _CVIMPSTUIICONFILEPROVIDER_H
#define _CVIMPSTUIICONFILEPROVIDER_H

#include <e32base.h>

#include <aknsitemid.h>
#include <akniconutils.h>

#include <AknIconArray.h>
#include <BADESCA.H>

 
 class CVIMPSTUiIconFileProvider : public CBase, public MAknIconFileProvider
    {
    public:
        /**
        * Constructor
        * @param aFile File handle to use
        */
        CVIMPSTUiIconFileProvider( RFile& aFile );
    public:

        /**
        * Destructor.
        */
        virtual ~CVIMPSTUiIconFileProvider();
       
       
        // from base class MAknIconFileProvider
        
        /**
        * Returns an open file handle to the icon file.
        * This method should leave if an icon file with specified type does
        * not exist. That may be the case e.g. with MBM file,
        * if there are no bitmap icons.
        *
        * Note! RFs::ShareProtected must be called to the RFs instance used
        * for opening the file.
        *
        * @param aFile Icon file should be opened in this file handle, which
        * is an empty file handle, when the AknIcon framework calls this method.
        * The AknIcon framework takes care of closing the file handle after
        * having used it.
        * @param aType Icon file type.
        */
        virtual void RetrieveIconFileHandleL(
            RFile& aFile, const TIconFileType /*aType*/ );
           
        /**
        * With this method, AknIcon framework informs that it does not use
        * this MAknIconFileProvider instance any more. After this call,
        * it is ok to delete the object. This can be implemented simply
        * e.g. by deleting self in this callback.
        * Normally, this callback is invoked when the icon in question
        * is deleted.
        * Note, however, that if the same MAknIconFileProvider instance is
        * supplied in multiple CreateIcon calls, then it must be accessible
        * by AknIcon framework until it has signalled a matching amount
        * of these callbacks.
        */
        virtual void Finished();
      
    private:
        /**
         * File to use.
         * Own.
         */
        RFile iFile;         
    };//_CVIMPSTUIICONFILEPROVIDER_H

