#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void insertCustomer(customer** customers, char name[], char surname[], char customertype[], int id, double totaldebt) {
    customer* newCustomer = (customer*)malloc(sizeof(customer));
    strcpy(newCustomer->name, name);
    strcpy(newCustomer->surname, surname);
    strcpy(newCustomer->customertype, customertype);
    newCustomer->customerid = id;
    newCustomer->totaldebt = totaldebt;
    newCustomer->loanptr = NULL;
    newCustomer->nextcust = NULL;

    if (*customers == NULL) {
        *customers = newCustomer;
    } else {
        customer* temp = *customers;
        while (temp->nextcust != NULL) {
            temp = temp->nextcust;
        }
        temp->nextcust = newCustomer;
    }
}

void printCustomers(customer* customers) {
    customer* temp = customers;
    while (temp != NULL) {
        printf("--------------------------------------------------\n");
        printf("%d - %s %s type : %s total debt : %.0f\n", temp->customerid, temp->name, temp->surname, temp->customertype, temp->totaldebt);
        temp = temp->nextcust;
    }
}

void readCustomers(customer** customers, const char* filename) {
    FILE* customers_txt = fopen(filename, "r");

    char line[150];
    char name[20];
    char surname[30];
    char customerType[20];
    int id = 1;
    double totaldebt = 0;

    if (customers_txt == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), customers_txt) != NULL) {
        sscanf(line, "%s %s %s", name, surname, customerType);
        insertCustomer(customers, name, surname, customerType, id, totaldebt);
        id++;
    }

    fclose(customers_txt);
}

void freeCustomerList(customer** customers) {
    customer* temp = *customers;
    customer* next;

    while (temp != NULL) {
        next = temp->nextcust;
        free(temp);
        temp = next;
    }

    *customers = NULL;
}

int main(){
    customer *customers = NULL;
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
                readCustomers(&customers, "customers.txt");
                break;
            case 2:
                //printCustomers function call here
                printCustomers(customers);
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