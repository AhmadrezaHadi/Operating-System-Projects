# Operating-System-Projects

This repository contains my Operating System course projects.
# First Project (adding a new system call):
In the first project We added a new system call that counts how many times __read()__ syscall is called by any process running.

### How it works:
Simply we added a global variable in __readcount.h__ and initialized it in __syscall.c__, and whenever a syscall is called by a program kernel routes to          __syscall()__ function in __syscall.c__ and here we added code below:
#### void syscall(void):
    void
    syscall(void)
    {
      int num;
      struct proc *curproc = myproc();

      num = curproc->tf->eax;
      if(num == SYS_read){
        global_read_counter = global_read_counter + 1;
      }
      if(num > 0 && num < NELEM(syscalls) && syscalls[num]) {
        curproc->tf->eax = syscalls[num]();
      } else {
        cprintf("%d %s: unknown sys call %d\n",
                curproc->pid, curproc->name, num);
        curproc->tf->eax = -1;
      }
    }
#### __getreadcount()__ function defined in __sysproc.c__:
    int
    sys_getreadcount(void)
    {
      return global_read_counter;         // Global counter for read() syscall
    }
#### Other changes:
> Created __readcount.h__ and added __global_read_counter__.
> Added __SYSCALL(getreadcount)__ in usys.S
> Added new syscall in __user.h__ in order to be accessible by other user programs
> Added new argument to syscalls function array in __syscall.c__
> Inculded __getreadcount()__ syscall in __syscall.c__
