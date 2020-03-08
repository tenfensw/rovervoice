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

struct RoverPhoneme_s {
	char representedLetter;
	char* path;
	char* vid;
	RoverPhoneme* previous;
	RoverPhoneme* next;
};

RoverPhoneme* RoverPhonemeCreate(const char* vid, const char ltr) {
	if (!vid || isspace(ltr) != 0 || isblank(ltr) != 0 || (ispunct(ltr) != 0 && ltr != '!'))
		return NULL;
	char* voicePath = RoverVoiceGetVoicePath(vid);
	if (!voicePath)
		return NULL;
	char* phonemeLetterPath = calloc(strlen(voicePath) + 7, sizeof(char));
	sprintf(phonemeLetterPath, "%s/%c.wav", voicePath, ltr);
	RoverVoiceFixPath(phonemeLetterPath);
	if (!RoverVoiceFileExists(phonemeLetterPath)) {
		fprintf(stderr, "'%s' - no such file\n", phonemeLetterPath);
		free(phonemeLetterPath);
		return NULL;
	}
	RoverPhoneme* phnm = malloc(sizeof(RoverPhoneme));
	phnm->vid = rvstrdup(vid);
	phnm->representedLetter = ltr;
	phnm->path = phonemeLetterPath;
	phnm->previous = NULL;
	phnm->next = NULL;
	return phnm;
}

bool RoverPhonemeSetAssociatedCharacter(RoverPhoneme* phm, const char letter) {
	if (!phm)
		return false;
	RoverPhoneme* stuntDouble = RoverPhonemeCreate(phm->vid, letter);
	if (!stuntDouble)
		return false;
	phm->representedLetter = letter;
	phm->path = rvstrdup(stuntDouble->path);
	RoverPhonemeRelease(stuntDouble);
	return true;
}

void RoverPhonemeDump(const RoverPhoneme* phnm) {
	(void)(phnm);
#ifdef DEBUG
	if (!phnm)
		fprintf(stderr, "(null)\n");
	else
		fprintf(stderr, "<RoverPhoneme*> %p { letter = '%c', path = \"%s\", previous = %p, next = %p }\n", phnm, phnm->representedLetter, phnm->path, phnm->previous, phnm->next);
#endif
}

bool RoverPhonemeDoMakeUpException(const RoverPhoneme* phnm, const char* expr) {
	if (!phnm || !expr || strlen(expr) < 2)
		return false;
	if (expr[0] != phnm->representedLetter)
		return false;
	RoverPhoneme* current = RoverPhonemeGetNext(phnm);
	for (unsigned i = 1; i < strlen(expr); i++) {
		if (!current || current->representedLetter != expr[i])
			return false;
		current = RoverPhonemeGetNext(phnm);
	}
	return true;
}

bool RoverPhonemeSetNext(RoverPhoneme* phnm, RoverPhoneme* nextPhnm) {
	if (!phnm)
		return false;
	phnm->next = nextPhnm;
	return true;
}

bool RoverPhonemeSetPrevious(RoverPhoneme* phnm, RoverPhoneme* prevPhnm) {
	if (!phnm)
		return false;
	phnm->previous = prevPhnm;
	return true;
}

RoverPhoneme* RoverPhonemeGetNext(const RoverPhoneme* phnm) {
	if (!phnm)
		return NULL;
	return phnm->next;
}

bool RoverPhonemeTryPlay(const RoverPhoneme* phnm) {
	RoverPhonemeDump(phnm);
	if (!phnm)
		return false;
	return RoverVoiceAudioPlayerWrapper(phnm->path);
}

char RoverPhonemeGetAssociatedCharacter(const RoverPhoneme* phnm) {
	if (!phnm)
		return '\0';
	return phnm->representedLetter;
}

void RoverPhonemeRelease(RoverPhoneme* phnm) {
	if (!phnm)
		return;
	phnm->previous = NULL;
	phnm->next = NULL;
	free(phnm);
}