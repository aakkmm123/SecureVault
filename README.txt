SecureVault - Advanced File-Based Security Management System
============================================================

Project Description
-------------------
SecureVault is a terminal-based security management application written in C99.
It demonstrates fundamental and intermediate C programming concepts including
structures, file handling, algorithms (SHA-256), input validation, and modular
program design. All persistent data is stored in local text files, and the
application compiles using standard GCC/MinGW compilers (e.g., Dev-C++).

The application provides a complete user authentication system with registration,
login, password management, login history tracking, and security event logging.
Administrator functions are available for managing users and reviewing security
events.

Features
--------
1. User Registration
   - Unique username validation with character checking
   - Password confirmation and strength checking
   - SHA-256 password hashing with per-user salt
   - Account creation timestamp storage
   - Duplicate username prevention

2. User Login
   - Password hash comparison (never plaintext)
   - Configurable account lockout after repeated failures
   - Login attempt recording with timestamps
   - Role-based access control (user/admin)

3. Password Strength Checker
   - Evaluates length, uppercase, lowercase, digits, special chars
   - Provides strength rating (WEAK/FAIR/STRONG) with suggestions
   - Never prints or logs plaintext passwords

4. User Management
   - Search accounts by username
   - Change passwords (verify current password first)
   - Delete accounts with confirmation
   - List all registered usernames

5. Login History
   - Record successful and failed logins with timestamps
   - View own login history after authentication

6. Security Event Logging
   - Log registration, login attempts, password changes, account deletion
   - Including timestamps and event descriptions
   - Never logs passwords or sensitive credentials

7. Admin Functionality
   - View account summaries and search users
   - Review security logs
   - Separate administrator role (not hardcoded password)
   - First-time admin setup: use username "admin" with initial password
     "Admin123!" and change it immediately after first login

8. Data Management
   - Separate files: users.txt, login_history.txt, security_logs.txt
   - Safe file operations with error checking
   - Temporary file and safe replacement strategy
   - Handle missing/empty/corrupted files gracefully

9. Professional Terminal Menu
   - Option selection appropriate to authentication status and role
   - Clear error messages and user prompts
   - Keyboard input validation

Configuration
-------------
All configuration is defined in config.h:
- MAX_USERNAME_LENGTH: 31 characters maximum
- MIN_USERNAME_LENGTH: 4 characters minimum
- MIN_PASSWORD_LENGTH: 6 characters minimum
- MAX_PASSWORD_LENGTH: 127 characters maximum
- MAX_FAILED_ATTEMPTS: 3 before lockout
- LOCKOUT_DURATION_MINUTES: 15 minutes

File Structure
--------------
- users.txt       - Stores registered user records
- login_history.txt - Records login events with timestamps
- security_logs.txt - Records security events with timestamps
- config.h        - Project constants and definitions
- *.c / *.h       - Source code modules

Security Considerations (Educational Disclosure)
------------------------------------------------
IMPORTANT: SHA-256 hash alone is NOT suitable for production password storage.
This implementation uses:
- SHA-256 hash for demonstration purposes
- Per-user salt to prevent rainbow table attacks

For production environments, always use a proper Key Derivation Function such as:
- Argon2 (memory-hard, winner of Password Hashing Competition)
- bcrypt
- PBKDF2 (with many iterations)

This project demonstrates the concepts in a standard-library-only environment
without external dependencies. Do NOT use this password hashing for production
systems.

Compilation Instructions (Dev-C++ / GCC)
-----------------------------------------

1. Using Dev-C++:
   a. Open Dev-C++ and create a new "PNG - Small Graphics Project" or
      "C Project".
   b. Name the project "SecureVault".
   c. Add all .c files to the project:
      - main.c
      - auth.c
      - users.c
      - password.c
      - file_manager.c
      - history.c
      - security_logs.c
      - utils.c
   c. Add all .h files to the project:
      - config.h
      - auth.h
      - users.h
      - password.h
      - file_manager.h
      - history.h
      - security_logs.h
      - utils.h
   d. Make sure the compiler is set to C99 or C11 mode:
      - Options -> Compiler Options
      - Add: -std=c99 (or -std=c11)
      - Add: -Wall -Wextra for warnings
   e. Build the project (F11).
   f. Run the executable.

2. Using command line (GCC/MingW):
   gcc -std=c99 -Wall -Wextra -o SecureVault *.c

3. After compilation, run:
   ./SecureVault   (Linux/macOS)
   SecureVault.exe (Windows)

Building from Source (Manual)
-----------------------------
1. Create a new directory SecureVault/
2. Copy all .c and .h files into the directory
3. Create empty data files: users.txt, login_history.txt, security_logs.txt
4. Compile with: gcc -std=c99 -o SecureVault.exe main.c auth.c users.c
    password.c file_manager.c history.c security_logs.c utils.c
5. Run SecureVault.exe

Testing Checklist
-----------------
[ ] Register a new user with valid username and password
[ ] Register with duplicate username - should fail
[ ] Register with short username - should fail
[ ] Register with invalid characters in username - should fail
[ ] Register with weak password - should warn
[ ] Login with correct credentials - should succeed
[ ] Login with incorrect password - should fail
[ ] Login after 3 failed attempts - account should lockout
[ ] Wait for lockout duration and try again
[ ] Change password (verify current password first)
[ ] Search for a registered user
[ ] View login history after authentication
[ ] Admin: view account summaries
[ ] Admin: search users
[ ] Admin: view security logs
[ ] Exit and restart - confirm data persists in files
[ ] Delete account with confirmation

Limitations
-----------
1. SHA-256 is used without key stretching or slow hashing - for educational
   only, not production password storage.
2. Password salt is generated using rand() - not cryptographically secure.
3. Admin role is determined by username "admin" - not a secure authentication
   method. For production, use proper role-based access control.
4. File-based storage does not support concurrent access well.
5. Lockout state is lost on program restart (stored in memory only until next
   save). For production, store lockout state in the user file.
6. No encryption of data files - anyone with file access can read all data.
7. The "admin" account is pre-configured with username "admin" - change the
   password immediately after first login.

For educational purposes only. This project demonstrates C programming
fundamentals and should not be used for actual security-sensitive data
management.