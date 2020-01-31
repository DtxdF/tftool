# tftool
**It is an educational program to transfer files trying to be as minimalist as possible.**

## Instalation

```bash
git clone https://github.com/DtxdF/tftool.git
cd tftool
make install
service tftool start && service tftool status
```

***You Need the minimun privileges***

## Tutorial basic

Open the configuration file and edit it to your liking:

```bash
editor /etc/tftool/gconf.cfg
```

****Once the program has started, it is not necessary to restart the daemon every time the configuration file is edited. It updates itself!***

The client executes:

```bash
python3 tfclient.py -action list -a localhost -p 8043
...
python3 tfclient.py -action download -f test.txt -o /tmp/test.txt -a localhost -p 8043
...
```

## Notes

* If you want to configure some things as buffers, the configuration file path; You can do it in "conf / config.h", but to modify it you will also need to modify the Makefile in case you have not already installed it.
* They can leave an issue, make a pull request if they have a doubt, a suggestion or find an error, but they can also do it through DtxdF@protonmail.com
* Don't wait long, it's an educational program

***\~ DtxdF***
