set export := true

OBJCACHE := "ccache"
# CXX := "ccache g++"

test tb="sa_tb": 
  verilator --cc {{tb}}.sv --trace-fst --build -CFLAGS -O0 -CFLAGS -fuse-ld=mold --verilate-jobs 16 --threads 4 --hierarchical --timing --binary --Mdir {{tb}}_obj/ -Wno-WIDTHTRUNC -Wno-WIDTHEXPAND -Wno-ASCRANGE -Wno-SELRANGE -Wno-MULTIDRIVEN -Wno-IMPLICITSTATIC
  # verilator --cc {{tb}}.sv --trace-fst --build -CFLAGS -O0 -CFLAGS -fuse-ld=mold --verilate-jobs 16 --threads 4 --hierarchical --timing --binary --Mdir {{tb}}_obj/
  cd ./{{tb}}_obj/ && ./V{{tb}}

# SV AXI-control testbench
test-ctrl:
  verilator --cc sa_wrapper_axi_ctrl_tb.sv --trace-fst --build \
    -CFLAGS -O0 -CFLAGS -fuse-ld=mold \
    --verilate-jobs 16 --threads 4 --hierarchical --timing --binary \
    --Mdir sa_wrapper_axi_ctrl_tb_obj/ -Wno-WIDTHTRUNC -Wno-WIDTHEXPAND -Wno-UNOPTFLAT -Wno-IMPLICITSTATIC
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
  touch ./{{tb}}_state
  surfer ./{{tb}}_obj/waveform.fst --state-file ./{{tb}}_state

test-dw:
  verilator --cc dw3x3_axis_tb.sv --trace-fst --build -CFLAGS -O0 -CFLAGS -fuse-ld=mold -CFLAGS -std=c++20 --verilate-jobs 8 --timing --binary --Mdir dw3x3_axis_tb_obj/ -Wno-UNOPTFLAT -Wno-WIDTHTRUNC -Wno-INITIALDLY
  cd ./dw3x3_axis_tb_obj/ && ./Vdw3x3_axis_tb

wave-dw:
  surfer ./dw3x3_axis_tb_obj/waveform.fst --state-file dw3x3_axis_tb_state

test-1dconv:
  verilator --cc 1dconv_tb.sv --trace-fst --build -CFLAGS -O0 -CFLAGS -fuse-ld=mold -CFLAGS -std=c++20 --verilate-jobs 8 --timing --binary --Mdir 1dconv_tb_obj/ -Wno-UNOPTFLAT -Wno-WIDTHTRUNC -Wno-INITIALDLY -Wno-WIDTHEXPAND
  cd ./1dconv_tb_obj/ && ./V__031dconv_tb

test-maxpool:
  verilator --cc tb_maxpool_axis.sv --trace-fst --build -CFLAGS -O0 -CFLAGS -fuse-ld=mold --verilate-jobs 8 --timing --binary --Mdir tb_maxpool_axis_obj/ -Wno-WIDTHTRUNC -Wno-WIDTHEXPAND -Wno-UNOPTFLAT
  cd ./tb_maxpool_axis_obj/ && ./Vtb_maxpool_axis

test-q:
  verilator --cc tb_1dconv_quant.sv --trace-fst --build -CFLAGS -O0 -CFLAGS -fuse-ld=mold --verilate-jobs 8 --timing --binary --Mdir tb_1dconv_quant_obj/ -Wno-WIDTHTRUNC -Wno-WIDTHEXPAND
  cd ./tb_1dconv_quant_obj/ && ./Vtb_1dconv_quant

test-8ch:
  verilator --cc tb_conv1d_8ch.sv --trace-fst --build -CFLAGS -O0 -CFLAGS -fuse-ld=mold --verilate-jobs 8 --timing --binary --Mdir tb_conv1d_8ch_obj/ -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC
  cd ./tb_conv1d_8ch_obj/ && ./Vtb_conv1d_8ch

test-8x8ch:
  verilator --cc tb_conv1d_8x8ch.sv --trace-fst --build -CFLAGS -O0 -CFLAGS -fuse-ld=mold --verilate-jobs 8 --timing --binary --Mdir tb_conv1d_8x8ch_obj/ -Wno-WIDTHEXPAND -Wno-WIDTHTRUNC
  cd ./tb_conv1d_8x8ch_obj/ && ./Vtb_conv1d_8x8ch

# Cocotb testbench — same tests can run on sim (cocotb) or PYNQ (sa_pynq.py)
cocotb-setup:
  cd tb/cocotb && uv sync

cocotb-test filter="":
  echo "See tb/cocotb/run.py for Cocotb build instructions"
  echo "Tests in tb/cocotb/test_sa.py run with numpy reference models"
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
  
