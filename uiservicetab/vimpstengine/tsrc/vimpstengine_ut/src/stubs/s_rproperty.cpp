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
* Description: s_rproperty.cpp
*
*/
#include <e32property.h>
#include <e32base.h>


TInt gKey = KErrNotFound;
TInt gValue = KErrNotFound; 
TRequestStatus gRequest = 0;
/*
	IMPORT_C static TInt Define(TUid aCategory, TUint aKey, TInt aAttr, TInt aPreallocate=0);
	IMPORT_C static TInt Define(TUid aCategory, TUint aKey, TInt aAttr, const TSecurityPolicy& aReadPolicy, const TSecurityPolicy& aWritePolicy, TInt aPreallocated=0);
	IMPORT_C static TInt Define(TUint aKey, TInt aAttr, const TSecurityPolicy& aReadPolicy, const TSecurityPolicy& aWritePolicy, TInt aPreallocated=0);
	IMPORT_C static TInt Delete(TUid aCategory, TUint aKey);
	IMPORT_C static TInt Delete(TUint aKey);
	IMPORT_C static TInt Get(TUid aCategory, TUint aKey, TDes8& aValue);
#ifndef __KERNEL_MODE__
	IMPORT_C static TInt Get(TUid aCategory, TUint aKey, TDes16& aValue);
#endif
	IMPORT_C static TInt Set(TUid aCategory, TUint aKey, TInt aValue);
	IMPORT_C static TInt Set(TUid aCategory, TUint aKey, const TDesC8& aValue);
#ifndef __KERNEL_MODE__
	IMPORT_C static TInt Set(TUid aCategory, TUint aKey, const TDesC16& aValue);
#endif

	IMPORT_C void Cancel();

	IMPORT_C TInt Get(TInt& aValue);
	IMPORT_C TInt Get(TDes8& aValue);
#ifndef __KERNEL_MODE__
	IMPORT_C TInt Get(TDes16& aValue);
#endif
	IMPORT_C TInt Set(TInt aValue);
	IMPORT_C TInt Set(const TDesC8& aValue);
#ifndef __KERNEL_MODE__
	IMPORT_C TInt Set(const TDesC16& aValue);
*/

TInt RProperty::Attach(TUid /*aCategory*/, TUint /*aKey*/, TOwnerType /*aType*/ )
    {
    return KErrNone;
    }

TInt RProperty::Set(TUid /*aCategory*/, TUint aKey, TInt aValue)
    {
    gKey = aKey;
    gValue = aValue;

    return KErrNone;
    }

TInt RProperty::Get( TInt& aValue )
    {
    aValue = gValue;

    return KErrNone;
    }

TInt RProperty::Get(TUid /*aCategory*/, TUint aKey, TInt& aValue)
    {
    if( aKey == gKey )
        {
        aValue = gValue;
        return KErrNone;
        }

    return KErrNotFound;
    }
           
void RProperty::Subscribe(TRequestStatus& /*aRequest*/)
    {
    }

void RProperty::Cancel()
    {
    }

TInt RProperty::Define(TUid /*aCategory*/, TUint /*aKey*/, TInt /*aAttr*/, TInt /*aPreallocate=0*/)
	{
	return KErrNone;	
	}
	
void RHandleBase::Close()
{
	
}

