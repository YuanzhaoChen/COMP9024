#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#define MAX_LEN 200

int main(){
	char c;
	while((c=getchar())!=EOF){
		printf("you entered: %c\n",c);
	}
	return 0;
}

/*
int main () {
	char s[MAX_LEN];
	while(fgets(s,MAX_LEN,stdin)!=NULL){
		int len=strlen(s);
		int i=0;
		while(i<len){
			if(s[i]=='('){//there's a new item
				int key=0,value=0,keySign=1,valueSign=1;
				while(s[i]!=','){ //before ',', all digits belong to key 
					if(s[i]=='-'){keySign=-1;}
					if(s[i]>='0' && s[i]<='9'){key=10*key+s[i]-'0';}
					i+=1;
					if(i==len){return 1;}
				}
				while(s[i]!=')'){ //digites between ',' and ')' belong to key
					if(s[i]=='-'){valueSign=-1;}
					if(s[i]>='0' && s[i]<='9'){value=10*value+s[i]-'0';}
					i+=1;
					if(i==len){return 1;}
				}
				key*=keySign;
				value*=valueSign;
				printf("(%d,%d) ",key,value);
			}
			i+=1;
		}
		printf("\n");
		if(strstr(s,"end")!=NULL){break;}
	}
   	return 0;
}
*/