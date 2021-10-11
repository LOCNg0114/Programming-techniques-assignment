#pragma once

#include <iostream>
#include <string>
#include "helper/helper.hpp"

class BankAccount {
public:
    BankAccount() 
    {
      initialNumber = "";
      accountNumber = "";
      accountName = "";
      bankName = "";
    };
    BankAccount(const std::string & initialNumberParam, const std::string & accountNumberParam, const std::string & accountNameParam, const std::string & bankNameParam)
    {
      //TODO Add validation for params
      initialNumber = initialNumberParam;
      accountNumber = accountNumberParam;
      accountName = accountNameParam;
      bankName = bankNameParam;
    };

    bool setInitialNumber(const std::string & newInitialNumber) {initialNumber = newInitialNumber;};
    std::string getInitialNumber() const {return initialNumber;};

    bool setAccountNumber(const std::string & newAccountNumber) {accountNumber = newAccountNumber;};
    std::string getAccountNumber() const {return accountNumber;};

    bool setAccountName(const std::string & newAccountName) {accountName = newAccountName;};
    std::string getAccountName() const {return accountName;}; 

    bool setBankName(const std::string & newBankName) {bankName = newBankName;};
    std::string getBankName() const {return bankName;}; 

    std::string getEncodedAccountNumber() const {
      int s = 0, splittedAccountNum;
      unsigned int accountNumberLength = accountNumber.length();
      std::string tempAccountNumber = accountNumber;

      for (int i = 0; i < accountNumberLength; i++) {   
        splittedAccountNum = tempAccountNumber.at(i) - '0';
        assert(0 <= splittedAccountNum && splittedAccountNum <= 9);
        s += splittedAccountNum;
      }
      int f = s % 3 + 3;

      // Start -- First implementation to split, fill before split 
      int amountOfNumberNeededToFill = f - (accountNumberLength % f); 
      for (int i = 1; i <= amountOfNumberNeededToFill; i++) {   
        tempAccountNumber = tempAccountNumber[0] + tempAccountNumber;
      }
      std::string filledAccountNumber = tempAccountNumber;
      int evenlySeparatedByFArrLength = std::ceil((double)accountNumberLength / f);
      std::string evenlySeparatedByFArr[evenlySeparatedByFArrLength];
      int tempCounter = 0;
      std::string tempNumber = "";
      for (int i = 0; i < accountNumberLength + amountOfNumberNeededToFill; i += f) {   
        tempNumber = tempAccountNumber.substr(i, f);
        evenlySeparatedByFArr[tempCounter] = tempNumber;
        tempCounter += 1;
      }
      // End -- First implementation to split, fill before split 

      // Start -- Second implementation to split, split then fill 
      // Propose refactor solution: Don't need to reverse array, just subtract from the end of array toward the start
      /* std::string revertedAccountNumber = accountNumber;
      std::string tempRevertedNumber = "";
      int evenlySeparatedByFArrLength = std::ceil((double)accountNumberLength / f);
      std::string evenlySeparatedByFArr[evenlySeparatedByFArrLength];
      std::reverse(revertedAccountNumber.begin(), revertedAccountNumber.end());
      int tempCounter = 0;
      for (int i = 0; i < revertedAccountNumber.length(); i += f) {
        tempRevertedNumber = revertedAccountNumber.substr(i, f);
        std::reverse(tempRevertedNumber.begin(), tempRevertedNumber.end());
        evenlySeparatedByFArr[tempCounter] = tempRevertedNumber;
        tempCounter += 1;
      }
      while (evenlySeparatedByFArr[evenlySeparatedByFArrLength - 1].length() < f) {
        evenlySeparatedByFArr[evenlySeparatedByFArrLength - 1] = accountNumber[0] + evenlySeparatedByFArr[evenlySeparatedByFArrLength - 1];
      } */
      // End -- Second implementation to split, split then fill 

      int k = 0;
      for (int i = 0; i < evenlySeparatedByFArrLength; i++) {
        k = k + std::stoi( evenlySeparatedByFArr[i] );
      }
      std::string stringK = std::to_string(k);
      std::string repeatedString[accountNumberLength + amountOfNumberNeededToFill];
      for (int x = 0; x < accountNumberLength + amountOfNumberNeededToFill; x += stringK.length()) {
        for (int y = 0; y < stringK.length(); y++ ){
          repeatedString[x+y] = stringK[y];
        }
      }

      int twoD[10][10];
      int oneD[10] = { 9, 0, 1, 2, 3, 4, 5, 6, 7, 8 };
      for (int x = 0; x < 10; x++) {
        int first = oneD[0];		
        for (int i = 1; i <= 10 - 1; i++) {
          oneD[i - 1] = oneD[i];
        }
        oneD[10 - 1] = first;
        for (int y = 0; y < 10; y++) {
          twoD[x][y] = oneD[y];
        }
      }

      std::string finalString = filledAccountNumber;
      for (int i = 0; i < filledAccountNumber.length(); i++) {
        unsigned int temp1 = std::stoi(repeatedString[i]);
        unsigned int temp2 = filledAccountNumber.at(i) - '0';
        finalString.replace(i, 1, std::to_string(twoD[temp1][temp2]));
      }

      /* comment
      50 % 52 = 2

      222129082021
      454454454454
    
      676573436475

      X 0 1 2 3 4 5 6 7 8 9
      0 0 1 2 3 4 5 6 7 8 9
      1 1 2 3 4 5 6 7 8 9 0
      2 2 3 4 5 6 7 8 9 0 1
      3 3 4 5 6 7 8 9 0 1 2
      4 4 5 6 7 8 9 0 1 2 3
      5 5 6 7 8 9 0 1 2 3 4
      6 6 7 8 9 0 1 2 3 4 5
      7 7 8 9 0 1 2 3 4 5 6
      8 8 9 0 1 2 3 4 5 6 7
      9 9 0 1 2 3 4 5 6 7 8
      comment */

      return finalString;
    };


    std::string getEncodedAccountName() const;
    std::string buildMessageForQR(const std::string & createdAt) const;

private:
    std::string initialNumber;
    std::string accountNumber;
    std::string accountName;
    std::string bankName;
};

class BankQR {
public:
    // static BankQR encode(string message);
    BankQR(const std::string & message);
    ~BankQR();

    bool saveToPNG(const std::string & filename) const;

    std::string toString() const;
    std::string toString(int margin) const;

    bool saveStringTo(const std::string & filename, int margin=0) const;

    static std::string decodePNG(const std::string & filename);

    static std::string decodeInfoOfMessage(const std::string & message);

    static BankQR convert(const BankQR & inQR, const std::string & targetBank);

private:
    /*
     * Propose your attributes
     *
     */
};
