#include <iostream>    
#include <string>      
#include <ctime>       
#include <iomanip>     
#include <stdexcept>   
#include <cstdlib>    
#include <locale>     

#define MAX_PENGGUNA 100
#define MAX_BARANG 100
#define MAX_LOG 100

using namespace std;

// Struktur untuk menyimpan data akun
struct Akun {
    string nama;
    string pw;
    string peran; // "admin" atau "user"
};

// Struktur untuk menyimpan data barang
struct Barang {
    string nama;
    int stok;
    double harga;
};

// Struktur untuk menyimpan log aktivitas
struct Log {
    string waktu;
    string aksi;
    string detail;
};

// Struktur untuk menyimpan riwayat pembelian
struct Pembelian {
    string namaBarang;
    int jumlah;
    string waktu;
    double hargaTotal; // Tambahan untuk menyimpan total harga pembelian
};

// Struktur untuk menyimpan data pengguna
struct Pengguna {
    Akun akun;
    Pembelian riwayatPembelian[MAX_LOG];
    int jumlahPembelian;
};

// Struktur untuk menyimpan semua log aktivitas
struct LogSistem {
    Log logs[MAX_LOG];
    int jumlahLog;
};

// Inventaris global
Barang inventarisGlobal[MAX_BARANG];
int jumlahBarangGlobal = 0;

// Fungsi untuk mendapatkan waktu saat ini sebagai string
string getWaktuSekarang() {
    time_t now = time(0);
    char* dt = ctime(&now);
    string waktu = dt;
    return waktu.substr(0, waktu.length() - 1); // Hapus newline
}

// Fungsi untuk menambah log aktivitas
void tambahLog(LogSistem &logSistem, string aksi, string detail) {
    if (logSistem.jumlahLog < MAX_LOG) {
        logSistem.logs[logSistem.jumlahLog].waktu = getWaktuSekarang();
        logSistem.logs[logSistem.jumlahLog].aksi = aksi;
        logSistem.logs[logSistem.jumlahLog].detail = detail;
        logSistem.jumlahLog++;
    }
}

// Fungsi untuk membersihkan layar (kompatibel Windows/Linux/macOS)
void clearScreen() {
#ifdef _WIN32
    system("cls"); // Untuk Windows
#else
    system("clear"); // Untuk Linux/macOS
#endif
}

// Fungsi Insertion Sort untuk mengurutkan stok barang secara ascending
void insertionSortStokAscending(Barang inventaris[], int jumlahBarang) {
    for (int i = 1; i < jumlahBarang; i++) {
        Barang key = inventaris[i];
        int j = i - 1;
        while (j >= 0 && inventaris[j].stok > key.stok) {
            inventaris[j + 1] = inventaris[j];
            j--;
        }
        inventaris[j + 1] = key;
    }
}

// Fungsi Insertion Sort untuk mengurutkan stok barang secara descending
void insertionSortStokDescending(Barang inventaris[], int jumlahBarang) {
    for (int i = 1; i < jumlahBarang; i++) {
        Barang key = inventaris[i];
        int j = i - 1;
        while (j >= 0 && inventaris[j].stok < key.stok) {
            inventaris[j + 1] = inventaris[j];
            j--;
        }
        inventaris[j + 1] = key;
    }
}

// Fungsi Insertion Sort untuk mengurutkan nama barang secara ascending (A-Z)
void insertionSortNamaAscending(Barang inventaris[], int jumlahBarang) {
    for (int i = 1; i < jumlahBarang; i++) {
        Barang key = inventaris[i];
        int j = i - 1;
        while (j >= 0 && inventaris[j].nama > key.nama) {
            inventaris[j + 1] = inventaris[j];
            j--;
        }
        inventaris[j + 1] = key;
    }
}

// Fungsi Insertion Sort untuk mengurutkan nama barang secara descending (Z-A)
void insertionSortNamaDescending(Barang inventaris[], int jumlahBarang) {
    for (int i = 1; i < jumlahBarang; i++) {
        Barang key = inventaris[i];
        int j = i - 1;
        while (j >= 0 && inventaris[j].nama < key.nama) {
            inventaris[j + 1] = inventaris[j];
            j--;
        }
        inventaris[j + 1] = key;
    }
}

// Fungsi untuk menampilkan inventaris
void tampilInventaris() {
    try {
        if (jumlahBarangGlobal == 0) {
            throw runtime_error("Inventaris kosong.");
        }

        int pilihan_urutan;
        cout << "--- Menampilkan Inventaris ---" << endl;
        cout << "Pilih metode pengurutan:" << endl;
        cout << "1. Stok Barang (Kecil ke Besar)" << endl;
        cout << "2. Stok Barang (Besar ke Kecil)" << endl;
        cout << "3. Nama Barang (A-Z)" << endl;
        cout << "4. Nama Barang (Z-A)" << endl;
        cout << "5. Tanpa Pengurutan" << endl;
        cout << "Pilihan: ";
        cin >> pilihan_urutan;

        // Validasi input numerik
        if (cin.fail()) {
            cin.clear();
            while (cin.get() != '\n'); // Buang sisa karakter
            throw invalid_argument("Input harus berupa angka.");
        }
        while (cin.get() != '\n'); // Buang sisa karakter

        switch (pilihan_urutan) {
            case 1:
                insertionSortStokAscending(inventarisGlobal, jumlahBarangGlobal);
                break;
            case 2:
                insertionSortStokDescending(inventarisGlobal, jumlahBarangGlobal);
                break;
            case 3:
                insertionSortNamaAscending(inventarisGlobal, jumlahBarangGlobal);
                break;
            case 4:
                insertionSortNamaDescending(inventarisGlobal, jumlahBarangGlobal);
                break;
            case 5:
                // Tanpa pengurutan
                break;
            default:
                throw out_of_range("Pilihan tidak valid");
        }

        cout << "\n=== Inventaris Toko ===" << endl;
        cout << left << setw(4) << "No."
             << "| " << setw(20) << "Nama Barang"
             << "| " << setw(10) << "Stok"
             << "| " << setw(15) << "Harga" << endl;
        cout << string(58, '-') << endl;

        for (int i = 0; i < jumlahBarangGlobal; i++) {
            cout << left << setw(4) << i + 1
                 << "| " << setw(20) << inventarisGlobal[i].nama
                 << "| " << setw(10) << inventarisGlobal[i].stok
                 << "| Rp " << right << setw(11) << fixed << setprecision(2) << inventarisGlobal[i].harga << endl;
        }
    }
    catch (const runtime_error& e) {
        cout << "\n=== INFO: " << e.what() << " ===" << endl;
        cout << "\nTekan Enter untuk melanjutkan..." << endl;
        cin.get();
        clearScreen();
        return;
    }
    catch (const exception& e) {
        cout << "\n=== ERROR: " << e.what() << " ===" << endl;
        cout << "\nTekan Enter untuk melanjutkan..." << endl;
        cin.get();
        clearScreen();
        return;
    }
    cout << "\nTekan Enter untuk melanjutkan..." << endl;
    cin.get();
    clearScreen();
}

// Fungsi untuk mencari barang berdasarkan nama (Linear Search)
void cariBarang(LogSistem &logSistem, bool for_purchase = false, int &selected_index_ref = *(new int())) {
    try {
        if (jumlahBarangGlobal == 0) {
            throw runtime_error("Inventaris kosong.");
        }
        cout << "\nMasukkan nama barang yang dicari: ";
        string namaCari;
        getline(cin, namaCari);
        if (namaCari.empty()) {
            throw invalid_argument("Nama barang tidak boleh kosong.");
        }
        bool ditemukan = false;
        cout << "\n=== Hasil Pencarian ===" << endl;
        cout << left << setw(4) << "No."
             << "| " << setw(20) << "Nama Barang"
             << "| " << setw(10) << "Stok"
             << "| " << setw(15) << "Harga" << endl;
        cout << string(58, '-') << endl;
        int count = 0;
        int indices[MAX_BARANG]; // Menyimpan indeks asli barang di inventarisGlobal
        for (int i = 0; i < jumlahBarangGlobal; i++) {
            if (inventarisGlobal[i].nama.find(namaCari) != string::npos) {
                cout << left << setw(4) << ++count
                     << "| " << setw(20) << inventarisGlobal[i].nama
                     << "| " << setw(10) << inventarisGlobal[i].stok
                     << "| Rp " << right << setw(11) << fixed << setprecision(2) << inventarisGlobal[i].harga << endl;
                indices[count - 1] = i;
                ditemukan = true;
            }
        }
        if (!ditemukan) {
            tambahLog(logSistem, "Pencarian Gagal", "Barang " + namaCari + " tidak ditemukan");
            throw runtime_error("Barang dengan nama '" + namaCari + "' tidak ditemukan.");
        }
    }
    catch (const runtime_error& e) {
        cout << "\n=== INFO: " << e.what() << " ===" << endl;
    }
    catch (const exception &e) {
        cout << "\n=== ERROR: " << e.what() << " ===" << endl;
    }
    cout << "\nTekan Enter untuk melanjutkan..." << endl;
    cin.get();
    clearScreen();
}

// Fungsi untuk menambah barang
void tambahBarang(LogSistem &logSistem) {
    try {
        if (jumlahBarangGlobal >= MAX_BARANG) {
            tambahLog(logSistem, "Gagal Tambah Barang", "Inventaris penuh");
            throw runtime_error("Inventaris penuh!");
        }
        cout << "Masukkan nama barang: ";
        string namaBarang;
        getline(cin, namaBarang);

        if (namaBarang.empty()) {
            tambahLog(logSistem, "Gagal Tambah Barang", "Nama barang kosong");
            throw invalid_argument("Nama barang tidak boleh kosong!");
        }
        cout << "Masukkan stok barang: ";
        int stok;
        if (!(cin >> stok)) {
            cin.clear();
            while (cin.get() != '\n');
            tambahLog(logSistem, "Gagal Tambah Barang", "Input stok tidak valid untuk barang " + namaBarang);
            throw invalid_argument("Input stok tidak valid!");
        }
        if (stok <= 0) { // Perbaikan: memeriksa stok <= 0
            tambahLog(logSistem, "Gagal Tambah Barang", "Stok tidak valid untuk barang " + namaBarang);
            throw out_of_range("Stok tidak boleh nol atau negatif!");
        }        
        cout << "Masukkan harga barang (gunakan titik sebagai pemisah desimal, contoh: 40000.00 atau 5.5): Rp ";
        double harga;
        if (!(cin >> harga)) {
            cin.clear();
            while (cin.get() != '\n');
            tambahLog(logSistem, "Gagal Tambah Barang", "Input harga tidak valid untuk barang " + namaBarang);
            throw invalid_argument("Input harga tidak valid! Pastikan menggunakan titik sebagai pemisah desimal.");
        }
        if (harga <= 0) { // Perbaikan: memeriksa harga <= 0
            tambahLog(logSistem, "Gagal Tambah Barang", "Harga tidak valid untuk barang " + namaBarang);
            throw out_of_range("Harga tidak boleh nol atau negatif!");
        }
        while (cin.get() != '\n'); // buang sisa input
        // Jika semua valid, tambahkan barang
        inventarisGlobal[jumlahBarangGlobal].nama = namaBarang;
        inventarisGlobal[jumlahBarangGlobal].stok = stok;
        inventarisGlobal[jumlahBarangGlobal].harga = harga;
        jumlahBarangGlobal++;
        cout << "\n=== Barang berhasil ditambahkan ===" << endl;
        cout << "Nama: " << namaBarang << ", Stok: " << stok << ", Harga: Rp " << fixed << setprecision(2) << harga << endl;
        tambahLog(logSistem, "Tambah Barang", "Barang " + namaBarang + " ditambahkan dengan stok " + to_string(stok) + " dan harga Rp " + to_string(harga));
    }
    catch (const exception &e) {
        cout << "\n=== ERROR: " << e.what() << " ===" << endl;
    }
    cout << "\nTekan Enter untuk melanjutkan..." << endl;
    cin.get();
    clearScreen();
}

// Fungsi untuk mengupdate barang di inventaris global
void updateBarang(LogSistem &logSistem) {
    try {
        if (jumlahBarangGlobal == 0) {
            throw runtime_error("Inventaris kosong.");
        }
        tampilInventaris(); // Tampilkan inventaris agar user bisa memilih
        cout << "\nMasukkan nomor barang yang ingin diubah: ";
        int nomor_barang;
        if (!(cin >> nomor_barang) || nomor_barang < 1 || nomor_barang > jumlahBarangGlobal) {
            cin.clear();
            while (cin.get() != '\n');
            tambahLog(logSistem, "Gagal Update Barang", "Nomor barang tidak valid");
            throw invalid_argument("Nomor barang tidak valid.");
        }
        while (cin.get() != '\n'); // buang sisa input
        string namaLama = inventarisGlobal[nomor_barang - 1].nama;
        cout << "Masukkan nama baru barang (kosongkan jika tidak diubah): ";
        string namaBaru;
        getline(cin, namaBaru);
        if (namaBaru.empty()) {
            namaBaru = inventarisGlobal[nomor_barang - 1].nama; // Gunakan nama lama jika kosong
        }
        cout << "Masukkan stok baru barang (saat ini: " << inventarisGlobal[nomor_barang - 1].stok << ", kosongkan jika tidak diubah): ";
        int stokBaru;
        string stokInputStr;
        getline(cin, stokInputStr);
        if (stokInputStr.empty()) {
            stokBaru = inventarisGlobal[nomor_barang - 1].stok; // Gunakan stok lama jika kosong
        } else {
            try {
                stokBaru = stoi(stokInputStr); // Konversi string ke int
            } catch (const invalid_argument& ia) {
                tambahLog(logSistem, "Gagal Update Barang", "Input stok tidak valid untuk barang " + namaLama);
                throw invalid_argument("Input stok tidak valid!");
            } catch (const out_of_range& oor) {
                tambahLog(logSistem, "Gagal Update Barang", "Input stok terlalu besar/kecil untuk barang " + namaLama);
                throw out_of_range("Input stok terlalu besar/kecil!");
            }
        }
        if (stokBaru < 0) {
            tambahLog(logSistem, "Gagal Update Barang", "Stok tidak valid untuk barang " + namaLama);
            throw out_of_range("Stok tidak valid (negatif)!");
        }
        cout << "Masukkan harga baru barang (saat ini: Rp " << fixed << setprecision(2) << inventarisGlobal[nomor_barang - 1].harga << ", gunakan titik sebagai pemisah desimal, contoh: 40000.00 atau 5.5, kosongkan jika tidak diubah): Rp ";
        double hargaBaru;
        string hargaInputStr;
        getline(cin, hargaInputStr);
        if (hargaInputStr.empty()) {
            hargaBaru = inventarisGlobal[nomor_barang - 1].harga; // Gunakan harga lama jika kosong
        } else {
            try {
                size_t pos;
                hargaBaru = stod(hargaInputStr, &pos); 
                if (pos != hargaInputStr.length()) {
                    throw invalid_argument("Input harga tidak valid! Pastikan menggunakan titik sebagai pemisah desimal dan tidak ada karakter lain.");
                }
            } catch (const invalid_argument& ia) {
                 tambahLog(logSistem, "Gagal Update Barang", "Input harga tidak valid untuk barang " + namaLama);
                throw invalid_argument("Input harga tidak valid! Pastikan menggunakan titik sebagai pemisah desimal.");
            } catch (const out_of_range& oor) {
                 tambahLog(logSistem, "Gagal Update Barang", "Input harga terlalu besar/kecil untuk barang " + namaLama);
                throw out_of_range("Input harga terlalu besar/kecil!");
            }
        }
        if (hargaBaru < 0) {
            tambahLog(logSistem, "Gagal Update Barang", "Harga tidak valid untuk barang " + namaLama);
            throw out_of_range("Harga tidak valid (negatif)!");
        }
        // Jika semua valid, update barang
        inventarisGlobal[nomor_barang - 1].nama = namaBaru;
        inventarisGlobal[nomor_barang - 1].stok = stokBaru;
        inventarisGlobal[nomor_barang - 1].harga = hargaBaru;

        cout << "\n=== Barang berhasil diupdate ===" << endl;
        cout << "Nama: " << namaBaru << ", Stok: " << stokBaru << ", Harga: Rp " << fixed << setprecision(2) << hargaBaru << endl;
        tambahLog(logSistem, "Update Barang", "Barang " + namaLama + " diubah menjadi " + namaBaru + " dengan stok " + to_string(stokBaru) + " dan harga Rp " + to_string(hargaBaru));
    }
    catch (const exception& e) {
        cout << "\n=== ERROR: " << e.what() << " ===" << endl;
    }
    cout << "\nTekan Enter untuk melanjutkan..." << endl;
    cin.get();
    clearScreen();
}

// Fungsi untuk menghapus barang dari inventaris global
void hapusBarang(LogSistem &logSistem) {
    try {
        if (jumlahBarangGlobal == 0) {
            throw runtime_error("Inventaris kosong.");
        }        
        tampilInventaris(); // Tampilkan inventaris agar user bisa memilih
        cout << "\nMasukkan nomor barang yang ingin dihapus: ";
        int nomor_barang;
        if (!(cin >> nomor_barang) || nomor_barang < 1 || nomor_barang > jumlahBarangGlobal) {
            cin.clear();
            while (cin.get() != '\n');
            tambahLog(logSistem, "Gagal Hapus Barang", "Nomor barang tidak valid");
            throw invalid_argument("Nomor barang tidak valid.");
        }
        while (cin.get() != '\n'); // buang sisa input
        string namaBarang = inventarisGlobal[nomor_barang - 1].nama;
        
        // Geser array agar item terhapus
        for (int i = nomor_barang - 1; i < jumlahBarangGlobal - 1; i++) {
            inventarisGlobal[i] = inventarisGlobal[i + 1];
        }
        jumlahBarangGlobal--;
        cout << "\n=== Barang berhasil dihapus ===" << endl;
        cout << "Nama: " << namaBarang << endl;
        tambahLog(logSistem, "Hapus Barang", "Barang " + namaBarang + " dihapus");
    }
    catch (const exception& e) {
        cout << "\n=== ERROR: " << e.what() << " ===" << endl;
    }
    cout << "\nTekan Enter untuk melanjutkan..." << endl;
    cin.get();
    clearScreen();
}

// Fungsi untuk pembelian barang (user only)
void pembelianBarang(Pengguna &pengguna, LogSistem &logSistem) {
    try {
        if (jumlahBarangGlobal == 0) {
            throw runtime_error("Inventaris toko kosong.");
        }        
        tampilInventaris(); // Tampilkan inventaris agar user bisa memilih
        cout << "\nMasukkan nomor barang yang ingin dibeli (0 untuk batal): ";
        int nomor_barang;
        if (!(cin >> nomor_barang) || nomor_barang < 0 || nomor_barang > jumlahBarangGlobal) {
            cin.clear();
            while (cin.get() != '\n');
            tambahLog(logSistem, "Gagal Pembelian", "Nomor barang tidak valid: " + to_string(nomor_barang));
            throw invalid_argument("Nomor barang tidak valid.");
        }
        if (nomor_barang == 0) {
            cout << "\n=== Pembelian dibatalkan ===" << endl;
            tambahLog(logSistem, "Batal Pembelian", "Pengguna " + pengguna.akun.nama + " membatalkan pembelian");
            cout << "\nTekan Enter untuk melanjutkan..." << endl;
            cin.get();
            clearScreen();
            return;
        }
        nomor_barang--; // Ubah ke indeks 0-based
        string namaBarang = inventarisGlobal[nomor_barang].nama;
        int stokTersedia = inventarisGlobal[nomor_barang].stok;
        double hargaBarang = inventarisGlobal[nomor_barang].harga;
        cout << fixed << setprecision(2);
        cout << "\nBarang: " << namaBarang << ", Stok tersedia: " << stokTersedia
             << ", Harga per unit: Rp " << hargaBarang << endl;
        cout << "Masukkan jumlah yang ingin dibeli: ";
        int jumlah;
        if (!(cin >> jumlah) || jumlah <= 0 || jumlah > stokTersedia) {
            cin.clear();
            while (cin.get() != '\n');
            tambahLog(logSistem, "Gagal Pembelian", "Jumlah tidak valid atau stok tidak cukup untuk barang " + namaBarang);
            throw invalid_argument("Jumlah tidak valid atau stok tidak cukup.");
        }
        double totalHarga = jumlah * hargaBarang;
        cout << "\nKonfirmasi pembelian: " << jumlah << " " << namaBarang
             << " dengan total harga Rp " << totalHarga << "? (y/n): ";
        char konfirmasi;
        cin >> konfirmasi;
        while (cin.get() != '\n'); // Buang sisa input
        if (konfirmasi == 'y' || konfirmasi == 'Y') {
            inventarisGlobal[nomor_barang].stok -= jumlah;
            if (pengguna.jumlahPembelian < MAX_LOG) {
                pengguna.riwayatPembelian[pengguna.jumlahPembelian].namaBarang = namaBarang;
                pengguna.riwayatPembelian[pengguna.jumlahPembelian].jumlah = jumlah;
                pengguna.riwayatPembelian[pengguna.jumlahPembelian].waktu = getWaktuSekarang();
                pengguna.riwayatPembelian[pengguna.jumlahPembelian].hargaTotal = totalHarga;
                pengguna.jumlahPembelian++;
            }
            cout << "\n=== Pembelian berhasil ===" << endl;
            cout << "Anda membeli " << jumlah << " " << namaBarang
                 << " dengan total harga Rp " << totalHarga << "." << endl;
            tambahLog(logSistem, "Pembelian", "Pengguna " + pengguna.akun.nama + " membeli " + to_string(jumlah) + " " + namaBarang + " dengan total Rp " + to_string(totalHarga));
        } else {
            cout << "\n=== Pembelian dibatalkan ===" << endl;
            tambahLog(logSistem, "Batal Pembelian", "Pengguna " + pengguna.akun.nama + " membatalkan pembelian " + namaBarang);
        }
    }
    catch (const exception& e) {
        cout << "\n=== ERROR: " << e.what() << " ===" << endl;
    }
    cout << "\nTekan Enter untuk melanjutkan..." << endl;
    cin.get();
    clearScreen();
}

// Fungsi untuk menampilkan riwayat pembelian (user only)
void tampilRiwayatPembelian(Pengguna &pengguna) {
    try {
        if (pengguna.jumlahPembelian == 0) {
            throw runtime_error("Belum ada riwayat pembelian.");
        }
        cout << "\n=== Riwayat Pembelian ===" << endl;
        cout << left << setw(20) << "Waktu"
             << "| " << setw(20) << "Nama Barang"
             << "| " << setw(10) << "Jumlah"
             << "| " << setw(15) << "Total Harga" << endl;
        cout << string(70, '-') << endl;
        for (int i = 0; i < pengguna.jumlahPembelian; i++) {
            cout << left << setw(20) << pengguna.riwayatPembelian[i].waktu
                 << "| " << setw(20) << pengguna.riwayatPembelian[i].namaBarang
                 << "| " << setw(10) << pengguna.riwayatPembelian[i].jumlah
                 << "| Rp " << right << setw(11) << fixed << setprecision(2) << pengguna.riwayatPembelian[i].hargaTotal << endl;
        }
    }
    catch (const exception& e) {
        cout << "\n=== INFO: " << e.what() << " ===" << endl;
    }
    cout << "\nTekan Enter untuk melanjutkan..." << endl;
    cin.get();
    clearScreen();
}

// Fungsi untuk login pengguna
int login(Pengguna pengguna[], int jumlahPengguna, string &peran) {
    int percobaan_login = 0;
    int index_pengguna = -1;
    string nama_input, pw_input;
    while (percobaan_login < 3) {
        try {
            cout << "Masukkan Nama: ";
            getline(cin, nama_input);            
            if (nama_input.length() > 15) {
                throw runtime_error("Jangan masukkan lebih dari 15 karakter");
            }            
            cout << "Masukkan Password: ";
            getline(cin, pw_input);

            bool login_berhasil = false;
            for (int i = 0; i < jumlahPengguna; i++) {
                if (pengguna[i].akun.nama == nama_input && pengguna[i].akun.pw == pw_input) {
                    login_berhasil = true;
                    index_pengguna = i;
                    peran = pengguna[i].akun.peran;
                    break;
                }
            }
            if (login_berhasil) {
                cout << "\n=== Login berhasil! ===" << endl;
                cout << "Selamat datang, " << pengguna[index_pengguna].akun.nama << " (" << peran << ")!" << endl;
                break;
            } else {
                percobaan_login++;
                if (percobaan_login < 3) {
                    throw runtime_error("Login gagal! Akun tidak ditemukan. Sisa percobaan: " + to_string(3 - percobaan_login));
                } else {
                    throw runtime_error("Login gagal 3 kali. Program keluar.");
                }
            }
        }
        catch (const runtime_error &e) {
            cout << "\n=== ERROR: " << e.what() << " ===" << endl;
            if (percobaan_login == 3) {
                return -2; // Kode khusus untuk keluar program
            }
        }
    }    
    cout << "\nTekan Enter untuk melanjutkan..." << endl;
    cin.get();
    clearScreen();
    return index_pengguna;
}

// menambah admin baru
void tambahAdminBaru(Pengguna pengguna[], int &jumlahPengguna, LogSistem &logSistem) {
    string username, password;
    try {
        if (jumlahPengguna >= MAX_PENGGUNA) {
            throw runtime_error("Kapasitas akun maksimum telah tercapai.");
        }
         string nama_input, pw_input;
        cout << "Masukkan Nama (maksimal 15 karakter): ";
        getline(cin, nama_input);
        
        if (nama_input.length() > 15) {
            throw invalid_argument("Nama pengguna tidak boleh lebih dari 15 karakter.");
        }        
        cout << "Masukkan Password: ";
        getline(cin, pw_input);
        if (nama_input.empty() || pw_input.empty()) {
            throw invalid_argument("Nama dan Password tidak boleh kosong.");
        }
        for (int i = 0; i < jumlahPengguna; i++) {
            if (pengguna[i].akun.nama == nama_input) {
                throw runtime_error("Nama pengguna sudah terdaftar. Gunakan nama lain.");
            }
        }        
        string peran_input = "admin";
        pengguna[jumlahPengguna].akun.nama = nama_input;
        pengguna[jumlahPengguna].akun.pw = pw_input;
        pengguna[jumlahPengguna].akun.peran = peran_input;
        pengguna[jumlahPengguna].jumlahPembelian = 0;
        jumlahPengguna++;
        cout << "\n=== Akun berhasil didaftarkan ===" << endl;
        cout << "Nama: " << nama_input << ", Peran: " << peran_input << endl;
        tambahLog(logSistem, "Registrasi", "Pengguna " + nama_input + " didaftarkan sebagai " + peran_input);
    }
    catch (const exception &e) {
        cout << "\n=== ERROR: " << e.what() << " ===" << endl;
        tambahLog(logSistem, "Gagal Registrasi", e.what());
    }
    cout << "\nTekan Enter untuk melanjutkan..." << endl;
    cin.get();
    clearScreen();
}

// Fungsi untuk registrasi pengguna baru
void registrasi(Pengguna pengguna[], int &jumlahPengguna, LogSistem &logSistem) {
    try {
        if (jumlahPengguna >= MAX_PENGGUNA) {
            throw runtime_error("Batas maksimum pengguna tercapai.");
        }
        string nama_input, pw_input;
        cout << "Masukkan Nama (maksimal 15 karakter): ";
        getline(cin, nama_input);
        
        if (nama_input.length() > 15) {
            throw invalid_argument("Nama pengguna tidak boleh lebih dari 15 karakter.");
        }        
        cout << "Masukkan Password: ";
        getline(cin, pw_input);
        if (nama_input.empty() || pw_input.empty()) {
            throw invalid_argument("Nama dan Password tidak boleh kosong.");
        }
        for (int i = 0; i < jumlahPengguna; i++) {
            if (pengguna[i].akun.nama == nama_input) {
                throw runtime_error("Nama pengguna sudah terdaftar. Gunakan nama lain.");
            }
        }        
        string peran_input = "user";
        pengguna[jumlahPengguna].akun.nama = nama_input;
        pengguna[jumlahPengguna].akun.pw = pw_input;
        pengguna[jumlahPengguna].akun.peran = peran_input;
        pengguna[jumlahPengguna].jumlahPembelian = 0;
        jumlahPengguna++;
        cout << "\n=== Akun berhasil didaftarkan ===" << endl;
        cout << "Nama: " << nama_input << ", Peran: " << peran_input << endl;
        tambahLog(logSistem, "Registrasi", "Pengguna " + nama_input + " didaftarkan sebagai " + peran_input);
    }
    catch (const exception &e) {
        cout << "\n=== ERROR: " << e.what() << " ===" << endl;
        tambahLog(logSistem, "Gagal Registrasi", e.what());
    }
    cout << "\nTekan Enter untuk melanjutkan..." << endl;
    cin.get();
    clearScreen();
}

// Fungsi untuk menampilkan riwayat aktivitas (admin only)
void tampilRiwayatAktivitas(LogSistem &logSistem) {
    try {
        if (logSistem.jumlahLog == 0) {
            throw runtime_error("Belum ada aktivitas yang tercatat.");
        }
        cout << "\n=== Riwayat Aktivitas ===" << endl;
        cout << left << setw(20) << "Waktu"
             << "| " << setw(20) << "Aksi"
             << "| " << "Detail" << endl;
        cout << string(70, '-') << endl;
        for (int i = 0; i < logSistem.jumlahLog; i++) {
            cout << left << setw(20) << logSistem.logs[i].waktu
                 << "| " << setw(20) << logSistem.logs[i].aksi
                 << "| " << logSistem.logs[i].detail << endl;
        }
    }
    catch (const exception &e) {
        cout << "\n=== ERROR: " << e.what() << " ===" << endl;
    }
    cout << "\nTekan Enter untuk melanjutkan..." << endl;
    cin.get();
    clearScreen();
}

// Fungsi untuk kelola pengguna (admin only)
void kelolaPengguna(Pengguna pengguna[], int &jumlahPengguna, LogSistem &logSistem, int index_pengguna_aktif) {
    cout << "Daftar Pengguna:" << endl;
    cout << "No. | Nama             | Password  | Peran" << endl;
    cout << "--------------------------------------------" << endl;
    for (int i = 0; i < jumlahPengguna; i++) {
        cout << i + 1 << ".  | "
             << pengguna[i].akun.nama;
        for (int j = pengguna[i].akun.nama.length(); j < 17; j++) cout << " ";
        cout << "| " << pengguna[i].akun.pw;
        for (int j = pengguna[i].akun.pw.length(); j < 10; j++) cout << " ";
        cout << "| " << pengguna[i].akun.peran << endl;
    }

    cout << "\nMasukkan nomor pengguna yang ingin dihapus (0 untuk batal): ";
    int nomor_pengguna;
    if (!(cin >> nomor_pengguna) || nomor_pengguna < 0 || nomor_pengguna > jumlahPengguna) {
        cout << "Input tidak valid." << endl;
        cin.clear();
        while (cin.get() != '\n');
        tambahLog(logSistem, "Gagal Hapus Pengguna", "Nomor pengguna tidak valid");
    } else if (nomor_pengguna == 0) {
        while (cin.get() != '\n');
        cout << "Operasi dibatalkan." << endl;
    } else if (nomor_pengguna - 1 == index_pengguna_aktif) {
        cout << "Tidak dapat menghapus akun Anda sendiri." << endl;
        tambahLog(logSistem, "Gagal Hapus Pengguna", "Percobaan menghapus akun sendiri: " + pengguna[index_pengguna_aktif].akun.nama);
        while (cin.get() != '\n');
    } else {
        while (cin.get() != '\n');
        string namaPengguna = pengguna[nomor_pengguna - 1].akun.nama;
        string peranPengguna = pengguna[nomor_pengguna - 1].akun.peran;
        for (int i = nomor_pengguna - 1; i < jumlahPengguna - 1; i++) {
            pengguna[i] = pengguna[i + 1];
        }
        jumlahPengguna--;
        cout << "Pengguna berhasil dihapus." << endl;
        tambahLog(logSistem, "Hapus Pengguna", "Pengguna " + namaPengguna + " (" + peranPengguna + ") dihapus");
    }
    cout << "\nTekan Enter untuk melanjutkan..." << endl;
    cin.get();
    clearScreen();
}

// Fungsi untuk logout
void logout(Pengguna &pengguna, LogSistem &logSistem) {
    cout << "\n=== Logout berhasil ===" << endl;
    cout << "Pengguna: " << pengguna.akun.nama << endl;
    tambahLog(logSistem, "Logout", "Pengguna " + pengguna.akun.nama + " keluar dari sistem");
    cout << "\nTekan Enter untuk kembali ke menu utama..." << endl;
    cin.get();
    clearScreen();
}

// Fungsi untuk keluar dari program
void keluarProgram() {
    cout << "\n=== Terima kasih telah menggunakan program ini ===" << endl;
    cout << "\nTekan Enter untuk keluar..." << endl;
    cin.get();
    clearScreen();
}

// Fungsi utama
int main() {
    Pengguna pengguna[MAX_PENGGUNA];
    int jumlahPengguna = 0;
    LogSistem logSistem = {{}, 0};
    int pilihan_menu;

    cout.imbue(locale("C"));
    cin.imbue(locale("C"));

    // --- Data Barang Bawaan (Barang Bangunan - 10 item) ---
    inventarisGlobal[jumlahBarangGlobal++] = {"Semen", 100, 40000.0};
    inventarisGlobal[jumlahBarangGlobal++] = {"Batu Bata", 5000, 500.0};
    inventarisGlobal[jumlahBarangGlobal++] = {"Besi Beton", 200, 150000.0};
    inventarisGlobal[jumlahBarangGlobal++] = {"Pasir Kubik", 100, 500000.0};
    inventarisGlobal[jumlahBarangGlobal++] = {"Cat Tembok", 50, 80000.0};
    inventarisGlobal[jumlahBarangGlobal++] = {"Keramik (40x40)", 100, 120000.0};
    inventarisGlobal[jumlahBarangGlobal++] = {"Pipa", 50, 25000.0};
    inventarisGlobal[jumlahBarangGlobal++] = {"Kayu Balok", 100, 30000.0};
    inventarisGlobal[jumlahBarangGlobal++] = {"Triplek", 50, 60000.0};
    inventarisGlobal[jumlahBarangGlobal++] = {"Atap Seng", 100, 45000.0};

    // Akun default admin
    pengguna[jumlahPengguna].akun.nama = "admin";
    pengguna[jumlahPengguna].akun.pw = "123";
    pengguna[jumlahPengguna].akun.peran = "admin";
    pengguna[jumlahPengguna].jumlahPembelian = 0;
    jumlahPengguna++;

    // Akun default user
    pengguna[jumlahPengguna].akun.nama = "user";
    pengguna[jumlahPengguna].akun.pw = "123";
    pengguna[jumlahPengguna].akun.peran = "user";
    pengguna[jumlahPengguna].jumlahPembelian = 0;
    jumlahPengguna++;

    do {
        cout << R"(
   _____ ______ _               __  __       _______   _____       _______       _   _  _____ 
  / ____|  ____| |        /\   |  \/  |   /\|__   __| |  __ \   /\|__   __|/\   | \ | |/ ____|
 | (___ | |__  | |       /  \  | \  / |  /  \  | |    | |  | | /  \  | |  /  \  |  \| | |  __ 
  \___ \|  __| | |      / /\ \ | |\/| | / /\ \ | |    | |  | |/ /\ \ | | / /\ \ | . ` | | |_ |
  ____) | |____| |____ / ____ \| |  | |/ ____ \| |    | |__| / ____ \| |/ ____ \| |\  | |__| |
 |_____/|______|______/_/    \_\_|  |_/_/    \_\_|    |_____/_/    \_\_/_/    \_\_| \_|\_____|
)" << endl;
        cout << R"(

___  ___                  _                             _____                     _             _      
|  \/  |                 (_)                           |_   _|                   | |           (_)     
| .  . | __ _ _ __   __ _ _  ___ _ __ ___   ___ _ __     | | _ ____   _____ _ __ | |_ __ _ _ __ _ ___  
| |\/| |/ _` | '_ \ / _` | |/ _ \ '_ ` _ \ / _ \ '_ \    | || '_ \ \ / / _ \ '_ \| __/ _` | '__| / __| 
| |  | | (_| | | | | (_| | |  __/ | | | | |  __/ | | |  _| || | | \ V /  __/ | | | || (_| | |  | \__ \ 
\_|  |_/\__,_|_| |_|\__,_| |\___|_| |_| |_|\___|_| |_|  \___/_| |_|\_/ \___|_| |_|\__\__,_|_|  |_|___/ 
                        _/ |                                                                           
                       |__/                                                                            
 _____     _          ______                                             _    _       _ _     _        
|_   _|   | |         | ___ \                                           | |  | |     | (_)   | |       
  | | ___ | | _____   | |_/ / __ _ _ __   __ _ _   _ _ __   __ _ _ __   | |  | | __ _| |_  __| |       
  | |/ _ \| |/ / _ \  | ___ \/ _` | '_ \ / _` | | | | '_ \ / _` | '_ \  | |/\| |/ _` | | |/ _` |       
  | | (_) |   < (_) | | |_/ / (_| | | | | (_| | |_| | | | | (_| | | | | \  /\  / (_| | | | (_| |       
  \_/\___/|_|\_\___/  \____/ \__,_|_| |_|\__, |\__,_|_| |_|\__,_|_| |_|  \/  \/ \__,_|_|_|\__,_|       
                                          __/ |                                                        
                                         |___/                                                         
)" << endl;
        cout << "\n=== Silahkan Pilih Menu ===" << endl;
        cout << "1. Registrasi" << endl;
        cout << "2. Login" << endl;
        cout << "3. Keluar" << endl;
        cout << "Pilihan: ";

        if (!(cin >> pilihan_menu)) {
            cout << "\n=== ERROR: Input tidak valid! ===" << endl;
            cout << "Masukkan angka." << endl;
            cin.clear();
            while (cin.get() != '\n');
            cout << "\nTekan Enter untuk melanjutkan..." << endl;
            cin.get();
            clearScreen();
            continue;
        }
        while (cin.get() != '\n');

        switch (pilihan_menu) {
            case 1:
                registrasi(pengguna, jumlahPengguna, logSistem);
                break;
            case 2: {
                string peran;
                int index_pengguna = login(pengguna, jumlahPengguna, peran);
                if (index_pengguna == -2) {
                    cout << "\n=== Program keluar ===" << endl;
                    return 0;
                }
                if (index_pengguna != -1) {
                    if (peran == "admin") {
                        int pilihan_admin;
                        do {
                            cout << "\n=== Menu Manajemen Inventaris Admin ===" << endl;
                            cout << "Selamat datang, " << pengguna[index_pengguna].akun.nama << " (" << peran << ")!" << endl;
                            cout << "1. Tampilkan Inventaris" << endl;
                            cout << "2. Tambah Barang" << endl;
                            cout << "3. Update Barang" << endl;
                            cout << "4. Hapus Barang" << endl;
                            cout << "5. Riwayat Aktivitas" << endl;
                            cout << "6. Kelola Pengguna" << endl;
                            cout << "7. Cari Barang" << endl;
                            cout << "8. Daftarkan admin lain" << endl;
                            cout << "9. Logout" << endl;
                            cout << "Pilihan: ";

                            if (!(cin >> pilihan_admin)) {
                                cout << "\n=== ERROR: Input tidak valid! ===" << endl;
                                cout << "Masukkan angka." << endl;
                                cin.clear();
                                while (cin.get() != '\n');
                                cout << "\nTekan Enter untuk melanjutkan..." << endl;
                                cin.get();
                                clearScreen();
                                continue;
                            }
                            while (cin.get() != '\n');

                            switch (pilihan_admin) {
                                case 1: tampilInventaris(); break;
                                case 2: tambahBarang(logSistem); break;
                                case 3: updateBarang(logSistem); break;
                                case 4: hapusBarang(logSistem); break;
                                case 5: tampilRiwayatAktivitas(logSistem); break;
                                case 6: kelolaPengguna(pengguna, jumlahPengguna, logSistem, index_pengguna); break;
                                case 7: {
                                    int selected_index_dummy = -1;
                                    cariBarang(logSistem, false, selected_index_dummy);
                                    break;
                                }
                                case 8: tambahAdminBaru(pengguna, jumlahPengguna, logSistem); break;
                                case 9: logout(pengguna[index_pengguna], logSistem); break;
                                default:
                                    cout << "\n=== ERROR: Pilihan tidak valid! ===" << endl;
                                    cout << "\nTekan Enter untuk melanjutkan..." << endl;
                                    cin.get();
                                    clearScreen();
                            }
                        } while (pilihan_admin != 9);
                    } else if (peran == "user") {
                        int pilihan_user;
                        do {
                            cout << "\n=== Menu Pengguna ===" << endl;
                            cout << "Selamat datang, " << pengguna[index_pengguna].akun.nama << " (" << peran << ")!" << endl;
                            cout << "1. Tampilkan Inventaris" << endl;
                            cout << "2. Beli Barang" << endl;
                            cout << "3. Riwayat Pembelian" << endl;
                            cout << "4. Cari Barang" << endl;
                            cout << "5. Logout" << endl;
                            cout << "Pilihan: ";

                            if (!(cin >> pilihan_user)) {
                                cout << "\n=== ERROR: Input tidak valid! ===" << endl;
                                cout << "Masukkan angka." << endl;
                                cin.clear();
                                while (cin.get() != '\n');
                                cout << "\nTekan Enter untuk melanjutkan..." << endl;
                                cin.get();
                                clearScreen();
                                continue;
                            }
                            while (cin.get() != '\n');

                            switch (pilihan_user) {
                                case 1: tampilInventaris(); break;
                                case 2: pembelianBarang(pengguna[index_pengguna], logSistem); break;
                                case 3: tampilRiwayatPembelian(pengguna[index_pengguna]); break;
                                case 4: {
                                    int selected_index_dummy = -1;
                                    cariBarang(logSistem, false, selected_index_dummy);
                                    break;
                                }
                                case 5: logout(pengguna[index_pengguna], logSistem); break;
                                default:
                                    cout << "\n=== ERROR: Pilihan tidak valid! ===" << endl;
                                    cout << "\nTekan Enter untuk melanjutkan..." << endl;
                                    cin.get();
                                    clearScreen();
                            }
                        } while (pilihan_user != 5);
                    }
                }
                break;
            }
            case 3:
                keluarProgram();
                break;
            default:
                cout << "\n=== ERROR: Pilihan tidak valid! ===" << endl;
                cout << "\nTekan Enter untuk melanjutkan..." << endl;
                cin.get();
                clearScreen();
        }
    } while (pilihan_menu != 3);

    return 0;
}