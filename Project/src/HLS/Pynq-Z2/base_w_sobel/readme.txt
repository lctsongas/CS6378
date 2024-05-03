In order to rebuild:
> source <vivado_install_path>/setup.sh
> cd Project/src/HLS/Pynq_Z2/base_w_sobel
> make

If all goes well, you should see a success print out.
The recompiled binary and hardware file will be present in
base_w_sobel/base_w_sobel/base_w_sobel.runs/impl_1/
and
base_w_sobel/base_w_sobel.srcs/sources_1/bd/base_w_sobel/hw_handoff
respectively. 