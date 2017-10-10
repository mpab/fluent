#if defined (DEBUG_LOGGING)
    #define DP(...) printf(__VA_ARGS__)
    #define RET(val) (void)0
    //#define DP(x) do { printf(x); } while (0)
#else
    #define DP(...) (void)0
    #define RET(val) return val
    //#define DP(x) do { (void)0; } while (0)
#endif