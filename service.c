/*
            [ 📮 DCUMENTASI SCRIPT 📮 ]
       
Fungsi script C :
 - Mengganti atau mengubah resolution device dan beberapa kode CMD bawaan device, untuk meningkatkan performance device di saat digunakan bermain game and dayli 
 
Cara kerja script C :
- Script akan medeteksi PID aplikasi yang sudah dilist di file gamelist.txt, kalo ada salah satu aplilasi yang running maka script secara otomatis akan meningkatkan performance device untuk mendapatkan pengamalan bermain game yang lebih baik sebalik nya jika tidak ada aplikasi dari llist gamelist.txt yang terdeteksi running maka script akan menurunkan performnace nya untuk menghemat battery dan panas device dill

Copyright 2025 @UnixeID

⚠️HARD WARMING USER :
- PENGGUNA DILARANG MENGANTI ATAU MEMODIFOKASI TANPA IZIN TERTULIS DEVELOPER
- PANGGUNA JUGA HARUS MENYERTAKAN CREADIT KALO INGGIN MENGGUAKAN KEMBALI SCRIPT INI, JIKA CERADIT TIDAK DI SECRTAKAN DEVELOPER TIDAK AKAN BERTANGGUNG JAWAB ATAS MSALAH SCRIPT YANG TIBA-TIBA EROR 
- PENGGUNA JUGA WAJIB MENYERTAKAN DOCUMENTASI PERUBAHAN SCRIP

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

// Fungsi untuk mengirim notifikasi menggunakan NFC (Android shell)
void NFC(const char *message) {
    char notifie[2048]; // Buffer lebih besar untuk menghindari overflow
    snprintf(notifie, sizeof(notifie),
        "CPU=$(dumpsys hardware_properties | grep 'CPU temperatures' | awk -F'[][]' '{print $2}') && "
        "BATTERY=$(dumpsys hardware_properties | grep 'Battery temperatures' | awk -F'[][]' '{print $2}') && "
        "cmd notification post -S bigtext "
        "-t \"[ Battery : ${BATTERY}°C | Chipset : ${CPU}°C ]\" \"Tag\" \"%s\" > /dev/null 2>&1",
        message);
        
    system(notifie);
}

// Fungsi untuk mengatur mode performa tinggi
void adjust_ionice_high(const char *pid) {
    char command[512];
    
    // Priority Background 
    snprintf(command, sizeof(command), "chrt -f -p 99 %s", pid);
    system(command);
    
    // Priority Input and Output
    snprintf(command, sizeof(command), "ionice -c 1 -n 0 -p %s", pid);
    system(command);
    
    // Priority CPU Changer
    snprintf(command, sizeof(command), "renice -n -20 -p %s", pid);
    system(command);
    
    snprintf(command, sizeof(command),
             "target_width=540; "
             "cmd window size \"$target_width\"x\"$(printf %%0.f \"$(echo \"$(echo \"$(cmd window size|cut -f3 -d ' '|head -n 1)\"|cut -d'x' -f2)\"*\"$(echo \"$target_width\"/\"$(echo \"$(cmd window size|cut -f3 -d ' '|head -n 1)\"|cut -d'x' -f1)\"|bc -l)\"|bc)\")\"; "
             "cmd window density \"$(echo \"$(cmd window density|cut -f3 -d ' '|head -n 1)\"*\"$target_width\"/\"$(echo \"$(cmd window size|cut -f3 -d ' '|head -n 1)\"|cut -d'x' -f1)\"|bc)\"");
    system(command);
   
    // idle Mode or deepsleep
    system("cmd activity fgs-notification-rate-limit disable");
    system("cmd power set-fixed-performance-mode-enabled true");
    system("cmd activity memory-factor set CRITICAL");
    system("dumpsys deviceidle enable");
    system("dumpsys deviceidle force-idle");
    snprintf(command, sizeof(command), "cmd deviceidle whitelist +%s", pid);
    system(command);
}

// Fungsi untuk mengatur mode hemat daya
void adjust_ionice_low(const char *pid) {
    char command[512];

    // Priority Background
    snprintf(command, sizeof(command), "chrt -f -p 25 %s", pid);
    system(command);
    
    // smart idle mode Changer
    system("cmd activity fgs-notification-rate-limit enable");
    system("cmd power set-fixed-performance-mode-enabled true");
    system("cmd activity memory-factor set LOW");
    system("dumpsys deviceidle disable");
    system("dumpsys deviceidle unforce");
    
    // smart resolution Changer (default ke 720p)
    snprintf(command, sizeof(command),
             "target_width=1080; "
             "cmd window size \"$target_width\"x\"$(printf %%0.f \"$(echo \"$(echo \"$(cmd window size|cut -f3 -d ' '|head -n 1)\"|cut -d'x' -f2)\"*\"$(echo \"$target_width\"/\"$(echo \"$(cmd window size|cut -f3 -d ' '|head -n 1)\"|cut -d'x' -f1)\"|bc -l)\"|bc)\")\"; "
             "cmd window density \"$(echo \"$(cmd window density|cut -f3 -d ' '|head -n 1)\"*\"$target_width\"/\"$(echo \"$(cmd window size|cut -f3 -d ' '|head -n 1)\"|cut -d'x' -f1)\"|bc)\"");
    system(command);
}

// Fungsi untuk mengecek apakah aplikasi sedang berjalan
int check_app_running(const char *app_name, char *pid) {
    char command[512];
    snprintf(command, sizeof(command), "pgrep -f %s", app_name);

    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        return 0;
    }

    if (fgets(pid, 16, fp) != NULL) {
        pid[strcspn(pid, "\n")] = '\0';  // Hapus karakter newline dari PID
        pclose(fp);
        return 1;
    }

    pclose(fp);
    return 0;
}

int main() {
    NFC("♨️ Priority is running in the background");
    sleep(1);
    NFC("♨️ AI Scaling for game: By @UnixeID");
    sleep(1);

    const char *primary_path = "/sdcard/scaling/gamelist.txt";
    char line[1024];
    char app_in_running[1024] = "";
    char current_pid[16] = "";

    while (1) {
        FILE *file = fopen(primary_path, "r");

        if (file == NULL) {
            static int error_count = 0;
            if (error_count == 0) {
                printf("File gamelist.txt tidak ditemukan!\n");
            }
            error_count++;
            sleep(2);
            continue;
        }

        int app_found = 0;
        char new_pid[16] = "";

        while (fgets(line, sizeof(line), file) != NULL) {
            line[strcspn(line, "\n")] = '\0';

            if (check_app_running(line, new_pid)) {
                app_found = 1;

                if (strcmp(line, app_in_running) != 0 || strcmp(new_pid, current_pid) != 0) {
                    snprintf(app_in_running, sizeof(app_in_running), "%s", line);
                    snprintf(current_pid, sizeof(current_pid), "%s", new_pid);

                    adjust_ionice_high(current_pid);
                    char message[512];
                    snprintf(message, sizeof(message), "[ Mode High For game : %s ]", line);
                    NFC(message);
                }
                break;
            }
        }
        fclose(file);

        if (!app_found && strlen(app_in_running) > 0) {
            NFC("[ Mode Low : game closed ]");
            adjust_ionice_low(current_pid);
            app_in_running[0] = '\0';
            current_pid[0] = '\0';
        }

        sleep(2);
    }

    return 0;
}