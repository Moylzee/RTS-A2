In order to stress the CPU I used ‘stress-ng’, a command that allows me to set the stress load as a percentage and the number of CPUs to stress. The command is formatted as follows: 
`stress-ng –cpu 8 –cpu-load 70 –cpu-method matrixprod`
Where:
-	Stress-ng = command 
-	--cpu = number of CPU cores to stress
-	--cpu-load = percentage of CPU to use 
-	--cpu-method = the stress method to use

I followed the same strategy for stressing the memory, with the command:
`stress –vm 4 –vm-bytes 128`
Where:
-	Stress = command 
-	--vm = The number of virtual memory stressors too run
-	--vm-bytes = The amount of memory allocated in bytes per stressor
-	The memory method used is vm for memory stress
I also allocated 1GB of swapping memory to prevent the VM from crashing, in the event it runs out of physical RAM, causing the VM to freeze or crash. It improves stability, as this is an experiment with heavy memory stress, it provides additional virtual memory. Preventing the system from running out of memory.
