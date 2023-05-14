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
    newCustomer->nextcust = NULL;
    newCustomer->loanptr = NULL;

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
        printf("%d - %s %s - type : %s total debt : %.0f\n", temp->customerid, temp->name, temp->surname, temp->customertype, temp->totaldebt);
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


void loanSort(customer** customers) {
    int swapped;
    customer *ptr1;
    loan *lptr, *prev_lptr = NULL;

    if (*customers == NULL) {
        return;
    }

    do {
        swapped = 0;
        ptr1 = *customers;

        while (ptr1 != NULL) {
            lptr = ptr1->loanptr;

            while (lptr != prev_lptr && lptr->nextloan != NULL) {
                loan* next_lptr = lptr->nextloan;

                // Extract the year, month, and day values from the two process dates
                int year1, month1, day1, year2, month2, day2;
                sscanf(lptr->processdate, "%d/%d/%d", &day1, &month1, &year1);
                sscanf(next_lptr->processdate, "%d/%d/%d", &day2, &month2, &year2);

                // Compare the two process dates
                if (year1 > year2 || (year1 == year2 && (month1 > month2 || (month1 == month2 && day1 > day2)))) {
                    // Swap the nodes if the left node's processDate is greater than the right node's processDate
                    lptr->nextloan = next_lptr->nextloan;
                    next_lptr->nextloan = lptr;
                    if (prev_lptr != NULL) {
                        prev_lptr->nextloan = next_lptr;
                    }
                    else {
                        ptr1->loanptr = next_lptr;
                    }
                    prev_lptr = next_lptr;
                    swapped = 1;
                }
                else {
                    prev_lptr = lptr;
                    lptr = lptr->nextloan;
                }
            }

            prev_lptr = NULL;
            ptr1 = ptr1->nextcust;
        }
    } while (swapped);
}

void insertLoan(customer** customers, char name[], char surname[], char loanType[], float totalAmount, int totalInstallmentNum, char processDate[]) {
    customer* temp = *customers;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0 && strcmp(temp->surname, surname) == 0) {
            break;
        }
        temp = temp->nextcust;
    }

    loan* newLoan = (loan*)malloc(sizeof(loan));
    strcpy(newLoan->type, loanType);
    newLoan->totalamount = totalAmount;
    newLoan->totalinstallmentnum = totalInstallmentNum;
    strcpy(newLoan->processdate, processDate);
    newLoan->nextloan = NULL;
    newLoan->insptr = NULL;


    if (temp != NULL) {
        if (temp->loanptr == NULL) {
            temp->loanptr = newLoan;
        }
        else {
            loan* current = temp->loanptr;
            while (current->nextloan != NULL) {
                current = current->nextloan;
            }
            current->nextloan = newLoan;
        }
    }

    loanSort(customers);
}


void printLoans(customer* customers){
    customer* temp = customers;
    while(temp != NULL){
        printf("--------------------------------------------------\n");
        printf("%d - %s %s - type : %s total debt : %.0f\n", temp->customerid, temp->name, temp->surname, temp->customertype, temp->totaldebt);
        loan* tempLoan = temp->loanptr;
        if(tempLoan == NULL){
            printf("        no loan\n");
        }
        while(tempLoan != NULL){
            printf("    %s : %s - %f - %s - %d\n",tempLoan->loanid , tempLoan->type, tempLoan->totalamount, tempLoan->processdate, tempLoan->totalinstallmentnum);
            tempLoan = tempLoan->nextloan;
        }
        temp = temp->nextcust;
    }
}

void createLoanID(customer** customers){
    customer* temp = *customers;

    int custNum = 1;
    while(temp != NULL){
        loan* tempLoan = temp->loanptr;
        int loanNum = 1;
        while(tempLoan != NULL){
            sprintf(tempLoan->loanid, "%dL%d", custNum, loanNum);
            tempLoan = tempLoan->nextloan;
            loanNum++;
        }
        temp = temp->nextcust;
        custNum++;
    }
}

void readLoans(customer** customers, const char* filename){
    FILE* loans_txt = fopen(filename, "r");
    char line[150];
    char name[20];
    char surname[30];
    char loanType[30];
    float totalAmount;
    int totalInstallmentNum;
    char processDate[11];
    int customerid;

    if (loans_txt == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), loans_txt) != NULL) {
        sscanf(line, "%s %s %s %f %d %s", name, surname, loanType, &totalAmount, &totalInstallmentNum, processDate);
        insertLoan(customers, name, surname, loanType, totalAmount, totalInstallmentNum, processDate);

    }
    createLoanID(customers);
    fclose(loans_txt);
}

void createInstallments(customer** customers){
    customer* temp = *customers;
    int installmentNum = 1;

    int day = 0;
    int month = 0;
    int year = 0;

    while(temp != NULL){
        loan* tempLoan = temp->loanptr;

        while(tempLoan != NULL){
            sscanf(tempLoan->processdate, "%d/%d/%d", &day, &month, &year);

            while(installmentNum <= tempLoan->totalinstallmentnum){
                installment* newInstallment  = (installment*)malloc(sizeof(installment));
                sprintf(newInstallment->insid, "%sI%d", tempLoan->loanid, installmentNum);
                newInstallment->ispaid = 0;
                newInstallment->amount = tempLoan->totalamount / tempLoan->totalinstallmentnum;

                //Date arrange
                if(month > 12){
                    month = 1;
                    year++;
                }
                sprintf(newInstallment->installmentdate, "%02d/%02d/%d", day, month, year);
                month++;

                newInstallment->nextins = NULL;

                if(temp != NULL){
                    if(tempLoan != NULL){
                        if(tempLoan->insptr == NULL){
                            tempLoan->insptr = newInstallment;
                        }
                        else{
                            installment* current = tempLoan->insptr;
                            while(current->nextins != NULL){
                                current = current->nextins;
                            }
                            current->nextins = newInstallment;
                        }
                    }
                }

                installmentNum++;
            }
            tempLoan = tempLoan->nextloan;

            installmentNum = 1;
        }
        temp = temp->nextcust;
    }
}

void printInstallments(customer* customers){
    customer* temp = customers;
    while(temp != NULL){
        printf("--------------------------------------------------\n");
        printf("%d - %s %s - type : %s total debt : %.0f\n", temp->customerid, temp->name, temp->surname, temp->customertype, temp->totaldebt);
        loan* tempLoan = temp->loanptr;
        if(tempLoan == NULL){
            printf("        no loan\n");
        }
        while(tempLoan != NULL){
            printf("    %s : %s - %f - %s - %d\n",tempLoan->loanid , tempLoan->type, tempLoan->totalamount, tempLoan->processdate, tempLoan->totalinstallmentnum);
            installment* tempInstallment = tempLoan->insptr;
            while(tempInstallment != NULL){
                char paymentStatusMessage[16];
                if(tempInstallment->ispaid == 0){
                    strcpy(paymentStatusMessage, "To be Paid");
                }
                else if(tempInstallment->ispaid == 1){
                    strcpy(paymentStatusMessage, "Paid");
                }
                else{
                    strcpy(paymentStatusMessage, "Delayed Payment");
                }
                printf("        %s -> %s - %f - %s\n", tempInstallment->insid, tempInstallment->installmentdate, tempInstallment->amount, paymentStatusMessage);
                tempInstallment = tempInstallment->nextins;
            }
            tempLoan = tempLoan->nextloan;
        }
        temp = temp->nextcust;
    }
}

void markInstallmentAsPaid(customer** customers, int customerIdInt, char installmentId[], char allId[], char insid[]) {
    customer* tempCust = *customers;
    while (tempCust != NULL) {
        loan* tempLoan = NULL;
        if(tempCust->customerid == customerIdInt){
            tempLoan = tempCust->loanptr;
        }
        while (tempLoan != NULL) {
            installment* tempIns = NULL;
            if (strcmp(tempLoan->loanid, allId) == 0) {
                tempIns = tempLoan->insptr;
            }
                while (tempIns != NULL) {
                    if(strcmp(installmentId, "ALL") == 0){
                        tempIns->ispaid = 1;
                    }
                    if (strcmp(tempIns->insid, insid) == 0) {
                        tempIns->ispaid = 1;
                        return;
                    }
                    tempIns = tempIns->nextins;
                }
            tempLoan = tempLoan->nextloan;
        }
        tempCust = tempCust->nextcust;
    }
}


void readPayments(customer** customers, const char* filename) {
    FILE* payments_txt = fopen(filename, "r");
    char line[150];
    char customerId[3];
    char loanId[3];
    char installmentId[4];
    char allId[6];
    char insid[30];


    if (payments_txt == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), payments_txt) != NULL) {
        sscanf(line, "%[^L]L%[^ ] %[^\n]", customerId, loanId, installmentId);
        sscanf(line, "%s %s", allId, installmentId);
        if(strcmp(installmentId, "ALL") != 0){
            sprintf(insid, "%sI%s", allId, installmentId);
        }
        int customerIdInt = atoi(customerId);
        markInstallmentAsPaid(customers, customerIdInt, installmentId, allId, insid);
    }

    fclose(payments_txt);
}



void findUnpaidInstallments(customer** customers){
    char inputDate[11];
    double totalDebt = 0;
    int delayedInstallments = 0;
    int day1, day2, month1, month2, year1, year2;

    printf("please enter date:\n");
    scanf("%s", inputDate);

    sscanf(inputDate, "%d/%d/%d", &day1, &month1, &year1);


    customer* tempCust = *customers;
    while (tempCust != NULL) {
        loan* tempLoan = NULL;
        tempLoan = tempCust->loanptr;

        while (tempLoan != NULL) {
            installment* tempIns = NULL;
            tempIns = tempLoan->insptr;

            while (tempIns != NULL) {
                sscanf(tempIns->installmentdate, "%d/%d/%d", &day2, &month2, &year2);
                if (year1 > year2 || (year1 == year2 && (month1 > month2 || (month1 == month2 && day1 > day2)))){
                    if(tempIns->ispaid != 1){
                        tempIns->ispaid = 2;
                        delayedInstallments++;
                        totalDebt += tempIns->amount;
                    }
                }

                tempIns = tempIns->nextins;
            }
            tempLoan = tempLoan->nextloan;
        }
        if(delayedInstallments != 0){
            printf("%s %s : Debt %.2f Number of Delayed Installments %d\n", tempCust->name, tempCust->surname, totalDebt, delayedInstallments);
        }
        tempCust->totaldebt = totalDebt;
        tempCust = tempCust->nextcust;
        totalDebt = 0;
        delayedInstallments = 0;
    }
}

void deletePaidInstallments(customer** customers) {
    customer* tempCust = *customers;

    while (tempCust != NULL) {
        loan* tempLoan = tempCust->loanptr;

        while (tempLoan != NULL) {
            int isAllPaid = 1;
            installment* tempIns = tempLoan->insptr;

            while (tempIns != NULL) {
                if (tempIns->ispaid != 1) {
                    isAllPaid = 0;
                    break;
                }
                tempIns = tempIns->nextins;
            }

            if (isAllPaid == 1) {
                installment* tempIns = tempLoan->insptr;
                while (tempIns != NULL) {
                    installment* nextIns = tempIns->nextins;
                    free(tempIns);
                    tempIns = nextIns;
                }
                tempLoan->insptr = NULL;

                loan* prevLoan = NULL;
                loan* currLoan = tempCust->loanptr;
                while (currLoan != NULL) {
                    if (currLoan == tempLoan) {
                        if (prevLoan == NULL) {
                            tempCust->loanptr = tempLoan->nextloan;
                        } else {
                            prevLoan->nextloan = tempLoan->nextloan;
                        }
                        free(tempLoan);
                        break;
                    }
                    prevLoan = currLoan;
                    currLoan = currLoan->nextloan;
                }
            }
            tempLoan = tempLoan->nextloan;
        }
        tempCust = tempCust->nextcust;
    }
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
                readLoans(&customers, "loans.txt");
                break;
            case 4:
                //printLoans function call here
                printLoans(customers);
                break;
            case 5:
                //createInstallments function call here
                createInstallments(&customers);
                break;
            case 6:
                //printInstallments function call here
                printInstallments(customers);
                break;
            case 7:
                //readPayments function call here
                readPayments(&customers, "payments.txt");
                break;
            case 8:
                //findUnpaidInstallments function call here
                findUnpaidInstallments(&customers);
                break;
            case 9:
                //DeletePaidInstallments function call here
                deletePaidInstallments(&customers);
                break;
            case 0:
                exit(0);
                break;
            default :
                printf("invalid option\n");
        }
    }
    return 0;
}
