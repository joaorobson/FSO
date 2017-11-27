#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

// module attributes
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Device Driver Demo");
MODULE_AUTHOR("João Robson e Vinicius Ferreira");

static char msg[100] = {0};
static short readPos = 0;
static int times = 0;

// prototypes, else the structure initialization that follows fail
static int dev_open(struct inode *, struct file *);
static int dev_rls(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

// structure containing callbacks

static struct file_operations fops = {
    .read = dev_read, // address of dev_read
    .open = dev_open, // address of dev_open
    .write = dev_write, // address of dev_write
    .release = dev_rls, // address of dev_rls
};


// called when module is loaded, similar to main()
int init_module(void){

    int t = register_chrdev(42,"device_drive",&fops); // register driver with major:89
    if (t<0) printk(KERN_ALERT "Device registration failed..\n");
    else printk(KERN_ALERT "Device registered...\n");

    return t;
}


// called when module is unloaded, similar to destructor in OOP
void cleanup_module(void){
    unregister_chrdev(42,"device_drive");
}


// called when 'open' system call is done on the device file
static int dev_open(struct inode *inod, struct file *fil){

    times++;
    printk(KERN_ALERT "Device opened %d times\n", times);
    return 0;
}


// called when 'read' system call is done on the device file
static ssize_t dev_read(struct file *filp, char *buff, size_t len, loff_t *off){

int i= len>= strlen(msg) ? strlen(msg) : len;
*off+= strlen(msg)-1;

if (*off>= strlen(msg)){
printk(KERN_ALERT"lida:: %s", msg);
return 0;
}
if (copy_to_user(buff,msg,i)){
printk(KERN_ALERT"lida:: %s", msg);
return -EFAULT;
}
return i;

}


// called when 'write' system call is done on the device file
static ssize_t dev_write(struct file *filp, const char *buff, size_t len, loff_t *off){

unsigned int ret;
if(len> sizeof(msg) - 1)
return -EINVAL;
ret = copy_from_user(msg,buff,len);
if(ret)
return -EFAULT;
msg[len] = '\0';
printk(KERN_ALERT"the ptr:: %p",off);
printk(KERN_ALERT"mensagem escr:: %s", msg);

printk(KERN_ALERT"the ptr after::%p",off);
return len;
}

// called when 'close' system call is done on the device file
static int dev_rls(struct inode *inod, struct file *fil){
    printk(KERN_ALERT "Device closed\n");
    return 0;
}
