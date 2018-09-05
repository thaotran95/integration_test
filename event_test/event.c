#include <linux/input.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

static const char *event_name(int ev)
{
    switch (ev)
    {
        case EV_SYN:
        {
            return "SYN";
        }
        case EV_KEY:
        {
            return "KEY";
        }
        case EV_REL:
        {
            return "REL";
        }
        case EV_ABS:
        {
            return "ABS";
        }
        case EV_MSC:
        {
            return "MSC";
        }
        case EV_SW:
        {
            return "SW";
        }
        case EV_LED:
        {
            return "LED";
        }
        case EV_SND:
        {
            return "SND";
        }
        case EV_REP:
        {
            return "REP";
        }
        case EV_FF:
        {
            return "FF";
        }
        case EV_PWR:
        {
            return "PWR";
        }
        case EV_FF_STATUS:
        {
            return "FF_STATUS";
        }
        default:
        {
            return "Unknown";
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("usage: %s <dev>\n", argv[0]);
        return 0;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return -1;
    }

    int ret;

    printf("DRIVER\n");

    printf("Version: ");
    int version;
    ret = ioctl(fd, EVIOCGVERSION, &version);
    if (ret != 0) {
        printf("(%s)\n\n", strerror(errno));
    }
    else
    {
        printf("%d.%d.%d\n\n", version >> 16, (version >> 8) & 0xFF, version & 0xFF);
    }

    printf("DEVICE\n");

    printf("Vendor: ");
    struct input_id info;
    ret = ioctl(fd, EVIOCGID, &info);
    if (ret != 0) {
        printf("(%s)\n", strerror(errno));
    }
    else
    {
        printf("0x%04hX\nProduct: 0x%04hX\nVersion: 0x%04hX\nBus: ", info.vendor, info.product, info.version);
        switch (info.bustype)
        {
            case BUS_PCI:
            {
                printf("PCI\n");
                break;
            }
            case BUS_ISAPNP:
            {
                printf("ISA PnP\n");
                break;
            }
            case BUS_USB:
            {
                printf("USB\n");
                break;
            }
            case BUS_HIL:
            {
                printf("HIL\n");
                break;
            }
            case BUS_BLUETOOTH:
            {
                printf("Bluetooth\n");
                break;
            }
            case BUS_VIRTUAL:
            {
                printf("Virtual\n");
                break;
            }
            default:
            {
                printf("Unknown\n");
                break;
            }
        }
    }

    printf("Name: ");
    char name[256];
    ret = ioctl(fd, EVIOCGNAME(sizeof(name)), name);
    if (ret < 0) {
        printf("(%s)\n", strerror(errno));
    }
    else
    {
        printf("%s\n", name);
    }

    printf("Location: ");
    char phys[256];
    ret = ioctl(fd, EVIOCGPHYS(sizeof(phys)), phys);
    if (ret < 0) {
        printf("(%s)\n", strerror(errno));
    }
    else
    {
        printf("%s\n", phys);
    }

    printf("Identity: ");
    char uniq[256];
    ret = ioctl(fd, EVIOCGUNIQ(sizeof(uniq)), uniq);
    if (ret < 0) {
        printf("(%s)\n", strerror(errno));
    }
    else
    {
        printf("%s\n", uniq);
    }

    printf("Events: ");
    int type;
    ret = ioctl(fd, EVIOCGBIT(0, EV_MAX), &type);
    if (ret < 0) {
        printf("(%s)\n\n", strerror(errno));
    }
    else
    {
        int i;
        for (i = 0; i < EV_MAX; i++)
        {
            if (type & (0x1 << i))
            {
                printf(" %s", event_name(i));
            }
        }
        printf("\n\n");
    }

    printf("EVENTS\n");

    for (;;)
    {
        struct input_event ev;
        size_t rb = read(fd, &ev, sizeof(struct input_event));
        if (rb < sizeof(struct input_event))
        {
            printf("(%s)\n", strerror(errno));
        }
        else
        {
            printf("Time %ld.%0ld Type %s Code %d Value %d\n", ev.time.tv_sec, ev.time.tv_usec, event_name(ev.type), ev.code, ev.value);
        }
    }

    close(fd);

    return 0;
}
