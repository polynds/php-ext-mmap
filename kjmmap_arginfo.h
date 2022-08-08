/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: efdd79c2c8ccff694699c86fdd6248a13839c744 */

ZEND_BEGIN_ARG_INFO(Kjmmap___construct, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(Kjmmap___destruct, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Kjmmap_open, 0, 0, 1)
				ZEND_ARG_TYPE_INFO(0, filename, IS_STRING, 0)
				ZEND_ARG_TYPE_INFO(0, size, IS_LONG, 0)
				ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Kjmmap_write, 0, 0, 1)
				ZEND_ARG_TYPE_INFO(0, buffer, IS_STRING, 0)
				ZEND_ARG_TYPE_INFO(0, length, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Kjmmap_read, 0, 0, 1)
				ZEND_ARG_TYPE_INFO(0, buffer, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(Kjmmap_flush, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(Kjmmap_seek, 0, 0, 1)
				ZEND_ARG_TYPE_INFO(0, offset, IS_LONG, 0)
				ZEND_ARG_TYPE_INFO(0, whence, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(Kjmmap_close, 0)
ZEND_END_ARG_INFO()

static const zend_function_entry ext_functions[] = {
		PHP_ME(Kjmmap, __construct, Kjmmap___construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
		PHP_ME(Kjmmap, __destruct, Kjmmap___destruct, ZEND_ACC_PUBLIC)
		PHP_ME(Kjmmap, open, Kjmmap_open, ZEND_ACC_PUBLIC)
		PHP_ME(Kjmmap, write, Kjmmap_write, ZEND_ACC_PUBLIC)
		PHP_ME(Kjmmap, read, Kjmmap_read, ZEND_ACC_PUBLIC)
		PHP_ME(Kjmmap, flush, Kjmmap_flush, ZEND_ACC_PUBLIC)
		PHP_ME(Kjmmap, seek, Kjmmap_seek, ZEND_ACC_PUBLIC)
		PHP_ME(Kjmmap, close, Kjmmap_close, ZEND_ACC_PUBLIC)
		PHP_FE_END
};

static zend_class_entry*
register_class_kjmmap(void)
{
	zend_class_entry kjmmap_ce, * class_entry;
	INIT_CLASS_ENTRY(kjmmap_ce, "Kjmmap", ext_functions);
	class_entry = zend_register_internal_class_ex(&kjmmap_ce, NULL);
	return class_entry;
}