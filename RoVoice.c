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

#include "RoVoice.h"
#define ROVERVOICE_ACCESS_PRIVATE
#include "RoVoice_uncross.h"

struct RoverVoice_s {
	char* langOrName;
	char* baseDir;
	RoverExceptionMap* exceptions;
};

void RoverPhonemeDump(const RoverPhoneme* phnm);

RoverVoice* RoverVoiceCreate(const char* vid) {
	if (!vid)
		return NULL;
	char* bd = RoverVoiceGetVoicePath(vid);
	if (!bd)
		return NULL;
	char* excPath = calloc(strlen(bd) + 17, sizeof(char));
	sprintf(excPath, "%s/exceptions.rvc", bd);
	RoverVoiceFixPath(excPath);
	if (!RoverVoiceFileExists(excPath)) {
		free(excPath);
		free(bd);
		return NULL;
	}
	RoverVoice* vc = malloc(sizeof(RoverVoice));
	vc->langOrName = rvstrdup(vid);
	vc->baseDir = bd;
	vc->exceptions = RoverExceptionMapCreateFromFile(excPath);
	return vc;
}

RoverPhoneme** RoverVoiceSplitIntoPhonemes(const RoverVoice* rvc, const char* str) {
	if (!str || !rvc)
		return NULL;
	RoverExceptionMap* exc = rvc->exceptions;
#ifdef DEBUG
	fprintf(stderr, "exc = %p\n", exc);
#endif
	if (!exc)
		return NULL;
	RoverPhoneme** result = calloc(strlen(str) + 2, sizeof(RoverPhoneme*));
	unsigned index = 0;
	for (unsigned i = 0; i < strlen(str); i++) {
		RoverPhoneme* phm = RoverPhonemeCreate(rvc->langOrName, tolower((str[i] != '!') ? str[i] : ' '));
		if (phm && index >= 1) {
			RoverPhoneme* prevPhm = result[index - 1];
			if (prevPhm)
				RoverPhonemeSetNext(prevPhm, phm);
			RoverPhonemeSetPrevious(phm, prevPhm);
			result[index] = phm;
			index += 1;
		} else if (phm) {
			result[index] = phm;
			index = 1;
		}
		RoverPhonemeDump(phm);
	}
	for (unsigned i = 0; i < strlen(str); i++) {
		RoverPhoneme* phmAgain = result[i];
		if (phmAgain) {
			RoverExceptionMapRewind(exc);
			char* key = NULL;
			char value = '\0';
			while (RoverExceptionMapIterate(exc, &key, &value)) {
				if (RoverPhonemeDoMakeUpException(phmAgain, key)) {
					RoverPhoneme* nextPhm = result[i + strlen(key)];
					RoverPhonemeSetNext(phmAgain, nextPhm);
					RoverPhonemeSetAssociatedCharacter(phmAgain, value);
					break;
				}
			}
		}
	}
	return result;
}

void RoverVoiceCorrectlyDeallocatePhonemes(RoverPhoneme** pnms) {
	if (!pnms)
		return;
	RoverPhoneme* pnm = pnms[0];
	unsigned index = 0;
	while (pnm) {
		RoverPhoneme* nextPnm = RoverPhonemeGetNext(pnm);
		RoverPhonemeRelease(pnm);
		pnms[index] = NULL;
		pnm = nextPnm;
		index += 1;
	}
	free(pnms);
}

bool RoverVoiceTrySpeak(const RoverVoice* rvc, const char* what) {
	if (!rvc || !what)
		return false;
	RoverPhoneme** pnms = RoverVoiceSplitIntoPhonemes(rvc, what);
	if (!pnms)
		return false;
	RoverPhoneme* pnm = pnms[0];
	while (pnm) {
		if (!RoverPhonemeTryPlay(pnm))
			return false;
		pnm = RoverPhonemeGetNext(pnm);
	}
	RoverVoiceCorrectlyDeallocatePhonemes(pnms);
	return true;
}

void RoverVoiceRelease(RoverVoice* rvc) {
	if (!rvc)
		return;
	RoverExceptionMapRelease(rvc->exceptions);
	rvc->exceptions = NULL;
	free(rvc);
}