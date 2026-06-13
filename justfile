set export := true

OBJCACHE := "ccache"
# CXX := "ccache g++"

test tb="sa_tb": 
  verilator --cc {{tb}}.sv --trace-fst --build -CFLAGS -O0 -CFLAGS -fuse-ld=mold --verilate-jobs 16 --threads 4 --hierarchical --timing --binary --Mdir {{tb}}_obj/
  cd ./{{tb}}_obj/ && ./V{{tb}}

# SV AXI-control testbench
test-ctrl:
  verilator --cc sa_wrapper_axi_ctrl_tb.sv --trace-fst --build \
    -CFLAGS -O0 -CFLAGS -fuse-ld=mold \
    --verilate-jobs 16 --threads 4 --hierarchical --timing --binary \
    --Mdir sa_wrapper_axi_ctrl_tb_obj/ -Wno-WIDTHTRUNC -Wno-WIDTHEXPAND -Wno-UNOPTFLAT
  cd sa_wrapper_axi_ctrl_tb_obj/ && timeout 15 ./Vsa_wrapper_axi_ctrl_tb

# # C++ testbench: compile DUT + tb_main.cpp into shared obj dir
# test-cpp: (bind-cpp)
#   cd ./sa_wrapper_axi_ctrl_tb_obj/ && timeout 30 ./Vsa_wrapper_axi_ctrl_sv

# # Build C++ testbench (without running)
# bind-cpp:
#   verilator --cc sa_wrapper_axi_ctrl_sv.sv --trace-fst --build \
#     -CFLAGS -O0 -CFLAGS -fuse-ld=mold \
#     --verilate-jobs 16 --exe tb_main.cpp --Mdir sa_wrapper_axi_ctrl_tb_obj/ \
#     -Wno-WIDTHTRUNC

# # Generate compile_commands.json for clangd LSP (stale build ok, just need commands)
# lsp-cpp: (clean-cpp)
#   rm -rf sa_wrapper_axi_ctrl_tb_obj/
#   bear -- verilator --cc sa_wrapper_axi_ctrl_sv.sv --trace-fst --build \
#     -CFLAGS -O0 -CFLAGS -fuse-ld=mold \
#     --verilate-jobs 16 --exe tb_main.cpp --Mdir sa_wrapper_axi_ctrl_tb_obj/ \
#     -Wno-WIDTHTRUNC
#   ln -sf ./sa_wrapper_axi_ctrl_tb_obj/compile_commands.json .

# clean-cpp:
#   rm -rf sa_wrapper_axi_ctrl_tb_obj/

wave tb="sa_tb":
  surfer ./{{tb}}_obj/waveform.fst --state-file {{tb}}_state

test-dw:
  verilator --cc dw3x3_axis_tb.sv --trace-fst --build -CFLAGS -O0 -CFLAGS -fuse-ld=mold -CFLAGS -std=c++20 --verilate-jobs 8 --timing --binary --Mdir dw3x3_axis_tb_obj/ -Wno-UNOPTFLAT -Wno-WIDTHTRUNC -Wno-INITIALDLY
  cd ./dw3x3_axis_tb_obj/ && ./Vdw3x3_axis_tb

wave-dw:
  surfer ./dw3x3_axis_tb_obj/waveform.fst --state-file dw3x3_axis_tb_state

# # Cocotb testbench
# cocotb-build:
#   cd tb/cocotb && uv run python3 run.py build

# cocotb-test filter="":
#   cd tb/cocotb && uv run python3 run.py test {{filter}}

# cocotb-setup:
#   cd tb/cocotb && COCOTB_IGNORE_PYTHON_REQUIRES=1 uv sync

upload num="1":
  ls -la /home/b83c/vivado/test_systolic/test_systolic.gen/sources_1/bd/design_{{num}}/hw_handoff/design_{{num}}.hwh
  ls -la /home/b83c/vivado/test_systolic/test_systolic.runs/impl_1/design_{{num}}_wrapper.bit
  scp /home/b83c/vivado/test_systolic/test_systolic.gen/sources_1/bd/design_{{num}}/hw_handoff/design_{{num}}.hwh pynq:~/jupyter_notebooks/test_systolic_vivado_manual/systolic.hwh
  scp /home/b83c/vivado/test_systolic/test_systolic.runs/impl_1/design_{{num}}_wrapper.bit pynq:~/jupyter_notebooks/test_systolic_vivado_manual/systolic.bit
  scp systolic.py pynq:~/jupyter_notebooks/test_systolic_vivado_manual/systolic.py
  
