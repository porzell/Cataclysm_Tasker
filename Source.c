#include "c_task.h"
#include "c_memory.h"

#include <stdio.h>

typedef struct
{
	//bool allocated;
	int num;
	int starting_num;
	int interval;
	int last_num;
} print_numbers_t;

TASK_STATUS print_numbers(ctask_t *task, TASK_MESSAGE msg, void *data)
{
	//print_numbers_t *data = (print_numbers_t*)task->task_data;
	switch (msg)
	{
		case TASK_INIT:
		{
			printf("%s: Init\n", task->task_name);
			if (!task->task_data)
			{
				task->task_data = c_malloc(sizeof(print_numbers_t));
				print_numbers_t *data = (print_numbers_t*)task->task_data;

				//data->allocated = true;
				data->interval = 1000;
				data->last_num = 10;
				data->starting_num = 2;
				data->num = data->starting_num;

				//task->next_tick += data->interval;
			}
			else
			{
				print_numbers_t *data = (print_numbers_t*)task->task_data;
				
				//data->allocated = false;
				data->num = data->starting_num;

				//task->next_tick += data->interval;
			}

			return TASK_CONTINUE;
		}

		case TASK_TICK:
		{
			print_numbers_t *data = (print_numbers_t*)task->task_data;
			if (data->num == data->last_num)
			{
				printf("%s: Leaving...\n", task->task_name);
				return TASK_FINISHED;
			}
			printf("%s: %i\n", task->task_name, ((print_numbers_t*)task->task_data)->num++);

			task->next_tick += data->interval;
			break;
		}

		case TASK_CLEANUP:
		{
			printf("%s: Cleaning up...\n", task->task_name);
			
			print_numbers_t *data = (print_numbers_t*)task->task_data;
			
			if(task->task_data /*&& data->allocated*/)
			{
				printf("%s: Freeing data because allocated...\n", task->task_name);
				c_free(data);
				printf("%s: We're ok!\n", task->task_name);
			}
			//else
				//printf("%s: Not freeing data because given from outside...\n", task->task_name);
			
			break;
		}

		default:
			printf("%s: Unknown\n", task->task_name);
			return TASK_UNKNOWN_MSG;
	}
}

#define HIGHEST_PRIORITY 0

int main()
{
	print_numbers_t *num_data = c_malloc(sizeof(print_numbers_t));
	
	num_data->num = 0;
	num_data->interval = 100;
	num_data->starting_num = 5;
	num_data->last_num = 8;
	
	printf("Time %llu\n", (uint64_t)SYS_getTicks());
	
	ctask_t *task1 = create_task("task1", print_numbers, num_data, SYS_getTicks()+350, 8);
	ctask_t *task2 = create_task("task2", print_numbers, NULL, SYS_getTicks()+000, 7);

	while (true)
	{
		run_tasks();
	}
};