#include <rtthread.h>
#include <rtdevice.h>
#include <drv_gpio.h>
#if defined(BSP_USING_LED)

#define DRV_DEBUG

#define LOG_TAG "drv.led"
#include <drv_log.h>

#define  GPIO_LED_PIN   GET_PIN(F,12)

struct led_device
{
    struct rt_device parent; // 继承 rt_device 结构体
};

static struct led_device led_dev;
static rt_err_t led_init(rt_device_t dev)
{
    rt_pin_mode(GPIO_LED_PIN,PIN_MODE_OUTPUT);
    rt_pin_write(GPIO_LED_PIN,PIN_HIGH); // 默认关闭LED
    return RT_EOK;
}

// 设备打开函数
static rt_err_t led_open(rt_device_t dev, rt_uint16_t oflag)
{
    return RT_EOK;
}

// 设备关闭函数
static rt_err_t led_close(rt_device_t dev)
{
    return RT_EOK;
}

// 设备读函数（这里不实现，因为LED不支持读操作）
static rt_size_t led_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    return 0;
}

// 设备写函数
static rt_size_t led_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
    if (size != 1)
    {
        return 0;
    }

    if (*(char *)buffer == 1)
    {
        rt_pin_write(GPIO_LED_PIN,PIN_LOW); // 打开LED
    }
    else
    {
        rt_pin_write(GPIO_LED_PIN,PIN_HIGH); // 关闭LED
    }

    return size;
}

// 设备控制函数
static rt_err_t led_control(rt_device_t dev, int cmd, void *args)
{
    switch (cmd)
    {
    case 0:
        rt_pin_write(GPIO_LED_PIN,PIN_HIGH); // 关闭LED
        break;
    case 1:
        rt_pin_write(GPIO_LED_PIN,PIN_LOW); // 打开LED
        break;
    default:
        return -RT_ERROR;
    }

    return RT_EOK;
}

int rt_hw_led_device_init(void)
{
    struct rt_device *device = &(led_dev.parent);

    // 设置设备类型和设备标志
    device->type = RT_Device_Class_Char;
    device->rx_indicate = RT_NULL;
    device->tx_complete = RT_NULL;

    // 设置设备操作接口
    device->init = led_init;
    device->open = led_open;
    device->close = led_close;
    device->read = led_read;
    device->write = led_write;
    device->control = led_control;

    // 注册设备
    if(rt_device_register(device, "led", RT_DEVICE_FLAG_RDWR)!=RT_EOK)
    {
        LOG_E("device register fail");
        return -RT_ERROR;
    }

    return RT_EOK;
}

// 导出到自动初始化
INIT_DEVICE_EXPORT(rt_hw_led_device_init);

#endif

