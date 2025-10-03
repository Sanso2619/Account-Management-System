#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>

//for MACOS
int getch(void) {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);\
    return ch;
}

int menu();
void signup();
void login();
void forgot();
void change_password();
void delete_account();

struct account {
    char first_name[20];
    char last_name[20];
    char username[30];
    char phone[15];
    char gender;
    char DOB[11];
    char pass1[20];
};

void divider() {
    for (int i = 0; i < 50; i++) {
        printf("-");
    }
}


int main() {
    while (1) {
        system("clear");
        switch (menu()) {
            case 1:
                signup();
                break;
            case 2:
                login();
                break;
            case 3:
                forgot();
                break;
            case 4:
                change_password();
                break;
            case 5:
                delete_account();
                break;
            case 6:
                exit(0);
            default:
                printf("Invalid Choice!\n");
        }
    }
    return 0;
}


int menu() {
    int c;
    printf("\n******************MAIN MENU******************\n");
    divider();
    printf("\n1.SIGNUP\n");
    printf("2.LOGIN\n");
    printf("3.FORGOT PASSWORD OR USERNAME\n");
    printf("4.CHANGE PASSWORD\n");
    printf("5.DELETE ACCOUNT\n");
    printf("6.EXIT\n");
    divider();
    printf("\nEnter your choice : ");
    scanf("%d", &c);
    while (getchar() != '\n');
    return c;
}

// to enter password:
void takepassword(char pass[20]) {
    int i = 0;
    char ch;
    while (1) {
        ch = getch();
        if (ch == '\n') {
            pass[i] = '\0';
            break;
        } else if (ch == 8 || ch == 127) {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else if (ch == 9 || ch == 32) {
            continue;
        } else {
            if (i < 19) {
                pass[i++] = ch;
                printf("*");
            }
        }
    }
}

// To create an account :
void signup() {
    struct account u;
    char pass2[20];
    system("clear");
    printf("CREATING NEW ACCOUNT \n");
    divider();
    printf("\nENTER your first name : ");
    scanf("%s", u.first_name);
    printf("Enter last name : ");
    scanf("%s", u.last_name);
    printf("Enter your username : ");
    scanf("%s", u.username);
    printf("Enter your phone number : ");
    scanf("%s", u.phone);
    while (getchar() != '\n');
    printf("Enter Gender : ");
    scanf("%c", &u.gender);
    printf("Enter your DOB (DD/MM/YYYY) : ");
    scanf("%s", u.DOB);
    while (getchar() != '\n');

    printf("Enter password : ");
    takepassword(u.pass1);

conf_pass:
    printf("\nConfirm password : ");
    takepassword(pass2);

    if (strcmp(u.pass1, pass2)) {
        printf("\nINCORRECT PASSWORD \n\n");
        goto conf_pass;
    } else {
        FILE *fp = fopen("account.txt", "w");
        fwrite(&u, sizeof(struct account), 1, fp);
        fclose(fp);
        printf("\nACCOUNT CREATED SUCCESSFULLY\n");
        printf("Press Enter to continue...");
        getchar();
    }
}

//To login account:
void login() {
    struct account u;
    char user[30], log_pass[20];
    FILE *fp = fopen("account.txt", "r");
    if (fp == NULL) {
        printf("No account found.\n");
        getchar();
        return;
    }
    fread(&u, sizeof(struct account), 1, fp);
    fclose(fp);
    system("clear");

name:
    printf("Enter Username : ");
    scanf("%s", user);
    while (getchar() != '\n');
    if (strcmp(u.username, user)) {
        printf("\nPLEASE ENTER CORRECT USERNAME\n\n");
        goto name;
    } else {
    logpass:
        printf("Enter password : ");
        takepassword(log_pass);
        if (strcmp(u.pass1, log_pass)) {
            printf("\nINCORRECT PASSWORD \n");
            goto logpass;
        } else {
            system("clear");
            printf("\n************WELCOME %s************\n", u.first_name);
            divider();
            printf("\nYour Details \n");
            divider();
            printf("\nUsername     : %s\n", u.username);
            printf("First name   : %s\n", u.first_name);
            printf("Last name    : %s\n", u.last_name);
            printf("Phone number : %s\n", u.phone);
            printf("Gender       : %c\n", u.gender);
            printf("Password     : %s\n", u.pass1);
            printf("\nPress Enter to continue...");
            getchar();
        }
    }
}

//To recreate password:
void forgot() {
    struct account u;
    int choice;
    char username[20], password[20], phone[20];
    FILE *fp = fopen("account.txt", "r");
    if (fp == NULL) {
        printf("No account file found.\n");
        getchar();
        return;
    }
    fread(&u, sizeof(struct account), 1, fp);
    fclose(fp);

    printf("\n1.Search by Username\n");
    printf("2.Search by Password\n");
    printf("3.Search by Phone no.\n");
    printf("4.MAIN MENU\n");
    divider();
    printf("\nEnter your choice : ");
    scanf("%d", &choice);
    while (getchar() != '\n');

    switch (choice) {
        case 1:
            printf("Enter your Username : ");
            scanf("%s", username);
            if (strcmp(username, u.username)) {
                printf("USERNAME NOT FOUND !!");
            } else {
                printf("ACCOUNT FOUND!\n");
                divider();
                printf("\nUsername    : %s \n", u.username);
                printf("First name  : %s \n", u.first_name);
                printf("Last name   : %s \n", u.last_name);
                printf("Phone no.   : %s \n", u.phone);
                printf("Password    : %s \n", u.pass1);
            }
            break;
        case 2:
            printf("Enter your Password : ");
            scanf("%s", password);
            if (strcmp(password, u.pass1)) {
                printf("ACCOUNT NOT FOUND !!");
            } else {
                printf("ACCOUNT FOUND!\n");
                divider();
                printf("\nUsername    : %s \n", u.username);
                printf("First name  : %s \n", u.first_name);
                printf("Last name   : %s \n", u.last_name);
                printf("Phone no.   : %s \n", u.phone);
                printf("Password    : %s \n", u.pass1);
            }
            break;
        case 3:
            printf("Enter your Phone no. : ");
            scanf("%s", phone);
            if (strcmp(phone, u.phone)) {
                printf("ACCOUNT NOT FOUND !!");
            } else {
                printf("ACCOUNT FOUND!\n");
                divider();
                printf("\nUsername    : %s \n", u.username);
                printf("First name  : %s \n", u.first_name);
                printf("Last name   : %s \n", u.last_name);
                printf("Phone no.   : %s \n", u.phone);
                printf("Password    : %s \n", u.pass1);
            }
            break;
        case 4:
            return;
        default:
            printf("INVALID CHOICE\n");
            break;
    }
    printf("\nPress Enter to continue...");
    getchar();
}

//To change password:
void change_password() {
    system("clear");
    int num1, num2;
    srand(time(0));

number:
    num1 = rand() % 10000;
    printf("\nThe random number is %d\n", num1);
    printf("Enter the above number : ");
    scanf("%d", &num2);
    while (getchar() != '\n');

    if (num1 == num2) {
        struct account u;
        char pass2[20];

        FILE *fp = fopen("account.txt", "r");
        fread(&u, sizeof(u), 1, fp);
        fclose(fp);

        printf("Enter new password : ");
        takepassword(u.pass1);

    pas:
        printf("\nConfirm new password : ");
        takepassword(pass2);
        if (strcmp(u.pass1, pass2)) {
            printf("\n*******INCORRECT PASSWORD******\nPlease try again.....\n");
            goto pas;
        } else {
            fp = fopen("account.txt", "w");
            fwrite(&u, sizeof(u), 1, fp);
            fclose(fp);
            printf("\n*********PASSWORD CHANGED SUCCESSFULLY*********\n");
        }
    } else {
        printf("Incorrect number. Try again.\n");
        goto number;
    }
    printf("Press Enter to continue...");
    getchar();
}

//To delete the account:
void delete_account() {
    system("clear");
    printf("ARE YOU SURE [Y/N]: ");
    char ch = getchar();
    while (getchar() != '\n');

    if (ch == 'Y' || ch == 'y') {
        FILE *fp = fopen("account.txt", "w");
        if (fp != NULL) {
            fprintf(fp, " ");
            fclose(fp);
            printf("\nACCOUNT DELETED SUCCESSFULLY\n");
        }
    } else {
        printf("\nACCOUNT DELETION CANCELLED\n");
    }
    printf("Press Enter to continue...");
    getchar();
}
