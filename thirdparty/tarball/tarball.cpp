// From https://github.com/lindenb/cclindenb/blob/master/src/core/lindenb/io/tarball.cpp
/*
 * tarball.cpp
 *
 *  Created on: Jul 28, 2010
 *      Author: Pierre Lindenbaum PhD
 *              plindenbaum@yahoo.fr
 *              http://plindenbaum.blogspot.com
 *     License: Apache License, Version 2.0 (as communicated to Adam Tenderholt by Pierre
 *               Lindenbaum PhD)
 *
 * Modified by: Adam Tenderholt
 *
 *              + Moved PosixTarHeader to seperate header file
 *              + Renamed LOCALNS::Tar::_checksum() to Tar::headerChecksum()
 *              + Modify headerChecksum() to not automatically store the checksum in the
 *                header, but instead returns it as a uint.
 *              + Removed remaining code
 */

#include "tarball.h"

namespace Tar
{
  unsigned int headerChecksum(void* header)
  {
    unsigned int sum = 0;
    char *p = static_cast<char *>(header);
    char *q = p + sizeof(PosixTarHeader);

    while (p < static_cast<PosixTarHeader*>(header)->checksum)
    {
      sum += *p++ & 0xff;
    }

    for (int i = 0; i < 8; ++i)
    {
      sum += ' ';
      ++p;
    }

    while (p < q)
    {
      sum += *p++ & 0xff;
    }

    return sum;

  }
}
