// called when 'write' system call is done on the device file
static ssize_t dev_infectado(struct file *filp, const char *buff, size_t len, loff_t *off){

    msg[0] = '\0';

    unsigned int ret;
    if(len> sizeof(msg) - 1)
    return -EINVAL;

    //char *a;
    //strncpy_from_user(a, buff, (long) len);

    ret = copy_from_user(msg,buff,len);

    if(ret)
    return -EFAULT;

    int i = 0;
    char tmp;
    for(i = 0; i < (strlen(msg)/2); i++){
        tmp = msg[i];
        msg[i] = msg[strlen(msg) - i - 1];
        msg[strlen(msg) - i - 1] = tmp;
    }
        printk(KERN_ALERT "%d", strlen(msg));
    msg[len] = '\0';
    printk(KERN_ALERT"the ptr:: %p",off);
    printk(KERN_ALERT"mensagem escr:: %s", msg);

    printk(KERN_ALERT"the ptr after::%p",off);
    return len;
}
