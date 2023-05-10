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

void readCustomers(struct customer **head) {
	FILE *fp;
	char line[100];
    int id = 1;
    struct customer *current = *head;
    
    // open customers file
    fp = fopen("customers.txt", "r");
    if (fp == NULL) {
        printf("Error: could not open customers file.\n");
        exit(1);
    }
    
    // read file line by line and create customers
    while (fgets(line, sizeof(line), fp)) {
        // parse customer information from line
        char name[20], surname[30], customertype[20];
        sscanf(line, "%s %s %s", name, surname, customertype);
        
        // create new customer
        struct customer *newcust = malloc(sizeof(struct customer));
        strcpy(newcust->name, name);
        strcpy(newcust->surname, surname);
        newcust->customerid = id++;
        strcpy(newcust->customertype, customertype);
        newcust->totaldebt = 0;
        newcust->loanptr = NULL;
        newcust->nextcust = NULL;
        
        // add customer to linked list
        if (*head == NULL) {
            *head = newcust;
            current = newcust;
        } else {
            current->nextcust = newcust;
            current = newcust;
        }
    }
    
    // close customers file
    fclose(fp);
}

void printCustomers(struct customer *head) {
    struct customer *current = head;

    printf("\n\n#############################################################\n");
    while (current != NULL) {
    	printf("---------------------------------------------------\n");
    	printf("%d - %s %s - type : %s - total debt: %.2f", current->customerid, current->name, 
		current->surname, current->customertype, current->totaldebt);
        current = current->nextcust;
    }
}




char* createLoanID(struct customer* customer) {
    static char loanID[30];
    int count = 0;
    struct loan* currentLoan = customer->loanptr;
    while (currentLoan != NULL) {
        count++;
        currentLoan = currentLoan->nextloan;
    }
    sprintf(loanID, "%dL%d", customer->customerid, count);
    return loanID;
}

void readLoans(struct customer *head) {
    struct customer *current = head;
    
    while (current != NULL) {
        FILE *file = fopen("loans.txt", "r");
        if (file == NULL) {
            printf("Error opening loans.txt file.\n");
            return;
        }

        struct loan *prevLoan = NULL;
        struct loan *currentLoan = current->loanptr;

        // Find the last loan in the linked list
        while (currentLoan != NULL && currentLoan->nextloan != NULL) {
            currentLoan = currentLoan->nextloan;
        }

        char name[20], surname[30], type[30], processdate[11];
        float totalamount;
        int totalinstallmentnum;

        // Read loan details from the file and create loan struct
        while (fscanf(file, "%s %s %s %f %d %s", name, surname, type, &totalamount, &totalinstallmentnum, processdate) == 6) {
            if (strcmp(name, current->name) == 0 && strcmp(surname, current->surname) == 0) {
                struct loan *newLoan = (struct loan *)malloc(sizeof(struct loan));
                if (newLoan == NULL) {
                    printf("Memory allocation failed.\n");
                    fclose(file);
                    return;
                }
                strcpy(newLoan->loanid, createLoanID(current));
                strcpy(newLoan->type, type);
                newLoan->totalamount = totalamount;
                newLoan->totalinstallmentnum = totalinstallmentnum;
                strcpy(newLoan->processdate, processdate);
                newLoan->nextloan = NULL;
                newLoan->insptr = NULL;

                // Insert the new loan into the linked list based on processdate
                if (currentLoan == NULL) {
                    current->loanptr = newLoan;
                } else {
                    currentLoan->nextloan = newLoan;
                }
                currentLoan = newLoan;
            }
        }

        fclose(file);
        current = current->nextcust;
    }
}

void printLoans(struct customer *head){
	struct customer *currentCustomer = head;
	struct loan* currentLoan = currentCustomer->loanptr;
	
	printf("\n\n#############################################################\n");
	while(currentCustomer != NULL){
		while (currentLoan != NULL) {
		printf("---------------------------------------------------\n");
		printf("%d - %s %s - type : %s - total debt: %.2f\n", currentCustomer->customerid, 
		currentCustomer->name, currentCustomer->surname, 
		currentCustomer->customertype, currentCustomer->totaldebt);
		printf("%s : ½s - %.2f - %s - %d\n", currentLoan->loanid, currentLoan->type, 
		currentLoan->totalamount, currentLoan->totalinstallmentnum);
        currentLoan = currentLoan->nextloan;
        }
        currentCustomer = currentCustomer->nextcust;
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
                readCustomers(&customers);
                break;
            case 2:
            	printCustomers(customers);
                break;
            case 3:
                readLoans(&customers);
                break;
            case 4:
                printLoans(customers);
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
