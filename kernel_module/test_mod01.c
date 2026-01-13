#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kern_levels.h>
#include <linux/uaccess.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");

#define ECE_BUF_SIZE 256
#define ECE_RESET_BUFFER 0

static char ece_buffer[ECE_BUF_SIZE];
int isReg;
int major;
int ece_offset_w;
int ece_offset_r;
int ece_size;

int ece_init(void);
void ece_end(void);
static ssize_t ece_write(struct file*, const char*, size_t , loff_t*);
static ssize_t ece_read(struct file*, char*, size_t , loff_t*);
static long ece_ioctl(struct file*, unsigned int, unsigned long);

static struct file_operations ece_fops = 
{
    .read = ece_read,
    .write = ece_write,
    .unlocked_ioctl = ece_ioctl,
};

int ece_init(void)
{
    major = register_chrdev(0, "test_mod01", &ece_fops);
    ece_offset_w=0;
    ece_offset_r=0;
    ece_size=0;

    if(major <0) {
        isReg=0;
        printk(KERN_INFO " ECE4310: Start FAIL \n");
    } else {
        isReg=1;
        printk(KERN_INFO " ECE4310: Start here \n");
    }

    return 0;
}

void ece_end(void)
{
    if(isReg) {
        unregister_chrdev(major, "test_mod01");
    }
    printk(KERN_INFO " ECE4310: End here \n");
}

static ssize_t ece_write(struct file *fp, const char *buf, size_t count, loff_t *op)
{
    int ret=0;
    if(ece_offset_w +count >= ECE_BUF_SIZE)
    {
        printk(KERN_INFO " ECE4310: Write overflow. Abort. \n");
        return -1;
    }

    printk(KERN_INFO " ECE4310: Copy from user. \n");
    ret=copy_from_user(&ece_buffer[ece_offset_w], buf, count);
    if(ret != 0)
    {
        printk(KERN_INFO " ECE4310: ERR copy from user. \n");
        return -1;
    }
    ece_offset_w=ece_offset_w + count;

    return count;
}

static ssize_t ece_read(struct file *fp, char *buf, size_t count, loff_t* offset)
{
    int ret;
    if(ece_offset_r + count >= ECE_BUF_SIZE)
    {
        printk(KERN_INFO " ECE4310: Read Overflow. Abort. \n");
        return -1;
    }
    printk(KERN_INFO " ECE4310: Copy to user. \n");
    ret=copy_to_user(buf, &ece_buffer[ece_offset_r], count);
    if(ret != 0)
    {
        printk(KERN_INFO " ECE4310: ERR copy to user. \n");
        return -1;
    }

    ece_offset_r = ece_offset_r + count;

    return count;
}

static long ece_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
    switch(cmd) {
        case ECE_RESET_BUFFER:
        printk(KERN_INFO " ECE4310: Resetting buffer offsets \n");
        ece_offset_r = 0;
        ece_offset_w = 0;
        ece_size = 0;
        return 0;

        default:
        printk(KERN_INFO " ECE4310: Unknown ioctl command \n");
        return -EINVAL;
    }
}

module_init(ece_init);
module_exit(ece_end);

/*
make

gcc test_app.c

sudo insmod test_mod01.ko 

sudo dmesg | grep ECE4310

cat /proc/devices | grep test (this is how you find the major) 

sudo mknod /dev/test_mod01 c 510 1

sudo chmod 0777 /dev/test_mod01 (set permissions) 

ll /dev/test_mod01 (verify device node exists)

./a.out (run it)

sudo dmesg | grep ECE4310 | tail -15 (check kernel messages)

echo "maybe... " > /dev/test_mod01
*/
