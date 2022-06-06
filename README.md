# Vestaboard-Reverse-Engineering

## Protocol
See [here](https://github.com/EngineOwningSoftware/Vestaboard-Reverse-Engineering/blob/main/Protocol.md)
## Examples
[C++](https://github.com/EngineOwningSoftware/Vestaboard-Reverse-Engineering/blob/main/examples/cpp/Main.cpp)
[Python](https://github.com/EngineOwningSoftware/Vestaboard-Reverse-Engineering/blob/main/examples/python/main.py)

## ToDo

 - [ ] Document firmware dumping process
 - [ ] Document vulnerabilities in original firmware
 - [ ] Take a deeper look at the ACK/Response packets sent from the board
 - [ ] Clean-up and release custom firmware that offers a local device-level API
 - [ ] Take a deeper look into all the docker containers running in the original firmware

## Credits

 - [Vestaboard](https://www.vestaboard.com/), for creating the board itself
 - [Bennet Huch](https://twitter.com/B3nn0_DE), author of this project

## Authors note
The hardware of the board is very well made, the look and feel of it is extraordinary, sadly I can't say the same about the firmware running on it, there are 5 docker containers running next to each other, highly redundant code. Legacy code with legacy API is left unused in many parts of the firmware, the whole software stack is a mix of Python, Java and Bash. The code quality of the python code is not very good, there are lots of things Vestaboard could (and hopefully will) improve over the next few months.

~~There is one big flaw in the firmware that needs to be disclosed to the vendor first, currently I don't really have the time to report it. (Vestaboard feel free to contact me via Twitter [linked above] or via `contact [at] bennethu [dot] ch`).~~
Vestabaord did in fact contact me (23.03.2022), I've provided them with the flaw in the firmware, but they didn't seem to care (at least not so much that they had the time to respond to my mail), so here is the flaw (Basically verbatim the mail I sent):

```
So basically I noticed that either the rootfs or one of the docker containers has a "vestaboard-root-key", a SSH private key.
The public part of that key is in the rootfs /root/.ssh/authorized_keys, that key is used to issue a reboot from within the install.sh (line 34 and line 61). 

As I only have one Vestaboard I can't verify that the key is not generated per-device, but I can't seem to find any indications that its generated on first boot. 

Per default the SSH daemon listens on 0.0.0.0:22, and allows root login via key. 

If the key is indeed shared among all Vestaboards, it would pose a big threat to all customers, as it would only require to be on the same network as the Vestaboard to modify the firmware, install a backdoor(thus allowing persistent network access), or maybe even bricking the device. 
```

I might release the private key too in a bit, when I'm back home.

Thats all for now, I hope there are at least a few users who will be able to create something with this.

## License

This work is licensed under the MIT license, as included in the [LICENSE](LICENSE) file.
