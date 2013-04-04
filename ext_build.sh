#!/bin/sh


read -p "Buildir?: " DIRNAME

mkdir $DIRNAME

cp config.m4 $DIRNAME
cp cs_extension.cc $DIRNAME
cp combat.h $DIRNAME
cp combat.cc $DIRNAME
cp php_combatsystem.h $DIRNAME

cd "$DIRNAME/"
phpize
./configure --enable-combatsystem
make
make install
/etc/init.d/lighttpd restart
