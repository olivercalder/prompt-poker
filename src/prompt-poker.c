#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int create_write_chmod_file(char *filepath) {
	/* create */
	const char *msg1 = "hello prompting\n";
	const char *msg2 = "hello again\n";
	size_t len_written;
	int result;
	int fd = open(filepath, O_WRONLY|O_CREAT, 0600);
	if (fd < 0) {
		return errno;
	}
	/* write */
	len_written = write(fd, msg1, strlen(msg1));
	if (len_written < 0) {
		goto cleanup;
	}
	/* chmod */
	result = fchmod(fd, 0644);
	if (result < 0) {
		goto cleanup;
	}
	/* sync */
	result = fsync(fd);
	if (result < 0) {
		goto cleanup;
	}
	/* write again */
	len_written = write(fd, msg2, strlen(msg2));
	if (len_written < 0) {
		goto cleanup;
	}
cleanup:
	close(fd);
	return errno;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "USAGE: %s FILEPATH\n", argv[0]);
	}
	return create_write_chmod_file(argv[1]);
}
