# Computer_Architecture
This program was built for the purpose simulating the RISC-V architecture. It simulates hardware components, memory, and decoding/encoding/execution of instructions given to the simulator via binary files. The simulator begins by waiting for user input and giving an appropriate usage message. The user has an array of options available to run the simulation including several dump functions, memory size limits, and instruction size limitations.

The simulation also interacts with a registerfile which has simulated GPR's to hold and execute the instructions. As the code is based on real architeture, it has many functions including a tick() function to simulate a working computer clock that will sync instructions.

Once the user has defined options they would like to use and given a binary file the simulator will allocate the appropriate memory size needed. It will continue to manage this memory as the simulation continues. Afterwards the file is read and decoded, and the instructions are either recongized and dumped or executed based on user option commands.
