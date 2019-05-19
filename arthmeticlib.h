#include <stdlib.h>
#include <stddef.h>
struct any_BIAS
{
	unsigned char * digits;
	unsigned long length;
};
typedef struct any_BIAS any_BIAS;
typedef any_BIAS * any_BIAS_p;

int any_BIAS_FromString(any_BIAS_p * i, unsigned char * s);
int any_ADD_BIAS(any_BIAS_p a, any_BIAS_p b);
int any_SUB_BIAS(any_BIAS_p a, any_BIAS_p b);
int any_DIV_BIAS(any_BIAS_p a, any_BIAS_p b);
int any_MUL_BIAS(any_BIAS_p a, any_BIAS_p b);
int any_MULLONG_BIAS(any_BIAS_p a, any_BIAS_p b,int acomp, int bcomp);
int any_DIVLONG_BIAS(any_BIAS_p a, any_BIAS_p b,int acomp, int bcomp);

int any_RussianMul_BIAS(any_BIAS_p a, any_BIAS_p b,int acomp, int bcomp);

int any_BIAS_Compare(const any_BIAS_p a, const any_BIAS_p b);
int any_BIAS_Duplicate (any_BIAS_p * a, const any_BIAS_p b);
int any_BIAS_Assign (any_BIAS_p a, any_BIAS_p b);
void any_BIAS_output (any_BIAS_p a);
void any_BIAS_Free (any_BIAS_p * i);
