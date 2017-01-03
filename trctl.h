/*
 * CSE-506 : Operating Systems HW2
 * Author  : Meng-Chieh Yang
 * ID      : 110452591
 * Date    : 11/1/16
 *
 * trctl.h
 * - define a TRCTL_MAGIC number.
 * - define action name for ioctl
 */

/* for EC, just uncomment the following line*/
//#define EXTRA_CREDIT

#define TRCTL_MAGIC 'm'

#define ALL _IOW(TRCTL_MAGIC, 0, int)
#define NONE _IOW(TRCTL_MAGIC, 1, int)
#define BITMAP _IOW(TRCTL_MAGIC, 2, int)
#define RETRIEVE _IOR(TRCTL_MAGIC, 3, int)

