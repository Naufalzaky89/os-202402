#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "spinlock.h"

// Simple spinlock implementation.

void
initlock(struct spinlock *lk, char *name)
{
  lk->name = name;
  lk->locked = 0;
}

// Acquire the lock.
void
acquire(struct spinlock *lk)
{
  pushcli(); // Disable interrupts.

  if(holding(lk))
    panic("acquire");

  while(xchg(&lk->locked, 1) != 0)
    ;

  // Lock acquired.
}

// Release the lock.
void
release(struct spinlock *lk)
{
  if(!holding(lk))
    panic("release");

  lk->locked = 0;

  popcli();
}

// Check if current CPU is holding the lock.
int
holding(struct spinlock *lk)
{
  return lk->locked && (readeflags() & FL_IF) == 0;
}

// Global interrupt nesting counter.
static int ncli = 0;
static int intena;

// Disable interrupts and record nesting.
void
pushcli(void)
{
  int eflags = readeflags();
  cli();
  if(ncli == 0)
    intena = eflags & FL_IF;
  ncli++;
}

// Restore interrupts if outermost level.
void
popcli(void)
{
  if((readeflags() & FL_IF))
    panic("popcli - interruptible");

  ncli--;
  if(ncli < 0)
    panic("popcli");

  if(ncli == 0 && intena)
    sti();
}