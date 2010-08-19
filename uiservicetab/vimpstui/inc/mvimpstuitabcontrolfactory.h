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
* Description:  Factory implementation  of the tabbed view controls
*
*/


#include "cvimpstuidoublelistboxtabviewcontrol.h"  // container with double line listbox
#include "cvimpstuisinglelistboxtabviewcontrol.h" // container with single line listbox
#include "mvimpstengine.h"


class MVIMPSTUiTabbedViewControlFactory
    {
public:
    
    /**
     * creates the MVIMPSTUiTabbedViewControl implementations based on the 
     * presence availability of the service : OWNERSHIP TO THE CALLER
     *  
     * it checks from aEngine.IsPresenceEnabled() ; the presence availability
     *  then it creates the corresponding container
     * 
     * @param aTabbedView reference to view.
     * @param aKeyEventHandler Key event handler to be used
     * @param aCommandHandler reference to commandhandler
     * @param aServiceId Id of the service
     * @param aBrandHandler reference to brandhandler
     * @param aEngine reference to engine
     *
     * @return MVIMPSTUiTabbedViewControl object based on the presence availability
     * : OWNERSHIP TO THE CALLER
     */
    static  MVIMPSTUiTabbedViewControl* NewTabControlL( CVIMPSTUiTabbedView& aTabbedView,
                                                        MPbk2KeyEventHandler* aKeyEventHandler, 
                                                        MVIMPSTCmdHandler& aCommandHandler,
                                                        TUint32 aServiceId,
                                                        CVIMPSTUiBrandData& aBrandHandler,
                                                        MVIMPSTEngine& aEngine,
                                                        CEikButtonGroupContainer* aCba)
                                                        
        {
        // LOGIC : IF presence is enabled for that service then 
        // it creates  double listbox control 
        // if presencxe is NOT Enabled creates single listbox control
        if( aEngine.SubService( TVIMPSTEnums::EPresence) )
            {
            return CVIMPSTUiDoubleListBoxTabViewControl::NewL(  aTabbedView,
                    aKeyEventHandler,
                    aCommandHandler,
                    aServiceId, 
                    aBrandHandler ,
                    aEngine,
                    aCba);

            }
        else
            {
            return CVIMPSTUiSingleListBoxTabViewControl::NewL(  aTabbedView,
                    aKeyEventHandler,
                    aCommandHandler,
                    aServiceId, 
                    aBrandHandler ,
                    aEngine,
                    aCba);
            }
        }
    
    };
