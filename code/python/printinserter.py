import re
import sys
import os

file = "test"
bak  = 0

if len( sys.argv ) > 1:
    file = sys.argv[1]
else:
    print( "No file specified." )
    exit()

if len( sys.argv ) > 2:
    bak = 1

function = re.compile( r"^([\w\*\_]+\s?){1,3}\(.*?\)\s?\{", re.MULTILINE )
#function = re.compile( r"\((.*)\)", re.MULTILINE )

print( "Parsing " + file )

f = open( file, "r" )
data = f.read()

iter = function.finditer(data)

#print('size ' + length(iter) )

for match in iter:
    if re.search( 'if\s?\(|for\s?\(|while\s?\(', match.group() ):
        print("NAY")
    else:
        data = data.replace( match.group(), match.group() + "\nprintf( \"Function " + match.group(1) + " at line " + str( match.start() ) + " in file " + file + '\\n' + "\" );" )

f.close()

if bak == 1:
    f = open( file + '.bak', "w" )
else:
    f = open( file, "w" )
f.write(data)
f.close()
