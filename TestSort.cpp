#include <iostream>


void QuickSort(int data[], int start, int end) { //start==0 ,end == length of array 
    if (start >= end)
        return ;

    int i = start;
    int j = end;  //end+1 elements
    int keyValue = data[start];//choose a key value ( any value in the array )

    while (true) {
        while (data[++i] < keyValue) {  //data[i]<key is ok , this element still should be puted in the left of key value
            if (i == end) //注意这里是 end ，cuz the max is array[end] , there isnot about length
                break;   //all elements have been compared 
        }

        while (data[--j] > keyValue) {  //data[j]>key  is ok , still in the right of key value ,go ahead
            if (j == start)             //all elements have compared 
                break;
        }

        if (i >= j) break;          //judge if correspond the condition
        
         //swap
        int temp = data[i];         //the elements didnt correspond the condition of data[]<keyValue
        data[i] = data[j];          //in other words , data[j]<key,but data[i]<key, so exchange thier's position 
        data[j] = temp;             //make all elements correspond the rule of the elements >key in the right

    }                           // finished one time 

    //make data[j] be a new keyvalue,but put it in start ,cuz we choose the data[start] to be keyvalue recently
    int temp = data[start]; //swap the orginal keyvalue with data[j] ,
    data[start] = data[j];    //we have knew data[j]>key, key value exchange with data[j],in the midlle of array finally
    data[j] = temp;             //now data[j]'s position is the original key's position 原来的key放到中间，这次把原来data[j]当key

    QuickSort(data, 0, j-1);  //be careful j-1 not j !!
    QuickSort(data, j+1, end); //be careful j+1 ,not j !!

}


int main()
{
    int data[] = { 23,3,56,7,8,56,83,94,5 };
    QuickSort(data, 0, sizeof(data) / sizeof(data[0]) ); //the third para is the length of array
    for(int i=0;i< sizeof(data) / sizeof(data[0]);i++)
    //   std::cout<< sizeof(data) / sizeof(data[0]);  //testing the length of array
    std::cout << data[i]<<std::endl;

}