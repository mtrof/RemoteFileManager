# RemoteFileManager
## Удаленный файловый менеджер с имперсонацией клиента
### Компиляция:
1. Для сборки Client.exe включить в проект следующие файлы:
   - client.cpp
   - interface.h
   - interface.idl
   - interface_c.c
2. Для сборки Server.exe включить в проект следующие файлы:
   - interface.h
   - interface.idl
   - interface_s.c
   - server.cpp

### Пример загрузки файла на сервер:
![picture1](pictures/picture1.png)
![picture2](pictures/picture2.png)
### Пример скачивания файла с сервера:
![picture3](pictures/picture3.png)
![picture4](pictures/picture4.png)