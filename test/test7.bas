PRINT "[";
GOSUB xx
PRINT "]"
GOTO END

xx:
	PRINT "ab";
	GOSUB x1
	RET

END:
	PRINT "end"
	END

x1:
	PRINT "c";
	GOSUB x2
	RET

x2:
	PRINT "d";
	RET