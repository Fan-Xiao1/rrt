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
    rt_thread_t thread1 = RT_NULL;
    rt_thread_t thread2 = RT_NULL;

    // 创建线程1
    thread1 = rt_thread_create("thread1",
                               thread1_entry,
                               RT_NULL,
                               THREAD_STACK_SIZE,
                               THREAD_PRIORITY,
                               THREAD_TIMESLICE);

    // 如果线程1创建成功，启动线程1
    if (thread1 != RT_NULL)
    {
        rt_thread_startup(thread1);
    }
    else
    {
        rt_kprintf("Thread 1 creation failed!\n");
    }

    // 创建线程2
    thread2 = rt_thread_create("thread2",
                               thread2_entry,
                               RT_NULL,
                               THREAD_STACK_SIZE,
                               THREAD_PRIORITY , // 设置不同的优先级以观察调度行为
                               THREAD_TIMESLICE);

    // 如果线程2创建成功，启动线程2
    if (thread2 != RT_NULL)
    {
        rt_thread_startup(thread2);
    }
    else
    {
        rt_kprintf("Thread 2 creation failed!\n");
    }

    return 0;
}


