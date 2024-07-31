// fake of pthread.c
#include <errno-base.h>
#include <pthread.h>

#include "os_api.h"

#define  MUTEX_NAME  "FAKE_FOR_WEBKIT"

/*******************************************************************/
//  Interface:		pthread_mutex_init
//  Description : 	pthread_mutex_init
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
int pthread_mutex_init(pthread_mutex_t *mutex,
					   const pthread_mutexattr_t *attr)
{
	uint32 priority_inherit = SCI_INHERIT;
	SCI_MUTEX_PTR	*mutex_ptr	= SCI_NULL;
	
	if (SCI_NULL == mutex)
		return EINVAL;
	
	if(SCI_NULL != attr)
	{
		priority_inherit = *attr;
	}
	
	mutex_ptr = SCI_CreateMutex(MUTEX_NAME, priority_inherit);
	
	if(SCI_NULL == mutex_ptr)
		return EINVAL;
	
	mutex->value = (int)mutex_ptr;
	
	return 0;      
}

/*******************************************************************/
//  Interface:		pthread_mutex_destroy
//  Description : 	pthread_mutex_destroy
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
int pthread_mutex_destroy(pthread_mutex_t *mutex)
{  
	if (SCI_NULL == mutex)
		return EINVAL;
	
	SCI_DeleteMutex((SCI_MUTEX_PTR)mutex->value);
	
	return 0;
}

/*******************************************************************/
//  Interface:		pthread_mutex_lock
//  Description : 	pthread_mutex_lock
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
int pthread_mutex_lock(pthread_mutex_t *mutex)
{
	uint32 status = 0;
	
	if (SCI_NULL == mutex)
		return EINVAL;
	
	status = SCI_GetMutex((SCI_MUTEX_PTR)mutex->value, SCI_WAIT_FOREVER);
	if(SCI_SUCCESS != status)
	{
		return EAGAIN;
	}
	
	return 0;
}

/*******************************************************************/
//  Interface:		pthread_mutex_unlock
//  Description : 	pthread_mutex_unlock
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	if (SCI_NULL == mutex)
		return EINVAL;
	
	SCI_PutMutex((SCI_MUTEX_PTR)mutex->value);
	
	return 0;
}
