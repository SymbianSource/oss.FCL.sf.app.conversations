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

#include "mvimpstengine.h"
#include "mvimpststoragecontactsobserver.h"
#include "mvimpstenginecchuieventobserver.h"

class TVIMPSTEnums;
class MVIMPSTEngineExtentionFeatures;

class vimpstengine_stub : public MVIMPSTEngine
{
public:		

    /**
            * See MVIMPSTEngine
            * 
            */
            TUint32 ServiceId() const  ;        


            /**
            * See MVIMPSTEngine
            * 
            */
            TVIMPSTEnums::TVIMPSTRegistrationState ServiceState() const ;


            /**
            * See MVIMPSTEngine
            * 
            */
            TInt GetBrandInfoL(TLanguage& aBrandLanguage, 
                        TInt& aBrandVersion, TDes8& aBrandId) const ;



            /**
            * See MVIMPSTEngine
            * 
            */          
            TBool IsSubServiceSupportedL(TVIMPSTEnums::SubServiceType aType) const ;
                        

            /**
            * See MVIMPSTEngine
            * 
            */
            TBool IsSubServiceEnabled(TVIMPSTEnums::SubServiceType aType) const ;

            /**
            * See MVIMPSTEngine
            * 
            */
            void ContactStoreIdL( 
                        TDes& aContactStoreId ) const ;

            /**
            * See MVIMPSTEngine
            * 
            */
            const TDesC& ServiceName() const ;
            
        
            /**
            * See MVIMPSTEngine
            * 
            */
             void RegisterServiceSessionObserverL
                                (MVIMPSTEngineServiceStateEventObserver* aObserver) ;
             
            /**
            * See MVIMPSTEngine
            * 
            */
            void UnRegisterServiceSessionObserver
                                (MVIMPSTEngineServiceStateEventObserver* aObserver) ;
               

            /**
            * See MVIMPSTEngine
            * 
            */  
            void IntializeStorageL() ;

            /**
            * See MVIMPSTEngine
            * 
            */  
            void UnIntializeStorage() ;         


            /**
            * See MVIMPSTEngine
            * 
            */
            MVIMPSTEngineSubService* SubService(TVIMPSTEnums::SubServiceType aType) const;


            /**
            * See MVIMPSTEngine
            * 
            */
            MVIMPSTEngineExtentionFeatures* ExtentionFeatures(TVIMPSTEnums::ExtentionType aType) const ;


        
            /**
            * See MVIMPSTEngine
            * 
            */
            HBufC* GetOwnUserIdFromCChOrStorageL() const;
           

        
            /**
            * See MVIMPSTEngine
            * 
            */
            TInt Login();

            /**
            * See MVIMPSTEngine
            * 
            */
            void LogoutL();
            
            /**
            * See MVIMPSTEngine
            * 
            */
            void DefaultDomainNameL( TDes& aDefaultDomainName ) const;
            
            /**
            * See MVIMPSTEngine
            * 
            */
            TInt ChangeConnectionL();
            /**
             * See MVIMPSTEngine
             * 
             * @return True if password is present in the settings else returns false
             *
             */
            TBool IsPasswordAvailableL();
            
            /**
             * See MVIMPSTEngine
             * 
            */
            TBool IsBlockSupportedL();
            
            /**
             * See MVIMPSTEngine
             * 
            */
            void DeleteDataBaseL();
            
            /**
             * See MVIMPSTEngine
             * 
            */
            MVIMPSTEnginePresenceSubService* GetPreseceSubService();

            /**
             * See MVIMPSTEngine
             * 
             */
            void FetchBlockedListL();
            
            /**
             * See MVIMPSTEngine
             * 
             */
            void RegisterBlockedListObserver(
                            MVIMPSTEngineBlockedListFetchEventObserver* aOb);
                            
                            
            /**
            *  See MVIMPSTEngine
            * 
            */
            void SetOwnUserIdIfChangedL(const TDesC& aUserId ) ;
            
            
            /**
            *  See MVIMPSTEngine
            * 
            */
            void RetriveContextIfCChEnabledL() ;

        private: // From MVIMPSTEngineServiceConnectionEventObserver
            /**
             * See MVIMPSTEngineServiceConnectionEventObserver
             */ 
            void  HandleServceConnectionEventL();
        
        private:
            /**
            * Returns a SubService is Supported or not
            * Components intertested in knowing whether a SubService is Supported or not can use this API
            * @param aType - Type of the SubService TVIMPSTEnums::SubServiceType - Defined in tvimpstenums.h
            * @return TBool, ETrue if the SubService is Supported in this Service, else returns EFalse
            */
            TBool IsSubServiceSupportedInternal(TVIMPSTEnums::SubServiceType aType) const ;
                
        public:     
             /**
              * Called when all the contacts are fetched from the virtual store.
              * 
              * @since s60 5.0
              */
             void HandleContactFetchedL();
                  
        public: // From MVIMPSTStorageContactsObserver
           
           
            /**
            * HandleChange: To handle the Chnage event from the MCAStoredContactsObserver
            * @see MCAStoredContactsObserver
            * @param aList: reference to the list at which a change has occured
            * @param aContact:  reference to the contact at which a change has occured
            * @param aEventType: TVIMPSTEnums::TCAObserverEventType,type of change event that has occured 
            * @param aUserIdChanged, ETrue if userid has changed, else EFalse.
            * @return void
            */
             void HandleStorageChangeL( TVIMPSTEnums::TVIMPSTStorgaeEventType aEventType,
                                        MVIMPSTStorageContactList* aList, 
                                        MVIMPSTStorageContact* aContact,
                                        TInt aContactIndex );


             //From MVIMPSTEngineCchUiEventObserver
             /**
              * Handles the change connection event from CCHUI
              *@ return void
              */
             void HandleChangeConnectionEventL();

};

