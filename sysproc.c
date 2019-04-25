#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

#include "syscall.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

/****************************NEW SYSCALL IMPLEMENTATIONS**********************/
char *name_of_syscall[NUM_SYSCALLS + 1] = {
" ",
"sys_fork",
"sys_exit",
"sys_wait",
"sys_pipe",
"sys_read",
"sys_kill",
"sys_exec",
"sys_fstat",
"sys_chdir",
"sys_dup",
"sys_getpid",
"sys_sbrk",
"sys_sleep",
"sys_uptime",
"sys_open",
"sys_write",
"sys_mknod",
"sys_unlink",
"sys_link",
"sys_mkdir",
"sys_close",

// new syscall numbers
"sys_print_count",
"sys_toggle",
"sys_add",
"sys_ps",
"sys_send",
"sys_recv",

//
"sys_create_container",
"sys_destroy_container",
"sys_join_container",
"sys_leave_container",
"sys_get_container",
// "sys_scheduler_log_on",
// "sys_scheduler_log_off",
};

int syscall_counter[NUM_SYSCALLS + 1];

int
sys_print_count(void)
{
    for (int i = 0; i < NUM_SYSCALLS + 1; i++)
    {
        if (syscall_counter[i] != 0)
            cprintf("%s %d\n", name_of_syscall[i], syscall_counter[i]);
    }
    return 0;
}

int trace_state;
int
sys_toggle(void)
{
    trace_state  = 1 - trace_state;
    for (int i = 0; i < NUM_SYSCALLS + 1; i++)
    {
        syscall_counter[i] = 0;
    }
    return 0;
}

int
sys_add(void)
{
    int a, b;
    if (argint(0, &a) < 0)
        return -1;
    if (argint(1, &b) < 0)
        return -1;
    return a + b;
}

int
sys_ps(void)
{
    return ps();
}

int
sys_send()
{
    int sender_pid, rec_pid;
    char * msg;

    if (argint(0, &sender_pid) < 0)
        return -1;
    if (argint(1, &rec_pid) < 0)
        return -1;
    if (argstr(2, &msg) < 0)
        return -1;
    return send_message(sender_pid, rec_pid, msg);
}

int
sys_recv()
{
    char * msg;
    if (argstr(0, &msg) < 0)
        return -1;
    return recv_message(msg);
}
/*****************************************************************************/
// Assignment 3 syscalls

int
sys_create_container(void)
{
  return create_container();
}

int
sys_destroy_container(void)
{
  uint cid;
  if(argint(0, (int*)&cid) < 0) return -1;
  return destroy_container(cid);
} 

int
sys_join_container(void)
{
  uint cid;
  if(argint(0, (int*)&cid) < 0) return -1;
  return join_container(cid);
}

int
sys_leave_container(void)
{
  return leave_container();
}

int
sys_get_container(void)
{
  return myproc()->cid;
}

int
sys_scheduler_log_on(void){
  // ptable.log = 1;
  return logoff();
}

int
sys_scheduler_log_off(void){
  // ptable.log = 0;
  return logon();
}
