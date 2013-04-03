//-----------------------------------------------------------------------
// <copyright file="FindBytePatternTests.cs" company="Larry Olson">
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
    public class FindBytePatternTests
    {
        private byte[] dataArray;

        [TestInitialize]
        public void Setup()
        {
           this.dataArray = new byte[20]
            {
                128, 56, 255, 33, 0,
                48, 101, 45, 97, 1,
                88, 53, 28, 58, 45,
                7, 9, 25, 79, 109
            };
        }

        [TestMethod, ExpectedException(typeof(ArgumentNullException))]
        public void DataNull()
        {
           this.dataArray = null;
            byte[] pattern = new byte[4] { 53, 28, 58, 45 };
            BitTools.FindBytePattern(this.dataArray, pattern);
        }

        [TestMethod, ExpectedException(typeof(ArgumentNullException))]
        public void PatternNull()
        {
            byte[] pattern = new byte[4] { 53, 28, 58, 45 };
            BitTools.FindBytePattern(this.dataArray, null);
        }

        [TestMethod]
        public void Simple()
        {
            byte[] pattern = new byte[4] { 53, 28, 58, 45 };
            int result = BitTools.FindBytePattern(this.dataArray, pattern);
            Assert.AreEqual(11, result);

            pattern = new byte[5] { 53, 28, 58, 45, 7 };
            BitTools.FindBytePattern(this.dataArray, pattern);
            Assert.AreEqual(11, result);
        }

        [TestMethod]
        public void PatternMatchAtBeginning()
        {
            byte[] pattern = new byte[1] { 128 };
            int result = BitTools.FindBytePattern(this.dataArray, pattern);
            Assert.AreEqual(0, result);
        }

        [TestMethod]
        public void PatternMatchAtEnd()
        {
            byte[] pattern = new byte[1] { 109 };
            int result = BitTools.FindBytePattern(this.dataArray, pattern);
            Assert.AreEqual(19, result);
        }

        [TestMethod]
        public void PatrialMatchAtEnd()
        {
            byte[] pattern = new byte[4] { 79, 109, 1, 2 };
            int result = BitTools.FindBytePattern(this.dataArray, pattern);
            Assert.AreEqual(-1, result);
        }

        [TestMethod]
        public void PatternLargerThanData()
        {
            byte[] pattern = new byte[4] { 79, 109, 1, 2 };
            byte[] data = new byte[3] { 79, 109, 1 };
            int result = BitTools.FindBytePattern(this.dataArray, pattern);
            Assert.AreEqual(-1, result);
        }

        [TestMethod]
        public void PatternSameSizeAsArray()
        {
            int result = BitTools.FindBytePattern(this.dataArray, this.dataArray);
            Assert.AreEqual(0, result);
        }

        [TestMethod, ExpectedException(typeof(ArgumentOutOfRangeException))]
        public void StartIndexTooSmall()
        {
            byte[] pattern = new byte[1] { 128 };
            BitTools.FindBytePattern(this.dataArray, pattern, -1);
        }

        [TestMethod, ExpectedException(typeof(ArgumentOutOfRangeException))]
        public void StartIndexTooLarge()
        {
            byte[] pattern = new byte[1] { 128 };
            BitTools.FindBytePattern(this.dataArray, pattern, this.dataArray.Length);
        }

        [TestMethod]
        public void SearchLaterInArray()
        {
           this.dataArray = new byte[4] { 128, 4, 128, 128 };
            byte[] pattern = new byte[1] { 128 };

            int result = BitTools.FindBytePattern(this.dataArray, pattern, 1);
            Assert.AreEqual(2, result);

            result = BitTools.FindBytePattern(this.dataArray, pattern, 3);
            Assert.AreEqual(3, result);
        }
    }
}
