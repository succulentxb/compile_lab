#include <stdio.h>

#include "util.h"
#include "slp.h"
#include "prog1.h"

#include "answer.h"

int maxargs(A_stm prog){
//    return 0;
    return maxargsStm(prog);
}

void interp(A_stm prog){
    interpWrapper(prog);
}

// DO NOT CHANGE!
int main(){
	printf(">> Right Prog Section:\n");
	A_stm rp = right_prog();
	printf("the maximum number of arguments of any print statement is %d\n",maxargs(rp));
	interp(rp);

	printf(">> Error Prog Section:\n");
	A_stm ep = error_prog();
	printf("the maximum number of arguments of any print statement is %d\n",maxargs(ep));
	interp(ep);	
	return 0;
}

