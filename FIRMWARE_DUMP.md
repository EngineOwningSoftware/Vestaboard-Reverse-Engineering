# How to dump the firmware (Draft)
## Procedure with HW access

1. Remove the RPi Compute Module (CM3+, 8GB) from the Vestaboard (Use some alcohol to remove the tamper sticker without triggering it)
2. Use a CM3+ to supply power to the module and to mount the filesystem
3. Create a raw dump using dd (`dd if=/dev/sdb of=vestaboard_fs.bin bs=16M`)

## Procedure with ssh access

1. Connect via SSH as root (Use mentioned private key from the README.md)
2. Dump each parition using dd and save it somewhere as a backup

## Further analysis
I've cloned the firmware onto my own CM3+ so I don't have to work on the original product, and I would suggest you do the same,
just get yourself an CM3+ with 8GB eMMC (more should prob. work too), write the vestaboard_fs.bin onto it and plug that into the board.

You can also mount the filesystem in linux and modify it before putting it back in, your imagination is your limit, add your own SSH keys,
startup scripts etc
