/* kjmmap extension for PHP */

#ifndef PHP_KJMMAP_H
# define PHP_KJMMAP_H

extern zend_module_entry kjmmap_module_entry;
# define phpext_kjmmap_ptr &kjmmap_module_entry

# define PHP_KJMMAP_VERSION "0.1.0"

PHP_METHOD (Kjmmap, __construct);
PHP_METHOD (Kjmmap, __destruct);
PHP_METHOD (Kjmmap, open);
PHP_METHOD (Kjmmap, write);
PHP_METHOD (Kjmmap, read);
PHP_METHOD (Kjmmap, flush);
PHP_METHOD (Kjmmap, seek);
PHP_METHOD (Kjmmap, close);

typedef struct php_kjmmap_object {
	size_t size;
	off_t offset;
	char* filename;
	void* memory;
	void* mmap_ptr;
	php_stream* stream;
	zend_object std;
} php_kjmmap_object;

static inline php_kjmmap_object*
php_kjmmap_object_from_obj(zend_object* obj)
{
	return (php_kjmmap_object*)((char*)(obj)-XtOffsetOf(php_kjmmap_object, std));
}

#define Z_KJMMAP_P(zv)  php_kjmmap_object_from_obj(Z_OBJ_P((zv)))

# if defined(ZTS) && defined(COMPILE_DL_KJMMAP)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif    /* PHP_KJMMAP_H */
