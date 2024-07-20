#include<iostream>
#include<fstream>
#include<cstdlib>
#include<map>
#include<vector>

using namespace std;

#define MIN_BALANCE 500
class InsufficientFunds{};
class Account{
private:
    long AccountNumber;
    string FirstName;
    string LastName;
    double Balance;
    static long NextAccountNumber;
public:
    Account(){};
    Account(string FName, string LName, double Bal);
        long getAccountNum(){return AccountNumber;}
        string getFirstName(){return FirstName;}
        string getLastName(){return LastName;}
        double getBalance(){return Balance;}    

        void Deposit(double amount);
        void Withdraw(double amount);
        static void setLastAccountNumber(long AccountNumber);
        static long getLastAccountNumber();
        friend ofstream & operator<<(ofstream &ofs,Account &acc);
        friend ifstream & operator>>(ifstream &ifs,Account &acc);
        friend ostream & operator<<(ostream &os,Account &acc);

};
long Account:: NextAccountNumber=0;
class Bank{
    private:
    map<long,Account> accounts;
    public:
    Bank();
    Account OpenAccount(string fname,string lname,double balance);
    Account Transfer(long fromAcc,long toAcc,double Amount);
    Account BalanceEnquiry(long AccountNumber);
    Account Withdraw(long AccountNumber,double amount);
    Account Deposit(long AccountNumber,double amount);
    void CloseAccount(long AccountNumber);
    void ShowAllAccounts();
    ~Bank();

};
int main(){
    Bank b;
    Account acc;
    int choice;
    string fname,lname;
    double balance;
    long AccountNumber;
    double amount;
    cout<<"***BANK***"<<endl;
    do{
        cout<<"\n\tSelect any one of the options below";
        cout<<"\n\t1 Open Bank account";
        cout<<"\n\t2 Balance Enquiry";
        cout<<"\n\t3 Deposit";
        cout<<"\n\t4 Withdrawal";
        cout<<"\n\t5 Close an account";
        cout<<"\n\t6 Show all accounts";
        cout<<"\n\t7 Transfer";
        cout<<"\n\t8 Quit";
        cout<<"\n\tEnter your choice:\n";
        cin>> choice;
        switch (choice){
            case 1:
                cout<<"Enter first name:\n";
                cin>>fname;
                cout<<"Enter last name:\n";
                cin>>lname;
                cout<<"Initial Balance:\n";
                cin>>balance;
                acc=b.OpenAccount(fname,lname,balance);
                cout<<"Congratulations Account has been created"<<endl;
                cout<<acc;
                break;
            case 2:
                cout<<"Enter account number:\n";
                cin>>AccountNumber;
                acc=b.BalanceEnquiry(AccountNumber);
                cout<<"Your account details"<<endl;
                cout<<acc;
                break;
            case 3:
                cout<<"Enter account number:\n";
                cin>>AccountNumber;
                cout<<"Enter the amount:\n";
                cin>>amount;
                acc=b.Deposit(AccountNumber,amount);
                cout<<"Amount is deposited"<<endl;
                cout<<acc;
                break;
            case 4:
                cout<<"Enter account number:\n";
                cin>>AccountNumber;
                cout<<"Enter amount:\n";
                cin>>amount;
                acc=b.Withdraw(AccountNumber,amount);
                cout<<"Amount is Withdrawn"<<endl;
                cout<<acc;
                break;
            case 5:
                cout<<"Enter account number:\n";
                cin>>AccountNumber;
                b.CloseAccount(AccountNumber);
                cout<<"Account has been deleted"<<endl;
                break;
            case 6:
                b.ShowAllAccounts();
                break;
            case 7:
            long fromAcc,toAcc;
            cout<<"Enter account(source) number:\n";
            cin>>fromAcc;
            cout<<"Enter account(destination) number:\n";
            cin>>toAcc;
            cout<<"Enter the amount:\n";
            cin>>amount;
            acc=b.Transfer(fromAcc,toAcc,amount);
            cout<<"Amount transfered successfully";
            cout<< acc;
            break;

            case 8:
                break;
            default:
                cout<<"\nEnter the right choice"<<endl;
                exit(0);
        }}
        while(choice!=7);
        return 0;

  }
  Account :: Account(string fname,string lname,double bal){
        NextAccountNumber++;
        AccountNumber=NextAccountNumber;
        this->FirstName=fname;
        this->LastName=lname;
        this->Balance=bal;

        
 
  }
void Account::Deposit(double amount){
    Balance=Balance+amount;

};
void Account::Withdraw(double amount){
  
    if(Balance-amount<MIN_BALANCE){
        throw InsufficientFunds();
    }
    Balance=Balance-amount;

}
void Account::setLastAccountNumber(long AccountNumber){
    NextAccountNumber=AccountNumber;
}
long Account::getLastAccountNumber(){
    return NextAccountNumber;
}
ofstream & operator<<(ofstream &ofs,Account &acc){
    ofs<<acc.AccountNumber<<endl;
    ofs<<acc.FirstName<<endl;
    ofs<<acc.LastName<<endl;
    ofs<<acc.Balance<<endl;
    return ofs;

}
ifstream & operator>>(ifstream &ifs,Account &acc){
    ifs>>acc.AccountNumber;
    ifs>>acc.FirstName;
    ifs>>acc.LastName;
    ifs>>acc.Balance;
    return ifs;

}
ostream & operator<<(ostream &os,Account &acc){
    os<<"First Name "<<acc.getFirstName()<<endl;
    os<<"Second Name "<<acc.getLastName()<<endl;
    os<<"Account number "<<acc.getAccountNum()<<endl;
    os<<"Balance "<<acc.getBalance()<<endl;
    return os;

}
Bank::Bank()
{
    Account account;
    ifstream infile;
    infile.open("Bank.data");
    if(!infile){
        cout<<"File not found"<<endl;
        return;
    }
    while(infile>>account){
        
        accounts.insert(pair<long,Account>(account.getAccountNum(),account));

    }
    if(!accounts.empty()){
	
    Account::setLastAccountNumber(accounts.rbegin()->second.getAccountNum());
}
    infile.close();
}
Account Bank :: Transfer(long fromAcc,long toAcc,double amount){
    map<long,Account>::iterator fromitr=accounts.find(fromAcc);
    map<long,Account>::iterator toitr=accounts.find(toAcc);
    if(fromitr== accounts.end()){
        cout<<"Source account not found";
        throw ("Source not found");
    }
    if(toitr== accounts.end()){
        cout<<"Destination account not found";
        throw ("Destination not found");
    }
    fromitr->second.Withdraw(amount);
    toitr->second.Deposit(amount);
    return fromitr->second;
}
Account Bank::OpenAccount(string fname,string lname,double balance)
{
 ofstream outfile;
 Account account(fname,lname,balance);
 accounts.insert(pair<long,Account>(account.getAccountNum(),account));
 
 outfile.open("Bank.data", ios::trunc);
 
 map<long,Account>::iterator itr;
 for(itr=accounts.begin();itr!=accounts.end();itr++)
 {
 outfile<<itr->second;
 }
 outfile.close();
 return account;
}
Account Bank::BalanceEnquiry(long accountNumber)
{
 map<long,Account>::iterator itr=accounts.find(accountNumber);
 return itr->second;
}
Account Bank::Deposit(long accountNumber,double amount)
{
 map<long,Account>::iterator itr=accounts.find(accountNumber);
 itr->second.Deposit(amount);
 return itr->second;
}
Account Bank::Withdraw(long accountNumber,double amount)
{
 map<long,Account>::iterator itr=accounts.find(accountNumber);
 itr->second.Withdraw(amount);
 return itr->second;
}
void Bank::CloseAccount(long accountNumber)
{
 map<long,Account>::iterator itr=accounts.find(accountNumber);
 cout<<"Account Deleted"<<itr->second;
 accounts.erase(accountNumber);
}
void Bank::ShowAllAccounts()
{
 map<long,Account>::iterator itr;
 for(itr=accounts.begin();itr!=accounts.end();itr++)
 {
 cout<<"Account "<<itr->first<<endl<<itr->second<<endl;
 }
}
Bank::~Bank()
{
 ofstream outfile;
 outfile.open("Bank.data", ios::trunc);
 
 map<long,Account>::iterator itr;
 for(itr=accounts.begin();itr!=accounts.end();itr++)
 {
 outfile<<itr->second;
 }
 outfile.close();
    

}


    

    
