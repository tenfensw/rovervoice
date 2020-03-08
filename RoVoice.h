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

#ifndef ROVERVOICE_H
#define ROVERVOICE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>

struct RoverVoice_s;
typedef struct RoverVoice_s RoverVoice;

RoverVoice* RoverVoiceCreate(const char* vid);
bool RoverVoiceTrySpeak(const RoverVoice* rvc, const char* what);
void RoverVoiceRelease(RoverVoice* rvc);

#endif