tcc -c -I.. -DTURBOC *.C
tlib myxinu.lib -+ create.obj
tlib myxinu.lib -+ initiali.obj
tlib myxinu.lib -+ REsCHED.obj
tcc -I.. ex9.c myxinu.lib
ex9.exe