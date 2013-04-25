//-----------------------------------------------------------------------
// <copyright file="MpegFrameTests.cs" company="Larry Olson">
// (c) Copyright Larry Olson.
// This source is subject to the Microsoft Public License (Ms-PL)
// See http://code.msdn.microsoft.com/ManagedMediaHelpers/Project/License.aspx
// All other rights reserved.
// </copyright>
//-----------------------------------------------------------------------
namespace MediaParsersTests
{
    using System;
    using System.IO;
    using System.Net;
    using MediaParsers;
    using Microsoft.VisualStudio.TestTools.UnitTesting;

    [TestClass]
    public class MpegFrameTests : IDisposable
    {
        private static byte[] headerData = new byte[4] { 255, 251, 50, 0 };
        private Stream s = new MemoryStream(MpegFrameTests.headerData);
        private MpegFrame mf;
        private MpegFrame mf2;

        [TestInitialize]
        public void Setup()
        {
            this.s.Position = 0;
            this.mf = new MpegFrame(this.s);

            this.s.Seek(0, SeekOrigin.Begin);
            this.s.Read(headerData, 0, 4);
            this.mf2 = new MpegFrame(this.s, MpegFrameTests.headerData);
        }

        [TestMethod]
        public void Version()
        {
            Assert.AreEqual(1, this.mf.Version);
        }

        [TestMethod]
        public void Layer()
        {
            Assert.AreEqual(3, this.mf.Layer);
        }

        [TestMethod]
        public void IsProtected()
        {
            Assert.AreEqual(false, this.mf.IsProtected);
        }

        [TestMethod]
        public void BitRateIndex()
        {
            Assert.AreEqual(3, this.mf.BitrateIndex);
        }

        [TestMethod]
        public void SamplingrateIndex()
        {
            Assert.AreEqual(0, this.mf.SamplingRateIndex);
        }

        [TestMethod]
        public void Padding()
        {
            Assert.AreEqual(1, this.mf.Padding);
        }

        [TestMethod]
        public void Channels()
        {
            Assert.AreEqual(Channel.Stereo, this.mf.Channels);
        }

        [TestMethod]
        public void Bitrate()
        {
            Assert.AreEqual(48000, this.mf.Bitrate);
        }

        [TestMethod]
        public void SamplingRate()
        {
            Assert.AreEqual(44100, this.mf.SamplingRate);
        }

        [TestMethod]
        public void FrameSize()
        {
            Assert.AreEqual(157, this.mf.FrameSize);
        }

        [TestMethod]
        public void ToStringTest()
        {
            Assert.AreEqual(
                "FrameSize\t157\nBitRate\t48000\nSamplingRate44100\n",
                this.mf.ToString());
        }

        [TestMethod]
        public void FrameHeaderSize()
        {
            Assert.AreEqual(4, MpegFrame.FrameHeaderSize);
        }

        [TestMethod]
        public void CopyHeaderTest()
        {
            byte[] newData = new byte[8];
            this.mf.CopyHeader(newData);
            Assert.AreEqual(255, newData[0]);
            Assert.AreEqual(50, newData[2]);
            Assert.AreEqual(0, newData[4]);
            Assert.AreEqual(0, newData[5]);
        }

        #region IDisposable Members

        public void Dispose()
        {
            this.Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (disposing)
            {
                this.mf = null;
            }

            this.s.Close();
        }
        #endregion
    }
}
