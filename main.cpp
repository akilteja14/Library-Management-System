//LIBRARY MANAGEMENT SYSTEM - ASSIGNMENT 1 CS253

#include<iostream>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

string timeToString(time_t t) {
    struct tm *timeinfo = localtime(&t);
    stringstream ss;
    ss << put_time(timeinfo, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

time_t stringToTime(string timeStr) {
    struct tm timeinfo = {};
    istringstream ss(timeStr);
    ss >> get_time(&timeinfo, "%Y-%m-%d %H:%M:%S");
    return mktime(&timeinfo);
}

class book;
class Account;
class user;
class Library;
class student;
class faculty;
class librarian;

class book {
    private:
    string title;
    string author;
    string publisher;
    int year;
    string ISBN;
    bool status;

    public:
    book(string t, string a, string p, int y, string isbn) 
        : title(t), author(a), publisher(p), year(y), ISBN(isbn), status(true) {}

    string getTitle() {return title;}
    string getisbn() {return ISBN;}
    string getPublisher() {return publisher;}
    int getYear() {return year;}
    string getAuthor() {return author;}

    void displayBookdetails() {
        cout << "Title : " << title << "\n";
        cout << "Author : " << author << "\n";
        cout << "Publisher : " << publisher << "\n";
        cout << "Year : " << year << "\n";
        cout << "ISBN : " << ISBN << "\n";
        cout << "Status : " << status << "\n\n";
        return;
    }

    void issuebook() {
        if(status) {
            status = false;
            //cout << "Book '" << title << "' has been issued." << "\n";
        }
        else {
            cout << "Book '" << title << "' is already issued." << "\n";
        }
        return;
    }

    void returnbook() {
        if(!status) {
            status = true;
            //cout << "Book '" << title << "' has been returned." << "\n";
        }
        else {
            cout << "Book '" << title << "' has not been issued." << "\n";
        }
        return;
    }

    bool checkStatus() {
        return status;
    }
};

class Account {
    private:
    vector<string> borrowedBooks;
    unordered_map<string, time_t> dueDate;
    unordered_map<string, time_t> returnDate;
    unordered_map<string,time_t> borrowedDate;
    int fineAmount;
    
    public:
    Account() : fineAmount(0) {}
    int getBorrowedbooks() {
        int temp = borrowedBooks.size();
        return temp;
    }

    unordered_map<string, time_t> giveBookData() {
        return dueDate;
    }

    void displayAccount() {  
        time_t now = time(0);
        for(auto it: dueDate) {
            time_t due = it.second;
            int leftDays = (due-now)/86400;
            cout << "Book : " << it.first ;
            if (leftDays >= 0) {
                cout << " Due in " << leftDays << " days." << "\n";
            } else {
                cout << " OVERDUE by " << abs(leftDays) << " days!" << "\n";
            }
        }
        return;
    }

    bool checkbook(string isbn){
        for(auto it: dueDate){
            if(it.first == isbn)return true;
        }
        return false;
    }

    void addBook(string bookISBN, int borrowDays) {
        borrowedBooks.push_back(bookISBN);
        time_t now = time(0);
        dueDate[bookISBN] = now + (borrowDays*86400);
        borrowedDate[bookISBN] = now;

        cout << "Book " << bookISBN << " is borrowed. Due in " << borrowDays << " days." << "\n";
        return;
    }

    void removeBook(string bookISBN) {
        auto it = find(borrowedBooks.begin(),borrowedBooks.end(),bookISBN);
        returnDate[bookISBN] = time(0);
        if(it != borrowedBooks.end()) {
            borrowedBooks.erase(it);
            dueDate.erase(bookISBN);
            cout << "Book " << bookISBN << " has been returned successfully." << "\n";
        }
        else {
            cout << "Book not found in borrowed list." << "\n";
        }
        return;
    }

    void calculateFine() {
        time_t now = time(0);
         
        for(auto it: dueDate) {
            int due = it.second;

            if(now > due) {
                int penaltyDays = (now-due)/86400;
                fineAmount += penaltyDays*10;
            }
        }
        return;
    }

    void payFine(int amount) {
        if(fineAmount > 0) {
            fineAmount -= amount;
            cout << "Paid Rs " << amount << ". Remaining fine: Rs " << fineAmount << endl;
        }
        else {
            cout << "No fine to pay." << "\n";
        }
        return;
    }

    int getFineAmount() {
        calculateFine();
        return fineAmount;
    }

    void saveTransactions(ofstream& outFile, int userID) {
        for (auto it : borrowedDate) {
            outFile<<"UserID :" <<userID<<" " <<"Book ISBN : "<< it.first << " Borrowed Time: " << timeToString(it.second) << "\n";
        }
        for (auto it : returnDate) {
            outFile<<"UserID :" <<userID<<" " <<"Book ISBN : "<< it.first << " Returned Time: " << timeToString(it.second) << "\n";
        }
    }
    
    void loadTransaction(string isbn, time_t dueDate) {
        borrowedBooks.push_back(isbn);
        this->dueDate[isbn] = dueDate;
    }    
};

class user {
    private: 
    string name;
    int id;
    string email;
    string phoneNumber;
    string userType;
    Account *account;
    
    public:
    user(string n, int i, string e, string p, string type)
        : name(n), id(i), email(e), phoneNumber(p), userType(type), account(nullptr) {}

    virtual ~user() {
        delete account;
    }

    Account* giveAccountData() {
        return account;
    }

    virtual void borrowBook(string bookISBN) = 0;
    virtual void returnBook(string bookISBN) = 0;
    virtual void viewBorrowedBooks() {
        cout << "This user does not have a borrowing account.\n";
    }
    virtual int getFineAmount() {
        return account->getFineAmount();
    }
    virtual int currentBookSize() {
        return account->getBorrowedbooks();
    }
    virtual void saveToFile(ofstream& outFile) {
        outFile << userType << "," << name << "," << id << "," << email << "," << phoneNumber << "\n";
    }

    void displayInfo() {
        cout << "Name : " << name << "\n";
        cout << "Id : " << id << "\n";
        cout << "Email : " << email << "\n";
        cout << "phoneNumber : " << phoneNumber << "\n";
        cout << "userType : " << userType << "\n";
        return;
    }

    int getID() {
        return id;
    }

    string getName() {
        return name;
    }

    string getUserType() {
        return userType;
    }

    string getEmail() {
        return email;
    }

    string getPhoneNumber() {
        return phoneNumber;
    }
};

class Library {
    private:
    vector<book> books;
    unordered_map<int, user*> users;
    Account account;

    public:
    void addUser(user *User) {
        users[User->getID()] = User;
        cout << User->getUserType()<<" " << User->getName() << " added succesfully." << "\n";
        return;
    }

    void removeUser(user *User) {
        for(auto it: users) {
            if(it.first == User->getID()) {
                cout << User->getUserType()<<" " << User->getName() << " removed succesfully." << "\n";
                users.erase(User->getID());
                return;
            }
        }
        return ;
    }

    vector<book>& getBooks() {
        return books;
    }

    user* getUser(int userID) {
        if (users.find(userID) != users.end()) {
            return users[userID];
        }
        return nullptr;
    }    

    void addbook(book &Book) {
        books.push_back(Book);
        cout << "Book added successfully: " << Book.getTitle() << "\n";
        return;
    }

    void removeBook(string &isbn) {
        for(auto it = books.begin();it != books.end();++it) {
            if(it->getisbn() == isbn) {
                books.erase(it);
                cout << "Book with ISBN: " << isbn << " removed successfully" << "\n";
                return;
            }
        }
        cout << "Book not found" << "\n";
        return;
    }

    void issueBook(string &isbn, int userID) {
        if(users.find(userID) == users.end()) {
            cout << "User not found!!" << "\n";
            return;
        }
        if(users[userID]->getUserType() == "Student" && users[userID]->currentBookSize()<3) {
            for(auto &it : books) {
                if(it.getisbn() == isbn) {
                    if(it.checkStatus() == true) {
                        users[userID]->borrowBook(isbn);
                        it.issuebook();return;
                    }
                    else {
                        cout << "Book is already issued" << "\n";return;
                    }
                }
            }
            cout << "Book not found!!" << "\n";
            return;
        }
        else if(users[userID]->getUserType() == "Faculty" && users[userID]->currentBookSize()<5) {
            for(auto &it : books) {
                if(it.getisbn() == isbn) {
                    if(it.checkStatus() == true) {
                        users[userID]->borrowBook(isbn);
                        it.issuebook();return;
                    }
                    else {
                        cout << "Book is already issued" << "\n";return;
                    }
                }
            }
            cout << "Book not found!!" << "\n";
            return;
        }
        else if(users[userID]->getUserType() == "Librarian") {
            cout << "Librarian cannot borrow books!!" << "\n";return;
        }
        else {
            cout << "Borrowing limit reached!!" << "\n";return;
        }
    }

    void returnBook(string &isbn, int userID) {
        if(users.find(userID) == users.end()) {
            cout << "User not found!!" << "\n";
            return;
        }
        
        for(auto &it : books) {
            if(it.getisbn() == isbn) {
                if(it.checkStatus() == false) {
                    users[userID]->returnBook(isbn);
                    it.returnbook();return;
                }
                else {
                    cout << "Book has not been issued" << "\n";return;
                }
            }
        }
        cout << "Book not found!!" << "\n";
    }

    void displayBooks() {
        if(books.size() == 0) {
            cout << "There are no books!!" << "\n";
            return;
        }
        for(auto &it : books) {
            it.displayBookdetails();
        }
        return;
    }

    bool isStudent(int userID) {
        return users[userID]->getUserType() == "Student";
    }

    bool isFaculty(int userID) {
        return users[userID]->getUserType() == "Faculty";
    }

    void viewBorrowedBooks(int userID) {
        users[userID]->viewBorrowedBooks();
    }

    int getFineAmount(int userID) {
        return users[userID]->getFineAmount();
    }

    void saveBooksToFile() {
        ofstream outFile("books.csv");
        if (!outFile) {
            cout << "Error opening books.csv for writing!\n";
            return;
        }
    
        for (auto &it : books) {
            outFile << it.getTitle() << "," 
                    << it.getisbn() << "," 
                    << it.getAuthor() << "," 
                    << it.getPublisher() << "," 
                    << it.getYear() << "," 
                    << (it.checkStatus() ? "Available" : "Issued") << "\n";
        }
    
        outFile.close();
    }
    
    void loadBooksFromFile() {
        ifstream inFile("books.csv");
        if (!inFile) {
            cout << "No previous book data found.\n";
            return;
        }
    
        string title, isbn, author, publisher;
        int year;
        string statusStr;
        
        while (getline(inFile, title, ',') &&
           getline(inFile, isbn, ',') &&
           getline(inFile, author, ',') &&
           getline(inFile, publisher, ',') &&
           (inFile >> year)) {

        inFile.ignore();
        inFile >> statusStr;
        inFile.ignore();

        books.push_back(book(title, author, publisher, year, isbn));

        if (statusStr=="Issued") books.back().issuebook();
    }
    
        inFile.close();
    }

    void saveUsersToFile(); 
    
    void loadUsersFromFile();   
    
    void saveTransactionsToFile();
    
    void loadTransactionsFromFile();   
};

class student : public user {
    private:
    Account account;
    static const int maxBooks = 3;

    public:
    student(string n, int i, string e, string p)
        : user(n, i, e, p, "Student") {}

    void borrowBook(string bookISBN) override {
        if(account.getFineAmount() > 0) {
            cout << "You cannot borrow the books until the fine is cleared" << "\n";
            return;
        }
        if(account.getBorrowedbooks() < maxBooks) {
            account.addBook(bookISBN,15);return;
            //cout << "Book " << bookISBN << " has been successfully borrowed." << "\n";return;
        }
        cout << "Borrowing limit reached!!" << "\n";return;
        return;
    }

    void returnBook(string bookISBN) override {
        account.calculateFine();
        account.removeBook(bookISBN);
        //cout << "Book " << bookISBN << " has been successfully returned." << "\n";
        return;
    }

    void payFine(int amount) {
        account.payFine(amount);
        return;
    }

    void viewBorrowedBooks() override {
        account.displayAccount();
    }

    bool ValidCheck(string& isbn) {
        return account.checkbook(isbn);
    }
    
    int getFineAmount() override {
        return account.getFineAmount();
    }

    int currentBookSize() override {
        return account.getBorrowedbooks();
    }
    
    void saveToFile(ofstream& outFile) override {
        outFile << "Student," << getName() << "," << getID() << "," << getEmail() << "," << getPhoneNumber() << "\n";
    }

    void saveTransactions(ofstream& outFile) {
        account.saveTransactions(outFile, getID());
    }

    void loadTransaction(string isbn, time_t dueDate) {
        account.loadTransaction(isbn, dueDate);
    }
};

class faculty : public user {
    private:
    Account account;
    static const int maxBooks = 5;

    public:
    faculty(string n, int i, string e, string p)
        : user(n, i, e, p, "Faculty") {}

    void borrowBook(string bookISBN) override {
        if(account.getBorrowedbooks() < maxBooks) {
            account.addBook(bookISBN,30);return;
            //cout << "Book " << bookISBN << " has been successfully borrowed." << "\n";
        }
        cout << "Borrowing limit reached!!" << "\n";
        return;
    }

    void returnBook(string bookISBN) override {
        account.removeBook(bookISBN);
        //cout << "Book " << bookISBN << " has been successfully returned." << "\n";
        return;
    }

    void viewBorrowedBooks() override {
        account.displayAccount();
    }

    bool ValidCheck(string& isbn) {
        return account.checkbook(isbn);
    }

    int currentBookSize() override {
        return account.getBorrowedbooks();
    }

    void saveToFile(ofstream& outFile) override {
        outFile << "Faculty," << getName() << "," << getID() << "," << getEmail() << "," << getPhoneNumber() << "\n";
    }

    void saveTransactions(ofstream& outFile) {
        account.saveTransactions(outFile, getID());
    }

    void loadTransaction(string isbn, time_t dueDate) {
        account.loadTransaction(isbn, dueDate);
    }
};

class librarian : public user {
    public:
    librarian(string n, int i, string e, string p)
        : user(n, i, e, p, "Librarian") {}

    void borrowBook(string bookISBN) override {
        cout << "Librarians cannot borrow books!!!" << "\n";
        return;
    }
    
    void returnBook(string bookISBN) override {
        cout << "Librarians cannot return books!!!" << "\n";
        return;
    }

    void addBook(Library& library,book& Book) {
        library.addbook(Book);
        //cout << "Book '" << Book.getTitle() << "' has been added to library." << "\n";
        return;
    }

    void removeBook(Library& library,string& isbn) {
        library.removeBook(isbn);
        //cout << "Book with ISBN " << isbn << " has been removed from the library." << "\n";
        return;
    }

    void addUser(Library& library,user* User) {
        library.addUser(User);
        //cout << "Book '" << Book.getTitle() << "' has been added to library." << "\n";
        return;
    }

    void removeUser(Library& library,user *User) {
        library.removeUser(User);
        //cout << "Book with ISBN " << isbn << " has been removed from the library." << "\n";
        return;
    }

    void saveToFile(ofstream& outFile) override {
        outFile << "Librarian," << getName() << "," << getID() << "," << getEmail() << "," << getPhoneNumber() << "\n";
    }
};

void Library::saveUsersToFile() {
    ofstream outFile("users.csv", ios::out);
    if (!outFile) {
        cout << "Error opening users.csv for writing!\n";
        return;
    }

    for (auto &it : users) {
        user* u = it.second;
        if (!u) continue;

        string type = u->getUserType();
        if (type == "Student") {
            if (student* s = dynamic_cast<student*>(u)) {
                outFile << "Student," << s->getName() << "," << s->getID() << ","
                        << s->getEmail() << "," << s->getPhoneNumber() << "\n";
            }
        } 
        else if (type == "Faculty") {
            if (faculty* f = dynamic_cast<faculty*>(u)) {
                outFile << "Faculty," << f->getName() << "," << f->getID() << ","
                        << f->getEmail() << "," << f->getPhoneNumber() << "\n";
            }
        } 
        else if (type == "Librarian") {
            if (librarian* l = dynamic_cast<librarian*>(u)) {
                outFile << "Librarian," << l->getName() << "," << l->getID() << ","
                        << l->getEmail() << "," << l->getPhoneNumber() << "\n";
            }
        }
    }

    outFile.close();
}

void Library::loadUsersFromFile() {
    ifstream inFile("users.csv");
    if (!inFile) {
        cout << "No previous user data found.\n";
        return;
    }

    string userType, name, email, phone;
    int id;

    while (getline(inFile, userType, ',') &&
           getline(inFile, name, ',') &&
           (inFile >> id) &&
           inFile.ignore() &&
           getline(inFile, email, ',') &&
           getline(inFile, phone)) {

        if (userType == "Student") {
            users[id] = new student(name, id, email, phone);
        } 
        else if (userType == "Faculty") {
            users[id] = new faculty(name, id, email, phone);
        } 
        else if (userType == "Librarian") {
            users[id] = new librarian(name, id, email, phone);
        }
    }

    inFile.close();
}

void Library::saveTransactionsToFile() {
    ofstream outFile("transactions.csv");
    if (!outFile) {
        cout << "Error opening transactions.csv for writing!\n";
        return;
    }

    for (auto &it : users) {
        user* u = it.second;
        string type = u->getUserType();
        if(type == "Student") {
            if (student* s = dynamic_cast<student*>(u)) {
                s->saveTransactions(outFile);
            }
        }
        else if(type == "Faculty") {
            if (faculty* f = dynamic_cast<faculty*>(u)) {
                f->saveTransactions(outFile);
            }
        }
    }

    outFile.close();
}

void Library::loadTransactionsFromFile() {
    ifstream inFile("transactions.csv");
    if (!inFile) {
        cout << "No previous transaction data found.\n";
        return;
    }

    int userID;
    string isbn;
    time_t dueDate;
    
    while (inFile >> userID >> isbn >> dueDate) {
        user* u = getUser(userID);
        if (u) {
            student* s = dynamic_cast<student*>(u);
            faculty* f = dynamic_cast<faculty*>(u);
            if (s) {
                s->loadTransaction(isbn, dueDate);
            }
            else if (f) {
                f->loadTransaction(isbn, dueDate);
            }
        }
    }

    inFile.close();
} 

void elibrary(Library &lib) {
    ifstream checkFile("books.csv");
    if(!checkFile.good()) {
        book b1("b1","a1","p1",2000,"i1");book b2("b2","a2","p2",2001,"i2");
        book b3("b3","a3","p3",2003,"i3");book b4("b4","a4","p4",2004,"i4");
        book b5("b5","a5","p5",2005,"i5");book b6("b6","a6","p6",2006,"i6");
        book b7("b7","a7","p7",2007,"i7");book b8("b8","a8","p8",2008,"i8");
        book b9("b9","a9","p9",2009,"i9");book b10("b10","a10","p10",2002,"i10");
        lib.addbook(b1);lib.addbook(b2);lib.addbook(b3);lib.addbook(b4);lib.addbook(b5);
        lib.addbook(b6);lib.addbook(b7);lib.addbook(b8);lib.addbook(b9);lib.addbook(b10);

        student *s1 = new student("s1",1,"e1","p1");student *s2 = new student("s2",2,"e2","p2");
        student *s3 = new student("s3",3,"e3","p3");student *s4 = new student("s4",4,"e4","p4");
        student *s5 = new student("s5",5,"e5","p5");
        lib.addUser(s1);lib.addUser(s2);lib.addUser(s3);lib.addUser(s4);lib.addUser(s5);

        faculty *f1 = new faculty("f1",11,"e11","p11");faculty *f2 = new faculty("f2",12,"e12","p12");
        faculty *f3 = new faculty("f3",13,"e13","p13");
        lib.addUser(f1);lib.addUser(f2);lib.addUser(f3);

        librarian *l1 = new librarian("l1",21,"e21","p21");
        lib.addUser(l1);
    }
}

int main() {
    Library lib;

    elibrary(lib);
    lib.loadBooksFromFile();
    lib.loadUsersFromFile();
    lib.loadTransactionsFromFile();

    int choice;
    while (true) {
        cout << "\n" <<"   Library Management System   "<< "\n";
        cout << "1. Display Books" << "\n";
        cout << "2. Issue Book" << "\n";
        cout << "3. Return Book" << "\n";
        cout << "4. Pay Fine" << "\n";
        cout << "5. View Account" << "\n";
        cout << "6. View Available Books" << "\n";
        cout << "7. Add Book" << "\n";
        cout << "8. Remove Book" << "\n";
        cout << "9. Add user" << "\n";
        cout << "10. Remove user" << "\n";
        cout << "11. Exit" << "\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            lib.displayBooks();
        } 
        else if (choice == 2) {
            int userID;
            string isbn;
            cout << "Enter User ID: ";
            cin >> userID;
            cout << "Enter Book ISBN: ";
            cin >> isbn;
            lib.issueBook(isbn, userID);
        } 
        else if (choice == 3) {
            int userID;
            string isbn;
            cout << "Enter User ID: ";
            cin >> userID;
            cout << "Enter Book ISBN: ";
            cin >> isbn;
            user* u = lib.getUser(userID);
            if(!u){
                cout<<"No such user exist"<<"\n";
                continue;
            }
            if(u->getUserType() == "Librarian"){
                cout<< " Librarians cannot return books " << "\n";
                continue;
            }

            student* s = dynamic_cast<student*>(u);
            faculty* f = dynamic_cast<faculty*>(u);
            if(s){
                bool ff = s->ValidCheck(isbn);
                if(ff==false){
                    cout<<"You dont have this book"<< "\n";
                    continue;
                }
            }
            else if(f){
                bool ff = f->ValidCheck(isbn);
                if(ff==false){
                    cout<<"You dont have this book"<< "\n";
                    continue;
                }
            }
            lib.returnBook(isbn, userID);
        } 
        else if (choice == 4) {
            int userID, amount;
            cout << "Enter User ID: ";
            cin >> userID;
            cout << "Enter amount to pay: Rs ";
            cin >> amount; 
            if (lib.getUser(userID) != nullptr) {
                student* s = dynamic_cast<student*>(lib.getUser(userID));
                if (s) {
                    s->payFine(amount);
                } else {
                    cout << "Only students have fines to pay!" << "\n";
                }
            } else {
                cout << "User not found!" << "\n";
            }
        }
        else if (choice == 5) {
            int userID;
            cout << "Enter User ID: ";
            cin >> userID;
            user* u = lib.getUser(userID);
            if(!u){
                cout<<"No such user exist"<<"\n";
                continue;
            }
            u->displayInfo();

            student* s = dynamic_cast<student*>(u);
            faculty* f = dynamic_cast<faculty*>(u);

            if (s) {
                cout << "Borrowed Books:\n";
                s->viewBorrowedBooks();
                cout << "Outstanding Fine: Rs " << (int)s->getFineAmount() << "\n";
            } else if (f) {
                cout << "Borrowed Books:\n";
                f->viewBorrowedBooks();
            } else {
                cout << "This user does not have a borrowing account.\n";
            }
        }   
        else if (choice == 6) {
            cout << "Available Books:" << "\n";
            bool found = false;
            for (auto &book : lib.getBooks()) {
                if (book.checkStatus()) {
                    book.displayBookdetails();
                    found = true;
                }
            }
            if (!found) {
                cout << "No books are currently available." << "\n";
            }
        }
        else if (choice == 7) {
            int userID;
            cout << "Enter Your ID: ";
            cin >> userID;
            user *u = lib.getUser(userID);
            if(!u){
                cout<<"No such user exist"<<"\n";
                continue;
            }
            if(u->getUserType() == "Librarian") {
                string title, author, publisher, isbn;
                int year;
                cout << "Enter Book Title: ";
                cin.ignore();
                getline(cin, title);
                cout << "Enter Author: ";
                getline(cin, author);
                cout << "Enter Publisher: ";
                getline(cin, publisher);
                cout << "Enter Year: ";
                cin >> year;
                cout << "Enter Book ISBN: ";
                cin >> isbn;
                book newBook(title, author, publisher, year, isbn);
                librarian* l = dynamic_cast<librarian*>(u);
                if(l) {
                    l->addBook(lib,newBook);
                    cout << "Book successfully added!" << "\n";
                }
            }
            else {
                cout << "Access denied! Only librarians can add books." << "\n";
            } 
        }
        else if(choice == 8) {
            int userID;string isbn;
            cout << "Enter Your ID: ";
            cin >> userID;
            user *u = lib.getUser(userID);
            if(!u){
                cout<<"No such user exist"<<"\n";
                continue;
            }
            if(u->getUserType() == "Librarian") {
                cout << "Enter Book ISBN: ";
                cin >> isbn;
                librarian *l = dynamic_cast<librarian*>(u);
                if(l) {
                    l->removeBook(lib, isbn);
                    cout << "Book successfully removed!" << "\n";
                }
            }
            else {
                cout << "Access denied! Only librarians can remove books." << "\n";
            }
        }
        else if(choice == 9) {
            int userID;
            cout << "Enter your ID: ";
            cin >> userID;
            user *u = lib.getUser(userID);
            if(!u){
                cout<<"No such user exist"<<"\n";
                continue;
            }
            if(u->getUserType() == "Librarian") {
                string name;int id;string email;string phoneNumber;string userType;
                cout << "Enter user name : ";
                cin >> name;
                cout << "Enter User ID: ";
                cin >> id;
                cout << "Enter Email: ";
                cin >> email;
                cout << "Enter PhoneNumber: ";
                cin >> phoneNumber;
                cout << "Enter UserType: ";
                cin >> userType;
                if(userType == "Student") {
                    student *s = new student(name,id,email,phoneNumber);
                    lib.addUser(s);
                }
                else if (userType == "Faculty") {
                    faculty *f = new faculty(name,id,email,phoneNumber);
                    lib.addUser(f);
                }
                else if (userType == "Librarian") {
                    librarian *l = new librarian(name,id,email,phoneNumber);
                    lib.addUser(l);
                }
            }
            else {
                cout << "Access denied! Only librarians can add users." << "\n";
            } 
        }
        else if(choice == 10) {
            int id;
            cout << "Enter your ID: ";
            cin >> id;
            user *u = lib.getUser(id);
            if(!u){
                cout<<"No such user exist"<<"\n";
                continue;
            }
            if(u->getUserType() == "Librarian") {
                int userID;
                cout << "Enter UserID: ";
                cin >> userID;
                user *u1 = lib.getUser(userID);
                if(!u1) {cout << "There is no such user. Please check again" << "\n";}
                else {
                    lib.removeUser(u1);
                }
            }
            else {
                cout << "Access denied! Only librarians can remove user." << "\n";
            }
        }
        else if (choice == 11) {
            lib.saveBooksToFile();
            lib.saveUsersToFile();
            lib.saveTransactionsToFile();
            cout << "Exiting... Data Saved." << "\n";
            break;
        } 
        else {
            cout << "Invalid choice. Try again." << "\n";
        }
    }
    return 0;
}