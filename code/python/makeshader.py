import re
import struct
import sys
import os
import commands

# Generic functions
types = [ 
    ### BINARY FUNCTIONS
    [ 'genericbinaryfunction.c', [ 
            [ 'arraymultiply', 'info->LS_blockDataArea[idx*3], info->LS_blockDataArea[idx*3+1], info->LS_blockDataArea[idx*3+2], numelements', '', '4', '4' ],
            [ 'arraydivide', 'info->LS_blockDataArea[idx*3], info->LS_blockDataArea[idx*3+1], info->LS_blockDataArea[idx*3+2], numelements', '', '4', '4' ],
            [ 'arrayremainder', 'info->LS_blockDataArea[idx*3], info->LS_blockDataArea[idx*3+1], info->LS_blockDataArea[idx*3+2], numelements', '', '4', '4' ],
            [ 'arrayadd', 'info->LS_blockDataArea[idx*3], info->LS_blockDataArea[idx*3+1], info->LS_blockDataArea[idx*3+2], numelements', '', '4', '4' ],
            [ 'arraysub', 'info->LS_blockDataArea[idx*3], info->LS_blockDataArea[idx*3+1], info->LS_blockDataArea[idx*3+2], numelements', '', '4', '4' ],
            [ 'arraysubtract', 'info->LS_blockDataArea[idx*3], info->LS_blockDataArea[idx*3+1], info->LS_blockDataArea[idx*3+2], numelements', '', '4', '4' ],
            [ 'arrayless', 'info->LS_blockDataArea[idx*3], info->LS_blockDataArea[idx*3+1], info->LS_blockDataArea[idx*3+2], numelements', '', '4', '1' ],
            [ 'arraylessequal', 'info->LS_blockDataArea[idx*3], info->LS_blockDataArea[idx*3+1], info->LS_blockDataArea[idx*3+2], numelements', '', '4', '1' ],
            [ 'arraygreater', 'info->LS_blockDataArea[idx*3], info->LS_blockDataArea[idx*3+1], info->LS_blockDataArea[idx*3+2], numelements', '', '4', '1' ],
            [ 'arraygreaterequal', 'info->LS_blockDataArea[idx*3], info->LS_blockDataArea[idx*3+1], info->LS_blockDataArea[idx*3+2], numelements', '', '4', '1' ]
            ] 
      ],

    [ 'genericbinaryfunction_scalar.c', [ 
            # op1 = array and op2 = scalar
            [ 'arraymultiply_arrayscalar', 'info->LS_blockDataArea[idx*3], &scalar_v, info->LS_blockDataArea[idx*3+2], numelements', '', '4', '4' ],
            [ 'arraydivide_arrayscalar', 'info->LS_blockDataArea[idx*3], &scalar_v, info->LS_blockDataArea[idx*3+2], numelements', '', '4', '4' ],
            [ 'arrayremainder_arrayscalar', 'info->LS_blockDataArea[idx*3], &scalar_v, info->LS_blockDataArea[idx*3+2], numelements', '', '4', '4' ],
            [ 'arrayadd_arrayscalar', 'info->LS_blockDataArea[idx*3], &scalar_v, info->LS_blockDataArea[idx*3+2], numelements', '', '4', '4' ],
            [ 'arraysubtract_arrayscalar', 'info->LS_blockDataArea[idx*3], &scalar_v, info->LS_blockDataArea[idx*3+2], numelements', '', '4', '4' ],
            [ 'arrayless_arrayscalar', 'info->LS_blockDataArea[idx*3], &scalar_v, info->LS_blockDataArea[idx*3+2], numelements', '', '4', '1' ] ,
            [ 'arraylessequal_arrayscalar', 'info->LS_blockDataArea[idx*3], &scalar_v, info->LS_blockDataArea[idx*3+2], numelements', '', '4', '4' ] ,
            [ 'arraygreater_arrayscalar', 'info->LS_blockDataArea[idx*3], &scalar_v, info->LS_blockDataArea[idx*3+2], numelements', '', '4', '1' ] ,
            [ 'arraygreaterequal_arrayscalar', 'info->LS_blockDataArea[idx*3], &scalar_v, info->LS_blockDataArea[idx*3+2], numelements', '', '4', '1' ] ,

            # op1 = scalar and op2 = array
            [ 'arraymultiply_scalararray', 'info->LS_blockDataArea[idx*3], &scalar_v, info->LS_blockDataArea[idx*3+2], numelements', '', '4', '4' ],
            [ 'arraydivide_scalararray', 'info->LS_blockDataArea[idx*3], &scalar_v, info->LS_blockDataArea[idx*3+2], numelements', '', '4', '4' ],
            [ 'arrayremainder_scalararray', 'info->LS_blockDataArea[idx*3], &scalar_v, info->LS_blockDataArea[idx*3+2], numelements', '', '4', '4' ],
            [ 'arrayadd_scalararray', 'info->LS_blockDataArea[idx*3], &scalar_v, info->LS_blockDataArea[idx*3+2], numelements', '', '4', '4' ],
            [ 'arraysubtract_scalararray', 'info->LS_blockDataArea[idx*3], &scalar_v, info->LS_blockDataArea[idx*3+2], numelements', '', '4', '4' ],
            [ 'arrayless_scalararray', 'info->LS_blockDataArea[idx*3], &scalar_v, info->LS_blockDataArea[idx*3+2], numelements', '', '4', '1' ],
            [ 'arraylessequal_scalararray', 'info->LS_blockDataArea[idx*3], &scalar_v, info->LS_blockDataArea[idx*3+2], numelements', '', '4', '1' ],
            [ 'arraygreater_scalararray', 'info->LS_blockDataArea[idx*3], &scalar_v, info->LS_blockDataArea[idx*3+2], numelements', '', '4', '1' ],
            [ 'arraygreaterequal_scalararray', 'info->LS_blockDataArea[idx*3], &scalar_v, info->LS_blockDataArea[idx*3+2], numelements', '', '4', '1' ],

            ] 
      ],
      
      [ 'genericbinaryfunction_scalar_slice.c', [ 
            # op1 = array and op2 = scalar
            [ 'arrayslicerow_arrayscalar', 'info->LS_blockDataArea[idx*3], info->LS_blockDataArea[idx*3+1], info->LS_blockDataArea[idx*3+2], numelements', '', '4', '4' ]
            ] 
      ],

    ### UNARY FUNCTIONS
    [ 'genericunaryfunction.c', [ 
            [ 'arraysqrt', 'info->LS_blockDataArea[idx*3], info->LS_blockDataArea[idx*3+2], numelements', '&res', '4', '4' ],
            [ 'arrayfloor', 'info->LS_blockDataArea[idx*3], info->LS_blockDataArea[idx*3+2], numelements', '&res', '4', '4' ],
            [ 'arrayceil', 'info->LS_blockDataArea[idx*3], info->LS_blockDataArea[idx*3+2], numelements', '&res', '4', '4' ],
            ] 
      ],

    # With scalar return value
    [ 'genericunaryfunction_srv.c', [ 
            [ 'arraysum', 'info->LS_blockDataArea[idx*3], &res, numelements', '&res', '4', '4' ],
            ] 
      ],

    # With scalar return value
    [ 'genericunaryfunction_srv_bool.c', [ 
            [ 'arraysum_bool', 'info->LS_blockDataArea[idx*3], resPointer, numelements', 'resPointer', '4', '4' ],
            ] 
      ],


    ### FILLERS
    [ 'genericfillerfunction.c', [ 
            [ 'randomfiller', 'info->LS_blockDataArea[idx], numelements, funcs->rand_0_to_1_f', 'info->LS_blockDataArea[nextidx], numelements, funcs->rand_0_to_1_f', '4', '4' ] ,
            [ 'zerofiller', 'info->LS_blockDataArea[idx], numelements', 'info->LS_blockDataArea[nextidx], numelements', '4', '4' ],
            [ 'onefiller', 'info->LS_blockDataArea[idx], numelements', 'info->LS_blockDataArea[nextidx], numelements', '4', '4' ] 
            ] 
      ]
]

# Reg Exp
r_header = re.compile( '\#\#header\#\#' )
regexp = re.compile(r"(\w+\s\w+\s\w+\s\w+)")

compiler_includes = [ "/opt/cell/sysroot/usr/spu/include/simdmath", "/opt/cell/sysroot/opt/cell/sdk/usr/spu/include", "/opt/cell/sysroot/usr/spu/include/simdmath" ]

# 
prefix = ""
make_ = 0
maketmp = 1
movetoshaders = 1

errors = [ 0, [] ]
shaderidx = 0
errorfile = "error.log"

# Compile file
#def Compile(file,function, _compute1, _compute2):
def Compile(file,function, replace):
#    print(file + ', ' + function)

    errors[1].append( [ function, 0, [] ] )

    f = open( file, "r" )
    data = f.read()
    f.close()

    data = data.replace( "##header##", function + '.h' );

    # REPLACE
    for r,w in replace.iteritems():
        data = data.replace( "##" + r + "##", w );

    # Run command
    cmd2 = "rm tmp.*"
    print( "$ " + cmd2 );
#    error = os.system( cmd2 )
    error = commands.getoutput( cmd2 )
    if error:
#        errors.append( [ func + ": Couldn't remove temp files", func, error ] )
#        errors[func].append( [ func + ": Couldn't remove temp files", error ] )
        errors[1][shaderidx][2].append( [ "Couldn't remove temp files", error ] )
        errors[0] += 1
        errors[1][shaderidx][1] += 1

    # Write changes
    f = open( 'tmp.c', 'w' )
    f.write( data )
    f.close()

    if maketmp:
        f = open( 'temp/' + function + '.c', 'w' )
        f.write( data )
        f.close()

    # Include the header
    cmd = "spu-gcc -O3 -Winline -finline-limit=3000"
    for inc in compiler_includes:
        cmd += " -I" + inc

    cmd += " -c tmp.c -o tmp.o"

    # Run command
    print( "$ " + cmd );
#    error = os.system( cmd )
    error = commands.getoutput( cmd )
    if error:
#        errors.append( [ func + ": Failed compiling", func, error ] )
        print("----------------->"+str(error))
        errors[1][shaderidx][2].append( [ "Failed compiling", error ] )
        errors[0] += 1
        errors[1][shaderidx][1] += 1

    # Dump object file
    cmd = "spu-objdump -D tmp.o > tmp.o.dump"
#    error = os.system( cmd )
    error = commands.getoutput( cmd )

    if error:
#        errors.append( [ func + ": Failed dumping", func, error ] )
        errors[1][shaderidx][2].append( [ "Failed dumping", error ] )
        errors[0] += 1
        errors[1][shaderidx][1] += 1
    
    print( "$ " + cmd );


# Parse dumped object file
def ParseOFile():
    infunction = 0
    intext     = 0
    textc      = 0
    
    functions = []
    fcounter  = 0

    filep = open( "tmp.o.dump", "r" )

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
#                if fname[0] != '_':
#                    print( "Creating function \"" + fname + "\"." )
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
        
    return functions


# Create binary files
def CreateBinary(functions, func):
    for bfile in functions:
        if bfile[0][0] == '_' and make_ == 0:
            continue;

#        print(bfile[0])

        myfileb = open( func + ".bin", "wb" )
        myfilea = open( func + ".h", "wb" )

        myfilea.write( "#ifndef __" + prefix + func.upper() + "_H_\n" )
        myfilea.write( "#define __" + prefix + func.upper() + "_H_\n\n" )
        myfilea.write( "unsigned int " + prefix + func + "[] __attribute__((aligned(16))) = { " )

        clen = len( bfile[1] );
        c = 1

#        print('length='+str(clen))
    
        for d in bfile[1]:
#            print(str(d))
            myfilea.write(str(d)+"U")
            if c < clen:
                myfilea.write(", ")

            d = struct.pack(">I", d)
            myfileb.write(str(d))
            c = c + 1


        myfilea.write( " };\n\n" )
        myfilea.write( "unsigned int " + prefix + func + "_size = " + str( clen * 4 ) + "; // In bytes\n\n" )
#        myfilea.write( "#define " + prefix + func + "_size = " + str( clen * 4 ) + "; // In bytes\n\n" )
        myfilea.write( "#endif /* __" + prefix + func.upper() + "_H_ */\n" )
        
        myfileb.close()
        myfilea.close()

        if movetoshaders:
            cmd  = 'mv ' + func + '.bin ../' + func + '.bin;'
            cmd += 'mv ' + func + '.h ../' + func + '.h;'
            print( "$ " + cmd )
            os.system( cmd )


shader = 'all'

if len( sys.argv ) > 1:
    shader = sys.argv[1]

c = 0

for t in types:
    for f in t[1]:
        func = f[0]
        if shader == 'all' or shader == func:
            _compute1 = f[1]
            _compute2 = f[2]
            replace = {'args1':f[1], 'args2':f[2], 'size1':f[3], 'size2':f[4]}
            print( "------------------------------------------\nMaking " + func )
            
            Compile( t[0], func, replace )
            functions = ParseOFile()
            CreateBinary(functions,func)

            c += 1
            shaderidx += 1


print( "---------------- DONE ----------------------------" )
if errors[0] > 0 :

    f = open( errorfile, "w" )
    
    print( "Error report(can be warnings)[" + str(errors[0]) + "]:" )

#    for v,e in errors:
#    for v,er in errors[1].iteritems():
    for er in errors[1]:
        if er[1] == 0:
            continue
        f.write( "---------------------------\n" )
        f.write( "Error report for " + er[0] + ":\n\n" )
        print( "Errors in " + er[0] )
        for e in er[2]:
            f.write( e[0] + ":" )
            f.write( "\n" )
            f.write( e[1] )
            f.write( "\n\n" )
            print(e[0])

        print("")


    f.close()

    print( "Read error.log for more details." )

else:
    print( str( c ) + " shaders compiled with no errors or warnings" )
