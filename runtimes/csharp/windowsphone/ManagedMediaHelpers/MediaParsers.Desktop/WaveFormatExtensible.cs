//-----------------------------------------------------------------------
// <copyright file="WaveFormatExtensible.cs" company="Larry Olson">
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

    /// <summary>
    /// A managed representation of the multimedia WAVEFORMATEX structure
    /// declared in mmreg.h.
    /// </summary>
    /// <remarks>
    /// This was designed for usage in an environment where PInvokes are not
    /// allowed.
    /// </remarks>
    public class WaveFormatExtensible
    {
        /// <summary>
        /// Gets or sets the audio format type. A complete list of format tags can be
        /// found in the Mmreg.h header file.
        /// </summary>
        /// <remarks>
        /// Silverlight 2 supports:
        /// WMA 7,8,9
        /// WMA 10 Pro
        /// Mp3
        /// WAVE_FORMAT_MPEGLAYER3 = 0x0055
        /// </remarks>
        public short FormatTag { get; set; }

        /// <summary>
        /// Gets or sets the number of channels in the data. 
        /// Mono            1
        /// Stereo          2
        /// Dual            2 (2 Mono channels)
        /// </summary>
        /// <remarks>
        /// Silverlight 2 only supports stereo output and folds down higher
        /// numbers of channels to stereo.
        /// </remarks>
        public short Channels { get; set; }

        /// <summary>
        /// Gets or sets the sampling rate in hertz (samples per second)
        /// </summary>
        public int SamplesPerSec { get; set; }

        /// <summary>
        /// Gets or sets the average data-transfer rate, in bytes per second, for the format.
        /// </summary>
        public int AverageBytesPerSecond { get; set; }

        /// <summary>
        /// Gets or sets the minimum size of a unit of data for the given format in Bytes.
        /// </summary>
        public short BlockAlign { get; set; }

        /// <summary>
        /// Gets or sets the number of bits in a single sample of the format's data.
        /// </summary>
        public short BitsPerSample { get; set; }

        /// <summary>
        /// Gets or sets the size in bytes of any extra format data added to the end of the
        /// WAVEFORMATEX structure.
        /// </summary>
        public short ExtraDataSize { get; set; }

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
            char[] data = new char[9 * 4];
            BitTools.ToHexHelper(4, this.FormatTag, 0, data);
            BitTools.ToHexHelper(4, this.Channels, 4, data);
            BitTools.ToHexHelper(8, this.SamplesPerSec, 8, data);
            BitTools.ToHexHelper(8, this.AverageBytesPerSecond, 16, data);
            BitTools.ToHexHelper(4, this.BlockAlign, 24, data);
            BitTools.ToHexHelper(4, this.BitsPerSample, 28, data);
            BitTools.ToHexHelper(4, this.ExtraDataSize, 32, data);
            return new string(data);
        }

        /// <summary>
        /// Returns a string representing all of the fields in the object.
        /// </summary>
        /// <returns>
        /// A string representing all of the fields in the object.
        /// </returns>
        public override string ToString()
        {
            return string.Format(
                CultureInfo.InvariantCulture,
                "WAVEFORMATEX FormatTag: {0}, Channels: {1}, SamplesPerSec: {2}, AvgBytesPerSec: {3}, BlockAlign: {4}, BitsPerSample: {5}, Size: {6} ",
                this.FormatTag,
                this.Channels,
                this.SamplesPerSec,
                this.AverageBytesPerSecond,
                this.BlockAlign,
                this.BitsPerSample,
                this.ExtraDataSize);
        }
    }
}
