/*
 *        buddy.c - RhythmOS
 *
 *        Codyright (C) 2011 Dustin Dorroh <dustindorroh@gmail.com>
 *
 *        RhythmOS is free software: you can redistribute it and/or modify
 *        it under the terms of the GNU General Public License as published by
 *        the Free Software Foundation, either version 3 of the License, or
 *        (at your option) any later version.
 *
 *        RhythmOS is distributed in the hope that it will be useful,
 *        but WITHOUT ANY WARRANTY; without even the implied warranty of
 *        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *        GNU General Public License for more details.
 *
 *        You should have received a copy of the GNU General Public License
 *        along with RhythmOS.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "kernel.h"
#include "filesystem.h"

/*
 * Prototypes for system call handlers defined in other files
 */

/*
 * pipe.c
 */
int syscall_close(int fd);
int syscall_pipe(int filedes[2]);
int syscall_dup2(int oldfd, int newfd);

/*
 * unixproc.c
 */
pid_t syscall_fork(regs * r);
int syscall_execve(const char *filename, char *const argv[],
		   char *const envp[], regs * r);
pid_t syscall_waitpid(pid_t pid, int *status, int options);

/*
 * fscalls.c
 */
int syscall_stat(const char *path, struct stat *buf);
int syscall_open(const char *pathname, int flags);
int syscall_getdent(int fd, struct dirent *entry);
int syscall_chdir(const char *path);
char *syscall_getcwd(char *buf, size_t size);

extern process *current_process;
process processes[MAX_PROCESSES];

/**
 * valid_pointer
 *
 * Checks to see if a pointer supplied to a system call is valid,
 * i.e. resides within the current process's user-accessible address
 * space. This is a security measure to stop processes from trying to
 * subvert the restrictions of user mode by tricking the kernel into
 * reading from or writing to an area of memory that the process would
 * not normally have access to.
 *
 * Any system call which has an invalid pointer supplied to it is
 * supposed to return -EFAULT.
 */
int valid_pointer(const void *ptr, unsigned int size)
{
	unsigned int start_address = (unsigned int)ptr;
	unsigned int end_address = start_address + size;

	if (0 == size)
		return 1;

	if (end_address < start_address)
		return 0;

	/*
	 * Within stack segment?
	 */
	if ((start_address >= current_process->stack_start) &&
	    (end_address <= current_process->stack_end))
		return 1;

	/*
	 * Within data segment?
	 */
	if ((start_address >= current_process->data_start) &&
	    (end_address <= current_process->data_end))
		return 1;

	/*
	 * Within text segment?
	 */
	if ((start_address >= current_process->text_start) &&
	    (end_address <= current_process->text_end))
		return 1;

	/*
	 * Pointer is invalid
	 */
	return 0;
}

/**
 * valid_string
 *
 * Similar to valid_pointer. In the case of strings, we can simply check
 * for a particular length, since they are just arrays of characters
 * terminated by '\0'.  So instead we have to scan through the string,
 * checking that each part is valid until we encounter the NULL
 * terminator.
 */
int valid_string(const char *str)
{
	unsigned int len = 0;
	while (valid_pointer(str, len + 1)) {
		if ('\0' == str[len])
			return 1;
		len++;
	}
	return 0;
}

/**
 * syscall_getpid
 *
 * Returns the identifier of the calling process
 */
static pid_t syscall_getpid()
{
	return current_process->pid;
}

/**
 * syscall_exit
 *
 * Terminates the calling process. The specified exit status is recorded
 * in the exit_status field of the process, which can later be retrieved
 * by the parent process using waitpid.
 */
static int syscall_exit(int status)
{
	disable_paging();
	current_process->exit_status = status;
	kill_process(current_process);
	return -ESUSPEND;
}

/**
 * syscall_write
 *
 * Writes the specified data to a file handle. What actually happens to
 * this data will depend on the type of file handle - it could be
 * printed to the screen, written to a pipe, written to a file etc. This
 * function uses the filehandle object corresponding to the specified
 * file descriptor to obtain a pointer to the handle's write function,
 * to which it then dispatches the call.
 */
static ssize_t syscall_write(int fd, const void *buf, size_t count)
{
	if (!valid_pointer(buf, count))
		return -EFAULT;
	if ((0 > fd) || (MAX_FDS <= fd)
	    || (NULL == current_process->filedesc[fd]))
		return -EBADF;

	filehandle *fh = current_process->filedesc[fd];
	return fh->write(fh, buf, count);
}

/**
 * syscall_read
 *
 * Read some data from a file descriptor. As for write, the way in which
 * the data is read depends on what type of file handle the descriptor
 * refers to. This function simply dispatches to the handle's read
 * function.
 */
static ssize_t syscall_read(int fd, void *buf, size_t count)
{
	if (!valid_pointer(buf, count))
		return -EFAULT;
	if ((0 > fd) || (MAX_FDS <= fd)
	    || (NULL == current_process->filedesc[fd]))
		return -EBADF;

	filehandle *fh = current_process->filedesc[fd];
	return fh->read(fh, buf, count);
}

/**
 * syscall_geterrno
 *
 * Retrieve the error code of the last system call. This is called
 * whenever a process access errno, which is defined in user.h as a call
 * to this function.
 *
 * Some systems take an alternative approach by making errno a global
 * variable within the process. We use a system call instead, since
 * global variables require each process to have a private text segment,
 * which is not the case for most versions of our kernel.
 */
static pid_t syscall_geterrno(void)
{
  return current_process->last_errno;
}

/**
 * syscall_brk
 *
 * We might have changed to a diff.erent process; if
 * this is the case, perform a context switch
 *
 */
if (old_current != current_process){
  context_switch(r);
 }

}
