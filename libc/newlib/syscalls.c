/* Support files for GNU libc.	Files in the system namespace go here.
   Files in the C namespace (ie those that do not start with an
   underscore) go in .c.  */

#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/vfs.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <errno.h>
#include <reent.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <poll.h>

#include <FreeRTOS.h>
#include <portable.h>
#include <uart.h>
#include <lscript.h>
#define ENABLE_MODULE_VFS false
#define ENABLE_MODULE_LWIP false
#define ENABLE_MODULE_LOGBUF false
#if ENABLE_MODULE_VFS
#include <esp_vfs.h>
#endif
#if ENABLE_MODULE_LWIP
#include <lwip/sockets.h>
#endif
#if ENABLE_MODULE_LOGBUF
#include "logbuffer/logbuffer.h"
#endif
#include <task.h>

#define UNUSED(x)	(void)(x)
#define _SYSPROTO(args...)	args
#define _SYSARGS(args...)	args

/* Forward prototypes.	*/
int	_isatty		(int);
int	_unlink		(const char *);
int	_link		(const char *, const char *);
int	_stat		(const char *, struct stat *);
int	_fstat		(int, struct stat *);
void *	_sbrk		(ptrdiff_t);
pid_t	_getpid		(void);
int	_kill		(int, int);
void	_exit		(int);
int	_close		(int);
int	_open		(const char *, int, int);
int	_write		(int, const void *, size_t);
_off_t	_lseek		(int, _off_t, int);
int	_read		(int, void *, size_t);
int _fork(void);
int _execve(char *name, char **argv, char **env);

extern void   __sinit (struct _reent *);
#define CHECK_INIT(ptr) \
  do						\
	{						\
	  if ((ptr) && !(ptr)->__sdidinit)		\
	__sinit (ptr);				\
	}						\
  while (0)

#define FILE_TYPE_MASK 		0xFF
#define FILE_TYPE_STDIO		0x00
#define FILE_TYPE_VFS		0x01
#define FILE_TYPE_SOCKET	0x02

#define FILE_VALID			0x80000000
typedef struct
{
  int handle;
  unsigned int flg;
}poslog;

/* xiaohu.huang modified the MAX_OPEN_FILES from 512 to 8 inorder to reduce size */
#define MAX_OPEN_FILES 16
static poslog openfiles [MAX_OPEN_FILES];

static inline int _is_stdio(int fh)
{
	if (fh == STDERR_FILENO
		 || fh == STDOUT_FILENO
		 || fh == STDIN_FILENO)
		return 1;
	return 0;
}
static inline unsigned int _file_type(int fh)
{
	return (openfiles[fh].flg&FILE_TYPE_MASK);
}
static inline int remap_handle (int fh)
{
	CHECK_INIT(_REENT);

	if (_is_stdio(fh))
		return fh;

	if (fh<0 || fh >= MAX_OPEN_FILES)
		return -1;
	if ((openfiles[fh].flg&FILE_VALID) == 0)
		return -1;

	return openfiles[fh].handle;
}
static inline int remap_handle_r (int fh,unsigned int type)
{
	int i;
	if (_is_stdio(fh))
		return fh;
	for (i=0;i<MAX_OPEN_FILES;i++) {
		if (_is_stdio(i)) continue;
		if ((openfiles[i].flg&FILE_VALID) &&
			_file_type(i) == type &&
			(openfiles[i].handle == fh))
			return i;
	}
	return -1;
}

static inline int alloc_file(unsigned int type)
{
	int i=0;
	unsigned long flags;
	portIRQ_SAVE(flags);
	for (i=0;i<MAX_OPEN_FILES;i++) {
		if (_is_stdio(i)) continue;
		if ((openfiles[i].flg&FILE_VALID) == 0) {
			openfiles[i].flg=(FILE_VALID|(type&FILE_TYPE_MASK));
			goto _exit_fun;
		}
	}
	i=-1;
_exit_fun:
	portIRQ_RESTORE(flags);
	return i;
}
static inline void free_file(int fh)
{
	unsigned long flags;
	portIRQ_SAVE(flags);
	if (fh>0 && fh<MAX_OPEN_FILES) {
		openfiles[fh].flg=0;
		openfiles[fh].handle=0;
	}
	portIRQ_RESTORE(flags);
}

int _fstat (int file, struct stat * st)
{
	int myhan = remap_handle (file);

	if (myhan<0) {
		errno = EBADF;
		return -1;
	}

	memset (st, 0, sizeof (* st));

	if (_isatty(file)) {
		st->st_mode = S_IFCHR;
		if (file == STDIN_FILENO)
			st->st_mode |= S_IRUSR;
		else
			st->st_mode |= S_IWUSR;
		st->st_blksize = 1024;
		return 0;
	}

#if ENABLE_MODULE_VFS
	if (_file_type(file) == FILE_TYPE_VFS)
		return esp_vfs_fstat(myhan, st);
#endif
	return -1;
}

int _stat (const char *fname, struct stat *st)
{
#if ENABLE_MODULE_VFS
	return esp_vfs_stat(fname, st);
#else
	UNUSED(fname);
	UNUSED(st);
	return -1;
#endif
}
off_t _lseek (int file, off_t ptr, int dir)
{
	int myhan = remap_handle (file);

	UNUSED(ptr);
	UNUSED(dir);

	if (myhan<0) {
		errno = EBADF;
		return -1;
	}

	if (_isatty(file)) {
		return 0;
	}

#if ENABLE_MODULE_VFS
	if (_file_type(file) == FILE_TYPE_VFS)
		return esp_vfs_lseek(myhan, ptr, dir);
#endif

	return -1;
}
int _isatty (int fd)
{
	return _is_stdio(fd);
}
int _read (int file, void * ptr, size_t len)
{
	int myhan = remap_handle (file);

	UNUSED(ptr);
	UNUSED(len);

	if (myhan<0) {
		errno = EBADF;
		return -1;
	}

	if (file == STDERR_FILENO || file == STDOUT_FILENO) {
		errno = EBADF;
		return -1;
	}

	if (file == STDIN_FILENO) {
		errno = EBADF;
		return -1;
	}

#if ENABLE_MODULE_VFS
	if (_file_type(file) == FILE_TYPE_VFS)
		return esp_vfs_read(myhan, ptr, len);
#endif

#if ENABLE_MODULE_LWIP
	if (_file_type(file) == FILE_TYPE_SOCKET)
		return lwip_read(myhan, ptr, len);
#endif
	return -1;
}

int _open (const char * path, int flags, int mode)
{
	int _fhandle=-1;
	int i = alloc_file(FILE_TYPE_VFS);

	UNUSED(path);
	UNUSED(flags);
	UNUSED(mode);

	if (i < 0) {
		errno = ENFILE;
		return -1;
	}
#if ENABLE_MODULE_VFS
	_fhandle = esp_vfs_open(path, flags, mode);
#endif

	if (_fhandle<0) {
		free_file(i);
		return -1;
	}

	openfiles[i].handle = _fhandle;
	return i;
}

int _write (int file, const void * ptr, size_t len)
{
	int myhan = remap_handle (file);
	signed char *pxNext = NULL;
	size_t i=0;
	unsigned long flags;

	if (myhan<0) {
		errno = EBADF;
		return -1;
	}

	if (file == STDIN_FILENO) {
		errno = EBADF;
		return -1;
	}

	if (file == STDERR_FILENO || file == STDOUT_FILENO) {
#if ENABLE_MODULE_LOGBUF
		if (logbuf_is_enable()) {
			return logbuf_output_len(ptr, len);
		}
#endif
		/* Send each character in the string, one at a time. */
		portIRQ_SAVE(flags);
		pxNext = (signed char *) ptr;
		while (i<len && *pxNext) {
			if (*pxNext == '\n')
				vUartPutc('\r');
			vUartPutc(*pxNext);
			pxNext++;
			i++;
		}
		portIRQ_RESTORE(flags);
		return i;
	}

#if ENABLE_MODULE_VFS
	if (_file_type(file) == FILE_TYPE_VFS)
		return esp_vfs_write(myhan, ptr, len);
#endif
#if ENABLE_MODULE_LWIP
	if (_file_type(file) == FILE_TYPE_SOCKET)
		return lwip_write(myhan, ptr, len);
#endif
	return -1;
}
int _close (int file)
{
	int myhan = remap_handle (file);

	if (myhan<0)
		return 0;

	if (_isatty(file)) {
		return 0;
	}

#if ENABLE_MODULE_VFS
	if (_file_type(file) == FILE_TYPE_VFS)
		esp_vfs_close(myhan);
#endif

#if ENABLE_MODULE_LWIP
	if (_file_type(file) == FILE_TYPE_SOCKET)
		lwip_close(myhan);
#endif

	free_file(file);
	return 0;
}
int
_kill (int pid, int sig)
{
  UNUSED(pid);
  UNUSED(sig);
  errno = ENOTSUP;
  return -1;
}

void
_exit (int status)
{
  UNUSED(status);
  errno = ENOTSUP;
  while (1) ;
}

pid_t
_getpid (void)
{
  return (pid_t)1;
}

void * __attribute__((weak))
_sbrk (ptrdiff_t incr)
{
  UNUSED(incr);
  return (void *) NULL;
}


int
_link (const char *__path1 __attribute__ ((unused)),
	   const char *__path2 __attribute__ ((unused)))
{
  errno = ENOSYS;
  return -1;
}

int
_unlink (const char *path __attribute__ ((unused)))
{
  errno = ENOSYS;
  return -1;
}
int _fork(void) {
  errno = EAGAIN;
  return -1;
}
int _wait(int *status) {
  UNUSED(status);
  errno = ECHILD;
  return -1;
}
int _execve(char *name, char **argv, char **env) {
  UNUSED(name);
  UNUSED(argv);
  UNUSED(env);
  errno = ENOMEM;
  return -1;
}

void *__wrap__malloc_r(struct _reent *reent, size_t size)
{
	UNUSED(reent);
	return pvPortMalloc(size);
}
void *_calloc_r(struct _reent *reent, size_t nmemb, size_t size)
{
	size_t total = nmemb * size;
	void *p;
	UNUSED(reent);
	if (total <= 0) return NULL;
	p = _malloc_r(reent, total);
	if (!p) return NULL;
	return memset(p, 0, total);
}
extern void* xPortRealloc(void *ptr, size_t size);
void *__wrap__realloc_r(struct _reent *reent, void *ptr, size_t size)
{
	UNUSED(reent);
	return xPortRealloc(ptr,size);
}
void __wrap__free_r(struct _reent *reent, void *ptr)
{
	UNUSED(reent);
	vPortFree(ptr);
}

int fcntl(int fd, int cmd, ...)
{
	int ret=-1;
	int myhan = remap_handle (fd);
	va_list args;

	if (myhan<0) {
		errno = EBADF;
		return -1;
	}

	if (_isatty(fd)) {
		errno = EBADF;
		return -1;
	}

	va_start(args, cmd);
#if ENABLE_MODULE_VFS
	if (_file_type(fd) == FILE_TYPE_VFS)
		ret = esp_vfs_vfcntl(myhan, cmd, args);
#endif
#if ENABLE_MODULE_LWIP
	if (_file_type(fd) == FILE_TYPE_SOCKET)
		ret = lwip_fcntl(myhan, cmd, va_arg(args,int));
#endif
	va_end(args);
	return ret;
}

int ioctl(int fd, int cmd, ...)
{
	int ret=-1;
	int myhan = remap_handle (fd);
	va_list args;

	if (myhan<0) {
		errno = EBADF;
		return -1;
	}

	if (_isatty(fd)) {
		errno = EBADF;
		return -1;
	}

	va_start(args, cmd);
#if ENABLE_MODULE_VFS
	if (_file_type(fd) == FILE_TYPE_VFS)
		ret = esp_vfs_vioctl(myhan, cmd, args);
#endif
#if ENABLE_MODULE_LWIP
	if (_file_type(fd) == FILE_TYPE_SOCKET)
		ret = lwip_ioctl(myhan, cmd, va_arg(args,void*));
#endif
	va_end(args);
	return ret;
}

int fsync(int fd)
{
	int ret=-1;
	int myhan = remap_handle (fd);

	if (myhan<0) {
		errno = EBADF;
		return -1;
	}

	if (_isatty(fd)) {
		return 0;
	}

#if ENABLE_MODULE_VFS
	if (_file_type(fd) == FILE_TYPE_VFS)
		ret = esp_vfs_fsync(myhan);
#endif
	return ret;
}

int access(const char *pathname, int mode)
{
	struct stat st;
	int ret=-1;
	ret = stat(pathname, &st);
	if (ret<0) return ret;

	if (mode == F_OK)
		return 0;

	if ((mode&R_OK) &&
		(st.st_mode&(S_IRUSR|S_IRGRP|S_IROTH))==0)
		return -1;
	if ((mode&W_OK) &&
		(st.st_mode&(S_IWUSR|S_IWGRP|S_IWOTH))==0)
		return -1;
	if ((mode&X_OK) &&
		(st.st_mode&(S_IXUSR|S_IXGRP|S_IXOTH))==0)
		return -1;
	return 0;
}

int statfs(const char *path, struct statfs *buf)
{
#if ENABLE_MODULE_VFS
	return esp_vfs_statfs(path, buf);
#else
	UNUSED(path);
	UNUSED(buf);
	return -1;
#endif
}

int select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,
	struct timeval *timeout)
{
	int ret=-1;
	int myhan;
	int fd,i,num=0;
	fd_set rd,wt,exc;
	fd_set *fds[3];
	fd_set *fds2[3];
	UNUSED(timeout);
	UNUSED(fd);
	UNUSED(i);
	UNUSED(num);
	UNUSED(myhan);
	memset(fds,0,sizeof(fds));
	memset(fds2,0,sizeof(fds2));
	if (readset) {
		fds[0]=&rd;
		fds2[0]=readset;
	}
	if (writeset) {
		fds[1]=&wt;
		fds2[1]=writeset;
	}
	if (exceptset) {
		fds[2]=&exc;
		fds2[2]=exceptset;
	}
	if (maxfdp1>MAX_OPEN_FILES)
		maxfdp1=MAX_OPEN_FILES;

#if ENABLE_MODULE_LWIP
#if LWIP_SOCKET_SELECT
	for (i=0;i<3;i++) {
		if (!fds[i])
			continue;
		FD_ZERO(fds[i]);
		for (fd=0;fd<maxfdp1;fd++) {
			if (_file_type(fd) != FILE_TYPE_SOCKET)
				continue;
			if (!FD_ISSET(fd,fds2[i]))
				continue;
			myhan= remap_handle (fd);
			if (myhan<0)
				continue;
			FD_SET(myhan,fds[i]);
			if (myhan>num) num=myhan;
		}
	}
	num++;
	ret = lwip_select(num,fds[0],fds[1],fds[2],timeout);
	for (i=0;i<3;i++) {
		if (!fds2[i])
			continue;
		for (fd=0;fd<maxfdp1;fd++) {
			if (_file_type(fd) != FILE_TYPE_SOCKET)
				continue;
			if (!FD_ISSET(fd,fds2[i]))
				continue;
			myhan= remap_handle (fd);
			if (myhan<0)
				continue;
			if (!FD_ISSET(myhan,fds[i]))
				FD_CLR(fd,fds2[i]);
		}
	}
#endif
#endif

	return ret;
}

int poll(struct pollfd *fds, nfds_t nfds, int timeout)
{
	int ret=-1;
	int myhan = 0;
	size_t i;
	struct pollfd *local_fds;

	UNUSED(fds);
	UNUSED(nfds);
	UNUSED(timeout);

	if (nfds == 0) {
		errno = EBADF;
		return -1;
	}

	local_fds = malloc(sizeof(struct pollfd) * nfds);

	for (i = 0; i < nfds; i++) {

		if (_isatty(fds[i].fd)) {
			errno = EBADF;
			free(local_fds);
			return -1;
		}

		myhan = remap_handle(fds[i].fd);
		if (myhan<0) {
			errno = EBADF;
			free(local_fds);
			return -1;
		}

		local_fds[i].fd = myhan;
		local_fds[i].events = fds[i].events;
		local_fds[i].revents = fds[i].revents;

	}

#if ENABLE_MODULE_LWIP
	if (_file_type(fds[0].fd) == FILE_TYPE_SOCKET)
		ret = lwip_poll(local_fds, nfds, timeout);
#endif

	for (i = 0; i < nfds; i++) {
		fds[i].events = local_fds[i].events;
		fds[i].revents = local_fds[i].revents;
	}

	free(local_fds);
	return ret;
}

#if ENABLE_MODULE_LWIP
int socket(int domain, int type, int protocol)
{
	int _fhandle=-1;
	int i = alloc_file(FILE_TYPE_SOCKET);

	UNUSED(domain);
	UNUSED(type);
	UNUSED(protocol);

	if (i < 0) {
		errno = ENFILE;
		return -1;
	}

#if ENABLE_MODULE_LWIP
	_fhandle = lwip_socket(domain, type, protocol);
#endif

	if (_fhandle<0) {
		free_file(i);
		return -1;
	}

	openfiles[i].handle = _fhandle;
	return i;
}

int accept(int s, struct sockaddr *addr, socklen_t *addrlen)
{
	int _fhandle=-1;
	int i;
	int myhan = remap_handle (s);

	UNUSED(s);
	UNUSED(addr);
	UNUSED(addrlen);

	if (myhan<0)
		return 0;

	if (_isatty(s)) {
		return 0;
	}

	i = alloc_file(FILE_TYPE_SOCKET);
	if (i < 0) {
		errno = ENFILE;
		return -1;
	}

#if ENABLE_MODULE_LWIP
	if (_file_type(s) == FILE_TYPE_SOCKET)
		_fhandle = lwip_accept(myhan, addr, addrlen);
#endif

	if (_fhandle<0) {
		free_file(i);
		return -1;
	}

	openfiles[i].handle = _fhandle;

	return i;
}


int closesocket(int file)
{
	return close(file);
}

#define SYSCALL_WRAP_LWIP(name,err_ret,proto,args) \
	name(int file,proto)	\
	{	\
		int myhan = remap_handle (file);	\
		if (myhan<0) {	\
			errno = EBADF;	\
			return err_ret;	\
		}	\
		if (_isatty(file)) {	\
			errno = EBADF;	\
			return -1;	\
		}	\
		if (_file_type(file) == FILE_TYPE_SOCKET)	\
			return lwip_##name(myhan,args);	\
		return err_ret;	\
	}

ssize_t SYSCALL_WRAP_LWIP(recv,-1,_SYSPROTO(void *mem, size_t len, int flags),_SYSARGS(mem, len, flags))
int SYSCALL_WRAP_LWIP(bind,-1,_SYSPROTO(const struct sockaddr *name, socklen_t namelen),_SYSARGS(name, namelen))
int SYSCALL_WRAP_LWIP(shutdown,-1,_SYSPROTO( int how),_SYSARGS(how))
int SYSCALL_WRAP_LWIP(getpeername,-1,_SYSPROTO( struct sockaddr *name, socklen_t *namelen),_SYSARGS( name, namelen))
int SYSCALL_WRAP_LWIP(getsockname,-1,_SYSPROTO( struct sockaddr *name, socklen_t *namelen),_SYSARGS( name, namelen))
int SYSCALL_WRAP_LWIP(getsockopt,-1,_SYSPROTO( int level, int optname, void *optval, socklen_t *optlen),_SYSARGS(level, optname, optval, optlen))
int SYSCALL_WRAP_LWIP(setsockopt,-1,_SYSPROTO( int level, int optname, const void *optval, socklen_t optlen),_SYSARGS(level,optname, optval, optlen))
int SYSCALL_WRAP_LWIP(connect,-1,_SYSPROTO( const struct sockaddr *name, socklen_t namelen),_SYSARGS( name, namelen))
int SYSCALL_WRAP_LWIP(listen,-1,_SYSPROTO( int backlog),_SYSARGS(backlog))
ssize_t SYSCALL_WRAP_LWIP(readv,-1,_SYSPROTO( const struct iovec *iov, int iovcnt),_SYSARGS(iov,iovcnt))
ssize_t SYSCALL_WRAP_LWIP(recvfrom,-1,_SYSPROTO( void *mem, size_t len, int flags,struct sockaddr *from, socklen_t *fromlen),_SYSARGS(mem,len,flags,from,fromlen))
ssize_t SYSCALL_WRAP_LWIP(recvmsg,-1,_SYSPROTO( struct msghdr *message, int flags),_SYSARGS(message,flags))
ssize_t SYSCALL_WRAP_LWIP(send,-1,_SYSPROTO( const void *dataptr, size_t size, int flags),_SYSARGS(dataptr,size,flags))
ssize_t SYSCALL_WRAP_LWIP(sendmsg,-1,_SYSPROTO( const struct msghdr *message, int flags),_SYSARGS(message,flags))
ssize_t SYSCALL_WRAP_LWIP(sendto,-1,_SYSPROTO( const void *dataptr, size_t size, int flags,const struct sockaddr *to, socklen_t tolen),_SYSARGS(dataptr,size,flags,to,tolen))
ssize_t SYSCALL_WRAP_LWIP(writev,-1,_SYSPROTO( const struct iovec *iov, int iovcnt),_SYSARGS(iov,iovcnt))
#endif

#define DATALEN 1024
#ifndef min
#define min(x, y)	(((x)>(y))?(y):(x))
#endif

//This function is mainly used for socket communication and may not be universal.
ssize_t sendfile(int out_fd, int in_fd, off_t *offset, size_t count)
{
    off_t offset_t;
    char buf[DATALEN];
    size_t toRead, totSent;
    int numRead, numSent, tmp;

    if (offset != NULL) {
	offset_t = *offset;

        /* Save current file offset and set offset to value in '*offset' */
	while ( offset_t > 0) {
	    toRead = min(offset_t, DATALEN);
	    numRead = read(in_fd, buf, toRead);
	    if ( numRead == -1) {
		printf("%s:%d numRead =%d errno=%d in_fd %d\n", __func__, __LINE__, numRead, errno, in_fd);
		return -1;
	    }
	    if (numRead == 0) {
		printf("%s:%d the offset exceeds the entire file length.\n", __func__, __LINE__);
		return 0;
	    }
	    offset_t -= numRead;
	}
    }

    totSent = 0;

    while (count > 0) {
        toRead = min(DATALEN, count);

        numRead = read(in_fd, buf, toRead);
        if (numRead == -1)
            return -1;
        if (numRead == 0)
            break;                      /* EOF */

	numSent = 0;
	while (numSent < numRead) {
            tmp = write(out_fd, buf + numSent, numRead - numSent);
            if (numSent == -1)
		return -1;
            if (numSent == 0)               /* Should never happen */
                printf("sendfile:%d write() transferred 0 bytes.\n", __LINE__);
	    numSent += tmp;
	}

        count -= numSent;
        totSent += numSent;
    }

    return totSent;
}
#define STACK_CHK_GUARD 0x00000000    /* xiaohu.huang need to use the real stack guard pointer */
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;
void __stack_chk_fail(void)
{
	printf("stack is corrupted\n");
#if ENABLE_STACKTRACE
	dump_stack();
#endif
	configASSERT(0);
}
