#!/bin/sh
MCC=cc
test ! -z "$CC" && MCC="$CC"
MCFLAGS="-fPIC -Wall -Wextra -g -std=c99 -I. $MCFLAGS"
MLDFLAGS="$LDFLAGS"
TARGETLIB="libRoVoice.so"
TARGETS="RoVoice_phonemes RoVoice_uncross RoVoice_map RoVoice"
if test `uname` = "Darwin"
then
	MLDFLAGS="-framework AVFoundation -framework Cocoa -framework CoreFoundation"
	TARGETS="RoVoice_macvoice $TARGETS"
	TARGETLIB="libRoVoice.dylib"
else
	MCFLAGS="$MCFLAGS `pkg-config --cflags glib-2.0 libcanberra`"
	MLDFLAGS="`pkg-config --libs glib-2.0 libcanberra`"
fi

rm -rf o_*.o $TARGETLIB rospeak
TARGETOBJS=''
echo "$MCC -c -o <output> $MCFLAGS <input>"
for tgt in $TARGETS
do
	output="o_$tgt.o"
	input="$tgt.c"
	test -f "$tgt.m" && input="$tgt.m"
	printf '\t%s\t%s\n' "$MCC" "$output"
	$MCC -c -o "$output" $MCFLAGS "$input" || exit 1
	TARGETOBJS="$TARGETOBJS $output"
done

printf '\t%s\t%s\n' "LINK" "$TARGETLIB"
$MCC -shared -o "$TARGETLIB" $TARGETOBJS $MLDFLAGS || exit 2
$MCC -o rospeak $MCFLAGS rospeak.c -L. -lRoVoice
exit $?
