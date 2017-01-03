/*
 * CSE-506 : Operating Systems HW2
 * Author  : Meng-Chieh Yang
 * ID      : 110452591
 * Date    : 11/1/16
 * 
 * record.h:
 * - define record structure
 * - define operation message number
 * - define MARCRO for checking corresponding bit of BITMAP for operation.
 */

/* record structure */
struct record{
	unsigned int r_id;
	unsigned short r_size;
	unsigned char r_type;
	int r_rval;
	unsigned short r_path_len;
	char* r_path_name;
};

/* operation messages */

#define OP_OPEN   0
#define OP_CLOSE  1
#define OP_READ	  2
#define OP_WRITE  3
#define OP_CREATE 4
#define OP_REMOVE 5


/* for bitmapping, we just check the bit that corresponding to operation number*/
#define CHECK_BIT(bitmap, op_num) ((bitmap)&(1<<(op_num)))