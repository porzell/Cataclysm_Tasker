     ,-----.,--.                  ,--. ,---.   ,--.,------.  ,------.
    '  .--./|  | ,---. ,--.,--. ,-|  || o   \  |  ||  .-.  \ |  .---'
    |  |    |  || .-. ||  ||  |' .-. |`..'  |  |  ||  |  \  :|  `--, 
    '  '--'\|  |' '-' ''  ''  '\ `-' | .'  /   |  ||  '--'  /|  `---.
     `-----'`--' `---'  `----'  `---'  `--'    `--'`-------' `------'
    ----------------------------------------------------------------- 

# Cataclysm Tasker

This is a test of a C priority-based, cooperatively-scheduled tasking system.

Since the Cataclysm scheduler is "cooperative", this means that execution of a task shall not be interrupted until the task update function returns with a return code.  As a direct result, care must be taken to ensure that no task will hog the CPU for an unacceptably large amount of time, so tasks should be kept short and sweet .

It is primarily intended as a barebones tasker for running multiple processes without having a full OS or in situations where timing is not critical and the operations of tasks are not at great risk of blocking other tasks from running for extended periods of time.

## Task duties:
A taskfunc_t must implement a message handler that is split into responses to three messages:
#### 1. TASK_INIT
Called on task creation.  Allocate and initialize task_data here if used and not pre-allocated before call to create_task().
#### 2. TASK_TICK
Called whenever next_update >= the current game time.  Be sure to set next_update at the end, or your task will not update!
#### 3. TASK_CLEANUP
Called immediately before task deletion and removal from task list.  Deinit and deallocate task_data here if used.

### Info
- Setting is_moribund to 1 will cause the task handler to call the task function with a TASK_CLEANUP message, then remove it from the list.
- This allows a task to deschedule itself in an orderly and well-behaved fashion.
- Tasks can also deschedule themselves in a more immediate fashion by returning TASK_FINISHED and should take care to free all allocated resources before doing so.

## Future work
### Fixing priority system
The priority system needs to be rewritten, and because the task system is currently "cooperative", only really is useful for determining the order that tasks are brought to focus in.
### A better task list data structure
The task list structure is currently a singly-linked list.
### Possibility for real-time capabilities
An alternate timer interrupt-scheduled version of the scheduler for use in real-time applications is in the works. In this system, tasks still have the opportunity to yield, however, will be interrupted if they do not yield within their respective set timeslice "quotas" or in the event of a higher priority task requiring an update.
