/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  s_enginecontmgtextfeatures.cpp
*
*/

// INCLUDES
#include "s_MVPbkContactLinkstub.h"

MVPbkContactLink* CVPbkContactLinkStub::NewL()
    {
    MVPbkContactLink* list = new(ELeave)CVPbkContactLinkStub;
        return list;
    }

/**
 * Returns the contact store which this link belongs to.
 * @return Contact store associated with this link.
 */
MVPbkContactStore& CVPbkContactLinkStub::ContactStore() const 
{
MVPbkContactStore* ret=NULL;
return *ret;	
}

/**
 * Returns ETrue if this link refers to the same contact than
 * aOther, EFalse otherwise.
 * @param aOther Contact to check equality for.
 * @return ETrue if this link refers to the same contact than
 *         aOther, EFalse otherwise.
 */
TBool CVPbkContactLinkStub::IsSame(const MVPbkContactLink& aOther) const 
{
if (this == &aOther)
return ETrue;

return EFalse;
}

/**
 * Checks if this link refers to the contact aContact.
 * @param aContact Contact to check.
 * @return ETrue if this link refers to aContact, EFalse otherwise.
 */
TBool CVPbkContactLinkStub::RefersTo(const MVPbkBaseContact& /*aContact*/) const 
{
return ETrue;	
}

/**
 * Returns persistent streaming interface for this object, or NULL
 * if persistent streaming is not supported.
 * @return Persistent streaming object or NULL if not supported.
 */
const MVPbkStreamable* CVPbkContactLinkStub::Streamable() const 
{
return NULL;	
};

/**
 * Returns a packing interface for this link.
 * @see CVPbkContactLinkArray
 * @internal
 * @return Link packing object.
 */
const MVPbkContactLinkPacking& CVPbkContactLinkStub::Packing() const 
{
MVPbkContactLinkPacking* ret = NULL;
return *ret;	
};

/**
 * Returns a clone of this contact link.
 * @return Contact link copy.
 */
MVPbkContactLink* CVPbkContactLinkStub::CloneLC() const 
{
MVPbkContactLink* link = CVPbkContactLinkStub::NewL();
    return link;	
}


        
    
// End of File
