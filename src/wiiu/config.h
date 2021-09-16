/*
 * Wii U config
 * Copied from an auto-generated config.h and tweaked until it was correct :)
 * Created by Terry Hearst on 2021/03/21
 */

/* true-color rendering */
/* #undef CRISPY_TRUECOLOR */

/* Define to 1 if you have the declaration of `strcasecmp', and to 0 if you
   don't. */
#define HAVE_DECL_STRCASECMP 1

/* Define to 1 if you have the declaration of `strncasecmp', and to 0 if you
   don't. */
#define HAVE_DECL_STRNCASECMP 1

/* Define to 1 if you have the <dev/isa/spkrio.h> header file. */
/* #undef HAVE_DEV_ISA_SPKRIO_H */

/* Define to 1 if you have the <dev/speaker/speaker.h> header file. */
/* #undef HAVE_DEV_SPEAKER_SPEAKER_H */

/* Define to 1 if you have the <dirent.h> header file. */
#define HAVE_DIRENT_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `ioperm' function. */
/* #undef HAVE_IOPERM */

/* Define to 1 if you have the `amd64' library (-lamd64). */
/* #undef HAVE_LIBAMD64 */

/* Define to 1 if you have the `i386' library (-li386). */
/* #undef HAVE_LIBI386 */

/* Define to 1 if you have the `m' library (-lm). */
#define HAVE_LIBM 1

/* libpng installed */
#ifndef __WIIU__
#define HAVE_LIBPNG 1
#endif // !__WIIU__

/* libsamplerate installed */
#ifndef __WIIU__
#define HAVE_LIBSAMPLERATE 1
#endif // !__WIIU__

/* zlib installed */
#ifndef __WIIU__
#define HAVE_LIBZ 1
#endif // !__WIIU__

/* Define to 1 if you have the <linux/kd.h> header file. */
/* #undef HAVE_LINUX_KD_H */

/* Define to 1 if you have the `mmap' function. */
/* #undef HAVE_MMAP */

/* Define to 1 if you have the `qsort' function. */
#define HAVE_QSORT 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdio.h> header file. */
#define HAVE_STDIO_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "fabian@greffrath.com"

#ifdef WIIU_CRISPY_DOOM

/* Name of package */
#define PACKAGE "crispy-doom-u"

/* Define to the full name of this package. */
#define PACKAGE_NAME "Crispy Doom U"

#endif // WIIU_CRISPY_DOOM

#ifdef WIIU_CRISPY_HERETIC

/* Name of package */
#define PACKAGE "crispy-heretic-u"

/* Define to the full name of this package. */
#define PACKAGE_NAME "Crispy Heretic U"

#endif // WIIU_CRISPY_HERETIC

#ifdef WIIU_CRISPY_HEXEN

/* Name of package */
#define PACKAGE "crispy-hexen-u"

/* Define to the full name of this package. */
#define PACKAGE_NAME "Crispy Hexen U"

#endif // WIIU_CRISPY_HEXEN

#ifdef WIIU_CRISPY_STRIFE

/* Name of package */
#define PACKAGE "crispy-strife-u"

/* Define to the full name of this package. */
#define PACKAGE_NAME "Crispy Strife U"

#endif // WIIU_CRISPY_STRIFE

/* Version number of package */
#define VERSION "5.10.3"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING PACKAGE_NAME " " VERSION

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME PACKAGE

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION VERSION

/* Change this when you create your awesome forked version */
#define PROGRAM_PREFIX "crispy-"

/* Define to 1 if all of the C90 standard headers exist (not just the ones
   required in a freestanding environment). This macro is provided for
   backward compatibility; new code need not use it. */
#define STDC_HEADERS 1

// TODO: Be able to change this rather than being a macro
#define HOMEBREW_APP_PATH "wiiu/apps/" PACKAGE
