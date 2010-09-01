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
* Description:  message extention class
*
*/


#include "cimcvappmessageextensionshandler.h"
#include "cimcvenginefactory.h"
#include <eikrted.h>
#include <txtrich.h>
#include "imcvlogger.h"
#include "cimcvappsmileyutil.h"
#include "imcvuiliterals.h"
#include "cimcvengine.h"


// -----------------------------------------------------------------------------
// CIMCVAppMessageExtensionsHandler::CIMCVAppMessageExtensionsHandler
// Constructor
// -----------------------------------------------------------------------------
//
CIMCVAppMessageExtensionsHandler::CIMCVAppMessageExtensionsHandler( MIMCVUiAppSkinVariant& aSkinVariant, 
                                                                    MIMCVAppUi& aAppUi,
                                                                    MIMCVEngineFactory& aEngineFactory )
    :iSkinVariant( aSkinVariant ),
    iAppUi( aAppUi ),
    iEngineFactory( aEngineFactory )
    {
    }

// -----------------------------------------------------------------------------
// CIMCVAppMessageExtensionsHandler::~CIMCVAppMessageExtensionsHandler
// Destructor
// -----------------------------------------------------------------------------
//
CIMCVAppMessageExtensionsHandler::~CIMCVAppMessageExtensionsHandler()
    {
    iExtensionArray.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
// CIMCVAppMessageExtensionsHandler::NewL
// Symbian two phased constructor
// -----------------------------------------------------------------------------
//
CIMCVAppMessageExtensionsHandler* CIMCVAppMessageExtensionsHandler::NewL(
                                MIMCVUiAppSkinVariant& aSkinVariant,
                                MIMCVAppUi& aAppUi,
                                MIMCVEngineFactory& aEngineFactory,
                                TBool /*aAddSmileyHandler = ETrue */)
    {
    IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::CIMCVAppMessageExtensionsHandler()    start" ));
    CIMCVAppMessageExtensionsHandler* self = 
                new ( ELeave ) CIMCVAppMessageExtensionsHandler(aSkinVariant,aAppUi,aEngineFactory);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::CIMCVAppMessageExtensionsHandler()    end" ));
    return self;
    }

// -----------------------------------------------------------------------------
// CIMCVAppMessageExtensionsHandler::ConstructL
// Symbian OS Constructor
// -----------------------------------------------------------------------------
//
void CIMCVAppMessageExtensionsHandler::ConstructL()
    {
    IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::ConstructL()	start" ));
    //get the count of number of services
    RPointerArray<CIMCVEngine>& serviceEngines = iEngineFactory.GetServiceEngines();
    for(TInt i=0; i<serviceEngines.Count() ; i++)
        {
        CIMCVAppSmileyUtil* smiley = CIMCVAppSmileyUtil::NewL(iSkinVariant, iAppUi, EMessageExtensionSmiley, *serviceEngines[i] );
        CleanupStack::PushL( smiley );
        User::LeaveIfError( iExtensionArray.Append( smiley ));
        CleanupStack::Pop( smiley ); 
        } 
   IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::ConstructL()	end" ));
    }

// -----------------------------------------------------------------------------
// CIMCVAppMessageExtensionsHandler::CreateServiceSmileyL
// Symbian OS Constructor
// -----------------------------------------------------------------------------
//
void CIMCVAppMessageExtensionsHandler::CreateServiceSmileyL( TInt aServiceId )
    {
    IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::HandleNewServiceL()  start" ));
    TInt error = KErrNone;
    CIMCVEngine& engine = iEngineFactory.GetServiceEngineL( aServiceId );
    //get the count of number of services
    TInt count = iExtensionArray.Count();
    for(TInt i=0; i<count ; i++)
        {
        if( iExtensionArray[i]->ServiceId() == aServiceId )
            {
			IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::CREATE SERVICE SMILEY ALREADY EXISTS" ));
			error = KErrAlreadyExists;
            break;
            }
        }
    if( error == KErrNone )
        {
        CIMCVAppSmileyUtil* smiley = CIMCVAppSmileyUtil::NewL(iSkinVariant, iAppUi, EMessageExtensionSmiley, engine );
        CleanupStack::PushL( smiley );
        User::LeaveIfError( iExtensionArray.Append( smiley ));
        CleanupStack::Pop( smiley ); 
        }
    IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::HandleNewServiceL()  end" ));
    }
// -----------------------------------------------------------------------------
// CIMCVAppMessageExtensionsHandler::DeleteServiceSmileyL
// Symbian OS Constructor
// -----------------------------------------------------------------------------
//
void CIMCVAppMessageExtensionsHandler::DeleteServiceSmileyL( TInt aServiceId )
    {
    IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::HandleServiceDeletedL()  start" ));
    TInt count = iExtensionArray.Count();
    for(TInt i=0; i<count ; i++)
        {
        if( iExtensionArray[i]->ServiceId() == aServiceId )
            {
			IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::DELET SMILEY SERVICES" ));
            MIMCVAppMessageExtension* smiley = iExtensionArray[ i ];
            iExtensionArray.Remove( i );
            delete smiley;
            smiley = NULL;
            iExtensionArray.Compress();
            break;
            }
        }
    IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::HandleServiceDeletedL()  end" ));
    }

// -----------------------------------------------------------------------------
// CIMCVAppMessageExtensionsHandler::ProcessMessageLC
// Process messages trough extensions.
// -----------------------------------------------------------------------------
//
HBufC* CIMCVAppMessageExtensionsHandler::ProcessMessageLC( 
                                            CEikRichTextEditor& aEditor,
                                            TInt aServiceId )
    {
    IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::ProcessMessageLC()	start" ));
    HBufC* msg = ExtensionToStringLC( aEditor, aServiceId );
    for( TInt a( 0 ); a < iExtensionArray.Count(); ++a )
        {
        if( aServiceId == iExtensionArray[a]->ServiceId() )
            {
            iExtensionArray[ a ]->Reset( );
            break;
            }
        }
    IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::ProcessMessageLC()	end" ));
    return msg;
    }

// -----------------------------------------------------------------------------
// CIMCVAppMessageExtensionsHandler::ExtensionToStringL
// Convert extensions to strings.
// -----------------------------------------------------------------------------
//
HBufC* CIMCVAppMessageExtensionsHandler::ExtensionToStringLC( 
                                            CEikRichTextEditor& aEditor,
                                            TInt aServiceId )
    {
    IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::ExtensionToStringLC()	start" ));
    HBufC* text = HBufC::NewMaxLC(KDefaultMsgLength );
    TPtr tBuf( text->Des() );

    HBufC* msgBuf = aEditor.GetTextInHBufL();
    if(!msgBuf)
        {
        tBuf.Zero();
        return text;	
        }
    CleanupStack::PushL( msgBuf );
    TPtr msgPtr( msgBuf->Des() );
   
    for( TInt a( 0 ); a < iExtensionArray.Count(); ++a )
        {
        if( aServiceId == iExtensionArray[a]->ServiceId() )
            {
            tBuf = msgPtr;
            iExtensionArray[ a ]->ExtensionToStringL( aEditor, tBuf );
            CleanupStack::PopAndDestroy( msgBuf );
            msgBuf = NULL;
            msgBuf = tBuf.AllocLC();
            TPtr newPtr( msgBuf->Des() );
            msgPtr.Set( newPtr );
            break;
            }       
        }
    
    // Remove all CEditableText::EZeroWidthNoBreakSpace characters
    // which are used to go around CRichText editor bug
    // when msg contains only smiley pictures
    // These rows below can be removed when CRichText editor
    // don't need anyrmore this "hack"
    tBuf = msgPtr;

    TInt length = tBuf.Length();
	for( TInt i = length-1; i >=0 ; --i )
		{
		if( tBuf[ i ] ==  CEditableText::EZeroWidthNoBreakSpace )
			{
			tBuf.Delete( i, 1 ); // removing 1 character
			}
		}
    CleanupStack::PopAndDestroy( msgBuf );
    IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::ExtensionToStringLC()	end" ));
    return text;
    }


// -----------------------------------------------------------------------------
// CIMCVAppMessageExtensionsHandler::DeleteExtensionIcon
// Find right extension handler and delete extension
// -----------------------------------------------------------------------------
//
void CIMCVAppMessageExtensionsHandler::DeleteExtensionL( CEikRichTextEditor& aEditor,
        TInt aServiceId ,
        TMessageExtensionTypes /*aType*/,
        TInt aPos )
    {
    IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::DeleteExtensionL()	start" ));
    for( TInt a( 0 ); a < iExtensionArray.Count(); ++a )
        {
        if( aServiceId == iExtensionArray[a]->ServiceId() )
            {
            iExtensionArray[ a ]->DeleteExtensionL( aEditor, aPos );
            break;
            }
        }
    IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::DeleteExtensionL()	end" ));
	}

// -----------------------------------------------------------------------------
// CIMCVAppMessageExtensionsHandler::ConvertSelectionToExtensionL
// Convert all extensions from strings to extensions in area.
// -----------------------------------------------------------------------------
//
void CIMCVAppMessageExtensionsHandler::ConvertSelectionToExtensionL( 
                                                CEikRichTextEditor& aEditor,
                                                TInt aServiceId )
    {
    IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::ConvertSelectionToExtensionL()	start" ));
    for( TInt a( 0 ); a < iExtensionArray.Count(); ++a )
        {
        if( aServiceId == iExtensionArray[a]->ServiceId() )
            {
            iExtensionArray[ a ]->ConvertSelectionToExtensionL( aEditor );
            break;
            }
        }
    IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::ConvertSelectionToExtensionL()	end" ));
    }
    
// -----------------------------------------------------------------------------
// CIMCVAppMessageExtensionsHandler::ConvertSelectionToExtensionL
// Convert all extensions from strings to extensions in area.
// -----------------------------------------------------------------------------
//
void CIMCVAppMessageExtensionsHandler::ConvertSelectionToExtensionL( 
                       CRichText& aRichText, 
                       TCursorSelection& aSelection,
                       TInt aServiceId )
    {
    IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::ConvertSelectionToExtensionL()	start" ));
    for( TInt a( 0 ); a < iExtensionArray.Count(); ++a )
        {
        if( aServiceId == iExtensionArray[a]->ServiceId() )
            {
            iExtensionArray[ a ]->ConvertSelectionToExtensionL( aRichText, aSelection );
            break;
            }
        }
    IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::ConvertSelectionToExtensionL()	end" ));
    }

// -----------------------------------------------------------------------------
// CIMCVAppMessageExtensionsHandler::ConvertSelectionToStringL
// Convert all extensions from extensions to strings in area.
// -----------------------------------------------------------------------------
//
void CIMCVAppMessageExtensionsHandler::ConvertSelectionToStringL( 
                    CEikRichTextEditor& aEditor, 
                    TInt aServiceId,
                    TBool aPreserve /*= EFalse*/ )
    {
    IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::ConvertSelectionToStringL()	start" ));
    
    for( TInt a( 0 ); a < iExtensionArray.Count(); ++a )
        {
        if( aServiceId == iExtensionArray[a]->ServiceId() )
            {
            iExtensionArray[ a ]->ConvertSelectionToStringL( aEditor, aPreserve );
            break;
            }
        }
    IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::ConvertSelectionToStringL()	end" ));
    }


// -----------------------------------------------------------------------------
// CIMCVAppMessageExtensionsHandler::SizeChanged
// Convert all extensions from extensions to strings in area.
// -----------------------------------------------------------------------------
//
void CIMCVAppMessageExtensionsHandler::SizeChanged( TSize& aSize,
        TInt aServiceId )
    {
    IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::SizeChanged()	start" ));
   
    for( TInt a( 0 ); a < iExtensionArray.Count(); ++a )
        {
        if( aServiceId == iExtensionArray[a]->ServiceId() )
            {
            iExtensionArray[ a ]->SizeChanged( aSize );
            break;
            }
        }
    IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::SizeChanged()	end" ));
    }

// -----------------------------------------------------------------------------
// CIMCVAppMessageExtensionsHandler::ConvertSelectionToStringL
// Convert all extensions from extensions to strings in area.
// -----------------------------------------------------------------------------
//
void CIMCVAppMessageExtensionsHandler::ConvertSelectionToStringL(
    CEikRichTextEditor& aEditor,
    TInt aServiceId ,
    TDes& aResultString,
    TCursorSelection& aSelectionAfterConversion,
    TBool aPreserve )
    {
    IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::ConvertSelectionToStringL()	start" ));
    for( TInt a( 0 ); a < iExtensionArray.Count(); ++a )
        {
        if( aServiceId == iExtensionArray[a]->ServiceId() )
            {
            iExtensionArray[ a ]->ConvertSelectionToStringL(
                    aEditor, aResultString, aSelectionAfterConversion, aPreserve );
            break;
            }
        }
    IM_CV_LOGS(TXT("CIMCVAppMessageExtensionsHandler::ConvertSelectionToStringL()	end" ));
    }
    
// -----------------------------------------------------------------------------
// CIMCVAppMessageExtensionsHandler::InsertExtensionIcon
// Find right extension handler and insert extension
// -----------------------------------------------------------------------------
//
void CIMCVAppMessageExtensionsHandler::InsertExtensionL( CEikRichTextEditor& aEditor,
                                                    TInt aServiceId ,
                                                    TMessageExtensionTypes aType,                                                    
                                                    TBool& aCancelled,
                                                    TSize size)
    {
    for( TInt a( 0 ); a < iExtensionArray.Count(); ++a )
        {
        if( aType == iExtensionArray[ a ]->Type() && aServiceId == iExtensionArray[a]->ServiceId() )
            {
            iExtensionArray[ a ]->InsertExtensionL( aEditor, aCancelled, size );
            break;
            }
        }
    }
    
// end of file
