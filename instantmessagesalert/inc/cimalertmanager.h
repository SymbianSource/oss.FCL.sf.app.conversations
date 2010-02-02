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
* Description:  This class gives interface for playing IM message alert
*
*/

#ifndef CIMALERTMANAGER_H
#define CIMALERTMANAGER_H

//  INCLUDES
#include "mimalertnotifierkeyobserver.h"
#include <apgcli.h>
#include <mdaaudiosampleplayer.h>
#include <mdaaudiotoneplayer.h>
#include <bldvariant.hrh>

#include <centralrepository.h>
#include <cenrepnotifyhandler.h>
#include <barsc.h>
#include <Profile.hrh>
#include <e32base.h>

#include "mimalertinterface.h"

// FORWARD DECLARATIONS
class CFLDRingingTonePlayer;
class MProfileEngine;
class MIMAlertManagerObserver;
class MIMAlertNotifierInterface;
class MProfile;
class CHWRMVibra;
class MVIMPSTSettingsStore;
class Ctestinstantmsgalert; //For Test code

#include "mimalertinterface.h"

const TInt KIMVibraDuration = 1000; // in milliseconds

/**
 *  This class gives interface to play
 *  simple sounds
 *
 *  @lib chat.app
 *  @since 1.2
 */
class CIMAlertManager
    : public CBase,
      public MMdaAudioPlayerCallback,
      public MIMAlertNotifierKeyObserver,
      public MMdaAudioToneObserver,
      public MCenRepNotifyHandlerCallback,
      public MIMAlertInterface
    {
    protected:  // Enumerations

        enum TAudioPlayerStatus
            {
            EAudioPlayerNotCreated,
            EAudioPlayerInitializing,
            EAudioPlayerReady,
            EAudioPlayerPlaying,
			EAudioPlayerInitialized
            };

    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         * param aServiceId, service id to get the tone to be played.
         */
        IMPORT_C static CIMAlertManager* NewL();

        /**
         * Destructor.
         */
        virtual ~CIMAlertManager();

    public: // New functions

        /**
         * Starts playing the tone that has been set
         * @since 1.2
         */
        IMPORT_C void PlayL(TUint32 aServiceId);

        /**
         * Stops playing
		 * @since 1.2
         */
        IMPORT_C void Stop();

        /**
        * Set the volume level on which the sound is played
        * If this method is not called, player uses the setting 
        * in active profile.
        * @param aVolume The volume level
        */
        IMPORT_C void SetVolume( TInt aVolume );

        /**
        * Sets the ringing type
        * If this method is not called, player uses the setting 
        * in active profile.
        * @param aRingingType The ringing type (see enum TProfileRingingType)
        */
        IMPORT_C void SetRingingType( TProfileRingingType aRingingType );

        /**
        * Sets vibrating alert on or off.
        * If this method is not called, player uses the 
        * setting in active profile.
        * @param aVibra True: Vibra is on. False: Vibra is off.
        */
        IMPORT_C void SetVibra( TBool aVibra );

        /**
         * Adds observer, which is informed when playing is completed
         * @param aObserver observing object
         */
        IMPORT_C void AddObserverL( const MIMAlertManagerObserver* aObserver );

        /**
         * Removes observer
         * @param aObserver observing object
         */
        IMPORT_C void RemoveObserver( const MIMAlertManagerObserver* aObserver );

    private: // constructors

        /**
         * C++ default constructor.
         * @see CIMAlertManager::NewL
         */
        CIMAlertManager();

        /**
         * By default Symbian 2nd phase constructor is private.
         */
        void ConstructL();

    private:
        /**
         * Does the actual sound playing
         * @since 2.1
         */
        void DoPlayL(TUint32 aServiceId);

        /**
         * Starts playing the sound file
         * @param aFileName The file to be processed
         */
        void ProcessFileL( const TDesC& aFileName );

        /** When audioplayer is ready, set its ringing type
         * @param aRingingType The ringing type (see enum TRingingTypes)
         */
        void DoSetRingingType( TInt aRingingType );

        /**
        * Converts volume from 1 to 10 to audio driver understandable value.
        * @param aVolume Volume level to be converted.
        * @return Returns converted volume level
        */
        TInt ConvertVolume( TInt aVolume );

        /**
         * Passess error to current activescheluder if
         * error = KErrNoMemory or KErrDiskFull
         */
        void HandleError( TInt aError );
        
        /**
         * Gets tone file name from SAP settings
         * @param aToneFileName After the call this will 
         *                       contain the tone file name         
         */
        void GetToneFileNameL( TDes& aToneFileName ,TUint32 aServiceId);
        
    private:    // Methods derived from MMdaAudioPlayerCallback

        /**
        * Derived from MMdaAudioPlayerCallback
        * This method is called when the audio player initialisation is ready
        * @see MMdaAudioPlayerCallback
        */
        void MapcInitComplete( TInt aError, 
                               const TTimeIntervalMicroSeconds& aDuration );

        /**
        * Derived from MMdaAudioPlayerCallback
        * This method is called when the audio player has finished playing
        * @see MMdaAudioPlayerCallback
        */
        void MapcPlayComplete( TInt aError );

    private:    // Methods derived from MMdaAudioToneObserver

        /**
         * Derived from MMdaAudioToneObserver
         * This method is called when the tone player initialisation is ready
         * @see MMdaAudioToneObserver
         */
	    virtual void MatoPrepareComplete( TInt aError );

	    /**
        * Derived from MMdaAudioToneObserver
        * This method is called when the tone player has finished playing
        * @see MMdaAudioToneObserver
        */
        virtual void MatoPlayComplete( TInt aError );

    private:    // Methods derived from MIMAlertNotifierKeyObserver

        /**
        * Derived from MIMAlertNotifierKeyObserver
        * Receive a notification from publish and subscribe keys
        */
        void HandleKeyChange( TUpdatedKey& aUpdatedValues );
        
    private:    // Methods derived from MCenRepNotifyHandlerCallback
        /**
        * Derived from MCenRepNotifyHandlerCallback
        * Receive a notification from central repository server
        */
        void HandleNotifyInt(TUint32 aId, TInt aNewValue);

    private:     // Data


        // Doesn't own. Profile API  if silent then there is no sounds
        MProfileEngine* iProfileApi;

        // Used to play other audio files than ringing tones. Owned.
        CMdaAudioPlayerUtility* iAudioPlayer;
        TAudioPlayerStatus iAudioPlayerStatus;

        // Used to play ringing tones. Owned
        CMdaAudioToneUtility* iTonePlayer;
        TAudioPlayerStatus iTonePlayerStatus;

        // Used for follow if user stops audio playing,
        // also for profile changes in volume level and vibrating alert
        CCenRepNotifyHandler* iRingingVolumeNotifyHandler;
        CCenRepNotifyHandler* iRingingTypeNotifyHandler;
        CCenRepNotifyHandler* iVibraNotifyHandler;

        CRepository* iProfilesRepository;
        CRepository* iVibraRepository;

        // Used to follow tone quit key
        MIMAlertNotifierInterface* iNotifierAPI;

        // ringing settings
        TInt iRingingVolume;
        TInt iRingingType;
        TBool iVibra;

        // Used when checking audio file type
        RApaLsSession iApaSession;

        // Audiomanager observers
        RPointerArray<MIMAlertManagerObserver> iObservers;

        // Used to keep track if current sound is default sound.
        TBool iIsPlayingDefaultIMSound;
 
        // Tone file name
        HBufC* iToneFileName;
        
        // File session for file operations
        RFs iFs;

        // currently active profile (ownership transferred)
        MProfile* iActiveProfile;             

        // owned, vibra API
        CHWRMVibra* iVibrator;

        // ETrue if Always Online is active, invoked from IM Launcher
        TBool iIMLauncherInvoked;
        
        // Beep once ringing tone
        HBufC* iBeepRingingTone;
        
        // owns. Access to central repository
        CRepository* iCenRep;
        
        // ETrue if IMPSCUI Session Id should be preserved upon exit
        TBool iPreserveSessionIdInExit;
        
        //service id
        TUint32 iServiceId;
        
        MVIMPSTSettingsStore* iSettingsStore;
        //for test cases
        friend class Ctestinstantmsgalert;
        };

#endif      // CIMALERTMANAGER_H

// End of File
