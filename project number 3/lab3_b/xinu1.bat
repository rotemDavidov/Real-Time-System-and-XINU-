tcc -c -I.. -DTURBOC *.C
tlib myxinu.lib + cByT.obj
tlib myxinu.lib -+ create.obj
tlib myxinu.lib -+ initiali.obj
tlib myxinu.lib + XEXECL.obj
tcc -I.. ex9.c myxinu.lib
ex9.exe