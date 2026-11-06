# Student Record Lookup System (Hash Table)

A C++ program that stores and retrieves student records quickly using a hash table implemented from scratch.

## Student data
Student ID, Name, Programme, Level, Telephone

## Operations
- Insert student
- Search student
- Delete student
- Update student
- Display hash table

## How it works
- Hash function: division method (`student ID mod table size`)
- Table size: 13 buckets
- Collision handling: separate chaining (a linked list in each bucket)
- Average time for insert, search, delete and update: O(1)

## How to run
(On Windows, run `student_hash_table.exe`, or open the file in Code::Blocks and press F9.)

