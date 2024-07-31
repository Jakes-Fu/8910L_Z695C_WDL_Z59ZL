
#if defined(__cplusplus)
extern "C" {
#endif

#include <stdio.h>
#include <string.h>

#include "javacall_dir.h"
#include "javacall_memory.h"
#include "javacall_file.h"
#include "javacall_logging.h"

#define PRELOAD_FILE_NUM 512
void util_wstr2str(char *str, unsigned short *wstr)
{
    while (*wstr) {
        *str = (char)*wstr;
        str++; wstr++;
    }
    *str = 0;
}

void util_str2wstr(unsigned short *wstr, char *str)
{
    while (*str){
        *wstr = (unsigned short)*str;/*lint !e571 */
        wstr++; str++;
    }
    *wstr = 0;
}

/////////////////////////////////////////////////////////////////////////////
static int preload_file_n = 0;
static char *preload_file_list[PRELOAD_FILE_NUM] = {0,};
static char preload_gen_path[256];
static int preload_midlet_n = 1;

void preload_init(void)
{
    unsigned short at_java_path[256] = {0,};
    int at_java_len = 255;
    javacall_handle enumhandle;

    javacall_dir_get_root_path(at_java_path, &at_java_len);
    enumhandle = javacall_dir_open(at_java_path, at_java_len);
    if (enumhandle != NULL) {
        unsigned short *nextFile;
        int fileLen;

        at_java_path[at_java_len] = 0;
        util_wstr2str(preload_gen_path, at_java_path);
        preload_file_n = 0;

        while ((nextFile = javacall_dir_get_next(enumhandle, &fileLen))!=NULL) {
            nextFile[fileLen] = 0;
            
            /*at_java_path[at_java_len]= '\\';
            at_java_len ++;
            at_java_path[at_java_len]=0;
            MMIAPICOM_Wstrcat(at_java_path, nextFile);*/
                
            preload_file_list[preload_file_n] = javacall_malloc(fileLen+1);
            util_wstr2str(preload_file_list[preload_file_n], nextFile);
            //javacall_printf("File Found= %s\n", preload_file_list[preload_file_n]);

            preload_file_n++;
            if (preload_file_n >= PRELOAD_FILE_NUM) {
                javacall_printf("[Warning! preload_init] >> Too many files\n");
                break;
            }
        }

        javacall_dir_close(enumhandle);
    }
}

#define FILE_BUF_LEN    1024
static void preload_gen_file_i(char *srcf, char *dstf)
{
    javacall_handle srcfd, dstfd;
    javacall_result result;
    unsigned short wfname[256];
    unsigned char rBuf[FILE_BUF_LEN];
    int nRead;
    int bufi, remi;
    unsigned char *wBuf;

    srcfd = dstfd = NULL;
    do {
        util_str2wstr(wfname, srcf);
        result = javacall_file_open(wfname, strlen(srcf), JAVACALL_FILE_O_RDWR, &srcfd);
        if (result == JAVACALL_FAIL) {
            javacall_printf("ERROR    Can't open src file %s\n", srcf);
            break;
        }

        util_str2wstr(wfname, dstf);
        result = javacall_file_open(wfname, strlen(dstf), 
            JAVACALL_FILE_O_RDWR|JAVACALL_FILE_O_CREAT|JAVACALL_FILE_O_TRUNC, &dstfd);
        if (result == JAVACALL_FAIL) {
            javacall_printf("ERROR    Can't open dst file %s\n", dstf);
            break;
        }

        wBuf = (unsigned char*)wfname;
        while ((nRead = javacall_file_read(srcfd, rBuf, FILE_BUF_LEN))>0) {
            bufi = 0;
            while (bufi < nRead) {
                if ((nRead - bufi) >= 8) {
                    sprintf((char*)wBuf, "0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x,\n",
                        rBuf[bufi], rBuf[bufi+1], rBuf[bufi+2], rBuf[bufi+3], 
                        rBuf[bufi+4], rBuf[bufi+5], rBuf[bufi+6], rBuf[bufi+7]);
                    bufi += 8;
                    
                    javacall_file_write(dstfd, wBuf, strlen((char*)wBuf));
                } else {
                    remi = 0;
                    for (; bufi<nRead; bufi++) {
                        sprintf((char*)&wBuf[remi], "0x%02x, ", rBuf[bufi]);
                        remi = strlen((char*)wBuf);
                    }

                    javacall_file_write(dstfd, wBuf, strlen((char*)wBuf));
                }
            }
        }

        // remove the last two or three bytes (",\n" or ", ")
        /*fileSize = javacall_file_sizeofopenfile(dstfd);
        if (fileSize == -1) {
            javacall_printf("WARNING    Can't check the size of %s\n", dstf);
        } else {
            int removen = strlen(wBuf);
            //javacall_printf("Current dst size= %d\n", stat_buf.st_size);
            if (wBuf[removen-1]==' ')
                removen = 2;
            else
                removen = 3;
                
            if (-1==javacall_file_truncate(dstfd, (long)(fileSize - removen))) {
                javacall_printf("WARNING    Can't change the file size to %s\n", dstf);
            }
        }*/
    } while(0);

    if (srcfd != NULL) {
        javacall_file_close(srcfd);
    }
    if (dstfd != NULL) {
        javacall_file_close(dstfd);
    }
}

void preload_gen_file_one2n(void)
{
    int i = 255;
    char srcf[256];
    char dstf[256];
    char atjava[256];
    unsigned short watjava[256];

    javacall_dir_get_root_path(watjava, &i);
    watjava[i] = 0;
    util_wstr2str(atjava, watjava);

    for (i=0; i<preload_file_n; i++) {
        sprintf(srcf, "%s\\%s", atjava, preload_file_list[i]);
        //javacall_printf("SRC FILE= %s\n", srcf);
        sprintf(dstf, "%s\\preload%02d.gen", preload_gen_path, i+1);
        //javacall_printf("DST FILE= %s\n", dstf);

        preload_gen_file_i(srcf, dstf);
    }
}

static void __build_suite_runtime_info(int outfd);

#define include_std "#include \"std_header.h\"\n\n"
#define DEFINE_NULL "#define NULL   ((void*)0)\n\n"
#define PRELOAD_LIST_T "typedef struct {\n\tchar *name;\n\tunsigned char *filep;\n\tint filen;\n} PRELOAD_LIST_T;\n\n"
#define FILE_BRACE_END "};\n\n"
#define TWONEWLINES "\n\n"
#define PRELOAD_LIST_START "static PRELOAD_LIST_T preload_list_static[] =\n{\n"
#define PRELOAD_LIST_END "\t{ NULL, NULL, 0 }\n};"

#define PRELOAD_LIST_N "PRELOAD_LIST_T *preload_list = preload_list_static;"
#define PRELOAD_LIST_NILL "PRELOAD_LIST_T *preload_list = NULL;"

#define COPYRIGHT01 "/*\n *\n"
#define COPYRIGHT02 " * Copyright 1990-2008 Sun Microsystems. Inc. All rights reserved.\n"
#define COPYRIGHT03 " * SUN PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.\n */\n\n"

#define WARNING01 COPYRIGHT01
#define WARNING02 " * Never modify this file, preload00_gen.c, which is generated by preloader.exe\n"
#define WARNING03 " * Check with 'preloader.exe -h' for more information\n */\n\n\n"
void preload_gen_file_zero(void)
{
    int i;
    int zrfd;
    char buf[256];
    unsigned short wbuf[256];
    char *srcline;
    javacall_result result;
    char *preloadNN;

    sprintf(buf, "%s\\preload00_gen.c", preload_gen_path);
    util_str2wstr(wbuf, buf);
    result = javacall_file_open(wbuf, strlen((char*)buf), 
        JAVACALL_FILE_O_RDWR|JAVACALL_FILE_O_CREAT|JAVACALL_FILE_O_TRUNC, &zrfd);/*lint !e64 */
    if (result == JAVACALL_FAIL) {
        javacall_printf("ERROR    Can't generate file %s\n", buf);
        return;
    }

    srcline = (char*)wbuf;

    sprintf(srcline, "%s%s%s%s%s%s%s%s%s", COPYRIGHT01, COPYRIGHT02, COPYRIGHT03,
        WARNING01, WARNING02, WARNING03, INCLUDE_STD,
        DEFINE_NULL, PRELOAD_LIST_T);
    javacall_file_write(zrfd, srcline, strlen(srcline));/*lint !e64 */

    // nothing
    if (0==preload_file_n) {
        javacall_file_write(zrfd, PRELOAD_LIST_NILL, strlen((char*)PRELOAD_LIST_NILL));/*lint !e64 */
        __build_suite_runtime_info(zrfd);
        javacall_file_close(zrfd);/*lint !e64 */
        return;
    }

    
    preloadNN = buf;
    // define static initialized arrays for preload files
    for (i=0; i<preload_file_n; i++) {
        sprintf(preloadNN, "preload%02d", i+1);
        
        sprintf(srcline, "static unsigned char %s[] = {\n", preloadNN); // line1
        sprintf(&srcline[strlen(srcline)], "\t#include \"%s.gen\"\n", preloadnn); // line 2,3,4
        sprintf(&srcline[strlen(srcline)], "%s", FILE_BRACE_END); // line 5
        javacall_file_write(zrfd, srcline, strlen((char*)srcline));/*lint !e64 */ // line 2,3,4
    }

    javacall_file_write(zrfd, TWONEWLINES, strlen((char*)TWONEWLINES));/*lint !e64 */ // two empty lines

    // define preload list array as to
    // typedef struct {
    //      char *name;
    //      unsigned char *filep;
    //      int filen; // no of bytes
    // } PRELOAD_LIST_T
    javacall_file_write(zrfd, PRELOAD_LIST_START, strlen((char*)PRELOAD_LIST_START));/*lint !e64 */
    for (i=0; i<preload_file_n; i++) {
        // file name
        sprintf(srcline, "\t{ \"%s\", ", preload_file_list[i]);
        // file pointer
        sprintf(&srcline[strlen(srcline)], "preload%02d, ", i+1);
        // file size
        sprintf(&srcline[strlen(srcline)], "sizeof(preload%02d) },\n", i+1);

        javacall_file_write(zrfd, srcline, strlen(srcline));/*lint !e64 */
    }

    sprintf(srcline, "%s\n\n\n%s", PRELOAD_LIST_END, PRELOAD_LIST_N);
    javacall_file_write(zrfd, srcline, strlen(srcline));/*lint !e64 */

    /*generate runtime data
     ***************************************/
    __build_suite_runtime_info(zrfd);

    javacall_file_close(zrfd);/*lint !e64 */
}

#define PRELOAD_RUNTIME_T "\n\n\n/////////////////////////////////////\ntypedef struct \
{\n\tint midSid;\n\tchar *midCls;\n\tchar *midName;\n} PRELOAD_RUNTIME_T;\n\n"

#define PRELOAD_RT_NILL "PRELOAD_RUNTIME_T *preload_runtime = NULL;"

#define PRELOAD_RT_START "static PRELOAD_RUNTIME_T preload_rt_static[] =\n{\n"
#define PRELOAD_RT_END "\t{0, NULL, NULL}\n};"

#define PRELOAD_RT_N "PRELOAD_RUNTIME_T *preload_runtime = preload_rt_static;"

static void __build_suite_runtime_info(int outfd)
{
    char buf[256];
    int i, j, sidLen, sid;
    char sidStr[8] = {0,};

    javacall_file_write(outfd, PRELOAD_RUNTIME_T, strlen(PRELOAD_RUNTIME_T));/*lint !e64 */

    // nothing
    if (0==preload_file_n) {
        javacall_file_write(outfd, PRELOAD_RT_NILL, strlen(PRELOAD_RT_NILL));/*lint !e64 */
        return;
    }
    javacall_file_write(outfd, PRELOAD_RT_START, strlen(PRELOAD_RT_START));/*lint !e64 */

    for (i=0; i<preload_midlet_n; i++) {
        //suite id starts from 2
        sid = i + 2;
        sidLen = 0;
        for (j=0; sid>0; j++) {
            sidLen++;
            sidStr[sidLen-j-1] = (char)((int)'0' + sid%10);
            sid = sid/10;
        }
        sidStr[sidLen] = 0;
        sprintf(buf, "\t{ %s, \"%s\", \"%s\" },\n",
            sidStr, "<classname>", "<midletname>");
        javacall_file_write(outfd, buf, strlen(buf));/*lint !e64 */
    } // for

    sprintf(buf, "%s\n\n\n%s", PRELOAD_RT_END, PRELOAD_RT_N);
    javacall_file_write(outfd, buf, strlen(buf));/*lint !e64 */
}

#if defined(__cplusplus)
}
#endif

