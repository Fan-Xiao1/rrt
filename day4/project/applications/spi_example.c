#include <rtthread.h>
#include <rtdevice.h>

#include <drv_spi.h>
#include <drv_gpio.h>

static int spi_attach(void)
{
    return rt_hw_spi_device_attach("spi2","spi20", GET_PIN(B,12));
}
// INIT_DEVICE_EXPORT(spi_attach);
