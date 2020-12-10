#include "debugger_tools.hpp"
#include <unistd.h>
#include <cassert>

#ifdef __APPLE__
#include <sys/sysctl.h>
#endif



bool Tools::am_I_being_debugged()
{
#ifdef __APPLE__
    struct kinfo_proc info;
    info.kp_proc.p_flag = 0;
    int mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid() };
    auto size = sizeof(info);
    const bool ok = sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, nullptr, 0) == 0;
    assert(ok);
    return ok && (info.kp_proc.p_flag & P_TRACED) != 0;
#else
    // FIXME: implement this
    return false;
#endif
}
