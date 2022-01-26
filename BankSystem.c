#include <stdio.h>
#include <string.h>

/***********
*structure * 
* **********/


struct Date {
    int day;
    int month;
    int year;
};

// Bank Account structure - one person can have several accounts
struct Activity {
    float Amount; // if negative it means withdraw otherwise its a deposit.
    struct Date date;
    char Description[40];
};

// Bank Account structure - one person can have several accounts
struct BankAccount {
    int AccountNumber; // Uniuque
    int ownerID;
    double Balance;
    int numOfActivities;
    struct Activity activity[1000];
};

// User structure
struct User {
    int id; // Uniuque
    char FirstName[12], LastName[24];
};

struct CustomerDatabase {
    int numOfUsers;
    struct User user[100];
}DB = {1, {{123456789, "Elon", "Musk"}}}; // dummy data

struct _AccountsDatabase{
    int numAccounts;
    struct BankAccount accounts[300];
}AccountsDatabase = {1, {{10000000, 123456789, 152.01, 0}}}; // dummy data

/* The purpose of this structure is to hold the information
of the current logged user  */
struct State{
    int userID;
    int currentAccount;
    int userIndex;
    struct BankAccount *accounts[3];
} currentUser = {-1, -1, -1};



/***********
* Functions* 
* **********/

/* USER HANDLERS  */

// Simply count how many digits in a given number!
int idValidator(int id){
    int count = 0;
    do {
        id /= 10;
        ++count;
    } while (id != 0);;
    return count;
    printf("Count, %d\n", count);
}



/* Make sure that the inputed ID is unique\
Find a specific id in the database */
int FindID(){
    int TheidToCheck;
    do
    {
        printf("\tEnter your national idenification number \n\t(Must be 9 digits)");
        printf(" ID -> ");
        scanf("%9d", &TheidToCheck);
        if(TheidToCheck <= 0){
            printf("\n\tID Must be a positive number\n");
        }
        else if (idValidator(TheidToCheck) != 9) 
        {
            printf("\tID Must be 9 digits long!\n"); // loop until the given number is 9 digits
        }
        
    } while (idValidator(TheidToCheck) != 9);
    
    for (int i = 0; i < DB.numOfUsers; ++i){
        if (DB.user[i].id == TheidToCheck) return TheidToCheck*-1; // id exists
    }

    return TheidToCheck;
}

int auth(int userID, char FirstName[12], char LastName[24]){
    for (int i = 0; i < DB.numOfUsers; i++)
    {
        
        if ((DB.user[i].id == userID) && (strcmp(DB.user[i].FirstName, FirstName) == 0) &&
				(strcmp(DB.user[i].LastName, LastName) == 0))
        return i;
    }
    return -1;
    
}


/* (gets personal info from the user) */
struct User inputUser(void){
    struct User user;

    printf("\t############[Create a new user]##############\n");
    printf("\tPlease answer the questions below\n\n");

    printf("\tEnter your First name and Last name\n\te.g Elon Musk -> ");
    scanf("\t%s %s", user.FirstName, user.LastName);

    do
	{
		user.id = FindID();
		if (user.id < 0)
			printf("\tID is already in use\n");
	} while (user.id < 0);
    printf("\n");
    return user;
}

/* This function loops over the whole user Db find the last user index and place the
the inputed structure right after it and returns the index */
int AddUserToDB(void){

    int nextBook = DB.numOfUsers;
	DB.user[nextBook] = inputUser();
    DB.numOfUsers++;
    
}

/* Create User | This funcrion 
    creates users aka customers, */
int CreateNewUser(){
    if (100 <= DB.numOfUsers){
        printf("Database is full!");
        return 0;
    }
    AddUserToDB();
}

/* return user ID if authentication was sucessful, otherwise
return -1 */
int userAuth(){
    struct User usertemp;

    printf("\n\t###################[Login]###################\n");
    printf("\tPlease answer the questions below\n\n");

    printf("\tEnter your First name and Last name\n\te.g Elon Musk -> ");
    scanf("%s %s", usertemp.FirstName, usertemp.LastName);

	currentUser.userID = FindID();
	if (currentUser.userID > 0){
        printf("\n\tID doesnt not exist\n");
        currentUser.userID = -1;
        return -1;
    }
    currentUser.userID *= -1;
    int output = auth(currentUser.userID, usertemp.FirstName, usertemp.LastName);
    if(output == -1){
        printf("\t[Authentication failed]: User Not Found!\n\n");
        currentUser.userID = -1;
    }; 

    return output; // if successful, return the user ID otherwise -1;

}



/* BANK HANDLERS  */ // =====================================================================================


/**
 * @brief 
 * In order to make a bank account we need a function
 * to generate a bank number and assign the bank to the users ID somehow
 * then add it to the accounts database and display something like a success msg 
 */

/* Generate a bank account number |  */
struct BankAccount generateBankcredentials(void){
    struct BankAccount Account;

    int generateNum = AccountsDatabase.numAccounts;
    generateNum += 10000000; // turning account num into 8 digits
    Account.AccountNumber = generateNum;
    Account.Balance = 0.00;
    Account.ownerID = currentUser.userID;
    Account.numOfActivities = 0;

    return Account;
}

/* Create bank account | Create Bank Account 
    And Assign it to the user */
void CreateBankAccount(){
    int nextbnk = AccountsDatabase.numAccounts;
    AccountsDatabase.accounts[nextbnk] = generateBankcredentials();
    AccountsDatabase.numAccounts++;
    printf("\n\n\tBank Account Made Successfully\n\tPress (2) to view or switch your accounts.\n\n");
}

int getBnkAccounts(int userID){
    int output = 0;
    for (int i = 0; i < AccountsDatabase.numAccounts; i++)
    {
        if (AccountsDatabase.accounts[i].ownerID == userID){
            currentUser.accounts[output] = &AccountsDatabase.accounts[i];
            ++output;
        } 
    }
    return output;    
}


/* IMPORTANT NOTE: THIS FUNCTION BELONGS TO THE USER SECTION */
void Logout(){
    struct Activity act;

    int amount = getBnkAccounts(currentUser.userID);
    currentUser.userID = -1;
    currentUser.currentAccount = 0;

    // reset accounts
    for (int i = 0; i < amount; i++)
    {
        currentUser.accounts[i] = NULL;
    }
    // [Logging Out...]
    printf("\n\t##############[Logging Out...]##############\n\n");
    
}

int dateValidate(struct Date date){
    if ((date.day < 0 || 31 < date.day) || (date.month < 0 || 12 < date.month))
    {
        printf("\tInvalid date\n");
        return -1;
    }
    return 0;
}


struct Date getdate(void){
    struct Date date;
    do
    {
        printf("\tEnter the date e.g. DD MM YYYY\n\t-> ");
        scanf("%d %d %d", &date.day, &date.month, &date.year);

    } while (dateValidate(date) != 0);

    return date;
}

// [[ACTIVITY SECTION]] --------------------------------------------------------------------------------------------------

/* This function handles input from the user regarding 
the deposit/withdraw activities*/
struct Activity createActivity(int action){
    struct Activity activity;
    struct Date date;
    char text[40] = { "\0" };

    do
    {
        printf("\tEnter Amount> "); // Get amount of withdrawal/deposit
        scanf("%f", &activity.Amount);
        if (activity.Amount < 0){
            printf("\tError: Amount can only be a positive number!\n");
        }
    } while (activity.Amount < 0);
    

    activity.date = getdate();

    (action < 0) ? strcpy(text, "essence of the withdrawal"): strcpy(text, "name of the depositor");
    printf("\tEnter the %s\n\t-> ", text);
    gets(text);
    strcpy(activity.Description, gets(text));

    if (activity.Amount == 0) // making sure that the amount is not 0
    {
        printf("\tError: Amount cannot be 0!\n");
        activity.Amount = -1;
    }
    

    // checking whether the user is trying to withdraw more that what they have in the account
    if (currentUser.accounts[currentUser.currentAccount]->Balance < activity.Amount && (action < 0)){
        printf("\tError: Insufficient funds!\n");
        activity.Amount = -1;
    }


    return activity;
}

/* Account Activity | This function handles user activities 
    such as deposit and withdraw */
void activityHandler(int action){
    struct Activity act;
    char title[10];
    (action < 0) ? strcpy(title, "Withdraw"): strcpy(title, "Deposit");
    printf("\t###########[Bank Activity - %s]################\n", title);

    int accID = currentUser.currentAccount;
    int numOfActs = currentUser.accounts[accID]->numOfActivities;
    act = createActivity(action);

    if (act.Amount != -1)
    {
        if (action < 0) act.Amount *= -1;
        currentUser.accounts[accID]->activity[numOfActs] = act;
        currentUser.accounts[accID]->numOfActivities++;
        currentUser.accounts[currentUser.currentAccount]->Balance += act.Amount;
    }
    

}

int convertDate(struct Date date){
    int time = 0;

    int year = date.year;
    int month = date.month;
    int day = date.day;

    time += 1 * day;
    time += (1*30) * month;
    time += ((1*30)*12) * year;

    return time;
}

void SwitchAccount(int userID){
    int max = getBnkAccounts(userID);
    int selection;
    
    printf("\n\n\tIn order to switch the current account make sure to\n");
    printf("\tEnter the number inside the parenthesis ()\n");
    for (int i = 0; i < max; i++){
        printf("(%d) - ", i);
        printf("Account Number: %d\t", currentUser.accounts[i]->AccountNumber);
        printf("Activities: %d\t", currentUser.accounts[i]->numOfActivities);
        printf("Balance: %.2f", currentUser.accounts[i]->Balance);
        printf("\n");
    }
    
    do
    {
        printf("\t\tSelect--> ");
        scanf("%d", &selection);
        (selection < 0 || max <= selection) ? printf("Make sure to select a number that exists in the list!\n"): 0;
    } while ((selection < 0) || (max <= selection));
    
    currentUser.currentAccount = selection;

}

// LIST AND DISPLAY FUNCTIONS ----------------------------------------------------------------------------------

/* This function will display
all users accounts given a userID */
void displayAccounts(int userID){
    int max = getBnkAccounts(userID);

    for (int i = 0; i < AccountsDatabase.numAccounts; i++)
    {
        if (AccountsDatabase.accounts[i].ownerID == userID){
            printf("Account Number: %d\t", currentUser.accounts[i]->AccountNumber);
            printf("Activities: %d\t", currentUser.accounts[i]->numOfActivities);
            printf("Balance: %.2f", currentUser.accounts[i]->Balance);
            printf("\n");
        } 
    }
}   




void displayActivity(struct Activity activity){
    
    char action[9] = {"\0"};
    (activity.Amount < 0) ? strcpy(action, "Withdraw") : strcpy(action, "Deposit");

    if (activity.Amount < 0){
        printf("%d/%d/%d\t%s\t$%.2f\t\t%s\n", activity.date.day, activity.date.month, activity.date.year, action, activity.Amount, activity.Description);
    }else{
        printf("%d/%d/%d\t%s\t\t$%.2f\t\t%s\n", activity.date.day, activity.date.month, activity.date.year, action, activity.Amount, activity.Description);
    }
    printf("-----------------------------------------------------------\n");

}




void ListActivity(){

    int max = currentUser.accounts[currentUser.currentAccount]->numOfActivities;
    struct Activity currentact;
    struct Date stateDate;
    struct Date endDate;

    printf("\n\t[Start Date]\n");
    stateDate = getdate();

    printf("\n\t[End Date]\n");
    endDate = getdate();

    int start = convertDate(stateDate);
    int end = convertDate(endDate);

    printf("Date\t\tAction\t\tAmount\t\tDescription\n");
    printf("----------------------------------------------------------------------------\n");

    for (int i = 0; i < max; i++)
    {
        currentact = currentUser.accounts[currentUser.currentAccount]->activity[i];
        int tempdate = convertDate(currentact.date);
        if (start < tempdate && tempdate < end) displayActivity(currentact);
    };
    
}
void displayAcc(int accountIndex){
    printf("\t[Account Number]: %d\t [Balance]: %.2f\n", AccountsDatabase.accounts[accountIndex].AccountNumber, AccountsDatabase.accounts[accountIndex].Balance);
}
void findAccounts(int userID){
    for (int i = 0; i < AccountsDatabase.numAccounts; i++)
    {
        if (AccountsDatabase.accounts[i].ownerID == userID){
            displayAcc(i);
        }
    }
    
}

void ListUsers(){
    printf("\n\t##############[List All Users]##############\n");
    int max = DB.numOfUsers;
    for (int i = 0; i < max; i++)
    {
        int SSN = DB.user[i].id;
        printf("[User]: %s %s\t\t[ID]: %d\n", DB.user[i].FirstName, DB.user[i].LastName, DB.user[i].id);
        findAccounts(DB.user[i].id);
    }
    printf("\n\n");

}

void HelloMsg(int parameter){
    if (parameter > 0) {
        printf("\t\nNum of Accounts: %d |\t Current Account: %d |\t Balance: $%.2f\n\n", parameter, currentUser.accounts[currentUser.currentAccount]->AccountNumber, currentUser.accounts[currentUser.currentAccount]->Balance);
    }else{
        printf("\t\nNum of Accounts: %d |\tPress (1) To make a bank account\n\n", parameter);
    };
}

void DisplayInterface(int numaccounts){
    if (numaccounts == 0)
    {
        printf("1 - Make Bank Account\t\t3 - List All Users\n2 - Logout\t\t\t4 - End\n");
    }else{
        printf("1 - Make Bank Account\t\t5 - List Activity\n2 - Switch Bank Account\t\t6 - List All Users\n3 - Deposit\t\t\t7 - Logout\n4 - Withdraw\t\t\t8 - End\n");
    }
    
}



/******************
*System Controller* 
******************/


int main(){
    DB.numOfUsers = 1; // reset DB - do not read this!
    AccountsDatabase.numAccounts = 1; // do not read this!
    currentUser.currentAccount = 0; // do not read this!

    int selection;
    printf("\n\n");
    while (1)

    if (currentUser.userID == -1) // checking whether a user is loggedin
    {

        printf("1 - Login\t\t3 - List All Users\n2 - Create New Account\t4 - End\n");
        printf("\t\tSelect--> ");
        scanf("%d",&selection);
        switch (selection)
        {
        case 1:
            currentUser.userIndex = userAuth();
            break;
        case 2:
            CreateNewUser();
            break;
        case 3:
            ListUsers();
            break;
        case 4:
            return 0;
            break;
        }
    }else{
        int amountOfUseraccounts = getBnkAccounts(DB.user[currentUser.userIndex].id);
        printf("\n\n\t\tHello %s %s \n", DB.user[currentUser.userIndex].FirstName, DB.user[currentUser.userIndex].LastName);
        HelloMsg(amountOfUseraccounts); // Say Hi
        
        DisplayInterface(amountOfUseraccounts);
        printf("\t\tSelect--> ");

        scanf("%d",&selection);
        if (amountOfUseraccounts == 0) // Checking whether a user has a bank account and  display an interface accordingly
        {
            switch (selection)
            {
            case 1:
                CreateBankAccount();
                break;
            case 2:
                Logout();
                break;
            case 3:
                ListUsers();
                break;
            case 4:
                return 0;
                break;
            }
        }else{

        switch (selection)
        {
        case 1:
            CreateBankAccount();
            break;
        case 2:
            SwitchAccount(DB.user[currentUser.userIndex].id);
            break;
        case 3:
            activityHandler(1);
            break;
        case 4:
            activityHandler(-1);
            break;
        case 5:
            ListActivity();
            break;
        case 6:
            ListUsers();
            break;
        case 7:
            Logout();
            break;
        case 8:
            return 0;
            break;
        }
        }
        
    }

}
