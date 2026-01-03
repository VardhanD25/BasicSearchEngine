# Basic Search Engine 

## Overview

- This project is a mini-search engine designed to crawl, index, and search web documents. It features a hybrid architecture with a Python-based web crawler for data acquisition and a C++ core engine for efficient indexing and query processing. The system implements a persistent Inverted Index and uses Vector Space Model (TF-IDF) scoring for ranked search results.

## Project Architecture
- The system is divided into two main components that communicate via a shared data repository:

1. Data Acquisition (Python Crawler):

    - Fetches web pages starting from seed URLs.

    - Parses HTML content to extract text and metadata (URL, Title).

    - Performs link discovery (Breadth-First Search) to populate the crawl frontier.

    - Saves processed documents as text files in the data/ directory.

2. Core Engine (C++):

    - Tokenizer: Processes raw text, removes stop words, and normalizes tokens.

    - Indexer: Scans the data/ directory and builds an in-memory Inverted Index mapping terms to document IDs.

    - Persistence: Serializes the index, lexicon, and document table to binary files on disk (index_data/) for fast loading.

    - Query Engine: Supports Boolean retrieval (AND/OR) and Ranked retrieval using TF-IDF with document length normalization.

## Directory Structure

```text

SEARCH_ENGINE/
├── CMakeLists.txt       # Main build configuration
├── crawler/             # Python Crawler component
│   ├── crawler.py       # Main crawling logic
│   └── requirements.txt # Python dependencies
├── src/                 # C++ Core Engine source code
│   ├── index/           # Inverted Index data structures
│   ├── indexer/         # File parsing and index construction
│   ├── parser/          # Tokenization and text processing
│   ├── query/           # Search algorithms (Boolean & Ranked)
│   ├── utils/           # Utility functions
│   └── main.cpp         # Entry point for the application
├── data/                # Raw crawled documents (Text files)
└── index_data/          # Persisted binary index files
```

## Prerequisites
- C++ Compiler: GCC, Clang, or MSVC (supporting C++17 or later).

- CMake: Version 3.10 or higher.

- Python: Version 3.6 or higher.

- Python Libraries: requests, beautifulsoup4.

## Setup and Usage Instructions

1. Phase 1: Data Acquisition (The Crawler)

- Before running the search engine, you need to populate the data directory with content.
Navigate to the crawler directory:

```bash
cd crawler
```
- Install the required dependencies:

```bash
pip install -r requirements.txt
```
- Run the crawler:

```bash
python crawler.py
```
- This will fetch pages from Wikipedia (configured in crawler.py) and save them to the ../data directory.

2. Phase 2: Building the Engine

- Navigate to the project root and create a build directory:

```Bash
mkdir build
cd build
```
- Generate build files using CMake:

```Bash
cmake ..
```
- Compile the project:

```Bash
cmake --build .
```

3. Phase 3: Indexing and Searching
- Run the compiled executable. The system operates in two logical modes: Indexing (on first run or rebuild) and Searching.

- Run the executable:
```text
Windows: .\src\Debug\search_engine.exe (path may vary based on configuration)

Linux/Mac: ./src/search_engine
```
## Indexing:

On the first run, ensure rebuildIndex is set to true in main.cpp.

The engine will scan ../data, tokenize documents, and write the index to ../index_data.

## Searching:

Once the index is loaded, you will be prompted to enter a query.

Example Query: software engineering

The system will return ranked results with document titles and relevance scores.

Type exit to quit.

## Features
- Inverted Indexing: Efficient mapping of terms to document postings lists.

- Stop Word Removal: Filters out common English words (e.g., "the", "is") to optimize storage and relevance.

- TF-IDF Scoring: Ranks documents based on Term Frequency-Inverse Document Frequency.

- Document Length Normalization: Penalizes strictly long documents to improve result quality.

- Persistent Storage: Saves the index structure to disk, allowing instant startup without re-indexing raw files.

- Metadata Support: Captures and displays document titles and original URLs alongside search results.

## Project Scope
This project serves as a foundational implementation of a full-text search engine. It demonstrates the core principles of Information Retrieval (IR), including web crawling, text processing, data structure design for indexing, and ranking algorithms. It is designed to be modular, allowing individual components (like the tokenizer or ranking formula) to be swapped or upgraded without refactoring the entire system.