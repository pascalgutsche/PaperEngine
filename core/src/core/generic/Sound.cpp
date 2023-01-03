#include "_Core.h"

#include "Sound.h"



Sound::Sound()
{
 
}
 
Sound::~Sound()
{
    ma_sound_uninit(&currentSound);
    ma_engine_uninit(&engine);
}
 
void Sound::dataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    // callback function, this is being called when playSound is issued, but as a background thread
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if(pDecoder == NULL)
    {
        Logger::Log("Failed to initialize the sound decoder", Logger::Level::Error);
        return;
    }
    ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);
 
    (void)pInput;
}
 
bool Sound::loadSound(std::string soundFile)
{
    Sound::soundFile = soundFile;
 
    // use the high level api || we are using it because we are already including the library 
    // and we can make use of the high level functions in order to play sounds with their own group
 
    // initialize the engine without specific configuration
    if(ma_engine_init(NULL, &engine) != MA_SUCCESS)
    {
        Logger::Log("Failed to initialize the engine", Logger::Level::Error);
        return false;
    }
 
    if (ma_sound_init_from_file(&engine, soundFile.c_str(), 0, NULL, NULL, &currentSound) != MA_SUCCESS)
    {
        Logger::Log("Failed to load sound '" + std::string(soundFile) + "'", Logger::Level::Error);
        return false;
    }
    return true;
}
 
bool Sound::playSound()
{
    // TODO: make the function add different sound ,,objects'' ma_sound sound = ma_sound_init_from_file ...
    // play sound and specify a sound group (different sound groups can be used to specify their volumes)
 
    if(ma_sound_start(&currentSound) != MA_SUCCESS)
    {
        Logger::Log("Could not play sound '" + std::string(soundFile) + "'", Logger::Level::Error);
        return false;
    }
 
    return true;
}
 
bool Sound::stopSound()
{
    if(ma_sound_stop(&currentSound) != MA_SUCCESS)
    {
        Logger::Log("Could not stop sound '" + std::string(soundFile) + "'", Logger::Level::Error);
        return false;
    }
    return true;
}
 
void Sound::setVolume(float volume)
{
    ma_sound_set_volume(&currentSound, volume);
}
 
float Sound::getVolume()
{
    return ma_sound_get_volume(&currentSound);
}