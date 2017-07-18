﻿/*
 * PreparationsControllerTest.cs
 * Test for proper responses after requests.
 * 
   Copyright 2017 Grzegorz Mrukwa, Michał Gallus

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

using System;
using System.Linq;
using NUnit.Framework;
using Spectre.Controllers;
using Spectre.Models.Msi;

namespace Spectre.Tests.Controllers
{
    [TestFixture]
    public class PreparationsControllerTest
    {
        private PreparationsController _controller;

        [SetUp]
        public void SetUp()
        {
            _controller = new PreparationsController();
        }

        [Test]
        public void TestGetListOfPreparations()
        {
            var list = _controller.Get();

            Assert.NotNull(list);
            Assert.IsNotEmpty(list);
            Assert.AreEqual(1, list.Count());
            Assert.AreEqual("Head & neck cancer, patient 1, tumor region only", list.First().Name);
        }

        [Test]
        public void TestGetFirstPreparation()
        {
            var first = _controller.Get(1);

            Assert.NotNull(first);
            Assert.IsInstanceOf<Preparation>(first);
            Assert.AreEqual(1, first.Id);
            Assert.AreEqual("Head & neck cancer, patient 1, tumor region only", first.Name);
        }
        
    }
}
