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
static rt_mutex_t mutex = RT_NULL;
static rt_mailbox_t mb = RT_NULL;

// 消息邮箱的大小
#define MB_SIZE 32

// 线程1的入口函数
void thread1_entry(void *parameter)
{
    int count = 0;
    while (1)
    {
        // 加锁保护共享资源
        rt_mutex_take(mutex, RT_WAITING_FOREVER);

        // 发送消息到消息邮箱
        if (rt_mb_send(mb, (rt_ubase_t)count) == RT_EOK)
        {
            rt_kprintf("Thread 1: sent message - %d\n", count);
        }
        else
        {
            rt_kprintf("Thread 1: failed to send message\n");
        }
        count++;

        // 解锁
        rt_mutex_release(mutex);

        rt_thread_mdelay(1000); // 延迟1000毫秒（1秒）
    }
}

// 线程2的入口函数
void thread2_entry(void *parameter)
{
    rt_ubase_t msg;
    while (1)
    {
        // 从消息邮箱接收消息
        if (rt_mb_recv(mb, &msg, RT_WAITING_FOREVER) == RT_EOK)
        {
            // 加锁保护共享资源
            rt_mutex_take(mutex, RT_WAITING_FOREVER);

            rt_kprintf("Thread 2: received message - %d\n", msg);

            // 解锁
            rt_mutex_release(mutex);
        }
        else
        {
            rt_kprintf("Thread 2: failed to receive message\n");
        }
    }
}

// 主函数
int main(void)
{
    rt_thread_t thread1 = RT_NULL;
    rt_thread_t thread2 = RT_NULL;

    // 创建互斥量
    mutex = rt_mutex_create("mutex", RT_IPC_FLAG_PRIO);
    if (mutex == RT_NULL)
    {
        rt_kprintf("Failed to create mutex\n");
        return -1;
    }

    // 创建消息邮箱
    mb = rt_mb_create("mb", MB_SIZE, RT_IPC_FLAG_PRIO);
    if (mb == RT_NULL)
    {
        rt_kprintf("Failed to create mailbox\n");
        rt_mutex_delete(mutex);
        return -1;
    }

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
        rt_mutex_delete(mutex);
        rt_mb_delete(mb);
        return -1;
    }

    // 创建线程2
    thread2 = rt_thread_create("thread2",
                               thread2_entry,
                               RT_NULL,
                               THREAD_STACK_SIZE,
                               THREAD_PRIORITY + 1, // 设置不同的优先级以观察调度行为
                               THREAD_TIMESLICE);

    // 如果线程2创建成功，启动线程2
    if (thread2 != RT_NULL)
    {
        rt_thread_startup(thread2);
    }
    else
    {
        rt_kprintf("Thread 2 creation failed!\n");
        rt_mutex_delete(mutex);
        rt_mb_delete(mb);
        return -1;
    }

    return 0;
}
