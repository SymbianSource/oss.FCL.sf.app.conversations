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
* Description:  This class gives interface to play IM message alert.
*
*/

// INCLUDE FILES
#include    "cimalertmanager.h"
#include    "imalertdebugprint.h"      
#include    "mimalertmanagerobserver.h"
#include    "mimalertnotifierinterface.h"
#include    "imalertnotifierfactory.h"

#include    <MProfileEngine.h>
#include	<pathinfo.h>
#include    <mda/common/resource.h>
#include    <AudioPreference.h>
#include    <bautils.h>
#include    <hwrmvibrasdkcrkeys.h>
#include    <coreapplicationuisdomainpskeys.h>
#include    <ProfileEngineSDKCRKeys.h>
#include    <MProfileTones.h>
#include    <TProfileToneSettings.h>
#include    <MProfile.h>
#include    <hwrmvibra.h>

//to get message tone file
#include <cvimpstsettingsstore.h>


// CONSTANTS
const TInt KMinVolumeLevel( 1 );
const TInt KMaxVolumeLevel( 10 );
const TInt KToneInterval( 1000000 ); // 1 second pause between tones

_LIT( KChatRngMimeType, "application/vnd.nokia.ringing-tone" );
_LIT( KBeepOnceSound, "beep.rng" );
_LIT( KProfileSilentTone, "No_Sound.wav" ); 

    
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CIMAlertManager::CIMAlertManager
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CIMAlertManager::CIMAlertManager()
    :   iAudioPlayerStatus( EAudioPlayerNotCreated ),
		iTonePlayerStatus( EAudioPlayerNotCreated ),
        iIsPlayingDefaultIMSound( EFalse )
    {
    }

// ---------------------------------------------------------
// CIMAlertManager::ConstructL
// (other items were commented in a header).
// ---------------------------------------------------------
//

void CIMAlertManager::ConstructL()
    {
    CHAT_DP_FUNC_ENTER("ConstructL");    
    iProfileApi = CreateProfileEngineL();
        
    User::LeaveIfError( iFs.Connect() );

    CHAT_DP_TXT("CMdaAudioPlayerUtility::NewL()" );

	iAudioPlayer = CMdaAudioPlayerUtility::NewL( *this );
	iAudioPlayerStatus = EAudioPlayerReady;

    CHAT_DP_TXT("CMdaAudioToneUtility::NewL()" );
	iTonePlayer = CMdaAudioToneUtility::NewL( *this );
	iTonePlayerStatus = EAudioPlayerReady;
 
    iProfilesRepository = CRepository::NewL( KCRUidProfileEngine );
    iVibraRepository = CRepository::NewL( KCRUidVibraCtrl );
 
    // Get initial settings from active profile
    iRingingVolumeNotifyHandler = CCenRepNotifyHandler::NewL( *this, 
                                              *iProfilesRepository,
                                              CCenRepNotifyHandler::EIntKey,
                                              KProEngActiveRingingVolume );
    iRingingVolumeNotifyHandler->StartListeningL();
    
    
    iRingingTypeNotifyHandler = CCenRepNotifyHandler::NewL( *this, 
                                              *iProfilesRepository,
                                              CCenRepNotifyHandler::EIntKey,
                                              KProEngActiveRingingType );
    iRingingTypeNotifyHandler->StartListeningL();
    
    
    iVibraNotifyHandler = CCenRepNotifyHandler::NewL( *this, *iVibraRepository,
                                              CCenRepNotifyHandler::EIntKey,
                                              KVibraCtrlProfileVibraEnabled );
    iVibraNotifyHandler->StartListeningL();
    
    User::LeaveIfError( iProfilesRepository->Get( KProEngActiveRingingVolume, 
                                                  iRingingVolume ) );
                                                 
    User::LeaveIfError( iVibraRepository->Get( KVibraCtrlProfileVibraEnabled, 
                                               iVibra ) );
    
    User::LeaveIfError( iApaSession.Connect() );

    // register for changes in message Tone quitting
    iNotifierAPI = IMAlertNotifierFactory::CreateNotifierL( this );
    iNotifierAPI->ListenKeyChanges( MIMAlertNotifierInterface::EMSGToneQuitKey );

    iToneFileName = HBufC::NewL( KMaxPath );

    // Get path to beep once ringing tone, first check simple sound folder.
    TFileName beepFile = PathInfo::RomRootPath();
    beepFile.Append( PathInfo::SimpleSoundsPath() );
    beepFile.Append( KBeepOnceSound );
    RFile file;
    TInt openErr = file.Open( iFs, beepFile, EFileRead );
    file.Close();
    
    if ( openErr == KErrNotFound || openErr == KErrPathNotFound )
        {
        // Check digital sounds folder
        beepFile.Zero();
        beepFile.Append( PathInfo::RomRootPath() );
        beepFile.Append( PathInfo::DigitalSoundsPath() );
        beepFile.Append( KBeepOnceSound );
        openErr = file.Open( iFs, beepFile, EFileRead );
        file.Close();
        }    
    
    CHAT_DP( D_PLAIN_LIT( "CIMAlertManager::ConstructL() openErr = %d" ), openErr );
    
    if ( openErr == KErrNone )
        {
        iBeepRingingTone = beepFile.AllocL();
        TPtrC ptr = iBeepRingingTone->Des();
        CHAT_DP( D_PLAIN_LIT( "CIMAlertManager::ConstructL() iBeepRingingTone = %S" ), &ptr );
        }
    else
        {
        HandleError( openErr );
        }

    // get active ringing type, change events come to us later...
    iActiveProfile = iProfileApi->ActiveProfileL();
    const MProfileTones& profileTones = iActiveProfile->ProfileTones();
    const TProfileToneSettings& profileSettings = profileTones.ToneSettings();
    iRingingType = profileSettings.iRingingType;
    iActiveProfile->Release();
    iActiveProfile = NULL; // prevent further usage
    // after this, active ringing type changes are notified via the cenrep
    // notify handler callback through HandleNotifyInt
    
    // TRAP to catch leaves, leaves with KErrNotSupported if vibra
    // is not supported
    TRAPD( err, iVibrator = CHWRMVibra::NewL() );
    if ( err == KErrNotSupported )
        {
        // Make sure it's NULL
        iVibrator = NULL;
        }
    else
        {
        // Leave with other errors e.g. KErrNoMemory
        User::LeaveIfError( err );
        }
    iSettingsStore = CVIMPSTSettingsStore::NewL();
    CHAT_DP_FUNC_DONE("ConstructL");
    }

// ---------------------------------------------------------
// CIMAlertManager::NewL
// (other items were commented in a header).
// ---------------------------------------------------------
EXPORT_C CIMAlertManager* CIMAlertManager::NewL()
    {
    CIMAlertManager* self = new( ELeave ) CIMAlertManager();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


// ---------------------------------------------------------
// CIMAlertManager::~CIMAlertManager
// (other items were commented in a header).
// ---------------------------------------------------------
CIMAlertManager::~CIMAlertManager()
    {
    CHAT_DP_FUNC_ENTER("~CIMAlertManager");
    Stop(); // Stops possible playback

    if ( iActiveProfile )
        {
        iActiveProfile->Release();
        iActiveProfile = NULL; // prevent further usage
        }

    if ( iProfileApi ) 
        {
        iProfileApi->Release();
        iProfileApi = NULL;
        }

    if ( iVibraNotifyHandler )
        {
        iVibraNotifyHandler->StopListening();
        delete iVibraNotifyHandler;
        iVibraNotifyHandler = NULL;
        }
    
    if ( iRingingTypeNotifyHandler )
        {
        iRingingTypeNotifyHandler->StopListening();
        delete iRingingTypeNotifyHandler;
        iRingingTypeNotifyHandler = NULL;
        }
    

    if ( iRingingVolumeNotifyHandler )
        {
        iRingingVolumeNotifyHandler->StopListening();
        delete iRingingVolumeNotifyHandler;
        iRingingVolumeNotifyHandler = NULL;
        }
    if(iProfilesRepository)
        {
        delete iProfilesRepository;
        iProfilesRepository = NULL;
        }
    if(iVibraRepository)
        {
        delete iVibraRepository;
        iVibraRepository = NULL;
        }
    
    iApaSession.Close();
    
    if(iNotifierAPI)
        {
        delete iNotifierAPI;
        iNotifierAPI = NULL;
        }
    
    if(iAudioPlayer)
        {
        CHAT_DP_TXT( "delete CMdaAudioPlayerUtility" );
        iAudioPlayer->Close();
        delete iAudioPlayer;
        iAudioPlayer = NULL;
        }
    if(iTonePlayer)
        {
        CHAT_DP_TXT( "delete CMdaAudioToneUtility" );
        delete iTonePlayer;
        iTonePlayer = NULL;
        }
    
    iObservers.Close();

    if(iToneFileName)
        {
        delete iToneFileName;
        iToneFileName = NULL;
        }

    if(iVibrator)
        {
        delete iVibrator;
        iVibrator = NULL;
        }

    if(iBeepRingingTone)
        {
        delete iBeepRingingTone;
        iBeepRingingTone = NULL;
        }

    iFs.Close();
    
    if(iSettingsStore)
        {
        delete iSettingsStore;
        iSettingsStore = NULL;
        }
    
    CHAT_DP_FUNC_DONE("~CIMAlertManager");
    }


// ---------------------------------------------------------
// CIMAlertManager::Play
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CIMAlertManager::PlayL(TUint32 aServiceId)
    {
    CHAT_DP_FUNC_ENTER("Play");
    TInt emergencyCallOngoing(0);
    TInt err = RProperty::Get( KPSUidCtsyEmergencyCallInfo, KCTSYEmergencyCallInfo, emergencyCallOngoing );

    if( !emergencyCallOngoing )
        {
        Stop();
        TRAPD( err, DoPlayL(aServiceId) );
        if( err )
            {
            HandleError( err );
            }
        }
    CHAT_DP_FUNC_DONE("Play");
    }

// ---------------------------------------------------------
// CIMAlertManager::DoPlayL
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CIMAlertManager::DoPlayL(TUint32 aServiceId)
    {
    CHAT_DP_FUNC_ENTER("DoPlayL");

    TBuf<KMaxPath> toneFileName;

    // check if we're silent or not
    iActiveProfile = iProfileApi->ActiveProfileL();
    TBool isSilent = iActiveProfile->IsSilent();
    iActiveProfile->Release();
    iActiveProfile = NULL; // prevent further usage

    // vibrate, if it's allowed
    CHWRMVibra::TVibraModeState vibraModeState = iVibrator->VibraSettings();
    CHWRMVibra::TVibraStatus vibraStatus = iVibrator->VibraStatus();
   
    // TODO: Find from Nikhil what the behaviour should be if Vibra is already
    // on and you get a new message. Aare you supposed to truncate the existing
    // vibration and start afresh or let the existing vibration continue?
    if ( vibraModeState == CHWRMVibra::EVibraModeON && 
         vibraStatus == CHWRMVibra::EVibraStatusStopped )
        {
        iVibrator->StartVibraL( KIMVibraDuration );
        }

    // sound ordering:
    //  silent: get out.
    //  beep once: beep, and if ok, exit.
    //  if beep failed, or not active: try to play sound
    //  for this server.
    //  if it failed, play default sound.

    if ( isSilent ) 
        {
        // Silent profile, get out.
        CHAT_DP_TXT( "DoPlayL - Returning" );
        return;
        }

    TInt err = KErrNone;

    if ( iRingingType == EProfileRingingTypeBeepOnce && iBeepRingingTone )
        {
        // Play beep.rng
        TRAP( err, ProcessFileL( *iBeepRingingTone ) );
        }
    
    if ( err != KErrNone || iRingingType != EProfileRingingTypeBeepOnce )
        {
        err = KErrNone;

        // Beep once not active, or beep once failed.
        // Try to play the sound set for this server
        TBuf<KMaxPath> toneFileName;
		GetToneFileNameL( toneFileName, aServiceId );
		TParsePtr parsedPtr( toneFileName );
		
		CHAT_DP( D_CHAT_LIT(" file = [%S] "),&toneFileName);

		// If alert tone OFF, don't play anything
        if( parsedPtr.NameAndExt().Compare( KProfileSilentTone ) != 0 )
            {
            // Not OFF
            TRAP( err, ProcessFileL( toneFileName ) );        
            }
        }

    // Not ringing once, but left
    User::LeaveIfError( err );

    CHAT_DP_FUNC_DONE("DoPlayL");
    }

// ---------------------------------------------------------
// CIMAlertManager::Stop
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CIMAlertManager::Stop()
    {
    CHAT_DP_FUNC_ENTER("Stop");

    if( iAudioPlayerStatus == EAudioPlayerPlaying )
        {
        CHAT_DP_TXT( 
            "Audio player is playing. CMdaAudioPlayerUtility::Stop()" );
        iAudioPlayer->Stop();
        iAudioPlayer->Close();
        iAudioPlayerStatus = EAudioPlayerReady;
        }
    
    if( iTonePlayerStatus == EAudioPlayerPlaying )
        {
        CHAT_DP_TXT( 
            "Tone player is playing. CMdaAudioToneUtility::CancelPlay()" );
        iTonePlayer->CancelPlay();
        iTonePlayerStatus = EAudioPlayerReady;
        }    
   
    CHAT_DP_FUNC_DONE("Stop");
    }

// ---------------------------------------------------------
// CIMAlertManager::SetVolume
// Set the volume at which the sound is played
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CIMAlertManager::SetVolume( TInt aVolume )
    {
    iRingingVolumeNotifyHandler->StopListening();
    iRingingVolume = aVolume;
    }

// ---------------------------------------------------------
// CIMAlertManager::SetRingingType
// Sets the ringing type how the sound is played
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CIMAlertManager::SetRingingType( TProfileRingingType aRingingType )
    {
    if(iRingingTypeNotifyHandler)
        {
        iRingingTypeNotifyHandler->StopListening();
        iRingingType = aRingingType;
        }
    }

// ---------------------------------------------------------
// CIMAlertManager::SetVibra
// Sets the vibrating alert on or off
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CIMAlertManager::SetVibra( TBool aVibra )
    {
    if(iVibraNotifyHandler)
        {
        iVibraNotifyHandler->StopListening();
        iVibra = aVibra;
        }
    }

// ---------------------------------------------------------
// CIMAlertManager::AddObserverL
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CIMAlertManager::AddObserverL( const MIMAlertManagerObserver* aObserver )
	{
    // make sure that aObserver does not get appended in release mode also.
	if( aObserver )  
		{
   		User::LeaveIfError( iObservers.Append( aObserver ) );
		}
	}

// ---------------------------------------------------------
// CIMAlertManager::RemoveObserver
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CIMAlertManager::RemoveObserver( const MIMAlertManagerObserver* aObserver )
	{
	TInt index( iObservers.Find( aObserver ) );

	if( index != KErrNotFound )
		{
		iObservers.Remove( index );
		}
	}

// ---------------------------------------------------------
// CIMAlertManager::ProcessFileL
// If filename is defined, prepare the audioplayer.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMAlertManager::ProcessFileL( const TDesC& aFileName )
    {
    CHAT_DP_FUNC_ENTER("ProcessFileL");

    TFileName *fileName = new (ELeave) TFileName();
    CleanupStack::PushL( fileName );

    // if filename is empty, use default sound
    if ( aFileName.Length() == 0 )
        {
        return;
        }
    else
        {
        *fileName = aFileName;
        }

    TPtr filePtr( NULL, 0 );
    if( !BaflUtils::FileExists( iFs, *fileName ) )
        {
        CHAT_DP( D_CHAT_LIT( "File does not exist ( %S )" ), &(*fileName) );
        User::Leave( KErrNotFound );
        }

    Stop(); // Stops possible playback 

    TUid dummyUid( KNullUid );
	TDataType dataType;
	
    User::LeaveIfError( iApaSession.AppForDocument( *fileName, dummyUid, 
                                                    dataType ) );

    if( dataType.Des().CompareF( KChatRngMimeType ) == 0 ) 
        {
        // making sure that another initializing is not already ongoing
        if( iTonePlayerStatus == EAudioPlayerReady )
            {
            CHAT_DP( D_CHAT_LIT( 
                "CMdaAudioToneUtility::PrepareToPlayFileSequence( %S )" ), 
                &(*fileName));
            iTonePlayerStatus = EAudioPlayerInitializing;
            iTonePlayer->PrepareToPlayFileSequence( *fileName );
            }
        }
    else
        {
        // making sure that another initializing is not already ongoing
        if( iAudioPlayerStatus == EAudioPlayerReady )
            {
            CHAT_DP( D_CHAT_LIT(
                "FileList - CMdaAudioPlayerUtility::OpenFileL( %S )" ), 
                &(*fileName));
            iAudioPlayerStatus = EAudioPlayerInitializing;
            TRAPD( err, iAudioPlayer->OpenFileL( *fileName ) );
            CHAT_DP( D_CHAT_LIT(
                "FileList - CMdaAudioPlayerUtility::OpenFileL err = %d" ), 
                err );
            if( err )
                {              
                iAudioPlayerStatus = EAudioPlayerReady;
                User::Leave( err );
                }
            }
        }    

    CleanupStack::PopAndDestroy( fileName );
    CHAT_DP_FUNC_DONE("ProcessFileL");
    }

// ---------------------------------------------------------
// CIMAlertManager::DoSetRingingType
// When audioplayer is ready, set its ringing type
// (other items were commented in a header).
// ---------------------------------------------------------
//

void CIMAlertManager::DoSetRingingType( TInt aRingingType )
    {
    if( iAudioPlayerStatus == EAudioPlayerInitialized )
        {
        switch( aRingingType )
            {
			// Fall through
            case EProfileRingingTypeRingingOnce:
                {
                iAudioPlayer->SetRepeats( 0, 
                        TTimeIntervalMicroSeconds( KToneInterval ) );
                break;
                }
            case EProfileRingingTypeBeepOnce:
                {
                iAudioPlayer->SetRepeats( 0, 
                        TTimeIntervalMicroSeconds( KToneInterval ) );
                break;
                }
            default:
                {
                break;
                }
            }
        }
    else if( iTonePlayerStatus == EAudioPlayerInitialized )
        {
        switch( aRingingType )
            {
			// Fall through
            case EProfileRingingTypeRingingOnce:
                {
                iTonePlayer->SetRepeats( 0, 
                        TTimeIntervalMicroSeconds( KToneInterval ) );
                break;
                }
            case EProfileRingingTypeBeepOnce:
                {
                iTonePlayer->SetRepeats( 0, 
                        TTimeIntervalMicroSeconds( KToneInterval ) );
                break;
                }
            default:
                {
                break;
                }
            }
        }
    }

// ---------------------------------------------------------
// CIMAlertManager::ConvertVolume
// Convert volume to the scale used by hardware.
// E.g. Profiles lets user select volume 1-10, but hw's scale is normally 1-9.
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CIMAlertManager::ConvertVolume( TInt aVolume )
    {
    TInt result( 0 );
    // if ringing type is "silent" then volume
    // is zero
    if( iRingingType != EProfileRingingTypeSilent )
        {
        if ( iAudioPlayerStatus == EAudioPlayerInitialized ||
            iAudioPlayerStatus == EAudioPlayerPlaying )
            {
            result = iAudioPlayer->MaxVolume() * aVolume / KMaxVolumeLevel;
            if ( aVolume == KMinVolumeLevel && result == 0 )
                {
                result = 1;
                }
            }
        else if( iTonePlayerStatus == EAudioPlayerInitialized ||
            iTonePlayerStatus == EAudioPlayerPlaying )
            {
            result = iTonePlayer->MaxVolume() * aVolume / KMaxVolumeLevel;
            if ( aVolume == KMinVolumeLevel && result == 0 )
                {
                result = 1;
                }
            }
        }
    return result;
    }

// ---------------------------------------------------------
// CIMAlertManager::HandleError
// Notifies all observers that playing is completed
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMAlertManager::HandleError( TInt aError )
    {
    if ( ( aError == KErrNoMemory ) || 
         ( aError == KErrDiskFull ) ) 
		{
		CActiveScheduler::Current()->Error( aError );
		}
    }

// ---------------------------------------------------------
// CIMAlertManager::GetToneFileNameL
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMAlertManager::GetToneFileNameL( TDes& aToneFileName ,TUint32 aServiceId)
	{
    RBuf buffer;
    buffer.CreateL( RProperty::KMaxPropertySize ); 
    CleanupClosePushL( buffer );
    iSettingsStore->GetL(aServiceId, EServiceToneFileName, buffer); 
    aToneFileName.Copy( buffer );
    CleanupStack::PopAndDestroy();  //buffer
    }

// ---------------------------------------------------------
// CIMAlertManager::MapcInitComplete
// This method is called when the audioplayer initialisation is ready.
// If file format is supported, start playing the sound file.
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMAlertManager::MapcInitComplete(
    TInt aError, const TTimeIntervalMicroSeconds& /* aDuration */ )
    {
    CHAT_DP( D_CHAT_LIT(
        "MMdaAudioPlayerCallback::MapcInitComplete() callback, error == %d" ), 
        aError);

    if ( !aError && iAudioPlayerStatus == EAudioPlayerInitializing )
        {
        iAudioPlayerStatus = EAudioPlayerInitialized;
        // currently IM messages supports only this ringing type
        // ringing once
        DoSetRingingType( EProfileRingingTypeRingingOnce );
        CHAT_DP( D_CHAT_LIT("CMdaAudioPlayerUtility::SetVolume( %d )" ), 
                 iRingingVolume);
        
        iAudioPlayer->SetVolume( ConvertVolume( iRingingVolume ) );
        
        // TODO: please check the value KAudioPrefNewSpecialMessage.
        // WHy does it have this particular value? What is the rationale.
        //
        CHAT_DP( D_CHAT_LIT("CMdaAudioPlayerUtility::SetPriority( %d, %d )" ),
            KAudioPriorityRecvMsg, 
            TMdaPriorityPreference( KAudioPrefNewSpecialMessage ) );
            
        iAudioPlayer->SetPriority( KAudioPriorityRecvMsg,
            TMdaPriorityPreference( KAudioPrefNewSpecialMessage ) );
            
        CHAT_DP_TXT("CMdaAudioPlayerUtility::Play()" );
        iAudioPlayer->Play();
        iAudioPlayerStatus = EAudioPlayerPlaying;
        iNotifierAPI->SetInt( MIMAlertNotifierInterface::EMSGToneQuitKey, 
                              ECoreAppUIsTonePlaying );
        }
    else
        {
        Stop();
        iAudioPlayerStatus = EAudioPlayerReady;
        if( ( ( aError == KErrNotSupported ) || ( aError == KErrCorrupt ) ))
            {
            return;
            }
        else
            {
            HandleError( aError );
            iIsPlayingDefaultIMSound = EFalse;
            }
        }
    }

// ---------------------------------------------------------
// CIMAlertManager::MapcPlayComplete
// This method is called when the audioplayer has finished playing a sound
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMAlertManager::MapcPlayComplete( TInt aError )
    {
    CHAT_DP( D_CHAT_LIT(
        "MMdaAudioPlayerCallback::MapcPlayComplete() callback, error == %d" ), 
        aError);
    iNotifierAPI->SetInt( MIMAlertNotifierInterface::EMSGToneQuitKey, ECoreAppUIsTonePlayingUninitialized );
    iAudioPlayer->Close();
    iAudioPlayerStatus = EAudioPlayerReady;
    
    if( ( ( aError == KErrNotSupported ) || ( aError == KErrCorrupt ) ) )
        {
        return;
        }
    else
        {
        HandleError( aError );
        iIsPlayingDefaultIMSound = EFalse;
        }
    }

// ---------------------------------------------------------
// CIMAlertManager::HandleKeyChange
// This method receives notifications from the shared data server
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMAlertManager::HandleKeyChange( TUpdatedKey& aUpdatedValues )
    {
    CHAT_DP_FUNC_ENTER("HandleKeyChange");
    
    // If we have an event which changes the key value to 
    // ECoreAppUIsStopTonePlaying -> stop playing. 
    if( aUpdatedValues.iValue == ECoreAppUIsStopTonePlaying )
        {
        // Message tone should be stopped
        CHAT_DP_TXT( "**stop playing!**" );
        Stop();
        }
    
    CHAT_DP_FUNC_DONE("HandleKeyChange");
    }

// ---------------------------------------------------------
// CIMAlertManager::HandleNotifyInt
// (other items were commented in a header).
// ---------------------------------------------------------
void CIMAlertManager::HandleNotifyInt( TUint32 aId, TInt aNewValue )
	{
    CHAT_DP_FUNC_ENTER("HandleNotifyL");
    CHAT_DP( D_CHAT_LIT("aKey == %d" ), aId);
    CHAT_DP( D_CHAT_LIT("aValue == %d" ), aNewValue);
    

    if ( aId == KProEngActiveRingingVolume  )  // Ringing volume has changed
        {
        iRingingVolume = aNewValue;
        }
    else if ( aId == KProEngActiveRingingType )  // Ringing type has changed
        {
        iRingingType = aNewValue;
        }

    if ( aId == KVibraCtrlProfileVibraEnabled )  // Vibra setting has changed
        {
        iVibra = aNewValue;
        }
        
    CHAT_DP_FUNC_DONE("HandleNotifyL");
	}

// ---------------------------------------------------------
// CIMAlertManager::MatoPrepareComplete
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMAlertManager::MatoPrepareComplete( TInt aError )
    {
    CHAT_DP( D_CHAT_LIT(
        "MMdaAudioToneObserver::MatoPrepareComplete() callback, error == %d" ), 
        aError);
    
    if ( !aError && iTonePlayerStatus == EAudioPlayerInitializing )
        {
        iTonePlayerStatus = EAudioPlayerInitialized;
        // TODO: confirm with UI spec/Nikhil if only this ringing type is
        // supported. If not in writing, get an entry into the UI spec done.
        // TODO: where is this requirement specified?
        
        // currently IM messages supports only this ringing type
        // ringing once
        DoSetRingingType( EProfileRingingTypeRingingOnce );
        CHAT_DP( D_CHAT_LIT("CMdaAudioToneUtility::SetVolume( %d )" ), 
                ConvertVolume( iRingingVolume ));
        iTonePlayer->SetVolume( ConvertVolume( iRingingVolume ) );


        CHAT_DP( D_CHAT_LIT("CMdaAudioToneUtility::SetPriority( %d, %d )" ),
                KAudioPriorityRecvMsg, 
                TMdaPriorityPreference( KAudioPrefNewSpecialMessage ) );
                
        iTonePlayer->SetPriority( KAudioPriorityRecvMsg,
            TMdaPriorityPreference( KAudioPrefNewSpecialMessage ) );
            
        CHAT_DP_TXT("CMdaAudioToneUtility::Play()" );
        iTonePlayer->Play();
        iTonePlayerStatus = EAudioPlayerPlaying;
        iNotifierAPI->SetInt( MIMAlertNotifierInterface::EMSGToneQuitKey, 
                              ECoreAppUIsTonePlaying );
        }
    else
        {
        Stop();
        iTonePlayerStatus = EAudioPlayerReady;
        
        if( ( ( aError == KErrNotSupported ) || ( aError == KErrCorrupt ) ) )
            {
            return;
            }
        else
            {
            HandleError( aError );
            iIsPlayingDefaultIMSound = EFalse;
            }
        }
    }

// ---------------------------------------------------------
// CIMAlertManager::MatoPlayComplete
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CIMAlertManager::MatoPlayComplete( TInt aError )
    {
    CHAT_DP( D_CHAT_LIT(
        "MMdaAudioToneObserver::MatoPlayComplete() callback, error == %d" ), 
        aError);
    iNotifierAPI->SetInt( MIMAlertNotifierInterface::EMSGToneQuitKey, ECoreAppUIsTonePlayingUninitialized );
    iTonePlayerStatus = EAudioPlayerReady;
    
    if( ( ( aError == KErrNotSupported ) || ( aError == KErrCorrupt ) ) )
        {
        return ;
        }
    else
        {
        HandleError( aError );
        iIsPlayingDefaultIMSound = EFalse;
        }
    }
        
//  End of File  
