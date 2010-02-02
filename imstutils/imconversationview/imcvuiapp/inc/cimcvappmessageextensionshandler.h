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



#ifndef _CIMCVAPPMESSAGEEXTENSIONSHANDLER_H
#define _CIMCVAPPMESSAGEEXTENSIONSHANDLER_H

//  INCLUDES
#include <e32std.h>
#include <e32base.h>

#include "mimcvappmessageextension.h"

// FORWARD DECLARATIONS
class CEikRichTextEditor;
class TCursorSelection;
class CIMCVAppUi;
class MIMCVAppUi;
class MIMCVUiAppSkinVariant;
class MIMCVEngineFactory;
class CIMCVEngine;
// CLASS DECLARATION

/**
*  Message extensions handler
*  @since 5.0
*/
class CIMCVAppMessageExtensionsHandler : public CBase
    {
    public:

        /**
         * Two-phased constructor
         * @param aAddSmileyHandler	
         * @since 5.0
         */
        static CIMCVAppMessageExtensionsHandler* NewL(
        						MIMCVUiAppSkinVariant& aSkinVariant,
                             	MIMCVAppUi& aAppUi,
                             	MIMCVEngineFactory& aEngineFactory,
                             	TBool aAddSmileyHandler = ETrue );

        /**
         * Destructor	
         * @since 5.0
         */
        ~CIMCVAppMessageExtensionsHandler();

    private:
        /**
         * Symbian OS Constructor	
         * @since 5.0
         */
        void ConstructL();

        /**
         * Constructor	
         * @since 5.0
         */
        CIMCVAppMessageExtensionsHandler(MIMCVUiAppSkinVariant& aSkinVariant,
                                        MIMCVAppUi& aAppUi,                     
                                        MIMCVEngineFactory& aEngineFactory  );

    public: 
        /*
         * new service has been installed
         */
        void CreateServiceSmileyL( TInt aServiceId );
        /*
         *  service has been deleted
          */
        void DeleteServiceSmileyL( TInt aServiceId );
        
    public:
       
       
        /**
         *	Process editor messages for sending them as characters.
         *  Resets extensions.
         *  @param aEditor Editor which contains message.
         *  @since 5.0
         *  @return Ownership of message returned is passed to caller.
         */
        HBufC* ProcessMessageLC( CEikRichTextEditor& aEditor, TInt aServiceId );

        /**
         *	Convert extensions to characters.
         *  @param aEditor Editor which contains message.
         *  @since 5.0
         *  @return Ownership of message returned is passed to caller.
         */
        HBufC* ExtensionToStringLC( CEikRichTextEditor& aEditor, TInt aServiceId );

       
        /**
         *  Delete extension from editor
         *  @param aEditor Editor which contains message.
         *  @since 5.0
         */
        void DeleteExtensionL( CEikRichTextEditor& aEditor,
                TInt aServiceId,
                TMessageExtensionTypes aType, TInt aPos );

        /**
         *  Converts all the extensions in selection area from 
         *  strings to extensions. 	
         *  @since 2.6
         *  @param aEditor, used editor
         */
        void ConvertSelectionToExtensionL( CEikRichTextEditor& aEditor, TInt aServiceId);
        
        /**
         *  Converts all the extensions in selection area from 
         *  strings to extensions. 	
         *  @since 3.0
         *  @param aRichText Rich text to be converted
         *  @param aSelection Selection area
         */
        void ConvertSelectionToExtensionL( CRichText& aRichText,
                TCursorSelection& aSelection ,
                TInt aServiceId );

        /**
         *  Converts all the extensions in selection area from 
         *  extensions to strings
         *  @since 2.6
         *  @param aEditor, used editor
         *  @param aPreserve, should we preserve original extension status. 
         *                    Are they strings or extensions.
         */
        void ConvertSelectionToStringL( CEikRichTextEditor& aEditor,
                TInt aServiceId,
                TBool aPreserve = EFalse );
        
        /**         
         * Informs the extensions about changed size
         * @param aSize
         * @since 3.0
         */
        void SizeChanged( TSize& aSize, TInt aServiceId );
        
        /**
         * Converts all the extensions in selection area from 
         * extensions to strings.
         * @since S60 v3.1
         * @param aEditor Used editor.
         * @param aResultString Descriptor for converted string,
         *                      caller is responsible to provide
         *                      large enough descriptor.
         * @param aSelectionAfterConversion Cursor selection after
         *                      extensions are converted to strings.
         * @param aPreserve Should we preserve original extension status.
         */
        void ConvertSelectionToStringL(
            CEikRichTextEditor& aEditor,
            TInt aServiceId,
            TDes& aResultString,
            TCursorSelection& aSelectionAfterConversion,
            TBool aPreserve );
            
         /**
         *  Insert extension to editor
         *  @param aEditor Editor which contains message.
         *  @param aType Type of extension needed.
         *  @param aCancelled ETrue after method returns if 
         *                    extension insertion was cancelled,
         *                    EFalse otherwise
         *  @since 2.1
         */
       void InsertExtensionL( CEikRichTextEditor& aEditor,
                        TInt aServiceId,  
       					TMessageExtensionTypes aType,TBool& aCancelled, TSize size );
                                                  
    private:
        // not owns
        MIMCVUiAppSkinVariant& iSkinVariant;
        // not owns
        MIMCVAppUi& iAppUi;   
        // not owns
        MIMCVEngineFactory& iEngineFactory;
        // owned , Array of extensions
        RPointerArray<MIMCVAppMessageExtension> iExtensionArray;
     
      
    };

#endif      // _CIMCVAPPMESSAGEEXTENSIONSHANDLER_H

// End of File
