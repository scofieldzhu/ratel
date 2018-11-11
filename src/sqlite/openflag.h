/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: openflag.h 
CreateTime: 2018-11-11 19:06
=========================================================================*/
#ifndef __openflag_h__
#define __openflag_h__

#include "sqlitepublic.h"

RATEL_NAMESPACE_BEGIN

/*
** CAPI3REF: Flags For File Open Operations
**
** These bit values are intended for use in the
** 3rd parameter to the [sqlite3_open_v2()] interface and
** in the 4th parameter to the [sqlite3_vfs.xOpen] method.
*/
#define RATEL_DB_OPEN_READONLY         0x00000001  /* Ok for sqlite3_open_v2() */
#define RATEL_DB_OPEN_READWRITE        0x00000002  /* Ok for sqlite3_open_v2() */
#define RATEL_DB_OPEN_CREATE           0x00000004  /* Ok for sqlite3_open_v2() */
#define RATEL_DB_OPEN_DELETEONCLOSE    0x00000008  /* VFS only */
#define RATEL_DB_OPEN_EXCLUSIVE        0x00000010  /* VFS only */
#define RATEL_DB_OPEN_AUTOPROXY        0x00000020  /* VFS only */
#define RATEL_DB_OPEN_URI              0x00000040  /* Ok for sqlite3_open_v2() */
#define RATEL_DB_OPEN_MEMORY           0x00000080  /* Ok for sqlite3_open_v2() */
#define RATEL_DB_OPEN_MAIN_DB          0x00000100  /* VFS only */
#define RATEL_DB_OPEN_TEMP_DB          0x00000200  /* VFS only */
#define RATEL_DB_OPEN_TRANSIENT_DB     0x00000400  /* VFS only */
#define RATEL_DB_OPEN_MAIN_JOURNAL     0x00000800  /* VFS only */
#define RATEL_DB_OPEN_TEMP_JOURNAL     0x00001000  /* VFS only */
#define RATEL_DB_OPEN_SUBJOURNAL       0x00002000  /* VFS only */
#define RATEL_DB_OPEN_MASTER_JOURNAL   0x00004000  /* VFS only */
#define RATEL_DB_OPEN_NOMUTEX          0x00008000  /* Ok for sqlite3_open_v2() */
#define RATEL_DB_OPEN_FULLMUTEX        0x00010000  /* Ok for sqlite3_open_v2() */
#define RATEL_DB_OPEN_SHAREDCACHE      0x00020000  /* Ok for sqlite3_open_v2() */
#define RATEL_DB_OPEN_PRIVATECACHE     0x00040000  /* Ok for sqlite3_open_v2() */
#define RATEL_DB_OPEN_WAL              0x00080000  /* VFS only */

RATEL_NAMESPACE_END

#endif
