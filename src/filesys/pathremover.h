/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.filesys 
Module: pathremover.h 
CreateTime: 2018-12-23 21:46
=========================================================================*/
#ifndef __pathremover_h__
#define __pathremover_h__

#include "path.h"

RATEL_NAMESPACE_BEGIN
class RATEL_FILESYS_API PathRemover
{
public:
    void perform(const Path& srcpath, bool remsubifdir = true);
    PathRemover();
    ~PathRemover();

private:
    void removeDir(const Path& sourcedir, bool remsubifdir);
};
RATEL_NAMESPACE_END

#endif
