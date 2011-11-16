namespace MoSync
{
    public class AudioModule : IIoctlModule
    {
        public void Init(Ioctls ioctls, Core core, Runtime runtime)
        {

            ioctls.maAudioDataCreateFromResource = delegate(int _mime, int _data, int _offset, int _length, int _flags)
            {

                return MoSync.Constants.MA_AUDIO_ERR_INVALID_SOUND_FORMAT;
            };

            ioctls.maAudioDataDestroy = delegate(int _audioData)
            {

                return MoSync.Constants.MA_AUDIO_ERR_OK;
            };


            ioctls.maAudioInstanceCreate = delegate(int _audioData)
            {

                return MoSync.Constants.MA_AUDIO_ERR_INVALID_DATA;
            };

            ioctls.maAudioInstanceDestroy = delegate(int _audioInstance)
            {

                return MoSync.Constants.MA_AUDIO_ERR_OK;
            };

            ioctls.maAudioPlay = delegate(int _audioInstance)
            {

                return MoSync.Constants.MA_AUDIO_ERR_OK;
            };
        }
    }
}
