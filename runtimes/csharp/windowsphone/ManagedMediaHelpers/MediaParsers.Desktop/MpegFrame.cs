//-----------------------------------------------------------------------
// <copyright file="MpegFrame.cs" company="Larry Olson">
// (c) Copyright Larry Olson.
// This source is subject to the Microsoft Public License (Ms-PL)
// See http://code.msdn.microsoft.com/ManagedMediaHelpers/Project/License.aspx
// All other rights reserved.
// </copyright>
//-----------------------------------------------------------------------

// Supressing Code Analysis rule(s)

[module: System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Performance", "CA1814:PreferJaggedArraysOverMultidimensional",
    Scope = "member",
    Target = "MediaParsers.MpegFrame.#bitrateTable",
    MessageId = "Member",
    Justification = "Array is not Jagged and does not waste space.")]

[module: System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Performance", "CA1814:PreferJaggedArraysOverMultidimensional",
    Scope = "member",
    Target = "MediaParsers.MpegFrame.#samplingRateTable",
    MessageId = "Member",
    Justification = "Array is not Jagged and does not waste space.")]

namespace MediaParsers
{
    using System;
    using System.IO;

    /// <summary>
    /// Reproduction mode of given audio data. Typically maps to the number of
    /// output devices used to reproduce it.
    /// </summary>
    public enum Channel
    {
        /// <summary>
        /// Stereo: independent audio typically output to 2 speakers and is intended
        /// to create a more realistic or pleasing representation of audio by
        /// representing sound coming from multiple directons.
        /// </summary>
        Stereo = 0,

        /// <summary>
        /// Joint Stereo: The joining of multiple channels of audio to create another separate
        /// one, to reduce the size of the file, or to increase the quality.
        /// </summary>
        JointStereo,

        /// <summary>
        /// Dual Channel: Two independent Mono channels. May overlap with stereo or may 
        /// be completely independent as in the case of foreign language audio dubbing.
        /// </summary>
        DualChannel,

        /// <summary>
        /// Single Channel: Also known as Mono. Typically the reproduction of a single
        /// independent audio stream in one device or of the same independent audio stream
        /// in multiple devices.
        /// </summary>
        SingleChannel,
    }

    /// <summary>
    /// A partial implementation of an MPEG audio frame
    /// </summary>
    /// <remarks>
    /// <para>
    /// The primary purpose of this class is to represent an Mpeg 1 Layer 3
    /// file or MP3 file for short. Many of the features not explicitly needed
    /// for audio rendering are omitted from the implementation.
    /// </para>
    /// <para>
    /// Data on this format is readily discoverable in many books as well as by
    /// searching for "MP3 Frame" in your favorite search engine. As always,
    /// Wikipedia is well stocked in all of these areas as well.
    /// </para>
    /// </remarks>
    public class MpegFrame
    {
        /// <summary>
        /// MP3 Headers are 4 Bytes long
        /// </summary>
        public const int FrameHeaderSize = 4;

        /// <summary>
        /// Frame Sync is 11 1s
        /// </summary>
        private const int SyncValue = 2047;

        /// <summary>
        /// A table of bitrates / 1000. These are all of the possible bitrates for Mpeg 1 - 2.5 audio. -1 encodes an error lookup.
        /// </summary>
        private static int[,] bitrateTable = new int[,]
            {   
                { 0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448 },
                { 0, 32, 48, 56, 64,  80,  96,  112, 128, 160, 192, 224, 256, 320, 384 },
                { 0, 32, 40, 48, 56,  64,  80,  96,  112, 128, 160, 192, 224, 256, 320 },
                { 0, 32, 48, 56, 64,  80,  96,  112, 128, 144, 160, 176, 192, 224, 256 },
                { 0, 8,  16, 24, 32,  40,  48,  56,  64,  80,  96,  112, 128, 144, 160 }
            };

        /// <summary>
        /// A table of all of the possible sampling rates of Mpeg 1 - 2.5 audio. 
        /// </summary>
        private static int[,] samplingRateTable = new int[,]
            {   
                { 44100, 48000, 32000 },
                { 22050, 24000, 16000 },
                { 11025, 12000, 8000 }
            };

        /// <summary>
        /// The frame header for this frame describing its contents. 
        /// </summary>
        private byte[] frameHeader;

        /// <summary>
        /// Initializes a new instance of the MpegFrame class.
        /// </summary>
        /// <param name="stream">
        /// A stream with its position at the SyncPoint of the header.
        /// </param>
        public MpegFrame(Stream stream)
            : this(stream, null)
        {
        }

        /// <summary>
        /// Initializes a new instance of the MpegFrame class.
        /// </summary>
        /// <param name="stream">
        /// A stream with its position at the SyncPoint of the header.
        /// </param>
        /// <param name="data">
        /// The first 4 bytes of the audiostream. This should contain the MpegFrame's
        /// header data.
        /// </param>
        public MpegFrame(Stream stream, byte[] data)
        {
            this.frameHeader = new byte[FrameHeaderSize];

            if (data != null)
            {
                for (int i = 0; i < this.frameHeader.Length; i++)
                {
                    this.frameHeader[i] = data[i];
                }
            }
            else
            {
                // Guard against a read error
                if (stream.Read(this.frameHeader, 0, FrameHeaderSize) != FrameHeaderSize)
                {
                    goto cleanup;
                }
            }

            // Sync
            int value = BitTools.MaskBits(this.frameHeader, 0, 11);
            if (!(value == SyncValue))
            {
                goto cleanup;
            }

            this.Version = ParseVersion(this.frameHeader);
            this.Layer = ParseLayer(this.frameHeader);
            this.IsProtected = BitTools.MaskBits(this.frameHeader, 15, 1) == 1 ? false : true;
            this.BitrateIndex = BitTools.MaskBits(this.frameHeader, 16, 4);
            this.SamplingRateIndex = BitTools.MaskBits(this.frameHeader, 20, 2);
            this.Padding = BitTools.MaskBits(this.frameHeader, 22, 1);
            //// Private Bit = BitTools.MaskBits(_mp3FrameHeader,8,1); //USELESS
            this.Channels = ParseChannel(this.frameHeader);
            //// Joint Mode = ParseJoitMode(_mp3FrameHeader); //Not used by  Mp3MSS
            //// CopyRight = BitTools.MaskBits(_mp3FrameHeader,3,1); //Not used by Mp3MSS
            //// Original = BitTools.MaskBits(_mp3FrameHeader,2,1); //Not used by Mp3MSS
            //// Emphasis = ParseEmphasis(_mp3FrameHeader); //Not used by Mp3MSS

            return;
        cleanup:
            this.frameHeader = null;
            return;
        }

        /**********************************************************************
         * FILE DATA- data which comes directly from the MP3 header.
         *********************************************************************/
        #region File Data

        /// <summary>
        /// Gets the Version of the MPEG standard this frame conforms to.
        /// MPEG 1, MPEG 2, or MPEG 2.5
        /// </summary>
        public int Version { get; private set; }

        /// <summary>
        /// Gets the layer of complexity used in this frame.
        /// Layer 1, 2, or 3.
        /// </summary>
        public int Layer { get; private set; }

        /// <summary>
        /// Gets a value indicating whether or not the frame is protected by a
        /// Cyclic Redundancy Check (CRC). If true, then a 16 bit
        /// CRC follows the header.
        /// </summary>
        public bool IsProtected { get; private set; }

        /// <summary>
        /// Gets the Index into the bitrate table as defined in the MPEG spec.
        /// </summary>
        public int BitrateIndex { get; private set; }

        /// <summary>
        /// Gets the Index into the samplingrate table as defined in the MPEG spec.
        /// </summary>
        public int SamplingRateIndex { get; private set; }

        /// <summary>
        /// Gets the number of additional bytes of padding in this frame.
        /// </summary>
        public int Padding { get; private set; }

        /// <summary>
        /// Gets the output channel used to playback this frame.
        /// </summary>
        public Channel Channels { get; private set; }
        #endregion

        /**********************************************************************
         * DERIVED DATA - data which is calculated from data in the header.
         *********************************************************************/
        #region Derived Data

        /// <summary>
        /// Gets the number of bits per second the raw audio is compressed into.
        /// </summary>
        public int Bitrate
        {
            get
            {
                switch (this.Version)
                {
                    case 1: // Version 1.0
                        switch (this.Layer)
                        {
                            case 1: // MPEG 1 Layer 1
                                return bitrateTable[0, this.BitrateIndex] * 1000;
                            case 2: // MPEG 1 Layer 2
                                return bitrateTable[1, this.BitrateIndex] * 1000;
                            case 3: // MPEG 1 Layer 3 (MP3)
                                return bitrateTable[2, this.BitrateIndex] * 1000;
                            default: // MPEG 1 LAYER ERROR
                                return -2;
                        }

                    case 2: // Version 2.0
                    case 3: // Version 2.5 in reality
                        switch (this.Layer)
                        {
                            case 1: // MPEG 2 or 2.5 Layer 1
                                return bitrateTable[3, this.BitrateIndex] * 1000;
                            case 2: // MPEG 2 or 2.5 Layer 2
                            case 3: // MPEG 2 or 2.5 Layer 3
                                return bitrateTable[4, this.BitrateIndex] * 1000;
                            default: // Mpeg 2 LAYER ERROR
                                return -2;
                        }

                    default: // VERSION ERROR
                        return -2;
                }
            }
        }

        /// <summary>
        /// Gets the number of samples per second in the frame.
        /// </summary>
        public int SamplingRate
        {
            get
            {
                switch (this.Version)
                {
                    case 1: // MPEG 1
                        return samplingRateTable[0, this.SamplingRateIndex];
                    case 2: // MPEG 2
                        return samplingRateTable[1, this.SamplingRateIndex];
                    case 3: // MPEG 2.5
                        return samplingRateTable[2, this.SamplingRateIndex];
                    default:
                        return -1; // RESERVED
                }
            }
        }

        /// <summary>
        /// Gets the number of bytes in the frame.
        /// </summary>
        public int FrameSize
        {
            get
            {
                switch (this.Layer)
                {
                    case 1:
                        return ((12 * this.Bitrate / this.SamplingRate) + this.Padding) * 4;
                    case 2:
                    case 3:
                        return ((this.Version == 1 ? 144 : 72) * this.Bitrate / this.SamplingRate) + this.Padding;
                    default:
                        return -1;
                }
            }
        }
        #endregion

        /// <summary>
        /// Converts the MpegFrame into a human readable form.
        /// </summary>
        /// <returns>
        /// A textual representation of the MpegFrame.
        /// </returns>
        public override string ToString()
        {
            string s = string.Empty;
            s += "FrameSize\t" + this.FrameSize + "\n";
            s += "BitRate\t" + this.Bitrate + "\n";
            s += "SamplingRate" + this.SamplingRate + "\n";
            return s;
        }

        /// <summary>
        /// Copies the MpegFrame's FrameHeader to the berginning of the desired buffer.
        /// </summary>
        /// <param name="destinationBuffer">Buffer to copy FrameHeader into.</param>
        public void CopyHeader(byte[] destinationBuffer)
        {
            this.frameHeader.CopyTo(destinationBuffer, 0);
        }

        /// <summary>
        /// Parses the version of the MPEG standard this frame header conforms to from the frame header.
        /// </summary>
        /// <param name="frameHeader"> The 4 byte header for this frame. </param>
        /// <returns>
        /// The version of the MPEG standard this frame conforms to.
        /// 1 = Mpeg 1
        /// 2 = Mpeg 2
        /// 3 = Mpeg 2.5
        /// </returns>
        private static int ParseVersion(byte[] frameHeader)
        {
            int version;
            int versionValue = BitTools.MaskBits(frameHeader, 11, 2);

            switch (versionValue)
            {
                case 3:
                    version = 1;
                    break;
                case 2:
                    version = 2;
                    break;
                case 0:
                    version = 3;
                    break;
                default:
                    version = -1;   // ERROR
                    break;
            }

            return version;
        }

        /// <summary>
        /// Parses which complexity layer of the MPEG standard this frame conforms to from the frame header.
        /// </summary>
        /// <param name="frameHeader">The 4 byte header for this frame.</param>
        /// <returns>The complexity layer this frame conforms to.</returns>
        private static int ParseLayer(byte[] frameHeader)
        {
            int layer;
            int layerValue = BitTools.MaskBits(frameHeader, 13, 2);

            switch (layerValue)
            {
                case 3:
                    layer = 1;
                    break;
                case 2:
                    layer = 2;
                    break;
                case 1:
                    layer = 3;
                    break;
                default:
                    layer = -1;
                    break;
            }

            return layer;
        }

        /// <summary>
        /// Parses the audio output mode of this frame's audio data.
        /// </summary>
        /// <param name="frameHeader">The 4 byte header for this frame.</param>
        /// <returns>The audio output mode of this frame's audio data.</returns>
        private static Channel ParseChannel(byte[] frameHeader)
        {
            Channel channel;
            int channelValue = BitTools.MaskBits(frameHeader, 24, 2);

            switch (channelValue)
            {
                case 3:
                    channel = Channel.SingleChannel;
                    break;
                case 2:
                    channel = Channel.DualChannel;
                    break;
                case 1:
                    channel = Channel.JointStereo;
                    break;
                case 0:
                    channel = Channel.Stereo;
                    break;
                default:
                    channel = Channel.SingleChannel; // ERROR CASE
                    break;
            }

            return channel;
        }
    }
}
