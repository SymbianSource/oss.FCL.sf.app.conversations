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

#ifndef S_VIMPSTCMDOBSERVER_H
#define S_VIMPSTCMDOBSERVER_H

#include "mvimpstcmdobserver.h"

//  INCLUDES
#include <e32std.h>
#include <e32base.h>



class vimpstcmdobserver_stub : public MVIMPSTCmdObserver
	{
	public :
        void CommandFinishedL(const MVIMPSTCmd& aCommand ) ;

        void HandleCommandEventL(TVIMPSTEnums::TVIMPSTRegistrationState aState, TInt aServiceError) ;
	};

#endif      // S_VIMPSTCMDOBSERVER_H

// End of File
