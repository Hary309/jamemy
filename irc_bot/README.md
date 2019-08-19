# IrcBot

Bot zbiera i zapisuje najbardziej lubiane linki z czatu

## Jak skompilować

Pobierz [libircclient](https://sourceforge.net/projects/libircclient/files/libircclient/1.10/libircclient-1.10.tar.gz/download) i wypakuj do folderu `vendor/libircclient`

### Sklonuj repozytorium

```bash
git clone https://github.com/Harry09/jamemy
cd jamemy
git submodule update --init --recursive
```

### Kompilacja mariadb-connector-c

```bash
cd vendor/mariadb-connector-c
mkdir bin
cd bin
cmake ..
cmake --build . --target libmariadb
```

### Kompilacja re2

```bash
cd vendor/re2
mkdir bin
cd bin
cmake ..
cmake --build . --target re2
```

### Kompilacja IrcBot

```bash
mkdir bin
cd bin
cmake ..
cmake --build .
```

## Po co mi klient SMTP?

Chce dostać powiadomienie w przypadku jakby Bot padł i wymagał ręcznego restartu. Jestem nowy w sprawach serwerowych, więc wolę robić takie rzeczy ręcznie, może kiedyś ogarnę jakiś system dobrego restartowania.
