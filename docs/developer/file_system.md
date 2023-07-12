How to Use File System    {#filesystem}
==========================

If you want to use the filesystem, you need to include ***<sys_vfs.h>*** header files.

### How to create/delete directory ###

#### Create directory

```c
void directory_create(void)
{
#if defined(CONFIG_SPINAND)
#define DEMOPATH "/data/demo"
#else
#define DEMOPATH "/user/demo"
#endif

	int err = mkdir(DEMOPATH, 0755);
	if (err)
		printk("Error creating %s (%d)", DEMOPATH, err);
}
```

#### Delete directory

```c
void delete_directory(void)
{
#if defined(CONFIG_SPINAND)
#define DEMOPATH "/data/demo"
#else
#define DEMOPATH "/user/demo"
#endif

	int err = remove(DEMOPATH);
	if (err)
		printk("Failed to remove %s (%d)", path, err);
}
```

### How to create/delete file ###

#### Create file

```c
void file_create(void)
{
#if defined(CONFIG_SPINAND)
#define DEMOPATH "/data/demo"
#else
#define DEMOPATH "/user/demo"
#endif

	int fd = open(DEMOPATH, O_RDWR | O_CREAT);
	if (fd == -1)
		printk("open %s error\r\n", DEMOPATH);
	else
		close(fd);
}
```

#### Delete file

```c
void file_delete(void)
{
#if defined(CONFIG_SPINAND)
#define DEMOPATH "/data/demo"
#else
#define DEMOPATH "/user/demo"
#endif

	int err = remove(DEMOPATH);
	if (err)
		printk("Failed to remove %s (%d)", DEMOPATH, err);
}
```
### How to write/read file ###

#### Read file

```c
void file_read(void)
{
#if defined(CONFIG_SPINAND)
#define DEMOPATH "/data/demo"
#else
#define DEMOPATH "/user/demo"
#endif
	char buffer[81];
	uint32_t length;
	int fd = open(DEMOPATH, O_RDONLY);

	if (fd == -1) {
		printk("open %s error\r\n", DEMOPATH);
		return;
	}

	do {
	memset(buffer, 0, sizeof(buffer));
	length = read(fd, buffer, sizeof(buffer) - 1);
	if (length > 0)
		printk("%s", buffer);
	} while (length > 0);

	close(fd);
}
```

#### Write file

```c
void file_write(void)
{
#if defined(CONFIG_SPINAND)
#define DEMOPATH "/data/demo"
#else
#define DEMOPATH "/user/demo"
#endif

	int fd = open(DEMOPATH, O_RDWR | O_CREAT);
	if (fd == -1) {
		printk("open %s error\r\n", DEMOPATH);
		return;
	}

	write(fd, "amlogic demo", sizeof("amlogic demo"));

	close(fd);
}
```
