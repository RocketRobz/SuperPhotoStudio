#include "sound.h"

#include "soundbank.h"
#include "streamingaudio.h"
#include "string.h"
#include "tonccpy.h"
#include <algorithm>

extern bool mepFound;
extern bool dsDebugRam;


extern volatile s16 fade_counter;
extern volatile bool fade_out;

extern volatile s16* play_stream_buf;
extern volatile s16* fill_stream_buf;

// Number of samples filled into the fill buffer so far.
extern volatile s32 filled_samples;

extern volatile bool fill_requested;
extern volatile s32 samples_left_until_next_fill;
extern volatile s32 streaming_buf_ptr;

#define SAMPLES_USED (STREAMING_BUF_LENGTH - samples_left)
#define REFILL_THRESHOLD STREAMING_BUF_LENGTH >> 2

#ifdef SOUND_DEBUG
extern char debug_buf[256];
#endif

extern volatile u32 sample_delay_count;

char* SFX_DATA = (char*)NULL;
mm_word SOUNDBANK[MSL_BANKSIZE] = {0};

SoundControl::SoundControl()
	: stream_is_playing(false), stream_start_source(NULL), stream_source(NULL)
 {

	sys.mod_count = MSL_NSONGS;
	sys.samp_count = MSL_NSAMPS;
	sys.mem_bank = SOUNDBANK;
	sys.fifo_channel = FIFO_MAXMOD;

	FILE* soundbank_file;

	soundbank_file = fopen("nitro:/soundbank.bin", "rb");

	SFX_DATA = new char[0x9800];
	fread(SFX_DATA, 1, 0x9800, soundbank_file);

	fclose(soundbank_file);

	// sprintf(debug_buf, "Read sample length %li for startup", startup_sample_length);
    // nocashMessage(debug_buf);

	mmInit(&sys);
	mmSoundBankInMemory((mm_addr)SFX_DATA);

	mmLoadEffect(SFX_SELECT);
	mmLoadEffect(SFX_BACK);
	mmLoadEffect(SFX_HIGHLIGHT);

	snd_select = {
		{ SFX_SELECT } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		128,	// panning
	};

	snd_back = {
		{ SFX_BACK } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		128,	// panning
	};

	snd_highlight = {
		{ SFX_HIGHLIGHT } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		128,	// panning
	};


	if (!dsDebugRam && !mepFound) return;

	init_streaming_buf();
}

mm_sfxhand SoundControl::playSelect() { return mmEffectEx(&snd_select); }
mm_sfxhand SoundControl::playBack() { return mmEffectEx(&snd_back); }
mm_sfxhand SoundControl::playHighlight() { return mmEffectEx(&snd_highlight); }

void SoundControl::loadStream(const char* path, const char* loopPath, u32 sampleRate, bool loop) {
	if (!dsDebugRam && !mepFound) return;

	if (stream_source) {
		stream_is_playing = false;
		mmStreamClose();
		fclose(stream_source);
	}

	resetStreamSettings();

	stream_start_source = fopen(path, "rb");
	stream_source = fopen(loopPath, "rb");
	if (!stream_source) return;
	bool loopableMusic = stream_start_source ? true : false;


	fseek(stream_source, 0, SEEK_SET);

	stream.sampling_rate = sampleRate;	 		// ?????Hz
	stream.buffer_length = 0x1000;	  		// should be adequate
	stream.callback = on_stream_request;    
	stream.format = MM_STREAM_8BIT_STEREO;  // select format
	stream.timer = MM_TIMER0;	    	   // use timer0
	stream.manual = false;	      		   // auto filling
	looping = loop;

	if (loopableMusic) {
		fseek(stream_start_source, 0, SEEK_END);
		size_t fileSize = ftell(stream_start_source);
		fseek(stream_start_source, 0, SEEK_SET);

		// Prep the first section of the stream
		fread((void*)play_stream_buf, sizeof(s16), STREAMING_BUF_LENGTH, stream_start_source);
		if (fileSize < STREAMING_BUF_LENGTH*sizeof(s16)) {
			size_t fillerSize = 0;
			while (fileSize+fillerSize < STREAMING_BUF_LENGTH*sizeof(s16)) {
				fillerSize++;
			}
			fread((void*)play_stream_buf+fileSize, 1, fillerSize, stream_source);

			// Fill the next section premptively
			fread((void*)fill_stream_buf, sizeof(s16), STREAMING_BUF_LENGTH, stream_source);

			loopingPoint = true;
		} else {
			// Fill the next section premptively
			fread((void*)fill_stream_buf, sizeof(s16), STREAMING_BUF_LENGTH, stream_start_source);
			fileSize -= STREAMING_BUF_LENGTH*sizeof(s16);
			if (fileSize < STREAMING_BUF_LENGTH*sizeof(s16)) {
				size_t fillerSize = 0;
				while (fileSize+fillerSize < STREAMING_BUF_LENGTH*sizeof(s16)) {
					fillerSize++;
				}
				fread((void*)fill_stream_buf+fileSize, 1, fillerSize, stream_source);

				loopingPoint = true;
			}
		}
	} else {
		// Prep the first section of the stream
		fread((void*)play_stream_buf, sizeof(s16), STREAMING_BUF_LENGTH, stream_source);

		// Fill the next section premptively
		fread((void*)fill_stream_buf, sizeof(s16), STREAMING_BUF_LENGTH, stream_source);

		loopingPoint = true;
	}
}

void SoundControl::beginStream() {
	if (!stream_source) return;

	// open the stream
	stream_is_playing = true;
	mmStreamOpen(&stream);
	SetYtrigger(0);
}

void SoundControl::stopStream() {
	if (!stream_source) return;

	stream_is_playing = false;
	mmStreamClose();
}

void SoundControl::fadeOutStream() {
	fade_out = true;
}

void SoundControl::cancelFadeOutStream() {
	fade_out = false;
	fade_counter = FADE_STEPS;
}


// Samples remaining in the fill buffer.
#define SAMPLES_LEFT_TO_FILL (abs(STREAMING_BUF_LENGTH - filled_samples))

// Samples that were already streamed and need to be refilled into the buffer.
#define SAMPLES_TO_FILL (abs(streaming_buf_ptr - filled_samples))

// Updates the background music fill buffer
// Fill the amount of samples that were used up between the
// last fill request and this.

// Precondition Invariants:
// filled_samples <= STREAMING_BUF_LENGTH
// filled_samples <= streaming_buf_ptr

// Postcondition Invariants:
// filled_samples <= STREAMING_BUF_LENGTH
// filled_samples <= streaming_buf_ptr
// fill_requested == false
volatile void SoundControl::updateStream() {
	
	if (!stream_is_playing) return;
	if (fill_requested && filled_samples < STREAMING_BUF_LENGTH) {
			
		// Reset the fill request
		fill_requested = false;
		int instance_filled = 0;

		// Either fill the max amount, or fill up the buffer as much as possible.
		int instance_to_fill = std::min(SAMPLES_LEFT_TO_FILL, SAMPLES_TO_FILL);

		// If we don't read enough samples, loop from the beginning of the file.
		instance_filled = fread((s16*)fill_stream_buf + filled_samples, sizeof(s16), instance_to_fill, loopingPoint ? stream_source : stream_start_source);		
		if (instance_filled < instance_to_fill) {
			if (looping) {
				fseek(stream_source, 0, SEEK_SET);
				int i = fread((s16*)fill_stream_buf + filled_samples + instance_filled,
					 sizeof(s16), (instance_to_fill - instance_filled), stream_source);
				if (i==0) {
					toncset((s16*)fill_stream_buf + filled_samples + instance_filled, 0, (instance_to_fill - instance_filled)*sizeof(s16));
				} else {
					instance_filled += i;
				}
				loopingPoint = true;
			} else {
				toncset((s16*)fill_stream_buf + filled_samples + instance_filled, 0, (instance_to_fill - instance_filled)*sizeof(s16));
			}
		}

		#ifdef SOUND_DEBUG
		sprintf(debug_buf, "FC: SAMPLES_LEFT_TO_FILL: %li, SAMPLES_TO_FILL: %li, instance_filled: %i, filled_samples %li, to_fill: %i", SAMPLES_LEFT_TO_FILL, SAMPLES_TO_FILL, instance_filled, filled_samples, instance_to_fill);
    	nocashMessage(debug_buf);
		#endif

		// maintain invariant 0 < filled_samples <= STREAMING_BUF_LENGTH
		filled_samples = std::min<s32>(filled_samples + instance_filled, STREAMING_BUF_LENGTH);

	
	} else if (fill_requested && filled_samples >= STREAMING_BUF_LENGTH) {
		// filled_samples == STREAMING_BUF_LENGTH is the only possible case
		// but we'll keep it at gte to be safe.
		filled_samples = 0;
		// fill_count = 0;
	}

}
