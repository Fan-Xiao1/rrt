#include <rtthread.h>
#include <rtdevice.h>

void led_test(void)
{
    rt_device_t dev = rt_device_find("led");
    if (dev)
    {
        char led_on = 1;
        char led_off = 0;

        rt_device_open(dev, RT_DEVICE_OFLAG_RDWR);

        rt_kprintf("Turning LED on...\n");
        rt_device_write(dev, 0, &led_on, 1);
        rt_thread_mdelay(1000);

        rt_kprintf("Turning LED off...\n");
        rt_device_write(dev, 0, &led_off, 1);
        rt_thread_mdelay(1000);

        rt_device_close(dev);
    }
    else
    {
        rt_kprintf("LED device not found!\n");
    }
}

// 导出测试函数到 msh 命令
MSH_CMD_EXPORT(led_test, Test LED device);
