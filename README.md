# 📚 Library Management System

A Library Management System built using concepts of OOPS(Object-Oriented Programming) in C++ Programming Language. This is made using a whole lot of concepts in OOPS such as Inheritance, Polymorphism, Encapsulation. This system manages students, faculty and librarians with essential library functionalities and file-baseddata persistance.

## 📖 Table of Contents

- [Features](#features)
- [Setup](#setup)
- [Run Instructions](#run-instructions)
- [Data Format](#data-format)
- [Usage](#usage)
- [Important Instructions](#️-important-instructions)

**If you are testing this code, It already has 10 books, 5 students, 3 faculties and a librarian. You can remove them and create your whole new library, books, users, accounts for students, faculty and librarian.**

## Features

✅ Multi-role support (Students, Faculty, Librarians)  
✅ Book borrowing system with timestamps  
✅ Fine management with payment options  
✅ Easy tracking of overdue books and borrowing history  
✅ Librarian tools for book and user management  
✅ File-based data persistence for secure storage 

## Setup

1. **Clone the Repository**

```bash
git clone https://github.com/akilteja-1625/Library-Management-System.git
cd Library-Management-System
```

## Run Instructions

Option 1: Compile & Run on Linux (Recommended)

```bash
g++ main.cpp -o library_system  # Compiles the file
./library_system  # Runs the compiled executable
```

Option 2: Compile & Run on Windows (Using MinGW)

```bash
g++ main.cpp -o library_system.exe  # Compiles the file
library_system.exe  # Runs the compiled executable
```

## Data Format

**In book Class**

title - string
author - string
publisher - string
year - integer
ISBN - string(**unique for every book**)
status - boolean(**0 - not available & 1 - available**)

**In Account Class**

borrowedBooks - vector of class book(i.e it is an array of book objects)
dueDate - is an unordered map where key is ISBN and value id due Date
borrowedDate - is an unordered map where key is ISBN and value id borrowed Date
returnDate - is an unordered map where key is ISBN and value id return Date
fineAmount - it has fine calculated for the student(i.e every user has a account)

**In user Class**

name - string
userID - int
email - string
phoneNumber - string
userType - string(i.e either "Student" or "Faculty" or "Librarian")
every user has an account.

**In Library Class**

books - vector of books(i.e an array of book objects which are stored in library)
users - is an unordered map where key is userID and value is the address of user(to avoid creating copies of memory and making the code inefficient)

**In student class**

It is derived from user class.
maxBooks - it is constant integer and can't exceed 3 for an student
Every student has to pay fine if his books are in overdue period 

**In Faculty class**

It is derived from user class.
maxBooks - it is constant integer and can't exceed 5 for an faculty

**In Librarian class**

It is derived from user class.
He is the only one who has access to add Books, remove Books, add users and remove users in our library management system.
He cannot borrow or return books like student and faculty.

**Books.csv**
It retains the data of books with it's availability status. when we terminate the program and when ever we run the program again it uses the data retained by this file to maintain integrity of our library management system.

**Users.csv**
It retains the data of users who are newly added and removed from our system. when we terminate the program and when ever we run the program again it uses the data retained by this file to maintain integrity of our library management system.

**Transactions.csv**
It retains the data of transactions i.e the book taken by which user and the time of borrow and the time of return. when we terminate the program and when ever we run the program again it uses the data retained by this file to maintain integrity of our library management system.

## Usage

📋 Step-by-Step Instructions

- Run the Program using the commands mentioned in [Run Instructions](#run-instructions) session.
  🔐 Login Instructions
  When you run the application, you'll see the following menu:

  1.  **Enter `1`** for display Books
  It shows you all the books present in library. The books are given as title,author,publisher,year,isbn and it's status.
  2.  **Enter `2`** for Issue Book
  It allows you to take book only if you are student or faculty member. It asks your userID and the book isbn which you want to take. Remember that entering any userID which is not a integer leads to segmentation fault.
  3.  **Enter `3`** for Return Book
  It allows you to return Book only if you are an student or a faculty member. It asks your userID and the book isbn which you want to return. Remember that entering any userID which is not a integer leads to segmentation fault.
  4.  **Enter `4`** to Pay Fine
  It allows you to pay fine if you are student and if you take a book for more than 15 days. It asks your userID and the amount you are willing to pay. You can also pay fine partly.
  5.  **Enter `5`** for View Account
  It displays your info such as name,email,id,phoneNumber,userType and the books which you have borrowed currently and the fine amount of how much you should pay. It takes your userID as input.
  6.  **Enter `6`** for View Available Books
  It displays the books which are available so that you can select a book from them.
  7.  **Enter `7`** for Add book
  This feature is only available for librarian. He can add book to the library by giving all the data which a book needs such as book title, author, publisher, year, ISBN. It takes your userID as input.(i.e librarian user id)
  8.  **Enter `8`** to Remove book
  This feature is only available for librarian. He can remove book from the library by giving ISBN(unique for an book). It takes your userID as input.(i.e librarian user id) 
  9.  **Enter `9`** for Add user
  This feature is only available for librarian. He is the only person who can add user to the library system. He needs to give the data which is essential for an user such as name,userID,phonenumber,email,userType.It takes your userID as input.(i.e librarian user id)
  10.  **Enter `10`** for Remove user
  This feature is only available for librarian. He is the only person who can remove user from the library system. He needs to give the data which is essential to remove an user such which his the unique ID of the user.It takes your userID as input.(i.e librarian user id)
  11.  **Enter `11`** for Exit
  After exitting the data is stored in the files books.csv,users.csv and transactions.csv and when ever we run the program again it loads the data from these files.

  💡 **Tip:** Ensure you follow the correct input format to avoid errors.

## ⚠️ Important Instructions

Please follow these guidelines to ensure smooth usage and testing of the system:

- ➤ **Enter valid data** to avoid unexpected errors or system crashes.
- ➤ **If you are testing:**
  - Use the data available in **Books.txt** and **Users.txt**.
  - For **Book IDs**, refer to the **Books.txt** or use the **Display Books** command in the interface for correct numbers.
- ➤ **Do not modify the data format** in the `.txt` files directly.

## 📞 Contact

If you have any questions or suggestions, feel free to reach out. I'm happy to help!

📧 **Email:** [pbosuateja23@iitk.ac.in](mailto:pbosuateja23@iitk.ac.in)











