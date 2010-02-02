/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of the header part control(s) of the details view layout
*
*/


#include "cvimpstdetailsheadercontrol.h"

#include "cvimpstdetailsimagedecoder.h"
#include "mvimpstdetailspresencehandler.h"
#include "cvimpstdetailsviewbrandhandler.h"
#include <vimpstui.mbg>
#include <conversations.mbg>
#include <eikimage.h> 
#include <eiklabel.h> 
#include <AknUtils.h>
#include <layoutmetadata.cdl.h>
#include <applayout.cdl.h> //cdl
#include <aknlayoutscalable_apps.cdl.h>
#include <AknsUtils.h>

// virtual phonebook
#include <MVPbkStoreContactField.h>
#include <MVPbkContactFieldData.h>
#include <MVPbkFieldType.h>
#include <MVPbkContactFieldBinaryData.h>
#include <VPbkEng.rsg>
#include <MVPbkStoreContact.h>

// constants declaration
const TInt KLabelLineCount = 3;

// ---------------------------------------------------------------------------
// NewL, two-phase construction
// ---------------------------------------------------------------------------
//
CVIMPSTDetailsHeaderControl* CVIMPSTDetailsHeaderControl::NewL(CVIMPSTDetailsViewBrandHandler& aBrandHandler,
												MVIMPSTDetailsPresenceHandler& aPresenceHandler)
    {
    CVIMPSTDetailsHeaderControl* self= new (ELeave) CVIMPSTDetailsHeaderControl(aBrandHandler, aPresenceHandler );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// C++ (first phase) constructor
// ---------------------------------------------------------------------------
//
CVIMPSTDetailsHeaderControl::CVIMPSTDetailsHeaderControl(CVIMPSTDetailsViewBrandHandler& aBrandHandler, 
							MVIMPSTDetailsPresenceHandler& aPresenceHandler)
   : iBrandHandler( aBrandHandler),
   iPresenceHandler( aPresenceHandler )
    {
    // No implementation required
    }

// ---------------------------------------------------------------------------
// ConstructL, second phase constructor
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsHeaderControl::ConstructL()
	{ 
	// Create the header image
	iImage = new (ELeave) CEikImage();
	iImage->SetPictureOwnedExternally(ETrue); //EHLeftVCenter
	iImage->SetAlignment(EHCenterVCenter);
	// Create the header labels
    for (TInt i=0; i < KLabelLineCount; i++)
        {
        CEikLabel* label = new(ELeave) CEikLabel;
        CleanupStack::PushL(label);
        label->SetTextL(KNullDesC());
        label->SetAlignment(EHLeftVCenter);// EHCenterVCenter, EHLeftVCenter
        label->CropText();
        iLabels.AppendL(label);
        CleanupStack::Pop(label);
        }
   	}

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CVIMPSTDetailsHeaderControl::~CVIMPSTDetailsHeaderControl()
    {
    iLabels.ResetAndDestroy();
  
    if (iImageDecoder)
        {
        if (iImageDecoder->IsActive())
            {
            iImageDecoder->Cancel();
            }
        delete iImageDecoder;
        iImageDecoder = NULL;
        }
    delete iHeaderField;
       
    if (iImage)
        {
        delete iImage;
        iImage = NULL;
        }
  
    if (iBitmap)
        {
        delete iBitmap;
        iBitmap = NULL;
        }
    if (iMask)
        {
        delete iMask;
        iMask = NULL;
        }
     }


// ---------------------------------------------------------------------------
// From CCoeControl, returns the number of child controls to the framework
// ---------------------------------------------------------------------------
//
TInt CVIMPSTDetailsHeaderControl::CountComponentControls() const
    {
    // image control + Label Count
    return 1 + iLabels.Count();
    }

// ---------------------------------------------------------------------------
// From CCoeControl, returns the child controls to the framework by index
// ---------------------------------------------------------------------------
//
CCoeControl* CVIMPSTDetailsHeaderControl::ComponentControl(TInt aIndex)  const
    {
    if (aIndex == 0)
        {
        return iImage;
        }
    else if (aIndex - 1 < iLabels.Count())
        {
        return iLabels[aIndex-1];
        }
    else
        {
        return NULL;
        }
    }

// ---------------------------------------------------------------------------
// From CCoeControl, called when the control's size changes, handles portrait-landscape switch
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsHeaderControl::SizeChanged()
    {
    TInt isLandscape = Layout_Meta_Data::IsLandscapeOrientation() ? 1 : 0;
    
    AknLayoutUtils::LayoutImage(
            iImage, Rect(), AknLayoutScalable_Apps::cl_header_pane_g1(isLandscape));
    
    TAknLayoutRect labelsRect;
    labelsRect.LayoutRect(Rect(), AknLayoutScalable_Apps::cl_header_name_pane(isLandscape));
    
    AknLayoutUtils::LayoutLabel(iLabels[0], labelsRect.Rect(), AknLayoutScalable_Apps::cl_header_name_pane_t1(2));
    AknLayoutUtils::LayoutLabel(iLabels[1], labelsRect.Rect(), AknLayoutScalable_Apps::cl_header_name_pane_t2(2));
    
    AknLayoutUtils::LayoutLabel(iLabels[2], labelsRect.Rect(), AknLayoutScalable_Apps::cl_header_name_pane_t3(2));
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsHeaderControl::LabelsSizeChangedL()
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsHeaderControl::LabelsSizeChangedL()
    {
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    TRgb color;
    AknsUtils::GetCachedColor( skin, color, KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG6 );
        
    if (iLabels.Count() > 0)
        {
         for (TInt i=0; i < iLabels.Count(); i++)
            {
            iLabels[i]->SetTextL( iPresenceHandler.GetHeaderLabelDataL(i) );
            iLabels[i]->OverrideColorL( EColorLabelText, color );
            iLabels[i]->DrawDeferred();
            }
         }
    }
// ---------------------------------------------------------------------------
// CVIMPSTDetailsHeaderControl::HeaderSizeChangedL()
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsHeaderControl::HeaderSizeChangedL()
    {
    CFbsBitmap* bmp = NULL;
    CFbsBitmap* bmpMask = NULL;
    TVIMPSTEnums::TOnlineStatus onlineStatus = iPresenceHandler.GetPresenceStatus();
    if(TVIMPSTEnums::EPending == onlineStatus)
        {
        iBrandHandler.LoadBitMapLC(bmp, 
                bmpMask,
                EMbmVimpstuiQgn_prop_service_friend_request_sent ,
                EMbmVimpstuiQgn_prop_service_friend_request_sent_mask  );
        }
    else if(TVIMPSTEnums::EBlocked == onlineStatus)
        {
        iBrandHandler.LoadBitMapLC(bmp, 
                bmpMask,
                EMbmVimpstuiQgn_servtab_avatar_blocked ,
                EMbmVimpstuiQgn_servtab_avatar_blocked_mask  );
        }
     else
        {
        iBrandHandler.LoadBitMapLC(bmp, 
                bmpMask,
                EMbmVimpstuiQgn_prop_service_default_avatar ,
                EMbmVimpstuiQgn_prop_service_default_avatar_mask  );
        }
    SetBitmap(bmp, bmpMask);//ownership transferred
    CleanupStack::Pop(2); // bitmap,bmpMask 
    SizeChanged();

    }
// ---------------------------------------------------------------------------
// From CCoeControl, overridden to set the container windows for the child
// controls of this control (labels and image)
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsHeaderControl::SetContainerWindowL(const CCoeControl& aContainer)
    {
    CCoeControl::SetContainerWindowL(aContainer);
    iImage->SetContainerWindowL(aContainer);
    for (TInt i=0; i < iLabels.Count(); i++)
        {
        iLabels[i]->SetContainerWindowL(aContainer);
        }
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsHeaderControl :: SetBitmap
// Sets the bitmap shown in the header image 
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsHeaderControl::SetBitmap(CFbsBitmap* aBmp, CFbsBitmap* aMask )
    {
    if (iBitmap)
        {
        delete iBitmap;
        iBitmap = NULL;
        }
    if (iMask)
        {
        delete iMask;
        iMask = NULL;
        }
    iBitmap = aBmp;
    iMask = aMask;
    
    iImage->SetPicture(aBmp, NULL);
    iImage->DrawDeferred();
    
    TRAP_IGNORE( LabelsSizeChangedL() );
    }

// ---------------------------------------------------------------------------
// CVIMPSTDetailsHeaderControl :: ClearL
// Clears the header texts & image to blank
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsHeaderControl::ClearL()
    {
    iImage->SetPicture(NULL, NULL);
    if (iBitmap)
        {
        delete iBitmap;
        iBitmap = NULL;
        }
    if (iMask)
        {
        delete iMask;
        iMask = NULL;
        }
    for (TInt i = 0; i < iLabels.Count(); i++)
        {
        iLabels[i]->SetTextL(KNullDesC());
        }
  
    }
// ---------------------------------------------------------------------------
// CVIMPSTDetailsHeaderControl :: CreateHeaderPresentationL
// Clears the header texts & image to blank
// ---------------------------------------------------------------------------
//
void CVIMPSTDetailsHeaderControl::CreateHeaderPresentationL(MVPbkStoreContact& aContact )
	{
	if( iHeaderField )
		{
		delete iHeaderField;
		iHeaderField = NULL	;
		}
				
	MVPbkStoreContactFieldCollection& fieldCollection = aContact.Fields();
	
	TInt fieldCount = fieldCollection.FieldCount();
	
	for( TInt i=0; i< fieldCount; i++ )
		{
		MVPbkStoreContactField& field = fieldCollection.FieldAt( i );	
		const MVPbkFieldType* type = field.BestMatchingFieldType() ;
		if( R_VPBK_FIELD_TYPE_THUMBNAILPIC == type->FieldTypeResId() )
			{
			iHeaderField = field.CloneLC();
			CleanupStack::Pop();
			break;	
			}
		}
	if( iHeaderField )
		{
		// field available	
		const MVPbkContactFieldData& fieldData = iHeaderField->FieldData();
		if( fieldData.DataType() == EVPbkFieldStorageTypeBinary )
			{
			TPtrC8 data = MVPbkContactFieldBinaryData::Cast(fieldData).BinaryData();
	       	if (iImageDecoder)
	            {
	            if (iImageDecoder->IsActive())
	                {
	                iImageDecoder->Cancel();
	                }
	            delete iImageDecoder;
	            iImageDecoder = NULL;
	            }
	        iImageDecoder = CVIMPSTDetailsImageDecoder::NewL(*this, data );
	        iImageDecoder->Start();
			}
		}
	
     }

// end of file

