SecureVault – Advanced File-Based Security Management System
============================================================

1. Project Title
----------------
SecureVault – Advanced File-Based Security Management System

2. Project Introduction
-----------------------
SecureVault is a terminal-based security management application written in standard
C (C99). It demonstrates fundamental and intermediate C programming concepts through
a complete, working security system. The application is designed as a university-level
Programming Fundamentals project and compiles using GCC/MinGW (e.g., Dev-C++).

All persistent data is stored in separate local text files. No databases, no GUI,
no external dependencies. The program uses only standard C headers: stdio.h, stdlib.h,
string.h, time.h, ctype.h, stdbool.h, and stdint.h.

3. Project Objectives
---------------------
The objectives of the SecureVault project are:

a) To demonstrate mastery of C programming fundamentals, including structures,
   functions, arrays, pointers, string handling, file I/O, loops, conditionals,
   and search algorithms.

b) To implement a complete security management system that goes beyond basic
   record-keeping, featuring user registration, authentication, password management,
   login history, and security event logging.

c) To educate students about computer security concepts in an academic context,
   including password hashing, salting, and the limitations of SHA-256 for password
   storage.

d) To provide a modular, well-structured C program with separate source and header
   files, each responsible for a specific domain of functionality.

e) To produce a compilable, testable application that runs in a terminal window
   and persists data across program executions using local text files.

4. Technologies Used
--------------------
- C programming language (C99 standard)
- Standard C library functions only (no external dependencies)
- File-based data storage using fopen, fprintf, fgets, fclose
- SHA-256 hash implementation in pure C for educational password demonstration
- time.h for timestamp generation
- No C++, Python, Java, JavaScript, HTML, CSS, web technologies, GUI frameworks,
  or database systems

5. Main Features
----------------

5.1 User Registration
   - Validates username length (4-31 characters) and allowed characters (letters,
     digits, underscores).
   - Accepts a password and confirms it via confirmation.
   - Checks password strength (minimum length, uppercase, lowercase, digits,
     special characters).
   - Generates a per-user salt and hashes the password using SHA-256 before
     storage.
   - Stores account creation time.
   - Prevents duplicate usernames.

5.2 User Login
   - Authenticates using username and password.
   - Compares password hashes (never plaintext passwords).
   - Records successful and failed login attempts with timestamps.
   - Implements a configurable temporary account lockout after 3 repeated failed
     attempts (15-minute lockout duration).
   - Logs timestamps and usernames for each attempt.

5.3 Password Strength Checker
   - Checks minimum length, uppercase letters, lowercase letters, digits, and
     special characters.
   - Displays a clear strength rating (WEAK/FAIR/STRONG) and suggestions for
     improvement.
   - Never prints or logs plaintext passwords.

5.4 User Management
   - Displays registered usernames and account information (without revealing
     password hashes).
   - Searches for an account by username.
   - Updates passwords after verifying the current password.
   - Deletes accounts with user confirmation.

5.5 Login History
   - Records successful logins and failed login attempts.
   - Records timestamps and event types (LOGIN_SUCCESS, LOGIN_FAILED).
   - Allows an authenticated user to view their own login history.

5.6 Security Event Logging
   - Logs registration, login attempts, password changes, account deletion, and
     lockout events.
   - Includes timestamps and event descriptions.
   - Never logs passwords or sensitive credentials.

5.7 Admin Functionality
   - Provides a clearly separated administrator role.
   - Allows an administrator to view account summaries, search users, and review
     security logs.
   - The administrator account uses username "admin"; the first-time administrator
     must set a new password after initial login.
   - Administrative functions are restricted to authenticated administrators.

5.8 Data Management
   - Uses separate files for users (users.txt), login history (login_history.txt),
     and security events (security_logs.txt).
   - Uses fopen, fclose, fprintf, fgets, and other appropriate standard C file
     functions.
   - Handles missing, empty, and corrupted files safely.
   - Checks every important file operation for errors.
   - Prevents accidental data loss when updating or deleting records using a
     temporary file and safe replacement strategy.

5.9 Main Menu
   - Displays a professional terminal menu with options appropriate to the
     user's authentication status and role.
   - Shows only registration and login options for unauthenticated users.
   - Shows user-specific options (change password, view history, manage account)
     for authenticated users.
   - Shows administrator options (view accounts, search users, review logs) for
     authenticated administrators.

6. Project Directory Structure
------------------------------
SecureVault/
  main.c              Program entry point and terminal menu loop
  auth.c              Authentication, login, lockout management
  auth.h              Authentication function declarations and types
  users.c             User registration, login, account management
  users.h             User management function declarations and types
  password.c          SHA-256 hashing and password strength checking
  password.h          Password hashing and strength checking declarations
  file_manager.c      File I/O operations for data persistence
  file_manager.h      File operation function declarations
  history.c           Login history recording and viewing
  history.h           Login history function declarations
  security_logs.c     Security event logging
  security_logs.h     Security event logging function declarations
  utils.c             Utility functions (input handling, time formatting)
  utils.h             Utility function declarations
  config.h            Project constants, enums, and definitions
  users.txt           Stores registered user records (persistent data)
  login_history.txt   Records login events with timestamps (persistent data)
  security_logs.txt   Records security events with timestamps (persistent data)
  README.txt          Project documentation
  DEV_CPP_COMPLETE_GUIDE.txt  Dev-C++ setup and running guide
  DEV_CPP_PROJECT_CHECKLIST.txt Project submission checklist

7. Explanation of Source Files
------------------------------

7.1 main.c
   The main entry point of the application. Contains the terminal menu loop and
   dispatches to the appropriate module based on user input and authentication
   status. Determines whether to show the unauthenticated menu, user menu, or
   administrator menu. Global variables track the logged-in user and their role.

7.2 auth.c
   Implements authentication functionality. Handles user login and logout, verifies
   password hashes against stored records, manages account lockout after repeated
   failed attempts (3 failed tries triggers 15-minute lockout), and records login
   events (successful and failed) in the login history.

7.3 auth.h
   Contains function declarations for authentication operations, including attempt_login,
   verify_credentials, check_lockout, and related types for tracking failed attempt
   counters and lockout state.

7.4 users.c
   Implements user management operations. Handles user registration (with username
   validation, password confirmation, and strength checking), user login verification,
   password changes (requiring current password verification), account search, and
   account deletion with user confirmation.

7.5 users.h
   Contains function declarations for user management, including register, login,
   change_password, delete, exists, list_all, and search functions.

7.6 password.c
   Implements SHA-256 hashing in pure C (FIPS 180-4 specification) and password
   strength checking. Generates per-user salts for password hashing, formats hashes
   as hex strings, and evaluates passwords for length, uppercase, lowercase, digits,
   and special characters. Includes educational disclosure about SHA-256 limitations.

7.7 password.h
   Contains function declarations for password hashing and strength checking,
   including PasswordHash struct, hash generation, verification, and strength
   evaluation functions.

7.8 file_manager.c
   Implements file input/output operations. Reads and writes user records to
   users.txt, appends login events to login_history.txt, and appends security events
   to security_logs.txt. Uses a temporary file and safe replacement strategy when
   updating or deleting records to prevent data loss. Handles missing and empty files
   gracefully.

7.9 file_manager.h
   Contains function declarations for file operations, including reading/writing
   users, history, and logs, and safe file deletion.

7.10 history.c
   Implements login history recording and viewing. Records successful and failed
   login attempts with timestamps and event types. Provides functionality to view
   an authenticated user's own login history.

7.11 history.h
   Contains function declarations for history operations, including recording logins
   and viewing user history.

7.12 security_logs.c
   Implements security event logging. Records registration, login attempts, password
   changes, account deletion, and lockout events with timestamps and descriptions.
   Provides functionality to view all logs and search by keyword.

7.13 security_logs.h
   Contains function declarations for security logging, including event logging and
   log viewing/search functions.

7.14 utils.c
   Provides utility functions used across multiple modules. Includes input buffer
   clearing, line reading, time formatting, file existence checking, and other
   helper functions.

7.15 utils.h
   Contains function declarations for utility functions.

7.16 config.h
   Defines project-wide constants and types. Includes maximum lengths for usernames
   and passwords, minimum password length, maximum failed login attempts before
   lockout, lockout duration, file names, return status codes, and user role
   enumeration (ROLE_USER, ROLE_ADMIN).

8. Explanation of Data Files
----------------------------

8.1 users.txt
   Persistent storage for registered user records. Each record follows the format:
   username|password_hash|salt|creation_time
   The file contains one record per line. Password hashes are SHA-256 hex strings
   combined with per-user salts. The file is created automatically on first run
   and updated whenever a user registers or is deleted.

8.2 login_history.txt
   Persistent storage for login events. Each record follows the format:
   timestamp|username|event_type
   Event types include LOGIN_SUCCESS and LOGIN_FAILED. The file is appended to
   on each login attempt and retains a historical record of all login activity.
   Can be viewed by administrators and authenticated users.

8.3 security_logs.txt
   Persistent storage for security events. Each record follows the format:
   timestamp|description
   Event types include USER_REGISTERED, LOGIN_SUCCESS, LOGIN_FAILED, PASSWORD_CHANGED,
   ACCOUNT_DELETED, and LOCKOUT_TRIGGERED. The file is appended to on each significant
   security event and provides an audit trail of all system activity.

9. C Programming Concepts Used
------------------------------

9.1 Structures (struct)
   - UserRecord: Stores username, password hash (with salt), and creation time
   - PasswordHash: Contains the SHA-256 hash and per-user salt
   - HistoryEntry: Records login timestamp, username, and event type
   - SecurityLog: Records security event timestamp and description
   - AccountStatus: Tracks login attempt counters and lockout state
   - SHA256_CTX: Context structure for SHA-256 computation

9.2 Functions and Modular Programming
   - Program divided into 16 modules (main, auth, users, password, file_manager,
     history, security_logs, utils, config)
   - Each module has a corresponding header file (.h) with function declarations
   - Clear separation of concerns: authentication, file I/O, user management, etc.

9.3 Arrays
   - Character arrays for usernames, passwords, hashes, timestamps, descriptions
   - Fixed-size buffers throughout the program
   - SHA-256 state arrays and working variables
   - Search and comparison operations on character arrays

9.4 Pointers
   - Function parameters passing strings and structures by pointer
   - String manipulation using pointer operations
   - Pointer arithmetic implicitly used in array indexing and string operations

9.5 String Handling
   - Using strlen, strcpy, strcat, strcmp, strncpy, strncat, strtok
   - Custom line reading functions that handle newline removal
   - Formatting with snprintf and printf for output
   - Careful buffer size management in all string operations

9.6 File Handling
   - Using fopen, fclose, fprintf, fscanf, fgets for file I/O
   - Reading structured text records delimited by pipes (|)
   - Writing records in consistent format for later parsing
   - Appending new records to existing files
   - Safe file replacement strategy: write to temp file, then rename
   - Checking return values of all file operations
   - Handling missing files gracefully (create on first write)
   - File existence checking before operations

9.7 Loops and Conditional Statements
   - Nested for loops in SHA-256 transformation function
   - While loops for input validation
   - Switch statements for menu navigation
   - If-else chains for password strength evaluation
   - Do-while loops for menu retry logic

9.8 Searching Algorithms
   - Linear search for username lookup in user database
   - Case-insensitive comparison using strcmp and tolower
   - File content scanning with fscanf to find specific records
   - Substring search using strstr for security log keyword search

9.9 Input Validation
   - Username length validation (MIN/MAX constraints)
   - Username character validation (alphanumeric plus underscore)
   - Password strength checking (length, case, digits, special chars)
   - Menu choice validation with retry loop
   - Password confirmation matching
   - Account deletion confirmation prompt
   - Buffer overflow prevention using size-limited I/O (fgets, read_line)
   - Lowercase conversion for user menu choices

9.10 Date and Time Functions
   - Using time() to get current calendar time
   - Using localtime() to convert to local time structure
   - Using strftime to format timestamps as "YYYY-MM-DD HH:MM:SS"
   - Using difftime to calculate elapsed time for lockout duration

9.11 Enumerations (enum)
   - UserRole: ROLE_USER, ROLE_ADMIN for role-based access control
   - StatusCode: SUCCESS, FAILURE, and various error codes for operation results
   - File mode constants: READ, WRITE, APPEND modes

9.12 Preprocessor Directives
   - #define for constants (buffer sizes, limits, format strings)
   - #ifndef/#define/#endif header guards
   - #include for header file inclusion

9.13 Type Definitions (typedef)
   - Typedef for UserRole enum to simplify usage
   - Custom type names for complex structures

9.14 Preprocessor #include and Header Management
   - Modular header file design (.h files for each .c module)
   - Dependency management between modules
   - Standard library headers: stdio.h, stdlib.h, string.h, time.h,
     ctype.h, stdbool.h, stdint.h
   - Project-specific header: config.h with shared constants

10. Security Features and Educational Limitations
-------------------------------------------------

10.1 Password Hashing
   - Passwords are hashed using SHA-256 with a per-user salt before storage.
   - Plaintext passwords are never stored or transmitted.
   - The salt is stored alongside the hash in users.txt.

10.2 Educational Disclosure (IMPORTANT)
   - SHA-256 hash alone is NOT suitable for production password storage.
   - This implementation demonstrates the concepts in a standard-library-only
     environment without external dependencies.
   - For production environments, always use a proper Key Derivation Function
     such as Argon2, bcrypt, or PBKDF2 with many iterations and per-user salts.
   - Password salt is generated using rand() - not cryptographically secure.
   - This project is for educational purposes only and should not be used for
     actual security-sensitive data management.

10.3 Account Lockout
   - After 3 consecutive failed login attempts, the account is locked for 15 minutes.
   - Lockout state is tracked in memory during program execution.
   - Note: Lockout state is lost on program restart (stored in memory only);
     for production use, store lockout state in the user data file.

10.4 Admin Access
   - Administrator functions are restricted to authenticated administrators.
   - The administrator username is "admin"; first-time admins must change the
     initial password immediately after first login.
   - No hidden or hardcoded universal administrator password exists.

11. System Requirements
----------------------
- Operating system: Windows (tested on Dev-C++ with GCC/MinGW)
- Compiler: GCC/Min supporting C99 standard
- Development environment: Dev-C++ (recommended) or any compatible C compiler
- No additional libraries required (standard C only)
- Disk space: Minimal (less than 1 MB for data files)

12. Compilation and Execution Instructions
-------------------------------------------

12.1 Using Dev-C++ (Recommended)
   1. Ensure Dev-C++ is installed on your Windows computer.
   2. Locate the SecureVault folder (the project directory).
   3. Open Dev-C++ and create a new C project (File -> New -> Project).
   4. Name the project "SecureVault" and confirm the save location.
   5. Add all .c source files to the project:
      - main.c, auth.c, users.c, password.c, file_manager.c, history.c,
        security_logs.c, utils.c
   6. Add all .h header files to the project:
      - config.h, auth.h, users.h, password.h, file_manager.h, history.h,
        security_logs.h, utils.h
   7. Configure the compiler:
      - Go to Options -> Compiler Options.
      - In the "Additional options" field, enter: -std=c99 -Wall -Wextra
      - Ensure "C++ Compiler" is set to "gcc.exe" (critical for C files).
   8. Build the project: Click Build -> Compile (or press F11).
   9. Run the program: Click Run -> Run (or press F10).
   10. The terminal window will display the SecureVault main menu.

12.2 Using Command Line (GCC/MingW)
   1. Open a command prompt (cmd.exe) and navigate to the SecureVault directory.
   2. Compile with: gcc -std=c99 -Wall -Wextra -o SecureVault.exe main.c auth.c users.c password.c file_manager.c history.c security_logs.c utils.c
   3. Run the program: SecureVault.exe

12.3 First-Time Setup
   - On first run, the program will create three data files in the running directory:
     users.txt, login_history.txt, and security_logs.txt (all initially empty).
   - The main menu will appear with options for unauthenticated users.
   - To register a new user, select "1. Register" and follow the prompts.
   - To log in as administrator, use username "admin" with initial password "Admin123!"
     and immediately change the password.

13. Testing Information
----------------------

13.1 Test Scenarios
   - Register a new user with valid credentials.
   - Register with duplicate username (should fail).
   - Register with invalid username (too short, too long, or invalid characters).
   - Register with weak password (should warn but allow registration).
   - Login with correct credentials (should succeed).
   - Login with incorrect password (should fail, increment failed attempt counter).
   - Login after 3 failed attempts (account should lock out).
   - Wait for lockout duration and login again (counter should reset).
   - Change password (verify current password first, then set new password).
   - Search for a registered user by username.
   - View login history after authentication.
   - View security logs as administrator.
   - Delete account with confirmation.
   - Exit and restart the program; verify data persists in files.

13.2 Expected Results
   - All registration, login, and management functions should work as described.
   - Error messages should be clear and informative.
   - Data files should retain records across program executions.
   - The program should not crash on invalid input (handles gracefully).
   - Password hashes should never be visible in plaintext output.

14. Conclusion
--------------
SecureVault is a complete, working terminal-based security management system
implemented in standard C99. The project demonstrates fundamental and intermediate
C programming concepts through a practical application that includes user
registration, authentication, password management, login history tracking, and
security event logging.

The application uses file-based storage with separate text files for users,
login history, and security events. All password hashing uses SHA-256 with
per-user salts, and the code includes educational disclosures about the
limitations of SHA-256 for production password storage.

This project is intended as a university-level Programming Fundamentals assignment.
It should be compiled and run using a standard C compiler (e.g., Dev-C++ with
GCC/MinGW). Students are encouraged to test all features, verify data persistence,
and understand the security educational limitations documented throughout the code.

For any issues or questions, refer to the DEV_CPP_COMPLETE_GUIDE.txt and
DEV_CPP_PROJECT_CHECKLIST.txt files included in this project directory.

---

SecureVault – Advanced File-Based Security Management System
Copyright (Educational Project) - Standard C99 Implementation