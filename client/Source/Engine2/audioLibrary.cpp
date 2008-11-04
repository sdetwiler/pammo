#include "pammo.h"
#include "audioLibrary.h"

namespace pammo
{

AudioClip gAudioClip[] =
{
//    {"audio/test.mp3", false, NULL, 0, 0},      // 0
    {"audio/intro.wav", false, NULL, 0, 0},      // 0
    {"audio/background.wav", false, NULL, 0, 0},       // 1
    {"audio/explosion00.wav", false, NULL, 0, 0},      // 2
    {"audio/flamethrower01.wav", false, NULL, 0, 0},      // 3
    {"audio/lightningGun.wav", false, NULL, 0, 0},      // 4
};

AudioLibrary* gAudioLibrary = NULL;

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

void alEnumToString(char const* str, int e)
{
    switch(e)
    {
        case AL_OUT_OF_MEMORY:
            dprintf("%s out of mem", str);
            break;
        case AL_INVALID_VALUE:
            dprintf("%s invalid value", str);
            break;
        case AL_INVALID_ENUM:
            dprintf("%s invalid enum %d", str, e);
            break;
        case AL_INVALID_OPERATION:
            dprintf("%s invalid operation", str);
            break;
        default:
            dprintf("%s unkown", str);
    }
}


AudioLibrary::AudioLibrary()
{
    int error;

    mAudioInstanceActiveHead = NULL;
    mAudioInstanceActiveTail = NULL;
    mAudioInstanceToDelete = NULL;

    mAudioEnabled = false;

    mPreloadComplete = true; // SCD For now disable preload.
    mCompleteNotified = false;
 //   return;
    
    mALDevice = alcOpenDevice(NULL);
	if(!mALDevice)
    {
        return;
    }
	
    mALContext = alcCreateContext(mALDevice, NULL);
    alcMakeContextCurrent(mALContext);
 	error = alGetError();
    if(error)
    {
        alEnumToString("makeContextCurrent", error);
        return;
    }

    mAudioEnabled = true;
    mObserver = 0;
    mAudioCount = sizeof(gAudioClip)/sizeof(AudioClip);

    //mPreloadIndex = 0;

    // Initialize AudioBuffer pointers.
    for(uint32_t i=0; i<kAudioBufferQueueSize-1; ++i)
    {
        alGenBuffers(1, &mAudioBuffers[i].mBuffer);
        mAudioBuffers[i].mNext = &mAudioBuffers[i+1];
    }
    mAudioBuffersFree = &mAudioBuffers[0];
    alGenBuffers(1, &mAudioBuffers[kAudioBufferQueueSize-1].mBuffer);
    mAudioBuffers[kAudioBufferQueueSize-1].mNext = NULL;

    // Initialize AudioInstance pointers.
    for(uint32_t i=0; i<kAudioInstanceQueueSize-1; ++i)
    {
        alGenSources(1, &mAudioInstances[i].mSource);

        // Set the distance attinuation values for the sources.
        alSourcef(mAudioInstances[i].mSource, AL_MAX_DISTANCE, 400.0f);
        alSourcef(mAudioInstances[i].mSource, AL_REFERENCE_DISTANCE, 300.0f);
        mAudioInstances[i].mNext = &mAudioInstances[i+1];
    }
    mAudioInstancesFree = &mAudioInstances[0];
    alGenSources(1, &mAudioInstances[kAudioInstanceQueueSize-1].mSource);
    mAudioInstances[kAudioInstanceQueueSize-1].mNext = NULL;



    //mRunning = true;
    //pthread_mutex_init(&mMutex, NULL);
    //pthread_cond_init(&mCondition, NULL);
    //spawnThread(threadBootFunc, this);
}

AudioLibrary::~AudioLibrary()
{
    // return ALBuffers.
    // return ALSources.

//    pthread_mutex_destroy(&mMutex);
//    pthread_cond_destroy(&mCondition);

    alcDestroyContext(mALContext);
    alcCloseDevice(mALDevice);
}

void AudioLibrary::enableAudio(bool enabled)
{
    mAudioEnabled = enabled;
}

bool AudioLibrary::getAudioEnabled()
{
    return mAudioEnabled;
}

void AudioLibrary::setObserver(AudioLibraryObserver* o)
{
    mObserver = o;
    mCompleteNotified = false;
}

uint32_t AudioLibrary::getUpdatePriority() const
{
    return kAudioLibraryPriority;
}

AudioInstance* AudioLibrary::tryGetAudioInstance(uint32_t id)
{
    if(gAudioClip[id].mCache)
    {
        if(gAudioClip[id].mData == 0)
            return NULL;
    }

    return getAudioInstance(id);
}

AudioBuffer* AudioLibrary::getAudioBuffer()
{
    if(!mAudioEnabled)
        return NULL;

    AudioBuffer* buffer = mAudioBuffersFree;
    if(!buffer)
        return NULL;

    mAudioBuffersFree = mAudioBuffersFree->mNext;

    return buffer;
}

void AudioLibrary::returnAudioBuffer(AudioBuffer* buffer)
{
    buffer->mNext = mAudioBuffersFree;
    mAudioBuffersFree = buffer;
}

AudioInstance* AudioLibrary::getAudioInstance(uint32_t id)
{
    if(!mAudioEnabled)
        return NULL;

    // Pop an instance off of the free stack.
    AudioInstance* instance = mAudioInstancesFree;
    if(!instance)
        return NULL;

    mAudioInstancesFree = mAudioInstancesFree->mNext;

    int ret;

    // Open an instance of the audio clip.
    instance->mClip = &gAudioClip[id];
    ret = openAudio_platform(instance); 
    if(ret < 0)
    {
        // Push the instance onto the free stack.
        instance->mNext = mAudioInstancesFree;
        instance->mPrev = NULL;
        mAudioInstancesFree = instance;
        return NULL;
    }

    instance->mState = AudioInstance::Stopped;

    if(!mAudioInstanceActiveHead)
        mAudioInstanceActiveHead = instance;

    if(mAudioInstanceActiveTail)
        mAudioInstanceActiveTail->mNext = instance;
    
    instance->mPrev = mAudioInstanceActiveTail;  
    mAudioInstanceActiveTail = instance;
    instance->mPlaysRemain = 1; // Only play once by default;
    instance->mAutoRemove = true; // By default, auto remove when done playing.
    instance->mBuffersHead = NULL; // No buffers to start.
    instance->mBuffersTail = NULL;
    instance->mNext = NULL;
    instance->mDeleteNext = NULL;
    return instance;
}


void AudioLibrary::reclaimBuffers(AudioInstance* instance)
{
    ALint numBuffers = 0;
    alGetSourcei(instance->mSource, AL_BUFFERS_PROCESSED, &numBuffers);
    if(numBuffers > 0)
    {
        ALuint* buffers = (ALuint*)alloca(sizeof(ALuint) * numBuffers);
        alSourceUnqueueBuffers(instance->mSource, numBuffers, buffers);
        int error = alGetError();
        if(error)
        {
            alEnumToString("reclaimBuffers alSourceUnqueueBuffers", error);
        }
    }

    // Reclaim buffers.
    AudioBuffer* curr = instance->mBuffersHead;
    while(curr)
    {
        AudioBuffer* next = curr->mNext;
//        dprintf("D %u on %u (reclaimed)", curr->mBuffer, instance->mSource);
        returnAudioBuffer(curr);
        curr = next;
    }
    instance->mBuffersHead = NULL;
    instance->mBuffersTail = NULL;
}
void AudioLibrary::closeAudioInstance(AudioInstance* instance)
{
    if(!mAudioEnabled)
        return;

    reclaimBuffers(instance);

    closeAudio_platform(instance);

    // Remove instance from active list.

    if(instance->mPrev)
    {
        instance->mPrev->mNext = instance->mNext;
    }
    // No prev, so this is a head remove.
    else
    {
        // Reset head.
        mAudioInstanceActiveHead = instance->mNext;
    }

    if(instance->mNext)
    {
        instance->mNext->mPrev = instance->mPrev;
    }
    /// No next, so this is a tail remove.
    else
    {
        mAudioInstanceActiveTail = instance->mPrev;
    }

    // Push the instance onto the free stack.
    instance->mNext = mAudioInstancesFree;
    mAudioInstancesFree = instance;
}

void AudioLibrary::playAudioInstance(AudioInstance* instance, uint32_t playCount, bool autoRemove)
{
    if(!mAudioEnabled)
        return;

    instance->mPlaysRemain = playCount;
    instance->mAutoRemove = autoRemove;
    alSourcePlay(instance->mSource);
    instance->mState = AudioInstance::ReadyToPlay;
};

void AudioLibrary::pauseAudioInstance(AudioInstance* instance)
{
    alSourcePause(instance->mSource);

    reclaimBuffers(instance);

    instance->mState = AudioInstance::Paused;
}

void AudioLibrary::stopAudioInstance(AudioInstance* instance)
{
    if(!mAudioEnabled)
        return;

    if(instance->mState == AudioInstance::Stopped)
        return;
    
    alSourceStop(instance->mSource);

    stopAudio_platform(instance);

    reclaimBuffers(instance);

    instance->mState = AudioInstance::Stopped;
}

    
void audioBufferSanityCheck(AudioInstance* instance)
{
    dprintf("-- %d", instance->mSource);
    for(AudioBuffer* curr = instance->mBuffersHead; curr; curr = curr->mNext)
    {
        dprintf("%p %d", curr, curr->mBuffer);
    }
}
    
void AudioLibrary::update()
{
    if(mPreloadComplete && !mCompleteNotified)
    {
        if(mObserver)
        {
            mObserver->onAudioPreloadComplete();
        }
        
        mCompleteNotified = true;
    }

    AudioInstance* currActive = mAudioInstanceActiveHead;
    while(currActive)
    {
//        audioBufferSanityCheck(currActive);
        
        
        // Check if any buffers are ready to be unqueued within currActive.
        ALint numBuffers = 0;
        alGetSourcei(currActive->mSource, AL_BUFFERS_PROCESSED, &numBuffers);
        if(numBuffers > 0)
        {
//            dprintf("%d processed ready to remove", numBuffers);
            ALuint* buffers = (ALuint*)alloca(sizeof(ALuint) * numBuffers);
            alSourceUnqueueBuffers(currActive->mSource, numBuffers, buffers);
            int error = alGetError();
            if(error)
            {
                dprintf("%d %p", numBuffers, currActive->mBuffersHead);
                alEnumToString("alSourceUnqueueBuffers", error);
                break;
            }
            // SCD: Hooray for O(n^2). 
            // In reality this is a queue and we will almost always 
            // just be removing the head. So this is OK.
            for(int i=0; i<numBuffers; ++i)
            {
                AudioBuffer* prev = NULL;
                for(AudioBuffer* curr = currActive->mBuffersHead; curr; curr = curr->mNext)
                {
                    if(curr->mBuffer == buffers[i])
                    {
                        // remove buffer from this list.
                        
                        // Middle/tail remove.
                        if(prev)
                        {
//                            dprintf("Middle/tail remove");
                            prev->mNext = curr->mNext;
                        }
                        // Head remove.
                        else
                        {
//                            dprintf("Head remove");
                            currActive->mBuffersHead = curr->mNext;
                        }

                        // Tail remove.
                        if(currActive->mBuffersTail == curr)
                        {
//                            dprintf("... actually tail remove");
                            currActive->mBuffersTail = prev;
                        }

                        // return it to the free queue.
//                        dprintf("D %u on %u", curr->mBuffer, currActive->mSource);

                        returnAudioBuffer(curr);
                        break;
                    }
                    
                    prev = curr;
                }    
            }
//            dprintf("done");
        }    

        if(currActive->mState == AudioInstance::ReadyToPlay || currActive->mState == AudioInstance::Playing)
        {
            // End of stream case.
            if(currActive->mState == AudioInstance::Playing && currActive->mBuffersHead == NULL)        
            {
                // Set to be removed when done playing?
                if(currActive->mAutoRemove)
                {
                    //                    dprintf("%d pushing to delete stack", currActive->mSource);
                    // Push onto toDelete stack.
                    currActive->mDeleteNext = mAudioInstanceToDelete;
                    mAudioInstanceToDelete = currActive;
                }
            }
            else
                updateAudio_platform(currActive);
        }

        currActive = currActive->mNext;
    }
    
    // Close and free all instances that were found to be closed and freed.
    while(mAudioInstanceToDelete)
    {
//        dprintf("Removing %d", mAudioInstanceToDelete->mSource);
        stopAudioInstance(mAudioInstanceToDelete);
        closeAudioInstance(mAudioInstanceToDelete);
        mAudioInstanceToDelete = mAudioInstanceToDelete->mDeleteNext;
    }

    //pthread_mutex_lock(&mMutex);

    //while(mToNotify)
    //{
    //    AudioLoadRequest* curr = mToNotify;
    //    mToNotify = curr->mNext;

    //    if(gAudioClip[curr->mId].mInUse == true)
    //    {
    //        load(curr->mId);
    //    }

    //    curr->mNext = mFree;
    //    mFree = curr;
    //}

    //pthread_mutex_unlock(&mMutex);
}

//void* AudioLibrary::threadBootFunc(void* arg)
//{
//    ((AudioLibrary*)arg)->threadFunc();
//    return 0;
//}
//
//void AudioLibrary::threadFunc()
//{
//    pthread_mutex_lock(&mMutex);
//    while(true)
//    {
//        if(mRunning == false)
//        {
//            pthread_mutex_unlock(&mMutex);
//            return;
//        }
//
//        while(mToService)
//        {
//            AudioLoadRequest* curr = mToService;
//            mToService = curr->mNext;
//            pthread_mutex_unlock(&mMutex);
//
//            load(curr->mId);
//
//            pthread_mutex_lock(&mMutex);
//
//            curr->mNext = mToNotify;
//            mToNotify = curr;
//        }
//        
//        //dprintf("AudioLibrary::threadFunc going to sleep.");
//        pthread_cond_wait(&mCondition, &mMutex);
//        //dprintf("AudioLibrary::threadFunc woke up.");
//    }
//}


} // namespace pammo