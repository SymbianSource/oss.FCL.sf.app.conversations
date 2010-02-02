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


#ifndef CIMCVAPPSMILEYUTIL_H
#define CIMCVAPPSMILEYUTIL_H

// INCLUDE

#include "mimcvappmessageextension.h"
#include "cimcvappsmileiconutility.h"
#include <e32base.h>

// FORWARD DECLARATIONS

class CIMCVAppSmileIconUtility;
class MIMCVAppUi;
class MIMCVUiAppSkinVariant;
class CIMCVEngine;

// CLASS DECLARATION


/**
* CIMCVAPPSmileyUtil
*
* Utility for dealing with smileys in editor
*/
class CIMCVAppSmileyUtil : public CBase, public MIMCVAppMessageExtension
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * aAppUi CIMCVAppUI reference
        * @param aType message extension type
        */
		static CIMCVAppSmileyUtil* NewL( MIMCVUiAppSkinVariant& aSkinVariant,  
                             MIMCVAppUi& aAppUi, 
                             TMessageExtensionTypes aType,
                             CIMCVEngine& aServiceEngine);

        /**
        * Destructor.
        */
        virtual ~CIMCVAppSmileyUtil();

	private: // From MIMCVAPPMessageExtension
       
      

        /**
        * Inserts icon to editor
        * @param anEditor Reference to editor
        */
		void InsertExtensionL( CEikRichTextEditor& anEditor,
		                       TBool& aCancelled, TSize size );

		/**
		* Deletes icon from editor
		* @param aEditor Reference to editor
		* @param aPos Position where icon is located
		*/
		void DeleteExtensionL( CEikRichTextEditor& aEditor, TInt aPos );

		/**
		* Converts smiley icons to string smileys
		* @param anEditor Reference to editor
		* @param aDes Text from editor
		*/
		void ExtensionToStringL( CEikRichTextEditor& anEditor, TDes& aDes );

        /**
         *  @see MIMCVAPPMessageExtension	
         */
        void ConvertSelectionToExtensionL( CEikRichTextEditor& aEditor );
        
        /**
         *  Converts all the extensions in selection area from strings to extensions. 	
         *  @since 3.0
         *  @param aRichText Rich text to be converted
         *  @param aSelection Selection area
         */
        void ConvertSelectionToExtensionL( CRichText& aRichText, TCursorSelection& aSelection );

        /**
         *  @see MIMCVAPPMessageExtension	
         */
        void ConvertSelectionToStringL( CEikRichTextEditor& aEditor, TBool aPreserve );

        /**
         *	@see MIMCVAPPMessageExtension
         */
        void Reset();
        
        /**         
         * Informs the extensions about changed size
         * @param aSize
         */
        void SizeChanged( TSize& aSize );

        /**
         *	@see MIMCVAPPMessageExtension
         */
        void ConvertSelectionToStringL(
            CEikRichTextEditor& aEditor,
            TDes& aResultString,
            TCursorSelection& aSelectionAfterConversion,
            TBool aPreserve );
        
        
       TInt ServiceId() const ;
                                          
    private:  // Constructors and destructor

        /**
        * C++ constructor.
        * @param aType Type of TMessageExtensionTypes
        */
        CIMCVAppSmileyUtil( TMessageExtensionTypes aType  );

        /**
        * Second-phased constructor
        */
        void ConstructL( MIMCVUiAppSkinVariant& aSkinVariant, 
                          MIMCVAppUi& aAppUi,
                          CIMCVEngine& aServiceEngine );

    private: // Data

		//util for icon/string convertings
		CIMCVAppSmileIconUtility* iSmileUtil;

		// Doesn't own
		CEikRichTextEditor* iEditor;

        // Count of current smiley icons in editor
		TInt iCurrentCount;

        // Is smiley converted or not
        RPointerArray< CIMCVAppSmileyInformation > iConvertArray;
        
        // service id
        TInt iServiceId;
       
    };

#endif  // CIMCVAPPSMILEYUTIL_H
