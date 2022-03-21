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

There is one big flaw in the firmware that needs to be disclosed to the vendor first, currently I don't really have the time to report it. (Vestaboard feel free to contact me via Twitter [linked above] or via `contact [at] bennethu [dot] ch`).

Thats all for now, I hope there are at least a few users who will be able to create something with this.

## License

This work is licensed under the MIT license, as included in the [LICENSE](LICENSE) file.
