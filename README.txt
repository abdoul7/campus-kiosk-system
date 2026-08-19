README.txt
CT077-3-2 DSTR Lab Work #1 - Individual Submission

Student ID : TP085697
Name       : Abdoul Hakim Issaka Amadou
Part       : B - Singly Linked List

=====================================================================
FILES IN THIS SUBMISSION
=====================================================================
- TP085697_AbdoulHakim_CT077_LW1.cpp   (my own C++ source code)
- README.txt                           (this file)
- students_500.csv                     (dataset: 500 records)
- students_2000.csv                    (dataset: 2,000 records)
- students_8000.csv                    (dataset: 8,000 records)
- students_30000.csv                   (dataset: 30,000 records)
- programmes.csv                       (programme reference table)
- DATASET_README.txt                   (dataset design notes)
- TP085697_PerfRecord.txt              (my individual performance record)

IMPORTANT: The CSV files MUST be in the SAME folder as the .cpp file
(or the compiled executable) so the program can open them at runtime.

=====================================================================
HOW TO COMPILE AND RUN IN CODE::BLOCKS
=====================================================================
1. Open Code::Blocks.
2. Go to File > New > Empty File (or File > New > Project > Console
   Application if you prefer a full project).
3. Save the file, or add TP085697_AbdoulHakim_CT077_LW1.cpp to the
   project as the source file.
4. Make sure the compiler is set to use the C++11 standard:
   Settings > Compiler > "Have g++ follow the C++11 ISO standard"
   (tick this option).
5. Copy all CSV files (students_500.csv, students_2000.csv,
   students_8000.csv, students_30000.csv, programmes.csv) into the
   SAME folder as the source file / executable.
6. Click Build and Run (F9).
7. Use the on-screen menu (see MENU OPTIONS below).

=====================================================================
HOW TO COMPILE AND RUN IN VISUAL STUDIO
=====================================================================
1. Open Visual Studio.
2. Create a new project: File > New > Project >
   "Empty Project" (C++) or "Console App" (C++).
3. In Solution Explorer, right-click "Source Files" > Add >
   Existing Item, and add TP085697_AbdoulHakim_CT077_LW1.cpp.
4. Copy all CSV files into the folder that contains the compiled
   executable (usually the project's Debug/Release output folder),
   OR run the program from the folder that contains the CSV files.
   The CSV files must sit next to the running program.
5. Build the solution: Build > Build Solution (Ctrl+Shift+B).
6. Run: Debug > Start Without Debugging (Ctrl+F5).
7. Use the on-screen menu (see MENU OPTIONS below).

=====================================================================
HOW TO COMPILE AND RUN WITH g++ (COMMAND LINE, OPTIONAL)
=====================================================================
1. Put the .cpp file and all CSV files in the same folder.
2. Open a terminal in that folder.
3. Compile:
      g++ -std=c++11 TP085697_AbdoulHakim_CT077_LW1.cpp -o member5
4. Run:
      ./member5        (Windows:  member5.exe)

=====================================================================
MENU OPTIONS
=====================================================================
1.  Load CSV file            (e.g. type: students_500.csv)
2.  Insert at beginning
3.  Insert at end
4.  Insert at specified position
5.  Delete by Student ID
6.  Search by Student ID
7.  Search by Name
8.  Display all records
9.  Count total records
10. Run edge-case tests
11. Run benchmark mode (times all four datasets)
0.  Exit

=====================================================================
IMPORTANT NOTES
=====================================================================
- This implementation uses a singly linked list coded from scratch.
- No built-in C++ containers (vector, list, array, queue, stack, map)
  and no std::sort/algorithm are used.
- Binary Search and Sort by CGPA are marked N/A because this is the
  linked-list part of the group assignment.
- Insert at beginning is O(1) for the pointer update itself; the public
  insert also checks for duplicate Student IDs, so the measured full
  operation can grow with n.
- Benchmark mode (option 11) reads the four official CSV datasets:
  students_500.csv, students_2000.csv, students_8000.csv,
  students_30000.csv. Keep them in the same folder before running it.
