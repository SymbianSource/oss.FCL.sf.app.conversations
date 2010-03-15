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
* Description:  class to publish the data into widget
*                
*
*/

#include "cservicewidgetcontentpublisher.h"

#include <liwservicehandler.h>
#include <liwvariant.h>
#include <liwgenericparam.h>
#include <avkon.hrh>
#include <aknnotewrappers.h> 
#include "swpdebugtrace.h"

#include <liwcommon.h>
#include "servicewidgetcpglobals.h"
#include <servicewidgetpluginres.rsg>
#include <StringLoader.h>

// ---------------------------------------------------------------------------
// CServiceWidgetContentPublisher::CServiceWidgetContentPublisher
// ---------------------------------------------------------------------------
// 
CServiceWidgetContentPublisher::CServiceWidgetContentPublisher(MServiceWidgetEventObserver& aObserver )
   :iObserver( aObserver )
    {
   TRACE_SWP(TXT("CServiceWidgetContentPublisher::CServiceWidgetContentPublisher") );
   }

// ---------------------------------------------------------------------------
// CServiceWidgetContentPublisher::NewL
// ---------------------------------------------------------------------------
// 
CServiceWidgetContentPublisher* CServiceWidgetContentPublisher::NewL(MServiceWidgetEventObserver& aObserver)
    {
    TRACE_SWP(TXT("CServiceWidgetContentPublisher::NewL() start") );
    CServiceWidgetContentPublisher* self = NewLC(aObserver);
    CleanupStack::Pop(self);
    TRACE_SWP(TXT("CServiceWidgetContentPublisher::NewL() End") );
    return self;
    }

// ---------------------------------------------------------------------------
// CServiceWidgetContentPublisher::NewLC
// ---------------------------------------------------------------------------
// 
CServiceWidgetContentPublisher* CServiceWidgetContentPublisher::NewLC(MServiceWidgetEventObserver& aObserver)
    {
    TRACE_SWP(TXT("CServiceWidgetContentPublisher::NewLC() start") );
    CServiceWidgetContentPublisher* self =
        new (ELeave) CServiceWidgetContentPublisher(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    TRACE_SWP(TXT("CServiceWidgetContentPublisher::NewLC() End") );
    return self;
    }

// ---------------------------------------------------------------------------
// CServiceWidgetContentPublisher::ConstructL
// ---------------------------------------------------------------------------
// ConstructL is called by the application framework
void CServiceWidgetContentPublisher::ConstructL()
    {
    TRACE_SWP(TXT("CServiceWidgetContentPublisher::ConstructL() start") );
	iMsgInterface = GetMessagingInterfaceL();
	TRACE_SWP(TXT("CServiceWidgetContentPublisher::ConstructL() End") );
    }
// ---------------------------------------------------------------------------
// CServiceWidgetContentPublisher::~CServiceWidgetContentPublisher
// ---------------------------------------------------------------------------
CServiceWidgetContentPublisher::~CServiceWidgetContentPublisher()
    {
    TRACE_SWP(TXT("CServiceWidgetContentPublisher::~CServiceWidgetContentPublisher() start") );
    if( iMsgInterface )
    	{
		iMsgInterface->Close();
		iMsgInterface = NULL;
    	}
   	if( iServiceHandler )
    	{
    	iServiceHandler->Reset();
    	delete iServiceHandler;
    	iServiceHandler = NULL;
    	}
    TRACE_SWP(TXT("CServiceWidgetContentPublisher::~CServiceWidgetContentPublisher() End") );
    }

// ---------------------------------------------------------------------------
// CServiceWidgetContentPublisher::GetMessagingInterfaceL
// ---------------------------------------------------------------------------

MLiwInterface* CServiceWidgetContentPublisher::GetMessagingInterfaceL( )
	{
	TRACE_SWP(TXT("CServiceWidgetContentPublisher::GetMessagingInterfaceL() start") );
	if( !iServiceHandler )
		{
		iServiceHandler = CLiwServiceHandler::NewL();
		TRACE_SWP(TXT("CServiceWidgetContentPublisher::GetMessagingInterfaceL() CLiwServiceHandler created") );
		}
	CLiwGenericParamList* inParam = CLiwGenericParamList::NewL();
	CleanupStack::PushL( inParam );
	CLiwGenericParamList* outParam = CLiwGenericParamList::NewL();
	CleanupStack::PushL( outParam );
	CLiwCriteriaItem* crit = CLiwCriteriaItem::NewL( KLiwCmdAsStr, KCPContentInterface,
		KCPService  );
	crit->SetServiceClass( TUid::Uid(KLiwClassBase) );
	
	RCriteriaArray array;
	array.AppendL(crit);    
	iServiceHandler->AttachL(array);
	iServiceHandler->ExecuteServiceCmdL( *crit, *inParam, *outParam ); 
	delete crit;
	array.Reset();
	
	// find service interface
	TInt pos = 0;
	MLiwInterface* msgInterface = NULL;
	outParam->FindFirst( pos, KCPContentInterface );
	if ( pos != KErrNotFound ) 
	    {
	    msgInterface = (*outParam)[pos].Value().AsInterface(); 
	    }
	outParam->Reset();
	inParam->Reset();
	CleanupStack::PopAndDestroy( outParam );
	CleanupStack::PopAndDestroy( inParam );
	TRACE_SWP(TXT("CServiceWidgetContentPublisher::GetMessagingInterfaceL() End") );
	return msgInterface;
	}
// ---------------------------------------------------------------------------
// CServiceWidgetContentPublisher::HandleNotifyL
// ---------------------------------------------------------------------------
//  
TInt CServiceWidgetContentPublisher::HandleNotifyL( TInt aCmdId, TInt /*aEventId*/,
                                                    CLiwGenericParamList& aEventParamList,
                                                    const CLiwGenericParamList& /*aInParamList*/ 
                                                   )
    { 
    TRACE_SWP( TXT("CServiceWidgetContentPublisher::HandleNotifyL() start") );
    TInt pos(0);
    aEventParamList.FindFirst(pos, KChangeInfo);

    if (pos != KErrNotFound)
        {
        TLiwVariant variant = (aEventParamList)[pos].Value();
        variant.PushL();
        const CLiwList* changeMapsList = variant.AsList();
        HBufC16* publisher  = NULL;       
        HBufC8* trigger = NULL;
        const TInt KSAPIContentNameMaxLength = 255;
        TBuf<KSAPIContentNameMaxLength> InstId;

        for (TInt i = 0; i < changeMapsList->Count(); ++i)
            {
            if ( changeMapsList->AtL(i, variant) ) 
                {
                const CLiwMap* map  = variant.AsMap();
                if( map->FindL( KActionTrigger, variant) )
                    {
                    trigger = variant.AsData().AllocLC(); // trigger name                      
                    // In template case trigger name can be 
                    // active ,  resume , suspend ,inactive, deactive or item names.
                    }
                if( map->FindL( KPublisherId, variant) )
                    {
                    publisher = variant.AsDes().AllocLC();  // ai3templatedwidget                                          
                    }
                if( map->FindL( KContentId, variant) )
                    {
                    variant.Get( InstId );
                    }
                if( trigger && publisher )
                    {
                    iObserver.HandleServiceEventL( *publisher, *trigger, InstId);
                    }//
                if ( publisher )
                    {
                    CleanupStack::PopAndDestroy( publisher );
                    publisher = NULL;
                    }
                if ( trigger )
                    {
                    CleanupStack::PopAndDestroy( trigger );
                    trigger = NULL;
                    }  
                }
            }// loop
    CleanupStack::Pop(&variant);    //variant
    variant.Reset(); 
    }//if (pos != KErrNotFound)
  TRACE_SWP(TXT("CServiceWidgetContentPublisher::HandleNotifyL() end") );
  return aCmdId;
  }//End HandleNotifyL

// ---------------------------------------------------------------------------
// CServiceWidgetContentPublisher::RegisterWidgetL
// ---------------------------------------------------------------------------
//
void CServiceWidgetContentPublisher::RegisterWidgetL(const TDesC& aPublisherId,
                                                           TInt aBitmapHandle,
                                                           TInt aMaskHandle )
    {
    TRACE_SWP(TXT("CServiceWidgetContentPublisher::RegisterWidgetL() start") );
    if( iMsgInterface )
        {   
        CLiwGenericParamList* inparam = &(iServiceHandler->InParamListL());
        CLiwGenericParamList* outparam = &(iServiceHandler->OutParamListL());

        TLiwGenericParam type( KType, TLiwVariant( KPublisher ));
        inparam->AppendL( type );

        CLiwDefaultMap* cpdatamap = CLiwDefaultMap::NewLC();
        CLiwDefaultMap* datamap = CLiwDefaultMap::NewLC();
        CLiwDefaultMap* mapAction = CLiwDefaultMap::NewLC(); 
          
        cpdatamap->InsertL( KPublisherId, TLiwVariant( aPublisherId )); 
        cpdatamap->InsertL( KContentType, TLiwVariant( KTemplateWidget ));
        cpdatamap->InsertL( KContentId, TLiwVariant( KAll ));
        //Take widget name as "service name"
        //Give widget name here which will be displayed in HomeScreen Add Content menu
        datamap->InsertL( KWidgetName, TLiwVariant( aPublisherId ));
        datamap->InsertL( KTemplateType, TLiwVariant( KServiceWidget )); 
        
        //To publish logo and widget description
        HBufC* text = NULL;
        text = StringLoader::LoadLC( R_QTN_SERVTAB_WIDGET_SERVDESC );
        datamap->InsertL(KWidgetDesc, TLiwVariant( *text ));
        CleanupStack::PopAndDestroy(text);//text
        datamap->InsertL(KWidgetLogo , TLiwVariant( aBitmapHandle ) ); // key - aKey, value - map (stringsMap)
        // append mask 
        // create mask key for the data item. Format is the image key with the '_mask' postfix.
        HBufC8* maskResult = HBufC8::NewLC( KWidgetLogo().Length() + KMask().Length() );
        TPtr8 maskResultPtr = maskResult ->Des();
        maskResultPtr .Append( KWidgetLogo );
        maskResultPtr .Append( KMask );
        datamap->InsertL( maskResultPtr,  TLiwVariant( aMaskHandle ) ); // key - aKey, value - map (stringsMap)
        CleanupStack::PopAndDestroy(maskResult);//maskResult
        
        cpdatamap->InsertL( KDataMap, TLiwVariant(datamap) );

        mapAction->InsertL(KActive, TLiwVariant(KTriggerMap));
        mapAction->InsertL(KDeActive, TLiwVariant(KTriggerMap));
        mapAction->InsertL(KSuspend, TLiwVariant(KTriggerMap));
        mapAction->InsertL(KInactive, TLiwVariant(KTriggerMap));
        mapAction->InsertL(KResume, TLiwVariant(KTriggerMap));
        // this will be called after clicking on widget, and generate "selected" event
        mapAction->InsertL( KSelected, TLiwVariant(  KTriggerMap  ) );
        cpdatamap->InsertL( KActionMap, TLiwVariant(mapAction) );

        TLiwGenericParam textItem( KItem, TLiwVariant( cpdatamap ));        
        inparam->AppendL( textItem );
        iMsgInterface->ExecuteCmdL( KAdd , *inparam, *outparam);
        CleanupStack::PopAndDestroy(mapAction);
        CleanupStack::PopAndDestroy(datamap);
        CleanupStack::PopAndDestroy(cpdatamap);
        
        textItem.Reset();
        outparam->Reset();
        inparam->Reset(); 
        
        // request for active/deactive/suspend/resume notification 
        CLiwDefaultMap*  reqFilter = CLiwDefaultMap::NewLC();
        reqFilter->InsertL(KPublisherId, TLiwVariant( aPublisherId ));
        reqFilter->InsertL(KContentType, TLiwVariant( KAll ));
        reqFilter->InsertL( KContentId, TLiwVariant( KAll ));
        reqFilter->InsertL(KOperation, TLiwVariant(KExecute));

        // fill in input list for RequestNotification command
        inparam->AppendL(TLiwGenericParam(KType, TLiwVariant(KPublisher)));
        inparam->AppendL(TLiwGenericParam(KFilter, TLiwVariant(reqFilter)));
       
        iMsgInterface->ExecuteCmdL( KRequestNotification,
                                    *inparam,
                                    *outparam,
                                    0,
                                    this ) ;

        CleanupStack::PopAndDestroy(reqFilter );  
        outparam->Reset();
        inparam->Reset();
        TRACE_SWP(TXT("CServiceWidgetContentPublisher::RegisterWidgetL() Registered widget %s"), &aPublisherId );
        } 
    TRACE_SWP(TXT("CServiceWidgetContentPublisher::RegisterWidgetL() end") );
    }

// ---------------------------------------------------------------------------
// CServiceWidgetContentPublisher::UnregisterWidgetL
// ---------------------------------------------------------------------------
//
void CServiceWidgetContentPublisher::UnregisterWidgetL(const TDesC& aPublisherId )
    {
    TRACE_SWP(TXT("CServiceWidgetContentPublisher::RegisterForObserverL() start") );
    
    CLiwGenericParamList* inparam = &(iServiceHandler->InParamListL()); 
    CLiwGenericParamList* outparam = &(iServiceHandler->OutParamListL());
    CLiwDefaultMap*  cpdatamap= CLiwDefaultMap::NewLC();

    cpdatamap->InsertL( KPublisherId, TLiwVariant( aPublisherId ));
    cpdatamap->InsertL( KContentType, TLiwVariant( KTemplateWidget ));
    cpdatamap->InsertL( KContentId, TLiwVariant( KAll ));
    // fill in input list for RequestNotification command
    inparam->AppendL(TLiwGenericParam(KType, TLiwVariant(KPublisher)));
    inparam->AppendL(TLiwGenericParam(KData, TLiwVariant(cpdatamap)));

    iMsgInterface->ExecuteCmdL(KDelete, *inparam, *outparam, 0, this );

    CleanupStack::PopAndDestroy(cpdatamap);      
    outparam->Reset();
    inparam->Reset();
    TRACE_SWP(TXT("CServiceWidgetContentPublisher::UnregisterWidget() end") );
    }//end UnregisterWidget

// ---------------------------------------------------------------------------
// CServiceWidgetContentPublisher::RemoveWidgetDataL
// ---------------------------------------------------------------------------
// 
void CServiceWidgetContentPublisher::RemoveWidgetDataL(const TDesC& aPublisherId, const TDesC& aInstId)
    {
    TRACE_SWP(TXT("CServiceWidgetContentPublisher::RemoveWidgetData() start") );
    CLiwGenericParamList* inparam = &(iServiceHandler->InParamListL());
    CLiwGenericParamList* outparam = &(iServiceHandler->OutParamListL());

    CLiwDefaultMap* cpdatamap = CLiwDefaultMap::NewLC();

    cpdatamap->InsertL( KPublisherId, TLiwVariant( aPublisherId ));
    cpdatamap->InsertL( KContentType, TLiwVariant( KAll )); 
    cpdatamap->InsertL( KContentId, TLiwVariant( aInstId ));
    
    // fill in input list for RequestNotification command
    inparam->AppendL(TLiwGenericParam(KType, TLiwVariant(KCpData)));
    inparam->AppendL(TLiwGenericParam(KData, TLiwVariant(cpdatamap)));

    TInt err = KErrNone;
    // if you have only one combination of publisher,content_type, contenid 
    // then all the items will be remove from your widget
    TRAP(err,iMsgInterface->ExecuteCmdL( KDelete , *inparam, *outparam, 0 , this));

    CleanupStack::PopAndDestroy( cpdatamap );
    outparam->Reset();
    inparam->Reset();
    TRACE_SWP(TXT("CServiceWidgetContentPublisher::RemoveWidgetData() end") );
    }
// ---------------------------------------------------------------------------
// CServiceWidgetContentPublisher::PublishDataWithActionL
// ---------------------------------------------------------------------------
// 
void CServiceWidgetContentPublisher::PublishDataWithActionL( const TDesC& aPublisherId,
                                                             const TDesC& aInstId,
                                                             const TDesC& aOwnTextToAdd, 
                                                             const TDesC& aStatusTextToAdd,
                                                             TInt aBitmapHandle,
                                                             TInt aMaskHandle,
                                                             TInt aSericonbitmapHandle, 
                                                             TInt aSericonmaskHandle
													         )
    {
    TRACE_SWP(TXT("CServiceWidgetContentPublisher::PublishDataWithActionL() start") );

    CLiwGenericParamList* inparam = &(iServiceHandler->InParamListL());
    CLiwGenericParamList* outparam = &(iServiceHandler->OutParamListL());
    inparam->Reset();
    outparam->Reset();

    if( iMsgInterface )
        {
         TLiwGenericParam type( KType,TLiwVariant(KCpData));
         inparam->AppendL( type );

         CLiwDefaultMap* cpdatamap = CLiwDefaultMap::NewLC();
         CLiwDefaultMap* datamap = CLiwDefaultMap::NewLC();

         cpdatamap->InsertL( KPublisherId, TLiwVariant( aPublisherId ));
         cpdatamap->InsertL( KContentType, TLiwVariant( KAll ));
         cpdatamap->InsertL( KContentId, TLiwVariant( aInstId ));

         //adding data to the map
         //add service icon data
         datamap->InsertL(KServiceImageKey , TLiwVariant( aSericonbitmapHandle ) ); // key - aKey, value - map (stringsMap)
         // append mask 
         // create mask key for the data item. Format is the image key with the '_mask' postfix.
         HBufC8* maskResult1 = HBufC8::NewLC( KServiceImageKey().Length() + KMask().Length() );
         TPtr8 maskResultPtr1 = maskResult1 ->Des();
         maskResultPtr1.Append( KServiceImageKey );
         maskResultPtr1.Append( KMask );
         datamap->InsertL( maskResultPtr1,  TLiwVariant( aSericonmaskHandle ) ); // key - aKey, value - map (stringsMap)
         CleanupStack::PopAndDestroy(maskResult1);//maskResult 
         //Add status icon data
         datamap->InsertL(KStatusIconKey , TLiwVariant( aBitmapHandle ) ); // key - aKey, value - map (stringsMap)
         // append mask 
         // create mask key for the data item. Format is the image key with the '_mask' postfix.
         HBufC8* maskResult = HBufC8::NewLC( KStatusIconKey().Length() + KMask().Length() );
         TPtr8 maskResultPtr = maskResult ->Des();
         maskResultPtr .Append( KStatusIconKey );
         maskResultPtr .Append( KMask );
         datamap->InsertL( maskResultPtr,  TLiwVariant( aMaskHandle ) ); // key - aKey, value - map (stringsMap)
         CleanupStack::PopAndDestroy(maskResult);//maskResult
         //add data for first text row and second text row
         datamap->InsertL(KOwnTextKey, TLiwVariant(aOwnTextToAdd));
         datamap->InsertL(KMessageTextKey, TLiwVariant(aStatusTextToAdd));
         cpdatamap->InsertL( KDataMap, TLiwVariant(datamap) );

         TLiwGenericParam item( KItem, TLiwVariant( cpdatamap ));        
         inparam->AppendL( item );
         iMsgInterface->ExecuteCmdL( KAdd,  *inparam, *outparam );
         item.Reset();
         type.Reset(); 
         CleanupStack::PopAndDestroy(datamap); // datamap
         CleanupStack::PopAndDestroy(cpdatamap);
         TRACE_SWP(TXT("CServiceWidgetContentPublisher::PublishDataWithActionL() success") );
        }	
    }

// end of file
