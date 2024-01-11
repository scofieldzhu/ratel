/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: dynamic_library_loader.h  
 *  Copyright (c) 2024-2024 scofieldzhu
 *  
 *  MIT License
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#ifndef __dynamic_library_loader_h__
#define __dynamic_library_loader_h__

#include <string>
#include "ratel/system/ratel_system_export.h"
#include "ratel/basic/ratel_nsp.h"

#ifdef PLATFORM_WIN
    #include "ratel/basic/win_header.h"
    using DLL_HANDLE = HMODULE;
#elif PLATFORM_LINUX
    #include <dlfcn.h>
    using DLL_HANDLE = void*;
#else 
    static_assert(false, "Error: DynamicLibraryLoader implementation not found in unsupport platform!");
#endif

RATEL_NAMESPACE_BEGIN

class RATEL_SYSTEM_API DynamicLibraryLoader 
{
public:
    bool load(const std::string& filename, std::string* err_detail = nullptr);
    template <typename ExportFunc>
    ExportFunc getFunction(const std::string& func_name)const
    {
        #ifdef PLATFORM_WIN
            return reinterpret_cast<ExportFunc>(::GetProcAddress(handle_, func_name.c_str()));
        #elif PLATFORM_LINUX
            return reinterpret_cast<ExportFunc>(dlsym(handle_, func_name.c_str());
        #endif
        return nullptr;
    }
    bool isLoaded()const;
    bool unload(std::string* err_detail = nullptr);
    DynamicLibraryLoader();
    ~DynamicLibraryLoader();

private:        
    DLL_HANDLE handle_ = nullptr;
};

RATEL_NAMESPACE_END

#endif
