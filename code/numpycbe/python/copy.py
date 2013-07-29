import sys
import os
import commands

sourcedir  = "src/"
includedir = "include/"
libdir     = "lib/"
shaderdir  = "shaders/"
linalgdir  = "linalg/"

error = ""

def Copy(file, to):
    cmd = "cp " + file + " " + to
    length = len(file)
    s = " "*(40-length)
    print "Copying " + file + s + " ==> \t" + to
    error = os.system( cmd )


Copy( "../common/common.h", includedir )
Copy( "../common/common_ppu.h", includedir )
Copy( "../libs/libppu.h", includedir )
Copy( "../libs/libpputool.a", libdir )
Copy( "../libs/libppu.c", sourcedir )
Copy( "../libs/fhbtool.h", includedir )
#Copy( "../libs/fhbtool.c", sourcedir )
Copy( "../libs/libppufhbtool.a", libdir )
Copy( "../libs/timer.h", includedir )
Copy( "../libs/libpputimer.a", libdir )
Copy( "/opt/cell/sysroot/opt/cell/sdk/usr/include/malloc_align.h", includedir )

# Dispatcher
Copy( "../dispatcher/spudispatcher", "." )

# Solve
Copy( "../ppe-code/solve.h", includedir )
Copy( "../ppe-code/solve.c", sourcedir )

#  Shaders
Copy( "../shaders/arraymultiply.h", shaderdir )
Copy( "../shaders/arrayadd.h", shaderdir )
Copy( "../shaders/arraysum.h", shaderdir )
Copy( "../shaders/randomfiller.h", shaderdir )
Copy( "../shaders/arraylessequal_arrayscalar.h", shaderdir )

Copy( "../shaders/zerofiller.h", shaderdir )
Copy( "../shaders/arraymultiply_scalararray.h", shaderdir )
Copy( "../shaders/arraymultiply_arrayscalar.h", shaderdir )
Copy( "../shaders/arraysub.h", shaderdir )
Copy( "../shaders/arraydivide.h", shaderdir )

Copy( "../shaders/sger.h", shaderdir )
Copy( "../shaders/sger64.h", shaderdir )

Copy( "../shaders/sger_lu.h", shaderdir )
Copy( "../shaders/strsm_llnu.h", shaderdir )
Copy( "../shaders/strsm_llnu_lu.h", shaderdir )
Copy( "../shaders/strsm_lunn.h", shaderdir )
Copy( "../shaders/spu_mult_shader.h", shaderdir )

Copy( "../shaders/sger_lu64.h", shaderdir )
Copy( "../shaders/strsm_llnu64.h", shaderdir )
Copy( "../shaders/strsm_llnu_lu64.h", shaderdir )
Copy( "../shaders/strsm_lunn64.h", shaderdir )
Copy( "../shaders/spu_mult_shader_not_unrolled.h", shaderdir )

# SOR
Copy( "../shaders/arrayslicerow_arrayscalar.h", shaderdir )



# Save to tar
cmd = "tar -cvf numpycbe.tar *"
print "Saving to numpycbe.tar"
error = os.system( cmd )
