#include <sound.h>
#include "iostream"

#define HIT_SOUND "Single-yard-dog-woof.wav"
Uint8 *soundPosition;
Uint32 soundRemaining;
SDL_atomic_t audioCallbackLeft;
void myCallback( void *userData, Uint8 *stream, int length );

Sound::Sound()
{
	 soundLoaded = false;
	 soundPlaying = false;
     alive = true;

	 threadManager = NULL;
}
Sound::~Sound()
{
    alive = false;
    
    if( threadManager != NULL )
    {

        threadManager->join( );
        delete threadManager;
        threadManager = NULL;
    }

    SDL_PauseAudioDevice( dev, 1 );

	SDL_CloseAudioDevice( dev );

	if( soundLoaded )
	{
		SDL_FreeWAV( soundBuffer );
	}
  
}

void Sound::loadSound( std::string soundPath )
{
	if( SDL_LoadWAV( soundPath.c_str(), &soundSpec, &soundBuffer, &soundLength ) == NULL )
	{
		std::cout << "Unable to load sound" << std::endl;
		return;
	}

	soundSpec.callback = myCallback;
	soundSpec.userdata = NULL;
	soundLoaded = true;

    dev = SDL_OpenAudioDevice( NULL, 0, &soundSpec, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE );

	if( dev == 0 )
	{
		std::cout << "Couldn't open audio: " << SDL_GetError( ) << std::endl;

	}

	SDL_PauseAudioDevice(  dev, 1 );

    threadManager = new std::thread( &Sound::playSound, this );

	std::cout << "sound was loaded successfully!" << std::endl;
}


void Sound::playSound()
{
    while( alive )
    {
		std::cout << "a" << std::endl;
        if( soundPlaying )
        {
			std::cout << "b" << std::endl;
            soundPosition = soundBuffer;
            soundRemaining = soundLength;

            SDL_PauseAudioDevice( dev, 0 );
			std::cout << "c" << std::endl;
            while( soundRemaining > 0 )
            {
                SDL_Delay( 50 );
				std::cout << soundRemaining << std::endl;
            }
			std::cout << "e" << std::endl;
            SDL_PauseAudioDevice( dev, 1 );
			std::cout << "f" << std::endl;
            soundPlaying = false;
			std::cout << "g" << std::endl;
        }
    }

}

void Sound::launchSound( )
{   
    if( soundPlaying )
    {
        return;
    }

    soundPlaying = true;
}

bool Sound::SoundPlaying( )
{
	return soundPlaying;
}

void myCallback( void *userData, Uint8 *stream, int length)
{
    Sint32 localAudioCbLeft = SDL_AtomicGet( &audioCallbackLeft );
    
    Uint32 index;

    for( index = 0; index < length; index++ )
    {
        stream[ index ] = soundPosition[localAudioCbLeft ];

        localAudioCbLeft++;

        if( localAudioCbLeft >= soundRemaining )
        {
            localAudioCbLeft = 0;
            soundRemaining = 0;
        }

        
    }

    SDL_AtomicSet( &audioCallbackLeft, localAudioCbLeft );

}
















// Sound::Sound(){

// }

// Sound::~Sound(){
    
// }

// void Sound::Initialize(){
// 	// Initialize SDL.
// 	if (SDL_Init(SDL_INIT_AUDIO) < 0){
// 			std::cout << "could not initialize sound" << std::endl;
// 	}

// 	/* Load the WAV */
// 	// the specs, length and buffer of our wav are filled
// 	if( SDL_LoadWAV(HIT_SOUND, &wav_spec, &wav_buffer, &wav_length) == NULL ){
// 	  std::cout << "could not load sound " << std::endl;
// 	}

// 	// set the callback function
// 	//wav_spec.callback = my_audio_callback;
// 	wav_spec.userdata = NULL;
// 	audio_pos = wav_buffer; // copy sound buffer
// 	audio_len = wav_length; // copy file length


// 		if ( SDL_OpenAudio(&wav_spec, NULL) < 0 ){
// 	  fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
// 	}
	

// 	/* Start playing */
// 	SDL_PauseAudio(0);
// printf("7\n");
// 	// wait until we're don't playing
// 	std::cout << audio_len << std::endl;
// 	while ( audio_len > 0 ) {
// 		SDL_Delay(100); 
// 	}
// printf("8\n");
// 	// shut everything down
// 	SDL_CloseAudio();
// printf("9\n");
// 	SDL_FreeWAV(wav_buffer);
// printf("10\n");

// }

// // audio callback function. here you have to copy the data of your audio buffer into the requesting audio buffer (stream) you should only copy as much as the requested length (len)
// void Sound::audioCallback(void *userdata, Uint8 *stream, int len){
//     // if (audio_len ==0){
//     //     return;
//     // }

// 	// len = ( len > audio_len ? audio_len : len );
// 	// //SDL_memcpy (stream, audio_pos, len); 					// simply copy from one buffer into the other
// 	// SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another
	
// 	// audio_pos += len;
// 	// audio_len -= len;
// }

// void Sound::playSound(){
// /* Open the audio device */
// 	if ( SDL_OpenAudio(&wav_spec, NULL) < 0 ){
// 	  fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
// 	}
	

// 	/* Start playing */
// 	SDL_PauseAudio(0);
// printf("7\n");
// 	// wait until we're don't playing
// 	std::cout << audio_len << std::endl;
// 	while ( audio_len > 0 ) {
// 		SDL_Delay(100); 
// 	}
// printf("8\n");
// 	// shut everything down
// 	SDL_CloseAudio();
// printf("9\n");
// 	SDL_FreeWAV(wav_buffer);
// printf("10\n");
// }