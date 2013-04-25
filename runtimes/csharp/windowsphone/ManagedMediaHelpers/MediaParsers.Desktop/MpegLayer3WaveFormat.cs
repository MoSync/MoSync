//-----------------------------------------------------------------------
// <copyright file="MpegLayer3WaveFormat.cs" company="Larry Olson">
// (c) Copyright Larry Olson.
// This source is subject to the Microsoft Public License (Ms-PL)
// See http://code.msdn.microsoft.com/ManagedMediaHelpers/Project/License.aspx
// All other rights reserved.
// </copyright>
//-----------------------------------------------------------------------

namespace MediaParsers
{
    using System;
    using System.Globalization;
    using System.Text;

    /// <summary>
    /// A managed representation of the multimedia MPEGLAYER3WAVEFORMATEX 
    /// structure declared in mmreg.h.
    /// </summary>
    /// <remarks>
    /// This was designed for usage in an environment where PInvokes are not
    /// allowed.
    /// </remarks>
    public class MpegLayer3WaveFormat
    {
        /// <summary>
        /// Gets or sets the core WaveFormatExtensible strucutre representing the Mp3 audio data's
        /// core attributes. 
        /// </summary>
        /// <remarks>
        /// wfx.FormatTag must be WAVE_FORMAT_MPEGLAYER3 = 0x0055 = (85)
        /// wfx.Size must be >= 12
        /// </remarks>
        public WaveFormatExtensible WaveFormatExtensible { get; set; }

        /// <summary>
        /// Gets or sets the FormatTag that defines what type of waveform audio this is.
        /// </summary>
        /// <remarks>
        /// Set this to 
        /// MPEGLAYER3_ID_MPEG = 1
        /// </remarks>
        public short Id { get; set; }

        /// <summary>
        /// Gets or sets the bitrate padding mode. 
        /// This value is set in an Mp3 file to determine if padding is needed to adjust the average bitrate
        /// to meet the sampling rate.
        /// 0 = adjust as needed
        /// 1 = always pad
        /// 2 = never pad
        /// </summary>
        /// <remarks>
        /// This is different than the unmanaged version of MpegLayer3WaveFormat
        /// which has the field Flags instead of this name.
        /// </remarks>
        public int BitratePaddingMode { get; set; }

        /// <summary>
        /// Gets or sets the Block Size in bytes. For MP3 audio this is
        /// 144 * bitrate / samplingRate + padding
        /// </summary>
        public short BlockSize { get; set; }

        /// <summary>
        /// Gets or sets the number of frames per block.
        /// </summary>
        public short FramesPerBlock { get; set; }

        /// <summary>
        /// Gets or sets the encoder delay in samples.
        /// </summary>
        public short CodecDelay { get; set; }

        /// <summary>
        /// Returns a string representing the structure in little-endian 
        /// hexadecimal format.
        /// </summary>
        /// <remarks>
        /// The string generated here is intended to be passed as 
        /// CodecPrivateData for Silverlight 2's MediaStreamSource
        /// </remarks>
        /// <returns>
        /// A string representing the structure in little-endia hexadecimal
        /// format.
        /// </returns>
        public string ToHexString()
        {
            string s = WaveFormatExtensible.ToHexString();
            char[] mpeglayer3Data = new char[6 * 4];
            BitTools.ToHexHelper(4, this.Id, 0, mpeglayer3Data);
            BitTools.ToHexHelper(8, this.BitratePaddingMode, 4, mpeglayer3Data);
            BitTools.ToHexHelper(4, this.BlockSize, 12, mpeglayer3Data);
            BitTools.ToHexHelper(4, this.FramesPerBlock, 16, mpeglayer3Data);
            BitTools.ToHexHelper(4, this.CodecDelay, 20, mpeglayer3Data);
            return s + new string(mpeglayer3Data);
        }

        /// <summary>
        /// Returns a string representing all of the fields in the object.
        /// </summary>
        /// <returns>
        /// A string representing all of the fields in the object.
        /// </returns>
        public override string ToString()
        {
            return "MPEGLAYER3 "
                + WaveFormatExtensible.ToString()
                + string.Format(
                    CultureInfo.InvariantCulture,
                    "ID: {0}, Flags: {1}, BlockSize: {2}, FramesPerBlock {3}, CodecDelay {4}",
                    this.Id,
                    this.BitratePaddingMode,
                    this.BlockSize,
                    this.FramesPerBlock,
                    this.CodecDelay);
        }
    }
}
