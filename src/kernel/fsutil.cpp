/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.kernel 
Module: fsutil.cpp 
CreateTime: 2019-3-17 20:32
=========================================================================*/
#include "fsutil.h"
#include "kernellogger.h"

RATEL_NAMESPACE_BEGIN
namespace fsutil{
	bool TransformDataBlock(std::ifstream& srcfs, std::ofstream& dstfs, uint32_t totalbytesize, uint32_t buffersize)
	{
		char* databuffer = new char[buffersize];
		memset(databuffer, 0, buffersize);
		uint32_t curdigestion = 0;
		int32_t leftover = 0, needreadbytes = 0;
		while(curdigestion < totalbytesize){
			leftover = totalbytesize - curdigestion;
			if(leftover <= 0) //realdy finished!
				break;
			needreadbytes = (leftover >= buffersize ? buffersize : leftover);
			srcfs.read(databuffer, needreadbytes);
			if(srcfs.gcount() < needreadbytes){//some exception error            
				if(srcfs.eof())
					slog_err(kernellogger) << "src file data is not enough!" << endl;
				dstfs.write(databuffer, srcfs.gcount());
				dstfs.flush();
				delete[] databuffer; //release resource
				return false;
			}
			dstfs.write(databuffer, needreadbytes);
			dstfs.flush();
			curdigestion += needreadbytes;
		};
		delete[] databuffer; //release resource
		return true;
	}

	bool TransformDataBlock(HANDLE srcfh, HANDLE dstfh, uint32_t totalbytesize, uint32_t buffersize)
	{
		if(srcfh == nullptr || dstfh == nullptr)
			return false;
		char* databuffer = new char[buffersize];
		memset(databuffer, 0, buffersize);
		int32_t leftover = 0;
		DWORD curdigestion = 0, needreadbytes = 0;		
		while(curdigestion < totalbytesize){
			leftover = totalbytesize - curdigestion;
			if(leftover <= 0) //realdy finished!
				break;
			needreadbytes = (leftover >= buffersize ? buffersize : leftover);
			DWORD actualbytestoread = 0;
			::ReadFile(srcfh, databuffer, needreadbytes, &actualbytestoread, nullptr);    
			if(actualbytestoread < needreadbytes){//some exception error            
				::WriteFile(dstfh, databuffer, actualbytestoread, nullptr, nullptr);
				::FlushFileBuffers(dstfh);
				delete[] databuffer; //release resource
				return false;
			}
			::WriteFile(dstfh, databuffer, actualbytestoread, nullptr, nullptr);
			::FlushFileBuffers(dstfh);
			curdigestion += needreadbytes;
		};
		delete[] databuffer; //release resource
		return true;
	}
}
RATEL_NAMESPACE_END