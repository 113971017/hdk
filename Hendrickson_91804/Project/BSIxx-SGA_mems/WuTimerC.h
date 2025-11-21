#ifndef WUTIMERC_H
#define WUTIMERC_H

// Wake up timer settings
#define CWuTimerM_4ms          4
#define CWuTimerE_4ms          0
#define CWuTimerM_8ms          8
#define CWuTimerE_8ms          0
#define CWuTimerM_100ms      100
#define CWuTimerE_100ms        0
#define CWuTimerM_200ms      100
#define CWuTimerE_200ms        0
#define CWuTimerM_1s         243
#define CWuTimerE_1s           2
#define CWuTimerM_4s         243
#define CWuTimerE_4s           4
#define CWuTimerM_5s          56
#define CWuTimerE_5s           5
#define CWuTimerM_8s         243
#define CWuTimerE_8s           5
#define CWuTimerM_10s         55
#define CWuTimerE_10s          6
#define CWuTimerM_30s        212
#define CWuTimerE_30s          7
#define CWuTimerM_max       0xFF            /* Maximal possible period */
#define CWuTimerE_max       0x0C

#define CWuTimerE_Storage CWuTimerE_30s // For debug mode 8s is OK. In real aplication this value can be bigger
#define CWuTimerM_Storage CWuTimerM_30s
#define CWuTimer_Storage  ((CWuTimerE_Storage<<8)|CWuTimerM_Storage)

#define CWuTimerE_MdPeriod CWuTimerE_1s
#define CWuTimerM_MdPeriod CWuTimerM_1s
#define CWuTimer_MdPeriod  ((CWuTimerE_MdPeriod<<8)|CWuTimerM_MdPeriod)

#define CWuTimerE_MdFilter CWuTimerE_1s
#define CWuTimerM_MdFilter CWuTimerM_1s
#define CWuTimer_MdFilter  ((CWuTimerE_MdFilter<<8)|CWuTimerM_MdFilter)

#endif	// WUTIMERC_H
