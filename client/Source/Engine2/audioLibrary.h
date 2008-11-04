#ifndef __AUDIO_LIBRARY_H__
#define __AUDIO_LIBRARY_H__

#include "pammo.h"
#include "view.h"

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

namespace pammo
{


#define AUDIO_BACKGROUND_INTRO        (uint32_t)0
#define AUDIO_BACKGROUND_INGAME       1
#define AUDIO_EXPLOSION               2
#define AUDIO_FLAMETHROWER            3
#define AUDIO_LIGHTNINGGUN            4


// An audio clip that can be played.
struct AudioClip
{
    char const* mPath;     // Path to audio file.
    bool        mCache;    // Should the clip be cached in memory.
    uint8_t*    mData;     // If cached in memory, the data.
    uint32_t    mDataLen;  // If cached in memory, the data length.
    uint32_t    mId;       // The id of the audio clip.
};

// Container for OpenAL buffer. Buffers contain audio to be played at a source.
struct AudioBuffer
{
    ALuint mBuffer;
    
    AudioBuffer* mNext;
};

#define PLAY_ONCE    1
#define PLAY_FOREVER 0xffffffff
// Represents an instance of an AudioClip in the scene.
struct AudioInstance
{
    AudioClip*     mClip;    // Name of the clip.
    Audio          mAudio;   // Platform specific representation of the audio.
    ALuint         mSource;  // OpenAL Source describing location of audio instance in scene.
    AudioBuffer*   mBuffersHead; // OpenAL Buffers holding audio.
    AudioBuffer*   mBuffersTail; // OpenAL Buffers holding audio.
    bool           mAutoRemove; // Should this instance be removed automatically when it is finished playing?    
    uint32_t       mPlaysRemain; // How many times more should this instance play?
    enum State
    {
        Stopped,
        Paused,
        ReadyToPlay,
        Playing
    };
    State          mState;


    ALenum         mFormat;
    uint32_t       mSampleRate;


    AudioInstance* mPrev;
    AudioInstance* mNext;

    AudioInstance* mDeleteNext;
};

// Contains all playable clips.
extern AudioClip gAudioClip[];

class AudioLibraryObserver
{
public:
    virtual ~AudioLibraryObserver(){}
    virtual void onAudioPreloadComplete() = 0;
};

class AudioLibrary : public View
{
public:
    AudioLibrary();
    virtual ~AudioLibrary();

    void setObserver(AudioLibraryObserver* o);
    virtual void update();
    virtual uint32_t getUpdatePriority() const;

    // Returns an instance of an audio clip.
    AudioInstance* getAudioInstance(uint32_t id);
    
    // Returns an instance of an audio clip. 
    // Will fail if the clip was tagged for in-memory 
    // caching and the clip is not yet loaded.
    AudioInstance* tryGetAudioInstance(uint32_t id);

    // Closes an AudioInstance. After calling this method,
    // the instance is no longer valid.
    void closeAudioInstance(AudioInstance* instance);

    // Begin playing the audio instance.
    // If autoClose is done, the instance will be freed when the stream completes.
    // The instance pointer could become invalid at anytime in this case, so fire and forget.
    void playAudioInstance(AudioInstance* instance, uint32_t playCount, bool autoClose);
   
    // Pause playback of the audio instance.
    void pauseAudioInstance(AudioInstance* instance);

    // Stops playback of the audio instance and returns 
    // the play cursor to the beginning of the clip.
    void stopAudioInstance(AudioInstance* instance);

    void enableAudio(bool enabled);
    bool getAudioEnabled();

    AudioBuffer* getAudioBuffer();
    void returnAudioBuffer(AudioBuffer* buffer);

private:
    uint32_t mAudioCount;

    //struct AudioLoadRequest
    //{
    //    uint32_t mId;
    //    AudioLoadRequest* mNext;
    //};

    //static const uint32_t kAudioLoadStackSize = 10;
    //AudioLoadRequest* mFree;
    //AudioLoadRequest* mToService;
    //AudioLoadRequest* mToNotify;

    void reclaimBuffers(AudioInstance* instance);

    // AudioBuffers.
    static const uint32_t kAudioBufferQueueSize = 128;
    AudioBuffer  mAudioBuffers[kAudioBufferQueueSize];
    AudioBuffer* mAudioBuffersFree;
    AudioBuffer* mAudioBuffersToReturn;

    // AudioIntances.
    static const uint32_t kAudioInstanceQueueSize = 32;
    AudioInstance  mAudioInstances[kAudioInstanceQueueSize];
    AudioInstance* mAudioInstancesFree;

    // Actice AudioInstances.
    AudioInstance* mAudioInstanceActiveHead;
    AudioInstance* mAudioInstanceActiveTail;

    AudioInstance* mAudioInstanceToDelete;

    void load(uint32_t id);

    pthread_t mThread;
    pthread_mutex_t mMutex;
    pthread_cond_t mCondition;

    static void* threadBootFunc(void* arg);
    void threadFunc();

    bool mRunning;

    AudioLibraryObserver* mObserver;
    bool mPreloadComplete;
    bool mCompleteNotified;

    bool mAudioEnabled;

    ALCdevice* mALDevice;
    ALCcontext* mALContext;
};

extern AudioLibrary* gAudioLibrary;

}


#endif