#include "arthmeticlib.h"
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

//assembly prototypes
void anyAddWithCarry(unsigned char * a, unsigned char * b, unsigned char * sum, int a_length, int b_length, int sum_q);
void anyAddWithCarryRestPositions(unsigned char * a, unsigned char * sum, int sum_q);
void anySubWithBorrowRestPositions(unsigned char * a, unsigned char * sum, int sum_q);
void anySubWithBorrow(unsigned char * a, unsigned char * b, unsigned char * sum, int a_length, int b_length, int sum_q);

//##########################################ADDITION###############################################
int any_Add (any_BIAS_p a,  any_BIAS_p b){//function add two any_int_p values. modify the first one (a+=b)
	if ((a == NULL) || (b == NULL))
		return 1; // error
	if ((a->digits == NULL) || (b->digits == NULL))
		return 1; //  error

		long sum_length = max(a->length, b->length) + 1;
		unsigned char * sum_digits = (unsigned char *) malloc(sum_length+1); // dont forget '\0'
		long k;

		for (k = 0; k < sum_length; k++){// initialize sum_digits to '0's
			*(sum_digits + k) = '0';
		}
		*(sum_digits+sum_length)='\0';
		long iterations = sum_length-1;
		anyAddWithCarry(a->digits,b->digits,sum_digits,a->length-1,b->length-1,iterations);

		if(a->length!=b->length){
			int iterations=0;
			if(a->length<b->length){
				iterations=b->length-a->length-1;
				anyAddWithCarryRestPositions(b->digits,sum_digits+1,iterations);
			}
			else{
				iterations=a->length-b->length-1;
				anyAddWithCarryRestPositions(a->digits,sum_digits+1,iterations);
			}			
		}
	
	long i;
		i = 0;
		while ((*(sum_digits + i) == '0') && (i < sum_length - 1)){
			i++;
		}
		a->length = sum_length - i;
		free(a->digits);
		a->digits = (unsigned char *) malloc(a->length + 1);
		memcpy(a->digits, sum_digits + i, a->length + 1);
		free(sum_digits);		
   	return 0;
}
//#######################################################SUBSTRACTION#############################################
int any_Sub (any_BIAS_p a, any_BIAS_p b){//substract two any_int_p values. modify first one (x-=y)
	if ((a == NULL) || (b == NULL))
		return 1; // error
	if ((a->digits == NULL) || (b->digits == NULL))
		return 1; // error
		
			long difference_length = a->length;
			long iterations=0;
			unsigned char * difference_digits = (unsigned char *) malloc(difference_length + 1); 
			long k;
			for (k = 0; k < difference_length; k++){ // initialize difference_digits to '0's
				*(difference_digits + k) = '0';
			}
			*(difference_digits + difference_length) = '\0';
		iterations=difference_length-1;
		anySubWithBorrow(a->digits,b->digits,difference_digits,a->length-1,b->length-1,iterations);

		if(a->length!=b->length){
			if(a->length<b->length){
				iterations=b->length-a->length-1;
				anySubWithBorrowRestPositions(b->digits,difference_digits,iterations);
			}
			else{
				iterations=a->length-b->length-1;
				anySubWithBorrowRestPositions(a->digits,difference_digits,iterations);
			}			
		}
			//a->length = difference_length;
			free(a->digits);
			a->digits = (unsigned char *) malloc(a->length + 1);
			memcpy(a->digits, difference_digits, a->length + 1);
			free(difference_digits);
	
return 0;
}
//##########################################################BIAS METHODS ############################################
void any_BIAS_output (any_BIAS_p a){//function printf given any_BIAS_p value
		printf("%s\n", a->digits);	
}
void any_BIAS_Free (any_BIAS_p * i){//deallocate memory allocated for i value
	free((*i)->digits);
	free((*i));
}
static void any_BIAS_HeadZerosRemove (any_BIAS_p a){// function remove the heading zeros and update length
	any_BIAS_p z;
	any_BIAS_Duplicate(&z, a);
	unsigned long i = 0;
	while ((*(z->digits + i) == '0') && (i < z->length - 1)){
		i ++;
	}
	if (i != 0){
		a->length = a->length - i;
		free(a->digits);
		a->digits = (unsigned char *) malloc(a->length + 1);
		*(a->digits + a->length) = '\0';
		memcpy(a->digits, z->digits + i, a->length + 1);
	}
	any_BIAS_Free(&z);
}
static int any_BIAS_CompareLength(const any_BIAS_p a, const any_BIAS_p b){//function compare lengths of two any_BIAS_p return 1 if a>b and 0 if a<b and -1 if equal
	if(a->length > b->length)
		return 1;
	else if(a->length < b->length)
		return 0;
	return -1;
}
int any_BIAS_Compare(const any_BIAS_p a, const any_BIAS_p b){
	if(any_BIAS_CompareLength(a, b)==1)
		return 1;
	if(any_BIAS_CompareLength(a, b)==0)
		return 0;
	unsigned long m;
			for (m = 0; m < a->length; m++){//loop check evry digit of numbers to find greater and set sign
				if (*(a->digits + m) > *(b->digits + m))
					return 1;
				if (*(a->digits + m) < *(b->digits + m))
					return 0;
			}
	return -1;
}
int any_BIAS_Duplicate (any_BIAS_p * a, const any_BIAS_p b){//function assign value of b to a.
if (b->digits == NULL)
		return 1; // error 
	(*a) = (any_BIAS_p) malloc(sizeof(any_BIAS));
	(*a)->length = b->length;

	(*a)->digits = (unsigned char *) malloc((*a)->length+1);
	memcpy((*a)->digits, b->digits, (*a)->length+1);

	return 0;
}
int any_BIAS_Assign (any_BIAS_p a, any_BIAS_p b){//function assign value of b to a.
	if ((a->digits == NULL) || (b->digits == NULL))
		return 1; // error

	a->length = b->length;
	free(a->digits);
	a->digits = (unsigned char *) malloc(a->length + 1);
	memcpy(a->digits, b->digits, a->length + 1);
	return 0;
}
static int any_Generate_BIAS(any_BIAS_p * new, const any_BIAS_p original){
	if (original->digits == NULL){
		return 1; // error 
	}
	(*new) = (any_BIAS_p) malloc(sizeof(any_BIAS));
	(*new)->length = original->length;

	(*new)->digits = (unsigned char *) malloc((*new)->length+1);

unsigned long j=0;
		(*((*new)->digits+j)) ='4';

	for (j = 1; j <original->length; j++)
		(*((*new)->digits+j))='9';	
	(*new)->digits[j]='\0';
return 0;
}
int any_BIAS_FromString (any_BIAS_p * i, unsigned char * s){//convert from string to any_int_p and assign value to *i
	if (s == NULL)
		return 1; // error

	unsigned long length = (unsigned long) (strlen((const char*)s));

	if (length == 0)
		return 1; // error

	(*i) = (any_BIAS_p) malloc(sizeof(any_BIAS));

	if ((*s >= '0') && (*s <= '9')){
		// Format check
		unsigned long j;
		for (j = 0; j < length; j ++){
			if ((*(s+j) < '0') || (*(s+j) > '9'))
				return 1; // error
		}
		(*i)->length = length;
		(*i)->digits = (unsigned char *) malloc(length+1);
		memcpy((*i)->digits, s , length+1);
		return 0;
	}
	else
		return 1; // error
}
void any_BIAS_Extend(int compare, any_BIAS_p a, any_BIAS_p b, any_BIAS_p a_BIAS, any_BIAS_p b_BIAS){
any_BIAS_p new__BIAS_value;
		if(compare==1){//a is longer
			any_Generate_BIAS(&new__BIAS_value, a);
			any_Sub(new__BIAS_value,b_BIAS);
			any_Add(b,new__BIAS_value);//increase b of difference in bias value
		}
		else{//b is longer
			any_Generate_BIAS(&new__BIAS_value,b);
			any_Sub(new__BIAS_value,a_BIAS);
			any_Add(a,new__BIAS_value);//increase a of difference in bias value
		}	
any_BIAS_Free(&new__BIAS_value);
}
void AssignWithZerosFromRight(any_BIAS_p a, unsigned long pos){//function assigns pos numbers of 0 in a number
	unsigned char * zeros = (unsigned char *) malloc(a->length+pos+1);
	unsigned char * copy = (unsigned char *) malloc(a->length+pos+1);

	unsigned long j=0;

	for (j = 0; j <pos; j++)
		zeros[j]='0';	
	zeros[pos]='\0';

	memcpy(copy, a->digits, a->length + 1);
	(unsigned char *)strcat((char *)copy,(char *)zeros);

	free(a->digits);
	a->digits= (unsigned char *) malloc(strlen((const char*)copy)+1);
	a->length+=strlen((const char*)zeros);
	memcpy(a->digits,copy , a->length + 1);
	a->digits[a->length]='\0';
	free(zeros);
	free(copy);
}
void DeleteFromRight(any_BIAS_p a, unsigned long pos){//function delete pos number from a 
	a->digits[a->length-pos]='\0';
	a->length=a->length-pos;
}
//####################################################ADDITION######################################################
int any_ADD_BIAS(any_BIAS_p a, any_BIAS_p b)
{
any_BIAS_p a_BIAS;
any_BIAS_p b_BIAS;

any_Generate_BIAS(&a_BIAS, a);//generate bias for a and b
any_Generate_BIAS(&b_BIAS, b);

int compare=any_BIAS_CompareLength(a,b);//check which of a and b is greater

if(compare!=-1)//if numbers are different lengths we need to equalize bias
	any_BIAS_Extend(compare,a,b,a_BIAS,b_BIAS);

any_Add(a,b);//add two numbers

if(any_BIAS_Compare(a,a_BIAS)!=0 || any_BIAS_Compare(a,b_BIAS)!=0){
	if(any_BIAS_CompareLength(b_BIAS,a_BIAS)==0){
		if(any_BIAS_Compare(a,a_BIAS)!=0)
			any_Sub(a,a_BIAS);
		else{
			b->length++;
			any_BIAS_Extend(0,a,b,a_BIAS,b_BIAS);
			any_Sub(a,a_BIAS);
		}
	}
	else{
		if(any_BIAS_Compare(a,b_BIAS)!=0)
			any_Sub(a,b_BIAS);
		else{
			a->length++;
			any_BIAS_Extend(1,a,b,a_BIAS,b_BIAS);
			any_Sub(b,a_BIAS);
			any_BIAS_Assign(a,b);
		}
	}
}
else{
	b->length++;
	any_BIAS_Extend(0,a,b,a_BIAS,b_BIAS);
	any_Sub(a,a_BIAS);
}
any_BIAS_HeadZerosRemove(a);

compare=any_BIAS_CompareLength(a,b);//check length of a and b again
	if(compare!=-1){//if numbers are different lengths
			if(compare==1){//a is longer
			any_BIAS_p new__BIAS_value;
			any_Generate_BIAS(&new__BIAS_value, a);
			if(any_BIAS_CompareLength(b_BIAS,a_BIAS)==0){
				any_Sub(new__BIAS_value,a_BIAS);
			}
			else{
				any_Sub(new__BIAS_value,b_BIAS);
			}
			any_Add(a,new__BIAS_value);
			any_BIAS_Free(&new__BIAS_value);
		}
	}

any_BIAS_Free(&a_BIAS);
any_BIAS_Free(&b_BIAS);
return 0;
}
//####################################################SUBSTRACTION#####################################################
int any_SUB_BIAS(any_BIAS_p a, any_BIAS_p b){
any_BIAS_p a_BIAS;
any_BIAS_p b_BIAS;

int resultIsNegative=0;//check if result is negative

any_Generate_BIAS(&a_BIAS, a);
any_Generate_BIAS(&b_BIAS, b);

int compare=any_BIAS_CompareLength(a,b);

if(compare!=-1)//if numbers are different length we need to convert shorter one to longer one format
	any_BIAS_Extend(compare,a,b,a_BIAS,b_BIAS);

compare=any_BIAS_Compare(a,b);
if(compare==0){//b greater than a
	any_Sub(b,a);
	any_BIAS_Assign(a,b);
	resultIsNegative=1;//result is negative
}
else
	any_Sub(a,b);

any_BIAS_HeadZerosRemove(a);
any_BIAS_HeadZerosRemove(b);

any_BIAS_p new;
any_BIAS_HeadZerosRemove(a);//remove heading zeros from a
any_Generate_BIAS(&new, a);
compare=any_BIAS_Compare(a,new);

if(compare==1){//a is greater than bias we need to generate longer one
	any_BIAS_p new_BIAS;	
	a->length++;
	any_Generate_BIAS(&new_BIAS, a);
	a->length--;
	if(resultIsNegative==0)
		any_Add(a,new_BIAS);
	else{
		any_Sub(new_BIAS,a);
		any_BIAS_Assign(a,new_BIAS);	
	}
		
	any_BIAS_Free(&new_BIAS);
}
else{
	compare=any_BIAS_CompareLength(a_BIAS,b_BIAS);//compare length's to chose proper bias
	if(compare==1)
		if(resultIsNegative==0)
			any_Add(a,a_BIAS);
		else{
			any_Sub(a_BIAS,a);
			any_BIAS_Assign(a,a_BIAS);	
		}
	else		
		if(resultIsNegative==0)
			any_Add(a,b_BIAS);
		else{
			any_Sub(b_BIAS,a);
			any_BIAS_Assign(a,b_BIAS);	
		}
}

any_BIAS_Free(&new);	
any_BIAS_Free(&a_BIAS);
any_BIAS_Free(&b_BIAS);
return 0;
}
void CodeMulResult(any_BIAS_p a, any_BIAS_p one, int acomp ,int bcomp){//function code multiply result in bias
	int compare;
	any_BIAS_p compare_BIAS;
	
	if((acomp==1 && bcomp==1)||(acomp==0 && bcomp==0)){//result of multiplication is positive number
	any_Generate_BIAS(&compare_BIAS, a);
	any_Add(compare_BIAS, one);
	compare=any_BIAS_Compare(a,compare_BIAS);
	any_BIAS_p new_BIAS;
		if(compare==1){//a is greater than bias
			a->length++;
			any_Generate_BIAS(&new_BIAS, a);
			a->length--;
			any_Add(a,new_BIAS);
		}
		else{
			any_Generate_BIAS(&new_BIAS, a);
			any_Add(a,new_BIAS);
		}
			any_BIAS_Free(&new_BIAS);
	}
	else{//result is negative number
		any_Generate_BIAS(&compare_BIAS, a);
		any_Add(compare_BIAS, one);
		compare=any_BIAS_Compare(a,compare_BIAS);
		any_BIAS_p new_BIAS;
		if(compare==1|| compare == -1){//a is greater than bias
			a->length++;
			any_Generate_BIAS(&new_BIAS, a);//generate longer bias
			a->length--;
			any_Sub(new_BIAS,a);//substract result from bias
			any_BIAS_Assign(a,new_BIAS);
		}
		else{	
			any_Generate_BIAS(&new_BIAS, a);
			any_Sub(new_BIAS,a);
			any_BIAS_Assign(a,new_BIAS);
		}	
			any_BIAS_Free(&new_BIAS);
	}
any_BIAS_Free(&compare_BIAS);
}
//#####################################################MULTIPLICATION#################################################
int any_MUL_BIAS(any_BIAS_p a, any_BIAS_p b){
	any_BIAS_p a_BIAS;
	any_BIAS_p b_BIAS;

	any_BIAS_p original;//variables for loop realization
	any_BIAS_p zero;
	any_Generate_BIAS(&a_BIAS, a);
	any_Generate_BIAS(&b_BIAS, b);

	int a_compare=any_BIAS_Compare(a,a_BIAS);
	int b_compare=any_BIAS_Compare(b,b_BIAS);

	if(a_compare==1)//if a is positive value
		any_Sub(a,a_BIAS);
	else{//a is negative value
		any_Sub(a_BIAS,a);	
		any_BIAS_Assign(a,a_BIAS);
	}	
	if(b_compare==1)//if b is positive value
		any_Sub(b,b_BIAS);
	else{//b is negative value
		any_Sub(b_BIAS,b);	
		any_BIAS_Assign(b,b_BIAS);
	}

	any_BIAS_HeadZerosRemove(a);
	any_BIAS_HeadZerosRemove(b);

		if(a->length >2 && b->length >2){
			any_BIAS_Free(&a_BIAS);
			any_BIAS_Free(&b_BIAS);
			any_MULLONG_BIAS(a,b,a_compare,b_compare);
			//any_RussianMul_BIAS(a,b,a_compare,b_compare);
			CodeMulResult(a,b,a_compare,b_compare);
			return 0;
		}

	int compare=any_BIAS_Compare(a,b);

	any_BIAS_p counter;
	any_BIAS_p one;

	any_BIAS_FromString(&counter,(unsigned char *)"1");//assign counter to 1
	any_BIAS_FromString(&one,(unsigned char *)"1");
	int x=0;
	if(compare==1){//if a is greater than b
		any_BIAS_Duplicate(&original,a);//duplicate original value
		while(any_BIAS_Compare(b,counter)==1){
			any_Add(a,original);
			any_Add(counter,one);
			x=1;
		}
		if(x==0 && any_BIAS_Compare(b,counter) != -1){
			any_BIAS_FromString(&zero,(unsigned char *)"0");
			any_BIAS_Assign(a,zero);
			any_BIAS_Free(&zero);
		}
	}
	else{//a is less than b
		any_BIAS_Duplicate(&original,b);//duplicate original value
		while(any_BIAS_Compare(a,counter)==1){//add values while counter is less than a
			any_Add(b,original);
			any_Add(counter,one);
			x=1;
		}
		if(x==0 && any_BIAS_Compare(a,counter) !=-1){//if result is zero
			any_BIAS_FromString(&zero,(unsigned char *)"0");
			any_BIAS_Assign(a,zero);
			any_BIAS_Free(&zero);
		}
		else//else assign b to a and its our result
			any_BIAS_Assign(a,b);
	}

	CodeMulResult(a,b,a_compare,b_compare);

	any_BIAS_Free(&a_BIAS);
	any_BIAS_Free(&b_BIAS);
	any_BIAS_Free(&original);
	any_BIAS_Free(&counter);
	any_BIAS_Free(&one);
	return 0;
}
//#################################################LONG MULTIPLICATION#############################################
int any_MULLONG_BIAS(any_BIAS_p a, any_BIAS_p b,int acomp, int bcomp){
	any_BIAS_p result;
	any_BIAS_FromString(&result,(unsigned char *)"0");//assign result to 0

	long i=a->length-1;

	while(i>=0){
		if(a->digits[i]!='0'){
			any_BIAS_p partialResult;
			any_BIAS_FromString(&partialResult,(unsigned char *)"0");//assign partial result to 0

			int  c=a->digits[i]-'0';
			int x=0;
			while(c>x){
				any_Add(partialResult,b);
				x++;
			}
			AssignWithZerosFromRight(partialResult,a->length-i-1);
			any_Add(result,partialResult);
			any_BIAS_Free(&partialResult);
		}
		i--;
	}
	any_BIAS_Assign(a,result);	
	any_BIAS_Free(&result);
	return 0;
}
//#########################################Russian ALGORITHM################################################
int any_RussianMul_BIAS(any_BIAS_p a, any_BIAS_p b,int acomp, int bcomp){
	any_BIAS_p result;
	any_BIAS_p divisor;
	any_BIAS_p one;

	any_BIAS_FromString(&result,(unsigned char *)"0");//assign result to 0
	any_BIAS_FromString(&divisor,(unsigned char *)"2");//assign divisor to 2
	any_BIAS_FromString(&one,(unsigned char *)"1");//assign divisor to 2

	while(any_BIAS_Compare(a,one)!=-1){
		if(a->digits[a->length-1]=='1' || a->digits[a->length-1]=='3' || a->digits[a->length-1]=='5' || a->digits[a->length-1]=='7' || a->digits[a->length-1]=='9'){
			any_Add(result,b);
		}
			any_DIVLONG_BIAS(a,divisor,acomp,1);
			any_BIAS_FromString(&divisor,(unsigned char *)"2");//assign divisor to 2
			any_MULLONG_BIAS(b,divisor,bcomp,1);
	}
	any_BIAS_Assign(a,result);
	CodeMulResult(a,b,acomp,bcomp);
	any_BIAS_Free(&result);
	any_BIAS_Free(&divisor);
	any_BIAS_Free(&one);
	return 0;
}
void CodeDivisionResult(any_BIAS_p a, any_BIAS_p one, int acomp ,int bcomp){//function code division result in bias
	int compare;
	if((acomp==1 && bcomp==1)||(acomp==0 && bcomp==0)){//result of division is positive number
	any_BIAS_p compare_BIAS;
	any_Generate_BIAS(&compare_BIAS, a);
	any_Add(compare_BIAS, one);
	compare=any_BIAS_Compare(a,compare_BIAS);

		if(compare==1){//a is greater than bias
			any_BIAS_p new_BIAS;	
			a->length++;
			any_Generate_BIAS(&new_BIAS, a);
			a->length--;
			any_Add(a,new_BIAS);
			any_BIAS_Free(&new_BIAS);
		}
		else{
			any_BIAS_p new_BIAS;	
			any_Generate_BIAS(&new_BIAS, a);
			any_Add(a,new_BIAS);
			any_BIAS_Free(&new_BIAS);
		}
		any_BIAS_Free(&compare_BIAS);
	}
	else{//result is negative number
		any_BIAS_p compare_BIAS;
		any_Generate_BIAS(&compare_BIAS, a);
		any_Add(compare_BIAS, one);
		compare=any_BIAS_Compare(a,compare_BIAS);

		if(compare==1){//a is greater than bias
			any_BIAS_p new_BIAS;	
			a->length++;
			any_Generate_BIAS(&new_BIAS, a);//generate longer bias
			a->length--;
			any_Sub(new_BIAS,a);//substract result from bias
			any_BIAS_Assign(a,new_BIAS);
			any_BIAS_Free(&new_BIAS);
		}
		else{
			any_BIAS_p new_BIAS;	
			any_Generate_BIAS(&new_BIAS, a);
			any_Sub(new_BIAS,a);
			any_BIAS_Assign(a,new_BIAS);
			any_BIAS_Free(&new_BIAS);
		}	
		any_BIAS_Free(&compare_BIAS);
	}
}
//#####################################################DIVISION#################################################
int any_DIV_BIAS(any_BIAS_p a, any_BIAS_p b){
	any_BIAS_p a_BIAS;
	any_BIAS_p b_BIAS;

	//variables for loop realization
	any_BIAS_p counter;
	any_BIAS_p one;

	any_Generate_BIAS(&a_BIAS, a);
	any_Generate_BIAS(&b_BIAS, b);

	int a_compare=any_BIAS_Compare(a,a_BIAS);
	int b_compare=any_BIAS_Compare(b,b_BIAS);

	if(a_compare==1)//if a is positive value
		any_Sub(a,a_BIAS);
	else{//a is negative value
		any_Sub(a_BIAS,a);	
		any_BIAS_Assign(a,a_BIAS);
	}
		
	if(b_compare==1)//if b is positive value
		any_Sub(b,b_BIAS);
	else{//b is negative value
		any_Sub(b_BIAS,b);	
		any_BIAS_Assign(b,b_BIAS);
	}

	any_BIAS_HeadZerosRemove(a);
	any_BIAS_HeadZerosRemove(b);

		if(a->length >2 && b->length >2){//if numbers are longer than 2 digits we use faster method
			any_BIAS_Free(&a_BIAS);
			any_BIAS_Free(&b_BIAS);
			any_DIVLONG_BIAS(a,b,a_compare,b_compare);
			any_BIAS_FromString(&one,(unsigned char *)"1");
			CodeDivisionResult(a,one,a_compare,b_compare);
			any_BIAS_Free(&one);
			return 0;
		}

	int compare=any_BIAS_Compare(a,b);

	if(compare == -1){//if numbers have same value result if 1 or -1
		any_BIAS_p result;
		if((a_compare==1 && b_compare==1)||(a_compare==0 && b_compare==0)){
			any_BIAS_FromString(&result,(unsigned char *)"5");
			any_BIAS_Assign(a,result);
		}
		else{
			any_BIAS_FromString(&result,(unsigned char *)"3");
			any_BIAS_Assign(a,result);		
		}
		any_BIAS_Free(&result);
		any_BIAS_Free(&a_BIAS);
		any_BIAS_Free(&b_BIAS);
		return 0;
	}

	any_BIAS_FromString(&counter,(unsigned char *)"0");//assign counter to 0
	any_BIAS_FromString(&one,(unsigned char *)"1");

	if(compare==1){//if a is greater than b
		any_BIAS_p original;
		any_BIAS_Duplicate(&original,b);//duplicate original value
		while(any_BIAS_Compare(a,b)!=0){
			any_Sub(a,original);
			any_BIAS_HeadZerosRemove(a);
			any_Add(counter,one);//increase counter
		}
		any_BIAS_Assign(a,counter);
	any_BIAS_Free(&original);
	}
	else if(compare==0){
		any_BIAS_p original;
		any_BIAS_Duplicate(&original,a);
		while(any_BIAS_Compare(a,b)!=0){
			any_Sub(b,original);
			any_BIAS_HeadZerosRemove(b);
			any_Add(counter,one);
		}
		any_BIAS_Assign(a,counter);
	any_BIAS_Free(&original);
	}

	CodeDivisionResult(a,one,a_compare,b_compare);

	any_BIAS_Free(&a_BIAS);
	any_BIAS_Free(&b_BIAS);
	any_BIAS_Free(&counter);
	any_BIAS_Free(&one);
		return 0;
}
//#################################################LONG DIVISION#############################################
int any_DIVLONG_BIAS(any_BIAS_p a, any_BIAS_p b,int acomp, int bcomp){
	any_BIAS_p counter;
	any_BIAS_p one;
	unsigned long originalLength = b->length;//set original b length
	
	any_BIAS_FromString(&counter,(unsigned char *)"0");//set counter value
	any_BIAS_FromString(&one,(unsigned char *)"1");

	if(any_BIAS_CompareLength(a,b)==1){//if a is longer than b we assign b with 0 from right side. number of 0 = a length - b length
		AssignWithZerosFromRight(b,a->length-b->length);
	}
	if(any_BIAS_CompareLength(a,b)==1){
		DeleteFromRight(b,1);
	}

	while(1){	
		while(any_BIAS_Compare(a,b)!=0){//while b is not greater than a
			any_Sub(a,b);
			any_Add(counter,one);//increase counter
			any_BIAS_HeadZerosRemove(a);
		}
		if(b->length>1)
			DeleteFromRight(b,1);
		if(b->length<originalLength || b->length == 1)
			break;
		else
			AssignWithZerosFromRight(counter,1);
		
	}
	any_BIAS_Assign(a,counter);//assign result to a
	any_BIAS_Free(&counter);
	any_BIAS_Free(&one);
	return 0;
}
