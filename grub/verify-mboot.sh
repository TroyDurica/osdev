if grub-file --is-x86-multiboot TroyOS.iso; then
    echo [INFO] ISO Multiboot Confirmed
else
    echo [WARN] ISO Not Multiboot
fi