/*
 * Copyright 2005-2012 Esmertec AG. All Rights Reserved.
 * $Id$
 * include this to get the jbed_processCmdLine function.
 */

#ifndef _JBED_CMDLINE_H_
#define _JBED_CMDLINE_H_

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * jbed_processCmdLine: parse the command line arguments and set the property.
 *                 don't forget to free the newArgv returned by this function
 *                 call because it's malloced.
 * @int argc: the oringinal arguments count.
 * @char **argv: the oringinal arguments.
 * @int *newArgc: return the new arguments count after some arguments have
 *                been fetched.
 * @ const char *midpHome: this argument can be set to NULL.
 *                         if it's not NULL, it will be treated as a pointer
 *                         to a path string pointing to rootdir if -rootdir
 *                         is not passed from command line.
 *                         also if -basedir or -localInstall are not passed in,
 *                         *midpHome + "/Installed/" or
 *                         *midphome + "/LocalInstall/"
 *                         will be used as default.
  * @return: the new arguments array without processed ones.
 */
char **jbed_processCmdLine(int argc, char **argv, int *newArgc,
                            const char *midpHome);

/* jbed_cmdline_release is use for releasing the memory returned by calling jbed_processCmdLine. */
void jbed_cmdline_release(char **newArgv);

#if defined(__cplusplus)
}
#endif

#endif  /* _JBED_CMDLINE_H_ */
