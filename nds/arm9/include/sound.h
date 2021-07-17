#pragma once
#ifndef __TWILIGHTMENU_SOUND__
#define __TWILIGHTMENU_SOUND__
#include <nds.h>
#include <mm_types.h>
#include <maxmod9.h>
#include "singleton.h"
#include <cstdio>

/*
 * Handles playing sound effects and the streaming background music control.
 * See streamingaudio.c for a technical overview of how streaming works.
 */
class SoundControl {
    public:
        SoundControl();
        mm_sfxhand playSelect();
        mm_sfxhand playBack();
        mm_sfxhand playHighlight();
        
        // Refill the stream buffers
        volatile void updateStream();

        void loadStream(const char* path, const char* loopPath, u32 sampleRate, bool stereo, bool loop);
        void beginStream();
        void stopStream();
        void fadeOutStream();
        void cancelFadeOutStream();

    private:
        mm_sound_effect snd_select;
        mm_sound_effect snd_back;
        mm_sound_effect snd_highlight;
        mm_stream stream;
        mm_ds_system sys;
        bool stream_is_playing;
        bool stream_is_stereo;
        bool loopingPoint;
        bool looping;
        FILE* stream_start_source;
        FILE* stream_source;
};

typedef singleton<SoundControl> soundCtl_s;
inline SoundControl &snd() { return soundCtl_s::instance(); }

#endif