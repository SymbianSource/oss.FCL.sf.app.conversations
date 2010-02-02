/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: s_presenceinfofieldcollection.h
*
*/
#ifndef MXIMPFWPRESENCEINFOFIELDCOLLECTIONSTUB_H
#define MXIMPFWPRESENCEINFOFIELDCOLLECTIONSTUB_H

#include <presenceinfofieldcollection.h>

class MPresenceInfoFieldCollectionStub : public MPresenceInfoFieldCollection
    {
    TInt FieldCount() const;

    const MPresenceInfoField& FieldAt( TInt aIndex ) const;

    void LookupFieldByFieldType( MPresenceInfoField*& aPresenceInfoField,
                                 const TDesC8& aMatch ) const;

    virtual TInt DeleteFieldByFieldType(const TDesC8& aMatch );

    void AddOrReplaceFieldL( MPresenceInfoField* aPresenceInfoField );
    
    const TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps ) const;

    TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps );

    TInt32 GetInterfaceId() const;
    };



#endif //MXIMPFWPRESENCEINFOFIELDCOLLECTION_H





