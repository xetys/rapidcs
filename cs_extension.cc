// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of David Steiman nor the name of any other
//    contributor may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY David Steiman AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL David Steiman OR ANY OTHER
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "php_combatsystem.h"
#include "combat.cc"

zend_object_handlers combatsystem_object_handlers;

struct combatsystem_object {
    zend_object std;
    CombatSys  *combatsystem;
};

zend_class_entry *combatsystem_ce;

void combatsystem_free_storage(void *object TSRMLS_DC)
{
    combatsystem_object *obj = (combatsystem_object *)object;
    delete obj->combatsystem; 

    zend_hash_destroy(obj->std.properties);
    FREE_HASHTABLE(obj->std.properties);

    efree(obj);
}

zend_object_value combatsystem_create_handler(zend_class_entry *type TSRMLS_DC)
{
    zval *tmp;
    zend_object_value retval;

    combatsystem_object *obj = (combatsystem_object *)emalloc(sizeof(combatsystem_object));
    memset(obj, 0, sizeof(combatsystem_object));
    obj->std.ce = type;

    ALLOC_HASHTABLE(obj->std.properties);
    zend_hash_init(obj->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
    zend_hash_copy(obj->std.properties, &type->default_properties,
        (copy_ctor_func_t)zval_add_ref, (void *)&tmp, sizeof(zval *));

    retval.handle = zend_objects_store_put(obj, NULL,
        combatsystem_free_storage, NULL TSRMLS_CC);
    retval.handlers = &combatsystem_object_handlers;

    return retval;
}
PHP_METHOD(CombatSys , __construct)
{
	CombatSys *combatsystem = NULL;
	zval *object = getThis();

	combatsystem = new CombatSys();
	combatsystem_object *obj = (combatsystem_object *)zend_object_store_get_object(object TSRMLS_CC);
	obj->combatsystem = combatsystem;
}
PHP_METHOD(CombatSys , getVersion)
{
	CombatSys *combatsystem;
    combatsystem_object *obj = (combatsystem_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);
    combatsystem = obj->combatsystem;

	if (combatsystem != NULL) {
		RETURN_LONG(combatsystem->getVersion());
	}
}

PHP_METHOD(CombatSys , setShootAgainstExploded)
{
    CombatSys *combatsystem;
    combatsystem_object *obj = (combatsystem_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);
    combatsystem = obj->combatsystem;
    bool modus;

	if (combatsystem != NULL) {
		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "b", &modus) == FAILURE) {
    	    RETURN_NULL();
		}
        
        combatsystem->setShootAgainstExploded(modus);
	}
}
PHP_METHOD(CombatSys , setRapidfire)
{
    CombatSys *combatsystem;
    combatsystem_object *obj = (combatsystem_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);
    combatsystem = obj->combatsystem;
    bool modus;

    if (combatsystem != NULL) {
		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "b", &modus) == FAILURE) {
    	    RETURN_NULL();
		}
        
        combatsystem->setRapidfire(modus);
	}
}
PHP_METHOD(CombatSys , setUnitCount)
{
	long unitCount;
	
	CombatSys *combatsystem;
    combatsystem_object *obj = (combatsystem_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);
    combatsystem = obj->combatsystem;

	if (combatsystem != NULL) {
			
		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &unitCount) == FAILURE) {
		    RETURN_NULL();
		}
		

		combatsystem->setUnitCount((int)unitCount);
	}
}

//void configShip(int id, long attack, long hull, long shield, char *rapidfire);
PHP_METHOD(CombatSys , configShip)
{
	long id;
	long attack;
	long hull;
	long shield;
	char *rapidfire;
	long rapidfire_length;
	
	CombatSys *combatsystem;
    combatsystem_object *obj = (combatsystem_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);
    combatsystem = obj->combatsystem;

	if (combatsystem != NULL) {
		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lllls", &id, &attack, &hull, &shield,&rapidfire,&rapidfire_length) == FAILURE) {
		    RETURN_NULL();
		}
		if(!combatsystem->canSetShip((int)id))
		{
			php_error(E_ERROR,"Error in ShipConfiguration!");
			RETURN_NULL();
		}

		combatsystem->configShip((int)id,attack,hull,shield,rapidfire);
	}
}

//void addAttacker(int id, int weapon, int shield, int armor);
PHP_METHOD(CombatSys , addAttacker)
{
	long id;
	long weapon;
	long shield;
	long armor;
	
	CombatSys *combatsystem;
    combatsystem_object *obj = (combatsystem_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);
    combatsystem = obj->combatsystem;

	if (combatsystem != NULL) {
			if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llll", &id, &weapon, &shield, &armor) == FAILURE) {
		    RETURN_NULL();
		}
		//allShips?
		if(!combatsystem->allShipsConfigured())
		{
			php_error(E_ERROR,"Not all ships are configured!");
			RETURN_NULL();
		}
		//check Attacker
		if(!combatsystem->canSetAttacker((int)id))
		{
			php_error(E_ERROR,"You cannot set this Player");
			RETURN_NULL();
		}

		combatsystem->addAttacker((int)id,(int)weapon,(int)shield,(int)armor);
	}
	
}

PHP_METHOD(CombatSys , addDefender)
{
	long id;
	long weapon;
	long shield;
	long armor;
	
	CombatSys *combatsystem;
    combatsystem_object *obj = (combatsystem_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);
    combatsystem = obj->combatsystem;

	if (combatsystem != NULL) {
			if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "llll", &id, &weapon, &shield, &armor) == FAILURE) {
		    RETURN_NULL();
		}
		//allShips?
		if(!combatsystem->allShipsConfigured())
		{
			php_error(E_ERROR,"Not all ships are configured!");
			RETURN_NULL();
		}
		//check Defender
		if(!combatsystem->canSetDefender((int)id))
		{
			php_error(E_ERROR,"You cannot set this Player");
			RETURN_NULL();
		}

		combatsystem->addDefender((int)id,(int)weapon,(int)shield,(int)armor);
	}
}
//void addShips(const unsigned fraction, int playerId, int shipId, int amount);
PHP_METHOD(CombatSys , addAttackShips)
{
	long pid;
	long sid;
	long sam;

	CombatSys *combatsystem;
    combatsystem_object *obj = (combatsystem_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);
    combatsystem = obj->combatsystem;

	if (combatsystem != NULL) {
			if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lll", &pid, &sid, &sam) == FAILURE) {
		    RETURN_NULL();
		}

		combatsystem->addShips(T_ATTACKER,(int)pid,(int)sid,(int)sam);
	}
}

PHP_METHOD(CombatSys , addDefendShips)
{
	long pid;
	long sid;
	long sam;

	CombatSys *combatsystem;
    combatsystem_object *obj = (combatsystem_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);
    combatsystem = obj->combatsystem;

	if (combatsystem != NULL) {
			if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lll", &pid, &sid, &sam) == FAILURE) {
		    RETURN_NULL();
		}

		combatsystem->addShips(T_DEFENDER,(int)pid,(int)sid,(int)sam);
	}
}

PHP_METHOD(CombatSys , battle)
{
	CombatSys *combatsystem;
    combatsystem_object *obj = (combatsystem_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);
    combatsystem = obj->combatsystem;

	if (combatsystem != NULL) {
		combatsystem->battle();
	}
}
PHP_METHOD(CombatSys , getResult)
{	
	CombatSys *combatsystem;
    combatsystem_object *obj = (combatsystem_object *)zend_object_store_get_object(
        getThis() TSRMLS_CC);
    combatsystem = obj->combatsystem;

	if (combatsystem != NULL) {
		RETURN_STRING(combatsystem->getResult(),1);
	}
}


function_entry cs_methods[] = {
    PHP_ME(CombatSys,  __construct,			NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(CombatSys,  getVersion,         NULL, ZEND_ACC_PUBLIC)
    PHP_ME(CombatSys,  setShootAgainstExploded,         NULL, ZEND_ACC_PUBLIC)
    PHP_ME(CombatSys,  setRapidfire,         NULL, ZEND_ACC_PUBLIC)
	PHP_ME(CombatSys,  setUnitCount,         NULL, ZEND_ACC_PUBLIC)
    PHP_ME(CombatSys,  configShip,      		NULL, ZEND_ACC_PUBLIC)
    PHP_ME(CombatSys,  addAttacker,          NULL, ZEND_ACC_PUBLIC)
    PHP_ME(CombatSys,  addDefender, 		NULL, ZEND_ACC_PUBLIC)
    PHP_ME(CombatSys,  addAttackShips,  				NULL, ZEND_ACC_PUBLIC)
	PHP_ME(CombatSys,  addDefendShips,  				NULL, ZEND_ACC_PUBLIC)
	PHP_ME(CombatSys,  battle,  					NULL, ZEND_ACC_PUBLIC)
	PHP_ME(CombatSys,  getResult,  			NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

PHP_MINIT_FUNCTION(combatsystem)
{
    zend_class_entry ce;
    INIT_CLASS_ENTRY(ce, "CombatSys", cs_methods);
    combatsystem_ce = zend_register_internal_class(&ce TSRMLS_CC);

	combatsystem_ce->create_object = combatsystem_create_handler;
    memcpy(&combatsystem_object_handlers,
        zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    combatsystem_object_handlers.clone_obj = NULL;
	
    return SUCCESS;
}

zend_module_entry combatsystem_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_COMBATSYSTEM_EXTNAME,
    NULL,                  /* Functions */
    PHP_MINIT(combatsystem),
    NULL,                  /* MSHUTDOWN */
    NULL,                  /* RINIT */
    NULL,                  /* RSHUTDOWN */
    NULL,                  /* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
    PHP_COMBATSYSTEM_EXTVER,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_COMBATSYSTEM
extern "C" {
ZEND_GET_MODULE(combatsystem)
}
#endif
