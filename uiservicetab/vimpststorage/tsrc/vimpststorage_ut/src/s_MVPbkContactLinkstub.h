/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef CVPBKCONTACTLINKSTUB_H
#define CVPBKCONTACTLINKSTUB_H

// INCLUDES
#include <MVPbkContactLink.h>


// CLASS DECLARATIONS

/**
 * Virtual Phonebook Contact link.
 * An object that uniquely identifies a single Contact and its ContactStore.
 */
class CVPbkContactLinkStub : public CBase, public MVPbkContactLink
    {
    public: // destructor
        /**
         * Destructor.
         */
        ~CVPbkContactLinkStub();

		CVPbkContactLinkStub();
    public: // interface
        
        /**
         * Returns the contact store which this link belongs to.
         * @return Contact store associated with this link.
         */
        MVPbkContactStore& ContactStore() const ;

        /**
         * Returns ETrue if this link refers to the same contact than
         * aOther, EFalse otherwise.
         * @param aOther Contact to check equality for.
         * @return ETrue if this link refers to the same contact than
         *         aOther, EFalse otherwise.
         */
        TBool IsSame(const MVPbkContactLink& aOther) const ;
        
        /**
         * Checks if this link refers to the contact aContact.
         * @param aContact Contact to check.
         * @return ETrue if this link refers to aContact, EFalse otherwise.
         */
        TBool RefersTo(const MVPbkBaseContact& aContact) const ;
        
        /**
         * Returns persistent streaming interface for this object, or NULL
         * if persistent streaming is not supported.
         * @return Persistent streaming object or NULL if not supported.
         */
        const MVPbkStreamable* Streamable() const ;

        /**
         * Returns a packing interface for this link.
         * @see CVPbkContactLinkArray
         * @internal
         * @return Link packing object.
         */
        const MVPbkContactLinkPacking& Packing() const ;
        
        /**
         * Returns a clone of this contact link.
         * @return Contact link copy.
         */
        MVPbkContactLink* CloneLC() const ;

        
    };



#endif // CVPBKCONTACTLINKSTUB_H

// End of File
