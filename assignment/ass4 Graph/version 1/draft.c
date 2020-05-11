#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<math.h>
#define LINE_LENGTH 100
int main(void){
    FILE*fp=fopen("Distance.txt","r");
    assert(fp!=NULL);
    char line[LINE_LENGTH];
    while(fgets(line,LINE_LENGTH,fp)!=NULL){
        if(strcmp(line,"\n")==0){continue;}
        int fromBusId=0,toBusId=0,distance=0;
        int state=0;
        for(int i=0;i<strlen(line);i+=1){
            if(line[i]=='-'){
                state=1;
            }else if(line[i]==':'){
                state=2;
            }
            if(line[i]<'0'||line[i]>'9'){
                continue;
            }
            if(state==0){
                fromBusId=fromBusId*10+line[i]-'0';
            }else if(state==1){
                toBusId=toBusId*10+line[i]-'0';
            }else if(state==2){
                distance=distance*10+line[i]-'0';
            }
        }
        printf("from bus: %d, to bus: %d, distance: %d\n",fromBusId,toBusId,distance);
    }
    fclose(fp);
    return 0;
}