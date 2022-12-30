```
  ____         __ _____           
 |  _ \       / _/ ____|          
 | |_) |_ __ | || (___   ___  ___ 
 |  _ <| '_ \|  _\___ \ / _ \/ __|
 | |_) | |_) | | ____) |  __/ (__ 
 |____/| .__/|_||_____/ \___|\___|
       | |                        
       |_|    
```

## What is it about?
Bpfsec is a basic implementation based on eBPF and KRSI (Kernel Runtime Security Instrumentation).
It just uses BPF LSM concept to monitor processes currently and in the future it can block them (TODO).

## Setup
Just run **build.sh** and it handles everything for you. There is also Vagrantfile to facilitate the setup of the virtual machine,
You can use `vagrant up` to setup the desired machine, then use `vagrant ssh` to ssh to the machine and test the bpfsec.
*Note*: libbpf is submodule of this project, so you don't need to build it from kernel source directly.

## Knowledge Sharing
There are multiple helpful websites and resources that are listed below:  
- what is vmlinux.h?
  -  https://blog.aquasec.com/vmlinux.h-ebpf-programs 
- libbpf library
  - https://github.com/libbpf/libbpf
- BPF Kernel Docs
  - https://www.kernel.org/doc/html/latest/bpf/
- KRSI Docs
  - https://github.com/torvalds/linux/commit/641cd7b06c911c5935c34f24850ea18690649917