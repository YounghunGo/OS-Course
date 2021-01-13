#include <linux/list.h>
#include <linux/slab.h>

static LIST_HEAD(my_list);//head 설정
struct sched_my_entity *curr_list;//현재entity
static struct my_rq *my_rq;//런큐

struct sched_my_entity// 런큐에 enqueue할 자료구조
{
	struct task_struct *ts;//task 정보
	struct list_head run_list;//prev, next
};


static void enqueue_my_entity(struct sched_my_entity *my_se)//entity를 enqueue
{
	list_add_tail(&my_se->run_list, &my_list);
}

static void dequeue_my_entity(void)//entity를 dequeue
{
	kfree(my_list.next);
	list_del(my_list.next);
}


static void check_preempt_curr_myfifo(struct rq *rq, struct task_struct *p, int flags)
{
	/* we're never preempted */
	printk(KERN_INFO "MYMOD: check_preempt_curr_fifo CALLED\n");
}

static struct task_struct *pick_next_task_myfifo(struct rq *rq, struct task_struct *prev, struct rq_flags *rf)//task 반환
{
	struct sched_my_entity *my_se;
	
	my_se = list_entry(my_list.next, struct sched_my_entity, run_list);//next entry 받아옴
	
	struct task_struct *task=my_se->ts;

	return task;
}

static void enqueue_task_myfifo(struct rq *rq, struct task_struct *p, int flags)
{       
	struct sched_my_entity *my_se;
        my_se = kmalloc(sizeof(my_se), GFP_KERNEL);//entity 동적할당
	my_se->ts = p;;//entity에 task_struct 정보 저장 할 수 있게끔
	enqueue_my_entity(my_se);//entity enqueue
	printk(KERN_INFO "MYMOD: enqueue_task_fifo CALLED task = %p\n", p);
}

static void dequeue_task_myfifo(struct rq *rq, struct task_struct *p, int flags)
{       
	dequeue_my_entity();
}
		
static void yield_task_myfifo(struct rq *rq)
{
	printk(KERN_INFO "MYMOD: yield_task_fifo CALLED\n");
	BUG();
}

static void put_prev_task_myfifo(struct rq *rq, struct task_struct *prev)
{

}

static void task_tick_myfifo(struct rq *rq, struct task_struct *curr, int queued)
{
	//do not use printk() in this function !!       
	////printk(KERN_DEBUG "MYMOD: task_tick_fifo CALLED\n");

}

static void set_curr_task_myfifo(struct rq *rq)
{
	printk(KERN_INFO "MYMOD: set_curr_task_fifo CALLED\n");
}

static void switched_to_myfifo(struct rq *rq, struct task_struct *new)
{
	printk(KERN_INFO "MYMOD: switched_to_fifo CALLED new = %p\n", new);
}


static void prio_changed_myfifo(struct rq *rq, struct task_struct *p, int oldprio)
{
	printk(KERN_INFO "MYMOD: prio_changed_fifo CALLED\n");
}

static unsigned int get_rr_interval_myfifo(struct rq *rq, struct task_struct *task)
{
	printk(KERN_INFO "MYMOD: get_rr_interval_fifo CALLED\n");
	return 0;
}

static void update_curr_myfifo(struct rq *rq)
{
}

