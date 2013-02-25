/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2007 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header,v 1.16.2.1.2.1 2007/01/01 19:32:09 iliaa Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_prlib.h"

ZEND_DECLARE_MODULE_GLOBALS(prlib)

/* True global resources - no need for thread safety here */
static int le_prlib;

/* {{{ prlib_functions[]
 *
 * Every user visible function must have an entry in prlib_functions[].
 */
zend_function_entry prlib_functions[] = {
	PHP_FE(confirm_prlib_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(method_acc_get,	NULL)		/* For testing, remove later. */
	PHP_FE(method_acc_set,	NULL)		/* For testing, remove later. */
	PHP_FE(prop_acc_get,	NULL)		/* For testing, remove later. */
	PHP_FE(prop_acc_set,	NULL)		/* For testing, remove later. */
	PHP_FE(enable_exit,	NULL)		/* For testing, remove later. */
	PHP_FE(disable_exit,	NULL)		/* For testing, remove later. */
	{NULL, NULL, NULL}	/* Must be the last line in prlib_functions[] */
};
/* }}} */

/* {{{ prlib_module_entry
 */
zend_module_entry prlib_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"prlib",
	prlib_functions,
	PHP_MINIT(prlib),
	PHP_MSHUTDOWN(prlib),
	PHP_RINIT(prlib),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(prlib),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(prlib),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_PRLIB
ZEND_GET_MODULE(prlib)
#endif

/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("prlib.exit_enable", "0", PHP_INI_SYSTEM, OnUpdateBool, exit_enable, zend_prlib_globals, prlib_globals)
PHP_INI_END()
/* }}} */

/* {{{ php_prlib_init_globals
 */
static void php_prlib_init_globals(zend_prlib_globals *prlib_globals)
{
	prlib_globals->exit_enable = 0;
}
/* }}} */


static int php_prlib_exit_handler(ZEND_OPCODE_HANDLER_ARGS) /* {{{ */ {
	if (PRLIB_G(exit_enable)) {
		return 256 + ZEND_NOP; // noop opcode
	}
	return ZEND_USER_OPCODE_DISPATCH;
} /* }}} */


static void php_prlib_register_handlers(TSRMLS_D) /* {{{ */ {

	zend_set_user_opcode_handler(ZEND_EXIT, php_prlib_exit_handler);

} /* }}} */



/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(prlib)
{
	REGISTER_INI_ENTRIES();
	php_prlib_register_handlers(TSRMLS_C);
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(prlib)
{
	UNREGISTER_INI_ENTRIES();
	
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(prlib)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(prlib)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(prlib)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "prlib support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_prlib_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_prlib_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "prlib", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/

/* {{{ php_runkit_fetch_class_int
 */
static int php_runkit_fetch_class_int(char *classname, int classname_len, zend_class_entry **pce TSRMLS_DC)
{
	char *lclass;
	zend_class_entry *ce;
#ifdef ZEND_ENGINE_2
	zend_class_entry **ze;
#endif

	/* Ignore leading "\" */
	if (classname[0] == '\\') {
		++classname;
		--classname_len;
	}

	lclass = estrndup(classname, classname_len);
	if (lclass == NULL) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Not enough memory");
		return FAILURE;
	}
	php_strtolower(lclass, classname_len);

#ifdef ZEND_ENGINE_2
	if (zend_hash_find(EG(class_table), lclass, classname_len + 1, (void**)&ze) == FAILURE ||
		!ze || !*ze) {
		efree(lclass);
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "class %s not found", classname);
		return FAILURE;
	}
	ce = *ze;
#else
	if (zend_hash_find(EG(class_table), lclass, classname_len + 1, (void**)&ce) == FAILURE ||
		!ce) {
		efree(lclass);
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "class %s not found", classname);
		return FAILURE;
	}
#endif

	if (pce) {
		*pce = ce;
	}

	efree(lclass);
	return SUCCESS;
}
/* }}} */

/* {{{ php_runkit_fetch_class_method
 */
static int php_runkit_fetch_class_method(char *classname, int classname_len, char *fname, int fname_len, zend_class_entry **pce, zend_function **pfe
TSRMLS_DC)
{
	HashTable *ftable = EG(function_table);
	zend_class_entry *ce;
	zend_function *fe;
	char *fname_lower;
#ifdef ZEND_ENGINE_2
	zend_class_entry **ze;
#endif

	if (php_runkit_fetch_class_int(classname, classname_len, &ce TSRMLS_CC) == FAILURE) {
		return FAILURE;
	}

	if (ce->type != ZEND_USER_CLASS) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "class %s is not a user-defined class", classname);
		return FAILURE;
	}

	if (pce) {
		*pce = ce;
	}

	ftable = &ce->function_table;

	fname_lower = estrndup(fname, fname_len);
	if (fname_lower == NULL) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Not enough memory");
		return FAILURE;
	}
	php_strtolower(fname_lower, fname_len);

	if (zend_hash_find(ftable, fname_lower, fname_len + 1, (void**)&fe) == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "%s::%s() not found", classname, fname);
		efree(fname_lower);
		return FAILURE;
	}

	if (fe->type != ZEND_USER_FUNCTION) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "%s::%s() is not a user function", classname, fname);
		efree(fname_lower);
		return FAILURE;
	}

	if (pfe) {
		*pfe = fe;
	}

	efree(fname_lower);
	return SUCCESS;
}
/* }}} */

PHP_FUNCTION(method_acc_get)
{	
	char *classname, *methodname;
	int classname_len, methodname_len;
    zend_class_entry *ce;
	zend_function *fe;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s/s/",	&classname, &classname_len,
																	&methodname, &methodname_len) == FAILURE) {
		RETURN_FALSE;
	}

	if (!classname_len || !methodname_len) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Empty parameter given");
		RETURN_FALSE;
	}

	if (php_runkit_fetch_class_method(classname, classname_len, methodname, methodname_len, &ce, &fe TSRMLS_CC) == FAILURE) {
		RETURN_FALSE;
	}

    switch (fe->common.fn_flags & ZEND_ACC_PPP_MASK) {
        case ZEND_ACC_PUBLIC:
            RETURN_STRINGL("public", sizeof("public") - 1, 1);
            break;
        case ZEND_ACC_PROTECTED:
            RETURN_STRINGL("protected", sizeof("protected") - 1, 1);
            break;
        case ZEND_ACC_PRIVATE:
            RETURN_STRINGL("private", sizeof("private") - 1, 1);
            break;
        default:
            RETURN_STRINGL("Unknown", sizeof("Unknown") - 1, 1);
            break;
    }
	RETURN_FALSE;
}

PHP_FUNCTION(method_acc_set) {
    char *classname, *methodname, *new_acc;
	int classname_len, methodname_len, new_acc_len;
    int acc_flag = 0;
    zend_class_entry *ce;
	zend_function *fe;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s/s/s/", &classname, &classname_len,
																   &methodname, &methodname_len,
																   &new_acc, &new_acc_len) == FAILURE) {
		RETURN_FALSE;
	}

	if (!classname_len || !methodname_len || !new_acc_len) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Empty parameter given");
		RETURN_FALSE;
	}

    if (strcmp(new_acc, "public") == 0) {
        acc_flag = ZEND_ACC_PUBLIC;
    } else if (strcmp(new_acc, "protected") == 0) {
        acc_flag = ZEND_ACC_PROTECTED;
    } else if (strcmp(new_acc, "private") == 0) {
        acc_flag = ZEND_ACC_PRIVATE;
    } else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Access property error %s", new_acc);
		RETURN_FALSE;
    }
        
	if (php_runkit_fetch_class_method(classname, classname_len, methodname, methodname_len, &ce, &fe TSRMLS_CC) == FAILURE) {
		RETURN_FALSE;
	}

    switch (acc_flag) {
        case ZEND_ACC_PUBLIC:
        case ZEND_ACC_PROTECTED:
        case ZEND_ACC_PRIVATE:
            fe->common.fn_flags &= ~ZEND_ACC_PPP_MASK;
            fe->common.fn_flags |= acc_flag;
            break;
        default:
            RETURN_FALSE;
            break;
    }
	RETURN_TRUE;
}

static int class_property_fetch(char *classname, int classname_len, char *propname, int propname_len, zend_property_info **ppi TSRMLS_DC) {

	zend_class_entry *ce, **pce;
	zend_property_info *pi;

	if (zend_lookup_class(classname, classname_len, &pce TSRMLS_CC) == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Class %s does not exist", classname);
		return FAILURE;
	}
	ce = *pce;

	if (zend_hash_find(&ce->properties_info, propname, propname_len + 1, (void**) &pi) == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Class property %s does not exist", propname);
		return FAILURE;
	}

	if (ppi) {
		*ppi = pi;
	}
	return SUCCESS;
}

PHP_FUNCTION(prop_acc_get) 
{
	char *classname, *propname;
	int classname_len, propname_len;
	zend_property_info *property_info;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s/s/", &classname, &classname_len,
																 &propname, &propname_len) == FAILURE) {
		RETURN_FALSE;
	}

	if (!classname_len || !propname_len) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Empty parameter given");
		RETURN_FALSE;
	}

    if (class_property_fetch(classname, classname_len, propname, propname_len, &property_info) == FAILURE) {
		RETURN_FALSE;
	}
	
	switch (property_info->flags & ZEND_ACC_PPP_MASK) {
        case ZEND_ACC_PUBLIC:
            RETURN_STRINGL("public", sizeof("public") - 1, 1);
            break;
        case ZEND_ACC_PROTECTED:
            RETURN_STRINGL("protected", sizeof("protected") - 1, 1);
            break;
        case ZEND_ACC_PRIVATE:
            RETURN_STRINGL("private", sizeof("private") - 1, 1);
            break;
        default:
            RETURN_STRINGL("Unknown", sizeof("Unknown") - 1, 1);
            break;
    }
	RETURN_FALSE;
}

PHP_FUNCTION(prop_acc_set) 
{
	char *classname, *propname, *new_acc;
	int classname_len, propname_len, new_acc_len;
	int acc_flag;
	zend_property_info *property_info;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s/s/s/", &classname, &classname_len,
																   &propname, &propname_len,
																   &new_acc, &new_acc_len) == FAILURE) {
		RETURN_FALSE;
	}

	if (!classname_len || !propname_len || !new_acc_len) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Empty parameter given");
		RETURN_FALSE;
	}

    if (class_property_fetch(classname, classname_len, propname, propname_len, &property_info) == FAILURE) {
		RETURN_FALSE;
	}
	
 	if (strcmp(new_acc, "public") == 0) {
        acc_flag = ZEND_ACC_PUBLIC;
    } else if (strcmp(new_acc, "protected") == 0) {
        acc_flag = ZEND_ACC_PROTECTED;
    } else if (strcmp(new_acc, "private") == 0) {
        acc_flag = ZEND_ACC_PRIVATE;
    } else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "Access property error %s", new_acc);
		RETURN_FALSE;
    }

  	switch (acc_flag) {
        case ZEND_ACC_PUBLIC:
        case ZEND_ACC_PROTECTED:
        case ZEND_ACC_PRIVATE:
            property_info->flags &= ~ZEND_ACC_PPP_MASK;
            property_info->flags |= acc_flag;
            break;
        default:
            RETURN_FALSE;
            break;
    }
	RETURN_TRUE;
}

PHP_FUNCTION(enable_exit) 
{
	PRLIB_G(exit_enable) = 0;
}

PHP_FUNCTION(disable_exit) 
{
	PRLIB_G(exit_enable) = 1;
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
