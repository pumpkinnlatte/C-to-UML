class BankAccount {
public:
    std::string owner;
    double balance = 0.0;
    void deposit(double amount);
    void withdraw(double amount);
};

class CheckingAccount : public BankAccount {
public:
    double insufficientFundsFee;
    void processCheck(Check checkToProcess);
    void withdraw(double amount);
};

class SavingsAccount : public BankAccount {
public:
    double annualInterestRate;
    void depositMonthlyInterest();
    void withdraw(double amount);
};