

#include <errno.h>
#include <poll.h>
//#include <sys/poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// for mknod
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <dirent.h>

#include <sys/socket.h>
#include <linux/netlink.h>
#include <fcntl.h>

#define COLDBOOT_DONE "/dev/.coldboot_done"
#define UEVENT_MSG_LEN  2048
#define NETLINK_KOBJECT_UEVENT 15

//#define SCM_RIGHTS  0x01            /* rw: access rights (array of int) */
#define SCM_CREDENTIALS 0x02        /* rw: struct ucred     */
//#define SCM_SECURITY    0x03        /* rw: security label       */
//
struct ucred {
    __u32   pid;
    __u32   uid;
    __u32   gid;
};

static int device_fd = -1;
static int uevent_index = 0;
static int uevent_stop_dump = 0;

int uevent_open_socket(int buf_sz, int passcred)
{
    struct sockaddr_nl addr;
    int on = passcred;
    int s;
    
    memset(&addr, 0, sizeof(addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_pid = getpid();
    addr.nl_groups = 0xffffffff;
    
    s = socket(PF_NETLINK, SOCK_DGRAM | SOCK_CLOEXEC, NETLINK_KOBJECT_UEVENT);
    if(s < 0)
        return -1;
    
    setsockopt(s, SOL_SOCKET, SO_RCVBUFFORCE, &buf_sz, sizeof(buf_sz));
    setsockopt(s, SOL_SOCKET, SO_PASSCRED, &on, sizeof(on));
    
    if(bind(s, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        close(s);
        return -1;
    }
    
    return s;
}




static int write_uevent_dump(const char *msg, int size) {
    FILE *fp;
    int write_len;
    char filename[512] = {'\0'};

    //snprintf(filename, sizeof(filename), "/uevent_dump/dump_%d", uevent_index);
    snprintf(filename, sizeof(filename), "uevent_dump/dump_%d", uevent_index);
    uevent_index++;

    if((fp = fopen(filename, "a+b")) == NULL) {
        printf("Failed to open uevent dump file! error: %s\n", strerror(errno));
        return -1;
    }

    write_len = fwrite(msg, 1, size, fp);
    if (write_len != size)
        printf("Write length is differ from size! write_len=%d, size=%d\n", write_len, size);
    fclose(fp);
    return 0;
}


ssize_t uevent_kernel_recv(int socket, void *buffer, size_t length, int require_group, uid_t *uid)
{       
    struct iovec iov = { buffer, length };
    struct sockaddr_nl addr;
    char control[CMSG_SPACE(sizeof(struct ucred))];
    struct msghdr hdr = {
        &addr,
        sizeof(addr),
        &iov,
        1,
        control,
        sizeof(control),
        0,
    };  
    
    *uid = -1;
    ssize_t n = recvmsg(socket, &hdr, 0);
    if (n <= 0) {
        return n;
    }   
    
    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&hdr);
    if (cmsg == NULL || cmsg->cmsg_type != SCM_CREDENTIALS) {
        /* ignoring netlink message with no sender credentials */
        goto out;
    }
 
    struct ucred *cred = (struct ucred *)CMSG_DATA(cmsg);
    *uid = cred->uid;
//    printf(" ---> uevent_kernel_recv cred->uid: %d", *uid);
//    printf(" ---> uevent_kernel_recv addr.nl_pid: %d", addr.nl_pid);
//    printf(" ---> uevent_kernel_recv require_group: %d", require_group);
//    printf(" ---> uevent_kernel_recv addr.nl_pid: %d", addr.nl_groups);
    
    if (cred->uid != 0) {
        /* ignoring netlink message from non-root user */
        goto out;
    }
    
    if (addr.nl_pid != 0) {
        /* ignore non-kernel */
        goto out;
    }
    if (require_group && addr.nl_groups == 0) {
        /* ignore unicast messages when requested */
        goto out;
    }   
    
    return n;
        
out:    
    /* clear residual potentially malicious data */
    bzero(buffer, length);
    errno = EIO;
    return -1;
}

ssize_t uevent_kernel_multicast_uid_recv(int socket, void *buffer, size_t length, uid_t *uid)
{   
    return uevent_kernel_recv(socket, buffer, length, 1, uid);
}

ssize_t uevent_kernel_multicast_recv(int socket, void *buffer, size_t length)
{
    uid_t uid = -1; 
    return uevent_kernel_multicast_uid_recv(socket, buffer, length, &uid);                                                                                                                            
}


void handle_device_fd()
{
    char msg[UEVENT_MSG_LEN+2];
    int n;
    while ((n = uevent_kernel_multicast_recv(device_fd, msg, UEVENT_MSG_LEN)) > 0) { 
        if(n >= UEVENT_MSG_LEN)   /* overflow -- discard */
            continue;

        msg[n] = '\0';
        msg[n+1] = '\0';

        if(!uevent_stop_dump) {
            if (write_uevent_dump(msg, n) < 0)
                printf("Error shows while dump uevent to file!\n");
        }

/*
        struct uevent uevent;
        parse_event(msg, &uevent);

        if (selinux_status_updated() > 0) { 
            struct selabel_handle *sehandle2;
            sehandle2 = selinux_android_file_context_handle();
            if (sehandle2) {
                selabel_close(sehandle);
                sehandle = sehandle2;
            }    
        }    

        handle_device_event(&uevent);
        handle_firmware_event(&uevent);
*/
    }    
}

static void do_coldboot(DIR *d)
{
    struct dirent *de; 
    int dfd, fd;

    dfd = dirfd(d);

    fd = openat(dfd, "uevent", O_WRONLY);
    if(fd >= 0) { 
        write(fd, "add\n", 4);
        close(fd);
        handle_device_fd();
    }    

    while((de = readdir(d))) {
        DIR *d2; 

        if(de->d_type != DT_DIR || de->d_name[0] == '.') 
            continue;

        fd = openat(dfd, de->d_name, O_RDONLY | O_DIRECTORY);
        if(fd < 0) 
            continue;

        d2 = fdopendir(fd);
        if(d2 == 0)
            close(fd);
        else {
            do_coldboot(d2);
            closedir(d2);
        }    
    }    
}

static void coldboot(const char *path)
{       
    DIR *d = opendir(path);
    if(d) {
        do_coldboot(d);
        closedir(d);
    }   
}   

void device_init() {
//    sehandle = selinux_android_file_context_handle();
//    selinux_status_open(true);
    
    /* is 256K enough? udev uses 16MB! */
    //device_fd = uevent_open_socket(256*1024, true);
    device_fd = uevent_open_socket(256*1024, 1);

    printf("device_init device_fd %d\n", device_fd);
    if (device_fd == -1) {
        return;
    }
    fcntl(device_fd, F_SETFL, O_NONBLOCK);
    
    if (access(COLDBOOT_DONE, F_OK) == 0) { 
        printf("Skipping coldboot, already done!\n");
        return;
    }    

    printf("device_init coldboot start\n");
    //Timer t;
    coldboot("/sys/class");
    coldboot("/sys/block");
    coldboot("/sys/devices");
    close(open(COLDBOOT_DONE, O_WRONLY|O_CREAT|O_CLOEXEC, 0000));
//    NOTICE("Coldboot took %.2fs.\n", t.duration());
    printf("device_init coldboot end\n");

    uevent_stop_dump = 1;
}

int dump_host_devices() {

    int listen = 1;

    if(mkdir("uevent_dump", 0666) != 0) {
        printf("Failed to create uevent_dump! error: %s\n", strerror(errno));
        return -1;
    }

    device_init();
    
    struct pollfd ufd;
    ufd.events = POLLIN;
    ufd.fd = device_fd;
    printf("dump_host_devices ufd.fd %d\n", ufd.fd);
    
    while (listen) {
        ufd.revents = 0;
        int nr = poll(&ufd, 1, -1);
        if (nr <= 0) {
            continue;
        }   
        if (ufd.revents & POLLIN) {
            handle_device_fd();
        }   
    }  
}

int main() {

    printf("This is main - mknod\n");
    
    dump_host_devices();

    return 0;
}
