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
* Description: s_presenceinfofieldcollection.cpp
*
*/
#include "s_presenceinfofieldcollection.h"

TInt MPresenceInfoFieldCollectionStub::FieldCount() const
    {
    return KErrNone;
    }

const MPresenceInfoField& MPresenceInfoFieldCollectionStub::FieldAt( TInt /*aIndex*/ ) const
    {
    MPresenceInfoField* field = NULL;
    return *field;
    }

void MPresenceInfoFieldCollectionStub::LookupFieldByFieldType( 
        MPresenceInfoField*& /*aPresenceInfoField*/,
                             const TDesC8& /*aMatch*/ ) const
    {
    }

TInt MPresenceInfoFieldCollectionStub::DeleteFieldByFieldType(const TDesC8& /*aMatch*/ )
    {
    return KErrNone;
    }


void MPresenceInfoFieldCollectionStub::AddOrReplaceFieldL( MPresenceInfoField* /*aPresenceInfoField*/ )
    {
    }

const TAny* MPresenceInfoFieldCollectionStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ ) const
    {
    return NULL;
    }

TAny* MPresenceInfoFieldCollectionStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ )
    {
    return NULL;
    }

TInt32 MPresenceInfoFieldCollectionStub::GetInterfaceId() const
    {
    return 0;
    }





    
