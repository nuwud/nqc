/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * The Initial Developer of this code is David Baum.
 * Portions created by David Baum are Copyright (C) 1999 David Baum.
 * All Rights Reserved.
 *
 * Portions created by John Hansen are Copyright (C) 2005 John Hansen.
 * All Rights Reserved.
 *
 */
#ifdef PDEBUG

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "PDebug.h"

static const char* get_leafname(const char *filename);
FILE* get_logfile();
bool open_logfile(char *filename);

static FILE* LogFile;
#define BUFSIZE 512


void _p_assert(const char* file, int line)
{
	char buffer[BUFSIZE];
	snprintf(buffer, BUFSIZE,
        "PASSERT() failed line #%d in file %s",
        line, get_leafname(file));

	_p_debugstr(const_cast<char *>(buffer));
}

void _p_require(const char* file, int line, const char* label)
{
	char buffer[BUFSIZE];
	snprintf(buffer, BUFSIZE,
        "PREQUIRE %s failed line #%d in file %s",
        label, line, get_leafname(file));

	_p_debugstr(const_cast<char *>(buffer));
}


void _p_requirenot(const char* file, int line, int val, const char* label)
{
	char buffer[BUFSIZE];
	snprintf(buffer, BUFSIZE,
        "PREQUIRENOT %s failed line #%d in file %s, value = %d",
		label, line, get_leafname(file), val);

	_p_debugstr(const_cast<char *>(buffer));
}


const char* get_leafname(const char *filename)
{
	const char *leaf = filename;
	const char *ptr = filename;

	while(*ptr != 0) {
		if (*ptr == ':')
			leaf = ptr+1;
		ptr++;
	}

	return leaf;
}


FILE* get_logfile()
{
    if (LogFile) {
        return LogFile;
    }

    char *filename = getenv(NCQ_LOGFILE);
    if (filename && open_logfile(filename)) {
        return LogFile;
    } else {
        return stderr;
    }
}


bool open_logfile(char *filename)
{
    FILE * file;
    if (filename && (file = fopen(filename, "w"))) {
        LogFile = file;
        return true;
    }

    return false;
}

#ifndef _p_debugstr

// TODO: this is lame. We should use a stream.
void _p_debugstr(const char *text)
{
	fprintf(get_logfile(), "DBG: %s\n", text);
    fflush(get_logfile());
}
#endif

#ifndef _p_debugvar

void _p_debugvar(const char *varname, const int var)
{
    char buffer[BUFSIZE];
    snprintf(buffer, BUFSIZE, "%s = %d", varname, var);

    _p_debugstr(const_cast<char *>(buffer));
}

#endif

#endif
