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

#include "s_processarrayobserver.h"

MVIMPSTProcessArrayObserver* vimpstprocessarrayobserver_stub::NewL()
    {
  MVIMPSTProcessArrayObserver* ob = new(ELeave)vimpstprocessarrayobserver_stub;
        return ob;
    }

void vimpstprocessarrayobserver_stub::HandleAdditionL(TVIMPSTEnums::TItem type,  TInt aIndex)
{
	
}

void vimpstprocessarrayobserver_stub::HandleDeletionL(TVIMPSTEnums::TItem type, TInt aIndex)
{
	
}
void vimpstprocessarrayobserver_stub:: HandleAvatarChangeL( const TDesC& aUserId )
{
    
}
        

// End of File
