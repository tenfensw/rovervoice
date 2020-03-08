# RoverVoice

*A really tiny, banal TTS engine*
*Copyright (C) Tim K 2019-2020 <timprogrammer@rambler.ru>*

Licensed under 0BSD License.

## Description (aka why did I even create a yet another TTS)

**RoverVoice** is an extremely minimalistic and portable text-to-speech synthesis engine written in C99. Its core advantage is portability and the quite simple process of creating voices for it.

RoverVoice also seems to be the only TTS (as of March 2020) to be licensed under 0BSD License.

### Pros

* Portable (written in C99, officially supports Windows, Mac, Linux and FreeBSD, other platforms with ``libcanberra`` or with Win32-like API might be supported too, but are untested)
* (Quite) fast

### Cons

* Does not support non-Latin languages right now
* Speaks broken English
* ...and, basically, it isn't that of a serious TTS engine, but rather just a fun research project

## Building
### Preparation steps
#### Mac users

macOS 10.7 and up is supported both as a host or target (the reason for absence of Snow Leopard support is the usage of ``AVFoundation`` for audio playback). You'll need Xcode Command Line tools to be present on the system.

#### Linux/FreeBSD users

You'll need ``libcanberra`` development headers to be present on your system.

#### Windows users

You'll need MSYS2 to build this thing. VS is not supported, sorry. :-(

### The building process

Just run these commands:

```
$ git clone https://github.com/timkoi/rovervoice
$ cd rovervoice
$ sh build.sh
```

As a result, you'll get:
- ``libRoVoice.so`` (or ``.dll``, or ``.dylib``) - the library itself
- ``RoVoice.h`` - the only header that you'll need to use RoverVoice
- ``rospeak`` - a standalone espeak-like program for using RoverVoice from bash

To use RoverVoice in your project, just link it to the library and add the ``RoVoice.h`` header to your project. And yes, you'll have to find a voice somewhere, too.

## API docs

So here are all the 3 (!!) functions that are available:

```
RoverVoice* RoverVoiceCreate(const char* vid);
bool RoverVoiceTrySpeak(const RoverVoice* rvc, const char* what);
void RoverVoiceRelease(RoverVoice* rvc);
```

- ``RoverVoiceCreate`` is responsible for initializing a specific voice. The name of the voice is specified as the argument. RoverVoice itself comes with one reference voice - the English voice, which can be loaded using ``RoverVoiceCreate("en");`` Notice that the voice must either be located in the current working directory or in the directory specified by **ROVERVOICE_PATH** environment variable.
- ``RoverVoiceTrySpeak`` is responsible for reading the specified text with the specified voice. All the unknown characters are skipped. This method returns ``true`` on success, ``false`` on failure.
- ``RoverVoiceRelease`` deallocates the voice correctly. Use this instead of ``free``.

Example:
```
RoverVoice* vc = RoverVoiceCreate("en");
if (!RoverVoiceTrySpeak(vc, "This will be spoken, probably"))
	fprintf(stderr, "EPIC FAIL\n");
else
	fprintf(stderr, "EPIC SUCCESS\n");
RoverVoiceRelease(vc);
```

## Voice format
RoverVoice voice is basically a folder full of prerecorded WAV files containing the pronunciation of each of the used character in the language. It also contains ``exceptions.rvc`` - a text file specified which character combinations shall be read differently.

Now the name of each ``wav`` file must be in the format of ``<letter in lowercase>.wav``. But, if, let's say, you have a character combination that is read so differently that you cannot replace it with a pronunciation of one character. So, in that case, you can create either ``!.wav`` or ``_<capital latin letters from A to Z>.wav\n`` to implement these (``\n`` is the newline character).

Then, of course, for each exception, you must provide a reference in the ``exceptions.rvc`` in the format of ``<syllable or character combination>@<replacement character>``. Example:

```
gh@g
ch@!
```

This will mean that when RoverVoice phoneme parser will encounter ``gh``, it will read it as if it encountered the letter ``g``, but if it encounters ``ch``, it will try to look for ``!.wav`` and play it instead.

The maximum duration of each ``.wav`` file is 135 ms.

## License

0BSD
