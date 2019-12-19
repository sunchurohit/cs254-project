TEAM NAME : RAMpage (160050072, 160050073, 160050074, 160050097)

VHDL compilation.
	Replace toplevel.vhd, harness.vhd, board.ucf in the templates folder
	with these files in their respective directories.
	Paste the remaining files in the cksum folder.
	compilation is to be done as per the instructions in the 
	github link posted on the course webpage.
C compilation
	Replace the files in C folder with the files in flci, lib directories in their 
	respective positions.
	Run make files in libfgpa, libwrapusp, flci folders.

UART communication.
	We have completed the optional part. Boards can communicate with each other using 
	the computer as a relay. 
	We have done the mandatory part as follows. 
	We used the basic_uart module to transfer and receive data from board to computer 
	using gtkterm gui.
	The command for running the gtkterm interface is 
		sudo gtkterm -p 'name of the device' -s 2400 // our baudrate.
	The Optional part is done by the backend computer acting as a relay between the boards.
	For the communication, the 2 python files are to be executed.


Input for optional part:
	The two python scripts should be running while implementing the optional part.
	First left_key is pressed, then input switches are changed and finally the right_key is pressed to send the data to the other controller.


