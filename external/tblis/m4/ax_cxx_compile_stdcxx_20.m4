# ============================================================================
#  modified from
#  http://www.gnu.org/software/autoconf-archive/ax_cxx_compile_stdcxx_11.html
# ============================================================================
#
# SYNOPSIS
#
#   AX_CXX_COMPILE_STDCXX_20([ext|noext],[mandatory|optional])
#
# DESCRIPTION
#
#   Check for baseline language coverage in the compiler for the C++20
#   standard; if necessary, add switches to CXXFLAGS to enable support.
#
#   The first argument, if specified, indicates whether you insist on an
#   extended mode (e.g. -std=gnu++11) or a strict conformance mode (e.g.
#   -std=c++11).  If neither is specified, you get whatever works, with
#   preference for an extended mode.
#
#   The second argument, if specified 'mandatory' or if left unspecified,
#   indicates that baseline C++20 support is required and that the macro
#   should error out if no mode with that support is found.  If specified
#   'optional', then configuration proceeds regardless, after defining
#   HAVE_CXX20 if and only if a supporting mode is found.
#
# LICENSE
#
#   Copyright (c) 2008 Benjamin Kosnik <bkoz@redhat.com>
#   Copyright (c) 2012 Zack Weinberg <zackw@panix.com>
#   Copyright (c) 2013 Roy Stogner <roystgnr@ices.utexas.edu>
#   Copyright (c) 2014, 2015 Google Inc.; contributed by Alexey Sokolov <sokolov@google.com>
#
#   Copying and distribution of this file, with or without modification, are
#   permitted in any medium without royalty provided the copyright notice
#   and this notice are preserved. This file is offered as-is, without any
#   warranty.
#
# CHANGES
#
#   The "override" check was removed so that icc 13.0 would be considered. 
#   Upgraded from C++11 to C++20

#serial 10

m4_define([_AX_CXX_COMPILE_STDCXX_20_testbody], [[
  template <typename T>
    struct check
    {
      static_assert(sizeof(int) <= sizeof(T), "not big enough");
    };

    typedef check<check<bool>> right_angle_brackets;

    int a;
    decltype(a) b;

    typedef check<int> check_type;
    check_type c;
    check_type&& cr = static_cast<check_type&&>(c);

    auto d = a;
    auto l = [](){};
    // Prevent Clang error: unused variable 'l' [-Werror,-Wunused-variable]
    struct use_l { use_l() { l(); } };

    // http://stackoverflow.com/questions/13728184/template-aliases-and-sfinae
    // Clang 3.1 fails with headers of libstd++ 4.8.3 when using std::function because of this
    namespace test_template_alias_sfinae {
        struct foo {};

        template<typename T>
        using member = typename T::member_type;

        template<typename T>
        void func(...) {}

        template<typename T>
        void func(member<T>*) {}

        void test();

        void test() {
            func<foo>(0);
        }
    }
        
    auto test_auto() { return 0; }
    
    template <typename T> void call_l(T t) { t(0); }
    
    struct use_al { use_al() { call_l([](auto){}); } };
    
    template<typename T>
    constexpr T pi = T(3.141592653589793238462643383);
    
    int mask = 0b0101'0011'0110'0011;
]])

AC_DEFUN([AX_CXX_COMPILE_STDCXX_20], [dnl
  m4_if([$1], [], [],
        [$1], [ext], [],
        [$1], [noext], [],
        [m4_fatal([invalid argument `$1' to AX_CXX_COMPILE_STDCXX_20])])dnl
  m4_if([$2], [], [ax_cxx_compile_cxx20_required=true],
        [$2], [mandatory], [ax_cxx_compile_cxx20_required=true],
        [$2], [optional], [ax_cxx_compile_cxx20_required=false],
        [m4_fatal([invalid second argument `$2' to AX_CXX_COMPILE_STDCXX_20])])
  AC_LANG_PUSH([C++])dnl
  ac_success=no

  m4_if([$1], [noext], [], [dnl
  if test x$ac_success = xno; then
    for switch in -std=gnu++20 -std=gnu++1y; do
      cachevar=AS_TR_SH([ax_cv_cxx_compile_cxx20_$switch])
      AC_CACHE_CHECK(whether $CXX supports C++20 features with $switch,
                     $cachevar,
        [ac_save_CXXFLAGS="$CXXFLAGS"
         CXXFLAGS="$CXXFLAGS $switch"
         AC_COMPILE_IFELSE([AC_LANG_SOURCE([_AX_CXX_COMPILE_STDCXX_20_testbody])],
          [eval $cachevar=yes],
          [eval $cachevar=no])
         CXXFLAGS="$ac_save_CXXFLAGS"])
      if eval test x\$$cachevar = xyes; then
        CXXFLAGS="$CXXFLAGS $switch"
        ac_success=yes
        break
      fi
    done
  fi])

  m4_if([$1], [ext], [], [dnl
  if test x$ac_success = xno; then
    for switch in -std=c++20 -std=c++1y; do
      cachevar=AS_TR_SH([ax_cv_cxx_compile_cxx20_$switch])
      AC_CACHE_CHECK(whether $CXX supports C++20 features with $switch,
                     $cachevar,
        [ac_save_CXXFLAGS="$CXXFLAGS"
         CXXFLAGS="$CXXFLAGS $switch"
         AC_COMPILE_IFELSE([AC_LANG_SOURCE([_AX_CXX_COMPILE_STDCXX_20_testbody])],
          [eval $cachevar=yes],
          [eval $cachevar=no])
         CXXFLAGS="$ac_save_CXXFLAGS"])
      if eval test x\$$cachevar = xyes; then
        CXXFLAGS="$CXXFLAGS $switch"
        ac_success=yes
        break
      fi
    done
  fi])
  AC_LANG_POP([C++])
  if test x$ax_cxx_compile_cxx20_required = xtrue; then
    if test x$ac_success = xno; then
      AC_MSG_ERROR([*** A compiler with support for C++20 language features is required.])
    fi
  else
    if test x$ac_success = xno; then
      HAVE_CXX20=0
      AC_MSG_NOTICE([No compiler with C++20 support was found])
    else
      HAVE_CXX20=1
      AC_DEFINE(HAVE_CXX20,1,
                [define if the compiler supports basic C++20 syntax])
    fi

    AC_SUBST(HAVE_CXX20)
  fi
])
