PHP_ARG_ENABLE(combatsystem,
    [Whether to enable the "combatsystem" extension],
    [  --enable-combatsystem      Enable "combatsystem" extension support])

if test $PHP_COMBATSYSTEM != "no"; then
    PHP_REQUIRE_CXX()
    PHP_SUBST(COMBATSYSTEM_SHARED_LIBADD)
    PHP_ADD_LIBRARY(stdc++, 1, COMBATSYSTEM_SHARED_LIBADD)
    PHP_NEW_EXTENSION(combatsystem, cs_extension.cc, $ext_shared)
fi
