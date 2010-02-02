/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  CCHUI Event Handler
*
*/

#ifndef MVIMPSTENGINECCHUIEVENTOBSERVER_H_
#define MVIMPSTENGINECCHUIEVENTOBSERVER_H_

// CLASS DECLARATION
    
/**
 *  MVIMPSTEngineChangeConnectionEventObserver declaration.
 *  @since S60 5.0
 */
class MVIMPSTEngineCchUiEventObserver
    {
    
public:
    /**
     * Handles the change connection event from CCHUI
     *@ return void
     */
    virtual void HandleChangeConnectionEventL() = 0;
    
    };


#endif /* MVIMPSTENGINECCHUIEVENTOBSERVER_H_ */
