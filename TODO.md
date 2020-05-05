#DLStorm To do list

- Add new DB class
    - create new database class that abstracts db layer (will be able to use different methods such as flatfile, mysql, sqlite)
    - (corrupted data file?) take out sqllite3 and replace with mysql
    - add flat file storage for database
        - lmdb ( Ubuntu sudo apt install liblmdb-dev liblmdb++-dev lmdb-utils ) (centos EPEL: lmdb-0.9.22-2.el7.x86_64.rpm)
        
- integrate database with server / cvars / console

- doxygen the dlstorm/src and put it in dlstorm/docs