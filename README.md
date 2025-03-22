# MP3 Metadata Parser & Renamer  

A simple C program that reads metadata from MP3 files and renames them based on their tags.

## Features  
- Parses MP3 metadata (ID3 tags)  
- Renames MP3 files using extracted metadata  
- Command-line interface for batch processing  

## Installation  
### Prerequisites  
Ensure you have a C compiler (e.g., GCC) installed.  

### Building from Source  
```bash
git clone https://github.com/La-cruz-kun/mp3-renamer.git
cd mp3-parser  
gcc -o mp3parser main.c
```

## Usage
To rename an MP3 file
```bash
./mp3parser my_song.mp3
```
For batch renaming 
```bash
./mp3parser *.mp3
```
