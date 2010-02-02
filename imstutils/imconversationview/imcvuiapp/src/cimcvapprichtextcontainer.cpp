/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - Initial contribution
* 
* Contributors:
* 
* 
* Description:
* Export help related files.
*
*/




// INCLUDE FILES
#include    "cimcvapprichtextcontainer.h"
#include    "cimcvappmessagewrapper.h"
#include    "mimcvapptextview.h"
#include    "cimcvappui.h"
#include    "mimcvenginemessage.h"
#include    "cimcvappmessageextensionshandler.h"

#include    <AknIconUtils.h>
#include    <txtrich.h>     // CRichtText
#include    <AknSettingCache.h>
#include    <aknenv.h>
#include    <aknconsts.h>
#include    <finditemengine.h>
#include    <barsread.h> // resourcereader
#include    <StringLoader.h>

#include    <AknsDrawUtils.h>
#include    <AknsUtils.h>
#include    <AknUtils.h>
#include 	"imcvlogger.h"
#include	"cimcvapppicture.h"
#include 	<imcvuiapp.mbg>
#include	"cimcvappsmileiconutility.h"
#include    <imcvuiapp.rsg>

// CONSTANTS
const TInt KIMCVHighlightColor = 244;
const TInt KTimeStampMaxLength = 15;     // max length of timestamp text
const TInt KUnicodeRLM         = 0x200F; // Right-to-Left Mark
const TInt KUnicodeLRM         = 0x200E; // Left-to-Right Mark
//Received message maximum length is 400 character,
//this must always be in sync with imconnectionprovider KReceiveMsgMaxLength
const TInt KReceiveMsgMaxLength = 400; 
const TInt KMsgMaxLength = 450; //Received message maximum length is 400 character + truncation message 
_LIT( KSeparator, ": " );
_LIT(KSpace, " ");


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::CIMCVAppRichTextContainer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CIMCVAppRichTextContainer::CIMCVAppRichTextContainer(MIMCVAppUi* aAppUi,
        MIMCVAppTextView* aTextView, 
        CIMCVAppMessageExtensionsHandler* aExtensionsHandler,
        CGulIcon* aFromMe,
        CGulIcon* aToMe,
        MGraphicsDeviceMap* aMap, 
        TInt aServiceId,
        TBool aScrollOver /*= ETrue*/):
    iAppUi(aAppUi),
    iTextView( aTextView ), 
    iItemHighlight( ETrue ), 
    iHighlightState( ENoHighlight ),
    iExtensionsHandler( aExtensionsHandler ),
    iScrollOver( aScrollOver ),
    iFromMe( aFromMe ), 
    iToMe( aToMe ),
    iMap(aMap),
    iServiceId( aServiceId )
	    {
	    }

// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::Initialize
// Initializes iTextView
// -----------------------------------------------------------------------------
//
void CIMCVAppRichTextContainer::Initialize( MIMCVAppUi* aAppUi,
        MIMCVAppTextView* aTextView, 
        CIMCVAppMessageExtensionsHandler* aExtensionsHandler,
        CGulIcon* aFromMe,
        CGulIcon* aToMe,
        MGraphicsDeviceMap* aMap,                              
        TBool aScrollOver /*= ETrue*/ )
    {
    iAppUi = aAppUi;
    iTextView = aTextView;   
    iExtensionsHandler = aExtensionsHandler;
    iFromMe = aFromMe;
    iToMe = aToMe;
    iMap = aMap;
    iScrollOver = aScrollOver;
    }

// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CIMCVAppRichTextContainer::ConstructL(const TDesC& aPrimaryKey)
    {
    
    iPrimaryKey = aPrimaryKey.AllocL();
    
    iAppUi->AddLayoutChangeObserver( this );
    iAppUi->AddResourceChangeObserver( this );
       // setup layout
    TAknLayoutId layoutId;
    CAknEnv::Static()->GetCurrentLayoutId( layoutId );
    
      
    iPrimaryFont = AknLayoutUtils::FontFromId( EAknLogicalFontPrimaryFont, 
    	NULL );
    iSecondaryFont = AknLayoutUtils::FontFromId( EAknLogicalFontPrimarySmallFont, 
    	NULL );

    CParaFormat paraFormat;
    TParaFormatMask paraFormatMask;
	paraFormat.iLanguage = CAknEnv::Static()->SettingCache().InputLanguage();
	paraFormatMask.SetAttrib( EAttParaLanguage );

    paraFormat.iFillColor = KRgbWhite;
    paraFormatMask.SetAttrib( EAttFillColor );
    
    paraFormat.iLeftMarginInTwips = 100;
    paraFormat.iIndentInTwips = -100;
    paraFormatMask.SetAttrib( EAttLeftMargin );    
    paraFormatMask.SetAttrib( EAttIndent );    

    TCharFormat charFormat;
    TCharFormatMask charFormatMask;
    charFormat.iFontSpec = iSecondaryFont->FontSpecInTwips();
    charFormatMask.SetAttrib( EAttFontTypeface );
    charFormatMask.SetAttrib( EAttFontHeight );

    // Text color
    TRgb defaultSkinTextColor( KRgbBlack );
    AknsUtils::GetCachedColor( AknsUtils::SkinInstance(), 
                               defaultSkinTextColor,
                               KAknsIIDQsnTextColors,
                               EAknsCIQsnTextColorsCG6 );
    charFormat.iFontPresentation.iTextColor = defaultSkinTextColor;
    charFormatMask.SetAttrib( EAttColor );

   	charFormat.iFontPresentation.iPictureAlignment = 
   	    TFontPresentation::EAlignTop;
   	charFormatMask.SetAttrib( EAttFontPictureAlignment );
    
    charFormat.iFontSpec.iFontStyle.SetStrokeWeight( EStrokeWeightNormal );
    charFormatMask.SetAttrib( EAttFontStrokeWeight );
    

    iParaFormatLayer = CParaFormatLayer::NewL( &paraFormat, paraFormatMask );
    iCharFormatLayer = CCharFormatLayer::NewL( charFormat, charFormatMask );

    iRichText = CRichText::NewL( iParaFormatLayer, iCharFormatLayer );    
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::ConstructL()	end" ));
    }

// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CIMCVAppRichTextContainer* CIMCVAppRichTextContainer::NewL( MIMCVAppUi* aAppUi,
                              MIMCVAppTextView* aTextView, 
                              CIMCVAppMessageExtensionsHandler* aExtensionsHandler,
                              CGulIcon* aFromMe,
                              CGulIcon* aToMe,
							  MGraphicsDeviceMap* aMap,                              
                              const TDesC& aPrimaryKey,
                              TInt aServiceId,
                              TBool aScrollOver /*= ETrue*/)
    {    
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::NewL()	start" ));
    CIMCVAppRichTextContainer* self = new( ELeave ) CIMCVAppRichTextContainer(aAppUi,
                                        aTextView, 
									    aExtensionsHandler, aFromMe, aToMe, aMap,aServiceId,
									    aScrollOver );

    CleanupStack::PushL( self );
    self->ConstructL(aPrimaryKey);
    CleanupStack::Pop( self );
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::NewL()	end" ));
    return self;
    }


// Destructor
CIMCVAppRichTextContainer::~CIMCVAppRichTextContainer()
    {
    if(iPrimaryKey)
        {
        delete iPrimaryKey;
        iPrimaryKey = NULL;
        }
    delete iParaFormatLayer;
    delete iCharFormatLayer;
    delete iRichText;
    iMessages.ResetAndDestroy();
    if(iAppUi)
        {
        iAppUi->RemoveLayoutChangeObserver( this );  
        iAppUi->RemoveResourceChangeObserver( this );  
        }
    }


// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::SelectedItem
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
HBufC* CIMCVAppRichTextContainer::SelectedItemL()
    {
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::SelectedItemL()	start" ));
    if( iHighlightState == EItemSelected )
        {
        TCursorSelection curSel( iMessages[iSelected]->Selection() );
        
        // calculate item highlight position        
        TCursorSelection itemSel( 
        					iMessages[iSelected]->Highlights()[iSelectedItem] );

        TInt messageStart( curSel.LowerPos() );
        curSel.iAnchorPos = itemSel.iAnchorPos + messageStart;
        curSel.iCursorPos = itemSel.iCursorPos + messageStart;
        
        HBufC* text = HBufC::NewMaxL( curSel.Length() );
        TPtr ptr( text->Des() );
        iRichText->Extract(	ptr, curSel.LowerPos(), curSel.Length() );
	    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::SelectedItemL()	end" ));
        return text;
        }
    else
        {
	    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::SelectedItemL()	end" ));
        return KNullDesC().AllocL();
        }
    }
    
// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::SelectedItemType
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVAppRichTextContainer::SelectedItemType()
    {
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::SelectedItemType()	start" ));
    if( (iHighlightState == EItemSelected) && ( iMessages.Count() >0  )
    	&& (iSelected >= 0  ) )
        {
	    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::SelectedItemType()	end" ));
	    
        return iMessages[iSelected]->HighlightTypes()[iSelectedItem];
        }
    else
        {
    	IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::SelectedItemType()	end" ));
        return KErrNotFound;
        }
    }

// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::RichText
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MLayDoc* CIMCVAppRichTextContainer::TextLayout()
    {
    return iRichText;
    }
// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::RichText
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVAppRichTextContainer::ServiceId() const
    {
    return iServiceId;
    }
// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::SetItemHighlight
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppRichTextContainer::SetItemHighlight( TBool aItemHighlight )
    {
    iItemHighlight = aItemHighlight;
	TInt richTextDocumentLength = iRichText->DocumentLength();
	if (!richTextDocumentLength)
		return;
	
	if (aItemHighlight)
	{
		HBufC* text = HBufC::NewMaxLC( richTextDocumentLength );
		TPtr txt( text->Des() );

		iRichText->Extract( txt);
		CFindItemEngine* findItem = CFindItemEngine::NewL( txt, 
									CFindItemEngine::TFindItemSearchCase(
									CFindItemEngine::EFindItemSearchPhoneNumberBin |
									CFindItemEngine::EFindItemSearchMailAddressBin |
									CFindItemEngine::EFindItemSearchURLBin/* |
									CFindItemEngine::EFindItemSearchScheme*/ )
									);									   
		
		CleanupStack::PushL( findItem );
		const CArrayFixFlat<CFindItemEngine::SFoundItem>* items = findItem->ItemArray();
		TInt count( items->Count() );
		
		for( TInt i(0); i<count; ++i )
			{
			const CFindItemEngine::SFoundItem item = items->At(i);
			
			// selection in rich text
			TCursorSelection realSel( item.iStartPos + item.iLength, item.iStartPos );
			
			HighLightItemL( realSel );
			}

		CleanupStack::PopAndDestroy( findItem );
		CleanupStack::PopAndDestroy( text);
	
	}
	else
    {
    	TCharFormat charFormat;
		TCharFormatMask charFormatMask;
		charFormatMask.SetAttrib( EAttFontUnderline );

		charFormat.iFontPresentation.iUnderline = EUnderlineOff;

		iRichText->ApplyCharFormatL ( charFormat, charFormatMask, 0, richTextDocumentLength);
	}
    }
// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::MoveHighlightL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVAppRichTextContainer::MoveHighlightL( THighlightMovement aDirection )
    {
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::MoveHighlightL()	start" ));
    TBool up( aDirection == EPrevious );
    TInt messageCount( iMessages.Count() );    
    if( messageCount == 0 )
        {
        // no messages, nothing to highlight
        //but make highlight state as EMessageBottom 
        //so that tabs can be switched when there is no conversation in text container.
        if(iHighlightState == EMessageBottom)
            {
            iHighlightState = ENoHighlight;            
            }
        else
            {
            iHighlightState = EMessageBottom;   
            }
        return KErrNone;
        }
    
   
    TInt highlightCount( iMessages[iSelected]->Highlights().Count() );
    
    if( !iItemHighlight )
        {
        // if item highlighting is not on, clear the item count
        highlightCount = 0;
        }    
        
    // if message is highlighted, but it doesn't fit to screen
    // scroll it line by line
    if( iHighlightState == EMessageTop || iHighlightState == EMessageBottom )
        {
        TCursorSelection& sel = iMessages[iSelected]->Selection();
        if( up )
            {
            // up
            if( !iTextView->IsVisible( sel.LowerPos() ) )
                {
                // we're scrolling up and top line is not shown, 
                // so scroll messages down by one line
                iTextView->ScrollLinesL( 1 );
                return KErrNone;
                }
            }
        else if( !iTextView->IsVisible( sel.HigherPos() ) )
            {
            // we're scrolling down and bottom line is not shown,
            // so scroll messages up by one line
            iTextView->ScrollLinesL( -1 );
            return KErrNone;
            }        
        }

    // update highlight position    
    switch( iHighlightState )
        {
        case ENoHighlight:  // no highlight, select last  or first message
            {
            if( up )
                {
                iSelected = messageCount - 1;
                
                // Get highlight count of prev message
                // if item highlighting is on
                if ( iItemHighlight )                
                    {
                    highlightCount = iMessages[iSelected]->Highlights().Count();
                    }                
                    
                if ( highlightCount > 0 )
                    {
                    // highlight items inside last message (if any)
                    iHighlightState = EItemSelected;
                    iSelectedItem = highlightCount - 1;
                    }
                else
                    {
                    // bottom of last message
                    iHighlightState = EMessageBottom;
                    }
                }
            else
                {
                // top of first message
                iHighlightState = EMessageTop;
                iSelected = 0;
                }            
            break;
            }
        case EItemSelected: // highlighting items            
            {
            if( up )
                {
                // up, previous item
                --iSelectedItem;
                if( iSelectedItem < 0 )
                    {
                    // end of highlights, focus message
                    iHighlightState = EMessageTop;
                    }
                }
            else
                {
                // down, next item
                ++iSelectedItem;
                if( iSelectedItem >= highlightCount )
                    {
                    // end of highlights
                    if ( iSelected < messageCount - 1 )
                        {
                        // focus next message
                        ++iSelected;
                        iHighlightState = EMessageTop;
                        }
                    else if ( iScrollOver )
                        {
                        // going down from last message,
                        // disable highlight so editor can take the focus
                        DisableHighlightL();
                        return KErrNone;
                        }
                    else
                        {
                        // Loop back to beginning
                        iSelected = 0;
                        iSelectedItem = 0;
                        iHighlightState = EMessageTop;
                        }
                    
                    }
                }
            break;
            }
        case EMessageTop:   // highlighting message ("top" of it)
            {
            if( up )
                {
                // up, highlighting previous message
                if( iSelected > 0 )
                    {
                    // prev
                    --iSelected;
                    
                    // get highlight count of prev message
                    // if item highlighting is on                
                    if ( iItemHighlight )                
                        {
                        highlightCount = iMessages[iSelected]->Highlights().Count();
                        }
                    
                    if ( highlightCount > 0 )
                        {
                        // highlight items inside prev message (if any)
                        iHighlightState = EItemSelected;
                        iSelectedItem = highlightCount - 1;
                        }
                    else
                        {
                        // highlight whole prev message
                        iHighlightState = EMessageBottom;
                        }    
                    }
                else if ( iScrollOver )
                    {
                    // going up from first message,
                    // disable highlight, so editor can take the focus
                    DisableHighlightL();
                    return KErrNone;
                    }
                else
                    {
                    // Loop to last message
                    iSelected = messageCount-1;
                    highlightCount = 0;
                    
                    // Get highlight count of last message
                    // if item highlighting is on
                    if ( iItemHighlight )                
                        {
                        highlightCount =
                            iMessages[iSelected]->Highlights().Count();
                        }
                    
                    if ( highlightCount > 0 )
                        {
                        // Highlight items inside last message (if any)
                        iHighlightState = EItemSelected;
                        iSelectedItem = highlightCount - 1;
                        }
                    else
                        {
                        // Highlight whole last message
                        iHighlightState = EMessageBottom;
                        }
                    }                      
                }
            else
                {
                // down, highlight items inside this message (if any)
                if( highlightCount > 0 )
                    {
                    iSelectedItem = 0;
                    iHighlightState = EItemSelected;
                    }
                else if( iSelected < messageCount-1 )
                    {
                    // next
                    ++iSelected;
                    iHighlightState = EMessageTop;
                    }
                else if( iScrollOver )
                    {
                    // going down from last message,
                    // disable highlight, so editor can take the focus
                    DisableHighlightL();
                    return KErrNone;
                    }
                else
                    {
                    // Loop back to beginning
                    iSelected = 0;
                    iSelectedItem = 0;
                    iHighlightState = EMessageTop;
                    }
                }
            break;
            }
        case EMessageBottom:   // highlighting message ("bottom" of it)
            {
            if( up )
                {
                // up, highlight items inside this message (if any)
                if( highlightCount > 0 )
                    {
                    iSelectedItem = highlightCount-1;
                    iHighlightState = EItemSelected;
                    }
                else if( iSelected > 0 )
                    {
                    // prev
                    --iSelected;
                    
                    // get highlight count of prev message
                    // if item highlighting is on                
                    if ( iItemHighlight )                
                        {
                        highlightCount = iMessages[iSelected]->Highlights().Count();
                        }
                    
                    if ( highlightCount > 0 )
                        {
                        // highlight items inside prev message (if any)
                        iHighlightState = EItemSelected;
                        iSelectedItem = highlightCount - 1;
                        }
                    else
                        {
                        iHighlightState = EMessageBottom;
                        }    
                    }
                else if( iScrollOver )
                    {
                    // going up from first message,
                    // disable highlight, so editor can take the focus
                    DisableHighlightL();
                    return KErrNone;
                    }
                else
                    {
                    // Loop to last message
                    iSelected = messageCount-1;
                    highlightCount = 0;
                    
                    // Get highlight count of last message
                    // if item highlighting is on
                    if ( iItemHighlight )                
                        {
                        highlightCount =
                            iMessages[iSelected]->Highlights().Count();
                        }
                    
                    if ( highlightCount > 0 )
                        {
                        // Highlight items inside last message (if any)
                        iHighlightState = EItemSelected;
                        iSelectedItem = highlightCount - 1;
                        }
                    else
                        {
                        // Highlight whole last message
                        iHighlightState = EMessageBottom;
                        }
                    }
                }
            else
                {
                // down, highlighting next message
                if( iSelected < messageCount-1 )
                    {
                    // next
                    ++iSelected;
                    iHighlightState = EMessageTop;
                    }
                else if( iScrollOver )
                    {                    
                    // going down from last message,
                    // disable highlight, so editor can take the focus
                    DisableHighlightL();
                    return KErrNone;
                    }
                else
                    {
                    // Loop back to beginning
                    iSelected = 0;
                    iSelectedItem = 0;
                    iHighlightState = EMessageTop;
                    }
                }
            break;
            }
        default:
            {
        
            break;
            }
        }

    // get the selection
    TCursorSelection curSel( CurrentSelection() );    
    
    // remove colors from old highlight
    TInt textLen( iRichText->DocumentLength() );
    if( iPrevious.iAnchorPos < textLen &&
        iPrevious.iCursorPos <= textLen )
        {
        BackColorL( iPrevious, KRgbWhite );
        TextBackColorL( iPrevious, KRgbWhite );        
        }
        
    // and set new one
    if ( iHighlightState == EItemSelected )
        {
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        TRgb color;

        TInt error = AknsUtils::GetCachedColor( skin,
                color,
                KAknsIIDQsnHighlightColors,
                EAknsCIQsnHighlightColorsCG2 );

        if ( !error )
            {
            // No error, use skinned background color
            TextBackColorL( curSel, color );
            }
        else
            {
            TextBackColorL( curSel, AKN_LAF_COLOR_STATIC( KIMCVHighlightColor ) );        
            }
        }
    else
        {
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        TRgb color;

        TInt error = AknsUtils::GetCachedColor( skin,
                color,
                KAknsIIDQsnHighlightColors,
                EAknsCIQsnHighlightColorsCG2 );
        
        if ( !error )
            {
            // No error, use skinned background color
            BackColorL( curSel, color );
            }
        else
            {
            BackColorL( curSel, AKN_LAF_COLOR_STATIC( KIMCVHighlightColor ) );
            }
        }

    // handle changed format
    TCursorSelection changed( Union( iPrevious, curSel ) );
    

    iTextView->HandleFormatChangedL( changed, ETrue );

    iTextView->ScrollVisibleL( curSel, ETrue );
    
    iPrevious = curSel;  
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::MoveHighlightL()	end" ));
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::DisableHighlightL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppRichTextContainer::DisableHighlightL()
    {
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::DisableHighlightL()	start" ));
    iHighlightState = ENoHighlight;
    
    TCursorSelection sel( 0,0 );
    TInt count( iMessages.Count() );
    if( count > 0 )
        {
        sel = iMessages[count-1]->Selection();
        }
    
    // remove colors from old highlight (if any)    
    TInt textLen( iRichText->DocumentLength() );
    if( iPrevious.iAnchorPos < textLen &&
        iPrevious.iCursorPos <= textLen )
        {
        BackColorL( iPrevious, KRgbWhite );
        TextBackColorL( iPrevious, KRgbWhite );
        iTextView->HandleFormatChangedL( iPrevious );
        }
    
    // set focus to last message
    iTextView->ScrollVisibleL( sel, EFalse );
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::DisableHighlightL()	end" ));
    }

    
// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::Highlighted
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CIMCVAppRichTextContainer::Highlighted()
    {
    return iHighlightState != ENoHighlight;
    }

// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::Highlighted
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVAppRichTextContainer::HighlightItemAtPosL( TInt aPos )
    {
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::HighlightItemAtPosL()	start" ));
    TInt messagecount = iMessages.Count();
    if( messagecount == 0 )
        {
        // no messages
        return KErrNotFound;
        }
    
    // Store old values
    TInt oldSelectedMessage = iSelected;
    TInt oldSelectedItem = iSelectedItem;
    THighlightState oldHighlight = iHighlightState;
    
    // Find message in position aPos
    TBool found = EFalse;
    for( TInt i = 0; i < messagecount; ++i )
        {
        TCursorSelection sel = iMessages[ i ]->WholeSelection();
        if( aPos >= sel.LowerPos() && 
            aPos <= sel.HigherPos() )
            {
            // Found the message
            iSelected = i;
            iHighlightState = EMessageTop;
            found = ETrue;
            // stop searching
            break;
            }
        }
    
    if( !found )
        {
        // not found
        return KErrNotFound;
        }
    
    // Check if there are items inside the message that can be highlighted.
    if( iItemHighlight )
        {
        TInt hlCount = iMessages[ iSelected ]->Highlights().Count();
        TInt relativePos = 
            aPos - iMessages[ iSelected ]->WholeSelection().LowerPos();
        
        for( TInt i = 0; i < hlCount; ++i )
            {
            TCursorSelection sel = iMessages[ iSelected ]->Highlights()[ i ];
            
            // Highlight's selection is relative to message
            if( relativePos >= sel.LowerPos() && 
                relativePos <= sel.HigherPos() )
                {
                // Found an item
                iHighlightState = EItemSelected;
                iSelectedItem = i;
                // stop searching
                break;
                }
            }
        }
    
    // Update UI only if selection has changed
    if( iSelected != oldSelectedMessage ||
        iSelectedItem != oldSelectedItem || 
        oldHighlight != iHighlightState )
        {
        UpdateSelectionL();
        return KErrNone;
        }
    // Already highlighted
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::HighlightItemAtPosL()	end" ));
    return KErrAlreadyExists;
    }

// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::UpdateSelection
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppRichTextContainer::UpdateSelectionL()
    {
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::UpdateSelectionL()	start" ));
    // get the selection
    TCursorSelection curSel( CurrentSelection() );    
    
    // remove colors from old highlight
    TInt textLen( iRichText->DocumentLength() );
    if( iPrevious.iAnchorPos < textLen &&
        iPrevious.iCursorPos <= textLen )
        {
        BackColorL( iPrevious, KRgbWhite );
        TextBackColorL( iPrevious, KRgbWhite );        
        }
        
    // and set new one
    if ( iHighlightState == EItemSelected )
        {
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        TRgb color;

        TInt error = AknsUtils::GetCachedColor( skin,
                color,
                KAknsIIDQsnHighlightColors,
                EAknsCIQsnHighlightColorsCG2 );

        if ( !error )
            {
            // No error, use skinned background color
            TextBackColorL( curSel, color );
            }
        else
            {
            TextBackColorL( curSel, AKN_LAF_COLOR_STATIC( KIMCVHighlightColor ) );        
            }
        }
    else
        {
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        TRgb color;

        TInt error = AknsUtils::GetCachedColor( skin,
                color,
                KAknsIIDQsnHighlightColors,
                EAknsCIQsnHighlightColorsCG2 );
        
        if ( !error )
            {
            // No error, use skinned background color
            BackColorL( curSel, color );
            }
        else
            {
            BackColorL( curSel, AKN_LAF_COLOR_STATIC( KIMCVHighlightColor ) );
            }
        }

    // handle changed format
    TCursorSelection changed( Union( iPrevious, curSel ) );

    iTextView->HandleFormatChangedL( changed );
    iTextView->ScrollVisibleL( curSel, ETrue );

    
    iPrevious = curSel;    
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::UpdateSelectionL()	end" ));
    }

// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::InsertTimeStampL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCursorSelection CIMCVAppRichTextContainer::InsertTimeStampL( 
										CIMCVAppMessageWrapper& aMessageWrapper )
    {
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::InsertTimeStampL()	start" ));
    TInt start = 0;
    TCursorSelection timeSel( start, start );
   
  
    // format time
    HBufC* timeStamp = HBufC::NewLC( KTimeStampMaxLength + KSpace().Length() );
    TPtr timePtr( timeStamp->Des() );
    
    HBufC* timeStampFormat = NULL;
    timeStampFormat = StringLoader::LoadLC( R_QTN_TIME_USUAL_WITH_ZERO );
    
    TTime time = aMessageWrapper.Message().TimeStamp();
    TRAPD( err, time.FormatL( timePtr, *timeStampFormat ) );
    if ( err == KErrOverflow )
        {
        // Reserved text space was not enough.
        // Adjust the KTimeStampMaxLength!
    

        // in release builds return without timestamp
        CleanupStack::PopAndDestroy( 2, timeStamp ); // timeStamp, timeStampFormat
        return timeSel;
        }
    
    // Convert numbers to local language
    AknTextUtils::LanguageSpecificNumberConversion( timePtr );
    
    // Set Alignment of Pm/Am from local setting
    // This keeps the am/pm text close to the time.
    timePtr.Append( AknLayoutUtils::LayoutMirrored() ? 
                    KUnicodeRLM : KUnicodeLRM );
    // append time to rich text
    timePtr.Append( KSpace );
    iRichText->InsertL( start, timePtr );
    
    CleanupStack::PopAndDestroy( 2, timeStamp ); // timeStamp, timeStampFormat
    timeSel.SetSelection( start, timePtr.Length() - 1 );

    // bold the time
    if ( timeSel.Length() > 0 )
        {
        BoldL( timeSel );
        }
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::InsertTimeStampL()	end" ));
    return timeSel;
    }

// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::InsertContentL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVAppRichTextContainer::InsertContentL( CIMCVAppMessageWrapper& aMessageWrapper )
    {    
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::InsertContentL()	start" ));
    MIMCVEngineMessage& message = aMessageWrapper.Message();

    MIMCVEngineMessage::TContentType type( message.ContentType() );
    switch( type ) 
        {
        case MIMCVEngineMessage::EContentText:
            {
            TInt oldLen = iRichText->DocumentLength();
            
            // Insert text
            TPtrC msg( message.Text() );
            
            HBufC* tempMsg = NULL;
            
            TInt paraBreak = msg.Locate( CEditableText::EParagraphDelimiter );
            // Replace all paragraph delimiters with line breaks
            // to keep alingment consistent
            if ( paraBreak != KErrNotFound )
                {
                tempMsg = msg.AllocLC();
                TPtr ptr( tempMsg->Des() );
                ptr.Zero();
                
                ReplaceParaDelimsWithLineBreaks( msg, ptr );
                msg.Set( ptr );
                }

            HBufC* numberConv = NULL;
            
            // Convert numerals to local language
            // 1) date item
            if ( aMessageWrapper.Message().SystemMessageType() ==
                MIMCVEngineMessage::ESystemMessageDateChange )
                {
                // Don't touch the original message
                numberConv = msg.AllocLC();
                TPtr ptr( numberConv->Des() );
                AknTextUtils::LanguageSpecificNumberConversion( ptr );
                msg.Set( ptr );
                }

            iRichText->InsertL( 0, msg );
             
            TInt textEnd = msg.Length();
                
            // Check for extensions (smileys)
            TCursorSelection selection( 0, textEnd );
            TInt bfrConv = iRichText->DocumentLength();
            iExtensionsHandler->ConvertSelectionToExtensionL( *iRichText, 
            												 selection,
            												 iServiceId );
            
            // Move textEnd index if extensions were found
            textEnd -= ( bfrConv - iRichText->DocumentLength() );
            
            // Add nbs to preserve formatting
            iRichText->InsertL( textEnd,
                                CEditableText::EZeroWidthNoBreakSpace );
            
            // If this is system message, it should be bold
            if( message.MessageType() == MIMCVEngineMessage::EMessageSystem )
                {
                TCursorSelection sel( 0, textEnd );
                BoldL( sel );
                // System messages need different kind of alignment
                CParaFormat paraFormat;
                TParaFormatMask paraFormatMask;
                paraFormat.iLeftMarginInTwips = 0;
                paraFormatMask.SetAttrib( EAttLeftMargin );    
                iRichText->ApplyParaFormatL( &paraFormat, paraFormatMask,
                                             0, textEnd + 1 );
                }
            else
                {
                // Apply alignment for "normal" messages
                CParaFormat paraFormat;
                TParaFormatMask paraFormatMask;
                paraFormat.iLeftMarginInTwips = 100;
                paraFormatMask.SetAttrib( EAttLeftMargin );    
                iRichText->ApplyParaFormatL( &paraFormat, paraFormatMask,
                                             0, textEnd + 1 );
                
                // Remove bolding from message content
                TCharFormat charFormat;
                TCharFormatMask charFormatMask;
                charFormat.iFontSpec.iFontStyle.SetStrokeWeight( EStrokeWeightNormal );
                charFormatMask.SetAttrib( EAttFontStrokeWeight );
                iRichText->ApplyCharFormatL( charFormat, charFormatMask, 
                                 0, textEnd + 1 );
                }
            
            // Cleanup
            if ( numberConv )
                {
                CleanupStack::PopAndDestroy( numberConv );
                }
            if ( tempMsg )
                {
                CleanupStack::PopAndDestroy( tempMsg );
                }
            
            return iRichText->DocumentLength() - oldLen;            
            }
        default:
            {
            // Unsupported type
         
            break;
            }
        }
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::InsertContentL()	end" ));
    return 0;
    }
    
// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::InsertNickL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCursorSelection CIMCVAppRichTextContainer::InsertNickL( 
										CIMCVAppMessageWrapper& aMessageWrapper )
    {
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::InsertNickL()	start" ));
    // insert nick    
    MIMCVEngineMessage& message = aMessageWrapper.Message();
    TPtrC sender;
    TPtrC recipient;
    if ( message.MessageType() == MIMCVEngineMessage::EMessageGroup ||
         message.MessageType() == MIMCVEngineMessage::EMessageWhisper )
        {
        sender.Set( message.Sender() );
        recipient.Set( message.Recipient() );
        }
   
    TCursorSelection nickSel( 0,0 );
    
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::InsertNickL()	end" ));
    return nickSel;
    }
        
// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::LayoutChangedL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppRichTextContainer::LayoutChangedL( TInt aType)
    {
    #if 0
    //if layout change.. then dont update skin text color.
    if (aType == KAknsMessageSkinChange)
    	{
    	UpdateSkinnedTextColourL();
    	}
	#endif
    }

// -----------------------------------------------------------------------------
// CCARichTextContainer::ResourceChangedL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppRichTextContainer::ResourceChangedL()
    {
    // When resource changes (branding):
    // clear content, so that old pictures don't cause any problems.
    // we are reloaded anyway.
    iRichText->DeleteL( 0, iRichText->DocumentLength() );
    iTextView->HandleGlobalChangeNoRedrawL();
    }
// CIMCVAppRichTextContainer::AddMessageL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppRichTextContainer::AddMessageL( MIMCVEngineMessage& aMessage )
    {   
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::AddMessageL()	start" ));
    if( !iShowTimeStamps &&
        aMessage.SystemMessageType() == MIMCVEngineMessage::ESystemMessageDateChange )
        {
        // if timestamp setting is OFF we don't show day change items
        return;
        }
    
    TInt oldCount = iMessages.Count();
    
    // create wrapper
    CIMCVAppMessageWrapper* wrapper = CIMCVAppMessageWrapper::NewL( aMessage );
                
    CleanupStack::PushL( wrapper );
    iMessages.AppendL( wrapper );   // takes the ownership
    CleanupStack::Pop( wrapper );
    
    
    // start new paragraph
    TInt lineStart = iRichText->DocumentLength();
    if( lineStart != 0 )
        {
        iRichText->InsertL( lineStart, CEditableText::EParagraphDelimiter );
        }
    
    // Set Alignment from local layout
    iRichText->InsertL( iRichText->DocumentLength(),
            AknLayoutUtils::LayoutMirrored() ? KUnicodeRLM : KUnicodeLRM );
        
    // add message to rich text    
    TInt startPos( iRichText->DocumentLength() );
    TCursorSelection timeSel = AddTimeStampL( *wrapper );
    TCursorSelection nickSel = AddNickL( *wrapper );
    AddContentL( startPos, *wrapper );
    TInt endPos( iRichText->DocumentLength() );    
    
    // set selection for highlight and the whole selection 
    // (so that this can be removed)
    // append deletioncount to new messages
    wrapper->Selection().SetSelection(    endPos - 1, startPos );
    wrapper->WholeSelection().SetSelection( endPos, lineStart );
    wrapper->MessageSelection().SetSelection( endPos - 1, nickSel.HigherPos() );
    
    
    // append coloring
    TCursorSelection& sel = iColorWholeMessage ? wrapper->Selection() : nickSel;
    TInt len( sel.Length() );
   

    // Make sure the background (highlight) is not copied from 
    // previous message.
    BackColorL( sel, KRgbWhite );
    TextBackColorL( sel, KRgbWhite );        
    
    if( len > 0 && 
        aMessage.MessageType() != MIMCVEngineMessage::EMessageSystem )
        {
        // custom colors
        if( aMessage.MessagerType() == MIMCVEngineMessage::EMessageSent )
            {
            // own msg
            if( iOwnMsgColorInUse )
                {
                TextColorL( sel, iOwnMsgColor );
                }
            }
        else if( aMessage.MessagerType() == MIMCVEngineMessage::EMessageReceived )
            {
            // other msg
            if( iMsgColorInUse )
                {
                TextColorL( sel, iMsgColor );
                }
            }    
        }
        
#ifdef RD_30_COMPATIBILITY_MODE    
    if( lineStart == 0 )
        {
        // This is needed only in 3.0 backport so that the alignment
        // character doesn't cause a virtual linebreak.
        TRAP_IGNORE( iTextView->HandleFormatChangedL( 
                        wrapper->WholeSelection() ) );
        }
#endif // RD_30_COMPATIBILITY_MODE
    
    // inform of inserted rich text
    TCursorSelection curSel( CurrentSelection() );
    TRAPD( error, iTextView->HandleAdditionL( lineStart == 0, curSel, EFalse ) );
    if( error != KErrNone )
        {
               
        // Remove failed text
        TCursorSelection del = wrapper->WholeSelection();
        iRichText->DeleteL( del.LowerPos(), del.Length() );
        
        // Remove the failed wrapper and leave
        TInt index = iMessages.Find( wrapper );
        if( index != KErrNotFound )
            {
            delete wrapper;
            iMessages.Remove( index );
            }
        
        // Try to handle changes once more
        TRAPD( updateErr, iTextView->HandleGlobalChangeNoRedrawL() );
        if( !updateErr )
            {
            TCursorSelection curSel( CurrentSelection() );
            TRAP_IGNORE( iTextView->ScrollVisibleL( curSel, EFalse ) );
            }
        User::Leave( error );
        }
    
    // and scroll (unless in highlight-mode)
    if( iHighlightState == ENoHighlight )
        {
        TCursorSelection curSel( CurrentSelection() );    
        iTextView->ScrollVisibleL( curSel, EFalse );
        
        // If this is recorded chat container (iScrollOver == EFalse)
        // and added message was first move highlight 
        // so we have focus on the topmost item.
        if ( oldCount == 0 && !iScrollOver )
            {
            MoveHighlightL( ENext );
            }
        }
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::AddMessageL()	end" ));
    }



// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::InsertMessageL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppRichTextContainer::InsertMessageL( MIMCVEngineMessage& aMessage )
    {
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::InsertMessageL()	start" ));
    if ( !iShowTimeStamps &&
        aMessage.SystemMessageType() == MIMCVEngineMessage::ESystemMessageDateChange )
        {
        // if timestamp setting is OFF we don't show day change items
        return;
        }
    
    // create wrapper
    CIMCVAppMessageWrapper* wrapper = CIMCVAppMessageWrapper::NewL( aMessage );
                
    CleanupStack::PushL( wrapper );
    iMessages.InsertL( wrapper, 0 ); // takes the ownership
    CleanupStack::Pop( wrapper );
    
        
    TInt oldLen = iRichText->DocumentLength();
    
    // Insert paragraph break if this is not the last message
    TInt paraBreak = 0;
    if ( iMessages.Count() > 1 )
        {
        iRichText->InsertL( 0, CEditableText::EParagraphDelimiter );
        paraBreak = 1;
        }
    
    // Insert message to rich text in reverse order compared to adding
    TInt contentLen = InsertContentL( *wrapper );
    TCursorSelection nickSel = InsertNickL( *wrapper );
    TCursorSelection timeSel = InsertTimeStampL( *wrapper );

    // Set Alignment from local layout
    TInt dirChrLen = iRichText->DocumentLength();
    iRichText->InsertL( 0, AknLayoutUtils::LayoutMirrored() ? 
                           KUnicodeRLM : KUnicodeLRM );
    dirChrLen = iRichText->DocumentLength() - dirChrLen;
    
    wrapper->SetInserted( ETrue );

    // Update timestamp selection
    timeSel.iAnchorPos += dirChrLen;
    timeSel.iCursorPos += dirChrLen;
    // Update nick selection
    nickSel.iAnchorPos += timeSel.HigherPos();
    nickSel.iCursorPos += timeSel.HigherPos();
    // Update thumb pos
    TCursorSelection& thumbPos = wrapper->ThumbPos();
    thumbPos.iAnchorPos += nickSel.HigherPos() + KSeparator().Length();
    thumbPos.iCursorPos += nickSel.HigherPos() + KSeparator().Length();
    
    TInt addedLen = iRichText->DocumentLength() - oldLen;
    
    // parse text for highlights and store positions to messagewrapper
    HBufC* text = HBufC::NewMaxLC( contentLen );
    TPtr txt( text->Des() );
    iRichText->Extract( txt,
        nickSel.HigherPos() + KSeparator().Length() + 1, // Don't extract separator and space
        contentLen );
    ParseTextL( txt, nickSel.HigherPos() + KSeparator().Length() + 1,
                nickSel.HigherPos() + KSeparator().Length() + 1, *wrapper );
    CleanupStack::PopAndDestroy( text );
    
    // Set selection for highlight and the whole selection
    // so that this message can be removed   
    wrapper->Selection().SetSelection( addedLen - 1 - paraBreak, 0 );
    wrapper->WholeSelection().SetSelection( addedLen - 1, 0 );
    wrapper->MessageSelection().SetSelection( addedLen - 1,
                                               nickSel.HigherPos() + 1 );
    
    if ( iHighlightState != ENoHighlight )
        {
        BackColorL( wrapper->WholeSelection(), KRgbWhite );
        TextBackColorL( wrapper->WholeSelection(), KRgbWhite );
        }
        
    // Update wrappers after this if there is any
    if ( iMessages.Count() > 1 )
        {
        UpdateWrappers( 1, addedLen );
        }
    
    // Append coloring
    TCursorSelection& sel = iColorWholeMessage ? wrapper->Selection() : nickSel;
    TInt len( sel.Length() );
       
    if ( len > 0 && 
         aMessage.MessageType() != MIMCVEngineMessage::EMessageSystem )
        {
        // Custom colors
        if ( aMessage.MessagerType() == MIMCVEngineMessage::EMessageSent )
            {
            // Own msg
            if( iOwnMsgColorInUse )
                {
                TextColorL( sel, iOwnMsgColor );
                }
            }
        else if ( aMessage.MessagerType() == MIMCVEngineMessage::EMessageReceived )
            {
            // Other msg
            if( iMsgColorInUse )
                {
                TextColorL( sel, iMsgColor );
                }
            }    
        }
    
    // Inform of inserted rich text
    TInt error = KErrNone;
    if ( oldLen == 0 )
        {
        TCursorSelection curSel( CurrentSelection() );
        // First message inserted, format the whole text
        TRAP( error, iTextView->HandleAdditionL( ETrue, curSel, EFalse ) );
        }
    else
        {
        TCursorSelection curSel( CurrentSelection() );
        // There might be a faster method to handle text addition, but 
        // this works for now.
        TRAP( error, iTextView->HandleAdditionL( ETrue, curSel, EFalse) );
        }    
    // And scroll (unless in highlight-mode)
    if ( iHighlightState == ENoHighlight )
        {
        TCursorSelection curSel( CurrentSelection() );    
        iTextView->ScrollVisibleL( curSel, EFalse );
        }
    else
        {
        // In highlight mode, message inserted -> increase selected
        // message index and previous selection indexes
        iSelected++;
        iPrevious.iAnchorPos += addedLen;
        iPrevious.iCursorPos += addedLen;
            
        TCursorSelection curSel( CurrentSelection() );
        iTextView->ScrollVisibleL( curSel, EFalse );     
        }
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::InsertMessageL()	end" ));
    }
    
// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::UpdateWrappers
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppRichTextContainer::UpdateWrappers( TInt aFirstIndex, TInt aChange )
    {
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::UpdateWrappers()	start" ));
    TInt count( iMessages.Count() );
    for( TInt i( aFirstIndex ); i<count; ++i )
        {
        CIMCVAppMessageWrapper* wrapper = iMessages[i];
        TCursorSelection& selection = wrapper->Selection();
        TCursorSelection& wholeSelection = wrapper->WholeSelection();
        TCursorSelection& thumbPos = wrapper->ThumbPos();
        selection.iAnchorPos += aChange;
        selection.iCursorPos += aChange;
        wholeSelection.iAnchorPos += aChange;
        wholeSelection.iCursorPos += aChange;
        thumbPos.iAnchorPos += aChange;
        thumbPos.iCursorPos += aChange;
        }
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::UpdateWrappers()	end" ));
    }
    
// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::Union
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCursorSelection CIMCVAppRichTextContainer::Union( const TCursorSelection& aSel1, 
												const TCursorSelection& aSel2 )
    {
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::Union()	start" ));
    // make union of selections
    TCursorSelection changed;
    changed.iAnchorPos = aSel1.LowerPos();
    changed.iCursorPos = aSel1.HigherPos();

    TInt low(  aSel2.LowerPos() );
    TInt high( aSel2.HigherPos() );
    if( low < changed.iAnchorPos )
        {
        changed.iAnchorPos = low;
        }
    if( high > changed.iCursorPos )
        {
        changed.iCursorPos = high;
        }
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::Union()	 end" ));
    return changed;
    }
    
// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::CurrentSelection
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCursorSelection CIMCVAppRichTextContainer::CurrentSelection() const
    {
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::CurrentSelection()	 start" ));
    TCursorSelection sel( 0,0 );
    if( iHighlightState == ENoHighlight )
        {
        // if nothing highlighted, return last message
        TInt count( iMessages.Count() );
        if( count > 0 )
            {
            sel = iMessages[count-1]->Selection();
            }    
        }
    else
        {
        // something highlighted, start with current message
        sel = iMessages[iSelected]->Selection();
        
        // check if the highlighted selection is item inside message
        if( iHighlightState == EItemSelected )
            {
            // calculate item highlight position
            TCursorSelection itemSel( 
            			iMessages[iSelected]->Highlights()[iSelectedItem] );

            TInt messageStart( sel.LowerPos() );
            sel.iAnchorPos = itemSel.iAnchorPos + messageStart;
            sel.iCursorPos = itemSel.iCursorPos + messageStart;
            }
        }
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::CurrentSelection()	 end" ));
    return sel;
    }
    

// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::TextBackColorL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppRichTextContainer::TextBackColorL( const TCursorSelection& aSel, 
											const TRgb& aColor )
    {
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::TextBackColorL()	 start" ));
    TCharFormat charFormat;
    TCharFormatMask charFormatMask;
    charFormatMask.SetAttrib( EAttFontHighlightColor );
    charFormatMask.SetAttrib( EAttFontHighlightStyle );
    
    charFormat.iFontPresentation.iHighlightColor = aColor;
    charFormat.iFontPresentation.iHighlightStyle = aColor == KRgbWhite ? 
                                    TFontPresentation::EFontHighlightNone :
                                    TFontPresentation::EFontHighlightNormal;
    
    iRichText->ApplyCharFormatL( charFormat, charFormatMask, 
                                 aSel.LowerPos(), 
                                 aSel.Length() );
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::TextBackColorL()	 end" ));                         
    }

// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::TextColorL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppRichTextContainer::TextColorL( const TCursorSelection& aSel, 
										const TRgb& aColor )
    {
	IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::TextColorL()	 start" ));
    TCharFormat charFormat;
    TCharFormatMask charFormatMask;
    charFormatMask.SetAttrib( EAttColor );    
    charFormat.iFontPresentation.iTextColor = aColor;

    iRichText->ApplyCharFormatL( charFormat, charFormatMask, 
                                 aSel.LowerPos(), 
                                 aSel.Length() );
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::TextColorL()	 end" ));
    }

// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::BackColorL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppRichTextContainer::BackColorL( const TCursorSelection& aSel, 
										const TRgb& aColor )
    {
	IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::BackColorL()	 start" ));
    CParaFormat paraFormat;
    TParaFormatMask paraFormatMask;
    paraFormatMask.SetAttrib( EAttFillColor );

    paraFormat.iFillColor = aColor;
    iRichText->ApplyParaFormatL( &paraFormat, paraFormatMask, 
                                 aSel.LowerPos(), 
                                 aSel.Length() );
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::BackColorL()	 end" ));
    }

// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::HighLightItemL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppRichTextContainer::HighLightItemL( const TCursorSelection& aSel )
    {

    TCharFormat charFormat;
    TCharFormatMask charFormatMask;
    charFormatMask.SetAttrib( EAttFontUnderline );
    
    charFormat.iFontPresentation.iUnderline = EUnderlineOn;
    
    iRichText->ApplyCharFormatL( charFormat, charFormatMask, 
                                 aSel.LowerPos(), 
                                 aSel.Length() );
    }


// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::GetMessageCount
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVAppRichTextContainer::GetMessageCount()
	{
	return iMessages.Count();	
	}

// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::BoldL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppRichTextContainer::BoldL( const TCursorSelection& aSel )
    {
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::BoldL()	 start" ));
#ifdef _DEBUG
    TInt len( iRichText->DocumentLength() );
    
#endif //_DEBUG

    TCharFormat charFormat;
    TCharFormatMask charFormatMask;    
    
    charFormat.iFontSpec.iFontStyle.SetStrokeWeight( EStrokeWeightBold );
    charFormatMask.SetAttrib( EAttFontStrokeWeight );
    
    iRichText->ApplyCharFormatL( charFormat, charFormatMask, 
                                 aSel.LowerPos(),
                                 aSel.Length() );
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::BoldL()	 end" ));
    }
    
// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::ParseTextL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppRichTextContainer::ParseTextL( const TDesC& aText, TInt aStartPos, 
						TInt aRelativePos, CIMCVAppMessageWrapper& aMessageWrapper )
    {
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::ParseTextL()	 start" ));
    CFindItemEngine* findItem = CFindItemEngine::NewL( aText, 
                                CFindItemEngine::TFindItemSearchCase(
                                CFindItemEngine::EFindItemSearchPhoneNumberBin |
                                CFindItemEngine::EFindItemSearchMailAddressBin |
                                CFindItemEngine::EFindItemSearchURLBin/* |
                                CFindItemEngine::EFindItemSearchScheme*/ )
                                );                                     

    CleanupStack::PushL( findItem );
    
    // reset array
    RArray<TCursorSelection>& highlights = aMessageWrapper.Highlights();
    RArray<TInt>& types = aMessageWrapper.HighlightTypes();
    highlights.Reset();
    types.Reset();
    
    // get found items
    const CArrayFixFlat<CFindItemEngine::SFoundItem>* items = 
    	findItem->ItemArray();
    TInt count( items->Count() );
    
    for( TInt i(0); i<count; ++i )
        {
        const CFindItemEngine::SFoundItem item = items->At(i);
        
        // selection in rich text
        TCursorSelection realSel( aStartPos + item.iStartPos + item.iLength,
                                  aStartPos + item.iStartPos );
                              
        // relative selection inside one message
        TCursorSelection relativeSel( aRelativePos + item.iStartPos + 
        							  item.iLength,
                                      aRelativePos + item.iStartPos );

        highlights.AppendL( relativeSel );
        types.AppendL( item.iItemType );
        
        // apply highlight item formatting
        HighLightItemL( realSel );
        }
    
    CleanupStack::PopAndDestroy( findItem );
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::ParseTextL()	 end" ));
    }
    
// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::AddNickL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCursorSelection CIMCVAppRichTextContainer::AddNickL( 
										CIMCVAppMessageWrapper& aMessageWrapper )
    {
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::AddNickL()	 start" ));
    // insert nick    
    MIMCVEngineMessage& message = aMessageWrapper.Message();
    TPtrC sender;
    TPtrC recipient;
    if( message.MessageType() == MIMCVEngineMessage::EMessageGroup ||
        message.MessageType() == MIMCVEngineMessage::EMessageWhisper )
        {
        sender.Set( message.Sender() );
        recipient.Set( message.Recipient() );
        }
    // need to implements todo
    TCursorSelection nickSel( 0,0 );
    //============================================================================
    if( message.MessagerType() == MIMCVEngineMessage::EMessageSent && 
    message.MessageType() == MIMCVEngineMessage::EMessagePTOP)
	    {
	    CIMCVAppPicture* pic = new( ELeave )CIMCVAppPicture( *iMap, iFromMe );
	    TPictureHeader header;
		header.iPicture = TSwizzle<CPicture>(pic );
		iRichText->InsertL( iRichText->DocumentLength(), header );// takes ownership
		iRichText->InsertL( iRichText->DocumentLength(), KSeparator );
	    }
	 
     else if( message.MessageType() == MIMCVEngineMessage::EMessagePTOP &&
         message.MessagerType() == MIMCVEngineMessage::EMessageReceived )
	    {
	    // received p2p message and we want to display icon
	    // insert "to me" icon and separator
	    CIMCVAppPicture* pic = new( ELeave )CIMCVAppPicture( *iMap,iToMe );
	    TPictureHeader header;
	    header.iPicture = TSwizzle<CPicture>( pic ); 
	    iRichText->CancelInsertCharFormat(); // to prevent ETEXT 31 panic
	    iRichText->InsertL( iRichText->DocumentLength(), header ); // takes ownership
	    iRichText->InsertL( iRichText->DocumentLength(), KSeparator );
	    }
    //============================================================================
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::AddNickL()	 end" ));
    return nickSel;
    }
    
// -----------------------------------------------------------------------------
// CIMCVAppSmileIconUtility::LoadDefaultBitmapL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CGulIcon* CIMCVAppRichTextContainer::LoadDefaultBitmapL( TInt aBitmapId, TInt aMaskId, 
                                const TDesC& aFullPath, TBool/* aOverrideBrand */)
	{
    
    IM_CV_LOGS(TXT("CIMCVAppSmileIconUtility::LoadDefaultBitmapL() start") );
    CFbsBitmap* bitmap = NULL;
    CFbsBitmap* mask = NULL;
	
	MAknsSkinInstance* skin = AknsUtils::SkinInstance();	

	TAknsItemID itemId( KAknsIIDNone );
    
    AknsUtils::CreateIconLC( skin, 
                itemId, 
                bitmap, mask, aFullPath, aBitmapId, aMaskId );    

    CGulIcon* gulIcon = CGulIcon::NewL( bitmap, mask );
    CleanupStack::Pop( 2 ); // bitmap, mask
    IM_CV_LOGS(TXT("CIMCVAppSmileIconUtility::LoadDefaultBitmapL() end") );
    return gulIcon;
    }

// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::AddTimeStampL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCursorSelection CIMCVAppRichTextContainer::AddTimeStampL( 
										CIMCVAppMessageWrapper& aMessageWrapper )
    {
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::AddTimeStampL()	 start" ));
    TInt start = iRichText->DocumentLength();
    
    TCursorSelection timeSel( start, start );
   
    
    // format time
    HBufC* timeStamp = HBufC::NewLC( KTimeStampMaxLength + KSpace().Length() );
    TPtr timePtr( timeStamp->Des() );
    
    HBufC* timeStampFormat = NULL;
    timeStampFormat = StringLoader::LoadLC( R_QTN_TIME_USUAL_WITH_ZERO );
    
    TTime time = aMessageWrapper.Message().TimeStamp();
    TRAPD( err, time.FormatL( timePtr, *timeStampFormat ) );
    if( err == KErrOverflow )
        {
        // Reserved text space was not enough.
        // Adjust the KTimeStampMaxLength!
         // in release builds return without timestamp
        CleanupStack::PopAndDestroy( 2, timeStamp); // timeStamp, timeStampFormat
        return timeSel;
        }
    
    // Convert numbers to local language
    AknTextUtils::LanguageSpecificNumberConversion( timePtr );
   
    // Set Alignment of Pm/Am from local setting
    // This keeps the am/pm text close to the time.
    timePtr.Append( AknLayoutUtils::LayoutMirrored() ? 
                    KUnicodeRLM : KUnicodeLRM );
        
    // append time to rich text
    timePtr.Append( KSpace );
    iRichText->InsertL( start, timePtr );
    
    CleanupStack::PopAndDestroy( 2, timeStamp); // timeStamp, timeStampFormat
    timeSel.SetSelection( start, iRichText->DocumentLength() - 1 );

    // bold the time
    if( timeSel.Length() > 0 )
        {
        BoldL( timeSel );
        }
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::AddTimeStampL()	 end" ));
    return timeSel;
    }
    
// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::AddContentL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppRichTextContainer::AddContentL( TInt aMsgStart, 
										CIMCVAppMessageWrapper& aMessageWrapper )
    {  
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::AddContentL()	 start" ));  
    MIMCVEngineMessage& message = aMessageWrapper.Message();

    MIMCVEngineMessage::TContentType type( message.ContentType() );
    switch( type ) 
        {
        case MIMCVEngineMessage::EContentText:
            {
            // insert text
            //If received message is longer than 400 characters it will be truncated in imconnectionprovider
            //so append truncation message to inform user that received message is truncated 
            HBufC* newmsg = HBufC::NewLC(KMsgMaxLength);
            TPtr msg = newmsg->Des();
            if(message.Text().Length() > KReceiveMsgMaxLength)
                {              
                msg.Append(message.Text().Left(KReceiveMsgMaxLength));
                HBufC* addText  = StringLoader::LoadLC( R_QTN_CONVIEW_TRUNCATED );
                msg.Append(addText->Des());
                CleanupStack::PopAndDestroy(addText);//addText             
                }
            else
                {
                msg.Append(message.Text());
                }
          //  TPtrC msg( message.Text() );
            HBufC* tempMsg = NULL;
            
            TInt paraBreak = msg.Locate( CEditableText::EParagraphDelimiter );
            // Replace all paragraph delimiters with line breaks
            // to keep alingment consistent
            if ( paraBreak != KErrNotFound )
                {
                tempMsg = msg.AllocLC();
                TPtr ptr( tempMsg->Des() );
                ptr.Zero();
                
                ReplaceParaDelimsWithLineBreaks( msg, ptr );
                msg.Set( ptr );
                }

            HBufC* numberConv = NULL;
            
            // Convert numerals to local language
            // 1) date item
            if( aMessageWrapper.Message().SystemMessageType() ==
                MIMCVEngineMessage::ESystemMessageDateChange )
            {
                // don't touch the original message
                numberConv = msg.AllocLC();
                TPtr ptr( numberConv->Des() );
                AknTextUtils::LanguageSpecificNumberConversion( ptr );
                msg.Set( ptr );
            }

            TInt textStart( iRichText->DocumentLength() );
            iRichText->InsertL( textStart, msg );
                
            CleanupStack::PopAndDestroy(newmsg);//newmsg
            // check for extensions (smileys)
            TCursorSelection selection( iRichText->DocumentLength(), 
            							textStart );
            iExtensionsHandler->ConvertSelectionToExtensionL( *iRichText, 
            													selection ,
            													iServiceId );
            
            TInt textEnd( iRichText->DocumentLength() );
                
            // add nbs to preserve formatting
            iRichText->InsertL( iRichText->DocumentLength(), 
            					CEditableText::EZeroWidthNoBreakSpace );
            
            // if this is system message, it should be bold
            if( message.MessageType() == MIMCVEngineMessage::EMessageSystem )
                {
                TCursorSelection sel( textStart, textEnd );
                BoldL( sel );
                // System messages need different kind of alignment
                CParaFormat paraFormat;
                TParaFormatMask paraFormatMask;
                paraFormat.iLeftMarginInTwips = 0;
                paraFormatMask.SetAttrib( EAttLeftMargin );    
                iRichText->ApplyParaFormatL( &paraFormat, paraFormatMask,
                                             textStart, sel.Length() );
                }
            else
                {
                // Apply alignment for "normal" messages
                CParaFormat paraFormat;
                TParaFormatMask paraFormatMask;
                paraFormat.iLeftMarginInTwips = 100;
                paraFormatMask.SetAttrib( EAttLeftMargin );    
                iRichText->ApplyParaFormatL( &paraFormat, paraFormatMask,
                                             textStart, textEnd - textStart );
                }

            // parse text for highlights and store positions to messagewrapper
            HBufC* text = HBufC::NewMaxLC( textEnd-textStart );
            TPtr txt( text->Des() );
            iRichText->Extract( txt, textStart, textEnd-textStart );
            ParseTextL( txt, textStart, textStart-aMsgStart, aMessageWrapper );
            CleanupStack::PopAndDestroy( text );
            
            // Cleanup
            if( numberConv )
                {
                CleanupStack::PopAndDestroy( numberConv );
                }
            if ( tempMsg )
                {
                CleanupStack::PopAndDestroy( tempMsg );
                }
            break;            
            }
       
        default:
            {
            // unsupported type
        
            break;
            }
        }
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::AddContentL()	 end" ));  
    }


// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::UpdateSkinnedTextColourL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppRichTextContainer::UpdateSkinnedTextColourL()  
    {
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::UpdateSkinnedTextColourL()	 start" ));  
    // Get current text's color
    TRgb defaultSkinTextColor( KRgbBlack );
    AknsUtils::GetCachedColor( AknsUtils::SkinInstance(),
                               defaultSkinTextColor,
                               KAknsIIDQsnTextColors,
                               EAknsCIQsnTextColorsCG6 );

    TCharFormat charFormat;
    TCharFormatMask charFormatMask;
    charFormat.iFontPresentation.iTextColor = defaultSkinTextColor;
    charFormatMask.SetAttrib( EAttColor );

	// also appended messages should have new colour
	TInt richTextDocumentLength = iRichText->DocumentLength();

	iRichText->ApplyCharFormatL ( charFormat, charFormatMask, 0, richTextDocumentLength);
	TCursorSelection cursorSelection(0, richTextDocumentLength);
	iTextView->HandleFormatChangedL (cursorSelection);
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::UpdateSkinnedTextColourL()	 end" ));  
    }

// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::SkinColoredSelection
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCursorSelection CIMCVAppRichTextContainer::SkinColoredSelection(  
                                            CIMCVAppMessageWrapper& aMessageWrapper,
                                            TBool aUseDefaultColors, 
                                            TBool aCustomDefinedColor )
    {   
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::SkinColoredSelection()	 start" ));  
    // This logic determines which part of the message is colored by the theme
    if( !aCustomDefinedColor && aUseDefaultColors )
        {
        return aMessageWrapper.WholeSelection();
        }
        
    if( !iColorWholeMessage )
        {
        return aMessageWrapper.MessageSelection();
        }
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::SkinColoredSelection()	 end" ));  
    return TCursorSelection( 0, 0 );    
    }

// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::ReplaceParaDelimsWithLineBreaks
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CIMCVAppRichTextContainer::ReplaceParaDelimsWithLineBreaks(
                                            const TPtrC& aSource,
                                            TPtr& aTarget ) const
    {
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::ReplaceParaDelimsWithLineBreaks()	 start" ));  
#ifdef _DEBUG
    TPtrC target;
    target.Set( aTarget );
   
#endif
    
    TInt paraBreak = aSource.Mid( aTarget.Length() ).Locate(
                        CEditableText::EParagraphDelimiter );
    
    if ( paraBreak == KErrNotFound )
        {
        // No more paragraph delimiters, append what's left
        aTarget.Append( aSource.Mid( aTarget.Length() ) );
        // And break away from recursion
        return;
        }
    
    aTarget.Append( aSource.Mid( aTarget.Length(), paraBreak ) );
    aTarget.Append( CEditableText::ELineBreak );

#ifdef _DEBUG
    target.Set( aTarget );
   
#endif

    ReplaceParaDelimsWithLineBreaks( aSource, aTarget );
    IM_CV_LOGS(TXT("CIMCVAppRichTextContainer::ReplaceParaDelimsWithLineBreaks()	 end" ));  
    }

// -----------------------------------------------------------------------------
// CIMCVAppRichTextContainer::GetPrimaryKey
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

const TDesC& CIMCVAppRichTextContainer::GetPrimaryKey()
    {
    if( iPrimaryKey)
        {
        return *iPrimaryKey;
        }
    return KNullDesC;
    }
//  End of File
