#include <cstdarg>

struct _s
{
  int   x;
  float y;
};
typedef struct _s t_s;
typedef _s (*t_ptr_s)(int a, int b);

typedef struct _s_inner
{
	int z_inner;
} t_s_inner;

typedef struct _s_outer
{
	t_s_inner z;
	void*     pVoid;
	char 			c;
} t_s_outer;

class _c
{
  int   l;
  float m;
};
typedef class _c t_c;

typedef class _c_inner
{
  int   l;
  float m;
} t_c_inner;

typedef unsigned int i_uinteger;

typedef void   t_void   (int i, const char *c, va_list v_l);
typedef void (*t_p_void)(int i, const char *c, va_list v_l);

int main (void)
{
    struct _s  s;
    struct _s& rs = s;
    struct _s* ps = &s;
    t_s        ts;
    t_s&       rts = ts;
    t_s*       pts = &ts;

//    s.
//    rs.
//    ps.
//    ps->
//    ts.
//    rts.
//    pts.
//    pts->

//    t_ptr_s(
//    t_ptr_s(3,3).

    t_s_outer  ts_outer;
    t_s_outer* pts_outer;

//    ts_outer.
//    ts_outer.z.
//    ts_outer.z->
//    ts_outer->
//    ts_outer->z.
//    ts_outer->z->
//
//    pts_outer.
//    pts_outer.z.
//    pts_outer.z->
//    pts_outer->
//    pts_outer->z.
//    pts_outer->z->

    t_c_inner  tc_inner;
    t_c_inner* ptc_inner;

//    tc_inner.
//    tc_inner->
//    ptc_inner.
//    ptc_inner->

//    i_

//    t_void(
//    t_p_void(

    return 0;
}
