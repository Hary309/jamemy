# JaMemy

Strona z najlepszymi memami z Poorchatu

## Jak skompilować backend

```bash
git clone https://github.com/Harry09/jamemy
cd jamemy
git submodule update --init --recursive
```

### Kompilacja mariadb-connector-c

```bash
cd irc_bot/vendor/mariadb-connector-c
mkdir bin
cd bin
cmake ..
cmake --build . --target libmariadb
```

### Kompilacja re2

```bash
cd irc_bot/vendor/re2
mkdir bin
cd bin
cmake ..
cmake --build . --target re2
```

### Kompilacja IrcBot

```bash
cd irc_bot
mkdir bin
cd bin
cmake ..
cmake --build .
```
