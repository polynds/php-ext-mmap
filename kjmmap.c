/* kjmmap extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_kjmmap.h"
#include "kjmmap_arginfo.h"
#include "ext/standard/php_var.h"
#include "zend_API.h"
#include "zend_exceptions.h"
#include <sys/mman.h>
#include <fcntl.h>

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

zend_class_entry* kjmmap_ce_ptr;
php_kjmmap_object* obj;

PHP_METHOD (Kjmmap, __construct)
{
	php_printf("__construct called.\n");
}

PHP_METHOD (Kjmmap, __destruct)
{
	php_printf("__destruct called.\n");

}

PHP_METHOD (Kjmmap, open)
{
	php_printf("mmap called.0----\n");
	char* filename;
	size_t filename_len = 0;
	long offset , pa_offset;
	long size = -1;

	obj = emalloc(sizeof(php_kjmmap_object));

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s|ll", &filename, &filename_len, &size, &offset)==FAILURE) {
		RETURN_THROWS();
	}
	php_printf("mmap called.1----\n");
	if (filename_len<=0) {
		RETURN_THROWS();
	}

	int fd;
	if ((fd = open(filename, O_RDWR | O_CREAT, 0777))<0) {
		php_printf("open(%s, O_RDWR) failed.\n", filename);
		RETURN_THROWS();
	}

	if (size<=0) {
		struct stat _stat;
		if (fstat(fd, &_stat)<0) {
			php_printf("fstat(%s) failed.\n", filename);
			close(fd);
			RETURN_THROWS();
		}
		if (_stat.st_size==0) {
			php_printf("file[%s] is empty.\n", filename);
			close(fd);
			RETURN_THROWS();
		}
		if (offset>0) {
			size = _stat.st_size-offset;
		} else {
			size = _stat.st_size;
		}
	}
	pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);
	php_printf("mmap 1----\n");
	void* addr = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, offset);
	php_printf("mmap 2----\n");
	if (addr==MAP_FAILED) {
		php_printf("mmap 3----\n");
		php_printf("mmap(%ld) failed.\n", size);
		RETURN_THROWS();
	}
	php_printf("mmap 4----\n");
	obj->filename = filename;
	obj->size = size;
	obj->offset = offset;
	obj->memory = addr;
	obj->mmap_ptr = addr;

	close(fd);

	obj->stream = php_stream_alloc(NULL, obj, NULL, "r+");

	php_printf("filename:%s\n", filename);

}

//const char *buffer, size_t length
PHP_METHOD (Kjmmap, write)
{
	php_printf("mmap write called.\n");
	char* buffer;
	size_t buffer_len = 0;
	size_t length;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s|l", &buffer, &buffer_len, &length)==FAILURE) {
		RETURN_THROWS();
	}
	int n_write = MIN(obj->memory+obj->size-obj->mmap_ptr, length);
	if (n_write==0) {
		RETURN_FALSE;
	}
	memcpy(obj->mmap_ptr, buffer, n_write);
	obj->mmap_ptr += n_write;
	RETURN_TRUE;
}
//size_t length
PHP_METHOD (Kjmmap, read)
{
	php_printf("mmap read called.\n");
	char* buffer;
	size_t buffer_len = 0;

	size_t length;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &length)==FAILURE) {
		RETURN_THROWS();
	}
	php_printf("mmap read1 %zu.\n", length);
	int n_read = MIN(obj->memory+obj->size-obj->mmap_ptr, length);
	if (n_read==0) {
		RETURN_FALSE;
	}
	php_printf("mmap read2 %d.\n", n_read);
	memcpy(buffer, obj->mmap_ptr, n_read);
	obj->mmap_ptr += n_read;
	php_printf("mmap read3 %s.\n", buffer);
	RETVAL_STRINGL(buffer, buffer_len);
}
PHP_METHOD (Kjmmap, flush)
{
	php_printf("mmap flush called.\n");
	msync(obj->memory, obj->size, MS_SYNC | MS_INVALIDATE);
	RETURN_TRUE;
}
//off_t offset, int whence
PHP_METHOD (Kjmmap, seek)
{
	php_printf("mmap seek called.\n");
	off_t offset;
	int whence;
	off_t* newoffset;
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &offset, &whence)==FAILURE) {
		RETURN_THROWS();
	}
	switch (whence) {
	case SEEK_SET:
		if (offset<0 || offset>obj->size) {
			*newoffset = (off_t)-1;
			RETURN_FALSE;
		}
		obj->mmap_ptr = obj->memory+offset;
		*newoffset = offset;
		RETURN_FALSE;
	case SEEK_CUR:
		if (obj->mmap_ptr+offset<obj->memory || obj->mmap_ptr+offset>obj->memory+obj->size) {
			*newoffset = (off_t)-1;
			RETURN_FALSE;
		}
		obj->mmap_ptr += offset;
		*newoffset = obj->mmap_ptr-obj->memory;
		RETURN_FALSE;
	case SEEK_END:
		if (offset>0 || -1*offset>obj->size) {
			*newoffset = (off_t)-1;
			RETURN_FALSE;
		}
		obj->mmap_ptr += offset;
		*newoffset = obj->mmap_ptr-obj->memory;
		RETURN_FALSE;
	default: *newoffset = (off_t)-1;
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
PHP_METHOD (Kjmmap, close)
{
	php_printf("mmap close called.\n");
	munmap(obj->memory, obj->size);
	efree(obj);
	RETURN_TRUE;
}

PHP_MINIT_FUNCTION (kjmmap)
{

	kjmmap_ce_ptr = register_class_kjmmap();

	return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION (kjmmap)
{
	php_printf("mmap PHP_MSHUTDOWN_FUNCTION called.\n");
	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION (kjmmap)
{
	php_printf("mmap PHP_RSHUTDOWN_FUNCTION called.\n");
	return SUCCESS;
}

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION (kjmmap)
{
#if defined(ZTS) && defined(COMPILE_DL_KJMMAP)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION (kjmmap)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "kjmmap support", "enabled");
	php_info_print_table_row(2, "URL", "https://github.com/polynds");
	php_info_print_table_row(2, "author", "Jian.kuang");
	php_info_print_table_end();
}
/* }}} */

/* {{{ kjmmap_module_entry */
zend_module_entry kjmmap_module_entry = {
		STANDARD_MODULE_HEADER,
		"kjmmap",                    /* Extension name */
		NULL,                    /* zend_function_entry */
		PHP_MINIT(kjmmap),                            /* PHP_MINIT - Module initialization */
		PHP_MSHUTDOWN(kjmmap),                            /* PHP_MSHUTDOWN - Module shutdown */
		PHP_RINIT(kjmmap),            /* PHP_RINIT - Request initialization */
		PHP_RSHUTDOWN(kjmmap),                            /* PHP_RSHUTDOWN - Request shutdown */
		PHP_MINFO(kjmmap),            /* PHP_MINFO - Module info */
		PHP_KJMMAP_VERSION,        /* Version */
		STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_KJMMAP
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(kjmmap)
#endif
