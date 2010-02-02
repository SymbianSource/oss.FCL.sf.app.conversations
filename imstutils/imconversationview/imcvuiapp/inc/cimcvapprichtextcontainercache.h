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
* Description:  Cache to Store Rich Text Containers
*
*/


#ifndef _CIMCVAPPRICHTEXTCONTAINERCACHE_H
#define _CIMCVAPPRICHTEXTCONTAINERCACHE_H

//  INCLUDES
#include "cimcvapprichtextcontainer.h"
#include  <e32base.h>

//forward declaration
class CIMCVAppRichTextContainer;

/**
*  Rich text container Cache
*  @since Series 60 5.0
*/
class CIMCVAppRichTextContainerCache : public CBase
    {
public:
    
    /**
     * NewL()
     */
    static CIMCVAppRichTextContainerCache* NewL();
    
    /**
     * Destructor
     */
    ~CIMCVAppRichTextContainerCache();
    
private:
    
    /**
     * Constructor
     */
    CIMCVAppRichTextContainerCache();

    
public:


	void UpdateSkinTextColorL  ();
	
    /**
     * StoreContainer - stores the container to the cache
     * @param aPrimaryKey - Primary key used for storage
     * @param aContainer - CIMCVAppRichTextContainer object to be stored
     */
    void StoreContainer( CIMCVAppRichTextContainer *aContainer);
    
    /**
     * LoadContainer - Returns the container from the cache
     * @param aPrimaryKey - Primary key used for Searching
     * @return - the object if found else returns NULL
     */
    CIMCVAppRichTextContainer* LoadContainer(TInt aServiceId, const TDesC& aPrimaryKey);
    
    /**
     * DeleteContainer - Deletes the container from the cache
     * @param aPrimaryKey - Primary key used for Searching
     * @ aServiceId, aservice id
     */
    void DeleteContainer(const TDesC& aPrimaryKey, TInt aServiceId );
    
    /**
     * DeleteAllServiceContainer - Deletes the container from the cache which has aServiceId
     * @param aServiceId - Primary key used for Searching
     */
    void DeleteAllServiceContainer(TInt aServiceId );
    
private:
    
    // Stores the CIMCVAppRichTextContainer objects
    // Takes the ownership of objects stored
    RPointerArray<CIMCVAppRichTextContainer> iRTContainerCache;

    };

#endif // _CIMCVAPPRICHTEXTCONTAINERCACHE_H

// End of File
