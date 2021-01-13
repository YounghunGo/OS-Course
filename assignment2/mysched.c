#include <linux/module.h>
#include "../sched.h"
#include "fifo.c"
/*
 * HCPARK: my scheduling class: ONE_PROCESS_SCHED
 *
 */

//extern void (*set_class_my) (struct task_struct *p);//sched.h에 적어놓음
extern int CONFIG_MYSCHED;
extern struct sched_class* classpointer;

struct task_struct *task;

static void
check_preempt_curr_fifo(struct rq *rq, struct task_struct *p, int flags)
{
	/* we're never preempted */
	printk(KERN_INFO "MYMOD: check_preempt_curr_fifo CALLED\n");
}

static struct task_struct *
pick_next_task_fifo(struct rq *rq, struct task_struct *prev, struct rq_flags *rf)
{
	//carefully use printk() in this function !!	
	//printk(KERN_DEBUG "MYMOD: pick_next_task_fifo CALLED\n");

	if(task!=NULL) {
		put_prev_task(rq, prev);
		//printk(KERN_DEBUG "MYMOD: pick_next_task_fifo return task p\n");
		return task;
	}
		
	return NULL;
}

static void
enqueue_task_fifo(struct rq *rq, struct task_struct *p, int flags)
{
	printk(KERN_INFO "MYMOD: enqueue_task_fifo CALLED task = %p\n", p);
	add_nr_running(rq, 1);//런큐에 동작중인 엔티티의 수를 증가

	task = p;
}

static void
dequeue_task_fifo(struct rq *rq, struct task_struct *p, int flags)
{
	printk(KERN_INFO "MYMOD: dequeue_task_fifo CALLED\n");
	sub_nr_running(rq, 1);//런큐에 동작중인 엔티티의 수를 감소

	task = NULL;
}

static void yield_task_fifo(struct rq *rq)
{
	printk(KERN_INFO "MYMOD: yield_task_fifo CALLED\n");
	BUG();
}

static void put_prev_task_fifo(struct rq *rq, struct task_struct *prev)
{

}

static void task_tick_fifo(struct rq *rq, struct task_struct *curr, int queued)
{
	//do not use printk() in this function !!	
	//printk(KERN_DEBUG "MYMOD: task_tick_fifo CALLED\n");
}

static void set_curr_task_fifo(struct rq *rq)
{
	printk(KERN_INFO "MYMOD: set_curr_task_fifo CALLED\n");
}

static void switched_to_fifo(struct rq *rq, struct task_struct *new)
{
	printk(KERN_INFO "MYMOD: switched_to_fifo CALLED new = %p\n", new);
}

static void
prio_changed_fifo(struct rq *rq, struct task_struct *p, int oldprio)
{
	printk(KERN_INFO "MYMOD: prio_changed_fifo CALLED\n");
}

static unsigned int
get_rr_interval_fifo(struct rq *rq, struct task_struct *task)
{
	printk(KERN_INFO "MYMOD: get_rr_interval_fifo CALLED\n");
	return 0;
}

static void update_curr_fifo(struct rq *rq)
{
}

const struct sched_class my_sched_class = { //현재는 fifo.c의 알고리즘으로 설정한 상태
	.next			= &idle_sched_class,

	.enqueue_task		= enqueue_task_myfifo,
	.dequeue_task		= dequeue_task_myfifo,
	.yield_task		= yield_task_myfifo,

	.check_preempt_curr	= check_preempt_curr_myfifo,

	.pick_next_task		= pick_next_task_myfifo,
	.put_prev_task		= put_prev_task_myfifo,


	.set_curr_task          = set_curr_task_myfifo,
	.task_tick		= task_tick_myfifo,

	.get_rr_interval	= get_rr_interval_myfifo,

	.prio_changed		= prio_changed_myfifo,
	.switched_to		= switched_to_myfifo,
	.update_curr		= update_curr_myfifo,
};

void myclass (struct task_struct *p) {
	printk(KERN_INFO "MYMOD: myclass CALLED\n");
}

static int __init init_mysched(void)
{
	const struct sched_class *class;	
	
	CONFIG_MYSCHED = 1;//core.c에 존재하는 변수의 값을 0 -> 1로 변경
	classpointer = &my_sched_class;//core.c에 존재하는 classpointer의 값에 my_sched_class 주소를 반환
	fair_sched_class.next = &my_sched_class;//fair 클래스의 next를 my_sched로 설정

	for_each_class(class)
		printk(KERN_INFO "INIT_MOD: class = %p\n", class);
	
	INIT_LIST_HEAD(&my_list);//fifo 알고리즘의 head 설정
	return 0;
	
}

static void __exit exit_mysched(void)
{
	
	const struct sched_class *class;
	
	CONFIG_MYSCHED = 0;
	classpointer = NULL;
	fair_sched_class.next = &idle_sched_class;

	for_each_class(class)
		printk(KERN_INFO "EXIT_MOD: class = %p\n", class);
	return;

}

MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Your Scheduler");
MODULE_LICENSE("GPL");

module_init(init_mysched)
module_exit(exit_mysched)


