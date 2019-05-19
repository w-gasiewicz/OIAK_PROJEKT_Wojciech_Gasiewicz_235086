#include <stdio.h>
#include <string.h>
#include <CUnit/CUnit.h>
#include "CUnit/Basic.h"
#include "arthmeticlib.h"
#include <time.h>
#include <stdlib.h>
    const char num[10]={'0','1','2','3','4','5','6','7','8','9'};

    static unsigned char input1[][15] ={"0","0","9","502","99","5673","4851"}; 
    static unsigned char input2[][15] ={"0","0","0","505","0","351","5673"};
    static unsigned const char outputADD[][15] ={"41","41","5","508","95"}; 
    static const char outputSUB[][9] ={"4","4","58","496","553","5821","4177"};
    static const char outputMUL[][14] ={"65","65","29","67","299"};
    static const char outputDIV[][9] ={"5","5","3","4","37","9"}; 

//function generate random goven length string from digits
unsigned char * genRandom(unsigned char * s, unsigned long len){
	s=malloc (sizeof(unsigned char)*(len+1));
    for (unsigned long i = 0; i < len; i++){
        s[i] = num[rand() % (int)(sizeof(num) - 1)];
    }
    s[len]='\0';
    return s;
}
void Test_Any_AddBIAS(void){
	for(int i=0;i<5;i++){
    any_BIAS_p a;
    any_BIAS_p b;
		any_BIAS_FromString(&a,(unsigned char *) input1[i]);
		any_BIAS_FromString(&b,(unsigned char *) input2[i]);
      any_ADD_BIAS(a,b);

		CU_ASSERT_EQUAL(strcmp((const char*)a->digits,(const char*)outputADD[i]),0);
		if ((strcmp((const char*)a->digits,(const char*)outputADD[i])!=0))
			printf("\n should be: %s is:%s \n",outputADD[i],a->digits);
    any_BIAS_Free(&a);
    any_BIAS_Free(&b);
	}
}
void Test_Any_AddBIASLong(){
    any_BIAS_p a;
    any_BIAS_p b;
    clock_t start,end;
    double time;
    unsigned char * s1=NULL;
    unsigned char * s2=NULL;
    s1=genRandom(s1,1000000000);
    s2=genRandom(s2,1000000000);
	
    	any_BIAS_FromString(&a, s1);
		any_BIAS_FromString(&b, s2);

        start=clock();//start measuring time
      if(any_ADD_BIAS(a,b)!=0)
        printf("failure \n");   

        end = clock();// stop measuring time
        time=((double)(end-start))/(CLOCKS_PER_SEC);
        printf("Test_Any_Add_BIASLong taken: %f [s] to execute. \n",time);

    free(s1);
    free(s2);
    any_BIAS_Free(&a);
    any_BIAS_Free(&b);
}
void Test_Any_SubBIAS(void){
	for(int i=0;i<7;i++){
    any_BIAS_p a;
    any_BIAS_p b;
		any_BIAS_FromString(&a,(unsigned char *) input1[i]);
		any_BIAS_FromString(&b,(unsigned char *) input2[i]);
      any_SUB_BIAS(a,b);
		CU_ASSERT_EQUAL(strcmp((const char*)a->digits,(const char*)outputSUB[i]),0);
		if ((strcmp((const char*)a->digits,(const char*)outputSUB[i])!=0))
			printf("\n should be: %s is:%s \n",outputSUB[i],a->digits);
    any_BIAS_Free(&a);
    any_BIAS_Free(&b);
	}
}
void Test_Any_SubBIASLong(){
    any_BIAS_p a;
    any_BIAS_p b;
    clock_t start,end;
    double time;
    unsigned char * s1=NULL;
    unsigned char * s2=NULL;
    s1=genRandom(s1,100000000);
    s2=genRandom(s2,100000000);
	
    	any_BIAS_FromString(&a, s1);
		any_BIAS_FromString(&b, s2);

        start=clock();//start measuring time
      if(any_SUB_BIAS(a,b)!=0)
        printf("failure \n");   

        end = clock();// stop measuring time
        time=((double)(end-start))/(CLOCKS_PER_SEC);
        printf("Test_Any_Sub_BIASLong taken: %f [s] to execute. \n",time);

    free(s1);
    free(s2);
    any_BIAS_Free(&a);
    any_BIAS_Free(&b);
}
void Test_Any_MulBIAS(void){
	for(int i=0;i<5;i++){
    any_BIAS_p a;
    any_BIAS_p b;
		any_BIAS_FromString(&a,(unsigned char *) input1[i]);
		any_BIAS_FromString(&b,(unsigned char *) input2[i]);
      any_MUL_BIAS(a,b);
		CU_ASSERT_EQUAL(strcmp((const char*)a->digits,(const char*)outputMUL[i]),0);
		if ((strcmp((const char*)a->digits,(const char*)outputMUL[i])!=0))
			printf("\n should be: %s is:%s \n",outputMUL[i],a->digits);
    any_BIAS_Free(&a);
    any_BIAS_Free(&b);
	}
}
void Test_Any_MulBIASLong(){
	any_BIAS_p a;
    any_BIAS_p b;
    clock_t start,end;
    double time;
    unsigned char * s1=NULL;
    unsigned char * s2=NULL;
    s1=genRandom(s1,100);
    s2=genRandom(s2,100);
	
    	any_BIAS_FromString(&a, s1);
		any_BIAS_FromString(&b, s2);
        
        start=clock();//start measuring time
      if(any_MUL_BIAS(a,b)!=0)
        printf("failure \n");   

        end = clock();// stop measuring time
        time=((double)(end-start))/(CLOCKS_PER_SEC);
        printf("Test_Any_MUL_BIASLong taken: %f [s] to execute. \n",time);

    free(s1);
    free(s2);
    any_BIAS_Free(&a);
    any_BIAS_Free(&b);
}
void Test_Any_DivBIAS(void){
	for(int i=0;i<4;i++){
    any_BIAS_p a;
    any_BIAS_p b;
		any_BIAS_FromString(&a, (unsigned char *)input1[i]);
		any_BIAS_FromString(&b, (unsigned char *)input2[i]);
      any_DIV_BIAS(a,b);
		CU_ASSERT_EQUAL(strcmp((const char*)a->digits,(const char*)outputDIV[i]),0);
		if ((strcmp((const char*)a->digits,(const char*)outputDIV[i])!=0))
			printf("\n should be: %s is:%s \n",outputDIV[i],a->digits);
    any_BIAS_Free(&a);
    any_BIAS_Free(&b);
	}
}
void Test_Any_DivBIASLong(){
    any_BIAS_p a;
    any_BIAS_p b;
    clock_t start,end;
    double time;
    unsigned char * s1=NULL;
    unsigned char * s2=NULL;
    s1=genRandom(s1,100000000);
    s2=genRandom(s2,100000000);
	
    	any_BIAS_FromString(&a, s1);
		any_BIAS_FromString(&b, s2);

        start=clock();//start measuring time
      if(any_DIV_BIAS(a,b)!=0)
        printf("failure \n");   

        end = clock();// stop measuring time
        time=((double)(end-start))/(CLOCKS_PER_SEC);
        printf("Test_Any_Div_BIASLong taken: %f [s] to execute. \n",time);

    free(s1);
    free(s2);
    any_BIAS_Free(&a);
    any_BIAS_Free(&b);
}
void BIAS_Test(){
    //#################################BIAS ########################################################
    printf("##########################METODY BIAS################################ \n");
    any_BIAS_p x;
    any_BIAS_p y;

    any_BIAS_FromString(&x, (unsigned char *)"499");
    any_BIAS_FromString(&y, (unsigned char *)"999");
    printf(" %s - %s = ",x->digits, y->digits);
    any_SUB_BIAS(x,y);

    any_BIAS_output(x);

    any_BIAS_Free(&x);
    any_BIAS_Free(&y);

    any_BIAS_FromString(&x, (unsigned char *)"74");
    any_BIAS_FromString(&y, (unsigned char *)"5716");
    printf(" %s * %s = ",x->digits, y->digits);
    any_MUL_BIAS(x,y);

    any_BIAS_output(x);

    any_BIAS_Free(&x);
    any_BIAS_Free(&y);

    any_BIAS_FromString(&x, (unsigned char *)"9999");
    any_BIAS_FromString(&y, (unsigned char *)"699");
    printf(" %s / %s = ",x->digits, y->digits);
    any_DIV_BIAS(x,y);

    any_BIAS_output(x);

    any_BIAS_Free(&x);
    any_BIAS_Free(&y);

    any_BIAS_FromString(&x, (unsigned char *)"5");
    any_BIAS_FromString(&y, (unsigned char *)"5");
    printf(" %s + %s = ",x->digits, y->digits);
    any_ADD_BIAS(x,y);

    any_BIAS_output(x);

    any_BIAS_Free(&x);
    any_BIAS_Free(&y);
}
void defined_test(char **argv){
    if (strlen(argv[2]) != 1){
        fprintf(stderr, "Error: \'%s\' is not a valid arithmetic operation!\n", argv[2]);
        exit(-1);
    }
    if (!((*(argv[2]) == '+') || (*(argv[2]) == '-') || (*(argv[2]) != '*') || (*(argv[2]) != '/'))){
        fprintf(stderr, "Error: \'%s\' is not a valid arithmetic operation!\n", argv[2]);
        exit(-1);
    }

    any_BIAS_p a;
    any_BIAS_p b;
    any_BIAS_FromString(&a, (unsigned char *)argv[1]);
    any_BIAS_FromString(&b, (unsigned char *)argv[3]);

    switch(*(argv[2])){
        case '+':
        {
            any_ADD_BIAS(a,b);
            any_BIAS_output(a);
            break;
        }
        case '-':
        {
            any_SUB_BIAS(a,b);
            any_BIAS_output(a);
            break;
        }
        case '.':
        {
            any_MUL_BIAS(a,b);
            any_BIAS_output(a);
            break;
        }
        case '/':
        {
            any_DIV_BIAS(a,b);
            any_BIAS_output(a);
            break;
        }        
    }
    any_BIAS_Free(&a);
    any_BIAS_Free(&b);
}
int main(int argc, char **argv){
    if (argc == 1){
        BIAS_Test();
        Test_Any_MulBIASLong();        
        printf("Start testing... \n");

        CU_initialize_registry();
        CU_pSuite suite = CU_add_suite("unit tests", 0, 0);

        CU_add_test(suite,"test of any_add_BIAS for given values",Test_Any_AddBIAS);
        CU_add_test(suite,"test of any_sub_BIAS for given values",Test_Any_SubBIAS);
        CU_add_test(suite,"test of any_div_BIAS for given values",Test_Any_DivBIAS);
        CU_add_test(suite,"test of any_mul_BIAS for given values",Test_Any_MulBIAS);
        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_tests();
        CU_cleanup_registry();
   }
    else if (argc == 4){
        defined_test(argv);
    }
    return 0;
}
