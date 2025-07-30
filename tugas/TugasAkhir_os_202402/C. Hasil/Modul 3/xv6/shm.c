#include "types.h"
#include "defs.h"
#include "mmu.h"
#include "proc.h"
#include "param.h"
#include "memlayout.h" // agar USERTOP terdefinisi di xv6-public

#define SHM_KEY_MAX 64

#define SHMBASE (USERTOP - PGSIZE)

struct {
  char *frames[SHM_KEY_MAX];
  int refcnt[SHM_KEY_MAX];
} shmtable;

char*
shmget(int key)
{
  struct proc *curproc = myproc();

  if (key < 0 || key >= SHM_KEY_MAX)
    return 0;

  // Sudah pernah dipasang sebelumnya
  if (curproc->shm_keys[key])
    return (char*)curproc->shm_keys[key];

  // Alokasi frame baru jika belum ada
  if (shmtable.frames[key] == 0) {
    shmtable.frames[key] = kalloc();
    if (!shmtable.frames[key]) return 0;
    memset(shmtable.frames[key], 0, PGSIZE);
    shmtable.refcnt[key] = 1;
  } else {
    shmtable.refcnt[key]++;
  }

  // Alamat virtual untuk SHM diletakkan di bawah USERTOP
  if (curproc->shm_va == 0)
    curproc->shm_va = USERTOP;

  curproc->shm_va -= PGSIZE;

  if (mappages(curproc->pgdir, (void*)curproc->shm_va, PGSIZE,
               v2p(shmtable.frames[key]), PTE_W | PTE_U) < 0)
    return 0;

  curproc->shm_keys[key] = (int)curproc->shm_va;

  return (char*)curproc->shm_va;
}

void
shmrelease(int key)
{
  if (key < 0 || key >= SHM_KEY_MAX)
    return;

  if (shmtable.refcnt[key] > 0)
    shmtable.refcnt[key]--;

  if (shmtable.refcnt[key] == 0) {
    kfree(shmtable.frames[key]);
    shmtable.frames[key] = 0;
  }
}
