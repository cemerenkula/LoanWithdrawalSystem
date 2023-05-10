#include <stdio.h>
#include <stdlib.h>

typedef struct installment{
    char insid[30];
    short ispaid;
    char installmentdate[11];
    float amount;
    struct installment *nextins;
}installment;

typedef struct loan{
    char loanid[30];
    char type[30];
    float totalamount;
    int totalinstallmentnum;
    char processdate[11];
    struct loan *nextloan;
    installment *insptr;
}loan;

typedef struct customer{
    char name[20];
    char surname[30];
    int customerid;
    char customertype[20];
    struct customer *nextcust;
    double totaldebt;
    loan *loanptr;
}customer;

void newCustomerFunc(customer* head, customer* currNode, int id){
    customer* newCustomer;
    newCustomer = (customer*) malloc(sizeof(customer));

    if (head == NULL) {
        head = newCustomer;
        currNode = newCustomer;
        newCustomer->customerid = id;
        newCustomer->totaldebt = 0;
        newCustomer->loanptr = NULL;
    }
    else {
        currNode->nextcust = newCustomer;
        currNode = newCustomer;
        newCustomer->customerid = id;
        newCustomer->totaldebt = 0;
        newCustomer->loanptr = NULL;
    }
}

void readCustomers(){

    int letter;
    int space = 0;
    int i = 0;
    int id = 1;

    //Creating head and currCustomer
    customer* head = NULL;
    customer* currCustomer = NULL;

    //Creating First Customer Node
    customer* newCustomer;
    newCustomer = (customer*) malloc(sizeof(customer));
    head = newCustomer;
    currCustomer = newCustomer;
    newCustomer->customerid = id;
    newCustomer->nextcust = NULL;
    newCustomer->totaldebt = 0;
    newCustomer->loanptr = NULL;

    //Reading File
    FILE *customer_txt = fopen("customer.txt", "r");
    if(customer_txt == NULL){
        printf("Error opening file\n");
    }

    //Getting First Letter
    letter = fgetc(customer_txt);



    //Reading letter by letter
    while(letter != EOF){

        if(letter == "\n"){
            id++;
            newCustomerFunc(head, currCustomer, id);
            letter = fgetc(customer_txt);
            space = 0;
            i = 0;
        }



        switch (space){
            case 0:
                if(letter == " ") {
                    newCustomer->name[i] = '\0';
                }
                else {
                    newCustomer->name[i] = letter;
                }
                break;
            case 1:
                if(letter == " ") {
                    newCustomer->surname[i] = '\0';
                }
                else {
                    newCustomer->surname[i] = letter;
                }
                break;
            case 2:
                newCustomer->customertype[i] = letter;
                break;
        }

        if(letter == " "){
            space++;
            letter = fgetc(customer_txt);
            i = 0;
            continue;
        }

        letter = fgetc(customer_txt);
        i++;

    }
}

void printCustomers(customer *n){
    printf("--------------------------------------------------");
    while(n != NULL){
        for (int i = 0; n->name[i] != '\0'; i++) {
            printf("%c", n->name[i]);
        }

        n = n->nextcust;
        printf("\n");
    }

}

int main(){
    customer *customers;
    int option = 1000;
    while (option != 0){
        printf("\n\n#############################################################\n");
        printf("1. read customers.\n");
        printf("2. print customers.\n");
        printf("3. read loans.\n");
        printf("4. print loans.\n");
        printf("5. create installments.\n");
        printf("6. print installments.\n");
        printf("7. read payments.\n");
        printf("8. find unpaid installments.\n");
        printf("9. delete completely paid installments.\n");
        printf("please select your option : \n");
        scanf("%d", &option);
        printf("#############################################################\n");

        switch (option){
            case 1:
                //readCustomers function call here
                readCustomers();
                break;
            case 2:
                //printCustomers function call here
                printCustomers(head);
                break;
            case 3:
                //readLoans function call here
                break;
            case 4:
                //printLoans function call here
                break;
            case 5:
                //createInstallments function call here
                break;
            case 6:
                //printInstallments function call here
                break;
            case 7:
                //readPayments function call here
                break;
            case 8:
                //findUnpaidInstallments function call here
                break;
            case 9:
                //DeletePaidInstallments function call here
                break;
            case 0:
                break;
            default :
                printf("invalid option\n");
        }
    }
    return 0;
}