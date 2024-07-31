#ifndef __ACPI_SYS__
#define __ACPI_SYS__
#define HAVE_TIMEGM
#include <stdio.h>
#include <time.h>

typedef int AcpiMutex;

#ifdef __WINDOWS__
#include<winsock2.h>
#include<process.h>
# define ACPI_TIMEVAL SYSTEMTIME
#else
# ifdef __ARMCC_VERSION
typedef struct acpi_timeval_tag {
        long    tv_sec;         /* seconds */
        long    tv_usec;        /* and microseconds */
} ACPI_TIMEVAL;
# else
# define ACPI_TIMEVAL struct timeval
# endif
#endif

#ifdef __ARMCC_VERSION
typedef struct acpi_timezone_tag{
    int tz_minuteswest;		/* Minutes west of GMT.  */
    int tz_dsttime;		        /* Nonzero if DST is ever in effect.  */
} ACPI_TIMEZONE;
/* tz_dsttime is for daylight conpensation
* DST_NONE       //not in use
* DST_USA         //usa
* DST_AUST       //australia
* DST_WET        //west europe
* DST_MET         //midle europe
* DST_EET         //east europe
* DST_CAN        //canada
* DST_GB          //greate britain
* DST_RUM        //romania
* DST_TUR        //turkey
* DST_AUSTALT //australia after 1986
*/
#else
# define ACPI_TIMEZONE long
#endif

#ifndef __ARMCC_VERSION

#define acpi_time_t    time_t
#define acpi_tm    tm
#define acpi_clock_t clock_t
#else

#define acpi_time_t    unsigned long
#define acpi_clock_t   long
typedef struct acpi_tm
{
    int tm_sec;			/* Seconds.	[0-60] (1 leap second) */
    int tm_min;			/* Minutes.	[0-59] */
    int tm_hour;			/* Hours.	[0-23] */
    int tm_mday;			/* Day.		[1-31] */
    int tm_mon;			/* Month.	[0-11] */
    int tm_year;			/* Year	- 1900.  */
    int tm_wday;			/* Day of week.	[0-6] */
    int tm_yday;			/* Days in year.[0-365]	*/
    int tm_isdst;			/* DST.		[-1/0/1]*/
# ifdef	__USE_BSD
    long tm_gmtoff;		/* Seconds east of UTC.  */
    const char *tm_zone;	/* Timezone abbreviation.  */
# else
    long __tm_gmtoff;		/* Seconds east of UTC.  */
    const char *__tm_zone;	/* Timezone abbreviation.  */
# endif    /*__USE_BSD*/
}ACPI_TM;

#endif   /*__ARMCC_VERSION*/

#ifdef __cplusplus
extern "C"{
#endif

void *acpi_malloc(size_t size);
void acpi_free(void *pt);

char *acpi_strdup(char *str1);

#ifdef HAVE_TIMEGM
/** @ time_t acpi_timegm(struct tm *time)
  * \ brief Get the Greenwich Meaning time formatted as UTC
  * @ param time:
  *           A pointer to a structure stores local time
  * @ return:
  *           The corresponding GM time formatted as UTC
  *  
  */
acpi_time_t acpi_timegm(struct acpi_tm *time);
#endif
struct acpi_tm *acpi_gmtime(acpi_time_t *timep);
acpi_time_t acpi_time(acpi_time_t *t);
int acpi_gettimeofday(ACPI_TIMEVAL *tv, ACPI_TIMEZONE *tz);
acpi_time_t acpi_mktime (struct acpi_tm *tp);
struct acpi_tm *acpi_localtime (acpi_time_t *t);
void acpi_tzset(void);

AcpiMutex *acpi_createMutex();
int acpi_getMutex(AcpiMutex *mutex);
int acpi_releaseMutex(AcpiMutex *mutex);
int acpi_deleteMutex(AcpiMutex *mutex);

int acpi_rand(void);
void acpi_srand (unsigned int seed);

acpi_clock_t acpi_clock(void);

#ifdef __cplusplus
}
#endif

#endif
