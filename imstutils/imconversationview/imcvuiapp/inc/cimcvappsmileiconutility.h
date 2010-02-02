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
*  Description : Handles statuspane, context pane, navi pane
*
*/


#ifndef CIMCVAPPSMILEICONUTILITY_H
#define CIMCVAPPSMILEICONUTILITY_H

// INCLUDES
#include <e32base.h>
#include "mimcvappresourcechangeobserver.h"
#include <gdi.h>

// FORWARD DECLARATIONS
class CGulIcon;
class CIMCVAppSmileString;
class CIMCVAppSmileyInformation;
class MIMCVUiAppSkinVariant;
class CIMCVUiAppIconFileProvider;
class CIMCVEngine;


class MIMCVAppUi;

// CLASS DECLARATION

/**
* CIMCVAppSmileIconUtility
*
* Utility for selecting smile icon and mapping
* between smile icons and smile strings.
*/
class CIMCVAppSmileIconUtility : public CBase,
                            public MIMCVAppResourceChangeObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor using default smile icon array.
        *@param aAppUi : CIMCVAppUI reference
        */
        static CIMCVAppSmileIconUtility* NewL(
        					 MIMCVUiAppSkinVariant& aSkinVariant,
                             MIMCVAppUi& aAppUi,
                             CIMCVEngine& aServiceEngine );   

        /**
        * Destructor.
        */
        ~CIMCVAppSmileIconUtility();

	public:
       
        /**
        * Launch pop-up dialog for selecting smile icon.
		* @param aSelectedSmileIndex Returns selected smile icon index
		* @return (TInt) Return dialog value
        */
		TInt LaunchSmileIconDialogL( TInt& aSelectedSmileIndex );

        /**
        * Smile icon count.
		* @return (TInt) Smile icon count
        */
		TInt SmileIconCount() const;

        /**
        * Return needed smile icon pointer from icon array.
		* @param aSmileIndex Smile icon index
        */
		const CGulIcon* GetSmileIcon( TInt aSmileIndex ) const;

        /**
        * Return needed smile string reference from string array.
		* @param aSmileIndex Smile icon index
        */
		const TDesC& GetSmileString( TInt aSmileIndex ) const;

		/**
        *   Search smileys from buffer. Replace every found smiley with KPuaCodeSmileyIconPadding 
        *   @param aStr String to search smiles from.
        *   @param aFixedStr, Target string where smileys are replaced with KPUaCodeSmileyIconPadding
		*   @param aArray Array reference to icon array indexes in right order
        */
		void SearchSmilesL( const TDesC& aStr, TDes& aFixedStr,
									 RArray<TInt>* aArray );

        /**
        *   Search smileys from buffer.
        *   @param aStr String to search smiles from.
        *   @param aSmileyInfoArray Information of found smileys.
        *   @param aFixedStr, Target string where smileys are replaced with KPUaCodeSmileyIconPadding
        *   @param aStartPos, Absolute position of index 0 in source string. Used when position of smiley is calculated.
        */
        void SearchSmilesL( const TDesC& aStr, 
        					RPointerArray< CIMCVAppSmileyInformation >& aSmileyInfoArray, 
        					TDes* aFixedStr = NULL, TInt aStartPos = 0 ) const;

		/**
		* Compares two TSmileyInformation objects' position field.
		* @param aFirst Reference to the first TSmileyInformation
		* @param aSecond Reference to the second TSmileyInformation
		* @return (TInt) Positive if aFirst > aSecond, negative if opposite,
		*			     0 if aFirst == aSecond
		*/
        static TInt Compare( const CIMCVAppSmileyInformation& aFirst,
                             const CIMCVAppSmileyInformation& aSecond );
        
		/**
		* Compares two CIMCVAppSmileString objects' smiley length.
		* @param aFirst Reference to the first CIMCVAppSmileString
		* @param aSecond Reference to the second CIMCVAppSmileString
		* @return (TInt) Positive if aFirst(length) < aSecond(length), 
        *                negative if opposite, 0 if aFirst == aSecond
		*/
        static TInt CompareSmileyLengthReversed( 
                             const CIMCVAppSmileString& aFirst,
                             const CIMCVAppSmileString& aSecond );
		
		/**
		* Resizes all icons.
		* @param aSize new icon size
		*/
		void ResizeIcons( const TSize& aSize );
		
	    
    	/**
         * @return aResourceId to use
        */
		void ConstructSmileDialogIconArrayL( CIMCVEngine& aServiceEngine );
		
		/**
		 * parses whole smiley string from branding and extracts each smiley code
		 */		
		void ParseAllSmileyStringL(RPointerArray<HBufC> aAllSmileyString);
		
    public: // From MIMCVAppResourceChangeObserver
    
        /**
         * Reload all icons on resource change
         */
        void ResourceChangedL();

    private:  // Constructors and destructor

        /**
        * C++ constructor.
        */
        CIMCVAppSmileIconUtility( 
                             MIMCVUiAppSkinVariant& aSkinVariant, 
                             MIMCVAppUi& aAppUi );

        /**
        * Second-phased constructor
        */
        void ConstructL(CIMCVEngine& aServiceEngine );

	private: // New methods		
		
        /**
         * Seek the first smiley which points to a given icon.
         * This is slow method, and it should only be used to 
         * precalculate the index table.
         * @param aIconIndex index of icon in iIconArray
         * @return index of smiley in iStringArray
         */
        TInt GetFirstSmileyIndex( TInt aIconIndex ) const;
        
        
       
    private: // Data
		// NOT Own
		MIMCVUiAppSkinVariant& iSkinVariant;       
		
		// Own. Smiley strings with ref to icon array 
		RPointerArray<CIMCVAppSmileString> iStringArray;
        
        // Own. Temporary array for searching the smileys.
        // This should be empty when not searching.
        RPointerArray<CIMCVAppSmileyInformation> iSmileArray;
        
        // Own. Icon reference to string smileys.
        // Using a precalculated array makes seeking a lot faster,
        RArray<TInt> iIconToSmileyPointers;
        
        // Cache length, so we don't need the seek this every time.
        TInt iLongestSmileyLength;
        
		// does not own        
        MIMCVAppUi& iAppUi;       
        
         //Own. Smiley icons for SmileDialog
        RPointerArray<CGulIcon> iSmileDlgIconArray;
        
        //Owns
        RFile iFile;
        
      
    };

#endif  // CIMCVAPPSMILEICONUTILITY_H
