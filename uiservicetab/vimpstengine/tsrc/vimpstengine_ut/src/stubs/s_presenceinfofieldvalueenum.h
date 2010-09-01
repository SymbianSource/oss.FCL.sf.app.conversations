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
* Description: s_presenceinfofieldvalueenum.h
*
*/
#ifndef MXIMPFWPRESENCEINFOFIELDVALUEENUMSTUB_H
#define MXIMPFWPRESENCEINFOFIELDVALUEENUMSTUB_H

#include <presenceinfofieldvalueenum.h>

class MPresenceInfoFieldValueEnumStub : public MPresenceInfoFieldValueEnum
    {
    TInt MinValue() const;

    TInt MaxValue() const;

    TInt Value() const;

    void SetValueL( TInt aValue );

    void SetLimits( TInt aMin, TInt aMax );

    const TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps ) const;

    TAny* GetInterface(
                        TInt32 aInterfaceId,
                        TIfGetOps aOps );

    TInt32 GetInterfaceId() const;

    };

#endif //MXIMPFWPRESENCEINFOFIELDVALUEENUM_H



