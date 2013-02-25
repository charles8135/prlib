#!/bin/bash

## CONFIG ##
PHP_PATH=/home/liuyue01/local/php
EXT_TMP_DIR=no-debug-non-zts-20060613
## /CONFIG ##

$PHP_PATH/bin/phpize

./configure --with-php-config=$PHP_PATH/bin/php-config 

make 

make install

cp $PHP_PATH/lib/php/extensions/$EXT_TMP_DIR/prlib.so $PHP_PATH/lib/php/extensions/ 
