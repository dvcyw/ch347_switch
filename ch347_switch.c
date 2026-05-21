#include <stdio.h>
#include <libusb-1.0/libusb.h>

#define VID 0x1A86
#define PID 0x55DB
#define TARGET_INTERFACE 2
#define EP_OUT 0x06

int main(int argc, char **argv) {
    libusb_context *ctx = NULL;
    libusb_device_handle *dev_handle = NULL;
    int driver_detached = 0;

    unsigned char payload_18v[11] = {0xCC, 0x08, 0x00, 0x08, 0x08, 0x08, 0x08, 0x28, 0x08, 0xF8, 0x08};
    unsigned char payload_33v[11] = {0xCC, 0x08, 0x00, 0x08, 0x08, 0x08, 0x08, 0x28, 0x08, 0xF0, 0x08};
    unsigned char *send_data;

    if (argc > 1 && argv[1][0] == '3') {
        send_data = payload_33v;
        printf("Sending Payload (Likely 3.3V)...\n");
    } else {
        send_data = payload_18v;
        printf("Sending Payload (Likely 1.8V)...\n");
    }

    libusb_init(&ctx);
    dev_handle = libusb_open_device_with_vid_pid(ctx, VID, PID);
    if (!dev_handle) {
        printf("Device not found! Make sure to run with sudo.\n");
        return 1;
    }

    // 暫時卸載 Kernel Driver
    if (libusb_kernel_driver_active(dev_handle, TARGET_INTERFACE) == 1) {
        if (libusb_detach_kernel_driver(dev_handle, TARGET_INTERFACE) == 0) {
            driver_detached = 1;
        }
    }

    libusb_claim_interface(dev_handle, TARGET_INTERFACE);

    int transferred;
    int ret = libusb_bulk_transfer(dev_handle, EP_OUT, send_data, 11, &transferred, 1000);

    if (ret == 0 && transferred == 11) {
        printf("Magic command sent successfully! (%d bytes)\n", transferred);
    } else {
        printf("Error sending command: %s\n", libusb_error_name(ret));
    }

    libusb_release_interface(dev_handle, TARGET_INTERFACE);

    // 註解掉這行，先不掛回驅動，方便你用三用電表測量！
    if (driver_detached) {
        libusb_attach_kernel_driver(dev_handle, TARGET_INTERFACE);
    }

    libusb_close(dev_handle);
    libusb_exit(ctx);
    return 0;
}