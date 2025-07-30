# 📝 Laporan Tugas Akhir

**Mata Kuliah**: Sistem Operasi  
**Semester**: Genap / Tahun Ajaran 2024–2025  
**Nama**: `Naufal Zaky`  
**NIM**: `240202874`  
**Modul yang Dikerjakan**:  
`Modul 3 – Implementasi Shared Memory (shmget dan shmrelease)`

---

## 📌 Deskripsi Singkat Tugas

* **Modul 3 – Implementasi Shared Memory**:  
  Menambahkan fitur shared memory ke dalam kernel XV6 dengan dua system call: `shmget()` untuk mendapatkan akses ke shared memory berdasarkan key, dan `shmrelease()` untuk melepas shared memory. Modul ini menguji kemampuan berbagi data antar proses menggunakan memori bersama.

---

## 🛠️ Rincian Implementasi

* Menambahkan dua system call baru di file `sysproc.c` dan `shm.c`
* Mengedit `user.h`, `usys.S`, dan `syscall.h` untuk mendaftarkan syscall baru (`shmget`, `shmrelease`)
* Menambahkan struktur `shmtable` dan `shmpage` di `shm.c` dengan maksimum 64 halaman shared memory
* Menyimpan virtual address shared memory di setiap proses (`shm_va`) dan mengatur mapping saat `fork()`
* Mengimplementasikan reference counting agar shared memory dilepas jika tidak ada proses yang menggunakan
* Mengedit `fork()` di `proc.c` agar child mewarisi shared memory dari parent
* Menambahkan file uji `shmtest.c`

---

## ✅ Uji Fungsionalitas

Program uji yang digunakan:

* `shmtest`: untuk menguji fungsi `shmget()` dan `shmrelease()` serta berbagi data antara parent dan child
* `cowtest` (uji tambahan): memastikan tidak terjadi copy-on-write saat shared memory digunakan

---

## 📷 Hasil Uji

### 📍 Contoh Output `shmtest`:

Child reads: A
Parent reads: B

### 📍 Screenshot Output Terminal:


> (Catatan: jika belum ada screenshot, hapus baris di atas atau ganti dengan output nyata.)

---

## ⚠️ Kendala yang Dihadapi

* Mengalami error `undefined reference to 'proc'` saat mencoba implementasi awal karena ketidaksesuaian antara versi XV6 yang digunakan dengan referensi online
* Tidak adanya `pagetable`, `mycpu()`, dan `trapframe` di XV6-public versi lama menyebabkan error saat mengadopsi kode dari versi baru
* Harus membangun ulang `proc.c` dan mengganti implementasi `myproc()` agar kompatibel
* Salah mapping alamat shared memory menyebabkan kernel panic ketika proses `fork` dilakukan
* Menyesuaikan mapping ke alamat `USERTOP` dan menghindari konflik dengan stack user

---

## 📚 Referensi

* Buku xv6 MIT: [https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf](https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf)
* Repositori xv6-public: [https://github.com/mit-pdos/xv6-public](https://github.com/mit-pdos/xv6-public)
* Diskusi GitHub Issues dan Stack Overflow
* Arahan dosen dan asisten praktikum saat debugging kernel
* Dokumentasi dan hasil debugging pribadi pada file `shm.c`, `proc.c`, dan `vm.c`
