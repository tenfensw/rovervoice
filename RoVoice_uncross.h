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

#ifndef ROVERVOICE_UNCROSS_H
#define ROVERVOICE_UNCROSS_H

#ifndef ROVERVOICE_ACCESS_PRIVATE
#error "Basically, this header is not for public use."
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>

#ifdef __linux__
#include <linux/limits.h>
#endif

char* rvstrdup(const char* orig);

char* RoverVoiceGetVoicePath(const char* vid);
bool RoverVoiceFileExists(const char* absPath);
void RoverVoiceFixPath(char* pb);

bool RoverVoiceAudioPlayerWrapper(const char* path);

struct RoverPhoneme_s;
typedef struct RoverPhoneme_s RoverPhoneme;

struct RoverExceptionMap_s;
typedef struct RoverExceptionMap_s RoverExceptionMap;

RoverPhoneme* RoverPhonemeCreate(const char* vid, const char ltr);
bool RoverPhonemeTryPlay(const RoverPhoneme* phnm);
char RoverPhonemeGetAssociatedCharacter(const RoverPhoneme* phnm);
bool RoverPhonemeSetNext(RoverPhoneme* phnm, RoverPhoneme* nextPhnm);
bool RoverPhonemeSetPrevious(RoverPhoneme* phnm, RoverPhoneme* prevPhnm);
bool RoverPhonemeSetAssociatedCharacter(RoverPhoneme* phm, const char letter);
RoverPhoneme* RoverPhonemeGetNext(const RoverPhoneme* phnm);
bool RoverPhonemeDoMakeUpException(const RoverPhoneme* phnm, const char* expr);
void RoverPhonemeRelease(RoverPhoneme* phnm);

RoverExceptionMap* RoverExceptionMapCreateFromFile(const char* path);
bool RoverExceptionMapIterate(RoverExceptionMap* mp, char** exc, char* correctLetter);
bool RoverExceptionMapRewind(RoverExceptionMap* mp);
void RoverExceptionMapRelease(RoverExceptionMap* mp);

#endif