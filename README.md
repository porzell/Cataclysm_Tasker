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
Called whenever next_update >= the current game time.  Be sure to set next_update at the end!
#### 3. TASK_CLEANUP
Called immediately before task deletion and removal from task list.  Deinit and deallocate task_data here if used.

### Info
- Setting is_moribund to 1 will cause the task handler to call the task function with a TASK_CLEANUP message, then remove it from the list.
- This allows a task to deschedule itself in an orderly and well-behaved fashion.
- Tasks can also deschedule themselves immediately by returning TASK_FINISHED.

## Future work
An alternate interrupt-scheduled version of the scheduler for use in real-time applications is in the works currently. In this system, tasks will still have the opportunity to yield, however, will be interrupted if they do not yield before their individually set timeslice "quota" or a higher priority task requires an update.
