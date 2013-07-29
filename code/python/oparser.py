import re
import struct
import sys
import os

file   = "test"
cpparg = ""
B64    = ""

if len( sys.argv ) > 1:
    file = sys.argv[1]

if len( sys.argv ) > 2:
    if sys.argv[2] == "64":
        cpparg = " -D_64BLOCKS"
        B64    = "64"


print( "Parsing " + file )

infunction = 0
intext     = 0
textc      = 0

functions = []
fcounter  = 0

#prefix = "np_"
prefix = ""

make_ = 0


compiler_includes = [ "/opt/cell/sysroot/usr/spu/include/simdmath", "/opt/cell/sysroot/opt/cell/sdk/usr/spu/include", "/opt/cell/sysroot/usr/spu/include/simdmath" ] #, "/root/Desktop/shadertest/spu_mult_shader" ]

compiler_libdirs  = [ "/opt/cell/sysroot/opt/cell/sdk/usr/spu/lib" ]
compiler_libs     = [ "misc" ]

# regexp
regexp = re.compile(r"(\w+\s\w+\s\w+\s\w+)")

# Compile file

cmd = "spu-gcc -O3 -Winline -finline-limit=10000" + cpparg
for inc in compiler_includes:
    cmd += " -I" + inc

#for inc in compiler_libdirs:
#    cmd += " -L" + inc

#for inc in compiler_libs:
#    cmd += " -l" + inc

cmd += " -c " + file + ".c -o " + file + ".o"
os.system( cmd )
print( "$ " + cmd );

# Dump object file

cmd = "spu-objdump -D " + file + ".o > " + file + ".o.dump"
os.system( cmd )

print( "$ " + cmd );

# Parse dumped object file

filep = open( file + ".o.dump", "r" )

for line in filep:
    pline = line.split(" ")
    if infunction == 1:
        # in function
        if pline[0] == "\n":
#            print("End of function")
            infunction = 0
            textc = 0
        else:
            m = regexp.search(line)
	    if m:
            	functions[fcounter-1][1].append( int( "0x" + m.group(0).replace(" ", ""), 16 ) )


    if intext == 1:
        # do something
        textc += 1
#        print("in text")
        if textc == 2 and pline[0] != "Disassembly": # New function
            fname = pline[1][1:-3]
            if fname[0] != '_':
                print( "Creating function \"" + fname + "\"." )
            functions.append([ fname, [] ] )
            fcounter += 1
            infunction = 1

    # Switch "environment"
    if pline[0] == "Disassembly":
        intext = 0

        pline[3] = pline[3].strip()
        if pline[3] == ".text:":
            # In the text section:
            intext = 1
        else:
            intext = 0
            infunction = 0
        


# Create binary files
for bfile in functions:
    if bfile[0][0] == '_' and make_ == 0:
#        print( "Skipping function " + bfile[0] );
        continue;

    myfileb = open( prefix + bfile[0] + B64 + ".bin", "wb" )
    myfilea = open( prefix + bfile[0] + B64 + ".h", "wb" )

    myfilea.write( "#ifndef __" + prefix + bfile[0].upper() + "_H_\n#define __" + prefix + bfile[0].upper() + "_H_\n\n" )
    myfilea.write( "unsigned int " + prefix + bfile[0] + "[] __attribute__((aligned(16))) = { " )

    clen = len( bfile[1] );
    c = 1
#    print( "Length of " + bfile[0] + " = " + str( clen ) )
    
    for d in bfile[1]:
        myfilea.write(str(d)+"U")
        if c < clen:
            myfilea.write(", ")

        d = struct.pack(">I", d)
        myfileb.write(str(d))
        c = c + 1

    myfilea.write( " };\n\n" )
    myfilea.write( "unsigned int " + prefix + bfile[0] + "_size = " + str( clen * 4 ) + "; // In bytes\n\n" )
#    myfilea.write( "#define " + prefix + bfile[0] + "_size = " + str( clen * 4 ) + "; // In bytes\n\n" )
    myfilea.write( "#endif /* __" + prefix + bfile[0].upper() + "_H_ */\n" )
        
    myfileb.close()
    myfilea.close()
