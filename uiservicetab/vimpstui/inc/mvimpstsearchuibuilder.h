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
* Description:  base class for search ui builder 
*
*/


#ifndef MVIMPSTSEARCHUIBUILDER_H
#define MVIMPSTSEARCHUIBUILDER_H

// INCLUDES
#include <e32def.h>

// FORWARD DECLARATIONS
class CCoeControl;
class CEikCaptionedControl;
// CLASS DECLARATION

/**
 * search field UI builder interface.
 */
class MVIMPSTSearchUiBuilder
    {
    public:  // Interface
    
		/**
         * create a new line
         * @param aCaption, caption of field line 
		 * @param aControlId, control id  
	  	 * @param aControlType, control type 
         * @return  A new instance of CCoeControl.
         */
        virtual CCoeControl* CreateLineL(
                const TDesC& aCaption,
                TInt aControlId,
                TInt aControlType ) = 0;

				
		/**
		 * Return the control corresponding to given id.
         * @param aControlId    Id of the control.
		 * @return  The control or NULL if the control does not exist.
		 */
		virtual CCoeControl* Control(
                TInt aControlId ) const = 0;

        /**
         * Returns the control line corresponding to given id.
         * @param aControlId    Id of the control.
         * @return  Line control.
         */
        virtual CEikCaptionedControl* LineControl(
                TInt aControlId ) const = 0;

        
		/**
		 * Try to change the focus to given control.
         * @param aControlId    Id of the control.
		 */
		virtual void TryChangeFocusL(
                TInt aControlId ) = 0;

		/**
		 * Change the line controls caption in the UI.
         * @param aText The caption to set.
		 */
		virtual void SetCurrentLineCaptionL(
                const TDesC& aText ) = 0;

		/**
		 * Sets the field to editable state.
         * @param aState    ETrue to set editable, EFalse otherwise.
		 */
		virtual void SetEditableL(
                TBool aState ) = 0;

       
    protected: // Disabled functions
    
        virtual ~MVIMPSTSearchUiBuilder()
                {};
    };

#endif // MVIMPSTSEARCHUIBUILDER_H

// End of File
