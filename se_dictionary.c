//#include <iostream.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


struct table
{
    int used;
    char word[200];
    char meaning[300];
};

int hash2(char *word,int size)
{
    int sum,i;
    int factor =2;
    for(sum=0,i=0;i<strlen(word);i++)
    {
        sum = sum + (word[i] *factor);
        factor = factor*3;
    }  
    return sum %(size-1);
}
int hash(char *word, int i,int size)
{
    int sum=0,j;
    int factor = 10;
    for(j=0;j<strlen(word);j++)
    {
        sum = sum + word[j]*factor;
        factor = factor *2;
    }
    int result = sum + (i*hash2(word,size));
    result = result%(size-1);
    return result;
}

void print_table(struct table *t, int size)
{
    int i;
    for(i=0;i<size;i++)
    {
        if(t[i].used == -1)
            continue;
        printf("Word: %s\nMeaning: %s\n",t[i].word,t[i].meaning);
        printf("-----------------------------------------------\n");
    }
}

int insert_table(struct table *t, char* word, char* temp, int size)
{
    char *text = word;
    char *meaning = temp;
    meaning[strlen(meaning)-1]='\0';
    //printf("word : %s || meaning: %s\n",text,meaning);
    int index = hash(text,0,size);
    int j=1,probes=1,flag=0;
    while(t[index].used != -1 && j<=101)
    {
        if(strcmp(t[index].word,text)==0)
        {
            int len = strlen(t[index].meaning);
            t[index].meaning[len-1] = ' ';
            t[index].meaning[len-2] = ';';
            strcat(t[index].meaning,meaning);
            //printf("Inside %d %s || %s\n",index,t[index].word,t[index].meaning);
            flag = 1;
            break;
        }
        index = hash(text,j,size);
        j++;
        probes++;
    }
    if(flag == 0 && j<100)
    {
        strcpy(t[index].word,text);
        strcpy(t[index].meaning,meaning);
        t[index].used = 0;
        flag=1;
        //printf("%d %s || %s\n",index,t[index].word,t[index].meaning);
    }
    printf("\t\t%d probes\n",probes);
    if(flag ==1)
        printf("\t\tWill insert pair %s,%s\n",t[index].word,t[index].meaning);
    else
        printf("\t\tCannot insert pair.\n");
    return probes;
}

int search_table(struct table *t, char* word, int size)
{
    int i=1,index,probes=1;;
    index = hash(word,0,size);
    while(strcmp(t[index].word,word)!=0 && i<=101)
    {
        //printf("table : %s || word: %s\n",t[index].word,word);
        index = hash(word,i,size);
        i++;
        probes++;
    }
    printf("\t\t%d probes\n",probes);
    if(strcmp(t[index].word,word) == 0)
        printf("\t\tTranslation: %s\n",t[index].meaning);
    else
        printf("\t\tNot Found.\n");
    return probes;
}

int delete_table(struct table *t, char* word, int size)
{
    int i=1,index,probes=1;
    index = hash(word,0,size);
    while(strcmp(t[index].word,word)!=0 && i<=101)
    {
        index = hash(word,i,size);
        i++;
        probes++;
    }
    printf("\t\t%d probes\n",probes);
    if(strcmp(t[index].word,word) == 0)
    {
        strcpy(t[index].word,"\0");
        strcpy(t[index].meaning,"\0");
        t[index].used =-1;
        printf("\t\tItem Deleted.\n");
    }
    else
        printf("Item not found => no deletion.\n");
    return probes;
    return 0;
}



int main()
{   
    printf("About the program: \n");
    printf("This program is made to show familiarity with hashing. \n");
    printf("This program accepts input of a filename and hashes each entry of the file, \n");
    printf("then it accepts commands to delete, search or insert words.\n");
    printf("It also displays the average and total number of collisions (probes).\n\n");
    int i,j,flag;
    int count_of_keys[101];
    int probes =0;
    char filename[50];
    printf("Enter the filename with the dictionary data (include the extension e.g. Spanish.txt): ");
    scanf("%s",filename);
    FILE *fp = fopen(filename,"r");
    if(fp == NULL)
    {   
        perror("Error opening file\n"); 
        exit(1);
    }
    int size = 22433;
    struct table *t = (struct table*)malloc(22433*sizeof(struct table)); 
    if(t == NULL)
    {
        perror("Error in malloc\n");
        exit(1);
    }
    for(i=0;i<size;i++)
    {    
        t[i].used = -1;
        strcpy(t[i].word,"\0");
        strcpy(t[i].meaning,"\0");
    }
    for(i=0;i<100;i++)
        count_of_keys[i] =0;

    //reading and storing

    char line[1001];
    char tab[] = "\t";
    int file_size =0;
    while(fgets(line,1001,fp)!= NULL)
    {
        file_size++;
        flag=0,probes =0,j=1;
        char* word = strtok(line,tab);
        char *meaning = strtok(NULL,tab);
        /*printf("word = %s ||",word);
        printf("Meaning = %s\n",meaning);*/
        int index = hash(word,0,size);
        while(t[index].used != -1)
        {
            if(strcmp(t[index].word,word)==0)
            {
                int len = strlen(t[index].meaning);
                t[index].meaning[len-1] = ' ';
                t[index].meaning[len-2] = ';';
                strcat(t[index].meaning,meaning);
                //printf("Inside %d %s || %s\n",index,t[index].word,t[index].meaning);
                flag = 1;
                count_of_keys[probes]= count_of_keys[probes] + 1;
                break;
            }
            index = hash(word,j,size);
            j++;
            probes++;
        }
        if(flag == 0)
        {
            strcpy(t[index].word,word);
            strcpy(t[index].meaning,meaning);
            t[index].used = 0;
            if(probes <=100)    
                count_of_keys[probes]= count_of_keys[probes] + 1;
            //printf("%d %s || %s\n",index,t[index].word,t[index].meaning);
        }
    }
    int max =0,total=0,sum =0;
    double avg=0;
    for(i=0;i<100;i++)
    {    
        sum += count_of_keys[i];
        if(count_of_keys[i]>=1)
            max= i;
        total = total + (i+1)*count_of_keys[i];
    }
    avg = (double)total/(double)sum;
    printf("Hash Table");
    printf("\n\tAverage number of probes: %7.2f",avg);
    printf("\n\tmax_run of probes: %14d",++max);
    printf("\n\tTotal Probes: %19d",total);
    printf("\n\tItems not hashed: %15d",sum-file_size);
    printf("\n\nProbes|Count of Keys");
    for(i=0;i<100;i++)
    {
        printf("\n-------------\n");
        printf("%6d|%6d",i+1,count_of_keys[i]);
    }
    printf("\n");
    //print_table(t,size);
    printf("\n\n");
    printf("The following commands are available:\n");
    printf("1. Search words - s <word>\n");
    printf("2. Delete words - d <word>\n");
    printf("3. Insert words - i <word> <meaning>\n");
    printf("Press q to quit\n\n");
    

    char input[100]; int count=0,num_of_probes=0;
    getchar();
    do
    {
        printf("Enter your choice: ");
        
        fgets(input,100,stdin);
        char *temp;
        if(input[0] == 'q')
        {   
            printf("Avg number of Probes for the commands = %f\n",(double)num_of_probes/count);
        }
        else
        {
            count++;
            char *token = strtok(input," ");
            token = strtok(NULL," ");
            printf("READ op: %c \nquery: %s\n",input[0],token);
            switch(input[0])
            {
                case 's': 
                            token[strlen(token)-1] ='\0';
                            num_of_probes += search_table(t,token,size);
                            break;
                case 'i': 
                            temp = strtok(NULL," ");
                            num_of_probes += insert_table(t,token,temp,size);
                            break;
                case 'd': 
                            token[strlen(token)-1] ='\0';
                            num_of_probes += delete_table(t,token,size);
                            break;
                default: 
                            printf("Wrong option. Try Again.\n");
                            break;
            } 
        }        
    }while(input[0]!='q');
    fclose(fp);
    free(t);
}