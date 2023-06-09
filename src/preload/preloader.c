//
// Purpose:
// Very simple preloader that runs a prolog and epilog and intercepts certain file based 
// system calls
//
// Compile Instructions:
// gcc -fPIC -shared -o preloader.so preloader.c -ldl
//
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <stdarg.h>

static const char *redirect_name(const char *name)
{
	return name;
}

static __attribute__((constructor)) void prolog(void) {
  fprintf(stdout, "Running preload prolog\n");
}

static __attribute__((destructor)) void epilog(void) {
  fprintf(stdout, "Running preload epilog\n");
}

int open(const char *filename, int flags, ...) {
	static int (*real_open)(const char *, int, mode_t);
	int ret;
	va_list ap;
	mode_t mode;

	if (!real_open) {
		real_open = dlsym(RTLD_NEXT, "open");
	}

	va_start(ap, flags);
	mode = va_arg(ap, mode_t);
	va_end(ap);

	ret = real_open(filename, flags, mode);
	fprintf(stdout, "open(%s, 0x%x, %o, %d)\n", filename, flags, mode, ret);
	return ret;	
}

int open64(const char *filename, int flags, ...) {
	static int (*real_open64)(const char *, int, mode_t);
	int ret;
	va_list ap;
	mode_t mode;

	if (!real_open64) {
		real_open64 = dlsym(RTLD_NEXT, "open64");
	}

	va_start(ap, flags);
	mode = va_arg(ap, mode_t);
	va_end(ap);

	ret = real_open64(filename, flags, mode);
	fprintf(stdout, "open64(%s, 0x%x, %o, %d)\n", filename, flags, mode, ret);
	return ret;	
}

int close(int filedes) {
	static int (*real_close)(int);
	int ret;

	if (!real_close) {
		real_close = dlsym(RTLD_NEXT, "close");
	}

	ret = real_close(filedes);
	fprintf(stdout, "close(%d, %d)\n", filedes, ret);
	return ret;	
}

ssize_t read(int fd, void *buf, size_t count) {
  static ssize_t (*real_read)(int, void *, size_t);
  ssize_t ret;

  if (!real_read) {
    real_read = dlsym(RTLD_NEXT, "read");
  }

  char filepath[PATH_MAX];
  char fd_path[PATH_MAX];
  snprintf(fd_path, sizeof(fd_path), "/proc/self/fd/%d", fd);
  ssize_t len = readlink(fd_path, filepath, sizeof(filepath) - 1);
  if (len != -1) {
    filepath[len] = '\0';
  } else {
    filepath[0] = '\0';
  }

  ret = real_read(fd, buf, count);
  fprintf(stdout, "read(%d, %lu, %s, %d)\n", fd, count, filepath, ret);
  return ret;
}


ssize_t write(int fd, const void *buf, size_t count) {
  static ssize_t (*real_write)(int, const void *, size_t);
  ssize_t ret;

  if (!real_write) {
    real_write = dlsym(RTLD_NEXT, "write");
  }

  char filepath[PATH_MAX];
  char fd_path[PATH_MAX];
  snprintf(fd_path, sizeof(fd_path), "/proc/self/fd/%d", fd);
  ssize_t len = readlink(fd_path, filepath, sizeof(filepath) - 1);
  if (len != -1) {
    filepath[len] = '\0';
  } else {
    filepath[0] = '\0';
  }

  ret = real_write(fd, buf, count);
  fprintf(stdout, "write(%d, %lu, %s, %d)\n", fd, count, filepath, ret);
  return ret;
}

FILE *fopen(const char *filename, const char *mode) {
	static FILE *(*real_fopen)(const char *, const char *);
	FILE *ret;

	if (!real_fopen) {
		real_fopen = dlsym(RTLD_NEXT, "fopen");
	}

	ret = real_fopen(filename, mode);
	fprintf(stdout, "fopen(%s, %s, %p)\n", filename, mode, ret);
	return ret;	
}

FILE *fopen64(const char *filename, const char *mode) {
	static FILE *(*real_fopen64)(const char *, const char *);
	FILE *ret;

	if (!real_fopen64) {
		real_fopen64 = dlsym(RTLD_NEXT, "fopen64");
	}

	ret = real_fopen64(filename, mode);
	fprintf(stdout, "fopen64(%s, %s, %p)\n", filename, mode, ret);
	return ret;	
}

int fclose(FILE *stream) {
	static int (*real_fclose)(FILE *);
	int ret;

	if (!real_fclose) {
		real_fclose = dlsym(RTLD_NEXT, "fclose");
	}

	ret = real_fclose(stream);
	fprintf(stdout, "fclose(%p, %d)\n", stream, ret);
	return ret;	
}

size_t fread(void *ptr, size_t size, size_t count, FILE *stream) {
  static size_t (*real_fread)(void *, size_t, size_t, FILE *);
  size_t ret;

  if (!real_fread) {
    real_fread = dlsym(RTLD_NEXT, "fread");
  }

  ret = real_fread(ptr, size, count, stream);
  fprintf(stdout, "fread(%lu, %lu, %d)\n", size, count, ret);
  return ret;
}

size_t fwrite(const void *ptr, size_t size, size_t count, FILE *stream) {
  static size_t (*real_fwrite)(const void *, size_t, size_t, FILE *);
  size_t ret;

  if (!real_fwrite) {
    real_fwrite = dlsym(RTLD_NEXT, "fwrite");
  }

  ret = real_fwrite(ptr, size, count, stream);
  fprintf(stdout, "fwrite(%lu, %lu, %d)\n", size, count, ret);
  return ret;
}

void sync(void) {
  static void (*real_sync)(void);

  if (!real_sync) {
    real_sync = dlsym(RTLD_NEXT, "sync");
  }

  real_sync();
  fprintf(stdout, "sync\n");
  return;
}

int fsync(int fd) {
  static int (*real_fsync)(int);
  int ret;

  if (!real_fsync) {
    real_fsync = dlsym(RTLD_NEXT, "fsync");
  }

  ret = real_fsync(fd);
  fprintf(stdout, "fsync(%d, %d)\n", fd, ret);
  return ret;
}

int puts(const char *str) {
  static int (*real_puts)(const char *);
  int ret;

  if (!real_puts) {
    real_puts = dlsym(RTLD_NEXT, "puts");
  }

  ret = real_puts(str);
  fprintf(stdout, "puts(%lu, %d)\n", strlen(str), ret);
  return ret;
}
