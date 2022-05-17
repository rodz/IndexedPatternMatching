import os

path_files = './data/test_archive/'

files = ("dna", "english", "sources")
zipped = ".myzip"


sizes = (".50MB", ".100MB", ".200MB", "")
print(f"----------------- ZIP ----------------")

for file in files:
    for termination in sizes:
        if termination == "":  
            size = "FULL" 
        else: 
            size = termination
        
        print(f"-------------This is {file} file with {size} size -------------")
        os.system(f"time ./bin/ipmt --count zip {path_files + file + termination}")


print(f"----------------- UNZIP ----------------")
for file in files:
    for termination in sizes:
        if termination == "":  
            size = "FULL" 
        else: 
            size = termination
        
        print(f"-------------This is {file} file with {size} size -------------")
        os.system(f"time ./bin/ipmt --count unzip {path_files + file + termination + zipped}")