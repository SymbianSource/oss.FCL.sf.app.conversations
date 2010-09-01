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


#include "s_mccaconnection.h"


class MCCAParameter;
class MCCAObserver;


MCCAConnection* MCCAConnectionStub::NewL()
    {
    MCCAConnection* parameter = new(ELeave)MCCAConnectionStub;
        return parameter;
    }

void MCCAConnectionStub::Close() 
    {
    
    }
    /**
     * DEPRECATED - USE LaunchAppL() instead!
     * Ownership of MCCAParameter is NOT transferred.
     */
     void MCCAConnectionStub::LaunchApplicationL( MCCAParameter& aParameter )
         {
         
         }

   

     void MCCAConnectionStub::LaunchAppL(
        MCCAParameter& aParameter,
        MCCAObserver* aObserver  )
         {
         
         }
    // End of File
    
