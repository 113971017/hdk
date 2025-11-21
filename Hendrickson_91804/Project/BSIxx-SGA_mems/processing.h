#ifndef PROCESSING_H
#define PROCESSING_H
#include "datatypes.h"

extern void TPMS_state_Measurements(void);
extern void TPMS_state_Motion_Detection(void);
extern void TPMS_state_processing_and_update(void);
extern void TPMS_state_processing_end(void);
extern void TPMS_state_Init(tpms_state_t state);


//extern RODATA_MEM tpms_state_property_t TPMS_STATE_PARAMETERS[STATE_TOTAL_NUMBER];
extern const tpms_state_property_t TPMS_STATE_PARAMETERS[STATE_TOTAL_NUMBER];


void update_measured_rec(void);




// Bit masks and shift positions
#define NORMAL_TO_STANDBY_MASK      0x000F  // Bits 0-3
#define NORMAL_TO_MONITORING_MASK   0x00F0  // Bits 4-7
#define MONITORING_TO_NORMAL_MASK   0x0F00  // Bits 8-11

#define NORMAL_TO_STANDBY_SHIFT     0
#define NORMAL_TO_MONITORING_SHIFT  4
#define MONITORING_TO_NORMAL_SHIFT  8

// Maximum values for each counter
#define NORMAL_TO_STANDBY_MAX       10
#define NORMAL_TO_MONITORING_MAX    15
#define MONITORING_TO_NORMAL_MAX    15



// Extract individual counters
#define GET_NORMAL_TO_STANDBY(x) \
    ((x & NORMAL_TO_STANDBY_MASK) >> NORMAL_TO_STANDBY_SHIFT)

#define GET_NORMAL_TO_MONITORING(x) \
    ((x & NORMAL_TO_MONITORING_MASK) >> NORMAL_TO_MONITORING_SHIFT)

#define GET_MONITORING_TO_NORMAL(x) \
    ((x & MONITORING_TO_NORMAL_MASK) >> MONITORING_TO_NORMAL_SHIFT)

// Set individual counters
#define SET_NORMAL_TO_STANDBY(x, val) \
    x = (x & ~NORMAL_TO_STANDBY_MASK) | ((val & 0xF) << NORMAL_TO_STANDBY_SHIFT)

#define SET_NORMAL_TO_MONITORING(x, val) \
    x = (x & ~NORMAL_TO_MONITORING_MASK) | ((val & 0xF) << NORMAL_TO_MONITORING_SHIFT)

#define SET_MONITORING_TO_NORMAL(x, val) \
    x = (x & ~MONITORING_TO_NORMAL_MASK) | ((val & 0xF) << MONITORING_TO_NORMAL_SHIFT)



// Increment NormalToStandbyMode_Counter
void IncrementNormalToStandby(void);

// Increment NormalToMonitoringMode_Counter
void IncrementNormalToMonitoring(void);

// Increment MonitoringToNormalMode_Counter
void IncrementMonitoringToNormal(void);

void queue_init(CircularEventQueue *queue);


// Function Prototypes
void queue_init(CircularEventQueue *queue);
bool queue_is_full(const CircularEventQueue *queue);
bool queue_is_empty(const CircularEventQueue *queue);
bool queue_enqueue(CircularEventQueue *queue, uint8_t event);
bool queue_dequeue(CircularEventQueue *queue, uint8_t *event);
bool queue_peek(const CircularEventQueue *queue, uint8_t *event);



/* ----------------------------------------------------------------------------
 */
#endif /* PROCESSING_H */
