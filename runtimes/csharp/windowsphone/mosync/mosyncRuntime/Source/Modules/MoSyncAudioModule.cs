using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework;

namespace MoSync
{
    public class AudioModule : IIoctlModule, ISyscallModule
    {
		public void Init(Syscalls syscalls, Core core, Runtime runtime)
		{
			syscalls.maSoundPlay = delegate(int _sound_res, int _offset, int _size)
			{
				// not implemented, but I don't wanna throw exceptions.
				return -1;
			};
		}

        public void Init(Ioctls ioctls, Core core, Runtime runtime)
        {
//			ioctls.maAudioDataCreateFromFile = delegate(int _mime, int _filename, int _flags)
//			{
//				return MoSync.Constants.MA_AUDIO_ERR_INVALID_SOUND_FORMAT;
//			};

            ioctls.maAudioDataCreateFromResource = delegate(int _mime, int _data, int _offset, int _length, int _flags)
            {
				return MoSync.Constants.IOCTL_UNAVAILABLE;
            };

            ioctls.maAudioDataDestroy = delegate(int _audioData)
            {
				return MoSync.Constants.IOCTL_UNAVAILABLE;
            };


            ioctls.maAudioInstanceCreate = delegate(int _audioData)
            {
				return MoSync.Constants.IOCTL_UNAVAILABLE;
            };

            ioctls.maAudioInstanceDestroy = delegate(int _audioInstance)
            {
				return MoSync.Constants.IOCTL_UNAVAILABLE;
            };

            ioctls.maAudioPlay = delegate(int _audioInstance)
            {
				return MoSync.Constants.IOCTL_UNAVAILABLE;
            };
        }
    }
}
