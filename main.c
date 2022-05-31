/*
 *NAME: jehad khaled halahla
 *ID:1201467
 *INSTRUCTOR:Dr.Ahmed Abu Snena
 *SECTION: 1
 * */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//defined the structures that will be used
typedef struct bus{
    int busNumber;
    int date;
    char time[6];
    char from[20];
    char to[20];
    int ticketPrice;
    int capacity;
    struct bus *next;
    struct passenger *headNode;
}bus;

typedef struct passenger{
    int ID;
    int date;
    char time[6];
    char from[20];
    char to[20];
    struct passenger *next;
}passenger;

//the bases for the data **three linked lists**
bus* headNodeB = NULL;
passenger* headNodeP = NULL;
passenger* unmatched = NULL;
void printMenu();
//methods needed for the bus linked list
bus* makeEmptyB(bus* head);
void deleteListB(bus* head);
void deleteNodeB(bus* L,int busNumber);
void printListB(bus* head);
void pushB(bus** headPtr,bus* newNode);
bus* findPreviousB(int busNumber, bus* L);

//methods for passengers linked lists
void deleteListP(passenger* head);
passenger* makeEmptyP(passenger* head);
void pushP(passenger** headPtr,passenger* newNode);
int isEmptyP(passenger* head);
void deleteNodeP(passenger* L,int ID);
passenger* findPreviousP(int ID, passenger* L);
passenger* findP(int ID, passenger* L);


//method to read busses file
void readBussesFile();
bus* makeBus(int busNumber,int date,char time[6],char from[20],char to[20],int ticketPrice,int capacity);
void printSpecificBus();

//method to read passengers' file
void readPassengersFile();
passenger* makePassenger( int id, int date, char time[6],char from[20], char to[20]);
void printListP(passenger* head);

//assigning passengers to different busses
void assignPassengers();
void printAssignInfo();
void addNewPassenger();

int main() {
    //making empty lists
    headNodeB = makeEmptyB(headNodeB);
    headNodeP = makeEmptyP(headNodeP);
    unmatched = makeEmptyP(unmatched);
    int choice;
    readBussesFile();
    readPassengersFile();
    do{
        printMenu();
        printf("please choose an option\n");
        scanf("%d",&choice);
        switch (choice) {
            case 1:{
                printListB(headNodeB);
                break;
            }
            case 2:{
                printListP(headNodeP);
                break;
            }
            case 3:{
                assignPassengers();
                printAssignInfo();
                break;
            }
            case 4:{
                printSpecificBus();
                break;
            }
            case 5:{
                printListP(unmatched);
                break;
            }
            case 6:{
                addNewPassenger();
                break;
            }
            case 7:{
                printf("please enter id of passenger you want to delete:\n");
                int ID;
                scanf("%d",&ID);
                for(bus *ptr =headNodeB->next;ptr != NULL ; ptr = ptr->next){
                    if(findP(ID,ptr->headNode)!= NULL){
                        deleteNodeP(ptr->headNode, ID);
                        (ptr->capacity)++;
                    }

                }

                deleteNodeP(headNodeP,ID);
                deleteNodeP(unmatched,ID);
                break;
            }
            case 8:{
                printf("please enter the bus's number you want to delete:\n");
                int busNumber;
                scanf("%d",&busNumber);
                deleteNodeB(headNodeB,busNumber);
                break;
            }
            case 9:{
                break;
            }
            default:
                printf("please enter a valid option\n");
        }
    }while(choice != 9);
    printf("thank you for using this program ! %c",1);
    return 0;
}
void readBussesFile(){
    FILE *busFile = fopen("busses.txt","r");
    int busNumber;
    int date;
    char time[6];
    char from[20];
    char to[20];
    int ticketPrice;
    int capacity;
    //the following expression matches the format which data in file follows
    while(fscanf(busFile,"%d#%d#%[^#]#%[^#]#%[^#]#%d#%d",&busNumber,&date,time,from,to,&ticketPrice,&capacity)==7){
        //allocating memory to a node and storing the segments
        bus* newBus =  makeBus(busNumber,date,time,from,to,ticketPrice,capacity);
        //this is not exactly a push function
        pushB(&headNodeB, newBus);
    }
    fclose(busFile);
}
//this function creates a node and allocates memory and stores data,then returns a pointer to it.
bus* makeBus(int busNumber,int date,char time[6],char from[20],char to[20],int ticketPrice,int capacity){
    bus* newBus =(bus*)malloc(sizeof(bus));//memory allocation (dynamic)
    newBus->busNumber = busNumber;
    newBus->date = date;
    strcpy(newBus->time,time);
    strcpy(newBus->from,from);
    strcpy(newBus->to,to);
    newBus->ticketPrice = ticketPrice;
    newBus->capacity = capacity;
    return newBus;
}
//initializes a head for the linked list of type bus
bus* makeEmptyB(bus* head){
    //if there exists a list in memory location, replace it
    if(head != NULL)
        deleteListB(head);
    head =(bus*)malloc(sizeof(bus));//returns null if it can't allocate
    if(head == NULL)
        printf("no memory space available");
    head->next= NULL;
    return head;
}
//to delete an entire bus list
void deleteListB(bus* head){
    bus *ptr,*temp;
    //since head doesn't have actual data we start iterating from its next
    ptr = head->next;
    head->next = NULL;
    //iteration loop
    while(ptr != NULL){
        temp = ptr->next;//so we don't lose the link
        free(ptr);//frees the allocated memory for the node
        ptr = temp;
    }
}
//adds an element to the end of a linked list **too lazy to rename to insert
void pushB(bus** headPtr,bus* newNode){
    bus* ptr;
    ptr = *headPtr;
    //loop gets the last node
    while(ptr->next != NULL){
        ptr = ptr->next;
    }
    //adding the new node at the end and making its next NULL
    ptr->next = newNode;
    ptr->next->next = NULL;
}

void printListB(bus* head){//prints the linked list in the order that it is stored
    bus* ptr  = head->next;//a pointer to traverse through the linked list
    while(ptr != NULL){
        printf("bus %d info:\ndate: %d\ntime of departure: %s\ndeparted city: %s\ndestination: %s\nticket price: %d\ncapacity: %d\n"
               "======================================\n",ptr->busNumber,ptr->date,ptr->time,ptr->from,ptr->to,ptr->ticketPrice,ptr->capacity);
        ptr = ptr->next;
    }
}
//allocates memory and copies data and returns a pointer for a passenger
passenger* makePassenger( int id, int date, char time[6],char from[20], char to[20]){
    passenger* newPassenger =(passenger*)malloc(sizeof(passenger));
    newPassenger->ID =id;
    newPassenger->date=date;
    strcpy(newPassenger->time,time);
    strcpy(newPassenger->from,from);
    strcpy(newPassenger->to,to);
    return newPassenger;
}
//allocates memory for the head of the linked list of type passenger
passenger* makeEmptyP(passenger* head){
    if(head != NULL)
        deleteListP(head);
    head =(passenger*)malloc(sizeof(passenger));
    if(head == NULL)
        printf("no memory space available");
    head->next= NULL;
    return head;
}
//deletes an entire passenger list
void deleteListP(passenger* head){
    passenger *ptr,*temp;
    ptr = head->next;
    head->next = NULL;
    //same as deleteListB
    while(ptr != NULL){
        temp = ptr->next;//so it doesn't lose the link and keeps iterating
        free(ptr);
        ptr = temp;
    }
}

void readPassengersFile(){
    FILE *passengerFile = fopen("passengers.txt","r");
    int id;
    int date;
    char time[6];
    char from[20];
    char to[20];
    char line[50];
    while(fgets(line,100,passengerFile)){
        sscanf(line, "%d#%d#%[^#]#%[^#]#%[^#]", &id, &date, time,from,to);//matches data format given
        strcpy(to, strtok(to,"\n"));//to remove trailing \n after the last token in the line
        passenger* newPassenger =  makePassenger(id,date,time,from,to);
        pushP(&headNodeP, newPassenger);//adds the new node to the end of the linked list
    }
    fclose(passengerFile);
}

void pushP(passenger** headPtr,passenger* newNode){
    passenger* ptr;
    ptr = *headPtr;
    while(ptr->next != NULL){
        ptr = ptr->next;
    }
    //gets the end node of the linked list and inserts element after it
    ptr->next = newNode;
    ptr->next->next = NULL;//for loops to be able to find the last node in the linked list
}

void printListP(passenger* head){
    for(passenger* ptr=head->next ; ptr!= NULL;ptr = ptr->next ){
        printf("passenger %d info:\ndate: %d\ntime of departure: %s\ndeparted city: %s\ndestination: %s\n"
               "======================================\n",ptr->ID,ptr->date,ptr->time,ptr->from,ptr->to);
    }
}
//the most complicated function
void assignPassengers(){
    //for loop better for iteration
    for(bus* bPtr = headNodeB->next; bPtr !=NULL; bPtr= bPtr->next){
        //initializes all linked lists of passengers in each bus
        bPtr->headNode = makeEmptyP(bPtr->headNode);
    }
    for(passenger* pPtr = headNodeP->next; pPtr != NULL; pPtr = pPtr->next){
        //the flag to see if the passenger is unmatched
        int isMatched = 0;
        for(bus* bPtr = headNodeB->next; bPtr != NULL; bPtr = bPtr->next){
            //if the capacity is not enough skip to the next bus
            if(bPtr->capacity ==0){
                printf("bus %d can't fit passenger %d!\n",bPtr->busNumber,pPtr->ID);
                bPtr=bPtr->next;
            }
            //all the following must match, the time is compared on ascii and the time of passenger needs to be less or equal to bus time
            if(pPtr->date ==bPtr->date && strcmp(pPtr->from,bPtr->from)==0 &&strcmp(pPtr->to,bPtr->to)==0 && strcmp(pPtr->time,bPtr->time)<=0){
                printf("passenger %d matches bus %d\n",pPtr->ID,bPtr->busNumber);
                passenger* newPassenger  = makePassenger(pPtr->ID,pPtr->date,pPtr->time,pPtr->from,pPtr->to);
                //after adding the remaining capacity decreases
                isMatched = 1;
                passenger* ptr= bPtr->headNode;

                int exist= 0;
                while(ptr!=NULL){
                    if(ptr->ID == pPtr->ID){
                        exist=1 ;
                        break;
                    }
                    ptr = ptr->next;
                }
                if(exist == 0){
                    pushP(&(bPtr->headNode),newPassenger);
                    (bPtr->capacity)--;
                }



                break;//no need to continue he already found a bus
            }
        }
        //add him to the unmatched passengers list
        if(isMatched==0){
            passenger* newPassenger  = makePassenger(pPtr->ID,pPtr->date,pPtr->time,pPtr->from,pPtr->to);
            int exists = 0;
            for(passenger* ptr = unmatched->next ; ptr!=NULL;ptr = ptr->next){
                //if passenger already exists in the list then don't add him
                if(ptr->ID == newPassenger->ID){
                    exists =1;
                    break;
                }
            }
            if(exists ==0)
                pushP(&unmatched,newPassenger);
        }
    }
}
//print passengers in busses
void printAssignInfo(){
    bus* ptr = headNodeB->next ;
    while(ptr != NULL){
        printf("======================================\n");
        printf("bus %d passengers:\n",ptr->busNumber);

        if(!isEmptyP(ptr->headNode)){
            printListP(ptr->headNode);
        }
        else{
            printf("bus has no passengers\n",ptr->busNumber);
            printf("======================================\n");
        }
        ptr = ptr->next;
    }
}

int isEmptyP(passenger* head){
    return head->next ==NULL;
}
//using bus number it prints the bus with its passengers' info
void printSpecificBus(){
    printf("please enter the bus number you want to see its' passengers:\n");
    int busNumber;
    scanf("%d",&busNumber);
    int isFound = 0;
    for(bus* ptr = headNodeB->next; ptr !=NULL;ptr=ptr->next){
        if(busNumber == ptr->busNumber){
            printf("bus %d has the following passengers:\n\n",busNumber);
            printListP(ptr->headNode);
            isFound =1;
            break;
        }
    }
    if (isFound==0)
        printf("this bus doesn't exist\n\n");
}

void printMenu(){
    printf("1. Load the bus information file\n"
           "2. Load the passenger information file\n"
           "3. Assign passengers and print assignment information of all busses\n"
           "4. Print a specific bus information along with its passengers information (names and IDs)\n"
           "5. Print unmatched passengers\n"
           "6. Add new passenger\n"
           "7. Delete passenger\n"
           "8. Delete bus number\n9. Exit\n");
}

void addNewPassenger(){
    printf("please enter passenger info in this format:(ID,DATE,TIME,FROM,TO)\n");
    int id;
    int date;
    char time[6];
    char from[20];
    char to[20];
    char line[50];
    int c;
    int exists =0;
    while ( (c = getchar()) != EOF && c != '\n' );
    fgets(line,50,stdin);
    sscanf(line, "%d,%d,%[^,],%[^,],%[^,]", &id, &date, time,from,to);//format requested from user
    strcpy(to, strtok(to,"\n"));//to remove trailing \n after the last token in the line
    for(passenger* ptr = headNodeP->next; ptr != NULL;ptr = ptr->next){
        if(id == ptr->ID){
            exists = 1;
        }
    }
    if(exists == 1){
        printf("please enter a valid id, you will be prompted once");
        scanf("%d",&id);
    }
    //adds the passenger to the main linked list that has all passengers
    passenger* newPassenger =  makePassenger(id,date,time,from,to);
    pushP(&headNodeP, newPassenger);
}
//deletes node from the linked list based on passengers unique id
void deleteNodeP( passenger* L,int ID){
    passenger* P, *temp;
    P = findPreviousP(ID, L);
    if( P->next !=NULL ){
        temp = P->next;
        P->next = temp->next; //bypass delete cell
        free(temp);
    }
}
//find the node that points to the node with the given id
passenger* findPreviousP(int ID, passenger* L){
    passenger* ptr;
    ptr = L;
    while(ptr->next != NULL && ptr->next->ID != ID)
        ptr = ptr->next;
    return ptr;
}
passenger* findP(int ID, passenger* L){
    passenger* ptr;
    ptr = L;
    while(ptr!= NULL && ptr->ID != ID)
        ptr = ptr->next;
    return ptr;
}

//same with passengers,but using bus number instead of id
void deleteNodeB( bus* L,int busNumber){
    bus* P, *temp;
    P = findPreviousB(busNumber, L);
    if( P->next !=NULL ){
        temp = P->next;
        P->next = temp->next; //bypass delete cell
        //empties the linked list to save memory
        deleteListP(temp->headNode);
        free(temp);
    }
}
bus* findPreviousB(int busNumber, bus* L){
    bus* ptr;
    ptr = L;
    while(ptr->next != NULL && ptr->next->busNumber != busNumber)
        ptr = ptr->next;
    return ptr;
}

/*
 * thanks for using this code!
*/


