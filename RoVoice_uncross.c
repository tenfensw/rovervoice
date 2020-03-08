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

#ifndef _WIN32
#ifndef __APPLE__
#include <glib.h>
#include <canberra.h>

static ca_context** RoverVoiceCanberraContext = NULL;
#endif

static const char RoverVoicePathDelimiter = '/';
#else
#include <windows.h>

static const char RoverVoicePathDelimiter = '\\';
#endif

char* rvstrdup(const char* orig) {
	if (!orig)
		return NULL;
	char* buf = calloc(strlen(orig) + 1, sizeof(char));
	strcpy(buf, orig);
	return buf;
}

char* rvgetcwd() {
#ifdef _GNU_SOURCE
	// Glibc has a convenient function for this
	return get_current_dir_name();
#else
	// The classical way
	char* buf = calloc(PATH_MAX, sizeof(char));
	getcwd(buf, PATH_MAX);
	return buf;
#endif
}

void RoverVoiceFixPath(char* pb) {
	if (!pb)
		return;
	for (unsigned i = 0; i < strlen(pb); i++) {
		const char thisChar = pb[i];
		if (thisChar == '/' || thisChar == '\\')
			pb[i] = RoverVoicePathDelimiter;
	}
}

bool RoverVoiceDirectoryExists(const char* path) {
	if (!path)
		return false;
	DIR* dirPtr = opendir(path);
	if (dirPtr) {
		closedir(dirPtr);
		return true;
	}
	return false;
}

char* RoverVoiceGetVoicePath(const char* vid) {
	if (!vid)
		return NULL;
	char* pwd = rvgetcwd();
	if (getenv("ROVERVOICE_PATH")) {
		free(pwd);
		pwd = rvstrdup(getenv("ROVERVOICE_PATH"));
	}
	char* potentialPath = calloc(strlen(pwd) + strlen(vid) + 65, sizeof(char));
	sprintf(potentialPath, "%s/%s_phonemes", pwd, vid);
	free(pwd);
	if (!RoverVoiceDirectoryExists(potentialPath)) {
		free(potentialPath);
		potentialPath = NULL;
	}
	return potentialPath;
}

bool RoverVoiceFileExists(const char* absPath) {
	if (!absPath)
		return false;
	return (access(absPath, F_OK) == 0);
}

#ifndef __APPLE__
bool RoverVoiceAudioPlayerWrapper(const char* path) {
	if (!path)
		return false;
#ifndef _WIN32
	if (!RoverVoiceCanberraContext) {
		RoverVoiceCanberraContext = malloc(sizeof(ca_context*));
		ca_context_create(RoverVoiceCanberraContext);
	}
	ca_context_play(*RoverVoiceCanberraContext, 0, CA_PROP_MEDIA_FILENAME, path, CA_PROP_MEDIA_ROLE, "music", NULL);
	g_usleep(1000 * 135);
	return true;
#else
	return PlaySound(path, NULL, SND_FILENAME);
#endif
}

#endif