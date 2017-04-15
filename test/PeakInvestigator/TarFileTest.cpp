// --------------------------------------------------------------------------
//  PeakInvestigator-API -- C++ library for accessing the public API of
//                              PeakInvestigator.
// --------------------------------------------------------------------------
// Copyright Veritomyx, Inc. 2016.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Adam Tenderholt $
// $Author: Adam Tenderholt $
// --------------------------------------------------------------------------
//
//

#include <sstream>
#include <fstream>

#include <PeakInvestigator/TarFile.h>
#include "TarFileTest_config.h"
#include "gtest/gtest.h"

using namespace Veritomyx::PeakInvestigator;

TEST(TarFile, WritesFromString)
{
  TarFile tarfile("test1.tar.gz", SAVE);
  std::stringstream out1;
  out1 << "Test" << " " << 1;
  std::stringstream out2;
  out2 << "Test" << " " << 2;
  std::stringstream out3;
  out3 << "Test" << " " << 3;

  ASSERT_NO_THROW(tarfile.writeFile("test1.txt", out1));
  ASSERT_NO_THROW(tarfile.writeFile("test2.txt", out2));
  ASSERT_NO_THROW(tarfile.writeFile("test3.txt", out3));
  ASSERT_NO_THROW(tarfile.close());

  TarFile tarfile2("test1.tar.gz", LOAD);
  std::stringstream in1, in2, in3;

  std::string name1 = tarfile2.readNextFile(in1);
  ASSERT_STREQ("test1.txt", name1.c_str());
  ASSERT_STREQ(out1.str().c_str(), in1.str().c_str());

  std::string name2 = tarfile2.readNextFile(in2);
  ASSERT_STREQ("test2.txt", name2.c_str());
  ASSERT_STREQ(out2.str().c_str(), in2.str().c_str());

  std::string name3 = tarfile2.readNextFile(in3);
  ASSERT_STREQ("test3.txt", name3.c_str());
  ASSERT_STREQ(out3.str().c_str(), in3.str().c_str());
}

TEST(TarFile, WritesFromFile)
{
  TarFile tarfile("test2.tar.gz", SAVE);
  std::fstream fout1(GET_TEST_DATA_PATH("scan1.txt"));
  std::fstream fout2(GET_TEST_DATA_PATH("scan2.txt"));

  std::stringstream out1;
  out1 << fout1.rdbuf();
  fout1.seekg(0);

  std::stringstream out2;
  out2 << fout2.rdbuf();
  fout2.seekg(0);

  ASSERT_NO_THROW(tarfile.writeFile("scan1.txt", fout1));
  ASSERT_NO_THROW(tarfile.writeFile("scan2.txt", fout2));
  ASSERT_NO_THROW(tarfile.close());

  TarFile tarfile2("test2.tar.gz", LOAD);
  std::stringstream in1, in2;

  std::string name1 = tarfile2.readNextFile(in1);
  ASSERT_STREQ("scan1.txt", name1.c_str());
  ASSERT_STREQ(out1.str().c_str(), in1.str().c_str());

  std::string name2 = tarfile2.readNextFile(in2);
  ASSERT_STREQ("scan2.txt", name2.c_str());
  ASSERT_STREQ(out2.str().c_str(), in2.str().c_str());
}

TEST(TarFile, OvaryRegression)
{
  TarFile tarfile(GET_TEST_DATA_PATH("Ovary_PS.broken.tar.gz"), LOAD);
  std::stringstream in1, in2;

  std::string name1 = tarfile.readNextFile(in1);
  ASSERT_STREQ("scan00478.mass_list.txt", name1.c_str());

  std::string name2 = tarfile.readNextFile(in2);
  ASSERT_STREQ("scan00177.mass_list.txt", name2.c_str());
}
