#include "c_task.h"

#include "string.h"

#include "stdio.h"

#include "c_memory.h"

//============================================================================

typedef struct ctask_link_t ctask_link_t;

///@brief A type to act as an link node for tasks within a task list.
struct ctask_link_t
{
	ctask_t task;
	ctask_link_t *next_link;
	
	uint16_t priority;
};

//============================================================================

static ctask_link_t *first_task_link = {0};

//============================================================================

ctask_t* create_task(char *task_name, task_func_t task_func, void *data, uint64_t next_tick, uint16_t priority)
{
	
	ctask_link_t *task_link = (ctask_link_t*)c_malloc(sizeof(ctask_link_t));
	
	if(!task_link)
		return NULL;
	
	// Get a local reference to the task itself for crappy compilers.
	ctask_t *the_task = &(task_link->task);
	
	// -----------------------------------------------------
	
	// 1. Copy over task name.
	
	memcpy(the_task->task_name, task_name, TASK_NAME_SIZE-2);
	
	// Null terminate that puppy.  YOU AH TAHMINAETED.
	the_task->task_name[TASK_NAME_SIZE-1] = '\0';
	
	// -----------------------------------------------------
	
	// 2. Set up the rest of the fields.
	
	the_task->is_moribund = 0;
	//the_task->last_tick = next_tick;
	the_task->next_tick = next_tick;
	
	the_task->task_func = task_func;
	the_task->task_data = data;
	
	task_link->priority = priority;
	
	//------------------------------------------------------
	
	// 3. Link us up, baby!
	
	//// OLD: Shitty non-prioritized list entry.
	////	last_task->next_link = task_link;
	////	last_task = task_link;
	
	// We need to inplace sort the list by priority.
	
	// TODO: Should maybe try to pick an optimal end to start from.
	
	ctask_link_t *cur_link = first_task_link;
	
    if(!cur_link)
    {
        first_task_link = task_link;
    }
    else
    {
    
    	// What if we should be first?
    	if(priority <= first_task_link->priority)
    	{
    		task_link->next_link = first_task_link;
    		
    		first_task_link = task_link;
    	}
    	else
    	{
    		// Find the link after which we shall attach ourselves.
    		while(cur_link->next_link && cur_link->next_link->priority < priority)
    			cur_link = cur_link->next_link;
    		
    		// This COULD be NULL...
    		ctask_link_t *tmp_link = cur_link->next_link;
    		
    		
    		cur_link->next_link = task_link;
    		
    		task_link->next_link = tmp_link;
    	}
    }
	
	// 4. We're done!
	
	// Init the task.
	// TODO: Check task_func retval.
	
	printf("Created task: %s\n", task_name);
	
	the_task->task_func(the_task, TASK_INIT, NULL);

	return the_task;
}

//============================================================================

inline uint16_t get_task_priority(ctask_t *task)
{
    return ((ctask_link_t*)task)->priority;
}

//============================================================================

//TODO: Fix this from the horribly broken and unfinished doubly-linked list implementation.
void set_task_priority(ctask_t *task, uint16_t priority)
{
    /*// Get the list entry.
    // Note: This works because the ctask_t is the first element in a ctask_link_t.
    ctask_link_t *task_link = (ctask_link_t*)task;
    
    uint16_t cur_priority = task_link->priority;
    
    task_link->priority = priority;
    
    // Check if our priority change doesn't need a shift in the list.
    if((!task_link->next_link && priority > cur_priority) || (!task_link->prev_link && priority < cur_priority))
        return;
    
    // Welp, we gotta shift...
    
    if(priority < first_task_link->priority)
    {
        ctask_link_t *tmp_link = first_task_link;
        
        first_task_link = task_link;
        
        // Note: if this case is hit, first_task_link cannot be last_task_link.
        
        // If we hit this case, task_link had better have a prev_link, so we don't have to check >:DO !
        task_link->prev_link->next_link = task_link->next_link;
        
        // TODO: MAKE THIS WORK.  Need to add checks for if task_link was last_task_link and task_link->prev_link will suddenly become last_task_link now.
        if(task_link->next_link)
            //task_link = HERE
        
        task_link->next_link = tmp_link;
        
        task_link->prev_link = tmp_link->prev_link;
        
        tmp_link->prev_link = task_link;
    }
    else if(priority > last_task_link->priority)
    {
        
    }
    
    // This is a weird little optimization idea.  Branch once and store the offset of next_link or prev_link, and use that to keep moving
    // through the list without needing extra code storage.
    
    if( priority > cur_priority )
    {
        ctask_link_t *cur_link = task_link;
        
        // Find the link after which we shall attach ourselves.
        while(cur_link->next_link && cur_link->next_link < cur_priority)
            cur_link = cur_link->next_link;
        
        //if(task_link->next_link)
           //task_link->next_link->prev_link = task_link->prev_link;
        
        // TODO: HERE!
        //if()
    }
    else if( priority < cur_priority )
    {
        ctask_link_t *cur_link = task_link;
        
        // Find the link before which we shall attach ourselves.
        while(cur_link->prev_link && cur_link->prev_link >= cur_priority)
           cur_link = cur_link->prev_link;
       
        // This COULD be NULL...
		//ctask_link_t *tmp_link = cur_link->prev_link;
        
        //task_link->next_link
        
        // We don't need to check if this is NULL because we did when we checked if we needed to shift ourselves in the list.
        task_link->prev_link->next_link = task_link->next_link;
        task_link->prev_link = cur_link->prev_link;
        task_link->next_link = cur_link;
        
        // We DO need to check this, however...
        if(cur_link->prev_link)
        {
            cur_link->prev_link->next_link = task_link;
            cur_link->prev_link = task_link;
        }
    }
    
    // The one fall through case here is if we have been sent the same priority.  No problemo!*/
}

//============================================================================

void remove_task(ctask_t *task)
{
    // First, let the task clean itself up by sending a cleanup message.
    task->task_func(task, TASK_CLEANUP, NULL);
    
    // TODO: Check return value for robustness.
    
    // Get the list entry.
    // Note: This works because the ctask_t is the first element in a ctask_link_t.
    ctask_link_t *task_link = (ctask_link_t*)task;
    
    // Checks for list endpoint tasks.
    if(first_task_link == task_link)
        first_task_link = task_link->next_link;
    else
    {
        // If we had a prev_link, find it and make its next_link our old next_link.
        if(task_link != first_task_link)
        {
            ctask_link_t *prev_link = first_task_link;
            
            while(prev_link->next_link != task_link)
            {
                // TODO: Robustify this maybe with a count to defeat memory corruption?
                prev_link = prev_link->next_link;
            }
            
            // Unlink ourselves politely.
            prev_link->next_link = task_link->next_link;
        }
    }
    
    // Now that the task is delinked, we can delete it.
    c_free(task_link);
    
}

//============================================================================

void run_tasks()
{
    ctask_link_t *the_task = first_task_link;
    
    while(the_task)
    {
        // Is the task marked for death?
        if(the_task->task.is_moribund)
        {
            // Cache a pointer to the next task.
            ctask_link_t *next_task = the_task->next_link;
            
            // Kill the current task.
            remove_task((ctask_t*)the_task);
            
            // Set the current task to the next task.
            the_task = next_task;
            
            continue;
        }
        else
        {
            // Should we update this task?
            if(the_task->task.next_tick <= SYS_getTicks())
            {
                #ifdef TASK_DEBUG
                //uint16_t orig_next_tick = the_task->task.next_tick;
                #endif
                
                // Do an update on the task.
                switch(the_task->task.task_func(&the_task->task, TASK_TICK, NULL))
                {
                    // Is the task ready to be done?
                    case TASK_FINISHED:
                    {
                        // Cache a pointer to the next task.
                        ctask_link_t *next_task = the_task->next_link;
                        
                        // Kill the current task.
                        remove_task((ctask_t*)the_task);
                        
                        // Set the current task to the next task.
                        the_task = next_task;
                        
                        continue;
                        
                        break;
                    }
                    // Reserved for handling future task update return codes.
                    default:
                        break;
                }
                
                #ifdef TASK_DEBUG
                //if(orig_next_tick == the_task->next_tick)
                    //printf("[Scheduler] Task %s did not assign next_tick.\n");
                #endif
            }
        }
        
        // Move on to the next priority task.
        the_task = the_task->next_link;
    }
}