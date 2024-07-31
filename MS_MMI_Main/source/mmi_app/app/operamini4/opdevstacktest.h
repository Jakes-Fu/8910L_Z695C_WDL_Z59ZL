/* -*- Mode: c++; tab-width: 4; c-basic-offset: 4 -*-
 *
 * Copyright (C) 1995-2009 Opera Software AS.  All rights reserved.
 *
 * This file is part of the Opera web browser.
 * It may not be redistributed under any circumstances.
 */

#ifndef OPDEV_STACK_TEST_H
#define OPDEV_STACK_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
 OD_STACKTEST_BREAM_TASK = 0,
 OD_STACKTEST_CORE_TASK = 1,
 OD_STACKTEST_TASK_NUMBER
} OD_STACKTEST_TASKID;

void OD_StackTest_init(void);
void OD_StackTest_initTaskInfo(OD_STACKTEST_TASKID st_tid); 

void OD_StackTest_printMaxStackDepth(void);

#ifdef __cplusplus
};
#endif

#endif /* OPDEV_STACK_TEST_H */
