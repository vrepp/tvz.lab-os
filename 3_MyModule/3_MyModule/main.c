//
//  main.c
//  3_MyModule
//
//  Created by Valentin Rep on 26/10/2018.
//  Copyright © 2018 ViCon. All rights reserved.
//

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kern_levels.h>
#include <linux/module.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("OS - mojmodul");
MODULE_AUTHOR("Valentin Rep");

int main_init(void) {
    printk(KERN_INFO "mojmodul: Loading kernel module\n");
    printk(KERN_INFO "mojmodul: Num jiffies: %lu; Num seconds: %lu\n", jiffies, jiffies/HZ);
    
    return 0;
}

void main_exit(void) {
    printk(KERN_INFO "mojmodul: Unloading kernel module\n");
    printk(KERN_INFO "mojmodul: Num jiffies: %lu; Num seconds: %lu\n", jiffies, jiffies/HZ);
}

module_init(main_init);
module_exit(main_exit);
