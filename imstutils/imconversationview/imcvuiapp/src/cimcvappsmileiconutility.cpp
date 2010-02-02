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
#include "cimcvappsmileiconutility.h"
#include "cimcvappsmileicondialog.h"
#include "cimcvappsmilestring.h"
#include "cimcvappsmileyinformation.h"
#include "mimcvuiappskinvariant.h"
#include "cimcvengine.h"
#include    "cimcvappbitmapmapping.h"     // defines the skinned bitmap ids

#include 	"mimcvappui.h"
#include 	<UniUtils.rsg>
#include 	<MsgEditorAppUi.rsg>
#include 	<imcvuiapp.rsg>
#include 	<bautils.h>
#include 	<PUAcodes.hrh>
#include 	<barsread.h> // RResourceReader
#include 	<coemain.h>
#include 	<eikenv.h>
#include 	<gulicon.h>

#include	<aknappui.h>
#include	<AknsSkinInstance.h>	// skins 
#include	<AknsUtils.h>
#include	<AknsDrawUtils.h>
#include	<AknsBasicBackgroundControlContext.h>
#include    <AknIconUtils.h>
#include 	"imcvlogger.h"
#include    <mbsaccess.h>
#include    "cimcvuiappiconfileprovider.h"
#include    "conversations.h"
#include    "imcvuiapp.h"
#include 	<utf.h>


// ================= MEMBER FUNCTIONS =======================

// Two-phased constructor.
CIMCVAppSmileIconUtility* CIMCVAppSmileIconUtility::NewL(MIMCVUiAppSkinVariant& aSkinVariant,
                             MIMCVAppUi& aAppUi,
                             CIMCVEngine& aServiceEngine  )
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileIconUtility::NewL() start") );
    CIMCVAppSmileIconUtility* self = new (ELeave) CIMCVAppSmileIconUtility(aSkinVariant, aAppUi);
    CleanupStack::PushL( self );
    self->ConstructL(aServiceEngine);
    CleanupStack::Pop( self );
    IM_CV_LOGS(TXT("CIMCVAppSmileIconUtility::NewL() end") );
    return self;
    }

// Destructor
CIMCVAppSmileIconUtility::~CIMCVAppSmileIconUtility()
    {
    iAppUi.RemoveResourceChangeObserver( this );  
	iStringArray.ResetAndDestroy();
	iSmileArray.ResetAndDestroy();
    iIconToSmileyPointers.Close();   
    iSmileDlgIconArray.ResetAndDestroy();
    iFile.Close();
    }

// C++ constructor can NOT contain any code, that
// might leave.
//
CIMCVAppSmileIconUtility::CIMCVAppSmileIconUtility(MIMCVUiAppSkinVariant& aSkinVariant, 
                             MIMCVAppUi& aAppUi )
    :iSkinVariant( aSkinVariant),                       
    iAppUi( aAppUi )
    {
    }

// ConstructL
//
void CIMCVAppSmileIconUtility::ConstructL(CIMCVEngine& aServiceEngine )
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileIconUtility::ConstructL() start") );
	// we need to be resource aware
	iAppUi.AddResourceChangeObserver( this );
	ConstructSmileDialogIconArrayL( aServiceEngine );
    IM_CV_LOGS(TXT("CIMCVAppSmileIconUtility::ConstructL() end") );
    }

// ---------------------------------------------------------
// CIMCVAppSmileIconUtility::ParseAllSmileyStringL
// (other items were commented in a header).
// ---------------------------------------------------------
// 
void CIMCVAppSmileIconUtility::ParseAllSmileyStringL(RPointerArray<HBufC> aAllSmileyString)
    {       
    TInt iconCount  = aAllSmileyString.Count();
    //make sure everything is fresh
    iStringArray.ResetAndDestroy();
    for(TInt i=0; i<iconCount; i++)
        {
        CIMCVAppSmileString* smileArray = CIMCVAppSmileString::NewL();
        CleanupStack::PushL( smileArray );
     
        HBufC* smileString = aAllSmileyString[i];
        //set longest smiley lenth
        if( smileString->Length() > iLongestSmileyLength )
            {
            iLongestSmileyLength = smileString->Length();
            }
        // ownership is transferred
        smileArray->Set( smileString, i );
        User::LeaveIfError( iStringArray.Append( smileArray ) );            
        CleanupStack::Pop( smileArray );
        }    
    }
// ---------------------------------------------------------
// CIMCVAppSmileIconUtility::ConstructSmileDialogIconArrayL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppSmileIconUtility::ConstructSmileDialogIconArrayL(CIMCVEngine& aServiceEngine )
    {   
    RPointerArray<HBufC> smileyString = aServiceEngine.GetSmileStringArray(); 
    CleanupClosePushL( smileyString );
    ParseAllSmileyStringL(smileyString); // all object ownership transfered
    smileyString.Reset();
    CleanupStack::Pop( &smileyString ); // no objects alive
    
    iSmileDlgIconArray = aServiceEngine.GetSmileyIconArray();
 
    // Sort smileys according to their string length, so that searching
    // for smileys is done from the longest smiley to the shortest. This
    // prevents the chat from detecting smaller smileys inside the big ones.
    TLinearOrder< CIMCVAppSmileString > 
    order( CIMCVAppSmileIconUtility::CompareSmileyLengthReversed );
    iStringArray.Sort( order );
    // Precalculate an ref table from icon to smiley.        
    iIconToSmileyPointers.Reset();   
    TInt count = iSmileDlgIconArray.Count();
    for( TInt i = 0; i < count; ++i )
        {
        User::LeaveIfError( 
                iIconToSmileyPointers.Append( GetFirstSmileyIndex( i ) ) );        
        }  
    }    

// ---------------------------------------------------------
// CIMCVAppSmileIconUtility::LaunchSmileIconDialogL
// (other items were commented in a header).
// ---------------------------------------------------------
//code scaner warning can be ignored since ExecuteLD is leaving and we need to return dialogVal.
TInt CIMCVAppSmileIconUtility::LaunchSmileIconDialogL( TInt& aSelectedIconId )
    {
    if( iSmileDlgIconArray.Count() <= 0 )
        {
        User::Leave(KErrArgument);
        }
	CIMCVAppSmileIconDialog* dialog = CIMCVAppSmileIconDialog::NewL( iSmileDlgIconArray,
														   aSelectedIconId );
	TInt dialogVal = dialog->ExecuteLD( R_SELECT_SMILE_DIALOG );
	IM_CV_LOGS(TXT("CIMCVAppSmileIconUtility::LaunchSmileIconDialogL() end") );
	return dialogVal;
    }

// ---------------------------------------------------------
// CIMCVAppSmileIconUtility::SmileIconCount
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CIMCVAppSmileIconUtility::SmileIconCount() const
	{
	return iSmileDlgIconArray.Count();
	}

// ---------------------------------------------------------
// CIMCVAppSmileIconUtility::GetSmileIcon
// (other items were commented in a header).
// ---------------------------------------------------------
//
const CGulIcon* CIMCVAppSmileIconUtility::GetSmileIcon( TInt aIconId ) const
	{
	return iSmileDlgIconArray[ aIconId ];
	}

// ---------------------------------------------------------
// CIMCVAppSmileIconUtility::GetSmileString
// (other items were commented in a header).
// ---------------------------------------------------------
//
const TDesC& CIMCVAppSmileIconUtility::GetSmileString( TInt aIconId ) const
	{
	return iStringArray[ iIconToSmileyPointers[ aIconId ] ]->SmileString();
	}

// ---------------------------------------------------------
// CIMCVAppSmileIconUtility::SearchSmilesL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppSmileIconUtility::SearchSmilesL( const TDesC& aStr, TDes& aFixedStr,
										 RArray<TInt>* aArray )
	{
	IM_CV_LOGS(TXT("CIMCVAppSmileIconUtility::SearchSmilesL() start") );
	TInt i;
    iSmileArray.ResetAndDestroy();
    SearchSmilesL( aStr, iSmileArray, &aFixedStr );

	aArray->Reset();
    TInt arrayCount( iSmileArray.Count() );

    for ( i = 0; i < arrayCount; ++i )
		{
		User::LeaveIfError( aArray->Append( iSmileArray[ i ]->Index() ) );
		}
    
	iSmileArray.ResetAndDestroy();
	IM_CV_LOGS(TXT("CIMCVAppSmileIconUtility::SearchSmilesL() end") );
	}

// -----------------------------------------------------------------------------
// CIMCVAppSmileIconUtility::SearchSmilesL
// Search smileys from string and sort them in linear order.
// -----------------------------------------------------------------------------
//
void CIMCVAppSmileIconUtility::SearchSmilesL( const TDesC& aStr, 
                         RPointerArray< CIMCVAppSmileyInformation >& aSmileyInfoArray, 
                         TDes* aFixedStr /*= NULL */,
                         TInt aStartPos /* = 0 */) const
    {
	IM_CV_LOGS(TXT("CIMCVAppSmileIconUtility::SearchSmilesL() start") );
	HBufC* text = HBufC::NewMaxLC( aStr.Length() );
	TPtr fixedText( text->Des() );
	fixedText.Copy( aStr.Left(fixedText.MaxLength()) );
    
    HBufC* replaceString = HBufC::NewMaxLC( iLongestSmileyLength );
	TPtr replace( replaceString->Des() );
	if ( aFixedStr )
		{
		replace.Fill( KPuaCodeSmileyIconPadding, 1 );
		}
	else
		{
		//we dont need fixed, but we do need correct positions
		replace.Fill( KPuaCodeSmileyIconPadding, iLongestSmileyLength );
		}

    // order to sort array by the position
	TLinearOrder< CIMCVAppSmileyInformation > order( CIMCVAppSmileIconUtility::Compare );

	// for i-loop goes throug every smile string
	// while-loop searches smiles from given string
    TInt arrayCount = iStringArray.Count(); 
	TInt index = 0;
	for ( TInt i = 0; i < arrayCount; ++i )
		{
		const TDesC& smileString( iStringArray[ i ]->SmileString() );
		index = fixedText.Find( smileString );
		while ( index != KErrNotFound )
			{
            CIMCVAppSmileyInformation* smileInfo = CIMCVAppSmileyInformation::NewL();
            CleanupStack::PushL( smileInfo );
            
			smileInfo->SetPosition( index + aStartPos );
			smileInfo->SetIndex( iStringArray[ i ]->SmileIconIndex() );
            smileInfo->SetSmileyStringL( smileString );
            smileInfo->SetIcon( EFalse );
            
			User::LeaveIfError( aSmileyInfoArray.Append( smileInfo ) );
            CleanupStack::Pop( smileInfo );
            
			TInt smilyLength = smileString.Length();
            // Only replace the same amount of chars than in smiley, so we 
            // don't mess up the length or index table.
            fixedText.Replace( index, 
                               smilyLength, 
                               replace.Left( smilyLength ) );
			index = fixedText.Find( smileString );
			} // while
		} // for i
    
	aSmileyInfoArray.Sort( order );
    if( aFixedStr )
        {
        *aFixedStr = fixedText;
        }
        
    CleanupStack::PopAndDestroy( 2, text ); // replaceString, text
	IM_CV_LOGS(TXT("CIMCVAppSmileIconUtility::SearchSmilesL() end") );
    }

// ---------------------------------------------------------
// CIMCVAppSmileIconUtility::Compare
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CIMCVAppSmileIconUtility::Compare( const CIMCVAppSmileyInformation& aFirst,
                                   const CIMCVAppSmileyInformation& aSecond )
    {
    return ( aFirst.Position() - aSecond.Position() );
    }

// -----------------------------------------------------------------------------
// CIMCVAppSmileIconUtility::CompareSmileyLengthReversed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CIMCVAppSmileIconUtility::CompareSmileyLengthReversed( 
                                            const CIMCVAppSmileString& aFirst,
                                            const CIMCVAppSmileString& aSecond )
    {
    return ( aSecond.SmileString().Length() - aFirst.SmileString().Length() );
    }

// ---------------------------------------------------------
// CIMCVAppSmileIconUtility::ResizeIcons
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMCVAppSmileIconUtility::ResizeIcons( const TSize& aSize )
	{
	IM_CV_LOGS(TXT("CIMCVAppSmileIconUtility::ResizeIcons() start") );
	TInt count( SmileIconCount() );
	for( TInt i(0); i < count; i++) 
		{
		AknIconUtils::SetSize( iSmileDlgIconArray[ i ]->Bitmap(), aSize );
		}
	IM_CV_LOGS(TXT("CIMCVAppSmileIconUtility::ResizeIcons() end") );
	}

// -----------------------------------------------------------------------------
// CIMCVAppSmileIconUtility::GetFirstSmileyIndex
// -----------------------------------------------------------------------------
//
TInt CIMCVAppSmileIconUtility::GetFirstSmileyIndex( TInt aIconIndex ) const
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileIconUtility::GetFirstSmileyIndex() start") );
	TInt count = iStringArray.Count();
	
	for( TInt i = 0; i < count; ++i )
	    {
	    if( iStringArray[ i ]->SmileIconIndex() == aIconIndex )
	        {
	        return i;
	        }
	    }
	IM_CV_LOGS(TXT("CIMCVAppSmileIconUtility::GetFirstSmileyIndex() end") );
	// Icon was not found.	
	return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// CIMCVAppSmileIconUtility::ResourceChangedL
// -----------------------------------------------------------------------------
//
void CIMCVAppSmileIconUtility::ResourceChangedL()
    {
    IM_CV_LOGS(TXT("CIMCVAppSmileIconUtility::ResourceChangedL() start") );		
	IM_CV_LOGS(TXT("CIMCVAppSmileIconUtility::ResourceChangedL() end") );
    }


// End of File

