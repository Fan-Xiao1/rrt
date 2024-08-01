/*
 * Copyright (c) 2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-07-06     Supperthomas first version
 * 2023-12-03     Meco Man     support nano version
 */
#include <hello.h>
#include <board.h>
#include <rtthread.h>
#include <drv_gpio.h>
#ifndef RT_USING_NANO
#include <rtdevice.h>
#endif /* RT_USING_NANO */

#define THREAD_STACK_SIZE 1024
#define THREAD_PRIORITY 25
#define THREAD_TIMESLICE 5

int count = 0;
// 线程1的入口函数
void thread1_entry(void *parameter)
{
    
    while (1)
    {
        rt_kprintf("Thread 1: %d\n", count++);
        rt_thread_mdelay(1000); // 延迟1000毫秒
    }
}

// 线程2的入口函数
void thread2_entry(void *parameter)
{
    
    while (1)
    {
        rt_kprintf("Thread 2: %d\n", count++);
        rt_thread_mdelay(500); // 延迟500毫秒
    }
}

// 主函数
int main(void)
{
    

    return 0;
}


