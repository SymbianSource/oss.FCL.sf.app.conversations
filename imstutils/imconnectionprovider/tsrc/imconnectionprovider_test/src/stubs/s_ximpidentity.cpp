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
	delete iIdentity;	
	}



const TDesC16& MyMXIMPIdentityStub::Identity() const 
	{
	return *iIdentity;
	}





void MyMXIMPIdentityStub::SetIdentityL(
            const TDesC16& aIdentity ) 
	{
	if (iIdentity)
	    {
	    delete iIdentity;
	    iIdentity = NULL;
	    }
	iIdentity = aIdentity.AllocL();
	}


const TAny* MyMXIMPIdentityStub::GetInterface(
                    TInt32 aInterfaceId,
                    TIfGetOps aOps ) const
                    {
                    	
                    }

TAny* MyMXIMPIdentityStub::GetInterface(
                    TInt32 aInterfaceId,
                    TIfGetOps aOps )
{
	
}

TInt32 MyMXIMPIdentityStub::GetInterfaceId() const
{
	
}
