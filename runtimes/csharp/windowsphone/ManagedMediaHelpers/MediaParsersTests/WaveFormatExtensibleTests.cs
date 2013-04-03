//-----------------------------------------------------------------------
// <copyright file="WaveFormatExtensibleTests.cs" company="Larry Olson">
// (c) Copyright Larry Olson.
// This source is subject to the Microsoft Public License (Ms-PL)
// See http://code.msdn.microsoft.com/ManagedMediaHelpers/Project/License.aspx
// All other rights reserved.
// </copyright>
//-----------------------------------------------------------------------
namespace MediaParsersTests
{
    using System;
    using System.Net;
    using MediaParsers;
    using Microsoft.VisualStudio.TestTools.UnitTesting;

    [TestClass]
    public class WaveFormatExtensibleTests
    {
        private WaveFormatExtensible wfx;

        [TestInitialize]
        public void SetupTests()
        {
            this.wfx = new WaveFormatExtensible();
        }

        [TestMethod]
        public void FormatTagTest()
        {
            this.wfx.FormatTag = 1;
            Assert.AreEqual(1, this.wfx.FormatTag);   
        }

        [TestMethod]
        public void FormatTagMaximumValueTest()
        {
            /* 
             * There is a maximum Format that has been defined for WFXs
             * should be in mmreg.h
             *  
             * Code currently does not check for this but could be added for extra
             * robustness
             * 
             * Clamp or throw
             */
            ////Assert.Inconclusive();
        }

        [TestMethod]
        public void FormatTagMinimumValueTest()
        {
            /*  
             * 0 is the smallest possible format ID
             * 
             * Code currently does not check for this but could be added for extra
             * robustness
             *
             * Clamp or throw
             */
            //// wfx.FormatTag = -1;
            ////Assert.Inconclusive();
        }

        [TestMethod]
        public void ChannelsTest()
        {
            this.wfx.Channels = 1;
            Assert.AreEqual(1, this.wfx.Channels);
            
            this.wfx.Channels = 2;
            Assert.AreEqual(2, this.wfx.Channels);
        }

        [TestMethod]
        public void ChannelsMaximumValueTest()
        {
            /*  
             * 2 is the largest number of channels
             * 
             * Code currently does not check for this but could be added for extra
             * robustness
             * 
             * Clamp or throw
             */
            ////wfx.Channels = 3;
            ////Assert.Inconclusive();
        }

        [TestMethod]
        public void ChannelsMinimumValueTest()
        {
            /*  
             * 1 is the smallest number of channels
             * 
             * Code currently does not check for this but could be added for extra
             * robustness
             * 
             * Clamp or throw
             */
            ////wfx.Channels = 0;
            ////Assert.Inconclusive();
        }

        [TestMethod]
        public void SamplesPerSecTest()
        {
            this.wfx.SamplesPerSec = 1000;
            Assert.AreEqual(1000, this.wfx.SamplesPerSec);
        }

        [TestMethod]
        public void SamplesPerSecMinimumValueTest()
        {
            /*  
             * 0 is the smallest number of SamplesPerSecTest
             * 
             * Code currently does not check for this but could be added for extra
             * robustness
             * 
             * Clamp or throw
             */
             //// wfx.SamplesPerSec = -1;
             ////Assert.Inconclusive();
        }

        [TestMethod]
        public void AverageBytesPerSecondTest()
        {
            this.wfx.AverageBytesPerSecond = 1000;
            Assert.AreEqual(1000, this.wfx.AverageBytesPerSecond);
        }

        [TestMethod]
        public void AverageBytesPerSecondMinimumValueTest()
        {
            /*  
             * 0 is the smallest number of AverageBytesPerSecond
             * 
             * Code currently does not check for this but could be added for extra
             * robustness
             * 
             * Clamp or throw
             */
            //// wfx.AverageBytesPerSecond = -1;
            ////Assert.Inconclusive();
        }

        [TestMethod]
        public void BlockAlignTest()
        {
            this.wfx.BlockAlign = 1000;
            Assert.AreEqual(1000, this.wfx.BlockAlign);
        }

        [TestMethod]
        public void BlockAlignMinimumValueTest()
        {
            /*  
             * 0 is the smallest number of BlockAlign
             * 
             * Code currently does not check for this but could be added for extra
             * robustness
             * 
             * Clamp or throw
             */
            //// wfx.BlockAlign = -1;
            ////Assert.Inconclusive();
        }

        [TestMethod]
        public void BitsPerSampleTest()
        {
            this.wfx.BitsPerSample = 16;
            Assert.AreEqual(16, this.wfx.BitsPerSample);
        }

        [TestMethod]
        public void BitsPerSampleMinimumValueTest()
        {
            /*  
             * 1 is the smallest number of BitsPerSample
             * 
             * Code currently does not check for this but could be added for extra
             * robustness
             * 
             * Clamp or throw
             */
            //// wfx.BitsPerSample = 0;
            ////Assert.Inconclusive();
        }

        [TestMethod]
        public void SizeTest()
        {
            this.wfx.ExtraDataSize = 12;
            Assert.AreEqual(12, this.wfx.ExtraDataSize);
        }

        [TestMethod]
        public void SizeMinimumValueTest()
        {
            /*  
             * 0 is the smallest number of Size
             * 
             * Code currently does not check for this but could be added for extra
             * robustness
             * 
             * Clamp or throw
             */
            //// wfx.Size = -1;
            ////Assert.Inconclusive();
        }

        [TestMethod]
        public void WaveFormatExtensibleCoherencyTests()
        {
            /*
             * There are certain combinations of fields that are valid
             * and invalid in a WaveFormatExtensible structure.
             * IE haveing Format = 0 means that Size = 0;
             * 
             * Code currently does not check for this but could be added for extra
             * robustness
             * 
             * See the documentation for WAVEFORMATEX on msdn
             * http://msdn.microsoft.com/en-us/library/ms713497.aspx
             */
            ////Assert.Inconclusive();
        }

        [TestMethod]
        public void ToHexStringTest()
        {
            this.wfx.FormatTag = 85;
            this.wfx.Channels = 2;
            this.wfx.SamplesPerSec = 8000;
            this.wfx.AverageBytesPerSecond = 500;
            this.wfx.BlockAlign = 1;
            this.wfx.BitsPerSample = 16;
            this.wfx.ExtraDataSize = 12;

            string s = this.wfx.ToHexString();
            string expectedResult = "55000200401F0000F4010000010010000C00";
            Assert.AreEqual(expectedResult, s);
        }

        [TestMethod]
        public void ToStringTest()
        {
            this.wfx.FormatTag = 85;
            this.wfx.Channels = 2;
            this.wfx.SamplesPerSec = 8000;
            this.wfx.AverageBytesPerSecond = 500;
            this.wfx.BlockAlign = 1;
            this.wfx.BitsPerSample = 16;
            this.wfx.ExtraDataSize = 12;

            string s = this.wfx.ToString();
            string expectedResult = "WAVEFORMATEX FormatTag: 85, Channels: 2, SamplesPerSec: 8000, AvgBytesPerSec: 500, BlockAlign: 1, BitsPerSample: 16, Size: 12 ";
            Assert.AreEqual(expectedResult, s);
        }
    }
}
