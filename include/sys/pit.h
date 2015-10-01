// PIT ports
#define PIT_DAT 0x40 // use channel 0
#define PIT_CMD 0x43

/* Source: FreeBSD
 * Frequency of all three count-down timers; (TIMER_FREQ/freq) is the
 * appropriate count to generate a frequency of freq hz.
 *
 * Note (us):
 *   The strange frequency division is probably to mitigate
 *   floating point error propagation (check).
 */
#define TIMER_FREQ    1193182
#define TIMER_DIV(x)  ((TIMER_FREQ+(x)/2)/(x))

#define INTR_FREQ     100
#define TICKS_PER_SEC TIMER_DIV(INTR_FREQ)/INTR_FREQ

void init_pit();
