#ifndef FILEUTIL_SUPPORT_H
#define FILEUTIL_SUPPORT_H

// https://stackoverflow.com/a/8556436
#define REP0(X)
#define REP1(X) X
#define REP2(X) REP1(X) X
#define REP3(X) REP2(X) X
#define REP4(X) REP3(X) X
#define REP5(X) REP4(X) X
#define REP6(X) REP5(X) X
#define REP7(X) REP6(X) X
#define REP8(X) REP7(X) X
#define REP9(X) REP8(X) X
#define REP10(X) REP9(X) X

#define REP(HUNDREDS,TENS,ONES,X) \
  REP##HUNDREDS(REP10(REP10(X))) \
  REP##TENS(REP10(X)) \
  REP##ONES(X)

#define TEST1_CONTENT \
  "helo\n"

#define TEST2_CONTENT \
  "booger\n" \
  "face\n" \
  "toodaloo\n"

#define TEST3_CONTENT \
  "when are we going\n" \
  "to the malll\n" \
  "mist\n" \
  "er\n"

#define TEST4_CONTENT \
  "\n"\
  REP(5,0,0, REP(1, 0, 0, ".") "\n")

#define TESTBIN0_CONTENT \
  "\x31\x42\x53\x13\x52\x56\x94"\
  "\x42\x53\x13\x52\x56\x94\x31"\
  "\x53\x13\x52\x56\x94\x31\x42"\
  "\x13\x52\x56\x94\x31\x42\x53"\
  "\x52\x56\x94\x31\x42\x53\x13"\

#define TESTBIN1_CONTENT \
  "\x32\x32\x53\x12\x52\x56\x94"\
  "\x25\x53\x43\x51\x56\x94\x31"\
  "\x22\x73\x22\x52\x94\x31\x42"\
  "\x39\x12\x46\x96\x31\x42\x53"\
  "\x52\x76\x54\x32\x42\x53\x13"\

#endif
