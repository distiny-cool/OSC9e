#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>

static LIST_HEAD(birthday_list);

struct birthday{
	int day;
	int month;
	int year;
	struct list_head list;
};

void add_list(int day,int month,int year,struct birthday *person)
{	person = kmalloc(sizeof(*person),GFP_KERNEL);
	person->day = day;
	person->month = month;
	person->year = year;
	INIT_LIST_HEAD(&person->list);
	list_add_tail(&person->list,&birthday_list);
}

/* This function is called when the module is loaded. */
int simple_init(void)
{
       printk(KERN_INFO "Loading Module\n");

       struct birthday *person[3];
       struct birthday *ptr;

       int i=0;
       while(i<3)
       {
	 int d = 1+i;
	 int m = 4+i;
	 int y = 1998+i;
	 add_list(d,m,y,person[i]); 
	 i=i+1;
       }

       i=0;

       list_for_each_entry(ptr, &birthday_list, list){
	       printk(KERN_INFO "No.%d person's birthday is : %d - %d - %d .\n",i,ptr->year,ptr->month,ptr->day);
	       i=i+1;
       }
       return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
	printk(KERN_INFO "Removing Module\n");
	struct birthday *ptr, *next;
  	list_for_each_entry_safe(ptr, next, &birthday_list, list){
      		list_del(&ptr->list);
      		kfree(ptr);
	}
}


/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

