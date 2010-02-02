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
* Description:  Interface for identity object.
*
*/


#include "s_ximpidentity.h"

MyMXIMPIdentityStub::MyMXIMPIdentityStub()
	{
	}
MyMXIMPIdentityStub::~MyMXIMPIdentityStub()
	{
	if(iIdentity)
	    {
	    delete iIdentity;
	    }
	}

const TDesC16& MyMXIMPIdentityStub::Identity() const 
	{
	return iIdentity->Des();
	}

void MyMXIMPIdentityStub::SetIdentityL(
            const TDesC16& aIdentity ) 
	{
	if ( iIdentity )
	    {
	    iIdentity->ReAlloc( aIdentity.Length() ); 
	    }
	else
	    {
	    iIdentity = HBufC::NewL( aIdentity.Length() );
	    }
	
    iIdentity->Des().Copy( aIdentity );
	
	}


const TAny* MyMXIMPIdentityStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ ) const
                    {
                    return this;
                    }

TAny* MyMXIMPIdentityStub::GetInterface(
                    TInt32 /*aInterfaceId*/,
                    TIfGetOps /*aOps*/ )
{
return this;
}

TInt32 MyMXIMPIdentityStub::GetInterfaceId() const
{
return KErrNone;	
}
