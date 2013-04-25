//-----------------------------------------------------------------------
// <copyright file="ConvertToSyncSafeInt32Tests.cs" company="Larry Olson">
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
    public class ConvertToSyncSafeInt32Tests
    {
        private byte[] dataArray;

        [TestInitialize]
        public void SetupTests()
        {
           this.dataArray = new byte[6] { 79, 13, 28, 0, 59, 128 };
        }

        [TestMethod]
        public void Simple()
        {
            int result = BitTools.ConvertSyncSafeToInt32(this.dataArray, 1);
            Assert.AreEqual(27721787, result);
        }

        [TestMethod]
        public void LargestValidSyncSafeIntValue()
        {
           this.dataArray = new byte[4] { 127, 127, 127, 127 };
            int result = BitTools.ConvertSyncSafeToInt32(this.dataArray, 0);
            Assert.AreEqual(268435455, result);
        }

        [TestMethod, ExpectedException(typeof(ArgumentException))]
        public void TooSmallData()
        {
           this.dataArray = new byte[2] { 128, 64 };
            int result = BitTools.ConvertSyncSafeToInt32(this.dataArray, 0);
        }

        [TestMethod, ExpectedException(typeof(ArgumentOutOfRangeException))]
        public void InvalidStartIndexTooSmall()
        {
            int result = BitTools.ConvertSyncSafeToInt32(this.dataArray, -1);
        }

        [TestMethod, ExpectedException(typeof(ArgumentOutOfRangeException))]
        public void InvalidStartIndexTooBig()
        {
            int result = BitTools.ConvertSyncSafeToInt32(this.dataArray, this.dataArray.Length);
        }

        [TestMethod]
        public void InvalidData()
        {
            ////dataArray = new byte[4] { 128, 128, 128, 128 };
            ////int result = BitTools.ConvertSyncSafeToInt32(dataArray, 0);
            ////Assert.Inconclusive();
            /*
             * Code currently does not check for this but could be added for extra
             * robustness
             * 
             * Clamp or throw
             */
        }

        [TestMethod]
        public void InvalidStartIndexTooCloseToEndOfArray()
        {
            int result;
            try
            {
                result = BitTools.ConvertSyncSafeToInt32(this.dataArray, 2);
            }
            catch (Exception)
            {
                Assert.Fail();
            }

            try
            {
                result = BitTools.ConvertSyncSafeToInt32(this.dataArray, 3);
            }
            catch (ArgumentOutOfRangeException)
            {
            }
            catch (Exception)
            {
                Assert.Fail();
            }
        }

        [TestMethod, ExpectedException(typeof(ArgumentNullException))]
        public void NullData()
        {
           this.dataArray = null;
            BitTools.ConvertSyncSafeToInt32(null, 0);
        }
    }
}
