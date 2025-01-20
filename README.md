# FPGA for Computer Vision Tasks over PCIe

## Hardware
### [RHS Research NiteFury II](https://github.com/RHSResearchLLC/NiteFury-and-LiteFury)
- Artix-7 200T (XC7A200T-2FBG484E)
- 1GB DDR3-800 RAM
- 256 MB Flash

### Computer with M.2 M-Key Port
- USB-A to M.2 DOES NOT WORK
- Need direct M.2 on motherboard or M.2 to Thunderbolt (need verification)
- Current PoC will be on an [Orange Pi 5 Plus](http://www.orangepi.org/html/hardWare/computerAndMicrocontrollers/details/Orange-Pi-5-plus.html), which includes an M.2 M-Key slot

## Software/Drivers
### [AMD Vitis 2024.2](https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/vitis.html)
- Includes Vivado and Vitis HLS

### [RapidStream TAPA](https://tapa.readthedocs.io/en/main/user/intro.html)
- HLS C++ framework for high-frequency FPGA dataflow accelerators
- Requires Vitis

### [Xilinx XDMA IP Drivers](https://github.com/Xilinx/dma_ip_drivers)
- For memory-mapping over PCIe

## Resources
### [RHS Research NiteFury/LiteFury example project](https://github.com/RHSResearchLLC/NiteFury-and-LiteFury)
- Includes basic Vivado project (outdated, needs IP upgrades for 2024.2) and sample host code for DDR transfer

# Project Timeline
## :white_check_mark: Initial sample testing, data transfer
- Successfully installed software/drivers on Arch Linux
- Successfully built RHS Research sample project kernel (with IP upgrades)
- Successfully ran RHS Research sample host code script for both device info reading and DDR transfer (1 GB in ~1.3s)
## :white_square_button: Test arbitrary operation on data, performance log
- Design basic vector operation (vadd/vmul/dot product) as TAPA HLS kernel
- Test correctness with C-sim
- Integrate with Vivado Block Diagram and XDMA IP for communication
- Create testing script for correctness and measuring latency
## :white_square_button: Godot initial project structure
- Set up basic Godot project structure
- Set up assets
## :white_check_mark: Godot basic city scene
- Add roads, traffic signs/lights, sidewalks, buildings, trees, vehicles
## :x: Godot vehicle controls
- Add user controls for vehicle
- Add movement for NPC vehicles
## :x: Godot simulator sensor I/O
- Save stereo camera buffers from Godot and write to file or stream
- Read processed depth buffer from file or stream
## :x: C++ implementation and simulation SGM kernel, performance log cycles
- Implement SGM kernel in C++, test with C-sim, and verify results in Python with OpenCV
- Log cycle count
## :x: Hardware emulation SGM kernel, performance log cycles
- Test SGM kernel with `v++` hardware emulation
- Log cycle count
## :x: Hardware implementation SGM kernel, performance log cycles/latency
- Integrate SGM kernel with Vivado Block Diagram with XDMA IP
- Create testing script for communication
- Log cycle count/end-to-end latency
## :x: Godot-XDMA Integration
- Transfer image data between Godot simulator and XDMA memory-mapped buffer
