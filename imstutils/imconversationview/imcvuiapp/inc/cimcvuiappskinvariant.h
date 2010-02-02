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
* Description:  skin variation  class
*
*/



#ifndef CIMCVUIAPPSKINVARIANT_H
#define CIMCVUIAPPSKINVARIANT_H

//  INCLUDES
#include "mimcvuiappskinvariant.h"

#include <e32base.h>

// FORWARD DECLARATIONS
class CCoeControl;
class CAknsBasicBackgroundControlContext;
class CGulIcon;
class TAknsItemID;
// CLASS DECLARATION

/**
*  Active implementation of skin feature.
*  See base class MCASkinVariant.h for comments.
*  @imcvuiapp.exe
*  @since 5.0
*/

class CIMCVUiAppSkinVariant : public CBase, 
							  public MIMCVUiAppSkinVariant
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CIMCVUiAppSkinVariant* NewL(  );

        /**
        * Destructor.
        */
        virtual ~CIMCVUiAppSkinVariant();

      public: // Functions from base classes MIMCVUiAppSkinVariant

                
        /**
        * Clears listbox background.
        * @since 5.0
        * @param aGc, graphics context to draw to.
		* @param aUsedPortionOfViewRect, rect to draw to.
		* @param aContext, skin control context.
        */
		void ClearChatListBoxBackGround( CWindowGc& aGc, 
		                                 const TRect& aUsedPortionOfViewRect,
										 MAknsControlContext* aContext );
		
		/**
        * Returns main pane background control context.
        * @since 5.0
        * @return MAknsControlContext, skin control context.
        */								 
		MAknsControlContext* MainPaneBackgroundContext() const;
		
		/**
		* Update layout for skinvariant
		* @return None
		*/
		void UpdateLayout();
		
		/**
        * Draws skinned version of the message editor.
        * @since 5.0
        * @param aGc, graphics context.
		* @param aParentRect, rects needed for drawing
		* @param aEditorRect
		* @param aOutlineRect
        * @return TBool, whether skins were drawn or not.
        */
		TBool DrawSkinnedEditor( const CCoeControl* aControl, 
                                 CWindowGc& aGc, 
                                 TRect& aParentRect, 
                                 TRect& aEditorRect, 
                                 TRect& aOutlineRect );
		 /**
        * Loads skinned/normal IM application bitmaps.
        * @since 1.2s
        * @param aBitmapId, bitmap id
        * @param aMaskId, bitmap mask's id
        * @param aFullPath, bitmap path
        * @param aOverrideBrand, if true, bitmap is got from skins, not through
        *               brand bitmaps (if brand is active)
        * @param aCustomColor  If provided, will change the color of the bitmap
        *                      to this color.
        * @return CGulIcon, created icon, user of this method gets the ownership
        */
        CGulIcon* LoadBitmapL( TInt aBitmapId, TInt aMaskId,
                                       const TDesC& aFullPath,
                                       TBool aOverrideBrand = EFalse,
                                       TRgb* aCustomColor = NULL ) ;
                                       
   private: 
	
	    void MapFromBitmapIdToAknsItemId( TInt aBitmapId, 
	                                      TAknsItemID& aItemId, 
	                                      TInt& aColorGroupId );
	                                      
	private:

		/**
		* C++ default constructor.
		*/
		CIMCVUiAppSkinVariant();

		/**
		* By default Symbian 2nd phase constructor is private.
		*/
		void ConstructL();

	private:    // Data

		// skin background control context
		CAknsBasicBackgroundControlContext* iBgContext;
		
			
	
    };

#endif      // CIMCVUIAPPSKINVARIANT_H

// End of File
