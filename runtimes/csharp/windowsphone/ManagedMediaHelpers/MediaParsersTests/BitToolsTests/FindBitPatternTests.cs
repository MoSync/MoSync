//-----------------------------------------------------------------------
// <copyright file="FindBitPatternTests.cs" company="Larry Olson">
// (c) Copyright Larry Olson.
// This source is subject to the Microsoft Public License (Ms-PL)
// See http://code.msdn.microsoft.com/ManagedMediaHelpers/Project/License.aspx
// All other rights reserved.
// </copyright>
//-----------------------------------------------------------------------
namespace MediaParsersTests.BitToolsTests
{
    using System;
    using System.Net;
    using MediaParsers;
    using Microsoft.VisualStudio.TestTools.UnitTesting;

    [TestClass]
    public class FindBitPatternTests
    {
        private byte[] dataArray;
        private byte[] mask;
        private byte[] pattern;

        [TestInitialize]
        public void Setup()
        {
           this.dataArray = new byte[20]
            {
                128, 56, 255, 33, 0,
                48, 101, 45, 97, 1,
                88, 53, 97, 58, 45,
                7, 9, 25, 79, 109
            };
        }

        [TestMethod, ExpectedException(typeof(ArgumentNullException))]
        public void DataNull()
        {
            this.pattern = new byte[4] { 10, 10, 10, 10 };
            this.mask = new byte[4] { 10, 10, 10, 10 };
            BitTools.FindBitPattern(null, this.pattern, this.mask);
        }

        [TestMethod, ExpectedException(typeof(ArgumentNullException))]
        public void PatternNull()
        {
            BitTools.FindBitPattern(this.dataArray, null, this.mask);
        }

        [TestMethod, ExpectedException(typeof(ArgumentNullException))]
        public void MaskNull()
        {
            BitTools.FindBitPattern(this.dataArray, this.pattern, null);
        }

        [TestMethod]
        public void MaskAndPatternAreDifferentSizes()
        {
            this.mask = new byte[4] { 128, 128, 128, 128 };
            this.pattern = new byte[3] { 36, 0, 72 };
            int result = BitTools.FindBitPattern(this.dataArray, this.pattern, this.mask);
            Assert.AreEqual(-1, result);

            this.pattern = new byte[5] { 7, 9, 25, 79, 109 };
            result = BitTools.FindBitPattern(this.dataArray, this.pattern, this.mask);
            Assert.AreEqual(-1, result);
        }

        [TestMethod]
        public void PatternLargerThanData()
        {
            this.dataArray = new byte[5] { 1, 2, 3, 4, 5 };
            this.mask = new byte[6] { 128, 128, 128, 128, 128, 128 };
            this.pattern = new byte[6] { 1, 2, 3, 4, 5, 6 };
            int result = BitTools.FindBitPattern(this.dataArray, this.pattern, this.mask);
            Assert.AreEqual(-1, result);
        }

        [TestMethod]
        public void EmptyPattern()
        {
            this.mask = new byte[4] { 128, 128, 128, 128 };
            int result = BitTools.FindBitPattern(this.dataArray, new byte[0] { }, this.mask);
            Assert.AreEqual(-1, result);
        }

        [TestMethod]
        public void EmptyData()
        {
            this.mask = new byte[4] { 128, 128, 128, 128 };
            this.pattern = new byte[4] { 128, 128, 128, 128 };
            int result = BitTools.FindBitPattern(new byte[0] { }, this.pattern, this.mask);
            Assert.AreEqual(-1, result);
        }

        [TestMethod]
        public void EmptyMask()
        {
            this.pattern = new byte[4] { 128, 128, 128, 128 };
            int result = BitTools.FindBitPattern(this.dataArray, this.pattern, new byte[0] { });
            Assert.AreEqual(-1, result);
        }

        [TestMethod]
        public void PatternIsData()
        {
            this.dataArray = new byte[4] { 1, 2, 3, 4 };
            this.mask = new byte[4] { 255, 255, 255, 255 };
            this.pattern = new byte[4] { 1, 2, 3, 4 };
            int result = BitTools.FindBitPattern(this.dataArray, this.pattern, this.mask);
            Assert.AreEqual(0, result);
        }

        [TestMethod]
        public void PartialPattern()
        {
            byte[] localDataArray = new byte[20]
            {
                0, 0, 0, 1, 0,
                48, 101, 45, 97, 1,
                88, 53, 97, 58, 45,
                0, 0, 0, 1, 109
            };
            this.mask = new byte[3] { 255, 255, 255 };
            this.pattern = new byte[3] { 0, 0, 1 };
            int result = BitTools.FindBitPattern(localDataArray, this.pattern, this.mask);
            Assert.AreEqual(1, result);
        }

        [TestMethod]
        public void PatternAtEnd()
        {
            this.mask = new byte[1] { 255 };
            this.pattern = new byte[1] { 109 };
            int result = BitTools.FindBitPattern(this.dataArray, this.pattern, this.mask);
            Assert.AreEqual(19, result);
        }

        [TestMethod]
        public void PatternAtBeginning()
        {
            this.mask = new byte[1] { 255 };
            this.pattern = new byte[1] { 128 };
            int result = BitTools.FindBitPattern(this.dataArray, this.pattern, this.mask);
            Assert.AreEqual(0, result);
        }

        [TestMethod]
        public void PatternCrossingByteBoundary()
        {
            this.pattern = new byte[3] { 0, 24, 240 };
            this.mask = new byte[3] { 15, 223, 240 };
            int result = BitTools.FindBitPattern(this.dataArray, this.pattern, this.mask);
            Assert.AreEqual(0, result);
        }

        [TestMethod, ExpectedException(typeof(ArgumentOutOfRangeException))]
        public void StartIndexTooSmall()
        {
            this.pattern = new byte[1] { 128 };
            this.mask = new byte[1] { 1 };
            BitTools.FindBitPattern(this.dataArray, this.pattern, this.mask, -1);
        }

        [TestMethod, ExpectedException(typeof(ArgumentOutOfRangeException))]
        public void StartIndexTooLarge()
        {
            this.pattern = new byte[1] { 128 };
            this.mask = new byte[1] { 1 };
            BitTools.FindBitPattern(this.dataArray, this.pattern, this.mask, this.dataArray.Length);
        }

        [TestMethod]
        public void SearchLaterInArray()
        {
            int result = BitTools.FindBitPattern(this.dataArray, new byte[1] { 1 }, new byte[1] { 1 }, 7);
            Assert.AreEqual(7, result);

            result = BitTools.FindBitPattern(this.dataArray, new byte[1] { 1 }, new byte[1] { 1 }, 13);
            Assert.AreEqual(14, result);
        }
    }
}
