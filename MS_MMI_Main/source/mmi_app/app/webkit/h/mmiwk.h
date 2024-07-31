#ifndef _MMIWK_H_
#define _MMIWK_H_

#include "sci_types.h"
#include "mmk_type.h"

#ifdef __cplusplus
extern   "C" {
#endif
PUBLIC int MMIWK_Exit(uint16 exit_type);
PUBLIC int MMIWK_OutOfMemory(void);

//PUBLIC int MMIWK_Minimize(void);

#ifdef __cplusplus
}
#endif

#endif//_MMIWK_H_
