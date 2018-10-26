//
//  main.c
//  3_AllProcesses
//
//  Created by Valentin Rep on 26/10/2018.
//  Copyright © 2018 ViCon. All rights reserved.
//

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kern_levels.h>
#include <linux/module.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("OS - sviprocesi");
MODULE_AUTHOR("Valentin Rep");

void list_all_processes(void)
{
    struct task_struct* task;
    
    for_each_process(task) {
        printk(KERN_INFO " -> %s [%d] (%ld) {%d}\n", task->comm, task->pid, task->state, task->parent->pid);
    }
}

int main_init(void) {
    printk(KERN_INFO "sviprocesi: Loading kernel module\n");
    printk(KERN_INFO "sviprocesi: List of all processes:\n");
    
    list_all_processes();
    
    return 0;
}

void main_exit(void) {
    printk(KERN_INFO "sviprocesi: Unloading kernel module\n");
}

module_init(main_init);
module_exit(main_exit);
