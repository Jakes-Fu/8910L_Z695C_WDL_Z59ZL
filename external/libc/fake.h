#ifndef _FAKE_H_
#define _FAKE_H_

#if defined(__cplusplus)
#define	__BEGIN_DECLS		extern "C" {
#define	__END_DECLS		}
#else
#define	__BEGIN_DECLS
#define	__END_DECLS
#endif

__BEGIN_DECLS

#if 0
#define SPLIBC_FAKE_LOG			SCI_TRACE_LOW
#else
#define SPLIBC_FAKE_LOG
#endif

typedef unsigned long sigset_t;
typedef int clockid_t;
typedef	int int32_t;
typedef	unsigned int uint32_t;
typedef unsigned int size_t;

// memory
void *SPLIBC_malloc( size_t size );
void SPLIBC_free( void *memblock );
void *SPLIBC_calloc( size_t num, size_t size );
void *SPLIBC_realloc( void *memblock, size_t size );
void *SPLIBC__alloca( size_t size );

// file 
//#include "sfs.h"

/*
#define SEEK_SET 		SFS_SEEK_BEGIN  
#define SEEK_CUR 		SFS_SEEK_CUR  
#define SEEK_END 		SFS_SEEK_END 
*/

#include <stdio.h>

FILE * SPLIBC_fopen(const char * path,const char * mode);
int SPLIBC_fclose(FILE * stream);
int SPLIBC_feof(FILE * stream);
size_t SPLIBC_fread(void * ptr,size_t size,size_t nmemb,FILE * stream);
size_t SPLIBC_fwrite(const void * ptr,size_t size,size_t nmemb,FILE * stream);
int SPLIBC_fseek(FILE * stream,long offset,int whence);
long SPLIBC_ftell(FILE * stream);
void SPLIBC_rewind(FILE * stream);

int SPLIBC_fileno(FILE * stream);
char * SPLIBC_mktemp(char * temp);
void SPLIBC_setbuf(FILE * stream,char * buf);
void SPLIBC_setbuffer(FILE * stream,char * buf,size_t size);
int SPLIBC_fflush(FILE* stream);
FILE * SPLIBC_freopen(const char * path, const char * mode,FILE * stream);
FILE * SPLIBC_fdopen(int fildes,const char * mode);

//io
int SPLIBC_printf( const char *format,... );
int SPLIBC__printf( const char *format,... );
int SPLIBC_fprintf( FILE *stream, const char *format,...);
int SPLIBC__fprintf( FILE *stream, const char *format,...);
int SPLIBC_getc(FILE * stream);
int SPLIBC_getchar(void);
int SPLIBC_fputc(int c,FILE * stream);
int SPLIBC_fputs(const char * s,FILE * stream);
int SPLIBC_fgetc(FILE * stream);
char* SPLIBC_fgets(char * s,int size,FILE * stream);
char* SPLIBC_gets(char *s);
int SPLIBC_putc(int c,FILE * stream);

__END_DECLS

#endif
