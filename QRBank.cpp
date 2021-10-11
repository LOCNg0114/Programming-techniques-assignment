#include <iostream>
#include <sstream>
#include <cassert>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>

#include "QRBank.hpp"

#include <iostream>
using namespace std;

int main ()
{
  BankAccount acc1("1","AccountNo", "Name", "Bank" );
  cout << acc1.getInitialNumber() << " ";
  cout << acc1.getAccountNumber() << " ";
  cout << acc1.getAccountName() << " ";
  cout << acc1.getBankName() << " ";
  cout << endl;
  acc1.setAccountNumber("2129082021");
  cout << acc1.getAccountNumber();
  cout << endl;
  cout << acc1.getEncodedAccountNumber();
  cout << endl;
  return 0;
}
