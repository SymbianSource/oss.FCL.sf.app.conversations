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
* Description: s_presenceinfofieldvaluetext.h
*
*/
#ifndef MXIMPFWPRESENCEINFOFIELDVALUETEXTSTUB_H
#define MXIMPFWPRESENCEINFOFIELDVALUETEXTSTUB_H

#include <presenceinfofieldvaluetext.h>

class MPresenceInfoFieldValueTextStub : public MPresenceInfoFieldValueText
    {
    TInt MaxLength() const;
    
    TPtrC TextValue() const;
    
    void SetTextValueL( const TDesC& aValue );

    const TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps ) const;

    TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps );

    TInt32 GetInterfaceId() const;

    };


#endif //MXIMPFWPRESENCEINFOFIELDVALUETEXT_H



