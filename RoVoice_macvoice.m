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

#import <CoreFoundation/CoreFoundation.h>
#import <AVFoundation/AVFoundation.h>
#define ROVERVOICE_ACCESS_PRIVATE
#include "RoVoice_uncross.h"

bool RoverVoiceAudioPlayerWrapper(const char* path) {
	if (!path)
		return false;
	NSString* pathMac = [[NSString alloc] initWithUTF8String:path];
	NSURL* url = [[NSURL alloc] initFileURLWithPath:pathMac isDirectory:NO];
	AVPlayer* plr = [[AVPlayer alloc] initWithURL:url];
	[plr play];
	usleep(1000 * 135);
	[plr pause];
	[plr dealloc];
	[url dealloc];
	[pathMac dealloc];
	return true;
}