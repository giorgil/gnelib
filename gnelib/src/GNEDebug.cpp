/* GNE - Game Networking Engine, a portable multithreaded networking library.
 * Copyright (C) 2001 Jason Winnebeck (gillius@mail.rit.edu)
 * Project website: http://www.rit.edu/~jpw9607/
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef _DEBUG

#include "gneintern.h"
#include "GNEDebug.h"
#include "Mutex.h"
#include <ctime>
#include <cstring>

namespace GNE {

static int dbgLevelMask = 0;
static FILE* logFile = NULL;
static Mutex* sync = NULL;
static char* buf = NULL;

bool initDebug(int levelMask, const char* fileName) {
	dbgLevelMask = levelMask;
	sync = new Mutex();

	buf = new char[512];
	if (fileName == NULL) {
		time_t now = time (NULL);
		struct tm *t = localtime (&now);
		strftime (buf, 100, "gne%H'%M'%S.log", t);
	} else
		strcpy(buf, fileName);

	logFile = fopen(buf, "wt");
	if (!logFile)
		return true;
	else
		return false;
}

void killDebug() {
	if (logFile != NULL)
		fclose(logFile);
	delete[] buf;
	delete sync;

	logFile = NULL;
	buf = NULL;
	sync = NULL;
}

void doTrace(int level, const char* fn, int lineno, const char* msg, ...) {
	if (((dbgLevelMask & DLEVEL1) && level == 1) ||
		  ((dbgLevelMask & DLEVEL2) && level == 2) ||
			((dbgLevelMask & DLEVEL3) && level == 3) ||
			((dbgLevelMask & DLEVEL4) && level == 4) ||
			((dbgLevelMask & DLEVEL5) && level == 5)) {
		va_list arg;  
		va_start(arg, msg);

		sync->acquire();
		vsprintf(buf, msg, arg);

		//Remove the path to the file, to conserve line width.
		char* temp = strrchr(fn, '\\'); //Try Microsoft style path
		if (temp == NULL) {
			temp = strrchr(fn, '/');      //Try UNIX style path
			if (temp == NULL)
				temp = buf;                 //If all else fails...
			else
				temp++;
		} else
			temp++;
		fprintf(logFile, "%30s, line %4i: %s\n", temp, lineno, buf);
		fflush(logFile); //Try to be resiliant to errors.
		sync->release();

		va_end(arg);
	}
}

}

#endif //_DEBUG