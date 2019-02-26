/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.package 
Module: rtlpkgfile.h 
CreateTime: 2018-9-1 14:25
=========================================================================*/
#ifndef __rtlpkgfile_h__
#define __rtlpkgfile_h__

#include <fstream>
#include "packagepublic.h"
#include "path.h"

RATEL_NAMESPACE_BEGIN
class RATEL_PACKAGE_API PkgFileReader
{
public:
    bool open();
    bool isopened()const { return fs_.is_open(); }
    bool readDBFileData(char*& filedata, uint32_t& datasize);
    bool readDBDataToFile(std::ofstream& os);
    bool readFileData(int32_t offset, uint32_t size, char*& outdata);
    void close();
    PkgFileReader(const Path& filepath);
    ~PkgFileReader();

private:
    void locateDBDataPos();
    bool readTypeId();
    bool readDBDataSize();
    Path pkgpath_;
    std::ifstream fs_;
    int32_t dbdatasize_ = -1;
};
RATEL_NAMESPACE_END
#endif
