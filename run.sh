#!/system/bin/sh
# Checking ID shell
if [ "$(id -u)" -ne 2000 ]; then
    echo "[ Error |@UnixeID(Yeye)]"
    exit 1
fi
# Smart Notification
shell() {
    sor="$1"
    cmd notification post -S bigtext -t '♨️ AI Scaling' 'Tag' "$sor" > /dev/null 2>&1
}
# Style display Terminal
    echo
    echo "     ☆================================☆"
    echo
    echo "       ~ Description AI Scaling.... "
    echo
    echo "       - Author                 :  @UnixeID"
    echo "       - Version                :  1.0"
    echo "       - Release               :  01 - Mar - 2025"
    echo "       - Name Shell         :  AI Scaling"
    echo
    echo "    |_______________________________________|"
    echo "    \______________________________________/"
    echo
    echo "   Priority scaling for game "
    sleep 2
    echo
    echo
        # Ambil daftar paket
        reso=$(cat /sdcard/scaling/scaling.txt)
        package_list=$(pm list package | cut -f2 -d: | tr -d '\r' | xargs -n1)
        control=1
       found_game=0  # Menandai apakah ada game yang ditemukan
    while IFS= read -r gamelist || [ -n "$gamelist" ]; do
         line=$(echo "$gamelist" | tr -d '\r' | xargs)    
          if [ -n "$line" ]; then
        if printf "%s\n" "$package_list" | grep -xq "$line"; then
            printf "  %d. %s\n" "$control" "$line"
            control=$((control + 1))
            found_game=1
           fi
        fi
            done < /sdcard/scaling/gamelist.txt
# Hanya tampilkan target resolution jika ada game yang ditemukan
# Hentikan proses logcat yang berjalan di background
pkill -f logcat
   # Variabel untuk mengontrol instalasi dan penghapusan script
if [ "$1" = "kill" ]; then
    if pgrep -f vm > /dev/null 2>&1; then
        printf "\n  Program is stopped in the backgurond "     
        rm /data/local/tmp/* > /dev/null 2>&1
        pkill -f vm > /dev/null 2>&1
        shell "  Program is stopped in the background\n"
        pkill -f sh > /dev/null 2>&1       
    else
        printf "Program failed to stop!\n"
    fi
else
    if ! pgrep -f vm > /dev/null 2>&1; then
        cp /sdcard/scaling/vm /data/local/tmp
        chmod +x /data/local/tmp/vm
        nohup /data/local/tmp/vm > /dev/null 2>&1 &
      fi 
    sleep 2
    if pgrep -f vm > /dev/null 2>&1; then
        printf "Program is running in the background\n"
    else
        printf "Program failed to run!\n"
    fi
fi