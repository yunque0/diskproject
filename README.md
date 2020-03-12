# diskproject
a ftp project like baidu netdisk written by c in linux
一个简单的ftp文件服务器。
在服务器端启动后，客户端可通过cd,ls,puts,gets,remove,pwd等命令查看下载和上传文件。
同时通过线程池实现了多个客户端同时下载文件，同时实现了密码加密，日志记录客户端请求操作，断点续传，大文件mmap传输，文件秒传，使用数据库实现虚拟文件系统等功能。
