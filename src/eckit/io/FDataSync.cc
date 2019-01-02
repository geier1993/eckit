/*
 * (C) Copyright 1996- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "FDataSync.h"

#include <unistd.h>

#include "eckit/eckit.h"

namespace eckit {

int fdatasync(int fd)
{
#if defined(ECKIT_HAVE_FDATASYNC)
    // usually available on Linux, but not Darwin (macosx) and xBSD
    // syncs all the data but avoids some of the metadata e.g. mtime
    int ret = ::fdatasync(fd);
#elif defined(ECKIT_HAVE_F_FULLSYNC)
    // usually available on Darwin (macosx) and xBSD
    // provides stronger guarantees than fsync that data fully committed to persistent storage
    int ret = ::fcntl(fd, F_FULLFSYNC);
#elif defined(ECKIT_HAVE_FSYNC)
    // last resort, but note this is slower than ::fdatasync and less strong as F_FULLSYNC
    int ret = ::fsync(fd);
#else
# error "Operating system does not support fdatasync, F_FULLSYNC or fsync"
#endif
    return ret;
}

}  // namespace eckit
