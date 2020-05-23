particle compile xenon
particle usb dfu
sleep 2
particle flash --usb xenon*.bin
rm xenon*.bin