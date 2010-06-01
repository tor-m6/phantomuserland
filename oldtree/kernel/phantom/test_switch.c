/**
 *
 * Phantom OS
 *
 * Copyright (C) 2005-2010 Dmitry Zavalishin, dz@dz.ru
 *
 * Test suit test selector
 *
 *
**/

#define DEBUG_MSG_PREFIX "test"
#include "debug_ext.h"
#define debug_level_flow 6
#define debug_level_error 10
#define debug_level_info 10


#include "config.h"
#include <phantom_libc.h>
#include <errno.h>
#include "misc.h"
#include "test.h"

#include "svn_version.h"


/**
 *
 * Each test must report into the stdout one of:
 *
 * FAILED
 * PASSED
 * SKIPPED - this usually follows by PASSED, though.
 *
**/




#define TEST(name) ({ if( all || (0 == strcmp( test_name, #name )) ) report( do_test_##name(test_parm), #name ); })

void report( int rc, const char *test_name )
{
    if( !rc )
    {
        printf("KERNEL TEST PASSED: %s\n", test_name );
        return;
    }

    printf("!!! KERNEL TEST FAILED: %s -> %d\n", test_name, rc );
    // todo strerror(rc)
}












void run_test( const char *test_name, const char *test_parm )
{
    int all = 0 == strcmp(test_name, "all" );

    printf("Phantom ver %s svn %s test suite\n-----\n", PHANTOM_VERSION_STR, svn_version() );

    TEST(malloc);

    TEST(udp_send);
    TEST(udp_syslog);

    printf("-----\nPhantom test suite FINISHED\n" );

}