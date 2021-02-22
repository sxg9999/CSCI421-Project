#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct{
    int id;
    int age;
    char* name;
}Person;


typedef union{
    char c[255];
}record_item;

void person_struct_test(){
    Person person;

    person = (Person){.age = 10, .id = 1, .name = "bob"};
    
    printf("Person: %d, %d, %s", person.id, person.age, person.name);
}

void clear_n_buffer(char* buffer, int end_of_buffer){
    int i;
    for(i=0; i<end_of_buffer; i++){
        buffer[i]=0;
    }
}

void remove_trailing_zeros(char* str, int len){
    // printf("length of string is: %d\n", len);
    int i;
    for(i=0; i<len; i++){
        if(str[i]=='0'){
            str[i]=0;
        }
    }
    // printf("result str: %s\n",str);
}

void convert_from_int_to_string(){

    int int_val = 13;

    char buffer[255];

    sprintf(buffer,"%d",int_val);

    printf("(int to str)The buffer contains : %s\n",buffer);
}

void convert_from_double_to_string(){
    
    double double_val = 24.12;

    char buffer[255];
    

    sprintf(buffer,"%.6f",double_val);

    remove_trailing_zeros(buffer, strlen(buffer));

    printf("(double to str)The buffer contains : %s\n",buffer);

}


void test_ternary_operator(){

    char result[255];
    
    strncpy(result, false?"true":"false",60);

    printf("size of result str : %d\n", strlen(result));
    printf("result : %s\n", result);

}


void test_pointing_strings(){

    char buffer[20];
    char name[] ="bobtheknowitall";

    


}

void test_getting_length_from_a_pointer(){
    record_item record_item1;
    strncpy(record_item1.c, "hello", 6);
    
    printf("the length of the record_item : %d\n", strlen(record_item1.c));

}


void test_converting_string_to_any(){

    char val[] = "123 hello";

    // int int_val = atoi(val);

    char* ptr;

    double double_val = strtod(val,&ptr);



    printf("double val is : %f\n",double_val);
    printf("ptr contains : %s\n",ptr);

    int int_val = strtol(val, &ptr, 10);    //base 10 for dec
    printf("int val is : %d\n", int_val);
    printf("ptr contains : %s\n",ptr);
    

}

int test_mod(){

    return 0;
}
int main(){
    
    test_converting_string_to_any();
    
    
    return 0;
}