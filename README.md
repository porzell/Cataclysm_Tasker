     ,-----.,--.                  ,--. ,---.   ,--.,------.  ,------.
    '  .--./|  | ,---. ,--.,--. ,-|  || o   \  |  ||  .-.  \ |  .---'
    |  |    |  || .-. ||  ||  |' .-. |`..'  |  |  ||  |  \  :|  `--, 
    '  '--'\|  |' '-' ''  ''  '\ `-' | .'  /   |  ||  '--'  /|  `---.
     `-----'`--' `---'  `----'  `---'  `--'    `--'`-------' `------'
    ----------------------------------------------------------------- 

# Cataclysm Tasker

This is a test of a C priority-based, blocking, scheduled tasking system.
It is primarily intended as a barebones system for running multiple things without having a full OS and in situations where timing is not critical and where the operations of tasks are not at risk to block other things from running.


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
