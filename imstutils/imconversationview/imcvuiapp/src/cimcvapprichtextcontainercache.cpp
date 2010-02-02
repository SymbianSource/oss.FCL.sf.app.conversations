/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  A class which show message to ui 
*
*/

// INCLUDE FILES
#include "cimcvapprichtextcontainercache.h"

// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainerCache::NewL
// -----------------------------------------------------------------------------
//
CIMCVAppRichTextContainerCache* CIMCVAppRichTextContainerCache::NewL()
    {
    CIMCVAppRichTextContainerCache* self = new( ELeave) CIMCVAppRichTextContainerCache();
    return self;
    }

// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainerCache::CIMCVAppRichTextContainerCache
// Default constructor
// -----------------------------------------------------------------------------
//
CIMCVAppRichTextContainerCache::CIMCVAppRichTextContainerCache()
    {
    
    }

// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainerCache::~CIMCVAppRichTextContainerCache
// Destructor
// -----------------------------------------------------------------------------
//
CIMCVAppRichTextContainerCache::~CIMCVAppRichTextContainerCache()
    {
    //destroy all the objects in the cache
    iRTContainerCache.ResetAndDestroy();

    }

// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainerCache::UpdateSkinTextColorL 
// This is called when the skin text color is changed, and it is applied to all the containers.
// -----------------------------------------------------------------------------
//
void CIMCVAppRichTextContainerCache::UpdateSkinTextColorL ()
{
	TInt count = iRTContainerCache.Count();
	 for(TInt i=0;i<count;i++)
		 {
		 iRTContainerCache [i]->UpdateSkinnedTextColourL();
		 }
}


// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainerCache::StoreContainer
// Used to store the container to the cache
// -----------------------------------------------------------------------------
//
void CIMCVAppRichTextContainerCache::StoreContainer(CIMCVAppRichTextContainer *aContainer)
    {
    if( aContainer )
        {
        TInt count = iRTContainerCache.Count();
        for(TInt i=0;i<count;i++)
            {
            if( ( aContainer->ServiceId() == iRTContainerCache[i]->ServiceId() ) && 
                    ( KErrNone == iRTContainerCache[i]->GetPrimaryKey().Compare( aContainer->GetPrimaryKey() ) ) )
                {
                //if found, no need to store
                return;
                }
            }
        //container is not in the cache so append it
        iRTContainerCache.Append(aContainer);
        }
     }

// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainerCache::LoadContainer
// Used to find the container in the cache
// -----------------------------------------------------------------------------
//
CIMCVAppRichTextContainer* CIMCVAppRichTextContainerCache::LoadContainer(TInt aServiceId, const TDesC& aPrimaryKey)
    {
    TInt count = iRTContainerCache.Count();
    for(TInt i=0;i<count;i++)
        {
        if( ( aServiceId == iRTContainerCache[i]->ServiceId() ) && 
             ( KErrNone == iRTContainerCache[i]->GetPrimaryKey().Compare(aPrimaryKey) ) )
            {
            //if found returns the pointer to the container
            return iRTContainerCache[i];
            }
        }
    return NULL; //if not found return NULL
    }

// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainerCache::DeleteContainer
// Used to delete the container in the cache
// -----------------------------------------------------------------------------
//
void CIMCVAppRichTextContainerCache::DeleteContainer(const TDesC& aPrimaryKey, TInt aServiceId )
    {
    TInt count = iRTContainerCache.Count();
    for(TInt i=0;i<count;i++)
        {
        if( ( aServiceId == iRTContainerCache[i]->ServiceId() ) && 
                ( KErrNone == iRTContainerCache[i]->GetPrimaryKey().Compare(aPrimaryKey)  ) )
            {
            //if found delete the container and remove the element from the array
            delete iRTContainerCache[i];
            iRTContainerCache.Remove(i);
            break;
            }
        }
    }
// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainerCache::DeleteAllServiceContainer
// Used to delete the container in the cache
// -----------------------------------------------------------------------------
//
void CIMCVAppRichTextContainerCache::DeleteAllServiceContainer(TInt aServiceId )
    {
    for(TInt index=0; index<iRTContainerCache.Count(); index++)
        {
        if( aServiceId == iRTContainerCache[index]->ServiceId() )
            {
            //if found delete the container and remove the element from the array
            delete iRTContainerCache[index];
            iRTContainerCache.Remove(index);
            iRTContainerCache.Compress();
            index=index-1;// start from same index again
            }
        }
    }

// end of file


