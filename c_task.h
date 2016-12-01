#pragma once
#ifndef _CATACLYSM_TASK_H_
#define _CATACLYSM_TASK_H_

#include "c_types.h"
#include "c_time.h"

#define TASK_DEBUG

///===============================
///	Task duties:
///===============================
///	A taskfunc_t must implement a message handler that is split into responses to three messages:
///		1. TASK_INIT
///			Called on task creation.  Allocate and initialize task_data here if used and not pre-allocated before call to create_task().
///		2. TASK_TICK
///			Called whenever next_update >= the current game time.  Be sure to set next_update at the end!
///		3. TASK_CLEANUP
///			Called immediately before task deletion and removal from task list.  Deinit and deallocate task_data here if used.
///
///		Setting is_moribund to 1 will cause the task handler to call the task function with a TASK_CLEANUP message, then remove it from the list.
///		This allows a task to deschedule itself in an orderly and well-behaved fashion.

///		Tasks can also deschedule themselves immediately by returning TASK_FINISHED.

///@brief An enumeration type to describe task messages dispatched to a task callback.
typedef enum
{
	TASK_INIT,
	TASK_TICK,
	TASK_CLEANUP
} TASK_MESSAGE;

///@brief Response codes to be returned by a task callback.
typedef enum
{
	TASK_UNKNOWN_MSG = -2,
	TASK_ERROR = -1,
	TASK_CONTINUE = 0,
    TASK_FINISHED = 1
} TASK_STATUS;

//============================================================================

typedef struct ctask_t ctask_t;

///@brief A type for task callback functions.
typedef TASK_STATUS (*task_func_t)(struct ctask_t*, TASK_MESSAGE, void*);

//============================================================================

#define TASK_NAME_SIZE	32

///@brief A structure to hold all data associated with a task.
struct ctask_t
{
	#ifdef TASK_DEBUG
	///@brief A human-readable task name.
	char		task_name[TASK_NAME_SIZE];
	#endif //TASK_DEBUG
	
	///@brief Tells the task dispatcher to remove the task at next update.
	bool		is_moribund;
	
	#ifdef TASK_DEBUG
	///@brief Used so that the dispatcher can detect an unruly task.
	ctime_t		last_tick;
	#endif //TASK_DEBUG
	
	///@brief The starting time for the task.
	ctime_t		next_tick;
	
	///@brief A pointer to hold a task specific data block.
	void		*task_data;
	
	///@brief 
	task_func_t	task_func;
};

//============================================================================

/// @brief All tasks should be created by a call to this function.  Allocates and enqueues a task.
/// @param[in] task_name	A string name for the task.
/// @param[in] task_func	A task function pointer with a task message handler inside.
/// @param[in] data			A pointer to preallocated task data, if extant.  (Can be NULL if task manages own data.)
/// @param[in] next_tick	A time value for the next time task_func should be called with TASK_TICK.
/// @param[in] priority		The priority level of the task.  (Lower priority and more recent tasks (in the case of a tie) will be executed first in a task list run.)
/// @return Returns a ctask_t.  If any problems arise, a NULL pointer shall be returned.
ctask_t* create_task(char *task_name, task_func_t task_func, void *data, ctime_t next_tick, uint16_t priority);

/// @brief Used to retrieve the task priority level of a task within the task list.
/// @param[in] task	A pointer to the task to be examined.
uint16_t get_task_priority(ctask_t *task);

/// @brief Used to reset the priority of a task already assigned to the task list.
/// @param[in] task		A pointer to the task to be affected.
/// @param[in] priority	The priority level to which the task should be set.
/// @warning UNFINISHED!
void set_task_priority(ctask_t *task, uint16_t priority);

/// @brief Passes a TASK_CLEANUP message to a task, then removes the task from the running list and deallocates it.
/// @param[in] task	A pointer to the task to be removed.
void remove_task(ctask_t *task);

/// @brief Runs through the task list once.
void run_tasks();

#endif