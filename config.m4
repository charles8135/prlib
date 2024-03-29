dnl $Id$
dnl config.m4 for extension prlib

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(prlib, for prlib support,
dnl Make sure that the comment is aligned:
dnl [  --with-prlib             Include prlib support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(prlib, whether to enable prlib support,
dnl Make sure that the comment is aligned:
[  --enable-prlib           Enable prlib support])

if test "$PHP_PRLIB" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-prlib -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/prlib.h"  # you most likely want to change this
  dnl if test -r $PHP_PRLIB/$SEARCH_FOR; then # path given as parameter
  dnl   PRLIB_DIR=$PHP_PRLIB
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for prlib files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       PRLIB_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$PRLIB_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the prlib distribution])
  dnl fi

  dnl # --with-prlib -> add include path
  dnl PHP_ADD_INCLUDE($PRLIB_DIR/include)

  dnl # --with-prlib -> check for lib and symbol presence
  dnl LIBNAME=prlib # you may want to change this
  dnl LIBSYMBOL=prlib # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $PRLIB_DIR/lib, PRLIB_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_PRLIBLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong prlib lib version or lib not found])
  dnl ],[
  dnl   -L$PRLIB_DIR/lib -lm -ldl
  dnl ])
  dnl
  dnl PHP_SUBST(PRLIB_SHARED_LIBADD)

  PHP_NEW_EXTENSION(prlib, prlib.c, $ext_shared)
fi
