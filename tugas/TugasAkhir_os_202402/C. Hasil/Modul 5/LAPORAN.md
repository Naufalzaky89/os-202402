# 📝 Laporan Tugas Akhir

**Mata Kuliah**: Sistem Operasi  
**Semester**: Genap / Tahun Ajaran 2024–2025  
**Nama**: `Naufal Zaky`  
**NIM**: `240202874`  
**Modul yang Dikerjakan**:  
`Modul 5 – Audit dan Keamanan Sistem (System Call Audit Log)`

---

## 📌 Deskripsi Singkat Tugas

* **Modul 5 – Audit dan Keamanan Sistem**:  
  Merekam semua pemanggilan system call ke dalam audit log kernel dan menyediakan syscall baru `get_audit_log()` yang hanya dapat diakses oleh proses dengan PID 1 (biasanya init). Audit log mencatat PID, nomor syscall, dan waktu (tick) saat dipanggil.

---

## 🛠️ Rincian Implementasi

* Menambahkan struct `audit_entry` di `syscall.c` sebagai penyimpan log
* Menambahkan array `audit_log[]` dan indeks `audit_index` global
* Menambahkan kode logging di fungsi `syscall()` untuk mencatat setiap system call yang valid
* Menambahkan syscall `get_audit_log()`:
  - Implementasi di `sysproc.c`
  - Deklarasi di `defs.h`, `user.h`, `syscall.h`, `usys.S`, dan pendaftaran di `syscall.c`
* Validasi PID agar hanya proses PID 1 yang bisa mengakses audit log
* Membuat program uji `audit.c` untuk membaca dan menampilkan isi log
* Menambahkan `audit.c` ke dalam `Makefile`

---

## ✅ Uji Fungsionalitas

Program uji yang digunakan:

* `audit`: untuk menampilkan isi log audit system call (harus dijalankan sebagai PID 1)

---

## 📷 Hasil Uji

### 📍 Output `audit` saat dijalankan sebagai PID 1:

=== Audit Log ===
[0] PID=1 SYSCALL=5 TICK=12
[1] PID=1 SYSCALL=6 TICK=13
[2] PID=1 SYSCALL=7 TICK=14

### 📍 Output `audit` saat dijalankan sebagai proses biasa:

Access denied or error.

---

## ⚠️ Kendala yang Dihadapi

* Awalnya lupa validasi `proc->pid != 1`, sehingga semua proses bisa mengakses audit log
* Salah menentukan ukuran buffer saat memanggil `argptr()`
* Lupa menyertakan `audit.c` di `Makefile`, menyebabkan program tidak ter-compile
* Kesalahan pointer saat `memmove()` menyebabkan kernel panic

---

## 📚 Referensi

* Buku xv6 MIT: [https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf](https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf)
* Repositori xv6-public: [https://github.com/mit-pdos/xv6-public](https://github.com/mit-pdos/xv6-public)
* Diskusi praktikum dan debugging langsung melalui output QEMU
* Dokumentasi system call di xv6 dan studi kasus logging syscall

---
