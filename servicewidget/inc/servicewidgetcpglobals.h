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
* Description:  constant declaration class
 *                
 *
*/

 

#ifndef SERVICEWIDGETLOBALS_H
#define SERVICEWIDGETLOBALS_H

#include <vwsdef.h>

// meco service uid
const TInt32 KMECOIMPLEMENTATIONUID ( 0x20012423 );

/*
* service tab app and view uid	
*/
const TInt32 KIMServicetabPluginUid ( 0x20007B6D );

/*
* define constants
*/
#define KIMWCP_FIRSTTEXT_SIZE 50	 // first row text length
#define KIMWCP_SECONDTEXT_SIZE 100	 // second row text length
#define KIMW_MAX_LENGTH 512			 // own user id
#define KTimerInterval 3000000 		 // 3  sc timer expiration time
#define KSW_RESOURCE_MAX_LENGTH 512	 // resource path max length
#define KIMWCP_ADD_DATA_SIZE 100     // additional data length
/*
* resource related strings
*/
_LIT(KResourceDrive, "\\resource\\" );
_LIT(KResourceFileName, "servicewidgetpluginres.rsc" );

/*
* some special character 
*/
_LIT(KAt, "@" );
_LIT(KSlash, "/");
_LIT(KColon,":");

/*
* content publishing service
*/
_LIT8( KCPInterface, "IDataSource" );
_LIT8( KCPContentInterface, "IContentPublishing" );
_LIT8( KCPService, "Service.ContentPublishing" );

/*
 * publisher id and types supported
 */
_LIT(KMask,"_mask");

//Supported Commands
_LIT8( KAdd, "Add" );
// Input arguments
_LIT8( KType, "type" );
_LIT8( KItem, "item" );
_LIT8( KPublisherId, "publisher" );
_LIT8( KContentType, "content_type" );
_LIT8( KContentId, "content_id" );
_LIT( KCpData, "cp_data" );
 
 /*
 * own id content id and key defined in xml
 */
_LIT8(KOwnTextKey, "owntext");

/*
 * message content id and key defined in xml
 */
_LIT8(KMessageTextKey, "msgtext");

/*
 * serviceimage content id and key defined in xml
 */
_LIT8(KServiceImageKey, "serviceimage");

/*
 * own status content id  and key defined in xml
 */
_LIT8(KStatusIconKey, "statusimage");

/*
 * HandleNotify constant
 */
_LIT8( KActive, "active");
_LIT8( KDeActive, "deactive");
_LIT8( KSuspend, "suspend");
_LIT8( KInactive, "inactive");
_LIT8( KResume, "resume");
_LIT8( KData, "data" );
_LIT8( KDelete, "Delete" );
_LIT8(KSelected, "selected");
// Output
_LIT8( KDataMap, "data_map" );
_LIT8( KActionMap, "action_map" );

//Register Widget
_LIT(KServiceWidget, "servicewidget");
//UID 20021383 is of Publisher used here as constant to avoid conflict with any other 
//Homescreen widget.This will be suffix to publisher name to make unique id for theme.
_LIT( KThemeUid, "_20021383"); 
_LIT8( KTemplateType, "template_type");
_LIT8( KWidgetName,   "widget_name");
_LIT8( KWidgetInfo,   "widget_info"); 
_LIT(KTemplateWidget, "hstemplate");
_LIT8( KExecute, "execute" );
_LIT( KAll, "all");
_LIT8( KMaxWidgetInst, "widget_inst");
_LIT8( KWidgetDesc, "widget_desc");
_LIT8( KWidgetLogo, "widget_icon");


_LIT8( KRequestNotification, "RequestNotification" );
_LIT( KPublisher, "publisher" );
_LIT8( KFilter, "filter" );
_LIT(KTriggerMap, "data");
_LIT8( KChangeInfo, "change_info" );
_LIT8( KActionTrigger, "action_trigger" );
_LIT8( KOperation, "operation" );

#endif /*SERVICEWIDGETLOBALS_H*/
