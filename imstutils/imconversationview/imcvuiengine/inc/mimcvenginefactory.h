/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  service engine factory class interface
*
*/

#ifndef MIMCVENGINEFACTORY_H
#define MIMCVENGINEFACTORY_H
#include <e32def.h>

//FORWARD DECLARATION
class CIMCVEngine;
class MIMCVEngineConnectionHandler;
class MIMCVEngineOpenChatsObserver;
class MIMCVEngineServiceChangeObserver;
//CLASS  DECLARATION
/**
 *  
 *  This class list the M-Class for Engine factory
 *
 *  @lib vimpstengine.lib
 *  @since S60 5.0
 */
class MIMCVEngineFactory
    {

public:	

    /**
     * return the engine instance for aServiceId.
     */
    virtual CIMCVEngine& GetServiceEngineL( TInt aServiceId ) = 0 ;

    /**
     * find and release the engine instance for aServiceId.
     */
    virtual void ReleaseServiceEngine( TInt aServiceId ) = 0 ;


    /**
     * GetDisplayNameL: returns the Identification       
     * @param aIndex - Index of the item for which Identification 
     *                 is required
     * @return TPtrC - Identification
     */
    virtual TPtrC GetDisplayNameL(TInt aIndex) const = 0;


    /**
     * GetTargetIdL: returns the Conversation Party ID      
     * @param aIndex - Index of the item 
     * @return Conversation Party ID
     */
    virtual const TDesC& GetTargetIdL(TInt aIndex) const = 0;


    /**
     * Count: Returns the number of items              
     * @return TInt - Total number of openchats item
     */
    virtual TInt Count() const = 0;


    /**
     * SetActiveItemL: Sets the item as the current active one       
     * @param aIndex - Index of the item
     * @return void
     */      
    virtual void SetActiveItemL( const TInt aIndex ) = 0;


    /**
     * GetActiveItem: Gets the current active conversation        
     * @return Active Conversation Party Id
     */
    virtual const TDesC& GetActiveItemL() const = 0;

    /**
     * Returns the pending messages status of the contact item 
     * @param aIndex - Index of the item
     * @return bool: true if the conversation item has pending messages
     * else return false
     */      
    virtual TBool IsMsgPendingL( const TInt aIndex ) const = 0;

    /**
     * Find the index of the item in the Conversation List 
     * @param aItem - Conversation Party Id      
     * @return aIndex - Index of the item        
     */
    virtual TInt FindIndexL(const TDesC& aItem) = 0;

    virtual void ResetL() = 0;
    
    /**
     * @see MVIMPSTEngineFactory
     */
    virtual void RegisterOpenChatObserver(MIMCVEngineOpenChatsObserver* aChatObserver) = 0 ;
    /**
     * @see MVIMPSTEngineFactory
     */
    virtual void UnRegisterOpenChatObserver() = 0 ;
    
       
    virtual TInt GetActiveServiceId() = 0 ;
	/*
	* return all availabe service engines
	*/
    virtual RPointerArray<CIMCVEngine>&  GetServiceEngines() = 0;
    /**
     * @see MVIMPSTEngineFactory
     */
    virtual void RegisterServiceObserver(MIMCVEngineServiceChangeObserver* aServiceObserver) = 0 ;
    /**
     * @see MVIMPSTEngineFactory
     */
    virtual void UnRegisterServiceObserver() = 0 ;
    
    /**
     * ~CVIMPSTEngineFactory 
     * dont call delete 
     */
    virtual ~MIMCVEngineFactory() 
        {

        }

    };

#endif //MIMCVENGINEFACTORY_H
