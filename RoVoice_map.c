/*

 RoverVoice - portable TTS library in C

 Copyright (C) 2020 Tim K/RoverAMD <timprogrammer@rambler.ru>
 
 Permission to use, copy, modify, and/or distribute this software 
 for any purpose with or without fee is hereby granted.

 THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL 
 WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED 
 WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL 
 THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CON-
 SEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS 
 OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLI-
 GENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION 
 WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 
*/

#define ROVERVOICE_ACCESS_PRIVATE
#include "RoVoice_uncross.h"

struct RoverExceptionMap_s {
	unsigned totalCount;
	unsigned iteratorIndex;
	char** keys;
	char* values;
};

RoverExceptionMap* RoverExceptionMapContinuePreprocessing(char* ctnts) {
	if (!ctnts)
		return NULL;
	RoverExceptionMap* mp = malloc(sizeof(RoverExceptionMap));
	const char delimiter = '\n';
	const char phnmDelimiter = '@';
	mp->totalCount = 0;
	mp->iteratorIndex = 0;
	mp->keys = calloc(strlen(ctnts) + 1, sizeof(char*));
	mp->values = calloc(strlen(ctnts) + 1, sizeof(char));
	char* bufTmp = NULL;
	for (unsigned i = 0; i < strlen(ctnts); i++) {
		if (!bufTmp)
			bufTmp = calloc(strlen(ctnts) + 1, sizeof(char));
		const char currentChar = ctnts[i];
		if (currentChar == delimiter) {
			if (strlen(bufTmp) >= 1) {
				mp->values[mp->iteratorIndex] = (i >= 1) ? ctnts[i - 1] : 'a';
				if (!mp->keys[mp->iteratorIndex])
					return NULL;
			}
			mp->iteratorIndex += 1;
			mp->totalCount += 1;
			free(bufTmp);
			bufTmp = NULL;
			continue;
		} else if (currentChar == phnmDelimiter) {
			mp->keys[mp->iteratorIndex] = rvstrdup(bufTmp);
			continue;
		}
		bufTmp[strlen(bufTmp)] = currentChar;
	}
	mp->iteratorIndex = 0;
	return mp;
}

RoverExceptionMap* RoverExceptionMapCreateFromFile(const char* path) {
	if (!path || !RoverVoiceFileExists(path))
		return NULL;
	FILE* fdesc = fopen(path, "r");
	if (!fdesc)
		return NULL;
	fseek(fdesc, 0L, SEEK_END);
	size_t size = ftell(fdesc);
	rewind(fdesc);
	char* buf = calloc(size + 1, sizeof(char));
	fread(buf, sizeof(char), size, fdesc);
	fclose(fdesc);
	return RoverExceptionMapContinuePreprocessing(buf);
}

bool RoverExceptionMapIterate(RoverExceptionMap* mp, char** exc, char* correctLetter) {
	if (!mp)
		return false;
	if (mp->iteratorIndex >= mp->totalCount)
		return false;
	if (exc)
		*exc = mp->keys[mp->iteratorIndex];
	if (correctLetter)
		*correctLetter = mp->values[mp->iteratorIndex];
	mp->iteratorIndex += 1;
	return true;
}

bool RoverExceptionMapRewind(RoverExceptionMap* mp) {
	if (!mp)
		return false;
	mp->iteratorIndex = 0;
	return true;
}

void RoverExceptionMapRelease(RoverExceptionMap* mp) {
	if (!mp)
		return;
	free(mp);
}