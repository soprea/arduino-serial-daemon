arduino-serial-daemon
=====================
g++ \`mysql_config --cflags\` \`mysql_config --libs\` arduino-serial-daemon.c arduino-serial-lib-daemon.c -o arduino-serial-daemon
=====================
 You are using this at your own risk. I am not a programmer. I've used my knowledge from school and I've googled a lot in order to make this work(BTW, it's also my first C program for linux). I am sure that the code can be cleaner and can be improved, but it's working for me.
 If you want to participate in improving this code, I will be more then happy to hearing from you.
 
 The code will listen on the serial interface, and he will write everything in a mysql database (you can find the schema in the pihome code in the other github branch) and also on a output.txt file in the current working directory.
 Everything is hard coded, I am thinking to create a configuration file who will be read from time to time by the app, but it will take some time, so you have to make some changes before compilation.
 
