/*-------------------------------------------------------------------------
 *
 * fetch.c
 *	  Functions for fetching files from a local or remote data dir
 *
 * This file forms an abstraction of getting files from the "source".
 * There are two implementations of this interface: one for copying files
 * from a data directory via normal filesystem operations (copy_fetch.c),
 * and another for fetching files from a remote server via a libpq
 * connection (libpq_fetch.c)
 *
 *
 * Portions Copyright (c) 1996-2012, PostgreSQL Global Development Group
 * Portions Copyright (c) 2013 VMware, Inc. All Rights Reserved.
 *
 *-------------------------------------------------------------------------
 */
#include "postgres_fe.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "pg_rewind.h"
#include "fetch.h"
#include "filemap.h"

void
fetchRemoteFileList(void)
{
	if (datadir_source)
		traverse_datadir(datadir_source, &process_remote_file);
	else
		libpqProcessFileList();
}

/*
 * Fetch all relation data files that are marked in the given data page map.
 */
void
executeFileMap(void)
{
	if (datadir_source)
		copy_executeFileMap(filemap);
	else
		libpq_executeFileMap(filemap);
}

/*
 * Fetch a single file into a malloc'd buffer. The file size is returned
 * in *filesize. The returned buffer is always zero-terminated.
 */
char *
fetchFile(char *filename, size_t *filesize)
{
	if (datadir_source)
		return slurpFile(datadir_source, filename, filesize);
	else
		return libpqGetFile(filename, filesize);
}
