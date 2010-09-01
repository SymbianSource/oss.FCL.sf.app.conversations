/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  
:                
*
*/



#include <TVPbkContactStoreUriPtr.h>

/**
 * Constructs a URI pointer to aStoreUri.
 *
 * @param aStoreUri A reference to the contact store URI.
 */
EXPORT_C TVPbkContactStoreUriPtr::TVPbkContactStoreUriPtr(const TDesC& /*aStoreUri*/){ }

/**
 * Constructor.
 */
EXPORT_C TVPbkContactStoreUriPtr::TVPbkContactStoreUriPtr(){ }


/**
 * Returns a descriptor that holds the whole Uri.
 *
 * @return A descriptor that holds the whole Uri.
 */
EXPORT_C const TDesC& TVPbkContactStoreUriPtr::UriDes() const{ return KNullDesC(); }

/**
 * Compares this URI's component to aUri's component.
 *
 * @param aUri The URI whose component is compared.
 * @param aComponent Defines the component that are compared.
 * @return Zero if the URIs are the same.
 */
EXPORT_C TInt TVPbkContactStoreUriPtr::Compare(const TVPbkContactStoreUriPtr& /*aUri*/, 
        TVPbkContactStoreUriComponent /*aComponent*/) const{ return 0; }

/**
 * Compares this URI's component to aUriComponent.
 *
 * @param aUriComponent A descriptor that contains the component data.
 * @param aComponent Defines the component of this URI that is compared
 *                   to aUriComponent.
 * @return Zero if components matched.
 */
EXPORT_C TInt TVPbkContactStoreUriPtr::Compare(const TDesC& /*aUriComponent*/, 
        TVPbkContactStoreUriComponent /*aComponent*/) const{ return 0; }

/**
 * Returns a pointer to the aComponent part of URI.
 *
 * @param aComponent Defines the component that is returned.
 * @return a pointer to the aComponent part of URI.
 */
EXPORT_C const TPtrC TVPbkContactStoreUriPtr::Component(
        TVPbkContactStoreUriComponent /*aComponent*/) const{ return KNullDesC(); }

/**
 * Sets this URI pointer to point to the same URI as aUri.
 *
 * @param aUri The URI that will be pointed to.
 */
EXPORT_C void TVPbkContactStoreUriPtr::Set(const TVPbkContactStoreUriPtr& /*aUri*/){ }

/**
 * Returns the length of the URI.
 *
 * @return The length of the URI.
 */
EXPORT_C TInt TVPbkContactStoreUriPtr::Length() const{ return 0; }


EXPORT_C void TVPbkContactStoreUriPtr::ExternalizeL(RWriteStream& /*aStream*/) const{ }
EXPORT_C TInt TVPbkContactStoreUriPtr::ExternalizedSize() const{ return 0; }




//End of file
