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
* Description:  Implementation of detailsview plugin menu handler
 *
*/


#ifndef CVIMPSTDETAILSVIEWMENUHANDLER_H
#define CVIMPSTDETAILSVIEWMENUHANDLER_H

#include <e32base.h>
#include <AiwServiceHandler.h>
#include "cvimpstdetailscontacthandler.h"

class CVIMPSTDetailsViewPlugin;
class CAiwServiceHandler;
/**
 *  Class implementing vimpstdetailsviewplugin menu functionality
 *
 *  @lib vimpstdetailsviewplugin.dll
 *  @since S60 v3.2
 */
 
class CVIMPSTDetailsViewMenuHandler : public CBase, public MAiwNotifyCallback
	{

	public:

	    /**
	     * Two-phased constructor.
	     * @param aPlugin, reference to CVIMPSTDetailsViewPlugin class
	     * @param aStoreType, store type to indicate wether server contact store or local store
	     * @return Pointer to this class
	     * @since S60 v5.0
	     */
	    
	    static CVIMPSTDetailsViewMenuHandler* NewL(CVIMPSTDetailsViewPlugin& aPlugin,
	                                                TStoreType aStoreType );

	    /**
	     * Destructor.
	     * @since S60 v5.0
	     */
	    ~CVIMPSTDetailsViewMenuHandler();

	public: // from base class CCCAppViewPluginBase

	     /**
	     * From CCCAppViewPluginBase
	     * (see details from header)
	     * @param aResourceId, a resource id 
	     * @param aMenuPane, reference to menu pane
	     * @since S60 v5.0
	     */
	    void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);

	     /**
	     * From CCCAppViewPluginBase
	     * (see details from header)
	     * @param aCommand, a command id to handle 
	     * @since S60 v5.0
	     */
	    void HandleCommandL(TInt aCommand);

	private:

	      
	    /**
	     * edit the selected field 
	     *
	     * @since S60 v5.0
	     */
	    void DoEditCmdL();
	        
	    /**
	     * delete the contact
	     *
	     * @since S60 v5.0
	     */
	     
	    void DoDeleteCmdL();
	    
	     /**
	     * copy the clip board of selected field
	     *
	     * @since S60 v5.0
	     */
	    void DoCopyCmdL();
	
	    /**
	     * Prepares plugin for AIW-menu usage
	     *
	     * @since S60 v5.0
	     */
	    void PrepareAiwMenusL();	
	   
	private: // constructors

		/**
	     * default constructor
	     * @param aPlugin, reference to CVIMPSTDetailsViewPlugin class
	     * @param aStoreType, store type to indicate wether server contact store or local store
	     * @since S60 v5.0
	     */
	    CVIMPSTDetailsViewMenuHandler(CVIMPSTDetailsViewPlugin& aPlugin, 
	                                   TStoreType aStoreType);

		/**
	     * provide the two phase construction 
	     *
	     * @since S60 v5.0
	     */
	    void ConstructL();

	public: // From MAiwNotifyCallback
	    
	    /**
	     * call back method for aiwcommands. MAiwNotifyCallback
	     * @param aCmdId, id of the aiwcommand
	     * @param aEventId, event id of the aiw command
	     * @param EventParamList, callback data is passed 
	     * @param aInParamList, aiw command params
	     * @since S60 v5.0
	     */
	    TInt HandleNotifyL(
	            TInt aCmdId,
	            TInt aEventId,
	            CAiwGenericParamList& aEventParamList,
	            const CAiwGenericParamList& aInParamList);
	private: // data

	    /**
	     * Not Owns : reference to class handling the view
	     */
	    CVIMPSTDetailsViewPlugin& iPlugin;

	    /**
	     * Pointer to AIW service handler
	     * Own.
	     */
	    CAiwServiceHandler* iAiwServiceHandler;
	    
	    // owns : store type
	    TStoreType iStoreType;
	  	};

	#endif // CVIMPSTDETAILSVIEWMENUHANDLER_H

	// End of File
