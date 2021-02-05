#ifndef NW_TASK_H
#define NW_TASK_H

#include <stdint.h>

#define NW_MAX_TASK_COUNT       (8)

#define NW_TASK_REPEAT          (0)
#define NW_TASK_ONESHOT         (1)     ///< Mode will be set to SLEEP after one execution
#define NW_TASK_SLEEP           (2)     ///< Won't be scheduled

#define NW_InvalidTask          (-1)
#define NW_IsTaskValid(tid)     (tid > 0)

typedef void(*NW_Functor)(void);
typedef int8_t NW_Priority;             ///< Highest -> [0 .. 127] <- Lowest
typedef int32_t NW_TaskId;
typedef uint8_t NW_Mode;

typedef struct
{
    NW_TaskId   _id;
    NW_Functor  _func;
    NW_Priority _priority;
    NW_Mode     _mode;
} NW_Task, *pNW_Task;

extern NW_Task g_pTasks[NW_MAX_TASK_COUNT];

/**
 * Create a task
 */
NW_TaskId NW_Task_Create(NW_Functor func, NW_Priority priority, NW_Mode mode);

/**
 * Remove a task
 */
int8_t NW_Task_Destroy(NW_TaskId id);

/**
 * Start an existing task, and set its mode to NW_TASK_ONESHOT
 */
int8_t NW_Task_StartSpecific(NW_TaskId id);

int8_t NW_Task_Schedule();

#endif
