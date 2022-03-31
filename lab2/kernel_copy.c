asmlinkage long sys_mycopy(char *filename1,char *filename2)
{
    char copy_buf[1024];
    int fd1, fd2;
    mm_segment_t old_fs = get_fs();
    set_fs(get_ds());
    if ((fd1 = sys_open(filename1, O_RDONLY, 0)) < 0)
    {
        return -1;   
    }
    if ((fd2 = sys_open(filename2, O_RDWR | O_CREAT , 0644)) < 0)
    {
        return -2;
    }

    memset(copy_buf, 0, sizeof(copy_buf));
    int len = 0;
    while (1)
    {
        len = sys_read(fd1, copy_buf, 1024);
        if (len <= 0) break;
        sys_write(fd2, copy_buf, len);
        memset(copy_buf, 0, sizeof(copy_buf));
    }
    sys_close(fd1);
    sys_close(fd2);
    set_fs(old_fs);
    return 1;
}