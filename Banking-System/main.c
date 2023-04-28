#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>


#define FILENAME "records.txt"
#define KEY 3
#define MAX_ACCOUNTS 100
#define BLOCK_SIZE 16

//This struct will be the basis of saving the info of the client to the file
typedef struct {
    char    name[50];
    char    account_number[10];
    char    date_of_birth[12]; //ISO Format
    char    PIN[10];
    char    encrypted_account_bal[20];
    double  account_balance;
    
}ACCOUNT;


typedef struct {
    char     account_number[10];      // Account number (e.g. "123456789")
    char     transaction_type[20];    // Transaction type (e.g. "Deposit", "Withdrawal", etc.)
    double   transaction_amount;    // Transaction amount
    char     transaction_timestamp[30]; // Transaction timestamp (e.g. "2022-05-01 12:34:56")
} TRANSACTION_LOG;


//account Management
void login();
void register_account();
void withdraw(ACCOUNT client);
void deposit(ACCOUNT client);
void transfer(ACCOUNT sender, ACCOUNT Reciever);
void balance_inquery(ACCOUNT* client);
void close_account(ACCOUNT);

//File Handling Encrption
void save();
ACCOUNT retrieve_account(const char account_number[10]);
bool delete(const char* account_number);
void encrypt();
void decrypt();

// Account information management
void view_account_details(ACCOUNT client);
void edit_account_details(ACCOUNT client);

// Transaction history management
void view_transaction_history(ACCOUNT client);
void clear_transaction_history(ACCOUNT client);

// Error handling and validation
bool is_valid_account_number(char account_number[10]);
bool is_valid_amount(double amount);

// Security management
void change_PIN(ACCOUNT* client);

// Administrative tasks
void create_admin_account();
void delete_account(ACCOUNT* client);
void view_all_accounts();

/**
 * Logs different types of transactions to a transaction log file, including withdrawals,
 * deposits, transfers, and balance inquiries.
 */
void log_transaction(ACCOUNT client, double amount, const char* transaction_type);
void log_withdrawal(ACCOUNT client, double amount);
void log_deposit(ACCOUNT client, double amount);
void log_transfer(ACCOUNT sender, ACCOUNT receiver, double amount);
void log_balance_inquiry(ACCOUNT client);

// Display management
void display_account_summary(ACCOUNT* client);
void display_error_message(const char* message);
void display_success_message(const char* message);




/**
* Saves an account to a file. If the account already exists, it will be overwritten.
*
* @param {ACCOUNT} account - The account to be saved.
* @returns {void}
*/
void save(ACCOUNT account) {
    FILE* file = fopen(FILENAME, "r+b"); // Open the file in read and write mode
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }

    encrypt_account(&account);
    // Search for the account by account number
    ACCOUNT temp;
    while (fread(&temp, sizeof(ACCOUNT), 1, file) == 1) {
        if (strcmp(temp.account_number, account.account_number) == 0) {
            // Found the account, overwrite it with the new data
            fseek(file, -sizeof(ACCOUNT), SEEK_CUR);
            fwrite(&account, sizeof(ACCOUNT), 1, file);
            fclose(file);
            return;
        }
    }

    // Account not found, append it to the end of the file
    fwrite(&account, sizeof(ACCOUNT), 1, file);

    fclose(file);
}


/**
 * Retrieves an account from the file with the given account number.
 * @param {string} account_number - The account number to search for.
 * @return {ACCOUNT} - The account with the matching account number, or an empty account if not found.
 */
ACCOUNT retrieve_account(const char* account_number) {
    FILE* file = fopen(FILENAME, "rb"); // Open the file in binary mode for reading
    if (file == NULL) {
        perror("Error opening file for reading");
        return (ACCOUNT) {0}; // return empty account if file couldn't be opened
    } else {
        ACCOUNT account;
        while(fread(&account, sizeof(ACCOUNT), 1, file) == 1 ){// Retrieve each account
            decrypt_account(&account);
            if (strcmp(account.account_number, account_number) == 0) {
                fclose(file);
                return account; // Return found account
            }
        }
    }
    fclose(file);
    return (ACCOUNT) {0}; // return empty account if account number not found
}


//Encryption

void swap_chars(char* str, int key) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        int ascii_code = (int) str[i];
        ascii_code += key;
        str[i] = (char) ascii_code;
    }
}

void restore_chars(char* str, int key) {
    swap_chars(str, -key);
}

void encrpyt_decimal(double amount, char* str) {
    int convert_amount = (round(amount * 100.0) / 100.0) * 100;
    sprintf(str, "%d", convert_amount);
    swap_chars(str, KEY);
}

double decrypt_decimal(double amount, char* str) {
    restore_chars(str, KEY);
    int converted_amount = atoi(str);
    return (double) converted_amount / 100;
}


void encrypt_account(ACCOUNT* account) {
    swap_chars(account->name, KEY);
    swap_chars(account->account_number, KEY);
    swap_chars(account->date_of_birth, KEY);
    swap_chars(account->PIN, KEY);
    encrpyt_decimal(account->account_balance, account->encrypted_account_bal);
    account->account_balance = 0;
}

void decrypt_account(ACCOUNT* account){
    restore_chars(account->name, KEY);
    restore_chars(account->account_number, KEY);
    restore_chars(account->PIN, KEY);
    restore_chars(account->date_of_birth, KEY);
    account->account_balance = decrypt_decimal(account->account_balance, account->encrypted_account_bal);

}


int main()
{
    
    ACCOUNT account1 = {
    "Jhon Philip Guiang",
    "123456781",
    "2000-01-01",
    "1234",
    "",
    5069.50
    };

    ACCOUNT account2 = {
        "Jane Smiths",
        "987654311",
        "1995-05-25",
        "5678",
        "",
        10000.00
    };

    save(account1);
    ACCOUNT acc = retrieve_account("123456781");
    printf("%f", acc.account_balance);


    return 0;
}
