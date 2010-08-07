#define __MINGW32__

#define __W32API_MAJOR_VERSION 3
#define __W32API_MINOR_VERSION 14

#if defined(__W32API_MAJOR_VERSION) && defined(__W32API_MINOR_VERSION)
    #define wxCHECK_W32API_VERSION( major, minor ) \
 ( ( ( __W32API_MAJOR_VERSION > (major) ) \
      || ( __W32API_MAJOR_VERSION == (major) && __W32API_MINOR_VERSION >= (minor) ) ) )
#else
    #define wxCHECK_W32API_VERSION( major, minor ) (0)
#endif

#if defined(__MINGW32__) && wxCHECK_W32API_VERSION( 1, 0 )
    void fly();
#endif

#define __WATCOMC__ 50000
#define wxWATCOM_VERSION(major,minor) ( major * 100 + minor * 10 + 1100 )
#define wxCHECK_WATCOM_VERSION(major,minor) ( __WATCOMC__ >= wxWATCOM_VERSION(major,minor) )

#if ( defined( __GNUWIN32__ ) || defined( __MINGW32__ ) || \
    ( defined( __CYGWIN__ ) && defined( __WINDOWS__ ) ) || \
      wxCHECK_WATCOM_VERSION(1,0) ) && \
    !defined(__DOS__) && \
    !defined(__WXPM__) && \
    !defined(__WXMOTIF__) && \
    !defined(__WXGTK__) && \
    !defined(__WXX11__) && \
    !defined(__WXPALMOS__)
    void test();
#endif
