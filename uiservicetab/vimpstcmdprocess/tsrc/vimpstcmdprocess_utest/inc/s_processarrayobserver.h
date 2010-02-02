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

#ifndef S_MVIMPSTPROCESSARRAYOBSERVER_H
#define S_MVIMPSTPROCESSARRAYOBSERVER_H

#include "mvimpstprocessArrayobserver.h"

//  INCLUDES
#include <e32std.h>
#include <e32base.h>



class vimpstprocessarrayobserver_stub : public MVIMPSTProcessArrayObserver
    {
    public :
        static MVIMPSTProcessArrayObserver* NewL();
         void HandleAdditionL(TVIMPSTEnums::TItem type, 
                                            TInt aIndex);

         void HandleDeletionL(TVIMPSTEnums::TItem type, TInt aIndex);
         void HandleAvatarChangeL( const TDesC& aUserId );
    };



#endif // MVIMPSTPROCESSARRAYOBSERVER_H



// End of File
