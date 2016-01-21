#define MODULE_API_REVISION 2

struct rtpp_cfg_stable;
struct rtpp_module_priv;
struct rtpp_accounting;

DEFINE_METHOD(rtpp_cfg_stable, rtpp_module_ctor, struct rtpp_module_priv *);
DEFINE_METHOD(rtpp_module_priv, rtpp_module_dtor, void);
DEFINE_METHOD(rtpp_module_priv, rtpp_module_on_session_end, void,
  struct rtpp_accounting *);

#include <stdarg.h>

DEFINE_RAW_METHOD(rtpp_module_malloc, void *, size_t,  void *, const char *,
  int, const char *);
DEFINE_RAW_METHOD(rtpp_module_free, void, void *, void *, const char *, int,
  const char *);
DEFINE_RAW_METHOD(rtpp_module_realloc, void *, void *, size_t,   void *,
  const char *, int, const char *);
DEFINE_RAW_METHOD(rtpp_module_strdup, char *, const char *,  void *,
  const char *, int, const char *);
DEFINE_RAW_METHOD(rtpp_module_asprintf, int, char **, const char *,
   void *, const char *, int, const char *, ...);
DEFINE_RAW_METHOD(rtpp_module_vasprintf, int, char **, const char *,
   void *, const char *, int, const char *, va_list);

#if !defined(MODULE_IF_CODE)
#define module_malloc(n) rtpp_module.malloc((n), rtpp_module.memdeb_p, \
  __FILE__, __LINE__, __func__)
#define module_free(p) rtpp_module.free((p), rtpp_module.memdeb_p, \
  __FILE__, __LINE__, __func__)
#define module_realloc(p,n) rtpp_module.realloc((p), (n), rtpp_module.memdeb_p, \
  __FILE__, __LINE__, __func__)
#define module_strdup(p) rtpp_module.strdup((p), rtpp_module.memdeb_p, \
  __FILE__, __LINE__, __func__)
#define module_asprintf(pp, fmt, args...) rtpp_module.asprintf((pp), (fmt), \
  rtpp_module.memdeb_p, __FILE__, __LINE__, __func__, ## args)
#define module_vasprintf(pp, fmt, vl) rtpp_module.vasprintf((pp), (fmt), \
  rtpp_module.memdeb_p, __FILE__, __LINE__, __func__, (vl))
#endif

struct api_version {
    int rev;
    size_t mi_size;
};

struct rtpp_minfo {
    /* Upper half, filled by the module */
    struct api_version ver;
    const char *name;
    const char *author;
    const char *copyright;
    const char *maintainer;
    rtpp_module_ctor_t ctor;
    rtpp_module_dtor_t dtor;
    rtpp_module_on_session_end_t on_session_end;
    /* Lower half, filled by the core */
    rtpp_module_malloc_t malloc;
    rtpp_module_free_t free;
    rtpp_module_realloc_t realloc;
    rtpp_module_strdup_t strdup;
    rtpp_module_asprintf_t asprintf;
    rtpp_module_vasprintf_t vasprintf;
    void *memdeb_p;
};

#define MI_VER_INIT() {.rev = MODULE_API_REVISION, .mi_size = sizeof(rtpp_module)}
#define MI_VER_CHCK(sptr) ((sptr)->ver.rev == MODULE_API_REVISION && \
  (sptr)->ver.mi_size == sizeof(struct rtpp_minfo))
