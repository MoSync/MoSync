//-----------------------------------------------------------------------
// <copyright file="MpegLayer3WaveFormatTests.cs" company="Larry Olson">
// (c) Copyright Larry Olson.
// This source is subject to the Microsoft Public License (Ms-PL)
// See http://code.msdn.microsoft.com/ManagedMediaHelpers/Project/License.aspx
// All other rights reserved.
// </copyright>
//-----------------------------------------------------------------------
namespace MediaParsersTests
{
    using System;
    using System.Diagnostics.CodeAnalysis;
    using System.Net;
    using MediaParsers;
    using Microsoft.VisualStudio.TestTools.UnitTesting;

    [TestClass]
    public class MpegLayer3WaveFormatTests
    {
        private WaveFormatExtensible wfx;
        [SuppressMessage("Microsoft.StyleCop.CSharp.NamingRules", "SA1305:FieldNamesMustNotUseHungarianNotation", Justification = "mp3 is not hungarian notation and is common parlance")]
        private MpegLayer3WaveFormat mp3wfx;

        [TestInitialize]
        public void SetupTests()
        {
            this.wfx = new WaveFormatExtensible();
            this.wfx.FormatTag = 85;
            this.wfx.Channels = 2;
            this.wfx.SamplesPerSec = 8000;
            this.wfx.AverageBytesPerSecond = 500;
            this.wfx.BlockAlign = 1;
            this.wfx.BitsPerSample = 16;
            this.wfx.ExtraDataSize = 12;

            this.mp3wfx = new MpegLayer3WaveFormat();
        }

        [TestMethod]
        public void WaveFormatExtensibleTest()
        {
            this.mp3wfx.WaveFormatExtensible = this.wfx;
            Assert.AreEqual(this.wfx, this.mp3wfx.WaveFormatExtensible);
        }

        [TestMethod]
        public void WaveFormatExtensibleNullTest()
        {
            this.mp3wfx.WaveFormatExtensible = null;
            Assert.AreEqual(null, this.mp3wfx.WaveFormatExtensible);
        }

        [TestMethod]
        public void WaveFormatExtensibleCoherencyTest()
        {
            /*
             * There are certain combinations of fields that are valid
             * and invalid in a MpegLayer3WaveFormat structure.
             * IE:
             *  mp3wfx.WaveFormatExtensible.Size must be 12;
             *  mp3wfx.Id must be 1
             *           
             * Code currently does not check for this but could be added for extra
             * robustness
             * 
             * See the documentation for MPEGLAYER3WAVEFORMAT on msdn
             * http://msdn.microsoft.com/en-us/library/cc307970(VS.85).aspx
             */
            ////Assert.Inconclusive();
        }

        [TestMethod]
        public void IdTest()
        {
            this.mp3wfx.Id = 1;
            Assert.AreEqual(1, this.mp3wfx.Id);
        }

        [TestMethod]
        public void IdMinimumValueTest()
        {
            /*  
             * 1 is the smallest possible Id
             * 
             * Code currently does not check for this but could be added for extra
             * robustness
             * 
             * Clamp or throw
             */
            ////mp3wfx.Id = 1;
            ////Assert.Inconclusive();
        }

        [TestMethod]
        public void IdMaximumValueTest()
        {
            /*  
             * 1 is the largest possible Id
             * 
             * Code currently does not check for this but could be added for extra
             * robustness
             * 
             * Clamp or throw
             */
            ////mp3wfx.Id = 1;
            ////Assert.Inconclusive();
        }

        [TestMethod]
        public void BitratePaddingModeTest()
        {
            this.mp3wfx.BitratePaddingMode = 2;
            Assert.AreEqual(2, this.mp3wfx.BitratePaddingMode);
        }

        [TestMethod]
        public void BitratePaddingModeMinimumValueTest()
        {
            /*  
             * 0 is the smallest possible BitratePaddingMode
             * 
             * Code currently does not check for this but could be added for extra
             * robustness
             * 
             * Clamp or throw
             */
            ////mp3wfx.BitratePaddingMode = -1;
            ////Assert.Inconclusive();
        }

        [TestMethod]
        public void BitratePaddingModeMaximumValueTest()
        {
            /*  
             * 2 is the largest possible BitratePaddingMode
             * 
             * Code currently does not check for this but could be added for extra
             * robustness
             * 
             * Clamp or throw
             */
            ////mp3wfx.BitratePaddingMode = 3;
            ////Assert.Inconclusive();
        }

        [TestMethod]
        public void BlockSizeTest()
        {
            this.mp3wfx.BlockSize = 500;
            Assert.AreEqual(500, this.mp3wfx.BlockSize);
        }

        [TestMethod]
        public void BlockSizeMinimumValueTest()
        {
            /*  
             * 1 is the smallest possible BlockSize
             * 
             * Code currently does not check for this but could be added for extra
             * robustness
             * 
             * Clamp or throw
             */
            ////mp3wfx.BlockSize = 0;
            ////Assert.Inconclusive();
        }

        [TestMethod]
        public void FramePerBlockTest()
        {
            this.mp3wfx.FramesPerBlock = 1;
            Assert.AreEqual(1, this.mp3wfx.FramesPerBlock);
        }

        [TestMethod]
        public void FramePerBlockMinimumValueTest()
        {
            /*  
             * 1 is the smallest possible number of FramesPerBlock
             * 
             * Code currently does not check for this but could be added for extra
             * robustness
             * 
             * Clamp or throw
             */
            ////mp3wfx.FramesPerBlock = 0;
            ////Assert.Inconclusive();
        }

        [TestMethod]
        public void CodecDelayTest()
        {
            this.mp3wfx.CodecDelay = 10;
            Assert.AreEqual(10, this.mp3wfx.CodecDelay);
        }

        [TestMethod]
        public void CodecDelayMinimumValueTest()
        {
            /*  
             * 0 is the smallest possible number of CodecDelay
             * 
             * Code currently does not check for this but could be added for extra
             * robustness
             * 
             * Clamp or throw
             */
            ////mp3wfx.CodecDelay = -1;
            ////Assert.Inconclusive();
        }

        [TestMethod]
        public void ToHexStringTest()
        {
            this.mp3wfx.WaveFormatExtensible = this.wfx;
            this.mp3wfx.Id = 1;
            this.mp3wfx.BitratePaddingMode = 2;
            this.mp3wfx.BlockSize = 1000;
            this.mp3wfx.FramesPerBlock = 1;
            this.mp3wfx.CodecDelay = 0;

            string s = this.mp3wfx.ToHexString();
            string expectedResult = "55000200401F0000F4010000010010000C00010002000000E80301000000";
            Assert.AreEqual(expectedResult, s);
        }

        [TestMethod]
        public void ToStringTest()
        {
            this.mp3wfx.WaveFormatExtensible = this.wfx;
            this.mp3wfx.Id = 1;
            this.mp3wfx.BitratePaddingMode = 2;
            this.mp3wfx.BlockSize = 1000;
            this.mp3wfx.FramesPerBlock = 1;
            this.mp3wfx.CodecDelay = 0;

            string s = this.mp3wfx.ToString();
            string expectedResult = "MPEGLAYER3 WAVEFORMATEX FormatTag: 85, Channels: 2, SamplesPerSec: 8000, AvgBytesPerSec: 500, BlockAlign: 1, BitsPerSample: 16, Size: 12 ID: 1, Flags: 2, BlockSize: 1000, FramesPerBlock 1, CodecDelay 0";
            Assert.AreEqual(expectedResult, s);
        }
    }
}
