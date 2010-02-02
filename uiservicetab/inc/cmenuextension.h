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
* Description: cmenuextension.h
 *
*/

#ifndef __CMENUEXTENSION_H__
#define __CMENUEXTENSION_H__


// INCLUDES
#include <e32base.h>
#include <ecom/ecom.h>

//  FORWARD DECLARATIONS
class CEikMenuPane;
class MCommandMapper;
class MVIMPSTUiAIWControl;


//CONSTANTS
const TUid KMenuCustomisationInterfaceUid = { 0x2002139E };

// CLASS DECLARATION

/**
 * Abstract interface for menuextensions. extension plugins  can
 * extend service tab menu  by implementing a class derived from this.
 */
class CMenuExtension : public CBase
    {
    
    public:
        
       static CMenuExtension* CreateImplementationL( TUid aUid );
       
       virtual ~CMenuExtension();
       
    public: // Interface
        /**
         * Filters the menu pane of the plugin connected to this.
         *
         * @param aPreviousId Menu command id after which the plugins menu
         *  should come.
         * @param aMenuPane Menu pane which will be filtered.
         * @param aServiceId service id.
         * @param aMenuExtManager, MenuExtention manager of service tab
         */
        virtual void DynInitMenuPaneL
            (TInt aPreviousId, CEikMenuPane& aMenuPane,TUint aServiceId, MVIMPSTUiAIWControl& aMenuExtManager) = 0;
            
        /**
         * Handles service tab extension commands.
         *
         * @param aCommandId Command ID.
         *
         * @return ETrue if command was handled, otherwise EFalse.
         */
        virtual TBool HandleCommandL(const TInt& aCommand  ) = 0;
        
        /**
         * Registers command ID mapper. This method is called automatically
         * by Servicetab menu extneions  at initialization phase.
         *
         * @param aMapper Command ID mapper
         */  
        virtual void RegisterCommandMapper( MCommandMapper& aMapper ) = 0;
              
        /**
         * Gets STM_COMMAND_INFO_ARRAY resource. Resource should be loaded.
         *
         * @return Resource ID or KErrNotFound
         */        
        virtual TInt CommandInfoResource() const = 0;  

               
    private:
           /**
            * Instance identifier key. When instance of an implementation is
            * created by ECOM framework, the framework will assign a UID for
            * it. The UID is used in the destructor to notify framework that
            * this instance is being destroyed and resources can be released.
            * Plugin implementations should leave this untouched, as the inline
            * destructor and constructor method take care of ECom releasing.
            */
           TUid iDtor_ID_Key; 
                               
  
    };
inline CMenuExtension* CMenuExtension::CreateImplementationL( TUid aUid )
    {
    TAny* ptr = REComSession::CreateImplementationL( aUid, _FOFF( CMenuExtension, iDtor_ID_Key ) );
    CMenuExtension* impl = static_cast<CMenuExtension*> (ptr);
    return impl;
    }

inline CMenuExtension::~CMenuExtension()
    {
    REComSession::DestroyedImplementation( iDtor_ID_Key );
    }

#endif // __MXSPVIEW_H__
