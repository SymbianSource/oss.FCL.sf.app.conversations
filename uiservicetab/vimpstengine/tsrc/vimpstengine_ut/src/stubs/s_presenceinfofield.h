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
* Description: s_presenceinfofield.h
*
*/
#ifndef MXIMPFWPRESENCEINFOFIELDSTUB_H
#define MXIMPFWPRESENCEINFOFIELDSTUB_H

#include <presenceinfofield.h>

class MPresenceInfoFieldStub : public MPresenceInfoField
    {
    const TDesC8& FieldType() const;
    
    void SetFieldTypeL( const TDesC8& aFieldType );

    const MXIMPBase& FieldValue() const;

    MXIMPBase& FieldValue();
    
    void SetFieldValue( MXIMPBase* aFieldValue );
    
    const TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps ) const;

    TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps );

    TInt32 GetInterfaceId() const;
    };



#endif //MXIMPFWPRESENCEINFOFIELD_H




