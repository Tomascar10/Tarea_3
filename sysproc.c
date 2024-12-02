#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

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
int sys_send(void) {
    int pid;
    char *msg_content;

    if (argint(0, &pid) < 0 || argstr(1, &msg_content) < 0)
        return -1;

    acquire(&msg_queue.lock);

    if (msg_queue.count == MSG_QUEUE_SIZE) {
        release(&msg_queue.lock);
        return -1; // Cola llena
    }

    message *msg = &msg_queue.messages[msg_queue.tail];
    msg->sender_pid = myproc()->pid;
    safestrcpy(msg->content, msg_content, sizeof(msg->content));
    msg_queue.tail = (msg_queue.tail + 1) % MSG_QUEUE_SIZE;
    msg_queue.count++;

    wakeup(&msg_queue); // Despertar procesos bloqueados
    release(&msg_queue.lock);

    return 0; // Mensaje enviado
}
int sys_receive(void) {
    char *msg_content;

    if (argstr(0, &msg_content) < 0)
        return -1;

    acquire(&msg_queue.lock);

    while (msg_queue.count == 0) {
        sleep(&msg_queue, &msg_queue.lock); // Bloquearse si la cola está vacía
    }

    message *msg = &msg_queue.messages[msg_queue.head];
    safestrcpy(msg_content, msg->content, sizeof(msg->content));
    msg_queue.head = (msg_queue.head + 1) % MSG_QUEUE_SIZE;
    msg_queue.count--;

    release(&msg_queue.lock);

    return msg->sender_pid; // Retorna el PID del remitente
}


