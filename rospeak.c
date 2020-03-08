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

int main(const int argc, char** argv) {
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <voice> <text>\n", argv[0]);
		return 1;
	}
	RoverVoice* vc = RoverVoiceCreate(argv[1]);
	if (!vc) {
		fprintf(stderr, "No such voice found - '%s'\n", argv[1]);
		return 2;
	}
	for (unsigned i = 2; i < (unsigned)(argc); i++) {
		if (!RoverVoiceTrySpeak(vc, argv[i])) {
			fprintf(stderr, "Failed to pronounce '%s'\n", argv[i]);
			RoverVoiceRelease(vc);
			return 3;
		}
	}
	RoverVoiceRelease(vc);
	return 0;
}