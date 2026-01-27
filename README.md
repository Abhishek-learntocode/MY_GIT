

# MyGit: A C++ Implementation of Git

MyGit is a ground-up implementation of a content-addressable version control system, built to understand the internal architecture of Git. It replicates the core functionality of Git, including the object database (blobs, trees, commits), the staging area (index), and commit history traversal.

## 🚀 Features

* **Repository Initialization**: Creates the hidden `.mygit` directory structure.
* **Object Database**: Stores compressed, content-addressable objects (SHA-1 + Zlib).
* **Staging Area**: Manages an index file to track files ready for commit.
* **Commit History**: Creates immutable snapshots linked by parent pointers.
* **Log Traversal**: Parses the linked list of commits to display history.

## 🛠 Architecture

MyGit mimics the data structures used by real Git.

### 1. The Object Store (`.mygit/objects`)

Every piece of data is stored as a zlib-compressed file named after its SHA-1 hash.

* **Blobs**: The raw contents of user files.
* **Trees**: Directories that map filenames to Blob hashes.
* **Commits**: Wrappers that point to a Tree and a Parent Commit.

### 2. The Index (`.mygit/index`)

A flat text file acting as the "Staging Area." It maps filenames to the SHA-1 hash of the content currently ready to be committed.

### 3. HEAD (`.mygit/HEAD`)

A reference file that points to the current branch (e.g., `ref: refs/heads/main`), tracking the latest commit in the timeline.

## 💻 Installation & Usage

### Prerequisites

* G++ Compiler (Supporting C++17)
* OpenSSL (`-lssl -lcrypto`)
* Zlib (`-lz`)
* Make

### Building

```bash
make

```

### Commands

#### 1. Initialize a Repository

Creates the `.mygit` folder structure.

```bash
./mygit init

```

#### 2. Add Files

Hashes the file content, stores it as a Blob, and updates the Index.

```bash
./mygit add <filename>

```

#### 3. Commit Changes

Creates a Tree object from the Index, wraps it in a Commit object with author info, and updates `HEAD`.

```bash
./mygit commit "Your message here"

```

#### 4. View History

Traverses the commit history backwards from `HEAD`.

```bash
./mygit log

```

#### 5. Low-Level Inspection (Plumbing)

Inspect raw objects in the database.

```bash
# Hash a file and write to database
./mygit hash-object -w <file>

# Read content of an object
./mygit cat-file -p <sha1>

# Check size of an object
./mygit cat-file -s <sha1>

```

## 🧩 Project Structure

```text
.
├── include/
│   ├── commands.h      # Command function declarations
│   ├── index.h         # Index class definition
│   └── utils.h         # File I/O, hashing, and compression helpers
├── src/
│   ├── commands/       # Implementation of add, commit, init, log...
│   ├── index.cpp       # Staging area logic
│   ├── utils.cpp       # Core utility implementation
│   └── main.cpp        # CLI entry point
└── Makefile            # Build configuration

```

## 🧠 What I Learned

* **Content-Addressable Storage**: How files can be stored by the hash of their content rather than their name.
* **Data Structures**: Implementing Trees and Linked Lists using file pointers (SHA-1 hashes) instead of memory pointers.
* **Systems Programming**: Direct file manipulation, binary data handling, and compression in C++.