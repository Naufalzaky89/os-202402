#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"

struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

static struct proc *initproc __attribute__((unused));


int nextpid = 1;
extern void forkret(void);
extern void trapret(void);
extern void swtch(struct context**, struct context*);
extern char* kalloc(void);
extern void kfree(char*);

// SHM globals
extern char* shm_frames[64];
extern int shm_refcnt[64];

void
pinit(void)
{
  initlock(&ptable.lock, "ptable");
}

// Find running process
struct proc*
myproc(void)
{
  pushcli();
  struct proc *p = 0;
  for(int i = 0; i < NPROC; i++) {
    if(ptable.proc[i].state == RUNNING) {
      p = &ptable.proc[i];
      break;
    }
  }
  popcli();
  return p;
}

static struct proc*
allocproc(void)
{
  struct proc *p;
  char *sp;

  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->state == UNUSED){
      goto found;
    }
  }
  release(&ptable.lock);
  return 0;

found:
  p->state = EMBRYO;
  p->pid = nextpid++;
  release(&ptable.lock);

  if((p->kstack = kalloc()) == 0){
    p->state = UNUSED;
    return 0;
  }
  sp = p->kstack + KSTACKSIZE;

  sp -= sizeof *p->tf;
  p->tf = (struct trapframe*)sp;

  sp -= sizeof *p->context;
  p->context = (struct context*)sp;
  memset(p->context, 0, sizeof *p->context);
  p->context->eip = (uint)forkret;

  // SHM init
  p->shm_va = (char*)(KERNBASE - PGSIZE);
  memset(p->shm_keys, 0, sizeof(p->shm_keys));

  return p;
}

int
fork(void)
{
  int i, pid;
  struct proc *np;
  struct proc *curproc = myproc();

  if((np = allocproc()) == 0){
    return -1;
  }

  if((np->pgdir = copyuvm(curproc->pgdir, curproc->sz)) == 0){
    kfree(np->kstack);
    np->state = UNUSED;
    return -1;
  }

  np->sz = curproc->sz;
  np->parent = curproc;
  *np->tf = *curproc->tf;
  np->tf->eax = 0;

  for(i = 0; i < NOFILE; i++)
    if(curproc->ofile[i])
      np->ofile[i] = filedup(curproc->ofile[i]);
  np->cwd = idup(curproc->cwd);
  safestrcpy(np->name, curproc->name, sizeof(curproc->name));

  // Copy shm keys
  for(i = 0; i < 64; i++) {
    if(curproc->shm_keys[i])
    memmove(np->shm_keys, curproc->shm_keys, sizeof(curproc->shm_keys));
np->shm_va = curproc->shm_va;
 {
      np->shm_keys[i] = curproc->shm_keys[i];
      mappages(np->pgdir,
        (char*)(KERNBASE - (i + 1) * PGSIZE),
        PGSIZE,
        V2P(shm_frames[i]),
        PTE_W|PTE_U);
      shm_refcnt[i]++;
    }
  }

  pid = np->pid;

  acquire(&ptable.lock);
  np->state = RUNNABLE;
  release(&ptable.lock);

  return pid;
}
