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
* Description: implementation for CVIMPSTCmdAddFromPbk 
*
*/


// INCLUDE FILES
#include "cvimpstcmdaddfrompbk.h"
#include "vimpstcmd.hrh"
#include "mvimpstcmdobserver.h"
#include "vimpstdebugprint.h" 

#include <e32def.h>
#include <mvpbkstorecontact.h>
#include "mvimpstengine.h"
#include "mvimpststorageserviceview.h"
#include "cvimpststoragemanagerfactory.h"
#include <mvpbkstorecontactfield.h>
#include <mvpbkfieldtype.h>
#include <vpbkeng.rsg>
#include "mvimpstenginecontactmgmtextention.h"
#include "mvimpstengineextentionfeatures.h"
#include <mvpbkcontactfieldtextdata.h>

// Constants

// --------------------------------------------------------------------------
// CVIMPSTCmdAddFromPbk::CVIMPSTCmdAddFromPbk
// --------------------------------------------------------------------------
//
CVIMPSTCmdAddFromPbk::CVIMPSTCmdAddFromPbk( const TInt aCommandId ,
                                            const MVPbkStoreContact& aStoreContact,
                                            MVIMPSTEngine& aEngine ):            
                                            iCommandId( aCommandId ),
                                            iStoreContact( aStoreContact ),
                                            iEngine(aEngine)
    {
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdAddFromPbk::~CVIMPSTCmdAddFromPbk
// --------------------------------------------------------------------------
//
CVIMPSTCmdAddFromPbk::~CVIMPSTCmdAddFromPbk()
    {    
    
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdAddFromPbk::NewL
// --------------------------------------------------------------------------
//
CVIMPSTCmdAddFromPbk* CVIMPSTCmdAddFromPbk::NewL( const TInt aCommandId,
                                                  const MVPbkStoreContact& aStoreContact,
                                                  MVIMPSTEngine& aEngine )
    {
    CVIMPSTCmdAddFromPbk* self = new (ELeave ) CVIMPSTCmdAddFromPbk( aCommandId ,aStoreContact, aEngine);
    self->ConstructL(); //use contsurctL if necessary
    return self;
    }
// --------------------------------------------------------------------------
// CVIMPSTCmdAddFromPbk::ConstructL
// --------------------------------------------------------------------------
//
void CVIMPSTCmdAddFromPbk::ConstructL()
    {    
    iError = KErrNone;
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdAddFromPbk::ExecuteLD
// --------------------------------------------------------------------------
//
void CVIMPSTCmdAddFromPbk::ExecuteLD()
    {
    CHAT_DP_FUNC_ENTER("CVIMPSTCmdAddFromPbk::ExecuteLD");
    //push to the cleanupstack
    CleanupStack::PushL( this );
    MVIMPSTStorageServiceView* storage = CVIMPSTStorageManagerFactory::ContactListInterfaceL( iEngine.ServiceId() ) ;
    if(storage)
        {
        //storage->AddPhoneBookContactL(iStoreContact);
        // get the link and form link identify the servicedetails and then sedn it to 
        //contact management to add it to server.
        //make search request using search manager in engine   
        // Read all necessary data from contact and store to 
        // own type
        CDesCArray* arrayForService = new (ELeave) CDesCArrayFlat( 
            12 );
        CleanupStack::PushL( arrayForService ); // 1
        
        GetServiceFieldsL(*arrayForService);
        
        TInt count = arrayForService->Count();
        MVIMPSTEngineContactMgmtExtention* contactMgr = 
                    dynamic_cast<MVIMPSTEngineContactMgmtExtention*> 
                    (iEngine.ExtentionFeatures(TVIMPSTEnums::EContactManagement));

        for(TInt i = 0; i< count; i++)
            {
            if(contactMgr)
                {
                iError = contactMgr->AddServerContactL(KNullDesC(),KNullDesC(),KNullDesC());
                }
            }
        CleanupStack::PopAndDestroy(arrayForService);
        }
    else
        {
        iError = KErrGeneral;
        }
    if(iObserver)
        {
        iObserver->CommandFinishedL(*this);
        }
    CHAT_DP_FUNC_ENTER("CVIMPSTCmdAddFromPbk:: CommandFinished");    
    CleanupStack::PopAndDestroy();  
    CHAT_DP_FUNC_DONE("CVIMPSTCmdAddFromPbk::ExecuteLD");    
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdAddFromPbk::AddObserver
// --------------------------------------------------------------------------
//
void CVIMPSTCmdAddFromPbk::AddObserver( MVIMPSTCmdObserver& aObserver )
    {
    // store the observer to notify the command completion
    iObserver = &aObserver;
    }


// --------------------------------------------------------------------------
// CVIMPSTCmdAddFromPbk::CommandId
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdAddFromPbk::CommandId() const
    {
    return iCommandId;  
    }

// --------------------------------------------------------------------------
// CVIMPSTCmdAddFromPbk::Result
// --------------------------------------------------------------------------
//
TInt CVIMPSTCmdAddFromPbk::Result() const
    {
    //return valid data regd the command operation
    return iError;
    }
// ---------------------------------------------------------------------------
// CVIMPSTCmdAddFromPbk::GetServiceFieldsL
// ---------------------------------------------------------------------------
//  
void CVIMPSTCmdAddFromPbk::GetServiceFieldsL( CDesCArray& aArrayForServiceFields )
    {
    CHAT_DP_FUNC_ENTER("[CVIMPSTStorageContact::GetServiceFieldsL]  ->  GetServiceFieldsL");
    HBufC* values[40];
    for( TInt index = 0 ; 
        index < iStoreContact.Fields().FieldCount() ; 
        index++ )
        {
         const MVPbkStoreContactFieldCollection& fieldCollection = iStoreContact.Fields();
       
        const MVPbkStoreContactField& field = fieldCollection.FieldAt( index );
        const MVPbkFieldType* fieldType = field.BestMatchingFieldType();
        
        //find the relevant voip fields/email id fileds.
        if ( fieldType &&  
            ( fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_VOIPHOME ) || 
            ( fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_VOIPWORK ) || 
            ( fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_VOIPGEN ) ||
            ( fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_SIP )||
            ( fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_EMAILWORK ) ||
            ( fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_EMAILHOME ) ||
            ( fieldType->FieldTypeResId() == R_VPBK_FIELD_TYPE_EMAILGEN ))
            {
            CHAT_DP_FUNC_ENTER("[CVIMPSTStorageContact::GetServiceFieldsL]  ->  GetServiceFieldsL - Found");
            values[0] = MVPbkContactFieldTextData::Cast( 
                    iStoreContact.Fields().FieldAt( index ).
                    FieldData() ).Text().AllocLC();
            if ( values[0] )
                {
                // Check for prefix and remove if found
                TInt prefixLocation = values[0]->Locate( ':' );
                if ( KErrNotFound != prefixLocation )
                    {
                    CHAT_DP_FUNC_ENTER("[CVIMPSTStorageContact::GetServiceFieldsL]  ->  Prefix found -> remove");
                    aArrayForServiceFields.AppendL( values[0]->Des().Mid(
                        prefixLocation+1 ) );
                    }
                else
                    {
                    CHAT_DP_FUNC_ENTER("[CVIMPSTStorageContact::GetServiceFieldsL]  ->  No Prefix found");
                    aArrayForServiceFields.AppendL( values[0]->Des() );    
                    }
                CleanupStack::PopAndDestroy( values[0] );
                }
            }
        }   
    }        
    
// End of File
