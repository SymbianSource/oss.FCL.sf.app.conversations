/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: This file contains STIFUnit implementation.
*
*/

/**
 * STIF_UNIT_INCLUDE SECTION - put all #includes between STIF_UNIT_INCLUDE_SECTION
 *                             and STIF_UNIT_INCLUDE_SECTION_END
 */
#ifdef STIF_UNIT_INCLUDE_SECTION
 

#endif //STIF_UNIT_INCLUDE_SECTION_END

/**
 * GLOBAL VARIABLES SECTION
 */
#ifdef TEST_VAR_DECLARATIONS
	/**
	 * Example of variable common for some test cases
	 */
	 
#endif
/**
 * END OF GLOBAL VARIABLES SECTION
 */


/**
 * TEST CASES SECTION
 */
#ifdef TEST_CASES
/**
 * STIF_SETUP defines activities needed before every test case.
 */
STIF_SETUP
{
	/** Example of use of STIF_SETUP - a variable common for some test cases is initialized
	 */

	
}

/**
 * STIF_TEARDOWN defines activities needed after every test case
 */
STIF_TEARDOWN
{
	/** Example of use of STIF_TEARDOWN - a variable common for some test cases is destroyed
	 */

}

/**
 * STIF_TESTDEFINE defines a test case
 *
 *  Example test case - length of string is checked.
 *  The only argument of macro is a name of test case.
 */
STIF_TESTDEFINE(t_CreateCommand_login)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
TInt data  = 1;
MVIMPSTCmd* commd = factory->CreateCommandForIdL(ELoginService,&data) ;
//delete factory;
//factory = NULL;
TInt comm_sent = ELoginService;
TInt comm_recev = commd->CommandId();
commd->ExecuteLD();
STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}


STIF_TESTDEFINE(t_CreateCommand_logout)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
TInt data  = 1;
MVIMPSTCmd* commd = factory->CreateCommandForIdL(ELogoutService,&data) ;
TInt comm_sent = ELogoutService;
TInt comm_recev = commd->CommandId();
commd->ExecuteLD();
STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}

STIF_TESTDEFINE(t_CreateCommand_LaunchCCA)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
TLaunchCCAData data;
data.iIndex = 1;
data.iConnection = MCCAConnectionStub::NewL();
MVIMPSTCmd* commd = factory->CreateCommandForIdL(ELaunchCCA,&data) ;
TInt comm_sent = ELaunchCCA;
TInt comm_recev = commd->CommandId();
//commd->ExecuteLD();
STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}

STIF_TESTDEFINE(t_CreateCommand_AddContact)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
TInt data  = 1;
MVIMPSTCmd* commd = factory->CreateCommandForIdL(EAddContact,&data) ;
TInt comm_sent = EAddContact;
TInt comm_recev = commd->CommandId();
commd->ExecuteLD();
STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}

STIF_TESTDEFINE(t_CreateCommand_DeleteContact)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
TInt data  = 1;
MVIMPSTCmd* commd = factory->CreateCommandForIdL(EDeleteContact,&data) ;
delete factory;
factory = NULL;
TInt comm_sent = EDeleteContact;
TInt comm_recev = commd->CommandId();
commd->ExecuteLD();
STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}


STIF_TESTDEFINE(t_CreateCommand_ChangeOwnStatus)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
TStatusAndStatusText status;
_LIT(KStatusMsg,"away");
status.iStatusText.Copy(KStatusMsg);
MVIMPSTCmd* commd = factory->CreateCommandForIdL(EChangeOwnStatus,&status) ;
delete factory;
factory = NULL;
TInt comm_sent = EChangeOwnStatus;
TInt comm_recev = commd->CommandId();
commd->ExecuteLD();
STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}

STIF_TESTDEFINE(t_CreateCommand_ChangeOwnMessage)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);

TStatusAndStatusText status;
_LIT(KStatusMsg,"away");
status.iStatusText.Copy(KStatusMsg);

TInt data  = 1;
MVIMPSTCmd* commd = factory->CreateCommandForIdL(EChangeOwnMessage,&status) ;
delete factory;
factory = NULL;
TInt comm_sent = EChangeOwnMessage;
TInt comm_recev = commd->CommandId();
commd->ExecuteLD();
STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}

STIF_TESTDEFINE(t_CreateCommand_Search)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
RArray<TVIMPSTSearchKeyData> aKeyDataArray;

MVIMPSTCmd* commd = factory->CreateCommandForIdL(ESearch,&aKeyDataArray) ;
delete factory;
factory = NULL;
TInt comm_sent = ESearch;
TInt comm_recev = commd->CommandId();
commd->ExecuteLD();
STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}


STIF_TESTDEFINE(t_CreateCommand_FriendRejected)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
_LIT(KContactId,"contact");
TBufC<10> buf (KContactId);
MVIMPSTCmd* commd = factory->CreateCommandForIdL(EFriendRejected, &buf) ;
delete factory;
factory = NULL;
TInt comm_sent = EFriendRejected;
TInt comm_recev = commd->CommandId();
commd->ExecuteLD();
STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}


STIF_TESTDEFINE(t_CreateCommand_ChangeOwnAvtar)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
TAvatarData data;

MVIMPSTCmd* commd = factory->CreateCommandForIdL(EChangeOwnAvtar,&data) ;
delete factory;
factory = NULL;
TInt comm_sent = EChangeOwnAvtar;
TInt comm_recev = commd->CommandId();
commd->ExecuteLD();
STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}


STIF_TESTDEFINE(t_CreateCommand_CloseConversation)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
_LIT(KContactId,"contact");
TBufC<10> buf(KContactId);
MVIMPSTCmd* commd = factory->CreateCommandForIdL(ECloseConversation,&buf) ;
delete factory;
factory = NULL;
TInt comm_sent = ECloseConversation;
TInt comm_recev = commd->CommandId();
commd->ExecuteLD();
STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}


STIF_TESTDEFINE(t_CreateCommand_AddToPbk)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);

vimpststoragecontact_stub store;

MVIMPSTCmd* commd = factory->CreateCommandForIdL(EAddToPbk,&store) ;
delete factory;
factory = NULL;
TInt comm_sent = EAddToPbk;
TInt comm_recev = commd->CommandId();
commd->ExecuteLD();
STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}


STIF_TESTDEFINE(t_CreateCommand_CancelLogin)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
TInt data  = 1;
MVIMPSTCmd* commd = factory->CreateCommandForIdL(ECancelLogin,&data) ;
delete factory;
factory = NULL;
TInt comm_sent = ECancelLogin;
TInt comm_recev = commd->CommandId();
commd->ExecuteLD();
STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}

STIF_TESTDEFINE(t_CreateCommand_ProcessAcceptedContactNew)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
TVPbkSelectedData iSelectedData;

_LIT8(KContactId,"contact");
TPtrC8 buddy = KContactId();
iSelectedData.iPackedLinks = buddy.AllocL();
MVIMPSTCmd* commd = factory->CreateCommandForIdL(ECmdProcessAcceptedContactNew,&iSelectedData) ;
//delete factory;
//factory = NULL;
TInt comm_sent = ECmdProcessAcceptedContactNew;
TInt comm_recev = commd->CommandId();
commd->ExecuteLD();
STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}
STIF_TESTDEFINE(t_CreateCommand_ProcessSelectedContactNew)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
TVPbkSelectedData iSelectedData;

_LIT8(KContactId,"contact");
TPtrC8 buddy = KContactId();
iSelectedData.iPackedLinks = buddy.AllocL();
MVIMPSTCmd* commd = factory->CreateCommandForIdL(ECmdProcessSelectedContactNew,&iSelectedData) ;
//delete factory;
//factory = NULL;
TInt comm_sent = ECmdProcessSelectedContactNew;
TInt comm_recev = commd->CommandId();
commd->ExecuteLD();
STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}
STIF_TESTDEFINE(t_CreateCommand_ProcessAcceptedContactExist)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
TVPbkSelectedData iSelectedData;

_LIT8(KContactId,"contact");
TPtrC8 buddy = KContactId();
iSelectedData.iPackedLinks = buddy.AllocL();
MVIMPSTCmd* commd = factory->CreateCommandForIdL(ECmdProcessAcceptedContactExist,&iSelectedData) ;
//delete factory;
//factory = NULL;
TInt comm_sent = ECmdProcessAcceptedContactExist;
TInt comm_recev = commd->CommandId();
commd->ExecuteLD();
STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}
STIF_TESTDEFINE(t_CreateCommand_ProcessSelectedContactExist)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
TVPbkSelectedData iSelectedData;

_LIT8(KContactId,"contact");
TPtrC8 buddy = KContactId();
iSelectedData.iPackedLinks = buddy.AllocL();
MVIMPSTCmd* commd = factory->CreateCommandForIdL(ECmdProcessSelectedContactExist,&iSelectedData) ;
//delete factory;
//factory = NULL;
TInt comm_sent = ECmdProcessSelectedContactExist;
TInt comm_recev = commd->CommandId();
commd->ExecuteLD();
STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}

//============================ExecuteLD---------------------

STIF_TESTDEFINE(t_ExeCommand_login)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
TInt data  = 1;
MVIMPSTCmd* commd = factory->CreateCommandForIdL(ELoginService,&data) ;
TInt comm_sent = ELoginService;
TInt comm_recev = commd->CommandId();
vimpstcmdobserver_stub ob;
commd->AddObserver(ob);
commd->ExecuteLD();
STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}


STIF_TESTDEFINE(t_ExeCommand_logout)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
TInt data  = 1;
MVIMPSTCmd* commd = factory->CreateCommandForIdL(ELogoutService,&data) ;
TInt comm_sent = ELogoutService;
TInt comm_recev = commd->CommandId();
vimpstcmdobserver_stub ob;
commd->AddObserver(ob);
commd->ExecuteLD();
STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}

STIF_TESTDEFINE(t_ExeCommand_LaunchCCA)
{/*
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
TLaunchCCAData data;
data.iIndex = 2;
//TInt data  = 1;
MVIMPSTCmd* commd = factory->CreateCommandForIdL(ELaunchCCA,&data) ;
TInt comm_sent = ELaunchCCA;
TInt comm_recev = commd->CommandId();
vimpstcmdobserver_stub ob;
commd->AddObserver(ob);
//commd->iIndex = 0;
commd->ExecuteLD();
STIF_ASSERT_EQUALS(comm_sent, comm_recev );*/
}
STIF_TESTDEFINE(t_ExeCommand_AddContact)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
TInt data  = 1;
MVIMPSTCmd* commd = factory->CreateCommandForIdL(EAddContact,&data) ;
TInt comm_sent = EAddContact;
TInt comm_recev = commd->CommandId();
vimpstcmdobserver_stub ob;
commd->AddObserver(ob);
commd->ExecuteLD();

STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}

STIF_TESTDEFINE(t_ExeCommand_DeleteContact)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
TInt data  = 1;
MVIMPSTCmd* commd = factory->CreateCommandForIdL(EDeleteContact,&data) ;
delete factory;
factory = NULL;
TInt comm_sent = EDeleteContact;
TInt comm_recev = commd->CommandId();
vimpstcmdobserver_stub ob;
commd->AddObserver(ob);
commd->ExecuteLD();

STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}


STIF_TESTDEFINE(t_ExeCommand_ChangeOwnStatus)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
TStatusAndStatusText status;
_LIT(KStatusMsg,"away");
status.iStatusText.Copy(KStatusMsg);
MVIMPSTCmd* commd = factory->CreateCommandForIdL(EChangeOwnStatus,&status) ;
delete factory;
factory = NULL;
TInt comm_sent = EChangeOwnStatus;
TInt comm_recev = commd->CommandId();
vimpstcmdobserver_stub ob;
commd->AddObserver(ob);
commd->ExecuteLD();

STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}

STIF_TESTDEFINE(t_ExeCommand_ChangeOwnMessage)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);

TStatusAndStatusText status;
_LIT(KStatusMsg,"away");
status.iStatusText.Copy(KStatusMsg);

TInt data  = 1;
MVIMPSTCmd* commd = factory->CreateCommandForIdL(EChangeOwnMessage,&status) ;
delete factory;
factory = NULL;
TInt comm_sent = EChangeOwnMessage;
TInt comm_recev = commd->CommandId();
vimpstcmdobserver_stub ob;
commd->AddObserver(ob);
commd->ExecuteLD();

STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}

STIF_TESTDEFINE(t_ExeCommand_Search)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
RArray<TVIMPSTSearchKeyData> aKeyDataArray;

MVIMPSTCmd* commd = factory->CreateCommandForIdL(ESearch,&aKeyDataArray) ;
delete factory;
factory = NULL;
TInt comm_sent = ESearch;
TInt comm_recev = commd->CommandId();
vimpstcmdobserver_stub ob;
commd->AddObserver(ob);
commd->ExecuteLD();

STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}


STIF_TESTDEFINE(t_ExeCommand_FriendRejected)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
_LIT(KContactId,"contact");
TBufC<10> buf (KContactId);
MVIMPSTCmd* commd = factory->CreateCommandForIdL(EFriendRejected, &buf) ;
delete factory;
factory = NULL;
TInt comm_sent = EFriendRejected;
TInt comm_recev = commd->CommandId();
vimpstcmdobserver_stub ob;
commd->AddObserver(ob);
commd->ExecuteLD();

STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}


STIF_TESTDEFINE(t_ExeCommand_ChangeOwnAvtar)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
TAvatarData data;

MVIMPSTCmd* commd = factory->CreateCommandForIdL(EChangeOwnAvtar,&data) ;
delete factory;
factory = NULL;
TInt comm_sent = EChangeOwnAvtar;
TInt comm_recev = commd->CommandId();
vimpstcmdobserver_stub ob;
commd->AddObserver(ob);
commd->ExecuteLD();

STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}


STIF_TESTDEFINE(t_ExeCommand_CloseConversation)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
_LIT(KContactId,"contact");
TBufC<10> buf(KContactId);
MVIMPSTCmd* commd = factory->CreateCommandForIdL(ECloseConversation,&buf) ;
delete factory;
factory = NULL;
TInt comm_sent = ECloseConversation;
TInt comm_recev = commd->CommandId();
vimpstcmdobserver_stub ob;
commd->AddObserver(ob);
commd->ExecuteLD();

STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}


STIF_TESTDEFINE(t_ExeCommand_AddToPbk)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);

vimpststoragecontact_stub store;

MVIMPSTCmd* commd = factory->CreateCommandForIdL(EAddToPbk,&store) ;
delete factory;
factory = NULL;
TInt comm_sent = EAddToPbk;
TInt comm_recev = commd->CommandId();
vimpstcmdobserver_stub ob;
commd->AddObserver(ob);
commd->ExecuteLD();

STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}


STIF_TESTDEFINE(t_ExeCommand_CancelLogin)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
TInt data  = 1;
MVIMPSTCmd* commd = factory->CreateCommandForIdL(ECancelLogin,&data) ;
delete factory;
factory = NULL;
TInt comm_sent = ECancelLogin;
TInt comm_recev = commd->CommandId();
vimpstcmdobserver_stub ob;
commd->AddObserver(ob);
commd->ExecuteLD();

STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}

//Test case for Block/unblock...

STIF_TESTDEFINE(t_CreateCommand_BlockContact)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
_LIT(KContactId,"contact");
TBufC<10> buf (KContactId);
MVIMPSTCmd* commd = factory->CreateCommandForIdL(ECmdBlockContact, &buf) ;
delete factory;
factory = NULL;
TInt comm_sent = ECmdBlockContact;
TInt comm_recev = commd->CommandId();

STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}

STIF_TESTDEFINE(t_CreateCommand_UnBlockContact)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
_LIT(KContactId,"contact");
TBufC<10> buf (KContactId);
MVIMPSTCmd* commd = factory->CreateCommandForIdL(ECmdUnBlockContact, &buf) ;
delete factory;
factory = NULL;
TInt comm_sent = ECmdUnBlockContact;
TInt comm_recev = commd->CommandId();

STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}

STIF_TESTDEFINE(t_ExecuteCommand_BlockContact)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
_LIT(KContactId,"contact");
TBufC<10> buf (KContactId);
MVIMPSTCmd* commd = factory->CreateCommandForIdL(ECmdBlockContact, &buf) ;
delete factory;
factory = NULL;
vimpstcmdobserver_stub observer;
commd->AddObserver(observer);
TInt comm_sent = ECmdBlockContact;
TInt comm_recev = commd->CommandId();
TRAPD(err,commd->ExecuteLD());

STIF_ASSERT_EQUALS(err, KErrNone );
}

STIF_TESTDEFINE(t_ExecuteCommand_UnBlockContact)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
_LIT(KContactId,"contact");
TBufC<10> buf (KContactId);
MVIMPSTCmd* commd = factory->CreateCommandForIdL(ECmdUnBlockContact, &buf) ;
delete factory;
factory = NULL;
vimpstcmdobserver_stub observer;
commd->AddObserver(observer);
TInt comm_sent = ECmdUnBlockContact;
TInt comm_recev = commd->CommandId();
TRAPD(err,commd->ExecuteLD());

STIF_ASSERT_EQUALS(err, KErrNone);
}
STIF_TESTDEFINE(t_ExecuteCommand_FetchBlockContactList)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
_LIT(KContactId,"contact");
TBufC<10> buf (KContactId);
MVIMPSTCmd* commd = factory->CreateCommandForIdL(EFetchBlockedList) ;
delete factory;
factory = NULL;
vimpstcmdobserver_stub observer;
commd->AddObserver(observer);
TInt comm_sent = EFetchBlockedList;
TInt comm_recev = commd->CommandId();
TRAPD(err,commd->ExecuteLD());

STIF_ASSERT_EQUALS(err, KErrNone);
}
STIF_TESTDEFINE(t_ExecuteCommand_ProcessAcceptedContactNew)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
TVPbkSelectedData iSelectedData;

_LIT8(KPackedlink,"packedlink");
TPtrC8 link = KPackedlink();
iSelectedData.iPackedLinks = link.AllocL();
MVIMPSTCmd* commd = factory->CreateCommandForIdL(ECmdProcessAcceptedContactNew,&iSelectedData) ;
//delete factory;
//factory = NULL;
vimpstcmdobserver_stub observer;
commd->AddObserver(observer);
TInt comm_sent = ECmdProcessAcceptedContactNew;
TInt comm_recev = commd->CommandId();
commd->ExecuteLD();
STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}

STIF_TESTDEFINE(t_ExecuteCommand_ProcessSelectedContactNew)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
TVPbkSelectedData iSelectedData;

_LIT8(KPackedlink,"packedlink");
TPtrC8 link = KPackedlink();
iSelectedData.iPackedLinks = link.AllocL();
MVIMPSTCmd* commd = factory->CreateCommandForIdL(ECmdProcessSelectedContactNew,&iSelectedData) ;
//delete factory;
//factory = NULL;
vimpstcmdobserver_stub observer;
commd->AddObserver(observer);
TInt comm_sent = ECmdProcessSelectedContactNew;
TInt comm_recev = commd->CommandId();
commd->ExecuteLD();
STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}

STIF_TESTDEFINE(t_ExecuteCommand_ProcessAcceptedContactExist)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
TVPbkSelectedData iSelectedData;

_LIT8(KPackedlink,"packedlink");
TPtrC8 link = KPackedlink();
iSelectedData.iPackedLinks = link.AllocL();
MVIMPSTCmd* commd = factory->CreateCommandForIdL(ECmdProcessAcceptedContactExist,&iSelectedData) ;
//delete factory;
//factory = NULL;
vimpstcmdobserver_stub observer;
commd->AddObserver(observer);
TInt comm_sent = ECmdProcessAcceptedContactExist;
TInt comm_recev = commd->CommandId();
commd->ExecuteLD();
STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}

STIF_TESTDEFINE(t_ExecuteCommand_ProcessSelectedContactExist)
{
vimpstengine_stub engin;
CVIMPSTCmdFactory *factory = CVIMPSTCmdFactory::NewL(engin);
TVPbkSelectedData iSelectedData;

_LIT8(KPackedlink,"packedlink");
TPtrC8 link = KPackedlink();
iSelectedData.iPackedLinks = link.AllocL();
MVIMPSTCmd* commd = factory->CreateCommandForIdL(ECmdProcessSelectedContactExist,&iSelectedData) ;
//delete factory;
//factory = NULL;
vimpstcmdobserver_stub observer;
commd->AddObserver(observer);
TInt comm_sent = ECmdProcessSelectedContactExist;
TInt comm_recev = commd->CommandId();
commd->ExecuteLD();
STIF_ASSERT_EQUALS(comm_sent, comm_recev );
}

#endif
/**
 * END OF TEST CASES SECTION
 */
