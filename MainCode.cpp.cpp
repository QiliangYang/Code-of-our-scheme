#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#include<sys/time.h>
#include<unistd.h>
#include<pbc.h>

/*System param*/
pairing_t pairing;
element_t g,Dx,Ex,num;
element_t fx;
element_t fx1,fx2,fx3;

/*Control tree*/
#define N   200
element_t KP[N];    //The Set of attributes

/*Time*/
clock_t   set_up_start, set_up_end ;
struct    timeval TimeStart, TimeEnd, TimeDay;
long int  RunTime  = 0;

/*Compute pairing*/
element_t temp,temp1,temp2;

void set_up();
void create_kp_ab();
int  get_random_node();


int get_random_node(){

    int randnumber = 2;
    srand((unsigned)time(NULL));
    randnumber = rand() % N;
    if(randnumber % 2 == 0 ){
        randnumber++;
        return randnumber;
    }else{
        return randnumber;
    }

}

/*From the leaf node to the root node, find the information for all nodes*/
/*index -3 = root; root + 1 = uncle leaf node;*/

int compute_fx(int i){
    
    int count = i;

    count = count - 3;

    while(count != 0){
        //printf("search root: %d\n",count);
        element_sub(fx1,KP[i],KP[count+1]);
        element_sub(fx2,KP[0],KP[count+1]);
        element_div(fx3,fx1,fx2);
        if(count == (i-3)){
            element_set(fx,fx3);   
        }else{  
            element_mul(fx,fx,fx3);
        }
        if(count == 2){
        
            break;
        }
        count -= 2;
    }

    return 0;
}


int main(){
    

    
    /*Create system parameters and use Type-A G1*G1->GT parameter*/
    gettimeofday(&TimeStart,NULL);
    set_up();                     //create system param
    gettimeofday(&TimeEnd,NULL);
    RunTime = (TimeEnd.tv_sec - TimeStart.tv_sec)*1000 + (TimeEnd.tv_usec - TimeStart.tv_usec)/1000;
    printf("Set up use time : %ld ms \n",RunTime);


    
    gettimeofday(&TimeStart,NULL);
    create_kp_ab();            /*Create an access tree structure*/
	gettimeofday(&TimeEnd,NULL);
	RunTime = (TimeEnd.tv_sec - TimeStart.tv_sec)*1000 + (TimeEnd.tv_usec - TimeStart.tv_usec)/1000;
	printf("Create tree time : %ld ms \n",RunTime);

    /*Test N times of pairing operation time respectively, test N attributes at a time£¬attribute locations are randomly selected*/
    int index_i = 0;        //Total number of tests
    int index_j = 0;        //Number of attributes per test
    for(index_i = 1; index_i < 10; index_i++){
        
        gettimeofday(&TimeStart,NULL);
        for(index_j = 0; index_j < 15 * index_i; index_j++){
            
            int randnode = get_random_node();
            compute_fx(randnode);
            element_pairing(temp,Dx,Ex);
            element_pow_zn(temp1,temp,fx);
            if(index_j == 1){
                element_set(temp,temp1);  
            }else{  
                element_mul(temp,temp1,temp);
            }
        }    
	    gettimeofday(&TimeEnd,NULL);
	    RunTime = (TimeEnd.tv_sec - TimeStart.tv_sec)*1000 + (TimeEnd.tv_usec - TimeStart.tv_usec)/1000;
        printf("Number:%dth test: Select %d attributes: Total time: %ld ms \n",index_i,index_j,RunTime);
    }

	return 0;
}

/*create System*/
void set_up(){

    char param[1024];
    FILE * fp = fopen("./a.param","r");
    size_t count = fread(param,1,1024,fp);
    fclose(fp);

    if(!count){
        pbc_die("input error\n");
    }
    
    pairing_init_set_buf(pairing,param,count);       
    
    element_init_G1(g,pairing);
    element_init_G1(Dx,pairing);
    element_init_G1(Ex,pairing);
    element_init_GT(temp,pairing);
    element_init_GT(temp1,pairing);
    element_init_GT(temp2,pairing);
    element_init_Zr(num,pairing);
    element_init_Zr(fx,pairing);
    element_init_Zr(fx1,pairing);
    element_init_Zr(fx2,pairing);
    element_init_Zr(fx3,pairing);

    element_random(g);
    element_random(Dx);
    element_random(Ex);
    element_random(num);
    element_random(fx);

}

void create_kp_ab(){
    
    for(int i = 0; i < N; i++){
        element_init_Zr(KP[i],pairing);
        element_random(KP[i]);
    }

}




