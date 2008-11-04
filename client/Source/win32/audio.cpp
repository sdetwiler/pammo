#include "pammo.h"
#include "audioLibrary.h"

#include <SDL_sound.h>

#define SDL_BUFFER_SIZE 65536

namespace pammo
{

int openAudio_platform(AudioInstance* instance)
{
    char path[512];
    char fullPath[512];
    getcwd(path, sizeof(path));
    snprintf(fullPath, sizeof(fullPath)-1, "%s\\data\\%s", path, instance->mClip->mPath);

    char* slash = strchr(fullPath, '/');
    while(slash)
    {
        *slash = '\\';
        slash = strchr(slash+1, '/');
    }

    instance->mAudio.mSample = Sound_NewSampleFromFile(fullPath, NULL, SDL_BUFFER_SIZE);
    if(instance->mAudio.mSample == NULL)
    {
        dprintf("%s", Sound_GetError());
        return -1;
    }

    dprintf("Opened as:\n  channels: %d\n  format: 0x%x\n  rate: %d",
        instance->mAudio.mSample->actual.channels,
        instance->mAudio.mSample->actual.format,
        instance->mAudio.mSample->actual.rate
        );

    int bytes;
    switch(instance->mAudio.mSample->actual.channels)
    {
    case 1:
        if((instance->mAudio.mSample->actual.format & 0x0008) == 0x0008)
        {
            instance->mFormat = AL_FORMAT_MONO8;
            bytes = 1;
        }
        else
        {
            instance->mFormat = AL_FORMAT_MONO16;
            bytes = 2;
        }
        break;
    case 2:
        if((instance->mAudio.mSample->actual.format & 0x0008) == 0x0008)
        {
            instance->mFormat = AL_FORMAT_STEREO8;
            bytes = 1;
        }
        else
        {
            instance->mFormat = AL_FORMAT_STEREO16;
            bytes = 2;
        }
        break;
    default:
        dprintf("WTF");
    }
    instance->mSampleRate = instance->mAudio.mSample->actual.rate;

    instance->mAudio.mReadFreq = ((float)SDL_BUFFER_SIZE/(float)(instance->mSampleRate * bytes)) * 1000000.0f;
    instance->mAudio.mReadFreq -= 500000;
    instance->mAudio.mNextRead = getTime();

    return 0;
}

void closeAudio_platform(AudioInstance* instance)
{
    Sound_FreeSample(instance->mAudio.mSample);
    instance->mAudio.mSample = NULL;
}
void stopAudio_platform(AudioInstance* instance)
{

}

void resetAudio_platform(AudioInstance* instance)
{
    int ret = Sound_Rewind(instance->mAudio.mSample);
    if(ret < 0)
        dprintf("ASSS");
    instance->mAudio.mNextRead = getTime();
}

void updateAudio_platform(AudioInstance* instance)
{
    uint64_t now = getTime();
    if(now < instance->mAudio.mNextRead)
        return;

    instance->mAudio.mNextRead+= instance->mAudio.mReadFreq;

    uint32_t numBytes = Sound_Decode(instance->mAudio.mSample);
    if(numBytes)
    {
        if(numBytes < SDL_BUFFER_SIZE)
        {
            instance->mPlaysRemain--;
            if(instance->mPlaysRemain)
            {
                resetAudio_platform(instance);
            }

//            dprintf("%d", instance->mAudio.mSample->flags);
//            dprintf("EOF");
        }
        AudioBuffer* b = gAudioLibrary->getAudioBuffer();
        if(!b)
        {
            dprintf("Out of audio buffers!");
            return;
        }
        alBufferData(b->mBuffer, instance->mFormat, instance->mAudio.mSample->buffer, numBytes, instance->mSampleRate);
        //dprintf("Q %u on %u", b->mBuffer, instance->mSource);
        alSourceQueueBuffers(instance->mSource, 1, &b->mBuffer);
        if(instance->mState == AudioInstance::ReadyToPlay)
        {
            instance->mState = AudioInstance::Playing;
            alSourcePlay(instance->mSource);
        }

        if(!instance->mBuffersHead)
        {
 //           dprintf("head insert");
            instance->mBuffersHead = b;
            instance->mBuffersTail = b;
        }

        instance->mBuffersTail->mNext = b;
        b->mNext = NULL;
        instance->mBuffersTail = b;
    }

    ALint state;
    alGetSourcei(instance->mSource, AL_SOURCE_STATE, &state);
    
    // Stopped but still buffers.
    if(state == AL_STOPPED && instance->mBuffersHead)
    {
        dprintf("Buffer underrun. Restarting stream.");
        alSourcePlay(instance->mSource);
    }
}

} // namespace pammo