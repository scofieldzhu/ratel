/* 
 *  Ratel is a application framework, which provides some convenient librarys
 *  for for those c++ developers pursuing fast-developement.
 *  
 *  File: dynamic_library_loader.cpp 
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

#include "dynamic_library_loader.h"
#include <stdexcept>
#include "spdlog/spdlog.h"

RATEL_NAMESPACE_BEGIN

#define THROW_EXCEPTION_OF_METHOD_NOT_IMPL() throw std::domain_error("This interface implementation not finished at unsupport platform!");

namespace 
{
    std::string GetLastErrorMsg()
    {
    #ifdef PLATFORM_WIN
        DWORD err_id = ::GetLastError();
        if(err_id == 0)
            return "no error!";
        LPSTR msg_buffer = nullptr;
        size_t size = ::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                       NULL, 
                                       err_id, 
                                       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
                                       (LPSTR)&msg_buffer, 
                                       0, 
                                       NULL);
        std::string err_msg(msg_buffer, size);
        LocalFree(msg_buffer);        
    #elif PLATFORM_LINUX
        std::string err_msg;
        if(const char* msg = dlerror())
            err_msg = msg;
    #else
        std::string err_msg("cannot get error from unsupport platform");
    #endif
        return err_msg;
    }
}

DynamicLibraryLoader::DynamicLibraryLoader()
{
}

DynamicLibraryLoader::~DynamicLibraryLoader()
{
    unload();
}   

bool DynamicLibraryLoader::load(const std::string& filename, std::string* err_detail)
{
    unload();
    #ifdef PLATFORM_WIN
        handle_ = ::LoadLibrary(filename.c_str());
    #elif PLATFORM_LINUX
        handle_ = dlopen(filename.c_str(), RTLD_LAZY);
    #else
        THROW_EXCEPTION_OF_METHOD_NOT_IMPL()
    #endif
    if(handle_ == nullptr && err_detail)
        *err_detail = GetLastErrorMsg();
    return handle_ != nullptr;
}

bool DynamicLibraryLoader::isLoaded() const
{
    return handle_ != nullptr;
}

bool DynamicLibraryLoader::unload(std::string *err_detail)
{
    if(!isLoaded())
        return true;
    #ifdef PLATFORM_WIN
    {
        if(::FreeLibrary(handle_)){
            handle_ = nullptr;
            return true;
        }
    }
    #elif PLATFORM_LINUX
    {
        if(dlclose(handle_) == 0){
            handle_ = nullptr;
            return true;
        }
    }            
    #else 
    {
        THROW_EXCEPTION_OF_METHOD_NOT_IMPL()
    }
    #endif
    if(err_detail)
        *err_detail = GetLastErrorMsg();
    return false;
}

RATEL_NAMESPACE_END
