/*=======================================================================
Ratel is a application framework, which provides some convenient librarys
for for those c++ developers pursuing fast-developement.
Copyright (c) scofieldzhu. All rights reserved.	

Project: ratel.sqlite 
Module: rescode.h 
CreateTime: 2018-11-11 18:55
=========================================================================*/
#ifndef __rescode_h__
#define __rescode_h__

#include "sqlitepublic.h"

RATEL_NAMESPACE_BEGIN

/*
** CAPI3REF: Result Codes
** KEYWORDS: {result code definitions}
**
** Many SQLite functions return an integer result code from the set shown
** here in order to indicate success or failure.
**
** New error codes may be added in future versions of SQLite.
**
** See also: [extended result code definitions]
*/

enum ResultCode 
{
    RESCODE_OK = 0,   /* Successful result */
    /* beginning-of-error-codes */
    RESCODE_ERROR,           /* Generic error */
    RESCODE_INTERNAL,        /* Internal logic error in sqlite3 */
    RESCODE_PERM,            /* Access permission denied */
    RESCODE_ABORT,           /* Callback routine requested an abort */
    RESCODE_BUSY,            /* The database file is locked */
    RESCODE_LOCKED,          /* A table in the database is locked */
    RESCODE_NOMEM,           /* A malloc() failed */
    RESCODE_READONLY,        /* Attempt to write a readonly database */
    RESCODE_INTERRUPT,       /* Operation terminated by sqlite3_interrupt()*/
    RESCODE_IOERR,          /* Some kind of disk I/O error occurred */
    RESCODE_CORRUPT,        /* The database disk image is malformed */
    RESCODE_NOTFOUND,       /* Unknown opcode in sqlite3_file_control() */
    RESCODE_FULL,           /* Insertion failed because database is full */
    RESCODE_CANTOPEN,       /* Unable to open the database file */
    RESCODE_PROTOCOL,       /* Database lock protocol error */
    RESCODE_EMPTY,          /* Internal use only */
    RESCODE_SCHEMA,         /* The database schema changed */
    RESCODE_TOOBIG,         /* String or BLOB exceeds size limit */
    RESCODE_CONSTRAINT,     /* Abort due to constraint violation */
    RESCODE_MISMATCH,       /* Data type mismatch */
    RESCODE_MISUSE,         /* Library used incorrectly */
    RESCODE_NOLFS,          /* Uses OS features not supported on host */
    RESCODE_AUTH,           /* Authorization denied */
    RESCODE_FORMAT,         /* Not used */
    RESCODE_RANGE,          /* 2nd parameter to sqlite3_bind out of range */
    RESCODE_NOTADB,         /* File opened that is not a database file */
    RESCODE_NOTICE,         /* Notifications from sqlite3_log() */
    RESCODE_WARNING,        /* Warnings from sqlite3_log() */
    RESCODE_ROW = 100,      /* sqlite3_step() has another row ready */
    RESCODE_DONE            /* sqlite3_step() has finished executing */
};

RATEL_NAMESPACE_END

#endif
