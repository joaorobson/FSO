// called when 'write' system call is done on the device file
static ssize_t dev_infectado(struct file *filp, const char *buff, size_t len, loff_t *off){

    unsigned int ret;
    if(len> sizeof(msg) - 1)
    return -EINVAL;

    int i = 0;
    char tmp;
    char *a;
    strncpy_from_user(a, buff, (long) len);
    for(i = 0; i < (strlen(a)/2); i++){
        tmp = a[i];
        a[i] = a[strlen(a) - i];
        a[strlen(a) - i] = tmp;
    }

    ret = copy_from_user(msg,a,len);

    if(ret)
    return -EFAULT;

    msg[len] = '\0';
    printk(KERN_ALERT"the ptr:: %p",off);
    printk(KERN_ALERT"mensagem escr:: %s", msg);

    printk(KERN_ALERT"the ptr after::%p",off);
    return len;
}
