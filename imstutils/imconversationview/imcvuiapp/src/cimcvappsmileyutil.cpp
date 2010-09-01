/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Utility for selecting smile icon and converting
*                between smile id and smile string.
*
*/

// INCLUDE FILES
#include 	"cimcvappsmileyutil.h"
#include 	"cimcvapppicture.h"
#include    "cimcvappsmileyinformation.h"

#include	<PUAcodes.hrh>

#include	<gulicon.h>
#include 	<txtrich.h>
#include 	<gdi.h>
#include    <eikrted.h>
#include	<frmtlay.h>
#include 	"imcvlogger.h"
#include    "imcvuiliterals.h"
#include    "cimcvengine.h"
//CONSTANT
const TInt KChatSmileyLength( 3 );
_LIT(KSpace, " ");

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CIMCVAppSmileyUtil::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CIMCVAppSmileyUtil* CIMCVAppSmileyUtil::NewL(
							 MIMCVUiAppSkinVariant& aSkinVariant, 
                             MIMCVAppUi& aAppUi, TMessageExtensionTypes aType,
                             CIMCVEngine& aServiceEngine)
    {
    CIMCVAppSmileyUtil* self = new (ELeave) CIMCVAppSmileyUtil( aType );
    CleanupStack::PushL( self );
    self->ConstructL( aSkinVariant, aAppUi,aServiceEngine );
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CIMCVAppSmileyUtil::~CIMCVAppSmileyUtil
// Destructor
// -----------------------------------------------------------------------------
//
CIMCVAppSmileyUtil::~CIMCVAppSmileyUtil()
    {
    iConvertArray.ResetAndDestroy();
	delete iSmileUtil;
    }

// -----------------------------------------------------------------------------
// CIMCVAppSmileyUtil::CIMCVAppSmileyUtil
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CIMCVAppSmileyUtil::CIMCVAppSmileyUtil( TMessageExtensionTypes aType ) 
: MIMCVAppMessageExtension( aType)
    {
    }

// -----------------------------------------------------------------------------
// CIMCVAppSmileyUtil::ConstructL
// ConstructL
// -----------------------------------------------------------------------------
//
void CIMCVAppSmileyUtil::ConstructL(
							 MIMCVUiAppSkinVariant& aSkinVariant, 
                             MIMCVAppUi& aAppUi,
							 CIMCVEngine& aServiceEngine )
    {
    iServiceId = aServiceEngine.GetServiceId();
	iSmileUtil = CIMCVAppSmileIconUtility::NewL(aSkinVariant, aAppUi, aServiceEngine );
    }


// ---------------------------------------------------------
// CIMCVAppSmileyUtil::InsertExtensionL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppSmileyUtil::InsertExtensionL( CEikRichTextEditor& aEditor,
                                      TBool& aCancelled, TSize asize )
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileyUtil::InsertExtensionL() start") );
	TInt bitmapId( KErrNone );
	TInt retVal = iSmileUtil->LaunchSmileIconDialogL( bitmapId );	
	iSmileUtil->ResizeIcons(asize);		
	if( retVal )
		{
		// check if there is room for icon, if not return
		if( ( aEditor.MaxLength() - aEditor.TextLength() ) < KChatSmileyLength )
			{
			return;
			}		
        //we don't own this
		const TDesC& iconString = iSmileUtil->GetSmileString(bitmapId);		
        TInt cursorPos = aEditor.CursorPos();        
        HBufC* msgBuf = aEditor.GetTextInHBufL();
		CleanupStack::PushL( msgBuf );
		if(msgBuf)
		    {
		    TPtr msgPtr( msgBuf->Des() );		    
		    aEditor.RichText()->InsertL(cursorPos,iconString);
		    cursorPos = aEditor.TextLength();
		    aEditor.SetCursorPosL( cursorPos, EFalse);		    
		    }
		else
		    {
		    aEditor.RichText()->InsertL(cursorPos,iconString);
		    cursorPos = aEditor.TextLength(); 
		    aEditor.SetCursorPosL( cursorPos, EFalse);		  
		    }
		CleanupStack::PopAndDestroy( msgBuf ); // msgBuf		
		if( &aEditor != iEditor )
			{
			iEditor = &aEditor;
			iCurrentCount = 1;
			}
		else
			{
			++iCurrentCount;
			}
			
		aCancelled = EFalse;
		}
    else
        {
        // apply formatting even in case of cancel, as the sizes of icons would have changed and when
		// shifted from portrait to landscape and vice versa, alignment differs.
        TCharFormat charFormat;
        TCharFormatMask charFormatMask;
		
		IM_CV_LOGS(TXT("CIMCVAppSmileyUtil::InsertExtensionL() before applying format") );
        charFormat.iFontPresentation.iPictureAlignment = TFontPresentation::EAlignCentered;
        charFormatMask.SetAttrib( EAttFontPictureAlignment );
        aEditor.RichText()->ApplyCharFormatL( charFormat, charFormatMask, 0,
                                              aEditor.TextLength() );
		IM_CV_LOGS(TXT("CIMCVAppSmileyUtil::InsertExtensionL() after applying format") );
		aEditor.HandleTextChangedL ();
        aCancelled = ETrue;
        }
    IM_CV_LOGS(TXT("CIMCVAppSmileyUtil::InsertExtensionL() end") );
	}

// ---------------------------------------------------------
// CIMCVAppSmileyUtil::ExtensionToStringL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppSmileyUtil::ExtensionToStringL( CEikRichTextEditor& anEditor, TDes& aDes )
	{
	IM_CV_LOGS(TXT("CIMCVAppSmileyUtil::ExtensionToStringL() start") );
	RArray<TInt> iconIndArray;
	CleanupClosePushL( iconIndArray );
	
	CPicture* editorPic;
	TInt i;
	TInt limit( 0 );

	// for loop gets icon positions to array
    TInt length = aDes.Length();
	for( i = 0; i < length; ++i )
		{
		if( aDes[ i ] ==  CEditableText::EPictureCharacter )
			{
			User::LeaveIfError( iconIndArray.Append( i ) );
			}
		}

	// real count of pictures
	TInt count = anEditor.RichText()->PictureCount();
	TInt indCount = iconIndArray.Count();
	
	if( &anEditor != iEditor )
		{
        Reset();
		}

	// whisper message
	if( indCount > iCurrentCount )
		{
		limit = indCount - iCurrentCount;
		}

    if( indCount > 0 )
        {
        for( i = count; i > limit; --i )
    		{
    		editorPic = anEditor.RichText()->PictureHandleL(
    							iconIndArray[ i - 1 ], MLayDoc::EForceLoadTrue );
    		if ( !editorPic )
    			{
    			User::Leave( KErrBadHandle );
    			}

    		CIMCVAppPicture* icon = static_cast<CIMCVAppPicture*> ( editorPic );

            TInt oldLen = aDes.Length();
            //delete padding char
		    aDes.Delete( iconIndArray[ i - 1 ], 1 ); // 1 = one char
		    //insert string smiley		    
		    aDes.Insert( iconIndArray[ i - 1 ],
		                 iSmileUtil->GetSmileString( icon->Index() ) );
    		}
        }
    CleanupStack::PopAndDestroy(); // iconIndArray.Close()
    IM_CV_LOGS(TXT("CIMCVAppSmileyUtil::ExtensionToStringL() end") );
	}

// -----------------------------------------------------------------------------
// CIMCVAppSmileyUtil::ConvertSelectionToExtensionL
// Convert selection smileys to icons.
// -----------------------------------------------------------------------------
//
void CIMCVAppSmileyUtil::ConvertSelectionToExtensionL( CEikRichTextEditor& aEditor )
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileyUtil::ConvertSelectionToExtensionL() start") );
    TInt selectAdd( 0 );

    // for loop gets icon positions to array
    TInt start = aEditor.Selection().LowerPos();
    TInt end = aEditor.Selection().HigherPos();

    // Clear convert array, it has something, but its not correct
    iConvertArray.ResetAndDestroy();
	
	const TInt KMaxLength( KDefaultMsgLength );
	HBufC* des = HBufC::NewLC( KMaxLength );
	TPtr desText( des->Des() );

    aEditor.GetText( desText );
    TInt txtLen = desText.Length();
    if ( txtLen < end )
        {
        end = txtLen;
        }
    desText.Delete( end, txtLen - end );
    desText.Delete( 0, start );
   
    iSmileUtil->SearchSmilesL( desText, iConvertArray, NULL, start );

	#ifdef _DEBUG
		TPtrC myDebug( aEditor.Text()->Read( 0 ) ); 
	#endif

    for( TInt a( iConvertArray.Count() - 1 ); a >= 0; --a )
        {
        if( iConvertArray[ a ]->Position() >= start && iConvertArray[ a ]->Position() < end ) // do we convert or not
            {                
            TPictureHeader picHeader;
            // Insert icon in place

		    //we don't own this
		    CGulIcon* icon = const_cast<CGulIcon*>(
		        iSmileUtil->GetSmileIcon( iConvertArray[ a ]->Index() ) );

		    //create CIMCVAppPicture from selected icon
		    CIMCVAppPicture* smileIcon = new ( ELeave ) CIMCVAppPicture(
									    *CCoeEnv::Static()->SystemGc().Device(),
									    icon, 
									    iConvertArray[ a ]->Index() );

		    picHeader.iPicture = TSwizzle<CPicture>( smileIcon );
            aEditor.Text()->DeleteL( iConvertArray[ a ]->Position(), 
            							iConvertArray[ a ]->SmileyString().Length() ); 

	#ifdef _DEBUG
			myDebug.Set( aEditor.Text()->Read( 0 ) );
			
	#endif

			aEditor.RichText()->InsertL( iConvertArray[ a ]->Position(), picHeader );
			
	#ifdef _DEBUG
			myDebug.Set( aEditor.Text()->Read( 0 ) );
			
	#endif

            selectAdd -= iConvertArray[ a ]->SmileyString().Length() - 1;

		    // icon is inserted, and it should be converted to text
		    // before sending, so it needs extra chars according to smiley string length
		    TInt textLimit =
		        aEditor.MaxLength() - iConvertArray[ a ]->SmileyString().Length();
            TInt currentlength = aEditor.Text()->DocumentLength();            
		    aEditor.SetTextLimit( Max( textLimit, currentlength ) );
            
		    if( &aEditor != iEditor )
			    {
			    iEditor = &aEditor;
			    iCurrentCount = 1;
			    }
		    else
			    {
			    ++iCurrentCount;
			    }
            }
        }
    
    // Apply formatting
    TInt selectionLen = end + selectAdd - start;
    TCharFormat charFormat;
	TCharFormatMask charFormatMask;
	charFormat.iFontPresentation.iPictureAlignment = 
	    TFontPresentation::EAlignCentered;
	charFormatMask.SetAttrib( EAttFontPictureAlignment );
	aEditor.RichText()->ApplyCharFormatL( charFormat, charFormatMask,
	                                      start, selectionLen );
    
    // Handle changes in editor
    aEditor.TextView()->HandleGlobalChangeL();
    
    // Set selection to correct after adding images
    aEditor.SetSelectionL( start, end + selectAdd );

    // Clear convert array
    iConvertArray.ResetAndDestroy();
    
    CleanupStack::PopAndDestroy( des );
    IM_CV_LOGS(TXT("CIMCVAppSmileyUtil::ConvertSelectionToExtensionL() end") );
    }
    
// -----------------------------------------------------------------------------
// CIMCVAppSmileyUtil::ConvertSelectionToExtensionL
// Convert selection smileys to icons.
// -----------------------------------------------------------------------------
//
void CIMCVAppSmileyUtil::ConvertSelectionToExtensionL( CRichText& aRichText, 
														TCursorSelection& aSelection )
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileyUtil::ConvertSelectionToExtensionL() start") );
    TInt start( aSelection.LowerPos() );
    TInt length( aSelection.Length() );

    // Clear convert array and fetch the text
    iConvertArray.ResetAndDestroy();	
    HBufC* text = HBufC::NewMaxLC( aRichText.DocumentLength() );
    TPtr desText( text->Des() );	   
    aRichText.Extract( desText, start, length );
    
    iSmileUtil->SearchSmilesL( desText, iConvertArray );

    for( TInt a( iConvertArray.Count() - 1 ); a >= 0; --a )
        {
        TPictureHeader picHeader;
        // Insert icon in place
        //we don't own this
	    CGulIcon* icon = const_cast<CGulIcon*>( 
	        iSmileUtil->GetSmileIcon( iConvertArray[ a ]->Index()) );			

	    //create CIMCVAppPicture from selected icon
	    CIMCVAppPicture* smileIcon = new ( ELeave ) CIMCVAppPicture(
								    *CCoeEnv::Static()->SystemGc().Device(),
								    icon, 
								    iConvertArray[ a ]->Index());

	    picHeader.iPicture = TSwizzle<CPicture>( smileIcon );
        aRichText.DeleteL( iConvertArray[ a ]->Position() + start,
                           iConvertArray[ a ]->SmileyString().Length() );

		aRichText.InsertL( iConvertArray[ a ]->Position() + start, picHeader );
        }

    // clean up
    iConvertArray.ResetAndDestroy();
    CleanupStack::PopAndDestroy( text );    
    IM_CV_LOGS(TXT("CIMCVAppSmileyUtil::ConvertSelectionToExtensionL() end") );
    }

// -----------------------------------------------------------------------------
// CIMCVAppSmileyUtil::ConvertSelectionToStringL
// Convert selection smileys to strings
// -----------------------------------------------------------------------------
//
void CIMCVAppSmileyUtil::ConvertSelectionToStringL( CEikRichTextEditor& aEditor, TBool aPreserve )
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileyUtil::ConvertSelectionToStringL() start") );
    // Clear convert array
    iConvertArray.ResetAndDestroy();

    HBufC* text = HBufC::NewMaxLC( KDefaultMsgLength );
    TPtr textCopy( text->Des() );
    aEditor.GetText( textCopy );

	// for loop gets icon positions to array
    TInt start = 0;
    TInt end = aEditor.Text()->DocumentLength();
    //aEditor.Selection().HigherPos();
	
	TInt i;
    for( i = start; i < end; ++i )
		{
		if( textCopy[ i ] ==  CEditableText::EPictureCharacter )
			{
            CIMCVAppSmileyInformation* smileInfo = CIMCVAppSmileyInformation::NewL(); 
            CleanupStack::PushL( smileInfo );
            smileInfo->SetPosition( i );
            smileInfo->SetIcon( ETrue );
			User::LeaveIfError( iConvertArray.Append( smileInfo ) );
			CleanupStack::Pop( smileInfo );
			}
        }

    TInt count = iConvertArray.Count();
	for( i = count - 1; i >= 0; --i )
		{
		CPicture* editorPic = aEditor.RichText()->PictureHandleL( 
            iConvertArray[ i ]->Position(), MLayDoc::EForceLoadTrue );
        
		if ( !editorPic )
			{
			User::Leave( KErrBadHandle );
			}

		CIMCVAppPicture* icon = static_cast<CIMCVAppPicture*> ( editorPic );

        const TDesC& iconString = iSmileUtil->GetSmileString( icon->Index() );
        iConvertArray[ i ]->SetSmileyStringL( iconString );
        iConvertArray[ i ]->SetIndex( icon->Index() );
		}

    textCopy.Delete( end, textCopy.Length() - end );
    textCopy.Delete( 0, start );
    iSmileUtil->SearchSmilesL( textCopy, iConvertArray, NULL, start );

    // Change icons to strings
	TPtrC myDebug( aEditor.Text()->Read( 0 ) ); 

    TInt selectAdd( 0 );
    count = iConvertArray.Count();
    for( i = count - 1; i >= 0; --i )
        {
        if( iConvertArray[ i ]->IsIcon() ) // Icon
            {
            TCursorSelection deleteSel( iConvertArray[ i ]->Position(),
                                        iConvertArray[ i ]->Position() + 1 );
                                        
            aEditor.InsertDeleteCharsL( iConvertArray[ i ]->Position(),
                                        iConvertArray[ i ]->SmileyString(),
                                        deleteSel );
            selectAdd += iConvertArray[ i ]->SmileyString().Length() - 1;
            //aEditor.SetMaxLength( aEditor.MaxLength() + selectAdd );
            }
        }
	myDebug.Set( aEditor.Text()->Read( 0 ) );
	

    aEditor.SetSelectionL( start, end + selectAdd );
    aEditor.TextView()->HandleGlobalChangeL();

    if( !aPreserve )
        {
	    iConvertArray.ResetAndDestroy();
        }
        
    CleanupStack::PopAndDestroy( text );
    IM_CV_LOGS(TXT("CIMCVAppSmileyUtil::ConvertSelectionToStringL() end") );
    }

// ---------------------------------------------------------
// CIMCVAppSmileyUtil::DeleteExtensionL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppSmileyUtil::DeleteExtensionL( CEikRichTextEditor& aEditor, TInt aPos )
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileyUtil::DeleteExtensionL() start") );
    // Set cursor position before deletion.
    // Deletion may cause editor size to change and if
    // cursor pos is not updated before that ETEXT 12 panic
    // is raised.
    aEditor.SetCursorPosL( aPos, EFalse );
    aEditor.RichText()->DeleteL( aPos, 1 );
	aEditor.HandleTextChangedL();
	// Ugly hack. Needed to keep alignment of
	// cursor correct.
	aEditor.SetCursorPosL( aPos, EFalse );
	
	// update editors length, it grows by 2 when icon is deleted
	TInt textLimit = aEditor.MaxLength() + 2;		
	aEditor.SetTextLimit( textLimit );
	
	--iCurrentCount;
	IM_CV_LOGS(TXT("CIMCVAppSmileyUtil::DeleteExtensionL() end") );
	}

// -----------------------------------------------------------------------------
// CIMCVAppSmileyUtil::Reset
// Reset extension
// -----------------------------------------------------------------------------
//
void CIMCVAppSmileyUtil::Reset()
    {
    iCurrentCount = 0;
    }

// -----------------------------------------------------------------------------
// CIMCVAppSmileyUtil::SizeChanged
// Reset extension
// -----------------------------------------------------------------------------
//
void CIMCVAppSmileyUtil::SizeChanged( TSize& aSize )
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileyUtil::SizeChanged() start") );
    iSmileUtil->ResizeIcons( aSize );
    IM_CV_LOGS(TXT("CIMCVAppSmileyUtil::SizeChanged() end") );
    }

// -----------------------------------------------------------------------------
// CIMCVAppSmileyUtil::ConvertSelectionToStringL
// Convert selection smileys to strings
// -----------------------------------------------------------------------------
//
void CIMCVAppSmileyUtil::ConvertSelectionToStringL(
    CEikRichTextEditor& aEditor,
    TDes& aResultString,
    TCursorSelection& aSelectionAfterConversion,
    TBool aPreserve )
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileyUtil::ConvertSelectionToStringL() start") );
    // Clear convert array
    iConvertArray.ResetAndDestroy();
	
    // Get text from editor
    HBufC* text = HBufC::NewMaxLC( KDefaultMsgLength );
    TPtr textCopy( text->Des() );
    aEditor.GetText( textCopy );

	// For loop gets icon positions to array
    TInt start = aEditor.Selection().LowerPos();
    TInt end = aEditor.Selection().HigherPos();
	
	// Copy text to result string
	aResultString.Append( textCopy );
	
	TInt i = 0;
    for( i = start; i < end; ++i )
        {
		if ( textCopy[i] ==  CEditableText::EPictureCharacter )
			{
            CIMCVAppSmileyInformation* smileInfo = CIMCVAppSmileyInformation::NewL(); 
            CleanupStack::PushL( smileInfo );
            smileInfo->SetPosition( i );
            smileInfo->SetIcon( ETrue );
			User::LeaveIfError( iConvertArray.Append( smileInfo ) );
			CleanupStack::Pop( smileInfo );
			}	
        }

    TInt count = iConvertArray.Count();
	for( i = count - 1; i >= 0; --i )
		{
		CPicture* editorPic = NULL;
		
		editorPic = aEditor.RichText()->PictureHandleL( 
                iConvertArray[i]->Position(), MLayDoc::EForceLoadTrue );
        
		if ( !editorPic )
			{
			User::Leave( KErrBadHandle );
			}

		CIMCVAppPicture* icon = static_cast<CIMCVAppPicture*> ( editorPic );

        const TDesC& iconString = iSmileUtil->GetSmileString( icon->Index() );
        iConvertArray[i]->SetSmileyStringL( iconString );
        iConvertArray[i]->SetIndex( icon->Index() );
		}

    textCopy.Delete( end, textCopy.Length() - end );
    textCopy.Delete( 0, start );
    iSmileUtil->SearchSmilesL( textCopy, iConvertArray, NULL, start );

    // Change icons to strings
    TInt selectAdd = 0;
    count = iConvertArray.Count();
    for( i = count - 1; i >= 0; --i )
        {
        if( iConvertArray[ i ]->IsIcon() ) // Icon
            {
            // Append smiley string to result
            aResultString.Delete( iConvertArray[i]->Position(), 1 );
            aResultString.Insert( iConvertArray[i]->Position(), iConvertArray[i]->SmileyString() );
            selectAdd += iConvertArray[ i ]->SmileyString().Length() - 1;
            }
        }

    if( !aPreserve )
        {
	    iConvertArray.ResetAndDestroy();
        }
    
    // Set correct selection
    aSelectionAfterConversion.SetSelection( end + selectAdd, start );
        
    CleanupStack::PopAndDestroy( text );
    IM_CV_LOGS(TXT("CIMCVAppSmileyUtil::ConvertSelectionToStringL() end") );
    }
// -----------------------------------------------------------------------------
// CIMCVAppSmileyUtil::ServiceId
// Convert selection smileys to strings
// -----------------------------------------------------------------------------
//
TInt CIMCVAppSmileyUtil::ServiceId() const
    {
    return iServiceId;
    }
        
// End of File
