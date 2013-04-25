//-----------------------------------------------------------------------
// <copyright file="MaskBitsTests.cs" company="Larry Olson">
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
    public class MaskBitsTests
    {
        private byte[] dataArray;

        [TestInitialize]
        public void SetupTests()
        {
           this.dataArray = new byte[36]
            {
                79,  128, 28,  13,
                59,    0, 128, 128,
                128, 128, 128, 128,
                128, 128, 128, 128,
                128, 128, 128, 128,
                128, 128, 128, 128,
                128, 128, 128, 128,
                128, 128, 128, 128,
                128, 128, 128, 128
            };
        }

        [TestMethod]
        public void SimpleTest()
        {
            int result = BitTools.MaskBits(this.dataArray, 16, 8);
            Assert.AreEqual(28, result);
        }

        [TestMethod]
        public void LowerEdgeArrayTest()
        {
            int result = BitTools.MaskBits(this.dataArray, 0, 8);
            Assert.AreEqual(79, result);
        }

        [TestMethod]
        public void UpperEdgeOfArrayTest()
        {
            int result = BitTools.MaskBits(this.dataArray, 40, 8);
            Assert.AreEqual(0, result);
        }

        [TestMethod]
        public void TooSmallSizeTest()
        {
            int result;
            
            try
            {
                result = BitTools.MaskBits(this.dataArray, 0, 0);
            }
            catch (ArgumentException)
            {
                Assert.AreEqual(1, 1);
            }
            catch (Exception)
            {
                Assert.Fail("Unexpected Exception");
            }

            try
            {
                result = BitTools.MaskBits(this.dataArray, 0, -1);
            }
            catch (ArgumentException)
            {
                Assert.AreEqual(1, 1);
            }
            catch (Exception)
            {
                Assert.Fail("Unexpected Exception");
            }

            try
            {
                result = BitTools.MaskBits(this.dataArray, 0, -8);
            }
            catch (ArgumentException)
            {
                Assert.AreEqual(1, 1);
            }
            catch (Exception)
            {
                Assert.Fail("Unexpected Exception");
            }
        }

        [TestMethod, ExpectedException(typeof(ArgumentException))]
        public void TooLargeSizeTest()
        {
            int result = BitTools.MaskBits(this.dataArray, 0, 33);
        }

        [TestMethod]
        public void InvalidFirstBitTest()
        {
            int result;
            try
            {
                // TOO Small
                result = BitTools.MaskBits(this.dataArray, -1, 8);
            }
            catch (ArgumentException)
            {
                Assert.AreEqual(1, 1);
            }
            catch (Exception)
            {
                Assert.Fail("Unexpected Exception");
            }

            try
            {
                // TOO Large
                result = BitTools.MaskBits(this.dataArray, 48, 8);
            }
            catch (ArgumentException)
            {
                Assert.AreEqual(1, 1);
            }
            catch (Exception)
            {
                Assert.Fail("Unexpected Exception");
            } 
        }

        [TestMethod, ExpectedException(typeof(ArgumentException))]
        public void InvalidLastBitTest()
        {
            int result = BitTools.MaskBits(this.dataArray, 287, 2);
        }

        [TestMethod]
        public void MaskOutFullSizedAlignedBitsTest()
        {
            int result = BitTools.MaskBits(this.dataArray, 240, 32);
            Assert.AreEqual(-2139062144, result);
            
            result = BitTools.MaskBits(this.dataArray, 8, 32);
            Assert.AreEqual(-2145645253, result);

            result = BitTools.MaskBits(this.dataArray, 16, 32);
            Assert.AreEqual(470629120, result);
        }

        [TestMethod]
        public void MaskOutFullSizeNonAlignedBitsTest()
        {
            int result = BitTools.MaskBits(this.dataArray, 4, 32);
            Assert.AreEqual(-134102829, result);
        }

        [TestMethod, ExpectedException(typeof(ArgumentNullException))]
        public void NullDataTest()
        {
           this.dataArray = null;
            BitTools.MaskBits(this.dataArray, 0, 8);
        }
    }
}
