

#include "javacall_file.h"
#include "javacall_dir.h"
#include "javacall_logging.h"

#include "sig_code.h"
#include "mmijava_main.h"
#define JAVA_SUITES_FILE    "_suites.dat"

typedef struct {
    char            *name;
    unsigned char   *filep;
    int             filen;
} PRELOAD_LIST_T;

extern PRELOAD_LIST_T *preload_list;

typedef struct {
    int midSid;
    char *midCls;
    char *midName;
} PRELOAD_RUNTIME_T;

extern PRELOAD_RUNTIME_T *preload_runtime;

static int __str2jstr(javacall_utf16 *jstr, const char *str)
{
    int len;
    
    if (NULL==str) return 0;
    len = 0;
    while (*str) {
        *jstr = (javacall_utf16)*str;/*lint !e571 */
        str++; jstr++; len++;
    }
    return len;
}

static javacall_result __file_2_abs_jpath
    (javacall_utf16 *jpath, int *jpath_len, const char *filename)
{
    javacall_result jres;
    int max_path_len = *jpath_len;

    jres = javacall_dir_get_root_path(jpath, jpath_len);
    if (JAVACALL_OK != jres) {
        javacall_print("Jpreload>    ERROR  can't get JAVAROOT\n");
        return JAVACALL_FAIL;
    }

    // separator + null terminator
    if (max_path_len <= (*jpath_len)+(int)strlen(filename)+2) {
        javacall_printf("JPreload>    OOM  can't get path for %s\n", filename);
        return JAVACALL_OUT_OF_MEMORY;
    }

    jpath[*jpath_len] = javacall_get_file_separator();
    (*jpath_len)++;
    *jpath_len += __str2jstr(&jpath[*jpath_len], filename);

    return JAVACALL_OK;
}

static javacall_result __install_preload_i(PRELOAD_LIST_T *preload_i)
{
    javacall_result jres = JAVACALL_FAIL;
    javacall_handle fhandle;
    javacall_utf16 jpath[JAVACALL_MAX_FILE_NAME_LENGTH] = {0,};
    int jpath_len;
    long aWrite, nWrite;
    unsigned char *pWrite;

    jpath_len = __str2jstr(jpath, preload_i->name);
    jres = javacall_file_open(jpath, jpath_len,
        JAVACALL_FILE_O_RDWR|JAVACALL_FILE_O_CREAT|JAVACALL_FILE_O_TRUNC,
        &fhandle);
    if(JAVACALL_OK != jres) {
        javacall_printf("JPreload>    ERROR  Can't open %s\n", preload_i->name);
        return JAVACALL_FAIL;
    }

    aWrite = preload_i->filen;
    nWrite = 0;
    pWrite = preload_i->filep;
    while (aWrite > 0) {
        nWrite = javacall_file_write(fhandle, pWrite, aWrite);
        if (0==nWrite) {
            javacall_printf("Jpreload>    ERROR write fail to %s\n", preload_i->name);
            break;
        }
        aWrite -= nWrite;
        pWrite += nWrite;
    }

    if (NULL != fhandle) {
        javacall_file_close(fhandle);
    }
    if (aWrite > 0) {
        return JAVACALL_FAIL;
    }
    return JAVACALL_OK;
}

static void __uninstall_preload_i(PRELOAD_LIST_T *preload_i)
{
    javacall_utf16 jpath[JAVACALL_MAX_FILE_NAME_LENGTH];
    int jpath_len = JAVACALL_MAX_FILE_NAME_LENGTH;
    javacall_result jres;

    jres = __file_2_abs_jpath(jpath, &jpath_len, preload_i->name);
    if (JAVACALL_OK != jres) {
        javacall_printf("Jpreload>    WARN  can't get file path:%s\n", preload_i->name);
        javacall_print ("                   fail to uninstall\n");
        return;
    }

    jres = javacall_file_exist(jpath, jpath_len);
    if (JAVACALL_OK==jres) {
        jres = javacall_file_delete(jpath, jpath_len);
    }

    if (JAVACALL_OK != jres) {
        javacall_printf("JPreload>    WARN  fail to uninstall:%s\n", preload_i->name);
    }
}

void javacall_preloader_check_n_install(void)
{
    javacall_result jres;
    PRELOAD_LIST_T *preload_i;
    javacall_utf16 jpath[JAVACALL_MAX_FILE_NAME_LENGTH];
    int jpath_len;
    ///////////////////////////////////////////////////

    jres = javacall_file_init();
    if (JAVACALL_OK != jres) {
        javacall_print("JPreload>    ERROR  Can't start javahome FS !!\n");
        return;
    }
    ///////////////////////////////////////////////////
    if (NULL==preload_list) {
        javacall_file_finalize();
        return;
    }

    jpath_len = JAVACALL_MAX_FILE_NAME_LENGTH;
    jres = __file_2_abs_jpath(jpath, &jpath_len, JAVA_SUITES_FILE);
    if (JAVACALL_OK != jres) {
        javacall_printf("Jpreload>    ERROR  can't check file:%s\n", JAVA_SUITES_FILE);
        javacall_file_finalize();
        return;
    }
    if (JAVACALL_OK == javacall_file_exist(jpath, jpath_len)) {
        // not the first time since phone FS initialization
        javacall_file_finalize();
        return;
    }

    preload_i = preload_list;
    for (; preload_i->name!=NULL; preload_i++) {
        jres = __install_preload_i(preload_i);
        if (JAVACALL_OK != jres)
            break;
    }

    ///////////////////////////////////////////////////
    javacall_file_finalize();

    // Couldn't install midlets successfully
    // reset the JAVA ROOT PATH (remove all may-have-been created files
    if (JAVACALL_OK != jres) {
        preload_i = preload_list;
        for (; preload_i->name!=NULL; preload_i++) {
            __uninstall_preload_i(preload_i);
        }
    }
}

javacall_bool javacall_preloader_is_preload_midlet(int suiteId) {
    PRELOAD_RUNTIME_T *preload_i;

    if (NULL==preload_runtime)
        return JAVACALL_FALSE;

    preload_i = preload_runtime;
    for (; preload_i->midSid != 0; preload_i++) {
        if(preload_i->midSid == suiteId) {
            return JAVACALL_TRUE;
        }
    }
    return JAVACALL_FALSE;
}

void MMIJAVA_LAUNCH_PRELOAD_MIDLET(char *midletName) {
    PRELOAD_RUNTIME_T *preload_i;

    preload_i = preload_runtime;
    for (; preload_i->midName != NULL; preload_i++) {
        if (strcmp(preload_i->midName, midletName)==0) {
            Prepare_Launch_Midlet_Parameter(preload_i->midSid, preload_i->midCls);
            MMIJAVA_SendJVMMessage2MMI(APP_JAVA_LAUNCH_PRELOAD_MIDLET_IND, TXT_LAUNCH_MIDLET);
            return;
        }
    }
}

void MMIJAVA_update_preload_midlet(void)
{
   javacall_result jres = JAVACALL_FAIL;
    PRELOAD_LIST_T *preload_i = PNULL;
    javacall_utf16 jpath[JAVACALL_MAX_FILE_NAME_LENGTH] = {0};
    int32  jpath_len = 0;
    ///////////////////////////////////////////////////

    jres = javacall_file_init();
    if (JAVACALL_OK != jres) 
   {
        javacall_print("JPreload>    ERROR  Can't start javahome FS !!\n");
        return;
    }
    ///////////////////////////////////////////////////
    if (NULL==preload_list) {
        javacall_file_finalize();
        return;
    }
    

    preload_i = preload_list;
    for (; preload_i->name!=NULL; preload_i++) {

		jpath_len = JAVACALL_MAX_FILE_NAME_LENGTH;
		SCI_MEMSET(jpath,0,(JAVACALL_MAX_FILE_NAME_LENGTH*sizeof(javacall_utf16)));
    		jres = __file_2_abs_jpath(jpath, &jpath_len, preload_i->name);/*lint !e64 */
    		if (JAVACALL_OK != jres) {
			jres = JAVACALL_OK;
        		javacall_printf("Jpreload>    ERROR  can't check file:%s\n", preload_i->name);
			continue;
		}
   		 if (JAVACALL_OK == javacall_file_exist(jpath, jpath_len)) {
       
        		continue;        
    		}
	
       	 jres = __install_preload_i(preload_i);
        	if (JAVACALL_OK != jres)
            		break;
    }

    ///////////////////////////////////////////////////
    javacall_file_finalize();

    // Couldn't install midlets successfully
    // reset the JAVA ROOT PATH (remove all may-have-been created files
    if (JAVACALL_OK != jres) {
        preload_i = preload_list;
        for (; preload_i->name!=NULL; preload_i++) {
            __uninstall_preload_i(preload_i);
        }
    }
    
}
