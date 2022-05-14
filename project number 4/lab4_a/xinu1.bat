
tcc -c -I.. -DTURBOC *.C
tlib myxinu.lib -+ initiali.obj
tlib myxinu.lib -+ clkint.obj
tlib myxinu.lib -+ create.obj
tcc -I.. lab4a.c myxinu.lib
lab4a.exe