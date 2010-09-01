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
* Description:  message extention interface
*
*/


#ifndef MIMCVAPPMESSAGEEXTENSION_H
#define MIMCVAPPMESSAGEEXTENSION_H

//  INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS
class CEikRichTextEditor;
class TCursorSelection;
class CRichText;
class MBSAccess;

// Type definitions for different smiley types
 enum TMessageExtensionTypes
    {
        EMessageExtensionSmiley = 0
    };

// CLASS DECLARATION

/**
*  Interface class for message extension classes
*/
class MIMCVAppMessageExtension
    {
    public:
        /**
         * Return type of Extension. Inherited message extension does not need
         * information of type. It is only needed by handler of extensions and
         * user of extension. That is why implementation is created in here.
         * @since 5.0
         * @return Type of extension
         */
        virtual TMessageExtensionTypes Type() const
            {
            return iType;
            }

        /**
         * Destructor	
         * @since 5.0
         */
        virtual ~MIMCVAppMessageExtension(){};

    protected:

        /** 
         *	Constructor Inherited message extension does not need
         *  information of type. It is only needed by handler of extensions and
         *  user of extension. That is why implementation is created in here.
         *  @since 5.0
         *  @param aType Type of extension
         */
        MIMCVAppMessageExtension( TMessageExtensionTypes aType ) : iType( aType ){}

    public: //Interface methods
        
        
		/**
        * Removes extension from given position in Editor
        * @param aEditor used editor
        * @param aPos    extension position in Editor
        * @since 5.0
        */
        virtual void DeleteExtensionL( CEikRichTextEditor& aEditor, 
                                       TInt aPos ) = 0;

		/**
		* Converts smiley icons to string smileys
        * @since 5.0
        * @param aEditor    used editor
		* @param aDes       Text from editor
		*/
		virtual void ExtensionToStringL( CEikRichTextEditor& aEditor, 
		                                 TDes& aDes ) = 0;

        /**
         *  Converts extension in selection area from strings to extensions.
         *  Used for example for cut, copy, paste features.
         *  @since 5.0
         *  @param aEditor, Used editor
         */
        virtual void ConvertSelectionToExtensionL( 
                                CEikRichTextEditor& aEditor ) = 0;
        
        /**
         *  Converts all the extensions in selection area from 
         *  strings to extensions. 	
         *  @since 5.0
         *  @param aRichText Rich text to be converted
         *  @param aSelection Selection area
         */
        virtual void ConvertSelectionToExtensionL( CRichText& aRichText, 
                                            TCursorSelection& aSelection ) = 0;

        /**
         *  Converts extension in selection area from extensions to strings.
         *  Used for example for cut, copy, paste features.
         *  @since 5.0
         *  @param aEditor, Used editor
         *  @param aPreserve, Preserve original status of extension
         */
        virtual void ConvertSelectionToStringL( CEikRichTextEditor& aEditor, 
                                                TBool aPreserve ) = 0;

        /**
         *  Reset extension counters.
         *  @sinze 5.0
         */
        virtual void Reset() = 0;
        
        /**         
         * Informs the extensions about changed size
         * @param aSize
         * @since 5.0
         */
        virtual void SizeChanged( TSize& aSize ) = 0;

        /**
         * Converts all the extensions in selection area from 
         * extensions to strings.
         * @since S60 v5.0
         * @param aEditor Used editor.
         * @param aResultString Descriptor for converted string,
         *                      caller is responsible to provide
         *                      large enough descriptor.
         * @param aSelectionAfterConversion Cursor selection after
         *                      extensions are converted to strings.
         * @param aPreserve Should we preserve original extension status.
         */
        virtual void ConvertSelectionToStringL( 
            CEikRichTextEditor& aEditor,
            TDes& aResultString,
            TCursorSelection& aSelectionAfterConversion,
            TBool aPreserve ) = 0;
            
               /**
        * Adds extension to current position in Editor
        * @param aEditor used editor
        * @param aCancelled
        * @since 2.1
        */
		virtual void InsertExtensionL( CEikRichTextEditor& aEditor,
		                               TBool& aCancelled, TSize size ) = 0;
		
		/*
		 * return service Id used
		 */
		virtual TInt ServiceId() const = 0 ;
            
    private:

        // Type of extension
        TMessageExtensionTypes iType;
        
    };

#endif      // MIMCVAPPMESSAGEEXTENSION_H

// End of File
