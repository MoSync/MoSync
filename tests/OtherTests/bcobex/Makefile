JDK	:=	C:\Program\Java\jdk1.6.0_04
CP	:=	-cp $(JDK)\jre\lib\bluecove-2.0.2.jar

Bcobex.class: Bcobex.java
	$(JDK)\bin\javac $(CP) $<

#-cp $(JDK)\lib\rt.jar;

run: Bcobex.class
	$(JDK)\bin\java $(CP);. Bcobex 0018c53f747e Makefile
