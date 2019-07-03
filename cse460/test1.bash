1.	sudo yum –y install yum-utils epel-release
2.	sudo vim /etc/yum.d/epel-repo
a.	change https to http
3.	sudo yum install –y http://rpms.remirepo.net/enterprise/remi-release-7.rpm
4.	sudo yum-config-manager –enable remi-php72
5.	sudo yum –y update
6.	sudo yum –y install php72 php72-php-fpm php72-php-gd php72-php-json 
php72-php-mbstring php72-php-mysqlnd php72-php-xml php72-php-xmlrpc php72-php-opcache
7.	php -v
